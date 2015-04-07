#include "ADThread.h"
#include <iostream>
using namespace std;
/*****************************************************************************/
int ADThreadProducer::IDGenerator = 0;//generate Unique ID for every ADThread object
/*****************************************************************************/
void *thread_function(void *thread_attr)
{
	ADThread *pThread;
	pThread=(ADThread*)thread_attr;
	int my_id=pthread_self();//get my unique id
	pThread->my_thread_func(my_id);
	pthread_exit((void*) thread_attr);
	return NULL;
}
/*****************************************************************************/
ADThread::ADThread():th_type(THREAD_TYPE_NONE),/*user_thread_func(NULL),*/user_data(NULL),init_flag(false)
{
	/* Initialize and set thread detached attribute */
	if(sem_init(&one_shot_sema,0,0)!=0)//dont wake-up thread in first shot
	{
		//printf("unable to init semaphore\n");
		return ;//-1;
	}
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	thread_state     = THREAD_STATE_INACTIVE;
	tid              = -1;

	//cout<<"ADThread:constructor"<<endl;
}
/*****************************************************************************/
ADThread::ADThread(THRD_TYPE type,/*CustomThreadFunc_t custom_func,*/void *usr_dat)
{
	th_type=type;
	//user_thread_func=custom_func;
	user_data=usr_dat;

	if(sem_init(&one_shot_sema,0,0)!=0)//dont wake-up thread in first shot
	{
		cout<<"unable to init semaphore"<<endl;
		return ;
	}
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	thread_state     = THREAD_STATE_INACTIVE;
	tid              = -1;
	init_flag=true;//thread is ready for use
	//cout<<"ADThread:constructor"<<endl;
}
/*****************************************************************************/
ADThread::~ADThread()
{
	stop_thread();
	//cout<<"ADThread:destructor"<<endl;
}
/*****************************************************************************/
//if properties are not set during constructor, use this function to set the
//values later.
int ADThread::set_thread_properties(THRD_TYPE type,/*CustomThreadFunc_t custom_func,*/void *usr_dat)
{
	th_type=type;
	//user_thread_func=custom_func;
	user_data=usr_dat;
	init_flag=true;
	return 0;
}
/*****************************************************************************/
int ADThread::test_print(void)
{
	cout << "This is ADThread" << endl;
	return 0;
}
/*****************************************************************************/
int ADThread::my_thread_func(int thread_id)
{
	tid=thread_id;
	while(thread_state==THREAD_STATE_ACTIVE)
	{
		//wait only if this is a monoshot thread
		if(th_type==THREAD_TYPE_MONOSHOT)
		{
			sem_wait(&one_shot_sema);
			if(is_user_callback_object_attached()==0)
				user_monoshot_callback_function(user_data);//call consumer
			//if(user_thread_func!=NULL)
			//  (user_thread_func)(user_data);
		}
		else
		{
			if(is_user_callback_object_attached()==0)
				user_thread_callback_function(user_data);//call consumer
			//if(user_thread_func!=NULL)
			//  (user_thread_func)(user_data);
			break;//call only once and break, actual looping happens in user_function
		}
	}
	return 0;
}
/*****************************************************************************/
int ADThread::start_thread(void)//thread* th)
{
	if(thread_state==THREAD_STATE_ACTIVE)
		return -1;//thread is already active
	if(init_flag==false)
		return -1;//properties are not initialized

	thread_state=THREAD_STATE_ACTIVE;
	if(pthread_create(&thread,&attr,thread_function,(void *)this)!=0) 
	{
		cout<<"thread could not be started"<<endl;
		return -1;
	}
	return 0;
}
/*****************************************************************************/
int ADThread::stop_thread()//thread* th)
{
	void* status;
	if(thread_state==THREAD_STATE_INACTIVE)
		return -1;//thread is already stopped, no need to stop again
	thread_state=THREAD_STATE_INACTIVE;

	if(th_type==THREAD_TYPE_MONOSHOT)
		sem_post(&one_shot_sema);//release the waiting thread in sema
	
	//destroy the attribute which was created during startup
	if(pthread_attr_destroy(&attr)!=0)
		cout<<"unable to destroy thread attribute"<<endl;
	
	if(tid!=-1)//if thread was created successfully
	{
		//printf("waiting for thread %d to stop\n",th->tid);
		if(pthread_join(thread, &status)!=0)
			cout<<"unable to stop the thread"<<endl;
		//printf(" stopped\n");
	}
	if(sem_destroy(&one_shot_sema)!=0)
		cout<<"unable to destroy semaphore!!!!!!"<<endl; /* destroy semaphore */
	return 0;
}
/*****************************************************************************/
int ADThread::wakeup_thread(void)
{
	if(th_type==THREAD_TYPE_MONOSHOT)
		sem_post(&one_shot_sema);
	return 0;
}
/*****************************************************************************/
