#include "ADTimer.hpp"
#include <iostream>

#include <stdio.h>
//#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "ADCommon.hpp"
#include "ADJsonRpcClient.hpp"

using namespace std;
int ADTimer::received_user_stop_sig=0;
int ADTimer::stoptimer=0;

ADTimer* pTmpTimer;//for static member function access(this limits the use of one ADTimer object per linux process)

/*****************************************************************************/
ADTimer::ADTimer():millisec_time(100),passive_mode(true)//by-default it is a passive timer
{
}
//this constructor is called when used passes the value of millisec_timer
//it means, it is an active timer receiving all signals from OS
ADTimer::ADTimer(int timer_millisec,int port)
{
	notifyPortNum=port;//event notification port number
	passive_mode=false;
	stoptimer=0;
	pTmpTimer=this;//initialize global pTmpTimer
	received_user_stop_sig=0;
	millisec_time=timer_millisec;
	prepare_to_stop();
	start_millisec_timer(millisec_time);

	//initially block all signals(unblock when main thread goes into sigwait for signals in wait_for_exit_signal() function)
	//custom_sig=-1;
	sigemptyset(&sigset);
//	sigaddset(&sigset, SIGINT);
//	sigaddset(&sigset, SIGTERM);
//	sigaddset(&sigset, SIGQUIT);
//	sigaddset(&sigset, SIGIO);
//	sigaddset(&sigset, SIGALRM);
	pthread_sigmask(SIG_SETMASK , &sigset, NULL);

	//prepare signal worker monoshot threads
	TimerThreadID=TimerThread.subscribe_thread_callback(this);//remember the ID, coz we only have one callback function, and we need to differentiate
	TimerThread.set_thread_properties(THREAD_TYPE_MONOSHOT,(void *)this);
	TimerThread.start_thread();
	CustomSigThreadID=CustomSigThread.subscribe_thread_callback(this);//remember the ID, coz we only have one callback function, and we need to differentiate
	CustomSigThread.set_thread_properties(THREAD_TYPE_MONOSHOT,(void *)this);
	CustomSigThread.start_thread();
}
/*****************************************************************************/
ADTimer::~ADTimer()
{
	TimerThread.stop_thread();
	CustomSigThread.stop_thread();
	if(passive_mode==false)
		stoptimer=1;
	custom_sig_chain.remove_all();
	SigInfoChain.remove_all();
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
void ADTimer::apptimer_stop_handler(int sig_no) //not used
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
	//signal(SIGINT , ADTimer::apptimer_stop_handler);
	//signal(SIGTERM, ADTimer::apptimer_stop_handler);
	//signal(SIGQUIT, ADTimer::apptimer_stop_handler);
	//signal(SIGIO,   ADTimer::apptimer_stop_handler);
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
void ADTimer::millisec_signal_handler(int sig_no) //not used
{
	if(stoptimer==1)return;
	pTmpTimer->notify_subscribers();
	//cout << "millisec handler" << endl;
}
int ADTimer::start_millisec_timer(int ms)
{
	if(passive_mode==true)return 0;//dont start any timer in passive_mode

	//struct sigaction sa;
//	struct itimerval timer;
	// Install timer_handler as the signal handler for SIGVTALRM.
	//memset (&sa, 0, sizeof (sa));
	//sa.sa_handler =  ADTimer::millisec_signal_handler;//handler;//&timer_handler;
	//sigaction (SIGALRM, &sa, NULL);

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
//	setitimer (ITIMER_REAL, &timer, NULL);
	return 0;
}
/*****************************************************************************/
int ADTimer::wait_for_exit_signal()//forever-loop, blocks the main() app till kill signal is received
{
	if(passive_mode==true)return 0;//in passive mode, dont do this
	//while (received_user_stop_sig==0)
	//{
	//	usleep(10000);
	//}

	sigemptyset(&sigset);
	sigfillset(&sigset);
	/*sigaddset(&sigset, SIGINT);
	sigaddset(&sigset, SIGTERM);
	sigaddset(&sigset, SIGQUIT);
	sigaddset(&sigset, SIGIO);
	sigaddset(&sigset, SIGALRM);
	sigaddset(&sigset, SIGSEGV);*/
//	sigaddset(&sigset, 49);
//	sigaddset(&sigset, 55);
	//sigprocmask(SIG_BLOCK, &sigset, NULL);
	pthread_sigmask(SIG_BLOCK, &sigset, NULL);
	setitimer (ITIMER_REAL, &timer, NULL);//start the timer

	//printf("main thread going for wait loop\n");
	int result,sig;
	siginfo_t info;
	while(received_user_stop_sig==0)
	{
		//result = sigwait(&sigset, &sig);
		//if(result == 0)
		sig=sigwaitinfo(&sigset,&info);
		switch(sig)
		{
			case SIGINT :LOG_ERR_MSG("AdLib","ADTimer received SIGINT");
					received_user_stop_sig=1;
					break;
			case SIGTERM:LOG_ERR_MSG("AdLib","ADTimer received SIGTERM");
					received_user_stop_sig=1;
					break;
			case SIGQUIT:LOG_ERR_MSG("AdLib","ADTimer received SIGQUIT");
					received_user_stop_sig=1;
					break;
			case SIGIO  :notify_sigio_to_subscribers();
					break;
			case SIGALRM:   if(stoptimer!=1)
					//pTmpTimer->notify_subscribers();
					TimerThread.wakeup_thread();
					break;
			case SIGSEGV :
					LOG_ERR_MSG("AdLib","ADTimer received Sementation fault!!!!!!!!!!!!!!!!!!!!!!");
					break;
			default     :
					//printf("ADTimer::wait_for_exit_signal: handling default sig = %d\n",sig);
					if(notify_registered_signals(sig,&info)!=0)
						printf("ADTimer::wait_for_exit_signal: unregistered signal=%d\n",sig);

						//printf("SDSRV:AdLib","ADTimer received unknown sig = %d!!!!",sig);
						//LOG_ERR_MSG_WITH_ARG("SDSRV:AdLib","ADTimer received unknown sig = %d!!!!",sig);
					//printf("wait_for_exit_signal:notifying custom sig = %d info.si_int=%d\n",sig,info.si_int);
				    //if(sig==custom_sig)
					//notify_custom_sig_to_subscribers_new(info.si_int);
				    //else
					//LOG_ERR_MSG_WITH_ARG("SDSRV:AdLib","ADTimer received unknown sig = %d!!!!",sig);
				    break;
		}
	}
	//printf("exiting program\n");
	if(notifyPortNum!=-1) //notify only when user request
	{
	NOTIFY_EVENT(ADLIB_EVENT_NUM_SHUT_DOWN,-1,notifyPortNum,-1);//send shutdown event notification before shutdown
	usleep(100000);usleep(100000);usleep(100000);usleep(100000);usleep(100000);//wait 500ms for shutdown notification to be sent
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
{
//	struct sigaction action1;
	pConsumer->notify_custom_sig=1;
	pConsumer->custom_sig_num=custom_sig_num;
//	action1.sa_flags = 0;
//	action1.sa_flags = SA_SIGINFO ;//| SA_RESTART ;
//	action1.sa_sigaction = ADTimer::custom_signal_handler;
//	sigemptyset(&action1.sa_mask);
//	sigaddset(&action1.sa_mask,custom_sig_num);
//	return sigaction(custom_sig_num,&action1,NULL);

	sigaddset(&sigset, custom_sig_num);
	pthread_sigmask(SIG_SETMASK, &sigset, NULL);
	//custom_sig=custom_sig_num;
	push_custom_sig_registration(custom_sig_num);
	return 0;
}

void ADTimer::custom_signal_handler_new(int sig, siginfo_t * info, void * context)
{
	//cout << "sig======================" << endl;
	//cout << "sig=" << sig << endl;
	//cout << "sig%%%%%%%%%%%%%%%%%%%%%%" << endl;
	pTmpTimer->notify_custom_sig_to_subscribers_new(info->si_int,sig);
}

int ADTimer::register_custom_signal_new(int custom_sig_num,ADTimerConsumer* pConsumer)
{
//	struct sigaction action1;
	pConsumer->notify_custom_sig=1;
	pConsumer->custom_sig_num=custom_sig_num;
//	action1.sa_flags = 0;
//	action1.sa_flags = SA_SIGINFO ;//| SA_RESTART ;
//	action1.sa_sigaction = ADTimer::custom_signal_handler_new;
//	sigemptyset(&action1.sa_mask);
//	sigaddset(&action1.sa_mask,custom_sig_num);
//	return sigaction(custom_sig_num,&action1,NULL);

	sigaddset(&sigset, custom_sig_num);
	pthread_sigmask(SIG_SETMASK, &sigset, NULL);
	//custom_sig=custom_sig_num;
	push_custom_sig_registration(custom_sig_num);
	return 0;
}
/*****************************************************************************/
void ADTimer::forced_exit()
{
	received_user_stop_sig=1;//this indicates that process will stop and exit
}
/*****************************************************************************/
int ADTimer::push_custom_sig_registration(int sig)
{
	ADTIMER_CUSTOM_SIG* pSigReg=NULL;
	OBJECT_MEM_NEW(pSigReg,ADTIMER_CUSTOM_SIG);
	if(pSigReg == NULL)
	{
		LOG_ERR_MSG("SDSRV:ADTimer","unable create custom sig registration object");
		return -1;
	}
	pSigReg->sig_num=sig;
	if(custom_sig_chain.chain_put((void *)pSigReg)!=0)
	{
		OBJ_MEM_DELETE(pSigReg);
		LOG_ERR_MSG("SDSRV:ADTimer","unable to push custom sig registration");
		return -1;
	}
	return 0;	
}
/*****************************************************************************/
int ADTimer::notify_registered_signals(int sig,siginfo_t * info)
{
	int result=-1;
	ADTIMER_CUSTOM_SIG* pSigReg=NULL;
	int chain_size = custom_sig_chain.get_chain_size();
	custom_sig_chain.chain_lock();
	for(int i=0;i<chain_size;i++)
	{
		pSigReg=(ADTIMER_CUSTOM_SIG*)custom_sig_chain.chain_get_by_index(i);
		if(pSigReg!=NULL)
		{
			if(sig == pSigReg->sig_num)			
			{
				//push the item for worker thread to process
				ADTIMER_CUSTOM_SIG* pSigInfo=NULL;
				OBJECT_MEM_NEW(pSigInfo,ADTIMER_CUSTOM_SIG);
				if(pSigInfo != NULL)
				{
					pSigInfo->sig_num=info->si_int;//sub-signal number is needed in thread-callback
					pSigInfo->sig_extra=sig;//record main signal
					if(SigInfoChain.chain_put((void *)pSigInfo)!=0)//push-to-chain
						OBJ_MEM_DELETE(pSigInfo);
					else
						CustomSigThread.wakeup_thread();//let the working know that there is some work to do
				}
				//CustomSigThread.wakeup_thread();
				//notify_custom_sig_to_subscribers_new(info->si_int);
				result=0;//atleast one signal was matching, let the caller know this
			}
		}
	}
	custom_sig_chain.chain_unlock();
	return result;
}
/*****************************************************************************/
int ADTimer::monoshot_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	int call_from=pObj->getID();
	if(call_from == TimerThreadID )
	{
		//printf("ADTimer::monoshot_callback_function: working in timer callback\n");
		pTmpTimer->notify_subscribers();
	}
	else if(call_from == CustomSigThreadID )
	{
		ADTIMER_CUSTOM_SIG* pSigReg=NULL;
		pSigReg=(ADTIMER_CUSTOM_SIG*)SigInfoChain.chain_get();//pull-from-chain
		if(pSigReg!=NULL)
		{
			notify_custom_sig_to_subscribers_new(pSigReg->sig_num,pSigReg->sig_extra);//info->si_int
			OBJ_MEM_DELETE(pSigReg);
		}
	}
	return 0;
}
/*****************************************************************************/

