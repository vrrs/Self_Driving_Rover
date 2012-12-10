#include <Measurements.h>
#include <pt.h>
#include <Wheels_Controller.h>
#include <Consts.h>

/*
	the rover is supposed to move forward for 10 s, then stop moving. In 3 s, it will start moving for 7 s, then for 2 seconds will turn. Then stop
*/

Measurements measurements;
Wheels_Controller cntr; 
Consts consts;
unsigned long t0;
bool flag[4];

void setup(){
	attachInterrupt(Measurements::CA1_INTERRUPT,channelA1,CHANGE);
	attachInterrupt(Measurements::CA2_INTERRUPT,channelA2,CHANGE);

        for(int i=0;i<4;i++){
          flag[i]=true;
        }
        delay(4000);
	cntr.start_moving(consts.FORWARD,500L,500L);
	measurements.start_measuring();
	t0=millis();
}


void loop(){
	cntr.schedule_wheel_motion();
//	measurements.schedule_sensors();
	
	if(millis()-t0>10000 && flag[0]){
		cntr.stop_moving();
		flag[0]=false;
	}
	if(millis()-t0>13000 && flag[1]){
		cntr.start_moving(consts.FORWARD,300L,300L);
		flag[1]=false;
	}
	if(millis()-t0>20000 && flag[2]){
  		cntr.stop_moving();
		cntr.start_moving(consts.FORWARD,400L,450L);
		flag[2]=false;
	}
	if(millis()-t0>22000){
		cntr.stop_moving();
	}
}

void channelA1(){
	measurements.channelA(Measurements::CA1_INTERRUPT,1);
}

void channelA2(){
	measurements.channelA(Measurements::CA2_INTERRUPT,3);
}