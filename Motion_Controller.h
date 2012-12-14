#ifndef Motion_Controller_h
#define Motion_Controller_h

#include "Arduino.h"
#include <pt/pt.h>
#include <math.h>
#include <Consts.h>
#include <Measurements.h>
#include <Wheels_Controller.h>

/* This motion controller moves the rover in linear and curvilinear motion by 
    giving a linear path. Linear path means a set of polar coordinates.
*/

class Motion_Controller{
	private:

		static struct pt ptPath;
		static struct pt ptSpeed;
		static struct pt ptAccelerate;
		
		static int path_thread(struct pt *ptt);
		static int speed_control_thread(struct pt* ptt);
		static int accelerate_thread(struct pt* ptt);
		
		static void add_motor_speed(float dv);
		static unsigned long calculate(float d,float angle);
		
		static float motor_linear_speed();
		static void set_motor_linear_speed(float v1,float v2);
		
		static volatile unsigned long t0;
		static volatile unsigned long tt0;
		static volatile unsigned long dt;
		static volatile int n;
		static volatile float dv;
		
		
		static volatile bool accelerate_activate;
		
<<<<<<< HEAD
		static volatile int path_size;
		static volatile int i;
		
		static volatile bool speed_changed;
=======
		static volatile int back;
		static volatile int front;
		
		static volatile bool speed_changed;
		bool time_flag;
		
		//control flags. Fire up the controls.
		static volatile bool path_activate;
		bool speed_control_activated;
		
		static void deltaFront(bool increment);
>>>>>>> Fixed motion controller and pt lib
		
	public:
		Motion_Controller();
		
		//Software layers
		static Wheels_Controller wheels;
		static Consts consts;
		static Measurements measurements;
<<<<<<< HEAD
		
		static volatile float theta_p[5];
		static volatile float dist_p[5];
		static volatile unsigned long path_t;
		
		//control flags. Fire up the controls.
		bool path_activate;
		bool speed_control_activated;
		
=======
		
		static volatile float theta_p[capacity];
		static volatile float dist_p[capacity];
		static volatile unsigned long path_t;
		
		static int size();
		static bool full();
	
>>>>>>> Fixed motion controller and pt lib
		//operations on the motion
		static void accelerate(unsigned long t,float vf);
		void move(bool regulate_speed);
		static void stop();
		
		//schedulers
		void schedule_path();	
		void schedule_speed_control();
		void schedule_acceleration();
};
#endif