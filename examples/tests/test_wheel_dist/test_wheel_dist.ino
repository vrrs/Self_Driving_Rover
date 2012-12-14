#include <Measurements.h>
#include <pt/pt.h>
#include <Wheels_Controller.h>
#include <Consts.h>

/*
	the rover is supposed to move forward for 10 s, then stop moving. In 3 s, it will start moving for 7 s, then for 2 seconds will turn. Then stop
*/

Measurements measurements;
Wheels_Controller cntr; 
Consts consts;
unsigned long t0;
unsigned long dt;
unsigned long speed;
bool flag[4];

void setup(){
	attachInterrupt(Measurements::CA1_INTERRUPT,channelA1,CHANGE);
	attachInterrupt(Measurements::CA2_INTERRUPT,channelA2,CHANGE);

    for(int i=0;i<4;i++){
        flag[i]=true;
    }
	
    delay(3000);
	
	dt=5000;
	speed=250L;
	
	t0=millis();
}


void loop(){
	cntr.schedule_wheel_motion();
	
	//start the car
	if(millis()-t0>0 && flag[0]){
		cntr.start_moving(consts.FORWARD,speed,speed);
		flag[0]=false;
	}

	//stop the car 
	if(millis()-t0>dt && flag[1]){
		cntr.stop_moving();
		flag[1]=false;
	}
}

void channelA1(){
	measurements.channelA(Measurements::CA1_INTERRUPT,1);
}

void channelA2(){
	measurements.channelA(Measurements::CA2_INTERRUPT,3);
}