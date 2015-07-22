#ifndef __ADTIMER_H_
#define __ADTIMER_H_
#include <vector>
#include <typeinfo>
#include <string>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include "ADGenericChain.hpp"
#include "ADThread.hpp"

typedef struct ADTIMER_CUSTOM_SIG_T
{
	int sig_num;
}ADTIMER_CUSTOM_SIG;
/*****************************************************************************/
//to understand this, read C++ subject observer pattern
class ADTimerProducer; //subject
class ADTimerConsumer //observer
{
public:
	int notify_custom_sig;
	virtual int timer_notification()=0;
	virtual int sigio_notification()=0;
	virtual int custom_sig_notification(int signum)=0;
	virtual ~ADTimerConsumer(){};
};
/*****************************************************************************/
class ADTimerProducer
{
	std::vector<ADTimerConsumer*> subscribers;
protected:
	void notify_subscribers()
	{
		std::vector<ADTimerConsumer*>::iterator iter;
		for(iter=subscribers.begin();iter != subscribers.end();++iter)
			(*iter)->timer_notification();//this);
	}
	void notify_sigio_to_subscribers()
	{
		std::vector<ADTimerConsumer*>::iterator iter;
		for(iter=subscribers.begin();iter != subscribers.end();++iter)
			(*iter)->sigio_notification();//this);
	}
	void notify_custom_sig_to_subscribers(int signum)
	{
		std::vector<ADTimerConsumer*>::iterator iter;
		for(iter=subscribers.begin();iter != subscribers.end();++iter)
		{
			if( (*iter)->notify_custom_sig==1 ) //send only to those who requested it
				(*iter)->custom_sig_notification(signum);
		}
	}
	
	void notify_custom_sig_to_subscribers_new(int signum)
	{
		std::vector<ADTimerConsumer*>::iterator iter;
		for(iter=subscribers.begin();iter != subscribers.end();++iter)
		{
			if( (*iter)->notify_custom_sig==1 ) //send only to those who requested it
				(*iter)->custom_sig_notification(signum);
		}
	}
	
public:
	virtual ~ADTimerProducer(){};
	void subscribe_timer_notification(ADTimerConsumer* pConsumer)
	{
		pConsumer->notify_custom_sig=0;//by disabled disable custom sig notifier(whoever wants it will enable it)
		//allow multiple subscribers(this will work if there is only one Producer Object, ex:one timer object per application)
		subscribers.push_back(pConsumer);
	}
};
/*****************************************************************************/
class ADTimer : public ADTimerProducer, public ADThreadConsumer
{
private:
	ADGenericChain custom_sig_chain;
	struct itimerval timer;
	//int custom_sig;
	sigset_t sigset;

	static int stoptimer;
	int millisec_time;
	bool passive_mode;//this timer doesnt receive any signals from OS, some-other application will send the heart-beat to this timer
	static int received_user_stop_sig;//only one copy exists
	int prepare_to_stop(void);
	static void apptimer_stop_handler(int sig_no);//only one instance of this function will exist
	static void millisec_signal_handler(int sig_no);//millisecond timer's signal handler
	int start_millisec_timer(int ms);
	static void custom_signal_handler(int sig, siginfo_t * info, void * context);
	static void custom_signal_handler_new(int sig, siginfo_t * info, void * context);
	int push_custom_sig_registration(int sig);
	int notify_registered_signals(int sig,siginfo_t * info);

	ADThread TimerThread,CustomSigThread;//worker threads
	int TimerThreadID,CustomSigThreadID;
	ADGenericChain SigInfoChain;

	//thread-callback functions
	virtual int monoshot_callback_function(void* pUserData,ADThreadProducer* pObj);
	virtual int thread_callback_function(void* pUserData,ADThreadProducer* pObj){return 0;};//we are not using this one..

public:
	ADTimer();
	ADTimer(int timer_millisec);
	~ADTimer();	
	int test_print();
	int restart_millisec_timer(int new_millisec);
	int wait_for_exit_signal();
	int stop_timer();
	int get_100ms_heartbeat();
	int get_sigio_event();
	int register_custom_signal(int custom_sig_num,ADTimerConsumer* pConsumer);
	int register_custom_signal_new(int custom_sig_num,ADTimerConsumer* pConsumer);
	
	void forced_exit();
};
/*****************************************************************************/
//note: this is how user classes can subscribe
/*
class someUserInNeedOf100msTimer: public ADTimerConsumer
{
public:
	virtual int timer_100ms_notification(ADTimerProducer* pObj)
	{
		//do whatever is needed in this 100ms timer context
		return 0;
	}
};
*/
/*****************************************************************************/

#endif
