#include "model.h"
#include "restart.c"
#include "hashtab.h"
#define FIFOARG 1
#define FIFO_PERMS (S_IRWXU | S_IWGRP| S_IWOTH)
#define BUFFER_SIZE PIPE_BUF

int main (int argc, char *argv[]) {
	int pipe_fd;
	int res;
	int ret;
	char buffer[PIPE_BUF];
	inithashtab();
	/* create a named pipe to handle incoming requests */
	if (access("./data", F_OK) == -1) {
                ret = mkfifo("./data", 0777);
                if(ret != 0){
                        printf("mkfifo error");
                exit(-1);
                }
        }
	/* open a read/write communication endpoint to the pipe */
	if ((pipe_fd = open("./data", O_RDONLY)) == -1) {
        	perror("Server failed to open its FIFO");
        	return 1;
	}
	
	do{	
		memset(buffer, '\0', PIPE_BUF);
		res = read(pipe_fd, buffer, PIPE_BUF);
		//printf("++++++ %s\n", buffer);
		if(res > 0){
			put(buffer);
		}
	}while(res > 0);
	(void)close(pipe_fd);
	displaytable();	
	return 0;
}

