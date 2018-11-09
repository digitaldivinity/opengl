#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include "input.h"
#include "camera.h"
#include <omp.h>
#include "bmpparser.h"
/*
 * полет вперед и по сторонам +
 * плавный полет +
 * регулировка относительно фпс + если падает фпс то падает и скорость камеры +
 * сделать чтобы при изменении размера не колбасило +
 * сделать измерение времени между вызовами таймера +
 * добавить двойную буферизацию +
 * отключил редислпей в обработчике клавитуры +
 * для мышки недостаточно таймера +
 * перенести все измерение времени в дисплей, баг появляется +
 * редисплей от мышки не учитывается в фпс +
 * иногда начинает проседать, хотя фпс показывает нормальный -
 */

Camera cam(0.3,0.9);
double FPS=60;
double fps=0;
int fpsn=0;
int flag=0; //это нужно подгонять под фпс
const double SPEED = 0.1;
double tEnd,tStart;
int Angle=1;
GLint Width=1024,  Height=1024;

unsigned char * array=new unsigned char[64*64*3];
GLUquadricObj * smthg = gluNewQuadric();
GLUquadricObj * earth = gluNewQuadric();
float mat_dif[]={0.9f,0.9f,0.9f};
float mat_amb[]= { 0.2f , 0.2f , 0.2f } ;
float mat_spec[] = { 0.6f ,0.6f , 0.6f } ;
float mat_shininess=0.1f*128;
float mat_dif_red[]={0.7f,0.3f,0.3f};
float mat_dif_blue[]={0.1f,0.1f,0.3f};
GLuint TEXID[2];


void init(){
	//light params
	GLfloat light_ambient[]={0.3,0.3,0.3,1};
	GLfloat light_diffuse[]={1,1,1,1};
	GLfloat light_specular[]={1,1,1,1};
	GLfloat light_position[]={0,0,0,1};
	
	glLightfv(GL_LIGHT0,GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
	glLightfv(GL_LIGHT0,GL_POSITION, light_position);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	
	//material params
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_amb);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_dif);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_spec);
	glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
	
	//texture params
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1 );
	glGenTextures(2,TEXID);
	korch_texture tex2("earth.bmp");
	glBindTexture(GL_TEXTURE_2D,TEXID[0]);
	gluBuild2DMipmaps(GL_TEXTURE_2D,3,tex2.getWidth(),tex2.getHeight(),GL_RGB,GL_UNSIGNED_BYTE,tex2.get());
	gluQuadricTexture(smthg,GL_TRUE);
	glTexParameterf (GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,( float )GL_NEAREST) ;
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,( float )GL_NEAREST) ;
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,( float )GL_REPEAT);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,( float )GL_REPEAT);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,( float )GL_MODULATE);
	
	korch_texture tex1("melon.bmp");
	glBindTexture(GL_TEXTURE_2D,TEXID[1]);
	gluBuild2DMipmaps(GL_TEXTURE_2D,3,tex1.getWidth(),tex1.getHeight(),GL_RGB,GL_UNSIGNED_BYTE,tex1.get());
	gluQuadricTexture(earth,GL_TRUE);
	glTexParameterf (GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,( float )GL_NEAREST) ;
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,( float )GL_NEAREST) ;
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,( float )GL_REPEAT) ;
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,( float )GL_REPEAT) ;
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,( float )GL_MODULATE) ;
	//glEnable(GL_TEXTURE_2D);
}

double bublicAngle=0;
double qubeAngle=0;

void timf(int value){
	bublicAngle+=Angle;
	qubeAngle+=Angle;
	if (qubeAngle>360) qubeAngle-=360;
	if (bublicAngle>360) bublicAngle-=360;
	cam.move();
	
	glutPostRedisplay();
	glutTimerFunc(20,timf,0);
}

int startx,starty;
int deltax,deltay;

Object obj("coco");

double stars[900];

void Display(void){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,1);
	
	//повороты и смещение для камеры
	glRotated(cam.oyz,-1,0,0);
	glRotated(cam.oxz,0,-1,0);
	glTranslated(cam.x,cam.y,cam.z);
	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_dif_red);
	//central sphere
	glPushMatrix();
	glutSolidSphere(3,30,30);
	glPopMatrix();
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_dif);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,TEXID[0]);
	//first sphere
	glPushMatrix();
	glRotated(qubeAngle,0,1,0);
	glTranslated(20,0,0);
	gluSphere(earth,2,30,30);
	glPopMatrix();
	
	glBindTexture(GL_TEXTURE_2D,TEXID[1]);
	//second sphere
	glPushMatrix();
	glRotated(qubeAngle*2,0,1,0);
	glTranslated(40,0,0);
	gluSphere(smthg,3,30,30);
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_dif_blue);
	//third sphere
	glPushMatrix();
	glRotated(qubeAngle,0,-1,0);
	glTranslated(60,0,0);
	glutSolidSphere(3,30,30);
	//torus bound with sphere
	glRotated(qubeAngle,0,-2,0);
	glTranslated(10,0,0);
	glRotated(qubeAngle,1,1,1);
	glutSolidTorus(0.275,0.85,30,30);
	glPopMatrix();
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_dif);
	
	//cube
	glPushMatrix();
	glTranslated(30,0,0);
	glRotated(qubeAngle,0,1,0);
	glDisable(GL_LIGHTING);
	glDrawArrays(GL_TRIANGLES,0, obj.getSize());
	glEnable(GL_LIGHTING);
	//glutSolidCube(3);
	glPopMatrix();
	
	//stars
	glDisable(GL_LIGHTING);
	glBegin(GL_POINTS);
	glColor3f(0.5,0.5,0.5);
	glPointSize(5);
	for (int i=0;i<900;i+=3)
		glVertex3i(stars[i],stars[i+1],stars[i+2]);
	glEnd();
	glEnable(GL_LIGHTING);
	
	glFlush();
	glutSwapBuffers();
	
	//вычисление fps
	fpsn++; //количество кадров
	tEnd=omp_get_wtime();
	FPS=tEnd-tStart; 
	fps+=FPS;
	if (fps>1) {
		printf("FPS = %d\n",fpsn);
		fps=0;
		fpsn=0;
	}
	FPS=1/FPS;
	tStart=omp_get_wtime();	
}

void Reshape(GLint w,GLint h){
	Width=w;
	Height=h;
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45,(GLfloat)w/h,1,1000);
	glutPostRedisplay();
}

void KeyDown(unsigned char key, int x, int y){
	
	switch (key){
		case 'w':
			cam.gf=true;
			break;
		case 's':
			cam.gb=true;
			break;
		case 'a':
			cam.gl=true;
			break;
		case 'd':
			cam.gr=true;
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
}

void KeyUp(unsigned char key, int x, int y){
	switch (key){
		case 'w':
			cam.gf=false;
			break;
		case 's':
			cam.gb=false;
			break;
		case 'a':
			cam.gl=false;
			break;
		case 'd':
			cam.gr=false;
			break;
		}	
}


void MouseButton(int button, int state, int x, int y) {

	// только при начале движения, если нажата левая кнопка
	if (button == GLUT_LEFT_BUTTON) {

		// когда кнопка отпущена
		if (state == GLUT_UP) {

		}
		else  {// state = GLUT_DOWN
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
	glutPostRedisplay();

}
//в глют есть функции для расчета сплайнов



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	//интерполяция цветов
	//glShadeModel(GL_SMOOTH);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowPosition(200,200);
	glutInitWindowSize(Width,Height);
	glutCreateWindow("Space shit");
	init();
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glEnable(GL_DEPTH_TEST);
	//включение и загрузка вершинных и цветовых массивов
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3,GL_DOUBLE,0,obj.getCoords());
	glColorPointer(3,GL_DOUBLE,0,obj.getColors());
	
	{
		double a;
		for (int i=0;i<900;i+=3){
			stars[i]=rand()%500-250;
			stars[i+1]=rand()%500-250;
			stars[i+2]=rand()%500-250;
			a=sqrt(pow(stars[i],2)+pow(stars[i+1],2)+pow(stars[i+2],2));
			stars[i]=300*stars[i]/a;
			stars[i+1]=300*stars[i+1]/a;
			stars[i+2]=300*stars[i+2]/a;
		}
	}
	glutTimerFunc(20,timf,0);
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMove);
	tStart=omp_get_wtime();
	glutMainLoop();
	glutSetKeyRepeat(GLUT_KEY_REPEAT_DEFAULT);
	
	return 0;
}
