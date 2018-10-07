#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include "input.h"
GLint ypos=0;
GLint Width=512,  Height=512;
const int CubeSize = 200;
GLdouble sx=0,sy=0,sz=0;
const double SPEED = 0.1;

struct Camera{
	double x,y,z;
	double dx,dy,dz;
	double speed;
	Camera(){
		x=y=z=dx=dy=0;
		dz=-1;
		speed=0.1;
	}
	//normalize direction to speed
	void ndts(){
		double module=sqrt(pow(dx,2)+pow(dy,2)+pow(dz,2));
		dx=dx*speed/module;
		dy=dy*speed/module;
		dz=dz*speed/module;
	}
	void goForward(){
		ndts();
		x+=dx;
		y+=dy;
		z+=dz;
	}
    void newdir(double x,double y,double z){
		dx=x;
		dy=y;
		dz=z;
	}

}

int startx,starty;
int deltax,deltay;

Object obj("coco");

void Display(void){
	glViewport(0,0,Width,Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-3,3,-3,3,1,10);
	gluPerspective(45,1,0.1,500);
	glRotated(deltax,sy,sz,sx);
	glRotated(deltay,sz,sx,sy);
	glTranslated(sx,sy,sz);


	//glRotated(deltay,sx,sy,sz);

	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glBegin(GL_LINES);
		glLineWidth(10);
		glColor3d(1,0,0);
		glVertex3d(0,0,0);
		glVertex3d(100,0,0);
		glColor3d(0,1,0);
		glVertex3d(0,0,0);
		glVertex3d(0,100,0);
		glColor3d(0,0,1);
		glVertex3d(0,0,0);
		glVertex3d(0,0,100);
	glEnd();
	glDrawArrays(GL_TRIANGLES,0, obj.getSize());

	glFlush();
	printf("Display()\n");
}

void Reshape(GLint w,GLint h){
	Width=w;
	Height=h;
	glViewport(0,0,w,h);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

	//glOrtho(0,w,0,h,-200,200);
	//ВТОРОЙ ПАРАМЕТР ЧТО-ТО ТИПО РАСТЯЖЕНИЯ
	//glFrustum(-1,1,-1,1	,1,300);
	//gluPerspective(45,1,1,500);

	//glTranslated(sx,sy,sz);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//Display();
	printf("Reshape()\n");
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y){
	const char ESCAPE = '\033';
	const char w='w';
	if (key== ESCAPE) exit(0);
	if (key=='w') {
		sz+=SPEED;
		printf("w\n");
	}
	if (key=='s'){
		sz-=SPEED;
		printf("s\n");
	}
	if (key=='d'){
		sx+=SPEED;
		printf("d\n");
	}
	if (key=='a'){
		sx-=SPEED;
		printf("a\n");
	}
	if (key=='q'){
		sy+=SPEED;
		printf("d\n");
	}
	if (key=='e'){
		sy-=SPEED;
		printf("a\n");
	}
	printf("Keyboard\n");
	glutPostRedisplay();
}

void MouseButton(int button, int state, int x, int y) {

	// только при начале движения, если нажата левая кнопка
	printf("%d %d\n",x,y);
	if (button == GLUT_LEFT_BUTTON) {

		// когда кнопка отпущена
		if (state == GLUT_UP) {
			printf("MouseButton() left off %d %d\n",x,y);

		}
		else  {// state = GLUT_DOWN
			printf("MouseButton() left on %d %d\n",x,y);
			startx=x;
			starty=y;
		}
	}
}

void MouseMove(int x, int y) {
	deltax=startx-x;
	deltay=starty-y;
	printf("MouseMove()\nsxy %d %d\nxy %d %d\ndxy %d %d\n",startx,starty,x,y,deltax,deltay);
	glutPostRedisplay();

}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glShadeModel(GL_SMOOTH);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
	glutInitWindowPosition(400,400);
	glutInitWindowSize(Width,Height);
	glutCreateWindow("Window name");

	//включение и загрузка вершинных и цветовых массивов
	glEnable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3,GL_DOUBLE,0,obj.getCoords());
	glColorPointer(3,GL_DOUBLE,0,obj.getColors());

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMove);
	glutMainLoop();
	return 0;
	//lllll
}
