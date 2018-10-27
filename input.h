#ifndef KORCHINPUT
#define KORCHINPUT

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>


class Object{
	int size;
	double * coords;
	double * colors;
	char charbuf[16];
	public:
	Object(const char * path){
		printf("Object() begin\n");
		FILE * in;
		in=fopen(path,"r");
		fscanf(in,"%d",&size);
		coords=new double[size*3];
		colors=new double[size*3];
		fscanf(in,"%s",charbuf);

		for (int i=0;i<size*3;i++) fscanf(in,"%lf",&coords[i]);
		fscanf(in,"%s",charbuf);
		for (int i=0;i<size*3;i++) fscanf(in,"%lf",&colors[i]);
		fclose(in);
		printf("Object() end\n");
	}
	~Object(){
		delete coords;
		delete colors;
	}
	double * getColors(){
		printf("Object.getcolors()\n");
		return colors;
	}
	double * getCoords(){
		printf("Object.getcoords()\n");
		return coords;
	}
	int getSize(){
		return size;
	}
};
#endif
