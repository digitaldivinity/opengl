#ifndef KORCHCAMERA
#define KORCHCAMERA

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
	}
	void  setDirection(){
		double phi=3.14*oxz/180;
		double psi=3.14*oyz/180;
		dz=cos(phi)*cos(psi);
		dx=sin(phi)*cos(psi);
		dy=sin(phi);
		double buf=sqrt(pow(dx,2)+pow(dy,2)+pow(dz,2));
		dx/=buf;
		dy/=buf;
		dz/=buf;
	}
	void goForward(){
		setDirection();
		x+=dx;
		y+=dy;
		z+=dz;
	}
	void goBack(){
		setDirection();
		x-=dx;
		y-=dy;
		z-=dz;
	}
	void goRight(){
		setDirection();
		vectmul(0,0,1);
		x+=dx;
		y+=dy;
		z+=dz;
	}
	void goLeft(){
		setDirection();
		vectmul(0,0,-1);
		x+=dx;
		y+=dy;
		z+=dz;
	}
	void toDefault(){
		x=0;y=-100;z=-100;
		dx=dy=dz=oxz=oyz=0;
		oyz=-85;
	}
	void vectmul(double x,double y,double z){
		double a,b,c;
		a=dy*z-dz*y;
		b=dz*x-dx*z;
		c=dx*y-dy*x;
		dx=a;
		dy=b;
		dz=c;
	}
};

#endif
