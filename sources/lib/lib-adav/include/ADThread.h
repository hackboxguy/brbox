#ifndef __ADTHREAD_H_
#define __ADTHREAD_H_
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

class ADThreadProducer; //subject
class ADThreadConsumer //observer
{
public:
	virtual int monoshot_callback_function(void* pUserData,ADThreadProducer* pObj)=0;//called on every sema-wakeup
	virtual int thread_callback_function(void* pUserData,ADThreadProducer* pObj)=0;//called only once and then thread exits
	virtual ~ADThreadConsumer(){};
};
class ADThreadProducer
{
	static int IDGenerator;
	ADThreadConsumer *pConsumer;//consumer object where notification goes to
	int id;//id number for consumer to distinguish between many producers

protected:
	int user_monoshot_callback_function(void* pUserData)
	{
		if(pConsumer!=NULL)		
			return pConsumer->monoshot_callback_function(pUserData,this);
		return -1;
	}
	int user_thread_callback_function(void* pUserData)
	{
		if(pConsumer!=NULL)		
			return pConsumer->thread_callback_function(pUserData,this);
		return -1;
	}

	int is_user_callback_object_attached(void)
	{
		if(pConsumer==NULL)		
			return -1;
		return 0;
	}
public:
	ADThreadProducer() {/*IDGenerator++;*/id=IDGenerator++;pConsumer=NULL;}
	virtual ~ADThreadProducer(){};
	int subscribe_thread_callback(ADThreadConsumer* c)
	{
		//allow only one Consumer to be attached		
		if(pConsumer==NULL)
		{
			pConsumer=c;
			return id;
			//id=id_num;//id number for helper to distinguish between many producers
		}
		else
			return -1;//some other Consumer has already been attached
	}
	int getID(){return id;}
};
/*****************************************************************************/
//typedef int (*CustomThreadFunc_t) (void* data);
typedef enum THRD_STATE_T
{
	THREAD_STATE_INACTIVE,
	THREAD_STATE_ACTIVE,
	THREAD_STATE_NONE
}THRD_STATE;
typedef enum THRD_TYPE_T
{
	THREAD_TYPE_MONOSHOT,
	THREAD_TYPE_NOBLOCK,
	THREAD_TYPE_NONE
}THRD_TYPE;
class ADThread : public ADThreadProducer
{
	THRD_TYPE th_type;
	//CustomThreadFunc_t user_thread_func;
	void *user_data;
	bool init_flag;
	THRD_STATE thread_state;
	int tid;
	pthread_t thread;
	pthread_attr_t attr;
	sem_t one_shot_sema;


	public:
	ADThread();
	ADThread(THRD_TYPE type,/*CustomThreadFunc_t custom_func,*/void *usr_dat);
	~ADThread();	
	//virtual void receive_data() = 0;
	int set_thread_properties(THRD_TYPE type,/*CustomThreadFunc_t custom_func,*/void *usr_dat);
	int start_thread(void);
	int test_print();
	int my_thread_func(int thread_id);
	int stop_thread();
	int wakeup_thread(void);
};

#endif
