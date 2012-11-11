#ifndef Measurements_h
#define Measurements_h

#include "Arduino.h"
#include <pt.h>
#include <Consts.h>

//assume that edges are synchronized to the same number of counts. 
class Measurements{
	private:
		volatile int cnt[2];
		static unsigned volatile long startime[2];
		
		
		void set_direction(int i,int j,int motor);
		Consts consts;
		
		static volatile bool channel_flag[2];
		static struct pt pt1,pt2;
		static int pollThread0(struct pt *ptt);
		static int pollThread1(struct pt *ptt);
		
	public:
		Measurements();
		static unsigned volatile long CURRENT_PERIOD[2];
		static volatile bool PERIOD_FLAG[2];
		
		volatile int CURRENT_DIRECTION[2];
		
		static const int CA1_INTERRUPT=0;
		static const int CA2_INTERRUPT=1;
		
		void channelA(int i,int j);
		void scheduleChannelPolling();
		
};
#endif