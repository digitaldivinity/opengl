#ifndef KORCHCAMERA
#define KORCHCAMERA
struct Camera{
	double x,y,z;
	double oxz,oyz;
	double dx,dy,dz;
	double speed;
	double sensitivity;
	double gf,gr,gl,gb;
	Camera(){
		x=y=oxz=oyz=z=0;
		sensitivity=0.3;
	}
	Camera(double sensitivity,double speed){
		x=y=oxz=oyz=z=0;
		this->speed=speed;
		this->sensitivity=sensitivity;
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
		dy=-sin(psi);
		double buf=sqrt(pow(dx,2)+pow(dy,2)+pow(dz,2))/speed;
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
		vectmul(0,1,0);
		x+=dx;
		y+=dy;
		z+=dz;
	}
	void goLeft(){
		setDirection();
		vectmul(0,-1,0);
		x+=dx;
		y+=dy;
		z+=dz;
	}
	void toDefault(){
		x=0;y=-100;z=-100;
		dx=dy=dz=oxz=oyz=0;
		oyz=-60;
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
	void ifgo(){
		if (gf>0){
			gf--;
			goForward();
		}
		if (gb>0){
			gb--;
			goBack();
		}
		if (gl>0){
			gl--;
			goLeft();
		}
		if (gr>0){
			gr--;
			goRight();
		}
	}
};

#endif
