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
		Consts consts;
		
	public:
		Measurements();
		
		//metrics and flag {
		 static unsigned volatile long CURRENT_PERIOD[2];
		 static volatile int CURRENT_DIRECTION[2];
		 static volatile bool PERIOD_FLAG[2];
		//}
		
		static const int CA1_INTERRUPT=0;
		static const int CA2_INTERRUPT=1;
		
		void start_measuring();
		void channelA(int i,int j);
};
#endif