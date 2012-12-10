#ifndef Consts_h
#define Consts_h

#include "Arduino.h"

class Consts{
	public:
	    Consts();

		const static int MOTOR1_CLOCKWISE=30;          //RED
		const static int MOTOR1_COUNTERCLOCKWISE=31;  //ORANGE
		
		const static int MOTOR2_CLOCKWISE=34;         //GREEN
		const static int MOTOR2_COUNTERCLOCKWISE=35; //YELLOW

		const static int MOTOR1_CHANNEL_A=2;        //YELLOW
		const static int MOTOR1_CHANNEL_B=43;       // WHITE
		
		const static int MOTOR2_CHANNEL_A=3;       //YELLOW  
		const static int MOTOR2_CHANNEL_B=45;       //WHITE
		
		const static float center_dist=0.1145; // meters
		const static float radius=0.054;       //meters
		
	   //linear model param{
		//motor1
		const static float alpha1u1=0;
		const static float alpha0u1=0.00000486;  //slope
		//motor2
		const static float alpha0u2=0.00000486;  //slope
		const static float alpha1u2=0;
	   //}
		
		int MOTOR_CHANNELS[4];
		const static unsigned long PERIOD_CYCLE=1000L;
		const static int NPPR=64;
		const static unsigned long default_initial_speed=200L;
		
		const static int NUMBER_PARTITIONS_INTERVAL=1000;
		const static int ULTRASONIC_SENSOR0=26;
		const static int ULTRASONIC_SENSOR1=27;
		const static int ULTRASONIC_SENSOR2=28;
		const static int SENSE_INTERVAL=20; //microseconds
		const static int FORWARD=1;
		const static int BACKWARD=2;
};
#endif