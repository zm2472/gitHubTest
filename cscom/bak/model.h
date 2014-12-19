#ifndef SERVER_HEADS_H_INCLUDED
#define SERVER_HEADS_H_INCLUDED

//#include <string>
//#include <iostream>
//#include <nftw.h>
#include <ftw.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <time.h>


#define BLOCKSIZE 256
#define MAXSIZE 256
//#define PARSERCOUNT 5

#endif

void recordlog(const char* log,const char* path)
{
	time_t rawtime;
	struct tm *timeinfo;
	FILE *fp;
	fp=fopen(path,"at+");
	time(&rawtime);
	timeinfo=localtime(&rawtime);
	if(!fp)
		return;
	fprintf(fp,"-%s- : %s\n",asctime(timeinfo),log);
	fclose(fp);
}

char path[] = "/tmp/mbsh.log";
