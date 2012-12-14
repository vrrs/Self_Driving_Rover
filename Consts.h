#ifndef Consts_h
#define Consts_h

#include "Arduino.h"

class Consts{
	public:
	    Consts();
/*
	*	Motor specs:
	*		2.54"×1.45"×1.45" ,12 V brushed DC motor with a 29:1 metal gearbox.
	*		Quadrature encoder: resolution of 64 counts per revolution of the motor shaft, corresponds to 1856 counts per revolution of the gearbox’s output shaft. 
	*		0.61"-long, 6 mm-diameter D-shaped output shaft.
	*		At 12 V: 350 RPM and 300 mA free-run, 110 oz-in (8 kg-cm) and 5 A stall.
	*   Given these specs, we have:
	*     f=(5.83/12)v , where v is voltage in V and f is frequency in RPS=hz. I assume linear proportionality.
	*  
	*	Wheel material friction on concrete	(static,kinetic)=(1.0,0.8)
*/


	  //Motor hardware constants{
		const static int MOTOR1_CLOCKWISE=30;          //RED
		const static int MOTOR1_COUNTERCLOCKWISE=31;  //ORANGE
		
		const static int MOTOR2_CLOCKWISE=34;         //GREEN
		const static int MOTOR2_COUNTERCLOCKWISE=35; //YELLOW

		const static int MOTOR1_CHANNEL_A=2;        //YELLOW
		const static int MOTOR1_CHANNEL_B=43;       // WHITE
		
		const static int MOTOR2_CHANNEL_A=3;       //YELLOW  
		const static int MOTOR2_CHANNEL_B=45;       //WHITE
		int MOTOR_CHANNELS[4];
		
<<<<<<< HEAD
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
=======
		const static int NPPR=1856;             //Number of pulses per revolution
	   //}
		
	   //Rover dimensions
		const static float center_dist=0.1145;     // meters
		const static float radius=0.054;          //meters
	   //}
	   
	   //linear model param{
		//motor1
		const static float alpha1u1=0;
		const static float alpha0u1=0.17153;  //slope
		//motor2
		const static float alpha0u2=0.17153;  //slope
		const static float alpha1u2=0;
	   //}
		
>>>>>>> Fixed motion controller and pt lib
		
		const static unsigned long PERIOD_CYCLE=1000L;  // microseconds 
		const static unsigned long default_initial_speed=500L;  //microseconds
		//const static int capacity=20;  //size of path buffers
		
		#define capacity 20
		
	   //Ultrasonic sensor constants {
		const static int NUMBER_PARTITIONS_INTERVAL=1000;
		const static int ULTRASONIC_SENSOR0=26;
		const static int ULTRASONIC_SENSOR1=27;
		const static int ULTRASONIC_SENSOR2=28;
		const static int SENSE_INTERVAL=20; //microseconds
	   //}
	   
		//Direction
		const static int FORWARD=1;
		const static int BACKWARD=2;
};
#endif