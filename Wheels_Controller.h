#ifndef Wheels_Controller_h
#define Wheels_Controller_h

#include "Arduino.h"
#include <pt.h>
#include <Consts.h>

class Wheels_Controller{
	private:
		
		static volatile unsigned long FREQ1[2];            
		static volatile int  PIN[2];
		static volatile bool flag[2];
		static volatile unsigned long startime[2];
		static volatile unsigned long PERIOD;
		static struct pt ptM1, ptM2;
		
		static int moveWheel0(struct pt *ptt);
		static int moveWheel1(struct pt *ptt);
		
		void reset();
		
		Consts consts;
	public:
		Wheels_Controller();
		
		static int DIRECTION;
		static bool MOVING;
		
		//operations on the wheels
		void start_moving(int direction,unsigned long m1u1,unsigned long m2u1);
		void set_freqs(unsigned long m1u1,unsigned long m2u1);
		void stop_moving();
		void set_direction(int direction);
		
		//schedule the motion on the loop() function
		void schedule_wheel_motion();
};
#endif	
	