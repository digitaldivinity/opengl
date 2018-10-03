#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

GLint Width=512,  Height=512;
const int CubeSize = 200;


void Display(void){
	int left,right,top,bottom;
	left=(Width-CubeSize)/2;
	right = left+ CubeSize;
	bottom=(Height - CubeSize)/2;
	top=bottom + CubeSize;
	glClearColor(0.7,0.7,0.7,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLE_STRIP);
	glColor3ub(255,0,0);
	glVertex2f(left,bottom);
	glColor3ub(0,255,0);
	glVertex2f(left,top);
	glColor3ub(0,0,255);
	glVertex2f(right,top);
	
	//glColor3ub(0,0,0);
	//glVertex2f(right,bottom);
	glEnd();
	glFinish();
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
  //Call to the drawing function
  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Keyboard);
  glutMainLoop();
  return 0;
  //lllll
}
