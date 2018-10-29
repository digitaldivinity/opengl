#ifndef BMPKORCHPARSER
#define BMPKORCHPARSER

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


unsigned char * bmpparser(const char * name){
	unsigned char * array=new unsigned char[64*64];
	try{
		
		char buf[64];
		int shift;
		for (int i=0;i<64;i++) buf[i]='\0';
		int fd=open(name,O_RDONLY);
		if (fd<0) throw "ex";
		read(fd,buf,10);
		for (int i=0;i<64;i++) buf[i]='\0';
		read(fd,&shift,4);
		printf("%d \n",shift);
		read(fd,buf,shift-14);
		
		int i=0;
		while (read(fd,&array[i],1)&& i<64*64) {printf("%d i = %d\n",array[i],i); i++;}
		
		close(fd);
	}
	catch(...){printf("Exception\n");}
	return array;
}
#endif
