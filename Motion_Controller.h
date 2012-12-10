#ifndef Motion_Controller_h
#define Motion_Controller_h

#include "Arduino.h"
#include <pt.h>
#include <math.h>
#include <Consts.h>
#include <Measurements.h>
#include <Wheels_Controller.h>

/* This motion controller moves the rover in linear and curvilinear motion by 
    giving a linear path. Linear path means a set of polar coordinates.
*/

class Motion_Controller{
	private:
		
		static int accelerate_thread(struct pt* ptt);
		static void add_motor_speed(float dv);

		static struct pt ptPath;
		static struct pt ptSpeed;
		static struct pt ptAccelerate;
		static int path_thread(struct pt *ptt);
		
		static unsigned long calculate(float d,float angle);
		static float motor_linear_speed();
		
		static volatile unsigned long t0;
		static volatile unsigned long tt0;
		static volatile unsigned long dt;
		static volatile int n;
		static volatile float dv;
		
		static unsigned long set_path(float d,float angle,int k);
		
		static volatile bool end_path_option;
		
		static volatile bool accelerate_activate;
		
		static void end_path();
		
		static volatile int path_size;
		static volatile int i;
		static void set_motor_linear_speed(float v1,float v2);
		static int speed_control_thread(struct pt* ptt);
		
	public:
		Motion_Controller();
		
		//Software layers
		static Wheels_Controller wheels;
		static Consts consts;
		static Measurements measurements;
		
		static volatile float theta_p[5];
		static volatile float dist_p[5];
		static volatile unsigned long path_t;
		
		//control flags. Fire up the controls.
		bool path_activate;
		bool speed_control_activated;
		
		//operations on the motion
		static void accelerate(unsigned long t,float vf);
		
		//schedulers
		void schedule_path();	
		void schedule_speed_control();
		void schedule_acceleration();
};
#endif