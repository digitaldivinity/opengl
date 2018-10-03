#ifndef korchinput
#define korchinput

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>


class object{
	int size;
	int * arr;
	public:
	object(const char * path){
		FILE * in;
		in=fopen(path,"r");
		fscanf(in,"%d",&size);
		size=size*3;
		arr=new int[size];
		for (int i=0;i<size;i++) fscanf(in,"%d",&arr[i]);
		fclose(in);
	}
	~object(){
		delete arr;
	}
	int * get(){
		for (int i=0;i<size;i++) printf("%d  ",arr[i]);
		return arr;
	}
};
#endif
