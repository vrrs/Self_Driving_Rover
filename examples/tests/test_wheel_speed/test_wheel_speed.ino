#include <Measurements.h>
#include <pt.h>
#include <Wheels_Controller.h>
#include <Consts.h>

/*   The rover will moved according to the set_new_speed_time function, which moves the car at multiples of 10 s at a given speed. It prints the 
 *  measured period on the wheel using the measurement class.
*/

Measurements measurements;
Wheels_Controller cntr; 
Consts consts;

unsigned long dt=60000L;
unsigned long t0;
unsigned long speed;
struct pt prth;
boolean flag[5];

void setup(){
        delay(4000);
        attachInterrupt(Measurements::CA1_INTERRUPT,channelA1,CHANGE);
	attachInterrupt(Measurements::CA2_INTERRUPT,channelA2,CHANGE);
	PT_INIT(&prth);

	Serial.begin(9600);
        for(int i=0;i<5;i++){
           flag[i]=true; 
        }
	t0=millis();
        measurements.start_measuring();      
}


void loop(){

	set_new_speed_time(250L,0);
        //dont use many set_new_speed_time, it would decrease the speed
	//set_new_speed_time(300L,3);

	stopM(1);
	cntr.schedule_wheel_motion();	
	pr_thread(&prth);
}
void set_new_speed_time(unsigned long speed, int tf){
	if(millis()-t0>=tf*dt && flag[tf]){
		cntr.stop_moving();
		Serial.print("v=");
		Serial.println(speed);
		cntr.start_moving(consts.FORWARD,speed,speed);
                flag[tf]=false;
	}
}

void stopM(int tf){
   if(millis()-t0>=tf*dt && flag[tf]){
	cntr.stop_moving(); 
        flag[tf]=false;
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