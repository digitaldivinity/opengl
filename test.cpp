#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include "input.h"

GLint Width=512,  Height=512;
const int CubeSize = 200;
Object obj("coco");

void Display(void){
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES,0, obj.getSize());
	glFlush();
}

void Reshape(GLint w,GLint h){
	Width=w;
	Height=h;
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,w,0,h,-1.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Keyboard(unsigned char key, int x, int y){
	const char ESCAPE = '\033';
	if (key== ESCAPE) exit(0);
}

//Main program
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	//Simple buffer
	//glEnable(GL_NORMALIZE) автоматическая нормализация нормалей
	glShadeModel(GL_SMOOTH);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB ); 
	glutInitWindowPosition(50,25);
	glutInitWindowSize(Width,Height);
	glutCreateWindow("Green window");
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_DOUBLE,0,obj.getCoords());
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3,GL_DOUBLE,0,obj.getColors());
	
	//glDrawArrays(GL_TRIANGLES,0,1);
	//Call to the drawing function
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
	return 0;
	//lllll
}
