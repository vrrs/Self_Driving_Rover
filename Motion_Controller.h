#ifndef Motion_Controller_h
#define Motion_Controller_h

#include "Arduino.h"
#include <pt.h>
#include <math.h>
#include <Consts.h>
//#include <Measurements.h>
#include <Wheels_Controller.h>

/* This motion controller moves the rover in linear and curvilinear motion by 
    giving a linear path. Linear path means a set of polar coordinates.
*/

class Motion_Controller{
	private:
		//long t;
		//static const int dt
		
		//static int acceleration_thread(struct pt* ptt);
		//static int speed_control_thread(struct pt* ptt);
		
		static struct pt ptPath;
		static int path_thread(struct pt *ptt);
		
		static unsigned long calculate(float d,float angle);
		static float motor_linear_speed();
		
		static volatile unsigned long t0;
		static unsigned long set_path(float d,float angle,int k);
		
		static volatile bool end_path_option;
		
		static void end_path();
		
		static volatile int path_size;
		static volatile int i;
		static void set_motor_linear_speed(float v1,float v2);
		
	public:
		//void accelerate(long t, float f);
		static Wheels_Controller wheels;
		static Consts consts;
		//Consts consts;
		Motion_Controller();
		static volatile float theta_p[2];
		static volatile float dist_p[2];
		static volatile unsigned long path_t;
		
		bool path_activate;
		
		//void schedule_acceleration();
		//void schedule_speed_control();
		void schedule_path();	
};
#endif