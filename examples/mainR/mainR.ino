#include <Motion_Controller.h>

Motion_Controller cntr;

unsigned long t0;

//initial interval before turning
const unsigned long dt=10000;

void setup(){

	attachInterrupt(Measurements::CA1_INTERRUPT,channelA1,CHANGE);
	attachInterrupt(Measurements::CA2_INTERRUPT,channelA2,CHANGE);
	
        //delay before moving
	delay(10000);

        //start moving in straight line
	cntr.wheels.start_moving(cntr.consts.FORWARD,500L,500L);
	t0=millis();

      //set up path in polar coordinates
	cntr.theta_p[0]=M_PI/4;
	cntr.dist_p[0]=3;
	cntr.theta_p[1]=M_PI/8;
	cntr.dist_p[1]=4;

     //Serial.begin(9600);
}


void loop(){
	cntr.wheels.schedule_wheel_motion();
	cntr.schedule_path();
	
	//activate path traveling
	if(millis()-t0>dt){
		cntr.path_activate=true;
	}
}

void channelA1(){
	measurements.channelA(Measurements::CA1_INTERRUPT,1);
}

void channelA2(){
	measurements.channelA(Measurements::CA2_INTERRUPT,3);
}

