#include "Arduino.h"
#include <Measurements.h>

unsigned volatile long    Measurements::CURRENT_PERIOD[2];
volatile int              Measurements::CURRENT_DIRECTION[2];
volatile bool             Measurements::PERIOD_FLAG[2];

volatile long     		  Measurements::timer0;
volatile long     		  Measurements::timer1;
volatile long     		  Measurements::timer2;
volatile bool             Measurements::flag0[2];
volatile bool             Measurements::flag1[2];
volatile bool             Measurements::flag2[2];
struct pt                 Measurements::ptsensor0;
struct pt                 Measurements::ptsensor1;
struct pt                 Measurements::ptsensor2;

volatile bool             Measurements::objflag[2];
volatile float            Measurements::objdist[2];
Consts 					  Measurements::consts;

Measurements::Measurements(){
	PT_INIT(&ptsensor0);
	PT_INIT(&ptsensor1);
	PT_INIT(&ptsensor2);
	
	for(int i=0;i<2;i++){
		cnt[i]=0;
		startime[i]=0L;
		CURRENT_DIRECTION[i]=consts.FORWARD;
		CURRENT_PERIOD[i]=0L;
		PERIOD_FLAG[i]=false;
		objflag[i]=false;
		objdist[i]=0;
	}
	measuring=false;
}

void Measurements::channelA(int motor,int channelb){
	if(cnt[motor]==0){
		PERIOD_FLAG[motor]=false;
	}
	if(++cnt[motor]==consts.NPPR){
		cnt[motor]=0;
		set_direction(digitalRead(consts.MOTOR_CHANNELS[channelb-1]),digitalRead(consts.MOTOR_CHANNELS[channelb]),motor);
		CURRENT_PERIOD[motor]=millis()-startime[motor];
		startime[motor]=millis();
		
		//notify that a measurement has been taken
		PERIOD_FLAG[motor]=true;
	}	
}

void Measurements::start_measuring(){
	startime[0]=millis();
	startime[1]=millis();
	timer0=millis();
	timer1=millis();
	timer2=millis();
	for(int i=0;i<2;i++){
		flag0[i]=true;
		flag1[i]=true;
		flag2[i]=true;
	}
	
	measuring=true;
}

void Measurements::stop_measuring(){
	measuring=false;
}

void Measurements::set_direction(int a,int b,int motor){
	if (a==b){
		CURRENT_DIRECTION[motor]=consts.FORWARD;
	}
	else{
		CURRENT_DIRECTION[motor]=consts.BACKWARD;
	}
}

void Measurements::schedule_sensors(){
	if(measuring){
		sensor0thread(&ptsensor0);
		sensor1thread(&ptsensor1);
		sensor2thread(&ptsensor2);
	}
}

int Measurements::sensor0thread(struct pt *ptt) {
  PT_BEGIN(ptt);
  while(1) {
    PT_WAIT_UNTIL(ptt, millis() - timer0 > consts.SENSE_INTERVAL);
    
	if(flag0[0]){
		pinMode(consts.ULTRASONIC_SENSOR0, OUTPUT);
		digitalWrite(consts.ULTRASONIC_SENSOR0, LOW);
		objflag[0]=false;
		flag0[0]=false;
    }
	
	PT_WAIT_UNTIL(ptt, millis() - timer0 > 5 + consts.SENSE_INTERVAL);
	
	if(flag0[1]){
		digitalWrite(consts.ULTRASONIC_SENSOR0, HIGH);
		flag0[1]=false;
	}
	
	PT_WAIT_UNTIL(ptt, millis() - timer0 > 10 + consts.SENSE_INTERVAL);
	
	digitalWrite(consts.ULTRASONIC_SENSOR0, LOW);
	pinMode(consts.ULTRASONIC_SENSOR0, INPUT);
    static long duration = pulseIn(consts.ULTRASONIC_SENSOR0, HIGH);
	objdist[0]= duration / 29 / 200;
	objflag[0]=true;
	
	timer0=millis();
	flag0[0]=true;
	flag0[1]=true;
  }
  PT_END(ptt);
}

int Measurements::sensor1thread(struct pt *ptt) {
  PT_BEGIN(ptt);
  while(1) {
    PT_WAIT_UNTIL(ptt, millis() - timer1 > consts.SENSE_INTERVAL);
    
	if(flag1[0]){
		pinMode(consts.ULTRASONIC_SENSOR1, OUTPUT);
		digitalWrite(consts.ULTRASONIC_SENSOR1, LOW);
		objflag[1]=false;
		flag1[0]=false;
    }
	
	PT_WAIT_UNTIL(ptt, millis() - timer1 > 5 + consts.SENSE_INTERVAL);
	
	if(flag1[1]){
		digitalWrite(consts.ULTRASONIC_SENSOR1, HIGH);
		flag1[1]=false;
	}
	
	PT_WAIT_UNTIL(ptt, millis() - timer1 > 10 + consts.SENSE_INTERVAL);
	
	digitalWrite(consts.ULTRASONIC_SENSOR1, LOW);
	pinMode(consts.ULTRASONIC_SENSOR1, INPUT);
    static long duration = pulseIn(consts.ULTRASONIC_SENSOR1, HIGH);
	objdist[1]= duration / 29 / 200;
	objflag[1]=true;
	
	timer1=millis();
	flag1[0]=true;
	flag1[1]=true;
  }
  PT_END(ptt);
}

int Measurements::sensor2thread(struct pt *ptt) {
  PT_BEGIN(ptt);
  while(1) {
    PT_WAIT_UNTIL(ptt, millis() - timer2 > consts.SENSE_INTERVAL);
    
	if(flag2[0]){
		pinMode(consts.ULTRASONIC_SENSOR2, OUTPUT);
		digitalWrite(consts.ULTRASONIC_SENSOR2, LOW);
		objflag[2]=false;
		flag2[0]=false;
    }
	
	PT_WAIT_UNTIL(ptt, millis() - timer2 > 5 + consts.SENSE_INTERVAL);
	
	if(flag2[1]){
		digitalWrite(consts.ULTRASONIC_SENSOR2, HIGH);
		flag2[1]=false;
	}
	
	PT_WAIT_UNTIL(ptt, millis() - timer2 > 10 + consts.SENSE_INTERVAL);
	
	digitalWrite(consts.ULTRASONIC_SENSOR2, LOW);
	pinMode(consts.ULTRASONIC_SENSOR2, INPUT);
    static long duration = pulseIn(consts.ULTRASONIC_SENSOR2, HIGH);
	objdist[2]= duration / 29 / 200;//dist in meter
	objflag[2]=true;
	
	timer2=millis();
	flag2[0]=true;
	flag2[1]=true;
  }
  PT_END(ptt);
}
