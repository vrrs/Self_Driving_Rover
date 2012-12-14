#include "Arduino.h"
#include <Motion_Controller.h>
 
// Linear model assumes u in microseconds and Tu in ms. Speed is set and return in km/s.

volatile unsigned long     Motion_Controller::path_t;
volatile unsigned long     Motion_Controller::t0;
<<<<<<< HEAD
volatile int               Motion_Controller::path_size;
volatile int               Motion_Controller::i;
=======

volatile int               Motion_Controller::back;
volatile int               Motion_Controller::front;
volatile float             Motion_Controller::theta_p[capacity];
volatile float             Motion_Controller::dist_p[capacity];

>>>>>>> Fixed motion controller and pt lib
Wheels_Controller          Motion_Controller::wheels;
Consts                     Motion_Controller::consts;
Measurements               Motion_Controller::measurements;

struct pt                  Motion_Controller::ptPath;
struct pt                  Motion_Controller::ptSpeed;
struct pt                  Motion_Controller::ptAccelerate;
<<<<<<< HEAD
volatile float             Motion_Controller::theta_p[5];
volatile float             Motion_Controller::dist_p[5];
=======

>>>>>>> Fixed motion controller and pt lib
volatile float             Motion_Controller::dv;
volatile unsigned long     Motion_Controller::dt;
volatile unsigned long     Motion_Controller::tt0;
volatile int               Motion_Controller::n;
<<<<<<< HEAD
Consts                     Motion_Controller::consts;
Measurements               Motion_Controller::measurements;
volatile bool 		   Motion_Controller::accelerate_activate;
volatile bool 		   Motion_Controller::speed_changed;
=======

volatile bool 			   Motion_Controller::accelerate_activate;
volatile bool 			   Motion_Controller::speed_changed;
volatile bool 			   Motion_Controller::path_activate;
>>>>>>> Fixed motion controller and pt lib

Motion_Controller::Motion_Controller(){
	PT_INIT(&ptPath);
	PT_INIT(&ptSpeed);
	PT_INIT(&ptAccelerate);
<<<<<<< HEAD
	path_size=5;
	path_activate=false;
	speed_control_activated=true;
	wheels.set_freqs(consts.default_initial_speed,consts.default_initial_speed);
	for(int i=0;i<path_size;i++){
=======
	
	back=0;
	front=0;
	path_t=0L;
	
	path_activate=false;
	speed_control_activated=false;
	accelerate_activate=false;
	speed_changed=false;
	time_flag=true;
	
	wheels.set_freqs(consts.default_initial_speed,consts.default_initial_speed);
	for(int i=0;i<capacity;i++){
>>>>>>> Fixed motion controller and pt lib
		theta_p[i]=0;
		dist_p[i]=0;
	}
}
//start moving
void Motion_Controller::move(bool regulate_speed){
	speed_control_activated=regulate_speed;
	wheels.MOVING=true;
	path_activate=true;
}
//stop moving
void Motion_Controller::stop(){
	path_activate=false;
	wheels.stop_moving();
}

//acceleration section {

//new speed will be updated after current path ends.
void Motion_Controller::accelerate(unsigned long t,float vf){
	float v0=motor_linear_speed();
	n=consts.NUMBER_PARTITIONS_INTERVAL;
	
	dv=(vf-v0)/n;
	dt=(unsigned long) t/n;
	tt0=millis();
	accelerate_activate=true;   //fire event
}

int Motion_Controller::accelerate_thread(struct pt* ptt){
	PT_BEGIN(ptt);
	while(1){
		PT_WAIT_UNTIL(ptt,millis()-tt0>=dt);
		add_motor_speed(dv);
		if(--n<0){
			accelerate_activate=false;
		}
		tt0=millis();
	}
	PT_END(ptt);
}

void Motion_Controller::schedule_acceleration(){
	if(accelerate_activate){
		accelerate_thread(&ptAccelerate);
	}
}
//}

//speed regulator section{ 
void Motion_Controller::schedule_speed_control(){
	if(speed_control_activated){
		speed_control_thread(&ptSpeed);
	}
}

//
int Motion_Controller::speed_control_thread(struct pt* ptt){
	PT_BEGIN(ptt);
	while(1){
		//wait until measurement is ready
		PT_WAIT_UNTIL(ptt,measurements.PERIOD_FLAG[0] && measurements.PERIOD_FLAG[1]);
		//get measured values, time period that each wheel takes to turn a revolution, in ms
		static unsigned long Tu1=measurements.CURRENT_PERIOD[0];
		static unsigned long Tu2=measurements.CURRENT_PERIOD[1];
		//get referenced values in microseconds
		static unsigned long u1=wheels.get_motor1_freq();
		static unsigned long u2=wheels.get_motor2_freq();
		
	   //control algol([u1,Tu1],[u2,Tu2]){
		static unsigned long err1=Tu1-u1;
		static unsigned long err2=Tu2-u2;
		static unsigned long du1=(err1-consts.alpha1u1)/consts.alpha0u1;
		static unsigned long du2=(err2-consts.alpha1u2)/consts.alpha0u2;
		u1=du1+u1;
		u2=du2+u2;
	   //}
	       
	    //set the new values
		wheels.set_freqs(u1,u2);
		speed_changed=true;
		measurements.PERIOD_FLAG[0]=false;
		measurements.PERIOD_FLAG[1]=false;
	}
	PT_END(ptt);
}

//Assume that a speed>0 has been scheduled.
void Motion_Controller::schedule_path(){
	if(path_activate){
<<<<<<< HEAD
		t0=millis();
=======
>>>>>>> Fixed motion controller and pt lib
		path_thread(&ptPath);
	}
	if(path_activate && time_flag){
		t0=millis();
		time_flag=false;
	}
}


int Motion_Controller::path_thread(struct pt* ptt){
	PT_BEGIN(ptt);
	static float dist_set=0;
	while(1){
		PT_WAIT_UNTIL(ptt,millis()-t0>=path_t && path_activate || speed_changed);
		if(speed_changed){
<<<<<<< HEAD
			dist_set=dist_p[i]-motor_linear_speed()*(millis()-t0);
			i--;
			speed_changed=false;
		}
		else{
			dist_set=dist_p[i];
		}
		if(path_size==i){
			//stop the rover
			wheels.stop_moving();
			path_activate=false;
		}
		else{
			path_t=calculate(dist_set,theta_p[i]);
			t0=millis();
		}
		i++;
	}
	PT_END(ptt);
}
=======
			dist_set=dist_p[front]-motor_linear_speed()*(millis()-t0);
			deltaFront(false); //decrease front
			
			speed_changed=false;
		}
		else{
			dist_set=dist_p[front];
		}
		if(back==front){
			stop();
		}
		else{
			path_t=calculate(dist_set,theta_p[front]);
			t0=millis();
		}
		deltaFront(true);//increase front
	}
	PT_END(ptt);
}
void  Motion_Controller::deltaFront(bool increment){
	if(increment){
		front=(front+1)%capacity;
	}
	else{	
		front--;
		if(front<0)
			front=front+capacity;
	}
}

bool  Motion_Controller::full(){
  return size()==capacity;
}

int  Motion_Controller::size(){
	static int siz=back-front+1;
	if(front>back)
		return siz+capacity;
	return siz;
}
>>>>>>> Fixed motion controller and pt lib

//return speed km/s
float Motion_Controller::motor_linear_speed(){
	//get the periods 
	static unsigned long u1=wheels.get_motor1_freq();
	static unsigned long u2=wheels.get_motor2_freq();
	//linear model for the periods. period_motor=T(period_given)
	static float Tu1=consts.alpha0u1*u1+consts.alpha1u1;
	static float Tu2=consts.alpha0u2*u2+consts.alpha1u2;
	//speeds
	static float v1=2*M_PI*consts.radius/Tu1;
	static float v2=2*M_PI*consts.radius/Tu2;
	if(v1>=v2){
		return v1;
	}
	else{
		return v2;
	}
}

void Motion_Controller::set_motor_linear_speed(float v1,float v2){
	static float Tu1=2*M_PI*consts.radius/v1;
	static float Tu2=2*M_PI*consts.radius/v2;
	
	static unsigned long u1=(unsigned long)((Tu1-consts.alpha1u1)/consts.alpha0u1);
	static unsigned long u2=(unsigned long)((Tu2-consts.alpha1u2)/consts.alpha0u2);
	
	wheels.set_freqs(u1,u2);
}

void Motion_Controller::add_motor_speed(float dv){
	//get the periods 
	static unsigned long u1=wheels.get_motor1_freq();
	static unsigned long u2=wheels.get_motor2_freq();
	//linear model for the periods. period_motor=T(period_given)
	static float Tu1=consts.alpha0u1*u1+consts.alpha1u1;
	static float Tu2=consts.alpha0u2*u2+consts.alpha1u2;
	//speeds
	static float v1=2*M_PI*consts.radius/Tu1+dv;
	static float v2=2*M_PI*consts.radius/Tu2+dv;
	
	//set the new speed
	Tu1=2*M_PI*consts.radius/v1;
	Tu2=2*M_PI*consts.radius/v2;
	
	u1=(unsigned long)((Tu1-consts.alpha1u1)/consts.alpha0u1);
	u2=(unsigned long)((Tu2-consts.alpha1u2)/consts.alpha0u2);
	
	wheels.set_freqs(u1,u2);
}

<<<<<<< HEAD
=======


>>>>>>> Fixed motion controller and pt lib
//theta in radians, dist in meters, time in ms
unsigned long Motion_Controller::calculate(float dist,float theta){
	static float vt=motor_linear_speed();
	
	if(theta == M_PI/2){
		set_motor_linear_speed(vt,vt);
		return (unsigned long) dist/vt;
	}
	
	static bool flag=theta > M_PI/2;
	if(flag){
		theta=M_PI-theta;
	}
	
	static float dist_pp=sqrt(pow(consts.center_dist,2)+pow(dist,2)-2*dist*consts.center_dist*cos(theta)); //meter
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