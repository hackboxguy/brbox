#ifndef __AD_RUNTIME_MIN_COUNTER_H_
#define __AD_RUNTIME_MIN_COUNTER_H_
/*****************************************************************************/
//ADRuntimeMinCounter is a reference class designed to handle various run times
//used in any device. ex: LED_ON runtime, projector_runtime etc..
//counting resolution of the this class is in minutes, i.e counted runtime 
//returned by this class is in minutes.
/*****************************************************************************/
#include <pthread.h>    /* POSIX Threads */
#include <stdlib.h>
#include "ADTimer.h"
#define ADRTC_SEMA_INIT()		 \
	do				 \
	{				 \
		pthread_mutex_init(&sema,NULL);\
	} while (0)

#define ADRTC_SEMA_LOCK()			 \
	do				 \
	{				 \
	pthread_mutex_lock( &sema );\
	} while (0)

#define ADRTC_SEMA_UNLOCK()			 \
	do				 \
	{				 \
	 pthread_mutex_unlock( &sema );\
	} while (0)

#define TEN_SECONDS 10000

class ADRuntimeMinCounter: public ADTimerConsumer
{
	pthread_mutex_t sema;//lock for runtime-counter
	unsigned long Counter;//counter
	bool counting;//flag to indicate if counting is in progress
	int heartbeat_ms_time;
	unsigned long current_uptime_sec;
	unsigned long last_uptime_sec;
	int ten_sec_counter;
	ADTimer* pMyTimer;
	virtual int sigio_notification(){return 0;};
	virtual int timer_notification();
	virtual int custom_sig_notification(int signum){return 0;};
	int IncreaseMinuteCount(int new_val=1);
	int get_uptime_in_seconds(unsigned long* uptime);
public:
	ADRuntimeMinCounter();
	~ADRuntimeMinCounter();
	int AttachHeartBeat(ADTimer* pTimer,int time_ms);
	int StartCounting(void);
	int StopCounting(void);
	int SetCounterValue(unsigned long minutes);
	unsigned long GetCounterValue(void);
};
#endif
