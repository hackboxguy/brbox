#include "ADRuntimeMinCounter.h"
#include <stdio.h>
/*****************************************************************************/
using namespace std;
/*****************************************************************************/
//this functions gets called every 100 milliseconds
int ADRuntimeMinCounter::timer_notification() //heartbeat function
{
	if(counting==false)return 0;
	ten_sec_counter+=heartbeat_ms_time;
	if(ten_sec_counter<TEN_SECONDS)
		return 0;
	ten_sec_counter=0;
	//if(ten_sec_counter++<100) //100ms*100=10sec
	//	return 0;
	//ten_sec_counter=0;

	if(get_uptime_in_seconds(&current_uptime_sec)!=0)
		return 0;
	if((current_uptime_sec-last_uptime_sec)<60)
		return 0;
	unsigned long diff=current_uptime_sec-last_uptime_sec;
	diff=diff/60;//convert to minutes
	IncreaseMinuteCount(diff);
	last_uptime_sec=current_uptime_sec;
	return 0;
}
/*****************************************************************************/
ADRuntimeMinCounter::ADRuntimeMinCounter()
{
	pMyTimer=NULL;
	ten_sec_counter=0;
	current_uptime_sec=0;
	last_uptime_sec=0;
	heartbeat_ms_time=100;
	Counter=0;//init counter by 0minutes.
	counting=false;//by default dont start counting
	ADRTC_SEMA_INIT();
}
/*****************************************************************************/
ADRuntimeMinCounter::~ADRuntimeMinCounter()
{
	counting=false;
}
/*****************************************************************************/
int ADRuntimeMinCounter::AttachHeartBeat(ADTimer* pTimer,int time_ms)
{
	if(pMyTimer!=NULL)
		return 0;//this function has already been called once.
	pMyTimer=pTimer;

	heartbeat_ms_time=time_ms;//tells frequency of this heartbeat
	pTimer->subscribe_timer_notification(this);
	return 0;
}
/*****************************************************************************/
int ADRuntimeMinCounter::StartCounting(void)
{
	if(pMyTimer==NULL)
	{
		printf("warning: HeartBeat must be attached before calling StartCounting\n");
		return -1;//heartbeat much be attached before calling this function
	}

	get_uptime_in_seconds(&last_uptime_sec);
	counting=true;
	return 0;
}
/*****************************************************************************/
int ADRuntimeMinCounter::StopCounting(void)
{
	counting=false;
	return 0;
}
/*****************************************************************************/
int ADRuntimeMinCounter::IncreaseMinuteCount(int new_val)
{
	ADRTC_SEMA_LOCK();
	Counter+=new_val;
	ADRTC_SEMA_UNLOCK();
	//printf("runtime_counter_val = %d\n",Counter);
	//time_t t = time(NULL);
	//struct tm tm = *localtime(&t);
	//printf("now: %d-%d-%d %d:%d:%d   runtime_counter_val = %d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,Counter);
	return 0;
}
/*****************************************************************************/
int ADRuntimeMinCounter::SetCounterValue(unsigned long minutes)
{
	ADRTC_SEMA_LOCK();
	Counter=minutes;
	ADRTC_SEMA_UNLOCK();
	return 0;
}
/*****************************************************************************/
unsigned long ADRuntimeMinCounter::GetCounterValue(void)
{
	unsigned long temp_count;
	ADRTC_SEMA_LOCK();
	temp_count=Counter;
	ADRTC_SEMA_UNLOCK();
	return temp_count;
}
/*****************************************************************************/
int ADRuntimeMinCounter::get_uptime_in_seconds(unsigned long* uptime)
{
	FILE *cmdline = fopen("/proc/uptime", "rb");
	char *arg = 0;
	size_t size = 0;
	double tmp_time=0.0;
	//int cpu_count;	
	while(getdelim(&arg, &size, 0, cmdline) != -1)
	{
		;
	}
	sscanf(arg,"%lf",&tmp_time);//this value is float in proc file system.
	*uptime=(unsigned long)tmp_time; 
	free(arg);
	fclose(cmdline);
	return 0;
}
/*****************************************************************************/

