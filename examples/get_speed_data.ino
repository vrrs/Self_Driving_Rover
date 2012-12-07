#include <Measurements.h>
#include <pt.h>
#include <Wheels_Controller.h>
#include <Consts.h>

//this code use wheel controller only to command the rover

Measurements measurements;
Wheels_Controller cntr; 
Consts consts;

unsigned long dt=10000L;
unsigned long t0;
unsigned long speed;
struct pt prth;
void setup(){
  attachInterrupt(Measurements::CA1_INTERRUPT,channelA1,CHANGE);
	attachInterrupt(Measurements::CA2_INTERRUPT,channelA2,CHANGE);
	PT_INIT(&prth);
	speed=200L;
	Serial.print("v=");
	Serial.println(speed);
	cntr.start_moving(consts.FORWARD,speed,speed);
	measurements.start_measuring();
	Serial.begin(9600);
	t0=millis();
}


void loop(){
	cntr.schedule_wheel_motion();
	set_new_speed(250L,1);
	set_new_speed(300L,2);
	
	/* dont use many set_new_speed, it would decrease the speed
	 set_new_speed(350L,1);
	 set_new_speed(400L,2);
	 set_new_speed(450L,1);
	 set_new_speed(500L,2);
	 set_new_speed(550L,1);
	 set_new_speed(600L,2);
	 set_new_speed(650L,1);
	 set_new_speed(700L,2);
	 set_new_speed(750L,1);
	 set_new_speed(800L,2);
	 set_new_speed(850L,1);
	 set_new_speed(900L,2);
	 set_new_speed(950L,1);
	 set_new_speed(1000L,2);
	*/
	pr_thread(&prth);
}
void set_new_speed(unsigned long speed, int tf){
	if(millis()-t0>=tf*dt){
		cntr.stop_moving();
		Serial.print("v=");
		Serial.println(speed);
		cntr.start_moving(consts.FORWARD,speed,speed);
	}

}

static int pr_thread(struct pt* ptt){
	PT_BEGIN(ptt);
	while(1){
		PT_WAIT_UNTIL(ptt,measurements.PERIOD_FLAG[0] && measurements.PERIOD_FLAG[1]);
		//get measured values
		static unsigned long Tu1=measurements.CURRENT_PERIOD[0];
		static unsigned long Tu2=measurements.CURRENT_PERIOD[1];
		Serial.print(Tu1);
		Serial.print("    ");
		Serial.println(Tu2);
	}
	PT_END(ptt);
}

void channelA1(){
	measurements.channelA(Measurements::CA1_INTERRUPT,1);
}

void channelA2(){
	measurements.channelA(Measurements::CA2_INTERRUPT,3);
}
