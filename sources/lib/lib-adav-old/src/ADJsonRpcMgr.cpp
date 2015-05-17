#include "ADJsonRpcMgr.hpp"
/* ------------------------------------------------------------------------- */
ADJsonRpcMgr::ADJsonRpcMgr(int ver,bool debuglog)
{
	//strcpy(svnVersion,ver);
	svnVersion=ver;
	ServiceDebugFlag=debuglog;
	JMapper.AttachProxy(&Proxy);//let Proxy know who to call back for mapping
	JMapper.AttachMapper(this); //attach myself as mapper, jmapper shall call me back for mapper functions
	JMapper.AttachWorker(this); //attach myself as worker
	AsyncTaskWorker.attach_helper(this);
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
					//OBJ_MEM_DELETE(pWorkData);
					ret_val=RPC_SRV_RESULT_SUCCESS;//anway server is going to shutdown, result may not be necessary
				}
				break;
			default:
				break;
		}
	}
	OBJ_MEM_DELETE(pWorkData);
	return ret_val;
}
/* ------------------------------------------------------------------------- */
int ADJsonRpcMgr::Start(int port,int socket_log,int emulation)
{
	char rpc_name[1024];
	//TODO:myCmdLine.get_emulation_mode()
	Proxy.start_listening(port,socket_log);
	//attach first 9 rpc's which are common to all services
	JMapper.attach_rpc_method(EJSON_RPCGMGR_GET_TASK_STS       ,(char*)RPCMGR_RPC_TASK_STS_GET);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_GET_RPC_SRV_VERSION,(char*)RPCMGR_RPC_SER_VER_GET);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_TRIGGER_DATASAVE   ,(char*)RPCMGR_RPC_TRIG_SAVE);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_GET_SETTINGS_STS   ,(char*)RPCMGR_RPC_STTNG_STS_GET);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_SHUTDOWN_SERVICE   ,(char*)RPCMGR_RPC_TRIG_SHUTDOWN);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_RESET_TASK_STS     ,(char*)RPCMGR_RPC_RESET_TASKSTS);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_GET_READY_STS      ,(char*)RPCMGR_RPC_READY_STS_GET);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_GET_DEBUG_LOG      ,(char*)RPCMGR_RPC_DEBUG_LOG_GET);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_SET_DEBUG_LOG      ,(char*)RPCMGR_RPC_DEBUG_LOG_SET);
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
		case EJSON_RPCGMGR_GET_TASK_STS       :result=json_to_bin_get_task_sts(pReq);break;
		case EJSON_RPCGMGR_GET_RPC_SRV_VERSION:result=json_to_bin_get_rpc_srv_version(pReq);break;
		case EJSON_RPCGMGR_TRIGGER_DATASAVE   :result=json_to_bin_trigger_datasave(pReq);break;
		case EJSON_RPCGMGR_GET_SETTINGS_STS   :result=json_to_bin_get_settings_sts(pReq);break;
		case EJSON_RPCGMGR_SHUTDOWN_SERVICE   :result=json_to_bin_shutdown_service(pReq);break;
		case EJSON_RPCGMGR_RESET_TASK_STS     :result=json_to_bin_reset_task_sts(pReq);break;
		case EJSON_RPCGMGR_GET_READY_STS      :result=json_to_bin_get_ready_sts(pReq);break;
		case EJSON_RPCGMGR_GET_DEBUG_LOG      :result=json_to_bin_get_debug_logging(pReq);break;
		case EJSON_RPCGMGR_SET_DEBUG_LOG      :result=json_to_bin_set_debug_logging(pReq);break;
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
		case EJSON_RPCGMGR_GET_TASK_STS       :result=bin_to_json_get_task_sts(pReq);break;
		case EJSON_RPCGMGR_GET_RPC_SRV_VERSION:result=bin_to_json_get_rpc_srv_version(pReq);break;
		case EJSON_RPCGMGR_TRIGGER_DATASAVE   :result=bin_to_json_trigger_datasave(pReq);break;
		case EJSON_RPCGMGR_GET_SETTINGS_STS   :result=bin_to_json_get_settings_sts(pReq);break;
		case EJSON_RPCGMGR_SHUTDOWN_SERVICE   :result=bin_to_json_shutdown_service(pReq);break;
		case EJSON_RPCGMGR_RESET_TASK_STS     :result=bin_to_json_reset_task_sts(pReq);break;
		case EJSON_RPCGMGR_GET_READY_STS      :result=bin_to_json_get_ready_sts(pReq);break;
		case EJSON_RPCGMGR_GET_DEBUG_LOG      :result=bin_to_json_get_debug_logging(pReq);break;
		case EJSON_RPCGMGR_SET_DEBUG_LOG      :result=bin_to_json_set_debug_logging(pReq);break;
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
		case EJSON_RPCGMGR_GET_TASK_STS       :return process_get_task_status(pPanelReq);break;
		case EJSON_RPCGMGR_GET_RPC_SRV_VERSION:return process_rpc_server_version(pPanelReq);break;
		case EJSON_RPCGMGR_TRIGGER_DATASAVE   :return process_trigger_datasave(pPanelReq);break;
		case EJSON_RPCGMGR_GET_SETTINGS_STS   :return process_get_settings_status(pPanelReq);break;
		case EJSON_RPCGMGR_SHUTDOWN_SERVICE   :return process_shutdown_service(pPanelReq);break;
		case EJSON_RPCGMGR_RESET_TASK_STS     :return process_reset_task_sts(pPanelReq);break;
		case EJSON_RPCGMGR_GET_READY_STS      :return process_get_ready_status(pPanelReq);break;
		case EJSON_RPCGMGR_GET_DEBUG_LOG      :return process_get_debug_logging(pPanelReq);break;
		case EJSON_RPCGMGR_SET_DEBUG_LOG      :return process_set_debug_logging(pPanelReq);break;
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
			pReq->result=RPC_SRV_RESULT_FAIL;
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
