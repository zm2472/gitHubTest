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
#define OPEN_APPEND O_RDWR | O_CREAT | O_APPEND    
int getMetadata(const char *dirpath, const struct stat *sb,int typeflag, struct FTW *ftwbuf); 
int pipe_fd;
int main (int argc, char *argv[]) {
        struct stat pathbuf;
	int ret, i, j;
	int pid;
	
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

        //printf("make fifo success");

        pipe_fd = open("./data", O_WRONLY);

	
	for(j = 1; j < argc; j++){
                pid = fork();
                
		if (pid == 0)
                {	//printf("come in childprocess");
                        ret = nftw(argv[j],getMetadata,FILEOPEN,FTW_PHYS);

                        if(ret<0){
                                printf("wrong ntfw search files/n");
                                return -1;
                        }else{
                              //printf("The nftw process is success!");
                              break;
                        }
                }

        }
	
	for(j = 1; j < argc; j++){
		wait(NULL);
	}
	return 0;
}
int getMetadata(const char *dirpath, const struct stat *sb,int typeflag, struct FTW *ftwbuf){   
//	printf("in the dirpath");
	if(S_ISREG((*sb).st_mode)){
	printf("begin in the path: %s\n",dirpath);
	
	FILE *fp;
	char requestbuf[PIPE_BUF];
	char temp[PIPE_BUF];
	char ch;
	int flag = 0;
	int i = 0;
	int j = 0;
	int ret;
	memset(requestbuf, '\0', PIPE_BUF);
	memset(temp, '\0', PIPE_BUF);

	if((fp = fopen(dirpath, "r")) == NULL){
		printf("error!");
	}    
	
	while(fgets(requestbuf, PIPE_BUF, fp)){
		while(requestbuf[i] != '\0'){
			if(('A' <= requestbuf[i] && requestbuf[i] <= 'Z') || ('a' <= requestbuf[i] && requestbuf[i] <= 'z')){
				temp[j++] = requestbuf[i];
				if(flag == 0)
				flag = 1;
			}else{
				if(flag == 1){
					//printf("++++++++++word: %s\n", temp);
					ret = write(pipe_fd, temp, strlen(temp));
					memset(temp, '\0', PIPE_BUF);
					flag = 0;
				}
				j = 0;
			}
			i++;
		}
		if(('A' <= temp[i] && temp[i] <= 'Z') || ('a' <= temp[i] && temp[i] <= 'z')){
			//printf("__________word+++++++++: %s\n", temp);
			ret = write(pipe_fd, temp, strlen(temp));
		}
		i = 0;	
	}

	fclose(fp);
	}
	return 0;
} 

