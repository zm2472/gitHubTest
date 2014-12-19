#define _XOPEN_SOURCE 500
#include "model.h"
#include<ftw.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<limits.h>
#define FILEOPEN 1024
#define FIFO_NAME "./data"
#define BUFFER_SIZE PIPE_BUF
    
int getMetadata(const char *dirpath, const struct stat *sb,int typeflag, struct FTW *ftwbuf); 

int main (int argc, char *argv[]) {
        struct stat pathbuf;
	int ret, i, j;
	pid_t pid;
	int open_mode = O_WRONLY;

	if(argc < 2){   
    		printf("please input e:./pipeclient ./proc... /n ");
    		return -1;
	}
	
	for(i = 1; i < argc; i++){
		//根据argv[i]中指定的目录
  		if(lstat(argv[i],&pathbuf)){   
    			printf("invalid dirpath:%s/n",argv[i]);   
    			return -1;   
  		}else{   
    			if(0 == S_ISDIR(pathbuf.st_mode)){   
      				printf("%s is not dirpath/n",argv[i]);   
      				return -1;   
    			}		   
  		}	   
	}
	
	if (access(FIFO_NAME, F_OK) == -1) {
		ret = mkfifo(FIFO_NAME, 0777);
		if(ret != 0){
			fprintf(stderr, "Could not create fifo %s\n", FIFO_NAME);
		exit(-1);
		}
	}	

//	printf("make fifo success");
	
	for(j = 1; j < argc; j++){
		if ((pid = fork()) < 0)
		{
			printf("fork error");
			return -1;
		}
		else if (pid == 0)
		{printf("sdasd");
			ret = nftw(argv[j],getMetadata,FILEOPEN,FTW_PHYS);
   
    			if(ret<0){   
    				printf("wrong ntfw search files/n");   
    				return -1;   
  			}else{   
    				printf("The nftw process is success!");   
    				break;   
  			}
		}	   
	
 	}
	return  0;
/*	if (write(requestfd, requestbuf, len) != len) {
       		perror("Client failed to write");
      		return 1;
	}
	close(requestfd);
*/
}

int getMetadata(const char *dirpath, const struct stat *sb,int typeflag, struct FTW *ftwbuf){   
	 printf("in the dirpath");
        if(S_ISREG((*sb).st_mode)){
        printf("%s",dirpath);

        char requestbuf[PIPE_BUF];
        FILE *fp;
        char ch;
        int flag = 0;
        int i = 0;

        memset(requestbuf, 0, PIPE_BUF);

        if((fp = fopen(dirpath, "r")) == NULL){
                printf("error!");
        }
        int pipe_fd;
        pipe_fd = open(FIFO_NAME, O_WRONLY);
        while((ch=fgetc(fp))!=EOF){
                if(ch == ' '&&flag == 0){
                        continue;
                }else if(ch != ' ' && ch != '\n'){
			flag = 1;
                        requestbuf[i] = ch;
                        i++;
                }else if(ch == ' ' && flag == 1){
                        requestbuf[i] = '\0';
                        write(pipe_fd, requestbuf, PIPE_BUF);
                        memset(requestbuf, 0, PIPE_BUF);
                        flag = 0;
                }else if(ch != ' ' && ch == '\n'){
                        requestbuf[i] = '\0';
                        write(pipe_fd, requestbuf, PIPE_BUF);
                        memset(requestbuf, 0, PIPE_BUF);
                        flag = 0;
                }
        }

        write(pipe_fd, requestbuf, PIPE_BUF);
        fclose(fp);
        (void)close(pipe_fd);
        }
        return 0;
} 
