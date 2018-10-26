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
	GLUquadricObj* m_qObj= gluNewQuadric();

struct Camera{
	double x,y,z;
	double oxz,oyz;
	double dx,dy,dz;
	double sensitivity;
	Camera(){
		x=y=oxz=oyz=z=0;
		dx=dy=0;
		dz=1;
		sensitivity=0.3;
	}
	void changeAngle(double phi,double psi){
		//нуждается в доработке, oxz может увеличиться слишком сильно
		oxz+=phi*sensitivity;
		oyz+=psi*sensitivity;
		if (oxz>=360) oxz-=360;
		else if (oxz<=0) oxz=360-oxz;
		if (oyz>85) oyz=85;
		else if (oyz<-85) oyz=-85;
		//printf("angle %lf %lf\n",oxz,oyz);
		//goForward();
	}
	void goForward(){
		double r1=3.14*oyz/180;
		double r2=3.14*oxz/180;
		x+=sin(r2)*sensitivity;
		z+=cos(r2)*sensitivity;
		//printf("moving %lf %lf %lf \n ",dx,dy,dz);
	}
	void recalculate(){
		
	}

//s
} cam;

int startx,starty;
int deltax,deltay;

Object obj("coco");

void Display(void){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45,1,1,500);
	
	glRotated(cam.oyz,-1,0,0);
	glRotated(cam.oxz,0,-1,0);
	
	
	glTranslated(cam.x,cam.y,cam.z);
	glMatrixMode(GL_MODELVIEW);
	
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES,0, obj.getSize());
	glColor4f(0.5,0.5,0.5,1);
	gluSphere(m_qObj, 2, 100, 100);

	glFlush();
	//printf("Display()\n");
}

void Reshape(GLint w,GLint h){
	Width=w;
	Height=h;
	glViewport(0,0,w,h);
	printf("Reshape()\n");
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y){
	const char ESCAPE = '\033';
	if (key== ESCAPE) exit(0);
	if (key=='x') {
		cam.goForward();
	}
	if (key=='w') {
		cam.z+=SPEED;
		//cam.goForward();
		//printf("w\n");
	}
	if (key=='s'){
		cam.z-=SPEED;
		//cam.goBack();
		printf("s\n");
	}
	if (key=='d'){
		cam.x-=SPEED;
		printf("d\n");
	}
	if (key=='a'){
		cam.x+=SPEED;
		printf("a\n");
	}
	if (key=='q'){
		cam.y+=SPEED;
		printf("d\n");
	}
	if (key=='e'){
		cam.y-=SPEED;
		printf("a\n");
	}
	if (key=='z'){
		cam.x=cam.y=cam.z=cam.oxz=cam.oyz=0;
	}
	if (key=='j') cam.changeAngle(3,0);
	if (key=='l') cam.changeAngle(-3,0);
	if (key=='i') cam.changeAngle(0,3);
	if (key=='k') cam.changeAngle(0,-3);
	printf("%lf %lf %lf\n",cam.x,cam.y,cam.z);
	//printf("Keyboard\n");
	glutPostRedisplay();
}

void MouseButton(int button, int state, int x, int y) {

	// только при начале движения, если нажата левая кнопка
	//printf("%d %d\n",x,y);
	if (button == GLUT_LEFT_BUTTON) {

		// когда кнопка отпущена
		if (state == GLUT_UP) {
			//printf("MouseButton() left off %d %d\n",x,y);

		}
		else  {// state = GLUT_DOWN
			//printf("MouseButton() left on %d %d\n",x,y);
			startx=x;
			starty=y;
		}
	}
}

void MouseMove(int x, int y) {
	deltax=startx-x;
	deltay=starty-y;
	startx=x;
	starty=y;
	cam.changeAngle(deltax,deltay);
	//printf("MouseMove()\nsxy %d %d\nxy %d %d\ndxy %d %d\n",startx,starty,x,y,deltax,deltay);
	glutPostRedisplay();

}
//в глют есть функции для расчета сплайнов



int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glShadeModel(GL_SMOOTH);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB |GLUT_DEPTH);
	glutInitWindowPosition(200,200);
	glutInitWindowSize(Width,Height);
	glutCreateWindow("Window name");

	//включение и загрузка вершинных и цветовых массивов
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_FALSE);
	glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,64);
	glMateriali(GL_FRONT_AND_BACK,GL_AMBIENT,0.5);
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
}
