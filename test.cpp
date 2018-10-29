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
 * плавный полет +-
 * регулировка относительно фпс +- если падает фпс то падает и скорость камеры
 * сделать чтобы при изменении размера не колбасило +
 * сделать измерение времени между вызовами таймера +
 * добавить двойную буферизацию +
 * отключил редислпей в обработчике клавитуры +
 * для мышки недостаточно таймера +
 * перенести все измерение времени в дисплей, баг появляется
 * редисплей от мышки не учитывается в фпс
 * 
 */

Camera cam(0.3,0.5);
double FPS;
double fps=0;
int fpsn=0;
int flag=0; //это нужно подгонять под фпс
const double SPEED = 0.1;
double tEnd,tStart;
int Angle=1;
GLint Width=1024,  Height=1024;

unsigned char * array=bmpparser("tiles.bmp");
unsigned char * array1 = new unsigned char[64*64];
GLUquadricObj * smthg = gluNewQuadric();
GLUquadricObj * earth = gluNewQuadric();
float mat_dif[]={0.0f,0.4f,0.4f};
float mat_amb[]= { 0.2f , 0.2f , 0.2f } ;
float mat_spec[] = { 0.6f ,0.6f , 0.6f } ;
float mat_shininess=0.1f*128;
float mat_dif1[]={0.9f,0.2f,0.2f};
float mat_dif2[]={0.2f,0.9f,0.2f};
GLuint TEXID[2];
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
	fpsn++;
	tEnd=omp_get_wtime();
	FPS=tEnd-tStart;
	fps+=FPS;
	if (fps>1) {
		printf("FPS = %d\n",fpsn);
		fps=0;
		fpsn=0;
	}
		
	FPS=1/FPS;
	bublicAngle+=Angle;
	qubeAngle+=Angle;
	
	cam.ifgo();
	
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
	//glutSolidSphere(3.0,15,15);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0,0);
	glVertex3f(0,0,0);
	glTexCoord2f(0,1);
	glVertex3f(1,0,0);
	glTexCoord2f(1,1);
	glVertex3f(1,0,1);
	glTexCoord2f(1,0);
	glVertex3f(0,0,1);
	glEnd();
	gluSphere(smthg,3,30,30);
	glDisable(GL_TEXTURE_2D);
	//Sphere 2
	//glEnable(GL_TEXTURE_2D);
	glTranslated(40,0,0);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_dif2);
	//gluSphere(smthg,2,30,30);
	glutSolidSphere(2.0,30,30);
	//glDisable(GL_TEXTURE_2D);
	//Torus
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_dif);
	glRotatef(qubeAngle*2,0,1,0);
	glTranslated(10,0,0);
	glutSolidTorus(0.275,0.85,30,30);
	
	
	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();
	glBegin(GL_POINTS);
	glColor3f(1,0,0);
	glPointSize(5);
	glVertex3i(0,0,-2);
	glEnd();
	
	//повороты для камеры
	//glLoadIdentity();
	glRotated(cam.oyz,-1,0,0);
	glRotated(cam.oxz,0,-1,0);
	glTranslated(cam.x,cam.y,cam.z);
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glBegin(GL_POINTS);
	glColor3f(0.5,0.5,0.5);
	glPointSize(5);
	for (int i=0;i<900;i+=3)
		glVertex3i(stars[i],stars[i+1],stars[i+2]);
	glEnd();
	
	
	
	
	glFlush();
	glutSwapBuffers();
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
			cam.gf=FPS/4;
			//cam.goForward();
			break;
		case 's':
			cam.gb=FPS/4;
			//cam.goBack();
			break;
		case 'a':
			cam.gl=FPS/4;
			//cam.goLeft();
			break;
		case 'd':
			cam.gr=FPS/4;
			//cam.goRight();
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
	//glutPostRedisplay();
}

void MouseButton(int button, int state, int x, int y) {

	// только при начале движения, если нажата левая кнопка
	if (button == GLUT_LEFT_BUTTON) {

		// когда кнопка отпущена
		if (state == GLUT_UP) {
			//printf("MouseButton() left off %d %d\n",x,y);

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

	//glShadeModel(GL_SMOOTH);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowPosition(200,200);
	glutInitWindowSize(Width,Height);
	glutCreateWindow("Space shit");
	init();
	
	glEnable(GL_DEPTH_TEST);
	//включение и загрузка вершинных и цветовых массивов
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3,GL_DOUBLE,0,obj.getCoords());
	glColorPointer(3,GL_DOUBLE,0,obj.getColors());
	
	for (int i=0;i<900;i++)
	stars[i]=rand()%300-150;
	
	for (int i=0;i<64*64;i+=6) {array[i]=255; array[i+1]=0; array[i+2]=0;
		array[i+3]=0; array[i+4]=255; array[i+5]=0;}
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1 );
	glGenTextures(1,TEXID);
	glBindTexture(GL_TEXTURE_2D,TEXID[0]);
	gluBuild2DMipmaps(GL_TEXTURE_2D,3,32,32,GL_RGB,GL_UNSIGNED_BYTE,array);
	gluQuadricTexture(smthg,GL_TRUE);
	
	glTexParameterf (GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,
	( float )GL_NEAREST) ;
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
	( float )GL_NEAREST) ;
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
	( float )GL_REPEAT) ;
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
	( float )GL_REPEAT) ;
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
	( float )GL_MODULATE) ;
	
	
	
	delete [] array;
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
