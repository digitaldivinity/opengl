#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include "input.h"
#include "camera.h"
#include <omp.h>


/*
 * полет вперед и по сторонам
 * плавный полет
 * сделать чтобы при изменении размера не колбасило +
 * сделать измерение времени между вызовами таймера
 * добавить двойную буферизацию
 */

Camera cam;
double FPS;
int flag=0; //это нужно подгонять под фпс
const double SPEED = 0.1;
double tEnd,tStart;
int Angle=1;
GLint Width=1024,  Height=1024;
//констрастность
float mat_dif[]={0.0f,0.4f,0.4f};
//яркость
float mat_amb[]= { 0.2f , 0.2f , 0.2f } ;
//глянцевость
float mat_spec[] = { 0.6f ,0.6f , 0.6f } ;
float mat_shininess=0.1f*128;

float mat_dif1[]={0.9f,0.2f,0.2f};
float mat_dif2[]={0.2f,0.9f,0.2f};
//яркость
float mat_amb1[]= { 0.4f , 0.4f , 0.4f } ;
//глянцевость
float mat_spec1[] = { 0.4f ,0.4f , 0.4f } ;
float mat_shininess1=0.9f*128;

void init(){
	GLfloat light_ambient[]={0.3,0.3,0.3,1};
	GLfloat light_diffuse[]={1,1,1,1};
	GLfloat light_specular[]={1,1,1,1};
	GLfloat light_position[]={1,1,1,0};
	
	glLightfv(GL_LIGHT0,GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
	glLightfv(GL_LIGHT0,GL_POSITION, light_position);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

double bublicAngle=0;
double qubeAngle=0;

void timf(int value){
	tEnd=omp_get_wtime();
	FPS=tEnd-tStart;
	printf("%lf\n",1/FPS);
	bublicAngle+=Angle;
	qubeAngle+=Angle;
	if (flag>0) {
		//здесь определяется расстояние
		cam.z+=8*FPS;
		flag--;
	}
	glutPostRedisplay();
	glutTimerFunc(20,timf,0);
	tStart=omp_get_wtime();
}




int startx,starty;
int deltax,deltay;

Object obj("coco");

int stars[900];

void Display(void){
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	
	
	glMatrixMode(GL_MODELVIEW);
	
	
	
	//qube
	glRotatef(qubeAngle,0,1,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDrawArrays(GL_TRIANGLES,0, obj.getSize());
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_amb);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_dif1);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_spec);
	glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);

	//Sphere 1 in center
	//glRotatef(qubeAngle,1,1,0);
	//glRotatef(bublicAngle,1,0,0);
	glutSolidSphere(3.0,15,15);
	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_dif);
	
	//Sphere 2
	//glRotatef(bublicAngle,1,0,0);
	
	glTranslated(40,0,0);
	//glRotatef(qubeAngle,0,1,0);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_dif2);
	
	//glPushMatrix();
	glutSolidSphere(2.0,30,30);
	
	//Torus
	//glPopMatrix();
	//glRotatef(bublicAngle,1,0,0);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_dif1);
	glRotatef(qubeAngle,0,1,0);
	glTranslated(10,0,0);
	
	
	glutSolidTorus(0.275,0.85,30,30);
	
	/*
	
	glPushMatrix();
	//glPushMatrix();
	//glRotatef(qubeAngle,1,1,0);
	
	//glPushMatrix();
	glTranslatef(0,0.0,30);
	glRotatef(bublicAngle,1,0,0);
	glutSolidSphere(1.0,15,15);
	glPopMatrix();
	
	
	glPushMatrix();
	glTranslated(0,2,0);
	glRotated(qubeAngle,1,0,0);
	
	glPushMatrix();
	glTranslatef(0,0,30);
	glRotatef(bublicAngle,1,0,0);
	glutSolidTorus(0.275,0.85,30,30);
	
	
	
	glPopMatrix();
	glPopMatrix();
	*/
	
	//повороты для камеры
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(cam.oyz,-1,0,0);
	glRotated(cam.oxz,0,-1,0);
	glTranslated(cam.x,cam.y,cam.z);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_DEPTH_TEST);
	glBegin(GL_POINTS);
	glColor3f(0.5,0.5,0.5);
	glPointSize(5);
	for (int i=0;i<900;i+=3)
		glVertex3i(stars[i],stars[i+1],stars[i+2]);
	glEnd();
	
	glFlush();
}

void Reshape(GLint w,GLint h){
	Width=w;
	Height=h;
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45,(GLfloat)w/h,1,500);
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y){
	
	switch (key){
		case 'w':
			//здесь определяется скорость
			flag=1/(FPS*10);
			//cam.z+=SPEED;
			break;
		case 's':
			cam.z-=SPEED;
			break;
		case 'a':
			cam.x+=SPEED;
			break;
		case 'd':
			cam.x-=SPEED;
			break;
		case 'i':
			cam.goForward();
			break;
		case 'k':
			cam.goBack();
			break;
		case 'j':
			cam.goLeft();
			break;
		case 'l':
			cam.goRight();
			break;
		case 'z':
			cam.toDefault();
			break;
		case '\033':
			exit(0);
		case '1':
			Angle++;
			break;
		case '2':
			Angle--;
			break;
		case '3':
			Angle=0;
			break;
			
	}
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
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB |GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowPosition(200,200);
	glutInitWindowSize(Width,Height);
	glutCreateWindow("Space shit");
	init();
	//включение и загрузка вершинных и цветовых массивов
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_NORMALIZE);
	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_FALSE);
	//glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,64);
	//glMateriali(GL_FRONT_AND_BACK,GL_AMBIENT,0.5);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3,GL_DOUBLE,0,obj.getCoords());
	glColorPointer(3,GL_DOUBLE,0,obj.getColors());
	
	for (int i=0;i<900;i++)
	stars[i]=rand()%300-150;
	
	
	glutTimerFunc(20,timf,0);
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMove);
	tStart=omp_get_wtime();
	glutMainLoop();
	return 0;
}
