#include <Motion_Controller.h>

Motion_Controller cntr;

//initial interval before turning
const unsigned long dt=2000;
unsigned long t0;
boolean flag;

void setup(){
	attachInterrupt(Measurements::CA1_INTERRUPT,channelA1,CHANGE);
	attachInterrupt(Measurements::CA2_INTERRUPT,channelA2,CHANGE);

      //set up path in polar coordinates
	cntr.theta_p[0]=M_PI/2;
	cntr.dist_p[0]=3;
/*	cntr.theta_p[1]=M_PI/4;
	cntr.dist_p[1]=2;
        cntr.theta_p[2]=M_PI/8;
	cntr.dist_p[2]=4;
*/
        cntr.set_initial_speed();
       
        //cntr.measurements.start_measuring();
        t0=millis();
        flag=true;
}


void loop(){
	cntr.wheels.schedule_wheel_motion();
        cntr.measurements.schedule_sensors();
	cntr.schedule_path();
	cntr.schedule_speed_control();	
//if (millis()-t0>dt && flag) {
  cntr.path_activate=true;
 // flag=false;
//}
}

void channelA1(){
	cntr.measurements.channelA(Measurements::CA1_INTERRUPT,1);
}

void channelA2(){
	cntr.measurements.channelA(Measurements::CA2_INTERRUPT,3);
}
