#ifndef Measurements_h
#define Measurements_h

#include "Arduino.h"
#include <pt.h>
#include <Consts.h>

//assume that edges are synchronized to the same number of counts. 
class Measurements{
	private:
		int cnt[2];
		long startime[2];
		
		void set_direction(int i,int j,int motor);
		static Consts consts;
		
		static volatile long     		  timer0;
		static volatile long     		  timer1;
		static volatile long     		  timer2;
		static volatile bool             flag0[2];
		static volatile bool             flag1[2];
		static volatile bool             flag2[2];
		static struct pt                 ptsensor0;
		static struct pt                 ptsensor1;
		static struct pt                 ptsensor2;
		
		static int sensor0thread(struct pt *ptt); 
		static int sensor1thread(struct pt *ptt); 
		static int sensor2thread(struct pt *ptt); 
		
		bool measuring;
		
	public:
		Measurements();
		
		//metrics and flag {
		 static unsigned volatile long CURRENT_PERIOD[2];
		 static volatile int CURRENT_DIRECTION[2];
		 static volatile bool PERIOD_FLAG[2];
		//}
		
		static const int CA1_INTERRUPT=0;
		static const int CA2_INTERRUPT=1;
		static volatile bool  objflag[2];
		static volatile float objdist[2];
		void start_measuring();
		void channelA(int i,int j);
		void schedule_sensors();
		void stop_measuring();
};
#endif