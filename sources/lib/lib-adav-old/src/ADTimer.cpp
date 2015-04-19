#include "ADTimer.hpp"
#include <iostream>

#include <stdio.h>
//#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

using namespace std;
int ADTimer::received_user_stop_sig=0;
int ADTimer::stoptimer=0;

ADTimer* pTmpTimer;//for static member function access(this limits the use of one ADTimer object per linux process)
/*****************************************************************************/
ADTimer::ADTimer():millisec_time(100),passive_mode(true)//by-default it is a passive timer
{
//	stoptimer=0;
	//pTmpTimer=this;//initialize global pTmpTimer
//	received_user_stop_sig=0;
	//prepare_to_stop();
	//start_millisec_timer(millisec_time);
}
//this constructor is called when used passes the value of millisec_timer
//it means, it is an active timer receiving all signals from OS
ADTimer::ADTimer(int timer_millisec)
{
	passive_mode=false;
	stoptimer=0;
	pTmpTimer=this;//initialize global pTmpTimer
	received_user_stop_sig=0;
	millisec_time=timer_millisec;
	prepare_to_stop();
	start_millisec_timer(millisec_time);
}
/*****************************************************************************/
ADTimer::~ADTimer()
{
	if(passive_mode==false)
		stoptimer=1;
}
/*****************************************************************************/
int ADTimer::test_print(void)
{
	cout << "This is ADTimer" << endl;
	return 0;
}
int ADTimer::restart_millisec_timer(int new_millisec)
{
	if(passive_mode==true)return 0;
	millisec_time=new_millisec;
	start_millisec_timer(millisec_time);
	return 0;
}

/*****************************************************************************/
void ADTimer::apptimer_stop_handler(int sig_no)
{
	//cout<<"got signal"<<endl;
	//got_signal=1;
	switch(sig_no)
	{
		case SIGINT :cout<<"got SIGINT"<<endl;break;
		case SIGTERM:cout<<"got SIGTERM"<<endl;break;
		case SIGQUIT:cout<<"got SIGQUIT"<<endl;break;
		case SIGIO  :pTmpTimer->notify_sigio_to_subscribers();return;//normally ADSerial needs sigio to detect rx-tx interrupt
		default:return;//idont know this signal //cout<<"got unknown signal="<<sig_no<<endl;break;
	}
	received_user_stop_sig=1;//this indicates that process will stop and exit
}
int ADTimer::prepare_to_stop(void)//prepare to catch the signals
{
	/*add my handler to this signal*/
	signal(SIGINT , ADTimer::apptimer_stop_handler);
	signal(SIGTERM, ADTimer::apptimer_stop_handler);
	signal(SIGQUIT, ADTimer::apptimer_stop_handler);
	signal(SIGIO,   ADTimer::apptimer_stop_handler);
	return 0;
}
/*****************************************************************************/
//passive timer mode(someone else will call these functions)
//take external event and notify to subscribers
int ADTimer::get_100ms_heartbeat()
{
	if(passive_mode==true)
		notify_subscribers();
	return 0;
}
int ADTimer::get_sigio_event()
{
	if(passive_mode==true)
		notify_sigio_to_subscribers();
	return 0;
}
/*****************************************************************************/
int ADTimer::stop_timer()
{
	if(passive_mode==true)
		return 0;
	stoptimer=1;
	return 0;
}
void ADTimer::millisec_signal_handler(int sig_no)
{
	if(stoptimer==1)return;
	pTmpTimer->notify_subscribers();
	//cout << "millisec handler" << endl;
}
int ADTimer::start_millisec_timer(int ms)
{
	if(passive_mode==true)return 0;//dont start any timer in passive_mode

	struct sigaction sa;
	struct itimerval timer;
	// Install timer_handler as the signal handler for SIGVTALRM.
	memset (&sa, 0, sizeof (sa));
	sa.sa_handler =  ADTimer::millisec_signal_handler;//handler;//&timer_handler;
	//sigaction (SIGVTALRM, &sa, NULL);
	sigaction (SIGALRM, &sa, NULL);

	if(ms<1000)
	{
		//Configure the timer to expire after 250 msec
		timer.it_value.tv_sec = 0;
		timer.it_value.tv_usec = (ms*1000);//250000;
		// and every 250 msec after that
		timer.it_interval.tv_sec = 0;
		timer.it_interval.tv_usec = (ms*1000);//250000;
	}
	else
	{
		//Configure the timer to expire after 250 msec
		timer.it_value.tv_sec =  (ms/1000);
		timer.it_value.tv_usec = (ms%1000)*1000;
		// and every 250 msec after that
		timer.it_interval.tv_sec = (ms/1000);
		timer.it_interval.tv_usec = (ms%1000)*1000;
	}
	//Start a virtual timer. It counts down whenever this process is executing.
	//setitimer (ITIMER_VIRTUAL, &timer, NULL);
	setitimer (ITIMER_REAL, &timer, NULL);
	return 0;
}
/*****************************************************************************/
int ADTimer::wait_for_exit_signal()//forever-loop, blocks the main() app till kill signal is received
{
	if(passive_mode==true)return 0;//in passive mode, dont do this
	while (received_user_stop_sig==0)
	{
		usleep(10000);
	}
	return 0;
}
/*****************************************************************************/
//void ADTimer::millisec_signal_handler(int sig_no)
void ADTimer::custom_signal_handler(int sig, siginfo_t * info, void * context)
{
	//cout << "sig======================" << endl;
	//cout << "sig=" << sig << endl;
	//cout << "sig%%%%%%%%%%%%%%%%%%%%%%" << endl;
	pTmpTimer->notify_custom_sig_to_subscribers(sig);
}
int ADTimer::register_custom_signal(int custom_sig_num,ADTimerConsumer* pConsumer)
//int ADTimer::register_custom_signal(int custom_sig_num)
{
	struct sigaction action1;
	//sigemptyset();
	pConsumer->notify_custom_sig=1;
	action1.sa_flags = 0;
	action1.sa_flags = SA_SIGINFO ;//| SA_RESTART ;
	action1.sa_sigaction = ADTimer::custom_signal_handler;
	sigemptyset(&action1.sa_mask);
	sigaddset(&action1.sa_mask,custom_sig_num);
	return sigaction(custom_sig_num,&action1,NULL);
}
/*****************************************************************************/
void ADTimer::forced_exit()
{
	received_user_stop_sig=1;//this indicates that process will stop and exit
}
/*****************************************************************************/

