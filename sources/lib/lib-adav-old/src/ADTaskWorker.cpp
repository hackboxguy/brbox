#include "ADTaskWorker.hpp"
//#include <iostream>
#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//using namespace std;
#include "ADJsonRpcClient.hpp"
/*****************************************************************************/
int ADTaskWorkerProducer::IDGenerator = 0;//generate Unique ID for every ADGenericChain object
/*****************************************************************************/
//chain-callback functions
int ADTaskWorker::identify_chain_element(void* element,int ident,ADChainProducer* pObj)
{
	int call_from=pObj->getID();
	if(call_from == work_inprog_chain_id)
	{
		WORK_CMD_TASK_IN_PROG* pPtr;
		pPtr=(WORK_CMD_TASK_IN_PROG*)element;
		if(pPtr->taskID==ident)
			return 0;
		else
			return -1;
	}
	else if(call_from == work_chain_id )
	{
		WORK_CMD_TASK* pPtr;
		pPtr=(WORK_CMD_TASK*)element;
		if(pPtr->taskID==ident)
			return 0;
		else
			return -1;
	}
	else
		return -1;
}
int ADTaskWorker::free_chain_element_data(void* element,ADChainProducer* pObj)
{
	int call_from=pObj->getID();
	if(call_from == work_chain_id )
	{
		WORK_CMD_TASK* pPtr;
		pPtr=(WORK_CMD_TASK*)element;
		//free up worker's obj(it is assumed that pWorkData doesnt have further dynamically allocated memory)
		OBJ_MEM_DELETE(pPtr->pWorkData);
	}
	return 0;
}
/*****************************************************************************/
//thread-callback functions
int ADTaskWorker::monoshot_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	WORK_CMD_TASK *work_obj=NULL;
	work_obj=(WORK_CMD_TASK*)work_chain.chain_get();
	if(work_obj!=NULL)
	{
		RPC_SRV_RESULT task_result=run_work(work_obj->command,work_obj->pWorkData);//call the consumers's function(callback)
		WORK_CMD_TASK_IN_PROG *work_inprog_obj=NULL;

		//just check if request is of type fire-and-forget(trigger)
		if(work_obj->resp_type==ADLIB_ASYNC_RESP_TYPE_TRIGGER)
			work_obj->done_action=WORK_CMD_AFTER_DONE_DELETE;

		if(work_obj->done_action==WORK_CMD_AFTER_DONE_PRESERVE)
		{
			work_inprog_chain.chain_lock();
			work_inprog_obj=(WORK_CMD_TASK_IN_PROG *)work_inprog_chain.chain_get_by_ident(work_obj->taskID);
			if(work_inprog_obj!=NULL)
			{
				work_inprog_obj->taskSts=task_result;
				work_inprog_obj->percent_complete=100;//for the moment, just say it is completed 100%
				//TODO: work_inprog_obj->task_err_message;
			}
			work_inprog_chain.chain_unlock();
			NOTIFY_EVENT(ADLIB_EVENT_NUM_INPROG_DONE,work_obj->taskID,notifyPortNum,task_result);//eventNum=0 for asyncTaskDone
		}
		else
		{
			work_inprog_obj=(WORK_CMD_TASK_IN_PROG *)work_inprog_chain.chain_remove_by_ident(work_obj->taskID);
			//work_inprog_obj=(WORK_CMD_TASK_IN_PROG *)work_inprog_chain.chain_get();
			if(work_inprog_obj!=NULL)//requested doent want to know result, hence just delete this object
			{
				//printf("ADTaskWorker::monoshot_callback_function->user doesnt want to know result, deleting object\n");
				OBJ_MEM_DELETE(work_inprog_obj);
			}
		}
		OBJ_MEM_DELETE(work_obj);//it is assumed that consumer has deleted its data(pWorkData) from work_obj;
	}
	return 0;
}
/*****************************************************************************/
ADTaskWorker::ADTaskWorker()
{
	notifyPortNum=-1;
	//initialize worker-thread and chain
	work_inprog_chain_id = work_inprog_chain.attach_helper(this);
	//work_done_chain_id   = work_done_chain.attach_helper(this);
	work_chain_id        = work_chain.attach_helper(this);
	work_thread.subscribe_thread_callback(this);
	work_thread.set_thread_properties(THREAD_TYPE_MONOSHOT,(void *)this);
	work_thread.start_thread();
}
/*****************************************************************************/
ADTaskWorker::~ADTaskWorker()
{
	work_thread.stop_thread();
	work_chain.remove_all();
	//work_done_chain.remove_all();
	work_inprog_chain.remove_all();
}
/*****************************************************************************/
/*EDISP_PANEL_RESULT ADTaskWorker::get_panel_task_status(int taskID,int* taskSts,char* errMsg)
{
	EDISP_PANEL_RESULT ret_val=EDISP_PANEL_RESULT_FAIL;
	WORK_CMD_TASK_IN_PROG *work_inprog_obj=NULL;

	work_inprog_chain.chain_lock();
	work_inprog_obj=(WORK_CMD_TASK_IN_PROG *)work_inprog_chain.chain_get_by_ident(taskID);
	if(work_inprog_obj==NULL)//no task by this ID is in my list
	{
		work_inprog_chain.chain_unlock();
		*taskSts=(int)RPC_SRV_RESULT_TASK_ID_NOT_FOUND;//RPC_SRV_RESULT_UNKNOWN;		
		return EDISP_PANEL_RESULT_SUCCESS;//RPC_SRV_RESULT_ARG_ERROR;
	}
	if(work_inprog_obj->taskSts==RPC_SRV_RESULT_IN_PROG)
	{
		work_inprog_chain.chain_unlock();
		*taskSts=(int)RPC_SRV_RESULT_IN_PROG;
		return EDISP_PANEL_RESULT_SUCCESS;
	}
	//else either works is success of fail.Hence remove the element from chain and return the status
	work_inprog_chain.chain_unlock();

	work_inprog_obj=(WORK_CMD_TASK_IN_PROG *)work_inprog_chain.chain_remove_by_ident(taskID);
	if(work_inprog_obj==NULL)//dont know why this happened, just return memory error
	{
		*taskSts=(int)RPC_SRV_RESULT_MEM_ERROR;//RPC_SRV_RESULT_SUCCESS;
		return EDISP_PANEL_RESULT_SUCCESS;
	}
	if(work_inprog_obj->taskSts==RPC_SRV_RESULT_SUCCESS)
	{
		*taskSts=(int)RPC_SRV_RESULT_SUCCESS;
		ret_val=EDISP_PANEL_RESULT_SUCCESS;
	}
	else
	{
		*taskSts=(int)work_inprog_obj->taskSts;//return the actual result//RPC_SRV_RESULT_FAIL;
		ret_val=EDISP_PANEL_RESULT_SUCCESS;
	}
	OBJ_MEM_DELETE(work_inprog_obj);
	return ret_val;
}*/
/*****************************************************************************/
//returns the status of the task given by ID
RPC_SRV_RESULT ADTaskWorker::get_task_status(int taskID,int* taskSts,char* errMsg)
{
	RPC_SRV_RESULT ret_val;
	WORK_CMD_TASK_IN_PROG *work_inprog_obj=NULL;

	work_inprog_chain.chain_lock();
	work_inprog_obj=(WORK_CMD_TASK_IN_PROG *)work_inprog_chain.chain_get_by_ident(taskID);
	if(work_inprog_obj==NULL)//no task by this ID is in my list
	{
		work_inprog_chain.chain_unlock();
		*taskSts=(int)RPC_SRV_RESULT_TASK_ID_NOT_FOUND;//RPC_SRV_RESULT_UNKNOWN;		
		return RPC_SRV_RESULT_SUCCESS;//RPC_SRV_RESULT_ARG_ERROR;
	}
	if(work_inprog_obj->taskSts==RPC_SRV_RESULT_IN_PROG)
	{
		work_inprog_chain.chain_unlock();
		*taskSts=(int)RPC_SRV_RESULT_IN_PROG;
		return RPC_SRV_RESULT_SUCCESS;
	}
	//else either works is success of fail.Hence remove the element from chain and return the status
	work_inprog_chain.chain_unlock();

	work_inprog_obj=(WORK_CMD_TASK_IN_PROG *)work_inprog_chain.chain_remove_by_ident(taskID);
	if(work_inprog_obj==NULL)//dont know why this happened, just return memory error
	{
		*taskSts=(int)RPC_SRV_RESULT_MEM_ERROR;//RPC_SRV_RESULT_SUCCESS;
		return RPC_SRV_RESULT_SUCCESS;
	}
	if(work_inprog_obj->taskSts==RPC_SRV_RESULT_SUCCESS)
	{
		*taskSts=(int)RPC_SRV_RESULT_SUCCESS;
		ret_val=RPC_SRV_RESULT_SUCCESS;
	}
	else
	{
		*taskSts=(int)work_inprog_obj->taskSts;//return the actual result//RPC_SRV_RESULT_FAIL;
		ret_val=RPC_SRV_RESULT_SUCCESS;
	}

	OBJ_MEM_DELETE(work_inprog_obj);
	return ret_val;
}
/*****************************************************************************/
//checks if specific command is in progress
int ADTaskWorker::is_command_in_progress(int cmd)
{
	return 0;
}
/*****************************************************************************/
int ADTaskWorker::push_task(int work_cmd, unsigned char* pWorkData,int *taskID,WORK_CMD_AFTER_DONE done)
{
	//PushTaskLock.chain_lock();

	WORK_CMD_TASK *work_obj=NULL;
	WORK_CMD_TASK_IN_PROG *work_inprog_obj=NULL;
	OBJECT_MEM_NEW(work_obj,WORK_CMD_TASK);

	if(work_obj==NULL)
	{
		//PushTaskLock.chain_unlock();
		return -1;
	}
	
	OBJECT_MEM_NEW(work_inprog_obj,WORK_CMD_TASK_IN_PROG);
	if(work_inprog_obj==NULL)
	{
		OBJ_MEM_DELETE(work_obj);
		//PushTaskLock.chain_unlock();
		return -1;
	}

	//id of work_inprog needs reset, hence generate ident is used from work_inprog_chain
	*taskID=work_inprog_chain.chain_generate_ident();//work_chain.chain_generate_ident();

	work_obj->taskID=*taskID;
	work_obj->percent_complete=0;//initially set the work completion percentage as 0
	work_obj->command   = work_cmd;
	work_obj->taskSts   = RPC_SRV_RESULT_IN_PROG;
	work_obj->done_action = done;
	work_obj->pWorkData = pWorkData;

	work_inprog_obj->taskID           = *taskID;
	work_inprog_obj->percent_complete = 0;
	work_inprog_obj->command          = work_cmd;
	work_inprog_obj->taskSts          = RPC_SRV_RESULT_IN_PROG;
	work_inprog_obj->task_err_message[0]='\0';//no error message yet.

	if(work_chain.chain_put((void *)work_obj)!=0)
	{
		OBJ_MEM_DELETE(work_obj);
		OBJ_MEM_DELETE(work_inprog_obj);
		printf("unable to put item in work_chain chain\n");
		//PushTaskLock.chain_unlock();
		return -1;
	}
	if(work_inprog_chain.chain_put((void *)work_inprog_obj)!=0)
	{
		work_obj=(WORK_CMD_TASK*)work_chain.chain_remove_by_ident(*taskID);
		if(work_obj!=NULL)
			OBJ_MEM_DELETE(work_obj);
		OBJ_MEM_DELETE(work_inprog_obj);
		printf("unable to put item in work_in_prog chain\n");
		//PushTaskLock.chain_unlock();
		return -1;
	}
	work_thread.wakeup_thread();//tell the worker to start working
	//PushTaskLock.chain_unlock();
	return 0;
}
/*****************************************************************************/
RPC_SRV_RESULT ADTaskWorker::reset_task_id_and_chain()
{
	work_inprog_chain.chain_empty();//chain is locked before removing items
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/


