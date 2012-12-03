#include "Arduino.h"
#include <Wheels_Controller.h>

struct pt                  Wheels_Controller::ptM1;
struct pt                  Wheels_Controller::ptM2;
volatile unsigned long	   Wheels_Controller::FREQ1[2];            
volatile int               Wheels_Controller::PIN[2];
volatile bool              Wheels_Controller::flag[2];
volatile unsigned long     Wheels_Controller::startime[2];
volatile unsigned long     Wheels_Controller::PERIOD;
int                        Wheels_Controller::DIRECTION;
bool                       Wheels_Controller::MOVING;

Wheels_Controller::Wheels_Controller(){
	PT_INIT(&ptM1); 
	PT_INIT(&ptM2); 
	
	reset();
}

void Wheels_Controller::set_direction(int direction){
	Wheels_Controller::DIRECTION=direction;
	int pin1=0;
	int pin2=0;
	if(direction==consts.FORWARD){
			pin1=consts.MOTOR1_CLOCKWISE;
			pin2=consts.MOTOR2_COUNTERCLOCKWISE;
	}
	else{
		if(direction==consts.BACKWARD){
			pin2=Wheels_Controller::consts.MOTOR2_CLOCKWISE;
			pin1=Wheels_Controller::consts.MOTOR1_COUNTERCLOCKWISE;
		}
	}
	Wheels_Controller::PIN[0]=pin1;
	Wheels_Controller::PIN[1]=pin2;
}

void Wheels_Controller::start_moving(int direction,unsigned long m1u1,unsigned long m2u1){
	if(!MOVING){
		FREQ1[0]=m1u1;
		FREQ1[1]=m2u1;
		set_direction(direction);
		MOVING=true;
	}
}

void Wheels_Controller::set_freqs(float m1v,float m2v){
	FREQ1[0]=(unsigned long)(m1v/consts.alpha);
	FREQ1[1]=(unsigned long)(m2v/consts.alpha);
}

void Wheels_Controller::stop_moving(){
	MOVING=false;
	reset();
}

void Wheels_Controller::reset(){
	PERIOD=1000L;   //1 ms
	for(int i=0;i<2;i++){
		FREQ1[i]=100L;
		flag[i]=true;
		startime[i]=0L;
	}
	MOVING=false;
	DIRECTION=consts.FORWARD;

}

void Wheels_Controller::schedule_wheel_motion(){
	if(MOVING){
		moveWheel0(&ptM1);
		moveWheel1(&ptM2);
	}
}

int Wheels_Controller::moveWheel0(struct pt *ptt){
	PT_BEGIN(ptt);
	while(1){
		PT_WAIT_UNTIL(ptt,flag[0] || (( micros() - startime[0]) > (PERIOD - FREQ1[0] )));
		if (!flag[0])	{
			startime[0]=micros();
			flag[0]=true;
		}
		digitalWrite(PIN[0],HIGH);
		PT_WAIT_UNTIL(ptt, (micros() - startime[0]) > FREQ1[0]);
		digitalWrite(PIN[0],LOW);
		startime[0]=micros();
		flag[0]=false;
	}
	PT_END(ptt);
}

int Wheels_Controller::moveWheel1(struct pt *ptt){
	PT_BEGIN(ptt);
	while(1){
		PT_WAIT_UNTIL(ptt,flag[1] || ( (micros() - startime[1]) > (PERIOD - FREQ1[1]) ));
		if (!flag[1])	{
			startime[1]=micros();
			flag[1]=true;
		}
		digitalWrite(PIN[1],HIGH);
		PT_WAIT_UNTIL(ptt, (micros() - startime[1]) > FREQ1[1]);
		digitalWrite(PIN[1],LOW);
		startime[1]=micros();
		flag[1]=false;
	}
	PT_END(ptt);
}

float Wheels_Controller::get_motor1_freq(){
	return (float) consts.alpha*FREQ1[0];
}

float Wheels_Controller::get_motor2_freq(){
	return (float) consts.alpha*FREQ1[1];
}