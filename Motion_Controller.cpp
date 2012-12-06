#include "Arduino.h"
#include <Motion_Controller.h>

volatile unsigned long     Motion_Controller::path_t;
volatile unsigned long     Motion_Controller::t0;
volatile bool              Motion_Controller::end_path_option;
volatile int               Motion_Controller::path_size;
volatile int               Motion_Controller::i;
Wheels_Controller          Motion_Controller::wheels;
struct pt                  Motion_Controller::ptPath;
struct pt                  Motion_Controller::ptSpeed;
volatile float             Motion_Controller::theta_p[2];
volatile float             Motion_Controller::dist_p[2];
Consts                     Motion_Controller::consts;

Motion_Controller::Motion_Controller(){
	PT_INIT(&ptPath);
	PT_INIT(&ptSpeed);
	path_size=10;
	path_activate=false;
	speed_control_activated=true;
	for(int i=0;i<path_size;i++){
		theta_p[i]=0;
		dist_p[i]=0;
	}
	
}

//void Motion_Controller::accelerate(t,ff){
	/*h=round(t/dt-1);
	df=(ff-fm)/h;
	accelerate_activate=true   //fire event
	dt0=millis();
	*/
//}

//void Motion_Controller::schedule_acceleration(){
	/*
	while(accelerate_activate){
		wait_until(millis()-dt0>=dt);
		set fm()=fm+df;
		if(h--==0){
			accelerate_activate=false;
		}
		dt0=millis();
	}
	*/
//}

void Motion_Controller::schedule_speed_control(){
	if(speed_control_activated){
		speed_control_thread(&ptSpeed);
	}
}

int Motion_Controller::speed_control_thread(struct pt* ptt){
	PT_BEGIN(ptt);
	while(1){
		//wait until measurement is ready
		PT_WAIT_UNTIL(ptt,measurements.PERIOD_FLAG[0] && measurements.PERIOD_FLAG[1]);
		//get measured values
		static unsigned long Tu1=measurements.CURRENT_PERIOD[0];
		static unsigned long Tu2=measurements.CURRENT_PERIOD[1];
		//get referenced values
		static unsigned long u1=wheels.get_motor1_freq();
		static unsigned long u2=wheels.get_motor2_freq();
		
	      //control algol([u1,Tu1],[u2,Tu2]){
		static unsigned long err1=Tu1-u1;
		static unsigned long err2=Tu2-u2;
		u1=Tu1;
		u2=Tu2;
	      //}
	       
	       //set the new values
		wheels.set_freqs(u1,u2);
	}
	PT_END(ptt);
}

//Assume that a speed>0 has been scheduled.
void Motion_Controller::schedule_path(){
	if(path_activate && i<path_size){
		path_thread(&ptPath);
	}
}


int Motion_Controller::path_thread(struct pt* ptt){
	PT_BEGIN(ptt);
	while(1){
		t0=millis();
		path_t=set_path(dist_p[i],theta_p[i],i);
		i++;
		PT_WAIT_UNTIL(ptt,millis()-t0>=path_t);
		end_path();
	}
	PT_END(ptt);
}

void Motion_Controller::end_path(){
	if(end_path_option){
		wheels.stop_moving();
	}
}

unsigned long Motion_Controller::set_path(float d,float theta,int i){
	if(path_size-i==1){
		end_path_option=true;
	}
	else{
		end_path_option=false;
	}
	return calculate(d,theta);
}

float Motion_Controller::motor_linear_speed(){
	//get the periods 
	static unsigned long u1=wheels.get_motor1_freq();
	static unsigned long u2=wheels.get_motor2_freq();
	//linear model for the periods. period_motor=T(period_given)
	static float Tu1=consts.alpha0u1*u1+consts.alpha1u1;
	static float Tu2=consts.alpha0u2*u2+consts.alpha1u2;
	//speeds
	static float v1=2*M_PI/Tu1;
	static float v2=2*M_PI/Tu2;
	if(v1>=v2){
		return v1;
	}
	else{
		return v2;
	}
}

void Motion_Controller::set_motor_linear_speed(float v1,float v2){
	static float Tu1=2*M_PI/v1;
	static float Tu2=2*M_PI/v2;
	
	static unsigned long u1=(unsigned long)((Tu1-consts.alpha1u1)/consts.alpha0u1);
	static unsigned long u2=(unsigned long)((Tu2-consts.alpha1u2)/consts.alpha0u2);
	
	wheels.set_freqs(u1,u2);
}

//theta in radians, dist in micro meters
unsigned long Motion_Controller::calculate(float dist,float theta){
	static bool flag=theta > M_PI/2;
	if(flag){
		theta=M_PI-theta;
	}
	
	static float dist_pp=sqrt(pow(consts.center_dist,2)+pow(dist,2)-2*dist*consts.center_dist*cos(theta));
	static float vt=motor_linear_speed();
	static unsigned long t=(dist_pp-theta*consts.radius)/vt;
	static float vr=theta*consts.radius/t;
	static float vm=vr+vt;
	
	if (flag){
		set_motor_linear_speed(vt,vm);
	}
	else{
		set_motor_linear_speed(vm,vt);
	}
	return t;
}