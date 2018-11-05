#ifndef BMPKORCHPARSER
#define BMPKORCHPARSER

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

class korch_texture{
	unsigned char * array;
	unsigned int height,width;
	public:
	korch_texture(const char * path){
		int shift;
		char buf[64];
		int fd=open(path,O_RDONLY);
		if (fd<0) throw "Cannot open file";
		read(fd,buf,10); // не нужно
		read(fd,&shift,4);
		printf("Pixel starts at   %d\n",shift);
		read(fd,&shift,4);
		printf("BITMAPINFO SIZE = %d\n",shift);
		read(fd,&width,4);
		printf("Width           = %d\n",width);
		read(fd,&height,4);
		printf("Height          = %d\n",height);
		read(fd,buf,2);
		read(fd,&shift,2);
		printf("BPPS %d\n",shift);
		close(fd);
		fd=open(path,O_RDONLY);
		read(fd,buf,52);
		array=new unsigned char[3*width*height];
		int i=0;
		while (read(fd,&array[i],1)&& i<width*height*3)  {
			if (i%3==0 && i!=0) swap(i-2,i);
			i++;
		}
		close(fd);
		
	}
	~korch_texture(){
		delete [] array;
	}
	unsigned char * get(){
		return array;
	}
	unsigned int getWidth(){
		return width;
	}
	unsigned int getHeight(){
		return height;
	}
	void swap(int i,int j){
		unsigned char buf=array[i];
		array[i]=array[j];
		array[j]=buf;
	}
};
#endif
