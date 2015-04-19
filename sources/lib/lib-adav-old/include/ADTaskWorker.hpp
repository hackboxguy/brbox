#ifndef __AD_TASK_WORKER_H_
#define __AD_TASK_WORKER_H_
#include "ADCommon.hpp"
#include "ADGenericChain.hpp"
#include "ADThread.hpp"
//#include "DispPanelCmnDef.h"

//what to do with chain-element after work is finished? to be deleted or to be preserved
typedef enum WORK_CMD_AFTER_DONE_T
{
	WORK_CMD_AFTER_DONE_PRESERVE, //delete element after client reads the result
	WORK_CMD_AFTER_DONE_DELETE,   //just delete element after completion of task(caller doesnt care for result)
	WORK_CMD_AFTER_DONE_UNKNOWN,
	WORK_CMD_AFTER_DONE_NONE
}WORK_CMD_AFTER_DONE;
typedef struct WORK_CMD_TASK_T
{
	int taskID;
	int percent_complete;//progress in percentage(for future use)
	int command;//command type
	RPC_SRV_RESULT taskSts;//status(success/fail/inprogress)
	WORK_CMD_AFTER_DONE done_action;
	//unsigned int  WorkDataSize;
	unsigned char* pWorkData;//caller knows what kind of object is it.
}WORK_CMD_TASK;

typedef struct WORK_CMD_TASK_IN_PROG_T
{
	int taskID;
	int percent_complete;//progress in percentage(for future use)
	int command;//command identify to check which command is in progress.
	RPC_SRV_RESULT taskSts;
	char task_err_message[255];//incase of failed task, this message tells why task has failed.
}WORK_CMD_TASK_IN_PROG;
/*****************************************************************************/
//to understand this, read C++ subject observer pattern
class ADTaskWorkerProducer; //subject
class ADTaskWorkerConsumer //observer
{
public:
	virtual RPC_SRV_RESULT run_work(int cmd,unsigned char* pWorkData,ADTaskWorkerProducer *pTaskWorker)=0;
	virtual ~ADTaskWorkerConsumer(){};
};
/*****************************************************************************/
class ADTaskWorkerProducer
{
	static int IDGenerator;
	ADTaskWorkerConsumer *pConsumer;//consumer object where notification goes to
	int id;//id number for consumer to distinguish between many producers

protected:
	RPC_SRV_RESULT run_work(int cmd,unsigned char* pWorkData)
	{
		if(pConsumer!=NULL)
		{		
			return pConsumer->run_work(cmd,pWorkData,this);
		}
		return RPC_SRV_RESULT_UNKNOWN;
	}
public:
	ADTaskWorkerProducer() {id=IDGenerator++;pConsumer=NULL;}
	virtual ~ADTaskWorkerProducer(){};
	int attach_helper(ADTaskWorkerConsumer* c)
	{
		//allow only one Consumer to be attached		
		if(pConsumer==NULL)
		{
			pConsumer=c;
			return id;
		}
		else
		{
			return -1;//some other Consumer has already been attached
		}
	}
	int getID(){return id;}
};
/*****************************************************************************/
class ADTaskWorker: public ADTaskWorkerProducer, public ADChainConsumer, public ADThreadConsumer
{
	//ADGenericChain PushTaskLock;//chain abused just for getting the sema-lock feature

	ADThread work_thread;//thread for commands that needs longer execution time(ex: fmw-update)
	ADGenericChain work_chain;//all the commands to be executed by work_thread are in this chain.
	//ADGenericChain work_done_chain;
	ADGenericChain work_inprog_chain;
	int work_chain_id;
	//int work_done_chain_id;
	int work_inprog_chain_id;

	//Chain-callback functions	
	virtual int identify_chain_element(void* element,int ident,ADChainProducer* pObj);
	virtual int double_identify_chain_element(void* element,int ident1,int ident2,ADChainProducer* pObj){return 0;};//we are not using this one..
	virtual int free_chain_element_data(void* element,ADChainProducer* pObj);
	//thread-callback functions
	virtual int monoshot_callback_function(void* pUserData,ADThreadProducer* pObj);
	virtual int thread_callback_function(void* pUserData,ADThreadProducer* pObj){return 0;};//we are not using this one..
	/*********************************************************************/

public:
	ADTaskWorker();
	~ADTaskWorker();
	RPC_SRV_RESULT get_task_status(int taskID,int* taskSts,char* errMsg);
	//EDISP_PANEL_RESULT get_panel_task_status(int taskID,int* taskSts,char* errMsg);
	int is_command_in_progress(int cmd);
	int push_task(int work_cmd,unsigned char* pWorkData,int *taskID,WORK_CMD_AFTER_DONE done=WORK_CMD_AFTER_DONE_PRESERVE);//pushes the new work in queue and returns new taskID to the caller
	RPC_SRV_RESULT reset_task_id_and_chain();
};
#endif
