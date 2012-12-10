#include <Motion_Controller.h>

Motion_Controller cntr;

unsigned long t0;

//initial interval before turning
const unsigned long dt=10000;

void setup(){

	attachInterrupt(Measurements::CA1_INTERRUPT,channelA1,CHANGE);
	attachInterrupt(Measurements::CA2_INTERRUPT,channelA2,CHANGE);
	
        //delay before moving
	delay(2000);

        t0=millis();

      //set up path in polar coordinates
	cntr.theta_p[0]=M_PI/4;
	cntr.dist_p[0]=3;
	cntr.theta_p[1]=M_PI/8;
	cntr.dist_p[1]=4;
        cntr.path_activate=true;
}


void loop(){
	cntr.wheels.schedule_wheel_motion();
	cntr.schedule_path();
	cntr.schedule_speed_control();	
}

void channelA1(){
	cntr.measurements.channelA(Measurements::CA1_INTERRUPT,1);
}

void channelA2(){
	cntr.measurements.channelA(Measurements::CA2_INTERRUPT,3);
}