#include "Arduino.h"
#include <Motion_Controller.h>

volatile unsigned long     Motion_Controller::path_t;
volatile unsigned long     Motion_Controller::t0;
volatile bool              Motion_Controller::end_path_option;
volatile int               Motion_Controller::path_size;
volatile int               Motion_Controller::i;
Wheels_Controller          Motion_Controller::wheels;
struct pt                  Motion_Controller::ptPath;
volatile float             Motion_Controller::theta_p[2];
volatile float             Motion_Controller::dist_p[2];
//void                     Motion_Controller::end_path()
//unsigned long            Motion_Controller::set_path(float,float,int)
Consts                     Motion_Controller::consts;
//float                    Motion_Controller::get_rotational_speed()

Motion_Controller::Motion_Controller(){
	PT_INIT(&ptPath);
	path_size=10;
	path_activate=false;
	
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
	/*while(accelerate_activate){
		wait_until(millis()-dt0>=dt);
		set fm()=fm+df;
		if(h--==0){
			accelerate_activate=false;
		}
		dt0=millis();
	}
	*/
//}

//void Motion_Controller::schedule_speed_control(){
	/*while(control_activate){
		wait_until(measurement.measurement_ready())
		f_measured=measurement.get_freq();
		err=fm-f_measured;
		//modify here to be a pid controller
		set fm()=fm+err
	}
	*/
//}

//static int speed_control_thread(struct pt* ptt){
	/*PT_BEGIN(ptt);
	while(1){
		wait_until(measurement_ready())
		f_measured=measurement.get_freq();
		err=fm-f_measured;
		//modify here to be a pid controller
		set fm()=fm+err
	}
	PT_END(ptt);
	*/
//}

//Assume that a speed>0 has been scheduled.
void Motion_Controller::schedule_path(){
	if(path_activate && i<path_size){
		path_thread(&ptPath);
	}
}

float Motion_Controller::get_rotational_speed(){
	return (float) (consts.alpha)*(consts.rotational_freq);
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
	static float f1=wheels.get_motor1_freq();
	static float f2=wheels.get_motor2_freq();
	if(f1>=f2){
		return 2*M_PI*consts.radius*f1;
	}
	else{
		return 2*M_PI*consts.radius*f2;
	}
}

//theta in radians, dist in meter
unsigned long Motion_Controller::calculate(float dist,float theta){
	static float vm=motor_linear_speed();
	static unsigned long t;
	static float vt;
	if(theta!=M_PI/2){
		static bool side=true;
		if(theta>M_PI/2){
			theta=M_PI-theta;
			side=false;
		}
		
		vt=vm-get_rotational_speed();
		static float dist_t=sqrt(pow(dist*cos(theta)-consts.center_dist,2)+pow(dist*sin(theta),2));
		t=(unsigned long)dist_t/vt;
		
		if (side){
			wheels.set_freqs(vm,vt);
		}
		else{
			wheels.set_freqs(vt,vm);
		}
	}
	else{
	  t=(unsigned long) dist/vm;
	}
	/*
	Serial.print("vm:");
	Serial.print(vm);
	Serial.print(",vt:");
	Serial.print(vt);
	Serial.print(",t:");
	Serial.println(t);
	*/
	return t;
}