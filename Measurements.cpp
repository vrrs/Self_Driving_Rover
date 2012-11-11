#include "Arduino.h"
#include <Measurements.h>

struct pt                 Measurements::pt1;
struct pt                 Measurements::pt2;
unsigned volatile long    Measurements::startime[2];
volatile bool             Measurements::channel_flag[2];
unsigned volatile long    Measurements::CURRENT_PERIOD[2];
volatile bool             Measurements::PERIOD_FLAG[2];

Measurements::Measurements(){
	PT_INIT(&pt1);
	PT_INIT(&pt2);
	
	for(int i=0;i<2;i++){
		cnt[i]=0;
		startime[i]=0L;
		CURRENT_DIRECTION[i]=consts.FORWARD;
		CURRENT_PERIOD[i]=consts.FORWARD;
		channel_flag[i]=false;
		PERIOD_FLAG[i]=false;
	}
}

void Measurements::channelA(int motor,int channel){
	if(cnt[motor]==0){
		channel_flag[motor]=true;
	}
	else{
		if(++cnt[motor]==consts.NPPR){
			cnt[motor]=0;
			channel_flag[motor]=true;
			set_direction(digitalRead(consts.MOTOR_CHANNELS[channel]),digitalRead(consts.MOTOR_CHANNELS[channel+1]),motor);
		}	
	}
}

void Measurements::set_direction(int a,int b,int motor){
	if (a==b){
		CURRENT_DIRECTION[motor]=consts.FORWARD;
	}
	else{
		CURRENT_DIRECTION[motor]=consts.BACKWARD;
	}
}

void Measurements::scheduleChannelPolling(){
	pollThread0(&pt1);
	pollThread1(&pt2);
}

int Measurements::pollThread0(struct pt *ptt){
	PT_BEGIN(ptt);
	while(1){
		PT_WAIT_UNTIL(ptt,channel_flag[0]);
		if(startime[0]==0){
			startime[0]=micros();
		}
		else{
			CURRENT_PERIOD[0]=micros()-startime[0];
			PERIOD_FLAG[0]=true;
			startime[0]=0;
		}
	}
	PT_END(ptt);
}

int Measurements::pollThread1(struct pt *ptt){
	PT_BEGIN(ptt);
	while(1){
		PT_WAIT_UNTIL(ptt,channel_flag[1]);
		if(startime[1]==0){
			startime[1]=micros();
		}
		else{
			CURRENT_PERIOD[1]=micros()-startime[1];
			PERIOD_FLAG[1]=true;
			startime[1]=0;
		}
	}
	PT_END(ptt);
}