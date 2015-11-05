#include "ADJsonRpcMgr.hpp"
#include "ADCmnPortList.h"

/* ------------------------------------------------------------------------- */
ADJsonRpcMgr::ADJsonRpcMgr(int ver,bool debuglog,ADCMN_DEV_INFO* pDev)
{
	pDevInfo=pDev;
	svnVersion=ver;
	ServiceDebugFlag=debuglog;
	JMapper.AttachProxy(&Proxy);//let Proxy know who to call back for mapping
	JMapper.AttachMapper(this); //attach myself as mapper, jmapper shall call me back for mapper functions
	JMapper.AttachWorker(this); //attach myself as worker
	AsyncTaskWorker.attach_helper(this);
	EventMgr.AttachReceiver(this);//attach myself for event-received-callback
	shutdown_support=true;//by default support shutdown-rpc
	ServiceReadyFlag=EJSON_RPCGMGR_READY_STATE_NOT_READY;
}
/* ------------------------------------------------------------------------- */
ADJsonRpcMgr::~ADJsonRpcMgr()
{
}
/* ------------------------------------------------------------------------- */
int ADJsonRpcMgr::AttachHeartBeat(ADTimer* pTimer)
{
	//give 100ms heartbeat to ADDisplayMgr
	//this is needed for detecting 3sec timeout for uart communication	
	pTimer->subscribe_timer_notification(this);
	myTimer=pTimer;
	return 0;
}
int ADJsonRpcMgr::SupportShutdownRpc(bool support)
{
	shutdown_support=support;
	return 0;
}
int ADJsonRpcMgr::SetServiceReadyFlag(EJSON_RPCGMGR_READY_STATE sts)
{
	ServiceReadyFlag=sts;
	return 0;
}
/* ------------------------------------------------------------------------- */
//external event receiver function
int ADJsonRpcMgr::receive_events(int cltToken,int evntNum,int evntArg)
{
	//std::cout << "receive_events: clt_token = " <<cltToken<<" evnt_num = "<<evntNum<<" evnt_arg = "<<evntArg <<endl;
	notify_event_recevers(cltToken,evntNum,evntArg);
	return 0;
}
RPC_SRV_RESULT ADJsonRpcMgr::run_work(int cmd,unsigned char* pWorkData,ADTaskWorkerProducer *pTaskWorker)
{
	RPC_SRV_RESULT ret_val=RPC_SRV_RESULT_FAIL;
	
	if(cmd>=EJSON_RPCGMGR_CMD_END)
		ret_val=AsyncTaskWork(cmd,pWorkData);
	else
	{
		switch(cmd)
		{
			case EJSON_RPCGMGR_SHUTDOWN_SERVICE:
				{
					RPCMGR_TASK_STS_PACKET *pPacket;
					pPacket=(RPCMGR_TASK_STS_PACKET *)pWorkData;
					myTimer->forced_exit();//tell timer to break mainloop so that server can shutdown
					OBJ_MEM_DELETE(pWorkData);
					ret_val=RPC_SRV_RESULT_SUCCESS;//anway server is going to shutdown, result may not be necessary
				}
				break;
			case EJSON_RPCGMGR_TRIGGER_FACTORY_STORE:
				{
					RPCMGR_TASK_STS_PACKET *pPacket;
					pPacket=(RPCMGR_TASK_STS_PACKET *)pWorkData;
					//myData->factory_store();TODO
					OBJ_MEM_DELETE(pWorkData);
					ret_val=RPC_SRV_RESULT_SUCCESS;
				}
				break;
			case EJSON_RPCGMGR_TRIGGER_FACTORY_RESTORE:
				{
					RPCMGR_TASK_STS_PACKET *pPacket;
					pPacket=(RPCMGR_TASK_STS_PACKET *)pWorkData;
					//myData->factory_restore();TODO
					OBJ_MEM_DELETE(pWorkData);
					ret_val=RPC_SRV_RESULT_SUCCESS;
				}
				break;
			case EJSON_RPCGMGR_TRIGGER_RUN:
				{
					RPCMGR_TASK_STS_PACKET *pPacket;
					pPacket=(RPCMGR_TASK_STS_PACKET *)pWorkData;
					//myData->trigger_run();TODO
					OBJ_MEM_DELETE(pWorkData);
					ret_val=RPC_SRV_RESULT_SUCCESS;
				}
				break;
			default:
				break;
		}
	}
	//OBJ_MEM_DELETE(pWorkData);
	return ret_val;
}
/* ------------------------------------------------------------------------- */
int ADJsonRpcMgr::Start(int port,int socket_log,int emulation)
{
	char rpc_name[1024];
	//TODO:myCmdLine.get_emulation_mode()
	Proxy.start_listening(port,socket_log);
	TaskWorkerSetPortNumber(port);//for notifying asyTaskDone events

	//attach first 9 rpc's which are common to all services
	JMapper.attach_rpc_method(EJSON_RPCGMGR_GET_TASK_STS           ,(char*)RPCMGR_RPC_TASK_STS_GET);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_GET_RPC_SRV_VERSION    ,(char*)RPCMGR_RPC_SER_VER_GET);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_TRIGGER_DATASAVE       ,(char*)RPCMGR_RPC_TRIG_SAVE);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_GET_SETTINGS_STS       ,(char*)RPCMGR_RPC_STTNG_STS_GET);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_SHUTDOWN_SERVICE       ,(char*)RPCMGR_RPC_TRIG_SHUTDOWN);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_RESET_TASK_STS         ,(char*)RPCMGR_RPC_RESET_TASKSTS);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_GET_READY_STS          ,(char*)RPCMGR_RPC_READY_STS_GET);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_GET_DEBUG_LOG          ,(char*)RPCMGR_RPC_DEBUG_LOG_GET);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_SET_DEBUG_LOG          ,(char*)RPCMGR_RPC_DEBUG_LOG_SET);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_TRIGGER_FACTORY_STORE  ,(char*)RPCMGR_RPC_TRIG_FACT_STORE);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_TRIGGER_FACTORY_RESTORE,(char*)RPCMGR_RPC_TRIG_FACT_RESTORE);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_EVENT_SUBSCRIBE        ,(char*)RPCMGR_RPC_EVENT_SUBSCRIBE);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_EVENT_UNSUBSCRIBE      ,(char*)RPCMGR_RPC_EVENT_UNSUBSCRIBE);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_EVENT_NOTIFY           ,(char*)RPCMGR_RPC_EVENT_NOTIFY);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_EVENT_PROCESS          ,(char*)RPCMGR_RPC_EVENT_PROCESS);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_TRIGGER_RUN            ,(char*)RPCMGR_RPC_TRIG_RUN);

	int total = get_total_attached_rpcs();
	for(int i=0;i<total;i++) 
	{
		ADJsonRpcMgrConsumer* rpc=getRpcHandler(i);//get the attached object pointer by index
		strcpy(rpc_name, rpc->strRpcName.c_str());
		rpc->parent_index=i+EJSON_RPCGMGR_CMD_END;//keep rpc-mgr global-index locally in attached rpc object
		JMapper.attach_rpc_method(i+EJSON_RPCGMGR_CMD_END,rpc_name);//attached with global-index offset of EJSON_RPCGMGR_CMD_END(7)
	}
	//TODO: TyWorker.SetReady();
	return 0;
}
/* ------------------------------------------------------------------------- */
int ADJsonRpcMgr::create_req_obj(JsonDataCommObj* pReq)
{
	//char param_value[255];
	RPC_SRV_REQ *pPanelReq=NULL;
	OBJECT_MEM_NEW(pPanelReq,RPC_SRV_REQ);
	if(pPanelReq==NULL) return -1;//memory error

	pPanelReq->clientId = pReq->req_id;//json req id
	pPanelReq->reqIdent = 0;
	pPanelReq->result   = RPC_SRV_RESULT_NOT_STARTED;
	pPanelReq->dataSize = 0;
	pPanelReq->dataRef  = NULL;//panel's command obj(will be created by the command-mapper)

	//put it in json-req object
	pReq->dataObjLen    = sizeof(RPC_SRV_REQ);
	pReq->pDataObj      = (unsigned char*)pPanelReq;
	return 0;
}
/* ------------------------------------------------------------------------- */
int ADJsonRpcMgr::delete_req_obj(JsonDataCommObj* pReq)
{
	if(pReq->pDataObj==NULL)return 0;//no panel req obj is present
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ  *)pReq->pDataObj;
	OBJ_MEM_DELETE(pPanelReq);
	return 0;
}
/* ------------------------------------------------------------------------- */
int ADJsonRpcMgr::process_json_to_binary(JsonDataCommObj* pReq)
{
	//printf("ADJsonRpcMgr::process_json_to_binary called\n");

	//RPC_SRV_REQ *pPanelReq=NULL;
	//pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	//EJSON_RPCGMGR_CMD cmd = (EJSON_RPCGMGR_CMD)pPanelReq->cmd;
	//if(cmd>=EJSON_RPCGMGR_CMD_END)
	//	return  MapJsonToBinary(pReq);//callback of the attached object from the list
	//else
	//	return  MyMapJsonToBinary(pReq);//handle my own rpc calls

	int result=-1;
	if(create_req_obj(pReq)!=0)return -1;
	EJSON_RPCGMGR_CMD rpc=(EJSON_RPCGMGR_CMD)pReq->cmd_index;
	if(rpc>=EJSON_RPCGMGR_CMD_END)
		result=MapJsonToBinary(pReq);//callback of the attached object from the list
	else
		result=MyMapJsonToBinary(pReq);//handle my own rpc calls
	if(result!=0)
		delete_req_obj(pReq);
	return result;
}
int ADJsonRpcMgr::process_binary_to_json(JsonDataCommObj* pReq)
{
	//printf("ADJsonRpcMgr::process_binary_to_json called\n");

	//RPC_SRV_REQ *pPanelReq=NULL;
	//pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	//EJSON_RPCGMGR_CMD cmd = (EJSON_RPCGMGR_CMD)pPanelReq->cmd;
	//if(cmd>=EJSON_RPCGMGR_CMD_END)
	//	return MapBinaryToJson(pReq);//callback of the attached object from the list
	//else
	//	return MyMapBinaryToJson(pReq);//handle my own rpc calls
	int result =-1;
	EJSON_RPCGMGR_CMD rpc=(EJSON_RPCGMGR_CMD)pReq->cmd_index;
	if(rpc>=EJSON_RPCGMGR_CMD_END)
		result=MapBinaryToJson(pReq);//callback of the attached object from the list
	else
		result=MyMapBinaryToJson(pReq);//handle my own rpc calls
	//after mapping the reply value, delete my object from JsonDataCommObj;	
	delete_req_obj(pReq);
	return result;
}
int ADJsonRpcMgr::process_work(JsonDataCommObj* pReq)
{
	//RPC_SRV_REQ *pPanelReq=NULL;
	//pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	EJSON_RPCGMGR_CMD rpc = (EJSON_RPCGMGR_CMD)pReq->cmd_index;
	if(rpc>=EJSON_RPCGMGR_CMD_END)
		return ProcessWork(pReq);//callback of the attached object from the list
	else
		return MyProcessWork(pReq);//handle my own rpc calls
}
/* ------------------------------------------------------------------------- */
int ADJsonRpcMgr::MyMapJsonToBinary(JsonDataCommObj* pReq)
{
	int result =-1;
	EJSON_RPCGMGR_CMD rpc=(EJSON_RPCGMGR_CMD)pReq->cmd_index;
	switch(rpc)
	{
		case EJSON_RPCGMGR_GET_TASK_STS           :result=json_to_bin_get_task_sts(pReq);break;
		case EJSON_RPCGMGR_GET_RPC_SRV_VERSION    :result=json_to_bin_get_rpc_srv_version(pReq);break;
		case EJSON_RPCGMGR_TRIGGER_DATASAVE       :result=json_to_bin_trigger_datasave(pReq);break;
		case EJSON_RPCGMGR_GET_SETTINGS_STS       :result=json_to_bin_get_settings_sts(pReq);break;
		case EJSON_RPCGMGR_SHUTDOWN_SERVICE       :result=json_to_bin_shutdown_service(pReq);break;
		case EJSON_RPCGMGR_RESET_TASK_STS         :result=json_to_bin_reset_task_sts(pReq);break;
		case EJSON_RPCGMGR_GET_READY_STS          :result=json_to_bin_get_ready_sts(pReq);break;
		case EJSON_RPCGMGR_GET_DEBUG_LOG          :result=json_to_bin_get_debug_logging(pReq);break;
		case EJSON_RPCGMGR_SET_DEBUG_LOG          :result=json_to_bin_set_debug_logging(pReq);break;
		case EJSON_RPCGMGR_TRIGGER_FACTORY_STORE  :result=json_to_bin_trigger_factory_store(pReq);break;
		case EJSON_RPCGMGR_TRIGGER_FACTORY_RESTORE:result=json_to_bin_trigger_factory_restore(pReq);break;
		case EJSON_RPCGMGR_EVENT_SUBSCRIBE        :result=json_to_bin_event_subscribe(pReq);break;
		case EJSON_RPCGMGR_EVENT_UNSUBSCRIBE      :result=json_to_bin_event_unsubscribe(pReq);break;
		case EJSON_RPCGMGR_EVENT_NOTIFY           :result=json_to_bin_event_notify(pReq);break;
		case EJSON_RPCGMGR_EVENT_PROCESS          :result=json_to_bin_event_process(pReq);break;
		case EJSON_RPCGMGR_TRIGGER_RUN            :result=json_to_bin_trigger_run(pReq);break;
		default:break;
	}
	return result;
}
/* ------------------------------------------------------------------------- */
int ADJsonRpcMgr::MyMapBinaryToJson(JsonDataCommObj* pReq)
{
	int result =-1;
	EJSON_RPCGMGR_CMD rpc=(EJSON_RPCGMGR_CMD)pReq->cmd_index;
	switch(rpc)
	{
		case EJSON_RPCGMGR_GET_TASK_STS           :result=bin_to_json_get_task_sts(pReq);break;
		case EJSON_RPCGMGR_GET_RPC_SRV_VERSION    :result=bin_to_json_get_rpc_srv_version(pReq);break;
		case EJSON_RPCGMGR_TRIGGER_DATASAVE       :result=bin_to_json_trigger_datasave(pReq);break;
		case EJSON_RPCGMGR_GET_SETTINGS_STS       :result=bin_to_json_get_settings_sts(pReq);break;
		case EJSON_RPCGMGR_SHUTDOWN_SERVICE       :result=bin_to_json_shutdown_service(pReq);break;
		case EJSON_RPCGMGR_RESET_TASK_STS         :result=bin_to_json_reset_task_sts(pReq);break;
		case EJSON_RPCGMGR_GET_READY_STS          :result=bin_to_json_get_ready_sts(pReq);break;
		case EJSON_RPCGMGR_GET_DEBUG_LOG          :result=bin_to_json_get_debug_logging(pReq);break;
		case EJSON_RPCGMGR_SET_DEBUG_LOG          :result=bin_to_json_set_debug_logging(pReq);break;
		case EJSON_RPCGMGR_TRIGGER_FACTORY_STORE  :result=bin_to_json_trigger_factory_store(pReq);break;
		case EJSON_RPCGMGR_TRIGGER_FACTORY_RESTORE:result=bin_to_json_trigger_factory_restore(pReq);break;
		case EJSON_RPCGMGR_EVENT_SUBSCRIBE        :result=bin_to_json_event_subscribe(pReq);break;
		case EJSON_RPCGMGR_EVENT_UNSUBSCRIBE      :result=bin_to_json_event_unsubscribe(pReq);break;
		case EJSON_RPCGMGR_EVENT_NOTIFY           :result=bin_to_json_event_notify(pReq);break;
		case EJSON_RPCGMGR_EVENT_PROCESS          :result=bin_to_json_event_process(pReq);break;
		case EJSON_RPCGMGR_TRIGGER_RUN            :result=bin_to_json_trigger_run(pReq);break;
		default:break;
	}
	return result;
}

/* ------------------------------------------------------------------------- */
int ADJsonRpcMgr::MyProcessWork(JsonDataCommObj* pReq)
{
	//TODO
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	EJSON_RPCGMGR_CMD rpc = (EJSON_RPCGMGR_CMD)pReq->cmd_index;
	switch(rpc)
	{
		case EJSON_RPCGMGR_GET_TASK_STS           :return process_get_task_status(pPanelReq);break;
		case EJSON_RPCGMGR_GET_RPC_SRV_VERSION    :return process_rpc_server_version(pPanelReq);break;
		case EJSON_RPCGMGR_TRIGGER_DATASAVE       :return process_trigger_datasave(pPanelReq);break;
		case EJSON_RPCGMGR_GET_SETTINGS_STS       :return process_get_settings_status(pPanelReq);break;
		case EJSON_RPCGMGR_SHUTDOWN_SERVICE       :return process_shutdown_service(pPanelReq);break;
		case EJSON_RPCGMGR_RESET_TASK_STS         :return process_reset_task_sts(pPanelReq);break;
		case EJSON_RPCGMGR_GET_READY_STS          :return process_get_ready_status(pPanelReq);break;
		case EJSON_RPCGMGR_GET_DEBUG_LOG          :return process_get_debug_logging(pPanelReq);break;
		case EJSON_RPCGMGR_SET_DEBUG_LOG          :return process_set_debug_logging(pPanelReq);break;
		case EJSON_RPCGMGR_TRIGGER_FACTORY_STORE  :return process_trigger_factory_store(pPanelReq);break;
		case EJSON_RPCGMGR_TRIGGER_FACTORY_RESTORE:return process_trigger_factory_restore(pPanelReq);break;
		case EJSON_RPCGMGR_EVENT_SUBSCRIBE        :return process_event_subscribe(pPanelReq);break;
		case EJSON_RPCGMGR_EVENT_UNSUBSCRIBE      :return process_event_unsubscribe(pPanelReq);break;
		case EJSON_RPCGMGR_EVENT_NOTIFY           :return process_event_notify(pPanelReq);break;
		case EJSON_RPCGMGR_EVENT_PROCESS          :return process_event_process(pPanelReq);break;
		case EJSON_RPCGMGR_TRIGGER_RUN            :return process_trigger_run(pPanelReq);break;
		default:break;
	}
	return -1;
}
/* ------------------------------------------------------------------------- */
/*int ADJsonRpcMgr::prepare_req_object(JsonDataCommObj* pReq,RPC_SRV_ACT action,int cmd)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	pPanelReq->action   = action;
	pPanelReq->cmd      = cmd;
	return 0;
}*/
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_GET_TASK_STS (common rpc for all services)
int ADJsonRpcMgr::json_to_bin_get_task_sts(JsonDataCommObj* pReq)
{
	RPCMGR_TASK_STS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,RPCMGR_TASK_STS_PACKET,RPC_SRV_ACT_READ,EJSON_RPCGMGR_GET_TASK_STS);
	JSON_STRING_TO_INT(RPCMGR_RPC_TASK_STS_ARGID,pPanelCmdObj->taskID);
	return 0;
}
int ADJsonRpcMgr::process_get_task_status(RPC_SRV_REQ* pReq)
{
	const char *status_table[]   = STR_RPC_SRV_RESULT_STRING_TABLE;
	RPCMGR_TASK_STS_PACKET* pPacket;
	pPacket=(RPCMGR_TASK_STS_PACKET*)pReq->dataRef;
	int taskSts;char errMsg[255];
	pReq->result=AsyncTaskWorker.get_task_status(pPacket->taskID,&taskSts,errMsg);
	strcpy(pPacket->task_sts,status_table[taskSts]);
	return 0;
}
int ADJsonRpcMgr::bin_to_json_get_task_sts(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,RPCMGR_TASK_STS_PACKET,RPCMGR_RPC_TASK_STS_ARGSTS,task_sts);
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_GET_RPC_SRV_VERSION (common rpc for all services)
int ADJsonRpcMgr::json_to_bin_get_rpc_srv_version(JsonDataCommObj* pReq)
{
	RPCMGR_INTEGER_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,RPCMGR_INTEGER_PACKET,RPC_SRV_ACT_READ,EJSON_RPCGMGR_GET_RPC_SRV_VERSION);
	return 0;
}
int ADJsonRpcMgr::process_rpc_server_version(RPC_SRV_REQ* pReq)
{	
	RPCMGR_INTEGER_PACKET* pPacket;
	pPacket=(RPCMGR_INTEGER_PACKET*)pReq->dataRef;
	if(pReq->action==RPC_SRV_ACT_READ)
	{
		pPacket->value=svnVersion;
		pReq->result=RPC_SRV_RESULT_SUCCESS;
	}
	else
		pReq->result=RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
	return 0;
}
int ADJsonRpcMgr::bin_to_json_get_rpc_srv_version(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_INT(RPC_SRV_REQ,RPCMGR_INTEGER_PACKET,RPCMGR_RPC_SER_VER_ARGVER,value);
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_TRIGGER_DATASAVE (common rpc for all services)
int ADJsonRpcMgr::json_to_bin_trigger_datasave(JsonDataCommObj* pReq)
{
	RPCMGR_TASK_STS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,RPCMGR_TASK_STS_PACKET,RPC_SRV_ACT_WRITE,EJSON_RPCGMGR_TRIGGER_DATASAVE);
	return 0;
}
int ADJsonRpcMgr::process_trigger_datasave(RPC_SRV_REQ* pReq)
{
	pReq->result=RPC_SRV_RESULT_FEATURE_UNSUPPORTED;
	return 0;
}
int ADJsonRpcMgr::bin_to_json_trigger_datasave(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_IN_PROG(RPC_SRV_REQ,RPCMGR_TASK_STS_PACKET,RPCMGR_RPC_TASK_STS_ARGSTS);
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_GET_SETTINGS_STS (common rpc for all services)
int ADJsonRpcMgr::json_to_bin_get_settings_sts(JsonDataCommObj* pReq)
{
	RPCMGR_SETTINGS_STS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,RPCMGR_SETTINGS_STS_PACKET,RPC_SRV_ACT_READ,EJSON_RPCGMGR_GET_SETTINGS_STS);
	return 0;
}
int ADJsonRpcMgr::process_get_settings_status(RPC_SRV_REQ* pReq)
{
	pReq->result=RPC_SRV_RESULT_FEATURE_UNSUPPORTED;
	return 0;
}
int ADJsonRpcMgr::bin_to_json_get_settings_sts(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,RPCMGR_SETTINGS_STS_PACKET,RPCMGR_RPC_STTNG_STS_ARGSTS,status);
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_SHUTDOWN_SERVICE (common rpc for all services)
//trigger shutdown of the service(just returns in-progress, and after 2seconds and service exits)
int ADJsonRpcMgr::json_to_bin_shutdown_service(JsonDataCommObj* pReq)
{
	RPCMGR_TASK_STS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,RPCMGR_TASK_STS_PACKET,RPC_SRV_ACT_WRITE,EJSON_RPCGMGR_SHUTDOWN_SERVICE);
	return 0;
}
int ADJsonRpcMgr::process_shutdown_service(RPC_SRV_REQ* pReq)
{
	if(shutdown_support==false)
	{
		pReq->result=RPC_SRV_RESULT_FEATURE_UNSUPPORTED;
		return 0;
	}		
	RPCMGR_TASK_STS_PACKET* pPacket;
	pPacket=(RPCMGR_TASK_STS_PACKET*)pReq->dataRef;
	if(pReq->action==RPC_SRV_ACT_WRITE)
	{	
		RPCMGR_TASK_STS_PACKET* pWorkData=NULL;
		OBJECT_MEM_NEW(pWorkData,RPCMGR_TASK_STS_PACKET);//delete this object in run_work() callback function
		if(AsyncTaskWorker.push_task(EJSON_RPCGMGR_SHUTDOWN_SERVICE,(unsigned char*)pWorkData,&pPacket->taskID,WORK_CMD_AFTER_DONE_DELETE)==0)
			pReq->result=RPC_SRV_RESULT_IN_PROG;
		else
		{
			OBJ_MEM_DELETE(pWorkData);
			pReq->result=RPC_SRV_RESULT_FAIL;
		}
	}
	else
	{
		pReq->result=RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
	}
	return 0;
}
int ADJsonRpcMgr::bin_to_json_shutdown_service(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_IN_PROG(RPC_SRV_REQ,RPCMGR_TASK_STS_PACKET,RPCMGR_RPC_TRIG_SHUTDOWN_ARGID);
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_RESET_TASK_STS (common rpc for all services)
int ADJsonRpcMgr::json_to_bin_reset_task_sts(JsonDataCommObj* pReq)
{
	RPCMGR_TASK_STS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,RPCMGR_TASK_STS_PACKET,RPC_SRV_ACT_WRITE,EJSON_RPCGMGR_RESET_TASK_STS);
	return 0;
}
int ADJsonRpcMgr::process_reset_task_sts(RPC_SRV_REQ* pReq)
{
	pReq->result=AsyncTaskWorker.reset_task_id_and_chain();//warning: resetting this chain may cause lost task status which was in progress
	return 0;
}
int ADJsonRpcMgr::bin_to_json_reset_task_sts(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,RPCMGR_TASK_STS_PACKET);//delete packet and return success after datasave
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_GET_READY_STS (common rpc for all services)
int ADJsonRpcMgr::json_to_bin_get_ready_sts(JsonDataCommObj* pReq)
{
	RPCMGR_READY_STS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,RPCMGR_READY_STS_PACKET,RPC_SRV_ACT_READ,EJSON_RPCGMGR_GET_READY_STS);
	return 0;
}
int ADJsonRpcMgr::process_get_ready_status(RPC_SRV_REQ* pReq)
{
	RPCMGR_READY_STS_PACKET* pPacket;
	pPacket=(RPCMGR_READY_STS_PACKET*)pReq->dataRef;
	pPacket->status=ServiceReadyFlag;//DataCache.service_ready_state;
	pReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
int ADJsonRpcMgr::bin_to_json_get_ready_sts(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,RPCMGR_READY_STS_PACKET,RPCMGR_RPC_READY_STS_ARGSTS,status,RPCMGR_RPC_READY_STS_ARGSTS_TBL,EJSON_RPCGMGR_READY_STATE_UNKNOWN);
	return 0;
}
/* ------------------------------------------------------------------------- */
int ADJsonRpcMgr::json_to_bin_get_debug_logging(JsonDataCommObj* pReq)
{
	RPCMGR_DEBUG_LOG_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,RPCMGR_DEBUG_LOG_PACKET,RPC_SRV_ACT_READ,EJSON_RPCGMGR_GET_DEBUG_LOG);
	return 0;
}
int ADJsonRpcMgr::process_get_debug_logging(RPC_SRV_REQ* pReq)
{
	RPCMGR_DEBUG_LOG_PACKET* pPacket;
	pPacket=(RPCMGR_DEBUG_LOG_PACKET*)pReq->dataRef;
	//cout<<"ADJsonRpcMgr::process_get_debug_logging:flag-value="<<ServiceDebugFlag<<endl;
	if(ServiceDebugFlag==true)
		pPacket->status=EJSON_RPCGMGR_FLAG_STATE_ENABLE;
	else if(ServiceDebugFlag==false)
		pPacket->status=EJSON_RPCGMGR_FLAG_STATE_DISABLE;
	else
		pPacket->status=EJSON_RPCGMGR_FLAG_STATE_UNKNOWN;
	pReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
int ADJsonRpcMgr::bin_to_json_get_debug_logging(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,RPCMGR_DEBUG_LOG_PACKET,RPCMGR_RPC_DEBUG_LOG_ARGSTS,status,RPCMGR_RPC_DEBUG_LOG_ARGSTS_TBL,EJSON_RPCGMGR_FLAG_STATE_UNKNOWN);
	return 0;
}
/* ------------------------------------------------------------------------- */
int ADJsonRpcMgr::json_to_bin_set_debug_logging(JsonDataCommObj* pReq)
{
	RPCMGR_DEBUG_LOG_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,RPCMGR_DEBUG_LOG_PACKET,RPC_SRV_ACT_WRITE,EJSON_RPCGMGR_SET_DEBUG_LOG);
	JSON_STRING_TO_ENUM(RPCMGR_RPC_DEBUG_LOG_ARGSTS,RPCMGR_RPC_DEBUG_LOG_ARGSTS_TBL,EJSON_RPCGMGR_FLAG_STATE,EJSON_RPCGMGR_FLAG_STATE_UNKNOWN,pPanelCmdObj->status);
	return 0;
}
int ADJsonRpcMgr::process_set_debug_logging(RPC_SRV_REQ* pReq)
{
	RPCMGR_DEBUG_LOG_PACKET* pPacket;
	pPacket=(RPCMGR_DEBUG_LOG_PACKET*)pReq->dataRef;
	switch(pPacket->status)
	{
		case EJSON_RPCGMGR_FLAG_STATE_DISABLE:ServiceDebugFlag=false;break;
		case EJSON_RPCGMGR_FLAG_STATE_ENABLE :ServiceDebugFlag=true ;break;
		default                              :pReq->result=RPC_SRV_RESULT_ARG_ERROR;return 0;
	}
	//apply debuglog flag to all attached rpc's
	setRpcDebugLogFlag(ServiceDebugFlag);
	pReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
int ADJsonRpcMgr::bin_to_json_set_debug_logging(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,RPCMGR_DEBUG_LOG_PACKET);
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_TRIGGER_FACTORY_STORE (common rpc for all services)
int ADJsonRpcMgr::json_to_bin_trigger_factory_store(JsonDataCommObj* pReq)
{
	RPCMGR_TASK_STS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,RPCMGR_TASK_STS_PACKET,RPC_SRV_ACT_WRITE,EJSON_RPCGMGR_TRIGGER_FACTORY_STORE);
	return 0;
}
int ADJsonRpcMgr::process_trigger_factory_store(RPC_SRV_REQ* pReq)
{
	//pReq->result=RPC_SRV_RESULT_FEATURE_UNSUPPORTED;
	RPCMGR_TASK_STS_PACKET* pPacket;
	pPacket=(RPCMGR_TASK_STS_PACKET*)pReq->dataRef;
	RPCMGR_TASK_STS_PACKET* pWorkData=NULL;
	OBJECT_MEM_NEW(pWorkData,RPCMGR_TASK_STS_PACKET);//delete this object in run_work() callback function
	if(AsyncTaskWorker.push_task(EJSON_RPCGMGR_TRIGGER_FACTORY_STORE,(unsigned char*)pWorkData,&pPacket->taskID,WORK_CMD_AFTER_DONE_DELETE)==0)
		pReq->result=RPC_SRV_RESULT_IN_PROG;
	else
	{
		OBJ_MEM_DELETE(pWorkData);
		pReq->result=RPC_SRV_RESULT_FAIL;
	}
	return 0;
}
int ADJsonRpcMgr::bin_to_json_trigger_factory_store(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_IN_PROG(RPC_SRV_REQ,RPCMGR_TASK_STS_PACKET,RPCMGR_RPC_TASK_STS_ARGSTS);
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_TRIGGER_FACTORY_RESTORE (common rpc for all services)
int ADJsonRpcMgr::json_to_bin_trigger_factory_restore(JsonDataCommObj* pReq)
{
	RPCMGR_TASK_STS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,RPCMGR_TASK_STS_PACKET,RPC_SRV_ACT_WRITE,EJSON_RPCGMGR_TRIGGER_FACTORY_RESTORE);
	return 0;
}
int ADJsonRpcMgr::process_trigger_factory_restore(RPC_SRV_REQ* pReq)
{
	//pReq->result=RPC_SRV_RESULT_FEATURE_UNSUPPORTED;
	RPCMGR_TASK_STS_PACKET* pPacket;
	pPacket=(RPCMGR_TASK_STS_PACKET*)pReq->dataRef;
	RPCMGR_TASK_STS_PACKET* pWorkData=NULL;
	OBJECT_MEM_NEW(pWorkData,RPCMGR_TASK_STS_PACKET);//delete this object in run_work() callback function
	if(AsyncTaskWorker.push_task(EJSON_RPCGMGR_TRIGGER_FACTORY_RESTORE,(unsigned char*)pWorkData,&pPacket->taskID,WORK_CMD_AFTER_DONE_DELETE)==0)
		pReq->result=RPC_SRV_RESULT_IN_PROG;
	else
	{
		OBJ_MEM_DELETE(pWorkData);
		pReq->result=RPC_SRV_RESULT_FAIL;
	}
	return 0;
}
int ADJsonRpcMgr::bin_to_json_trigger_factory_restore(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_IN_PROG(RPC_SRV_REQ,RPCMGR_TASK_STS_PACKET,RPCMGR_RPC_TASK_STS_ARGSTS);
	return 0;
}
/* ------------------------------------------------------------------------- */
int ADJsonRpcMgr::json_to_bin_event_subscribe(JsonDataCommObj* pReq)
{
	char temp_param[255];
	RPCMGR_EVENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,RPCMGR_EVENT_PACKET,RPC_SRV_ACT_WRITE,EJSON_RPCGMGR_EVENT_SUBSCRIBE);

	//mandatory arg:
	//this is the token from suscriber, while event notification, just send this token back to subscriber for his own book-keeping
	JSON_STRING_TO_INT(RPCMGR_RPC_EVENT_ARG_CLTTOK,pPanelCmdObj->cltToken);

	//optional arg-port: client has passed the port number where even to be sent to(ip is auto read)
	if(find_single_param((char*)pReq->socket_data,(char*)RPCMGR_RPC_EVENT_ARG_PORT,temp_param)==0)
	{
		JSON_STRING_TO_INT(RPCMGR_RPC_EVENT_ARG_PORT,pPanelCmdObj->portNum);
	}
	else
		pPanelCmdObj->portNum=-1;//ADCMN_PORT_SYSMGR;//-1;//send events on same connection

	//optional arg-event_num: client can request for specific event to be notified,
	if(find_single_param((char*)pReq->socket_data,(char*)RPCMGR_RPC_EVENT_ARG_EVENTNUM,temp_param)==0)
	{
		JSON_STRING_TO_INT(RPCMGR_RPC_EVENT_ARG_EVENTNUM,pPanelCmdObj->eventNum);
	}
	else
		pPanelCmdObj->eventNum=-1;//subscriber wants to be notified of all events.
	
	pPanelCmdObj->sock_id   =pReq->sock_id;
	pPanelCmdObj->sock_descr=pReq->sock_descr;
	strcpy(pPanelCmdObj->ip,pReq->ip);

	return 0;
}
int ADJsonRpcMgr::process_event_subscribe(RPC_SRV_REQ* pReq)
{
	RPCMGR_EVENT_PACKET* pPacket;
	pPacket=(RPCMGR_EVENT_PACKET*)pReq->dataRef;

	EventEntry* pEvent=NULL;
	OBJECT_MEM_NEW(pEvent,EventEntry);
	if(pEvent==NULL)
		pReq->result=RPC_SRV_RESULT_FAIL;
	else
	{
		pEvent->cltToken = pPacket->cltToken;
		pEvent->portNum  = pPacket->portNum;
		pEvent->eventNum = pPacket->eventNum;
		pEvent->sock_id   =pPacket->sock_id;
		pEvent->sock_descr=pPacket->sock_descr;
		strcpy(pEvent->ip,pPacket->ip);
		pEvent->deleteFlag=false;//dont delete, its a fresh subscription(deleted by the worker thread in case of not socket connection)
		int res = EventMgr.register_event_subscription(pEvent,&pPacket->srvToken);//!=0)//on success, serverToken is returned
		if(res == 0)
			pReq->result=RPC_SRV_RESULT_SUCCESS;
		else if(res == -1)//duplicate entry found
		{
			OBJ_MEM_DELETE(pEvent);
			pReq->result=RPC_SRV_RESULT_ITEM_DUPLICATE_FOUND;//RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
		}
		else
		{
			OBJ_MEM_DELETE(pEvent);
			pReq->result=RPC_SRV_RESULT_FAIL;//RPC_SRV_RESULT_ACTION_NOT_ALLOWED
		}
	}
	return 0;
}
int ADJsonRpcMgr::bin_to_json_event_subscribe(JsonDataCommObj* pReq)
{
	//on successful subscription, return a token
	PREPARE_JSON_RESP_INT(RPC_SRV_REQ,RPCMGR_EVENT_PACKET,RPCMGR_RPC_EVENT_ARG_SRVTOK,srvToken);// return the serverToken to caller
	return 0;
}
/* ------------------------------------------------------------------------- */
//this rpc call has an integer arg which is the serverToken
int ADJsonRpcMgr::json_to_bin_event_unsubscribe(JsonDataCommObj* pReq)
{
	//char temp_param[255];
	RPCMGR_EVENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,RPCMGR_EVENT_PACKET,RPC_SRV_ACT_WRITE,EJSON_RPCGMGR_EVENT_UNSUBSCRIBE);
	//mandatory arg:
	//previously assigned server-token number while subscription
	JSON_STRING_TO_INT(RPCMGR_RPC_EVENT_ARG_SRVTOK,pPanelCmdObj->srvToken);
	return 0;
}
int ADJsonRpcMgr::process_event_unsubscribe(RPC_SRV_REQ* pReq)
{
	RPCMGR_EVENT_PACKET* pPacket;
	pPacket=(RPCMGR_EVENT_PACKET*)pReq->dataRef;
	int res = EventMgr.unregister_event_subscription(pPacket->srvToken);
	if(res==0)
		pReq->result=RPC_SRV_RESULT_SUCCESS;
	else if(res==-1)
		pReq->result=RPC_SRV_RESULT_ITEM_NOT_FOUND;
	else
		pReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
int ADJsonRpcMgr::bin_to_json_event_unsubscribe(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,RPCMGR_EVENT_PACKET);
	return 0;
}
/* ------------------------------------------------------------------------- */
//this rpc call has an integer arg which is the event number
int ADJsonRpcMgr::json_to_bin_event_notify(JsonDataCommObj* pReq)
{
	char temp_param[255];
	RPCMGR_EVENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,RPCMGR_EVENT_PACKET,RPC_SRV_ACT_WRITE,EJSON_RPCGMGR_EVENT_NOTIFY);
	JSON_STRING_TO_INT(RPCMGR_RPC_EVENT_ARG_EVENTNUM,pPanelCmdObj->eventNum);//reuse existing eventNum in this object
	//optional arg-port: client has passed the port number where even to be sent to(ip is auto read)
	if(find_single_param((char*)pReq->socket_data,(char*)RPCMGR_RPC_EVENT_ARG_EXTRA,temp_param)==0)
	{
		JSON_STRING_TO_INT(RPCMGR_RPC_EVENT_ARG_EXTRA,pPanelCmdObj->eventArg);
	}
	else
		pPanelCmdObj->eventArg=-1;//-1 indicates no extra arg sent with eventNum
	return 0;
}
int ADJsonRpcMgr::process_event_notify(RPC_SRV_REQ* pReq)
{
	RPCMGR_EVENT_PACKET* pPacket;
	pPacket=(RPCMGR_EVENT_PACKET*)pReq->dataRef;
	int res = EventMgr.notify_event(pPacket->eventNum,pPacket->eventArg);
	if(res==0)
		pReq->result=RPC_SRV_RESULT_SUCCESS;
	else
		pReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
int ADJsonRpcMgr::bin_to_json_event_notify(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,RPCMGR_EVENT_PACKET);
	return 0;
}
/* ------------------------------------------------------------------------- */
//this rpc call has two integer args (cltToken+eventNum) for processing the events
int ADJsonRpcMgr::json_to_bin_event_process(JsonDataCommObj* pReq)
{
	char temp_param[255];
	RPCMGR_EVENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,RPCMGR_EVENT_PACKET,RPC_SRV_ACT_WRITE,EJSON_RPCGMGR_EVENT_PROCESS);
	JSON_STRING_TO_INT(RPCMGR_RPC_EVENT_ARG_CLTTOK,pPanelCmdObj->cltToken);//who is sending this event?
	JSON_STRING_TO_INT(RPCMGR_RPC_EVENT_ARG_EVENTNUM,pPanelCmdObj->eventNum);//event number
	//optional arg-port: client has passed the port number where even to be sent to(ip is auto read)
	if(find_single_param((char*)pReq->socket_data,(char*)RPCMGR_RPC_EVENT_ARG_EXTRA,temp_param)==0)
	{
		JSON_STRING_TO_INT(RPCMGR_RPC_EVENT_ARG_EXTRA,pPanelCmdObj->eventArg);
	}
	else
		pPanelCmdObj->eventArg=-1;//-1 indicates no extra arg sent with eventNum
	return 0;
}
int ADJsonRpcMgr::process_event_process(RPC_SRV_REQ* pReq)
{
	RPCMGR_EVENT_PACKET* pPacket;
	pPacket=(RPCMGR_EVENT_PACKET*)pReq->dataRef;
	EventMgr.process_event(pPacket->eventNum,pPacket->eventArg,pPacket->cltToken);//actual return value not possible due to thread based de-coupling
	pReq->result=RPC_SRV_RESULT_SUCCESS; //further processing is needed based on event-number(user callback??)
	return 0;
}
int ADJsonRpcMgr::bin_to_json_event_process(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,RPCMGR_EVENT_PACKET);//tell event sender that event has been received(subscriber will take further action)
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_RPCGMGR_TRIGGER_RUN (common rpc for all services)
int ADJsonRpcMgr::json_to_bin_trigger_run(JsonDataCommObj* pReq)
{
	RPCMGR_TASK_STS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,RPCMGR_TASK_STS_PACKET,RPC_SRV_ACT_WRITE,EJSON_RPCGMGR_TRIGGER_RUN);
	return 0;
}
int ADJsonRpcMgr::process_trigger_run(RPC_SRV_REQ* pReq)
{
	RPCMGR_TASK_STS_PACKET* pPacket;
	pPacket=(RPCMGR_TASK_STS_PACKET*)pReq->dataRef;
	RPCMGR_TASK_STS_PACKET* pWorkData=NULL;
	OBJECT_MEM_NEW(pWorkData,RPCMGR_TASK_STS_PACKET);//delete this object in run_work() callback function
	if(AsyncTaskWorker.push_task(EJSON_RPCGMGR_TRIGGER_RUN,(unsigned char*)pWorkData,&pPacket->taskID,WORK_CMD_AFTER_DONE_DELETE)==0)
		pReq->result=RPC_SRV_RESULT_IN_PROG;
	else
	{
		OBJ_MEM_DELETE(pWorkData);
		pReq->result=RPC_SRV_RESULT_FAIL;
	}
	return 0;
}
int ADJsonRpcMgr::bin_to_json_trigger_run(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_IN_PROG(RPC_SRV_REQ,RPCMGR_TASK_STS_PACKET,RPCMGR_RPC_TASK_STS_ARGSTS);
	return 0;
}
/* ------------------------------------------------------------------------- */


