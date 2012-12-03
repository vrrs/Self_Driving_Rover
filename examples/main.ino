#include <Motion_Controller.h>

/*

#include <Measurements.h>
#include <pt.h>
#include <cntr.wheels_Controller.h>
#include <Consts.h>

*/
Motion_Controller cntr;

unsigned long dt;

void setup(){
/*
	attachInterrupt(Measurements::CA1_INTERRUPT,channelA1,CHANGE);
	attachInterrupt(Measurements::CA2_INTERRUPT,channelA2,CHANGE);
*/	
	cntr.wheels.start_moving(cntr.consts.FORWARD,500L,500L);
	dt=millis();
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
	if(millis()-dt>10000){
		cntr.path_activate=true;
	}
	//measurements.scheduleChannelPolling();
}

void channelA1(){
	//measurements.channelA(Measurements::CA1_INTERRUPT,0);
}

void channelA2(){
	//measurements.channelA(Measurements::CA2_INTERRUPT,2);
}

