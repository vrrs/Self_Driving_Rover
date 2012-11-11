#include <Measurements.h>
#include <pt.h>
#include <Wheels_Controller.h>
#include <Consts.h>

/*Replaces any previous function that was attached to the interrupt.
 Most Arduino boards have two external interrupts: numbers 0(on digital pin 2) 
 and 1 (on digital pin 3). The Arduino Mega has an additional four: 
 numbers 2 (pin 21), 3 (pin 20), 4 (pin 19), and 5 (pin 18). */

Measurements measurements;
Wheels_Controller cntr; 
Consts consts;

void setup(){
	attachInterrupt(Measurements::CA1_INTERRUPT,channelA1,CHANGE);
	attachInterrupt(Measurements::CA2_INTERRUPT,channelA2,CHANGE);
	
	cntr.start_moving(consts.FORWARD,500L,500L);
	
}


void loop(){
	cntr.schedule_wheel_motion();
	measurements.scheduleChannelPolling();
}

void channelA1(){
	measurements.channelA(Measurements::CA1_INTERRUPT,0);
}

void channelA2(){
	measurements.channelA(Measurements::CA2_INTERRUPT,2);
}


