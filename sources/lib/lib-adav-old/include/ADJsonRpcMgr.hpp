#ifndef __ADJSON_RPC_H_
#define __ADJSON_RPC_H_
#include <vector>
#include <typeinfo>
#include <string>
#include <iostream>
#include <stdint.h>
#include "ADCommon.hpp"
#include "JsonCmnDef.h"
#include "ADJsonRpcMapper.hpp"
#include "ADTimer.hpp"
#include "ADTaskWorker.hpp"
#include "ADCmnStringProcessor.hpp"
#include "ADCmnDevTypes.h"
#include "ADEvntMgr.hpp"
using namespace std;
/* ------------------------------------------------------------------------- */
// following commands are common rpc's when a server uses this rpc-manager
typedef enum EJSON_RPCGMGR_CMD_T
{
	EJSON_RPCGMGR_GET_TASK_STS=0,	     //read the task-status of a rpc which is in progress
	EJSON_RPCGMGR_GET_RPC_SRV_VERSION=1, //read the rpc-server version
        EJSON_RPCGMGR_TRIGGER_DATASAVE=2,    //triggers the data savefunction of rpc-server(not to be confused with saveall)
        EJSON_RPCGMGR_GET_SETTINGS_STS=3,    //reads the state of rpc-server-settings
        EJSON_RPCGMGR_SHUTDOWN_SERVICE=4,    //rpc-server shotdown cmd
	EJSON_RPCGMGR_RESET_TASK_STS=5,      //reset task-status chain(warning: ensure that no task is in progress when resetting)
	EJSON_RPCGMGR_GET_READY_STS=6,       //ready status of the service
	EJSON_RPCGMGR_GET_DEBUG_LOG=7,       //read the  status of debug logging flag
	EJSON_RPCGMGR_SET_DEBUG_LOG=8,       //enable/disable debug logging
	//EJSON_RPCGMGR_GET_DEV_TYPE=9,      //read device type
	EJSON_RPCGMGR_TRIGGER_FACTORY_STORE=9,   //read current rpc-cache-settings, and write to factory.data.file
	EJSON_RPCGMGR_TRIGGER_FACTORY_RESTORE=10,//read factory.data.file and apply to current rpc-cache-settings
	EJSON_RPCGMGR_EVENT_SUBSCRIBE=11,    //subscribe for the event with service(on successfull subscription, an ident is returned)
	EJSON_RPCGMGR_EVENT_UNSUBSCRIBE=12,  //unsubscribe the event notification from service(ident is required to unsubscribe)
	EJSON_RPCGMGR_EVENT_NOTIFY=13,       //event notification; for self use within service as local-host rpc client caller
	EJSON_RPCGMGR_EVENT_PROCESS=14,      //event reception and processing from other service
	EJSON_RPCGMGR_CMD_END,
	EJSON_RPCGMGR_CMD_NONE
}EJSON_RPCGMGR_CMD;
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_GET_TASK_STS
#define RPCMGR_RPC_TASK_STS_GET         "get_rpc_req_status"
#define RPCMGR_RPC_TASK_STS_ARGID       "taskId"
#define RPCMGR_RPC_TASK_STS_ARGSTS      "taskStatus"
//standard integer data communication packet
typedef struct RPCMGR_TASK_STS_PACKET_T
{
	int taskID;
	char task_sts[512];//inProg/Success/Fail
}RPCMGR_TASK_STS_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_GET_RPC_SRV_VERSION
#define RPCMGR_RPC_SER_VER_GET          "get_rpc_srv_version"
#define RPCMGR_RPC_SER_VER_ARGVER       "version"
//standard integer data communication packet
typedef struct RPCMGR_INTEGER_PACKET_T
{
	int value;
}RPCMGR_INTEGER_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_TRIGGER_DATASAVE
#define RPCMGR_RPC_TRIG_SAVE            "trigger_settings_save"
#define RPCMGR_RPC_TRIG_SAVE_ARGID      RPCMGR_RPC_TASK_STS_ARGID
//#define RPCMGR_RPC_TRIG_SAVE_ARGSTS   RPCMGR_RPC_TASK_STS_ARGSTS
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_GET_SETTINGS_STS
#define RPCMGR_RPC_STTNG_STS_GET        "get_settings_status"
#define RPCMGR_RPC_STTNG_STS_ARGSTS     "status"
typedef struct RPCMGR_SETTINGS_STS_PACKET_T
{
	char status[512];
}RPCMGR_SETTINGS_STS_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_SHUTDOWN_SERVICE
#define RPCMGR_RPC_TRIG_SHUTDOWN        "trigger_shutdown"
#define RPCMGR_RPC_TRIG_SHUTDOWN_ARGID  RPCMGR_RPC_TASK_STS_ARGID
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_RESET_TASK_STS
#define RPCMGR_RPC_RESET_TASKSTS        "reset_task_status"
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_GET_READY_STS
#define RPCMGR_RPC_READY_STS_GET        "get_ready_status"
#define RPCMGR_RPC_READY_STS_ARGSTS     "status"
#define RPCMGR_RPC_READY_STS_ARGSTS_TBL {"notready","ready","busy","unknown","none","\0"}
typedef enum EJSON_RPCGMGR_READY_STATE_T
{
	EJSON_RPCGMGR_READY_STATE_NOT_READY,
	EJSON_RPCGMGR_READY_STATE_READY,
	EJSON_RPCGMGR_READY_STATE_BUSY,
	EJSON_RPCGMGR_READY_STATE_UNKNOWN,
	EJSON_RPCGMGR_READY_STATE_NONE
}EJSON_RPCGMGR_READY_STATE;
typedef struct RPCMGR_READY_STS_PACKET_T
{
	EJSON_RPCGMGR_READY_STATE status;
	char status_str[512];
}RPCMGR_READY_STS_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_GET_DEBUG_LOG
//EJSON_RPCGMGR_SET_DEBUG_LOG
#define RPCMGR_RPC_DEBUG_LOG_GET        "get_debug_logging"
#define RPCMGR_RPC_DEBUG_LOG_SET        "set_debug_logging"
#define RPCMGR_RPC_DEBUG_LOG_ARGSTS     "status"
#define RPCMGR_RPC_DEBUG_LOG_ARGSTS_TBL {"disable","enable","unknown","none","\0"}
typedef enum EJSON_RPCGMGR_FLAG_STATE_T  //generic enable/disable flag type
{
	EJSON_RPCGMGR_FLAG_STATE_DISABLE,
	EJSON_RPCGMGR_FLAG_STATE_ENABLE,
	EJSON_RPCGMGR_FLAG_STATE_UNKNOWN,
	EJSON_RPCGMGR_FLAG_STATE_NONE
}EJSON_RPCGMGR_FLAG_STATE;
typedef struct RPCMGR_DEBUG_LOG_PACKET_T
{
	EJSON_RPCGMGR_FLAG_STATE status;
	//char status_str[512];
}RPCMGR_DEBUG_LOG_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_GET_DEV_TYPE
#define RPCMGR_RPC_DEV_TYPE_GET        "get_device_type"
#define RPCMGR_RPC_DEV_TYPE_ARG        "type"
typedef struct RPCMGR_DEV_TYPE_PACKET_T
{
	//EJSON_RPCGMGR_READY_STATE status;
	char status_str[512];
}RPCMGR_DEV_TYPE_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_TRIGGER_FACTORY_STORE=9,   //read current rpc-cache-settings, and write to factory.data.file
#define RPCMGR_RPC_TRIG_FACT_STORE           "trigger_factory_store"
#define RPCMGR_RPC_TRIG_FACT_STORE_ARGID     RPCMGR_RPC_TASK_STS_ARGID
//EJSON_RPCGMGR_TRIGGER_FACTORY_RESTORE=10,//read factory.data.file and apply to current rpc-cache-settings
#define RPCMGR_RPC_TRIG_FACT_RESTORE         "trigger_factory_restore"
#define RPCMGR_RPC_TRIG_FACT_RESTORE_ARGID   RPCMGR_RPC_TASK_STS_ARGID
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_EVENT_SUBSCRIBE
//EJSON_RPCGMGR_EVENT_UNSUBSCRIBE
//EJSON_RPCGMGR_EVENT_NOTIFY
//EJSON_RPCGMGR_EVENT_PROCESS
//note: json string definition moved to ADEvntMgr.hpp
typedef struct RPCMGR_EVENT_PACKET_T
{
	int cltToken; //event receiver wants to know who sent the event
	int portNum;  //port number where events to be sent.
	int eventNum; //if event-num is -1=then, notify all events
	int srvToken; //subscription token(every subscriber will get a token-number)
	int sock_descr;//socket descriptor(incase of event to be notified on same connection)
	int sock_id;  //unique client connection ident(needed in case of client connection breaks)
	char ip[512];
}RPCMGR_EVENT_PACKET;
/* ------------------------------------------------------------------------- */
//to understand this, read C++ subject observer pattern
class ADJsonRpcMgrProducer; //subject
class ADJsonRpcMgrConsumer  //observer
{
//protected:
//	std::string strGetRpc;
//	std::string strSetRpc;
	bool emulation;
	bool logmsg;
public:
	int parent_index;
	int index;
	std::string strRpcName;

	//std::string strSetRpc;

	std::string GetRpcName(){return strRpcName;};
	int GetRpcParentIndex(){return parent_index;};
	bool get_debug_log_flag(){return logmsg;};
	int  set_debug_log_flag(bool flag){logmsg=flag;return 0;};
	bool get_emulation_flag(){return emulation;};

	//std::string GetRpcNameSetType(){return strGetRpc;};
	//std::string GetPageTag(){return Tag;};
	//std::string GetPageOverrideName(){return OvrName;};
	//int GetMenuInfo(mnuTypes_t &menuType,std::string &menuName,std::string &menuTag)//,std::string &NameOveride="none")
	//{
	//	menuType=Menu;
	//	menuName=Name;
	//	menuTag=Tag;
	//	return 0;
	//};
	//ADJsonRpcMgrConsumer(std::string getrpc,std::string setrpc){strGetRpc=getrpc;strSetRpc=setrpc;};
	ADJsonRpcMgrConsumer(std::string rpc_name,int consumer_index,bool emu,bool log)
	{
		strRpcName=rpc_name;
		index=consumer_index;
		emulation=emu;
		logmsg=log;
	};
	virtual ~ADJsonRpcMgrConsumer(){};
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index)=0;
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index)=0;
	//virtual int WorkRpc(RPC_SRV_REQ *pPanelReq/*data-cache?*/)=0;
	virtual int ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj)=0;
	virtual RPC_SRV_RESULT ProcessWorkAsync(int index,unsigned char* pWorkData)=0;

};
/* ------------------------------------------------------------------------- */
class ADJsonRpcMgrProducer
{
	std::vector<ADJsonRpcMgrConsumer*> rpclist;
protected:
	ADTaskWorker AsyncTaskWorker;
	ADEvntMgr EventMgr;

	ADJsonRpcMgrConsumer* getRpcHandler(std::string rpcName) //based on rpc-name string, returns the object ptr
	{
		std::vector<ADJsonRpcMgrConsumer*>::iterator iter;
		for(iter=rpclist.begin();iter != rpclist.end();++iter)
		{
			if((*iter)->GetRpcName() == rpcName)
				return (*iter);
			//else if((*iter)->GetRpcNameSetType() == rpcName)
			//	return (*iter);
		}
		return NULL;
	}
	ADJsonRpcMgrConsumer* getRpcHandler(int index) //based on index, returns the object ptr
	{
		std::vector<ADJsonRpcMgrConsumer*>::iterator iter = rpclist.begin() + index;
		return (*iter);
	}
	ADJsonRpcMgrConsumer* getRpcHandlerByParentIndex(int index) //based on index, returns the object ptr
	{
		std::vector<ADJsonRpcMgrConsumer*>::iterator iter;
		for(iter=rpclist.begin();iter != rpclist.end();++iter)
		{
			if((*iter)->GetRpcParentIndex() == index)
				return (*iter);
		}
		return NULL;
	}
	int setRpcDebugLogFlag(bool debuglog) //set debuglog flag to all attached rpc's
	{
		std::vector<ADJsonRpcMgrConsumer*>::iterator iter;
		for(iter=rpclist.begin();iter != rpclist.end();++iter)
			(*iter)->set_debug_log_flag(debuglog);
		return 0;
	}
public:
	virtual ~ADJsonRpcMgrProducer(){};
	void AttachRpc(ADJsonRpcMgrConsumer* pRpc)
	{
		rpclist.push_back(pRpc);
	}
	int get_total_attached_rpcs()
	{
		return rpclist.size();
	}
	int MapJsonToBinary(JsonDataCommObj* pReq)
	{
		if((pReq->cmd_index-EJSON_RPCGMGR_CMD_END)>=get_total_attached_rpcs())
			return -1;//not my call, i dont have an object of this index
		if((pReq->cmd_index-EJSON_RPCGMGR_CMD_END)<0)
			return -1;//something is wrong
		ADJsonRpcMgrConsumer* pPageHandler = getRpcHandler(pReq->cmd_index-EJSON_RPCGMGR_CMD_END);
		if(pPageHandler==NULL)
			return -1;
		else
			return pPageHandler->MapJsonToBinary(pReq,pPageHandler->index);
	}
	int MapBinaryToJson(JsonDataCommObj* pReq)
	{
		if((pReq->cmd_index-EJSON_RPCGMGR_CMD_END)>=get_total_attached_rpcs())
			return -1;//not my call, i dont have an object of this index
		if((pReq->cmd_index-EJSON_RPCGMGR_CMD_END)<0)
			return -1;//something is wrong
		ADJsonRpcMgrConsumer* pPageHandler = getRpcHandler(pReq->cmd_index-EJSON_RPCGMGR_CMD_END);
		if(pPageHandler==NULL)
			return -1;
		else
			return pPageHandler->MapBinaryToJson(pReq,pPageHandler->index);//pass rpc's own index
	}
	//int WorkRpc(RPC_SRV_REQ *pPanelReq)
	int ProcessWork(JsonDataCommObj* pReq)
	{
		if((pReq->cmd_index-EJSON_RPCGMGR_CMD_END)>=get_total_attached_rpcs())
			return -1;//not my call, i dont have an object of this index
		if((pReq->cmd_index-EJSON_RPCGMGR_CMD_END)<0)
			return -1;//something is wrong
		ADJsonRpcMgrConsumer* pPageHandler = getRpcHandler(pReq->cmd_index-EJSON_RPCGMGR_CMD_END);
		if(pPageHandler==NULL)
			return -1;
		else
			return pPageHandler->ProcessWork(pReq,pPageHandler->index,this);
	}
	RPC_SRV_RESULT AsyncTaskWork(int cmd,unsigned char* pWorkData)
	{
		//ADJsonRpcMgrConsumer* pPageHandler=getRpcHandlerByParentIndex(cmd);//get the attached object pointer by parent_index)
		//if(pPageHandler == NULL)
		//	return RPC_SRV_RESULT_FAIL;
		//if(pPageHandler->ProcessWorkAsync(0,pWorkData)==0)//TODO: handle correct return value
		//	return RPC_SRV_RESULT_SUCCESS;
		//else
		//	return RPC_SRV_RESULT_FAIL;
		if((cmd-EJSON_RPCGMGR_CMD_END)>=get_total_attached_rpcs())
			return RPC_SRV_RESULT_FAIL;//not my call, i dont have an object of this index
		if((cmd-EJSON_RPCGMGR_CMD_END)<0)
			return RPC_SRV_RESULT_FAIL;//something is wrong
		ADJsonRpcMgrConsumer* pPageHandler = getRpcHandler(cmd-EJSON_RPCGMGR_CMD_END);
		if(pPageHandler==NULL)
			return RPC_SRV_RESULT_FAIL;
		else
		{
			return pPageHandler->ProcessWorkAsync(cmd-EJSON_RPCGMGR_CMD_END,pWorkData);
			//if(pPageHandler->ProcessWorkAsync(cmd-EJSON_RPCGMGR_CMD_END,pWorkData)==0)
			//	return RPC_SRV_RESULT_SUCCESS;
			//else
			//	return RPC_SRV_RESULT_FAIL;
		}
	}
	RPC_SRV_RESULT PushAsyncTask(int cmd,unsigned char* pWorkData,int *taskID,WORK_CMD_AFTER_DONE done_flag)
	{
		//TODO: add my own cmd offset to 'cmd' parameter
		if(AsyncTaskWorker.push_task(cmd+EJSON_RPCGMGR_CMD_END,pWorkData,taskID,done_flag)==0)
			//pReq->result=RPC_SRV_RESULT_IN_PROG;
			return RPC_SRV_RESULT_IN_PROG;
		else
			//pReq->result=RPC_SRV_RESULT_FAIL;
			return RPC_SRV_RESULT_FAIL;
	}

};
/* ------------------------------------------------------------------------- */
class ADJsonRpcMgr : public ADJsonRpcMgrProducer, public ADJsonRpcMapConsumer, public ADTaskWorkerConsumer, public ADTimerConsumer, public ADCmnStringProcessor
{
	ADTimer* myTimer;
	ADJsonRpcProxy  Proxy;//json-net-proxy
	ADJsonRpcMapper	JMapper;
	int svnVersion;
	ADCMN_DEV_INFO* pDevInfo;

	//ADTaskWorker AsyncTaskWorker;
	bool shutdown_support;
	EJSON_RPCGMGR_READY_STATE ServiceReadyFlag;
	//EJSON_RPCGMGR_FLAG_STATE  ServiceDebugFlag;
	bool  ServiceDebugFlag;

	//ADJsonRpcMapConsumer overrides
	virtual int process_json_to_binary(JsonDataCommObj* pReq);//{return 0;};//only applicable for mapper
	virtual int process_binary_to_json(JsonDataCommObj* pReq);//{return 0;};//only applicable for mapper
	virtual int process_work(JsonDataCommObj* pReq);//{return 0;};

	//ADTaskWorkerConsumer override
	virtual RPC_SRV_RESULT run_work(int cmd,unsigned char* pWorkData,ADTaskWorkerProducer *pTaskWorker);//{return RPC_SRV_RESULT_SUCCESS;};

	//ADTimerConsumer overrides: 100ms timer and sigio 
	virtual int sigio_notification(){return 0;};
	virtual int timer_notification(){return 0;};
	virtual int custom_sig_notification(int signum){return 0;};

	int MyMapJsonToBinary(JsonDataCommObj* pReq);
	int MyMapBinaryToJson(JsonDataCommObj* pReq);
	int MyProcessWork(JsonDataCommObj* pReq);
	int create_req_obj(JsonDataCommObj* pReq);
	int delete_req_obj(JsonDataCommObj* pReq);
	//int prepare_req_object(JsonDataCommObj* pReq,RPC_SRV_ACT action,int cmd);

	//EJSON_RPCGMGR_GET_TASK_STS
	int json_to_bin_get_task_sts(JsonDataCommObj* pReq);
	int process_get_task_status(RPC_SRV_REQ* pReq);
	int bin_to_json_get_task_sts(JsonDataCommObj* pReq);

	//EJSON_RPCGMGR_GET_RPC_SRV_VERSION
	int json_to_bin_get_rpc_srv_version(JsonDataCommObj* pReq);
	int process_rpc_server_version(RPC_SRV_REQ* pReq);
	int bin_to_json_get_rpc_srv_version(JsonDataCommObj* pReq);

	//EJSON_RPCGMGR_TRIGGER_DATASAVE
	int json_to_bin_trigger_datasave(JsonDataCommObj* pReq);
	int process_trigger_datasave(RPC_SRV_REQ* pReq);
	int bin_to_json_trigger_datasave(JsonDataCommObj* pReq);

	//EJSON_RPCGMGR_GET_SETTINGS_STS
	int json_to_bin_get_settings_sts(JsonDataCommObj* pReq);
	int process_get_settings_status(RPC_SRV_REQ* pReq);
	int bin_to_json_get_settings_sts(JsonDataCommObj* pReq);

	//EJSON_RPCGMGR_SHUTDOWN_SERVICE
	int json_to_bin_shutdown_service(JsonDataCommObj* pReq);
	int process_shutdown_service(RPC_SRV_REQ* pReq);
	int bin_to_json_shutdown_service(JsonDataCommObj* pReq);

	//EJSON_RPCGMGR_RESET_TASK_STS
	int json_to_bin_reset_task_sts(JsonDataCommObj* pReq);
	int process_reset_task_sts(RPC_SRV_REQ* pReq);
	int bin_to_json_reset_task_sts(JsonDataCommObj* pReq);

	//EJSON_RPCGMGR_GET_READY_STS
	int json_to_bin_get_ready_sts(JsonDataCommObj* pReq);
	int process_get_ready_status(RPC_SRV_REQ* pReq);
	int bin_to_json_get_ready_sts(JsonDataCommObj* pReq);

	//EJSON_RPCGMGR_GET_DEBUG_LOG=7
	int json_to_bin_get_debug_logging(JsonDataCommObj* pReq);
	int process_get_debug_logging(RPC_SRV_REQ* pReq);
	int bin_to_json_get_debug_logging(JsonDataCommObj* pReq);

	//EJSON_RPCGMGR_SET_DEBUG_LOG=8
	int json_to_bin_set_debug_logging(JsonDataCommObj* pReq);
	int process_set_debug_logging(RPC_SRV_REQ* pReq);
	int bin_to_json_set_debug_logging(JsonDataCommObj* pReq);

	//EJSON_RPCGMGR_TRIGGER_FACTORY_STORE=9
	int json_to_bin_trigger_factory_store(JsonDataCommObj* pReq);
	int process_trigger_factory_store(RPC_SRV_REQ* pReq);
	int bin_to_json_trigger_factory_store(JsonDataCommObj* pReq);

	//EJSON_RPCGMGR_TRIGGER_FACTORY_RESTORE=10
	int json_to_bin_trigger_factory_restore(JsonDataCommObj* pReq);
	int process_trigger_factory_restore(RPC_SRV_REQ* pReq);
	int bin_to_json_trigger_factory_restore(JsonDataCommObj* pReq);

	//EJSON_RPCGMGR_EVENT_SUBSCRIBE=11
	int json_to_bin_event_subscribe(JsonDataCommObj* pReq);
	int process_event_subscribe(RPC_SRV_REQ* pReq);
	int bin_to_json_event_subscribe(JsonDataCommObj* pReq);

	//EJSON_RPCGMGR_EVENT_UNSUBSCRIBE=12
	int json_to_bin_event_unsubscribe(JsonDataCommObj* pReq);
	int process_event_unsubscribe(RPC_SRV_REQ* pReq);
	int bin_to_json_event_unsubscribe(JsonDataCommObj* pReq);

	//EJSON_RPCGMGR_EVENT_NOTIFY=13
	int json_to_bin_event_notify(JsonDataCommObj* pReq);
	int process_event_notify(RPC_SRV_REQ* pReq);
	int bin_to_json_event_notify(JsonDataCommObj* pReq);

	//EJSON_RPCGMGR_EVENT_PROCESS=14
	int json_to_bin_event_process(JsonDataCommObj* pReq);
	int process_event_process(RPC_SRV_REQ* pReq);
	int bin_to_json_event_process(JsonDataCommObj* pReq);

public:
	ADJsonRpcMgr(int ver,bool debuglog=false,ADCMN_DEV_INFO* pDev=NULL);
	~ADJsonRpcMgr();
	int AttachHeartBeat(ADTimer* pTimer);
	int Start(int port,int socket_log,int emulation);
	int SupportShutdownRpc(bool support);
	int SetServiceReadyFlag(EJSON_RPCGMGR_READY_STATE sts);

};
/* ------------------------------------------------------------------------- */

#endif
