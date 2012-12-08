#include <Measurements.h>
#include <pt.h>
#include <Wheels_Controller.h>
#include <Consts.h>

/*
	the rover is supposed to move forward for 10 s, then stop moving. In 3 s, it will start moving for 7 s, this time slower. 
*/

Measurements measurements;
Wheels_Controller cntr; 
Consts consts;
unsigned long t0;
bool flag;
bool flag1;

void setup(){
	attachInterrupt(Measurements::CA1_INTERRUPT,channelA1,CHANGE);
	attachInterrupt(Measurements::CA2_INTERRUPT,channelA2,CHANGE);
	flag=true;
	flag1=true;
	cntr.start_moving(consts.FORWARD,500L,500L);
	measurements.start_measuring();
	t0=millis();
}


void loop(){
	cntr.schedule_wheel_motion();
	measurements.schedule_sensors();
	
	if(millis()-t0>10000 & flag){
		cntr.stop_moving();
		flag=false;
	}
	if(millis()-t0>13000 & flag1){
		cntr.start_moving(consts.BACKWARD,300L,300L);
		flag1=false;
	}
	if(millis()-t0>20000){
		cntr.stop_moving();
	}
}

void channelA1(){
	measurements.channelA(Measurements::CA1_INTERRUPT,1);
}

void channelA2(){
	measurements.channelA(Measurements::CA2_INTERRUPT,3);
}