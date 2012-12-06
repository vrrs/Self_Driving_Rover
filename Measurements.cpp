#include "Arduino.h"
#include <Measurements.h>

unsigned volatile long    Measurements::CURRENT_PERIOD[2];
volatile int              Measurements::CURRENT_DIRECTION[2];
volatile bool             Measurements::PERIOD_FLAG[2];

Measurements::Measurements(){
	for(int i=0;i<2;i++){
		cnt[i]=0;
		startime[i]=0L;
		CURRENT_DIRECTION[i]=consts.FORWARD;
		CURRENT_PERIOD[i]=0L;
		PERIOD_FLAG[i]=false;
	}
}

void Measurements::channelA(int motor,int channel){
	if(cnt[motor]==0){
		PERIOD_FLAG[motor]=false;
	}
	if(++cnt[motor]==consts.NPPR){
		cnt[motor]=0;
		set_direction(digitalRead(consts.MOTOR_CHANNELS[channel]),digitalRead(consts.MOTOR_CHANNELS[(channel+1)%2]),motor);
		CURRENT_PERIOD[motor]=millis()-startime[motor];
		startime[motor]=millis();
		
		//notify that a measurement has been taken
		PERIOD_FLAG[motor]=true;
	}	
}

void Measurements::start_measuring(){
	startime[0]=millis();
	startime[1]=millis();
}

void Measurements::set_direction(int a,int b,int motor){
	if (a==b){
		CURRENT_DIRECTION[motor]=consts.FORWARD;
	}
	else{
		CURRENT_DIRECTION[motor]=consts.BACKWARD;
	}
}
