#ifndef __ADCHAIN_H_
#define __ADCHAIN_H_
#include <pthread.h>    /* POSIX Threads */
#include <stdlib.h>
#define SEMA_INIT()			 \
	do				 \
	{				 \
		pthread_mutex_init(&sema,NULL);\
	} while (0)

#define SEMA_LOCK()			 \
	do				 \
	{				 \
	pthread_mutex_lock( &sema );\
	} while (0)


#define SEMA_UNLOCK()			 \
	do				 \
	{				 \
	 pthread_mutex_unlock( &sema );\
	} while (0)
/*****************************************************************************/
//important Note:
//ADGenericChain in intended for storing/removing data structure of anytype
//it uses a linked list mechanism for managing user data, and this user data is
//typically dynamically allocated memory.
//ADGenericChain stores the user data pointer as void*, dont use this for
//storing class object(for storing class objects, use vector library class)
/*****************************************************************************/
class ADChainProducer; //subject
class ADChainConsumer //observer
{
public:
	virtual int identify_chain_element(void* element,int ident,ADChainProducer* pObj)=0;
	virtual int double_identify_chain_element(void* element, int ident1,int ident2,ADChainProducer* pObj)=0;
	virtual int free_chain_element_data(void*,ADChainProducer* pObj)=0;
	virtual ~ADChainConsumer(){};
};
/*****************************************************************************/
class ADChainProducer
{
	static int IDGenerator;
	ADChainConsumer *pConsumer;//consumer object where notification goes to
	int id;//id number for consumer to distinguish between many producers

protected:
	int identify_chain_element(void* element,int ident)
	{
		if(pConsumer!=NULL)		
			return pConsumer->identify_chain_element(element,ident,this);
		return -1;
	}
	int double_identify_chain_element(void* element, int ident1,int ident2)
	{
		if(pConsumer!=NULL)		
			return pConsumer->double_identify_chain_element(element,ident1,ident2,this);
		return -1;
	}
	int free_chain_element_data(void* element)
	{
		if(pConsumer!=NULL)		
			return pConsumer->free_chain_element_data(element,this);
		return 0;
	}
	int is_helper_attached(void)
	{
		if(pConsumer==NULL)		
			return -1;
		return 0;
	}
public:
	ADChainProducer() {/*IDGenerator++;*/id=IDGenerator++;pConsumer=NULL;}
	//ADChainProducer(ADChainConsumer* c,int id_num) {pConsumer=c;id=id_num;}
	virtual ~ADChainProducer(){};
	int attach_helper(ADChainConsumer* c)//,int id_num)
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
//an example of helper function
/*
class MyChainHelper : public ADChainConsumer //observer
{
	public:
		MyChainHelper(){}
		~MyChainHelper(){}
		virtual int identify_chain_element(void* element,int ident)
		{
			//if(ident==2)return 0;
			//else return -1;
		}
		virtual int double_identify_chain_element(void* element,int ident1,int ident2)
		{
			//if(ident1==2 && ident2==1)return 0;
			//else return -1;
		}
		virtual int free_chain_element_data(void* element)
		{
			//check if this element holds other dynamically allocated pointers
			return 0;
		}
};
*/
/*****************************************************************************/

/*****************************************************************************/
struct chain_holder
{
	void *pData;
	struct chain_holder * pPrev;
	struct chain_holder * pNext;
};
//typedef int  (*datIdfunc_t)(void* data,int ident);
//typedef int  (*dbdatIdfunc_t)(void* data,int ident1,int ident2);
//typedef int  (*DataRemoverFunc_t) (void* data);


class ADGenericChain : public ADChainProducer
{
	int disable_autoremove;//do not free elements

	struct chain_holder base_chain;
	int chain_peak_size;
	int chain_size;
	int ident_generator;
	bool init_flag;
	//DataRemoverFunc_t custom_data_remover;
	pthread_mutex_t sema;

	int remove();
	public:
	ADGenericChain();
	//ADChain();//DataRemoverFunc_t remover);
	~ADGenericChain();	
	void chain_lock();
	void chain_unlock();

	int chain_put(void *data);
	void* chain_get();

	int chain_add(void *data);
	int chain_remove( void );

	void *chain_get_by_ident(int ident);//, datIdfunc_t custom_func);
	void *chain_remove_by_ident(int ident);//,datIdfunc_t custom_func);
	void* chain_remove_by_double_ident(int ident1,int ident2);//,dbdatIdfunc_t cust_func);
	void* chain_get_by_double_ident(int ident1,int ident2);//,dbdatIdfunc_t cust_func);
	void *chain_get_by_index(int index);
	void* get_top();
	int get_chain_size(void);
	int chain_generate_ident(void);

	//virtual void receive_data() = 0;
	int test_print();
	//int add_remover(DataRemoverFunc_t remover);
	int remove_all();//doesnt lock before cleaning the elements
	int chain_empty();//does the locking before cleaning the elements
	int disable_auto_remove();
	int enable_auto_remove();

};

#endif
