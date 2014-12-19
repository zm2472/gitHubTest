#include "model.h"
#include "restart.c"
#define FIFOARG 1
#define FIFO_PERMS (S_IRWXU | S_IWGRP| S_IWOTH)
int main (int argc, char *argv[]) {
	int requestfd;
	if (argc != 2) { /* name of server fifo is passed on the command line */
        	 fprintf(stderr, "Usage: %s fifoname > logfile\n", argv[0]);
        	 return 1;
	}
	/* create a named pipe to handle incoming requests */
	if ((mkfifo(argv[FIFOARG], FIFO_PERMS) == -1) && (errno != EEXIST)) {
		perror("Server failed to create a FIFO");
       		return 1;
	}
	/* open a read/write communication endpoint to the pipe */
	if ((requestfd = open(argv[FIFOARG], O_RDONLY)) == -1) {
        	perror("Server failed to open its FIFO");
        	return 1;
	}
//	copyfile(requestfd, STDOUT_FILENO);
	return 0;
}

