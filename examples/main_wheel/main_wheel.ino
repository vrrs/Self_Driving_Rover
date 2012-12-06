#include <Measurements.h>
#include <pt.h>
#include <Wheels_Controller.h>
#include <Consts.h>

//this code use wheel controller only to command the rover

Measurements measurements;
Wheels_Controller cntr; 
Consts consts;

void setup(){
	attachInterrupt(Measurements::CA1_INTERRUPT,channelA1,CHANGE);
	attachInterrupt(Measurements::CA2_INTERRUPT,channelA2,CHANGE);
	
	cntr.start_moving(consts.FORWARD,500L,500L);
	measurements.start_measuring();
}


void loop(){
	cntr.schedule_wheel_motion();
}

void channelA1(){
	measurements.channelA(Measurements::CA1_INTERRUPT,0);
}

void channelA2(){
	measurements.channelA(Measurements::CA2_INTERRUPT,2);
}


