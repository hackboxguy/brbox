#include "SmsRpc.h"
#include "SmsMgr.h"
#define ASYNC_TASK_EVENT_DELAY 200000 //needed because event might go too fast to subscriber before being handled in a proper way

/* ------------------------------------------------------------------------- */
SmsRpc:: SmsRpc(std::string rpcName,int myIndex,bool emu, bool log,BBOXSMS_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
	pDataCache=pData;	
}
/* ------------------------------------------------------------------------- */
SmsRpc::~ SmsRpc()
{
}
/* ------------------------------------------------------------------------- */
int SmsRpc::MapJsonToBinary(JsonDataCommObj* pReq,int index)
{
	EJSON_BBOXSMS_RPC_TYPES command =(EJSON_BBOXSMS_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_BBOXSMS_RPC_GET_ASYNCTASK   :return json_to_bin_get_async_task_in_progress(pReq);
		case EJSON_BBOXSMS_RPC_SMS_DELETE_ALL  :return json_to_bin_delete_all(pReq);
		case EJSON_BBOXSMS_RPC_SMS_DELETE      :break;
		case EJSON_BBOXSMS_RPC_SMS_TOTAL_GET   :return json_to_bin_get_total_sms(pReq);
		case EJSON_BBOXSMS_RPC_SMS_GET         :return json_to_bin_get_sms(pReq);
		case EJSON_BBOXSMS_RPC_SMS_SEND        :return json_to_bin_send_sms(pReq);
		case EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE :return json_to_bin_sms_list_update(pReq);
		case EJSON_BBOXSMS_RPC_SMS_IDENTIFY_DEV:return json_to_bin_ident_device(pReq);
		case EJSON_BBOXSMS_RPC_DIAL_VOICE      :return json_to_bin_dial_voice(pReq);
		case EJSON_BBOXSMS_RPC_DIAL_USSD       :return json_to_bin_dial_ussd(pReq);
		case EJSON_BBOXSMS_RPC_USSD_GET        :return json_to_bin_get_ussd(pReq);
		default:break;
	}
	return -1;//0;
}
/* ------------------------------------------------------------------------- */
int SmsRpc::MapBinaryToJson(JsonDataCommObj* pReq,int index)
{
	EJSON_BBOXSMS_RPC_TYPES command =(EJSON_BBOXSMS_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_BBOXSMS_RPC_GET_ASYNCTASK   :return bin_to_json_get_async_task_in_progress(pReq);
		case EJSON_BBOXSMS_RPC_SMS_DELETE_ALL  :return bin_to_json_delete_all(pReq);
		case EJSON_BBOXSMS_RPC_SMS_DELETE      :break;
		case EJSON_BBOXSMS_RPC_SMS_TOTAL_GET   :return bin_to_json_get_total_sms(pReq);
		case EJSON_BBOXSMS_RPC_SMS_GET         :return bin_to_json_get_sms(pReq);
		case EJSON_BBOXSMS_RPC_SMS_SEND        :return bin_to_json_send_sms(pReq);
		case EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE :return bin_to_json_sms_list_update(pReq);
		case EJSON_BBOXSMS_RPC_SMS_IDENTIFY_DEV:return bin_to_json_ident_device(pReq);
		case EJSON_BBOXSMS_RPC_DIAL_VOICE      :return bin_to_json_dial_voice(pReq);
		case EJSON_BBOXSMS_RPC_DIAL_USSD       :return bin_to_json_dial_ussd(pReq);
		case EJSON_BBOXSMS_RPC_USSD_GET        :return bin_to_json_get_ussd(pReq);
		default:break;
	}
	return -1;//0;
}
/* ------------------------------------------------------------------------- */
int SmsRpc::ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj)
{
	EJSON_BBOXSMS_RPC_TYPES command =(EJSON_BBOXSMS_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_BBOXSMS_RPC_GET_ASYNCTASK   :return process_get_async_task_in_progress(pReq);
		case EJSON_BBOXSMS_RPC_SMS_DELETE_ALL  :return process_delete_all(pReq,pObj);
		case EJSON_BBOXSMS_RPC_SMS_DELETE      :break;
		case EJSON_BBOXSMS_RPC_SMS_TOTAL_GET   :return process_get_total_sms(pReq);
		case EJSON_BBOXSMS_RPC_SMS_GET         :return process_get_sms(pReq);
		case EJSON_BBOXSMS_RPC_SMS_SEND        :return process_send_sms(pReq,pObj);
		case EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE :return process_sms_list_update(pReq,pObj);
		case EJSON_BBOXSMS_RPC_SMS_IDENTIFY_DEV:return process_ident_device(pReq,pObj);
		case EJSON_BBOXSMS_RPC_DIAL_VOICE      :return process_dial_voice(pReq,pObj);
		case EJSON_BBOXSMS_RPC_DIAL_USSD       :return process_dial_ussd(pReq,pObj);
		case EJSON_BBOXSMS_RPC_USSD_GET        :return process_get_ussd(pReq);
		default:break;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
BBOXSMS_ASYNCTASK_TYPE SmsRpc::get_async_task_in_progress()
{
	//!!!!!!!important: when a new async task is added to ProcessWorkAsync(),
	//ensure that it is also added in this function
	BBOXSMS_ASYNCTASK_TYPE task=BBOXSMS_ASYNCTASK_UNKNOWN;
	switch(pDataCache->AsyncCmdInProgress)
	{
		case EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE :task=BBOXSMS_ASYNCTASK_LIST_UPDATE;break;
		case EJSON_BBOXSMS_RPC_SMS_IDENTIFY_DEV:task=BBOXSMS_ASYNCTASK_IDENTIFY_DEV;break;
		case EJSON_BBOXSMS_RPC_SMS_DELETE_ALL  :task=BBOXSMS_ASYNCTASK_DELETE_ALL;break;
		case EJSON_BBOXSMS_RPC_SMS_SEND        :task=BBOXSMS_ASYNCTASK_SMS_SEND;break;
		case EJSON_BBOXSMS_RPC_DIAL_VOICE      :task=BBOXSMS_ASYNCTASK_DIAL_VOICE;break;
		case EJSON_BBOXSMS_RPC_DIAL_USSD       :task=BBOXSMS_ASYNCTASK_DIAL_USSD;break;
		default                                :break;
	}
	return task;
}
RPC_SRV_RESULT SmsRpc::ProcessWorkAsync(int cmd,unsigned char* pWorkData)
{
	//!!!!!!!important: when a new async task is added in this function,
	//ensure that it is also added in get_async_task_in_progress()
	RPC_SRV_RESULT ret_val=RPC_SRV_RESULT_FAIL;
	pDataCache->AsyncCmdInProgress=(EJSON_BBOXSMS_RPC_TYPES)cmd;//To know which async command is in progress
	switch(cmd)
	{
		case EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE:
			{
				BBOXSMS_SMS_PACKET *pPacket;
				pPacket=(BBOXSMS_SMS_PACKET*)pWorkData;
				ret_val=process_async_list_update(pPacket);
				OBJ_MEM_DELETE(pWorkData);
			}
			break;
		case EJSON_BBOXSMS_RPC_SMS_IDENTIFY_DEV:
			{
				BBOXSMS_SMS_PACKET *pPacket;
				pPacket=(BBOXSMS_SMS_PACKET*)pWorkData;
				ret_val=process_async_ident_device(pPacket);
				OBJ_MEM_DELETE(pWorkData);
			}
			break;
		case EJSON_BBOXSMS_RPC_SMS_DELETE_ALL:
			{
				BBOXSMS_SMS_PACKET *pPacket;
				pPacket=(BBOXSMS_SMS_PACKET*)pWorkData;
				ret_val=process_async_delete_all(pPacket);
				//after deleting sms from sim-card, empty my local cache list
				SmsMgr *pMgr=(SmsMgr*)pDataCache->pSmsMgr;
				pMgr->LogFlag=get_debug_log_flag();
				pMgr->EmptySmsList();	
				OBJ_MEM_DELETE(pWorkData);
			}
			break;
		case EJSON_BBOXSMS_RPC_SMS_SEND:
			{
				BBOXSMS_SMS_PACKET *pPacket;
				pPacket=(BBOXSMS_SMS_PACKET*)pWorkData;
				ret_val=process_async_send_sms(pPacket);
				OBJ_MEM_DELETE(pWorkData);
			}
			break;
		case EJSON_BBOXSMS_RPC_DIAL_VOICE:
			{
				BBOXSMS_SMS_PACKET *pPacket;
				pPacket=(BBOXSMS_SMS_PACKET*)pWorkData;
				ret_val=process_async_dial_voice(pPacket);
				OBJ_MEM_DELETE(pWorkData);
			}
			break;
		case EJSON_BBOXSMS_RPC_DIAL_USSD:
			{
				BBOXSMS_SMS_PACKET *pPacket;
				pPacket=(BBOXSMS_SMS_PACKET*)pWorkData;
				ret_val=process_async_dial_ussd(pPacket);
				OBJ_MEM_DELETE(pWorkData);
			}
			break;
		default:
			break;
	}
	pDataCache->AsyncCmdInProgress=EJSON_BBOXSMS_RPC_NONE;
	return ret_val;
}
/* ------------------------------------------------------------------------- */
int SmsRpc::json_to_bin_get_async_task_in_progress(JsonDataCommObj* pReq)
{
	BBOXSMS_ASYNCTASK_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,BBOXSMS_ASYNCTASK_PACKET,RPC_SRV_ACT_READ,EJSON_BBOXSMS_RPC_GET_ASYNCTASK);
	return 0;
}
int SmsRpc::bin_to_json_get_async_task_in_progress(JsonDataCommObj* pReq)
{
PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,BBOXSMS_ASYNCTASK_PACKET,BBOXSMS_RPC_ASYNCTASK_ARG,task,BBOXSMS_RPC_ASYNCTASK_ARG_TABL,BBOXSMS_ASYNCTASK_UNKNOWN);
	return 0;
}
int SmsRpc::process_get_async_task_in_progress(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	BBOXSMS_ASYNCTASK_PACKET* pPacket;
	pPacket=(BBOXSMS_ASYNCTASK_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->action!=RPC_SRV_ACT_READ)
	{
		pPanelReq->result=RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
		return 0;
	}
	pPacket->task=get_async_task_in_progress();
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
/* ------------------------------------------------------------------------- */
int SmsRpc::json_to_bin_delete_all(JsonDataCommObj* pReq)
{
	BBOXSMS_SMS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,BBOXSMS_SMS_PACKET,RPC_SRV_ACT_WRITE,EJSON_BBOXSMS_RPC_SMS_DELETE_ALL);
	return 0;
}
int SmsRpc::bin_to_json_delete_all(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_IN_PROG(RPC_SRV_REQ,BBOXSMS_SMS_PACKET,RPCMGR_RPC_TASK_STS_ARGID);
	return 0;
}
int SmsRpc::process_delete_all(JsonDataCommObj* pReq,ADJsonRpcMgrProducer* pObj)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	BBOXSMS_SMS_PACKET* pPacket;
	pPacket=(BBOXSMS_SMS_PACKET*)pPanelReq->dataRef;
	BBOXSMS_SMS_PACKET* pWorkData=NULL;
	OBJECT_MEM_NEW(pWorkData,BBOXSMS_SMS_PACKET);//delete this object in run_work() callback function
	if(pWorkData == NULL)
	{
		pPanelReq->result=RPC_SRV_RESULT_MEM_ERROR;
		return -1;
	}
	//pWorkData->operation=pPacket->operation;
	pPanelReq->result=pObj->PushAsyncTask(EJSON_BBOXSMS_RPC_SMS_DELETE_ALL,(unsigned char*)pWorkData,&pPacket->taskID,WORK_CMD_AFTER_DONE_PRESERVE);
	if(pPanelReq->result!=RPC_SRV_RESULT_IN_PROG)
		OBJ_MEM_DELETE(pWorkData);
	return 0;
}
RPC_SRV_RESULT SmsRpc::process_async_delete_all(BBOXSMS_SMS_PACKET* pPacket)
{
	usleep(ASYNC_TASK_EVENT_DELAY);//needed because event might go too fast to subscriber before being handled in a proper way
	SmsMgr *pMgr=(SmsMgr*)pDataCache->pSmsMgr;
	pMgr->LogFlag=get_debug_log_flag();
	if(pMgr->DeleteAllSMS(1)!=0) //0,1,2,3,4(5 fails)
		return RPC_SRV_RESULT_FAIL;
	if(pMgr->DeleteAllSMS(2)!=0) 
		return RPC_SRV_RESULT_FAIL;
	if(pMgr->DeleteAllSMS(3)!=0) 
		return RPC_SRV_RESULT_FAIL;
	if(pMgr->DeleteAllSMS(4)!=0) 
		return RPC_SRV_RESULT_FAIL;
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
int SmsRpc::json_to_bin_get_sms(JsonDataCommObj* pReq)
{
	BBOXSMS_SMS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,BBOXSMS_SMS_PACKET,RPC_SRV_ACT_READ,EJSON_BBOXSMS_RPC_SMS_GET);
	JSON_STRING_TO_INT(BBOXSMS_RPC_SMS_ARG_INDX,pPanelCmdObj->index);
	return 0;
}
int SmsRpc::bin_to_json_get_sms(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,BBOXSMS_SMS_PACKET,BBOXSMS_RPC_SMS_ARG_MSG,sms);
	return 0;
}
int SmsRpc::process_get_sms(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	BBOXSMS_SMS_PACKET* pPacket;
	pPacket=(BBOXSMS_SMS_PACKET*)pPanelReq->dataRef;
	SmsMgr *pMgr=(SmsMgr*)pDataCache->pSmsMgr;
	pMgr->LogFlag=get_debug_log_flag();
	pPanelReq->result=pMgr->GetSms(pPacket->index,pPacket->sms);	
	return 0;
}
/* ------------------------------------------------------------------------- */
int SmsRpc::json_to_bin_get_total_sms(JsonDataCommObj* pReq)
{
	BBOXSMS_SMS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,BBOXSMS_SMS_PACKET,RPC_SRV_ACT_READ,EJSON_BBOXSMS_RPC_SMS_TOTAL_GET);
	return 0;
}
int SmsRpc::bin_to_json_get_total_sms(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_INT(RPC_SRV_REQ,BBOXSMS_SMS_PACKET,BBOXSMS_RPC_SMS_ARG_TOTAL,total_sms);
	return 0;
}
int SmsRpc::process_get_total_sms(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	BBOXSMS_SMS_PACKET* pPacket;
	pPacket=(BBOXSMS_SMS_PACKET*)pPanelReq->dataRef;
	SmsMgr *pMgr=(SmsMgr*)pDataCache->pSmsMgr;
	pMgr->LogFlag=get_debug_log_flag();
	pPanelReq->result=pMgr->GetTotalSms(&pPacket->total_sms);

	//pPacket->total_sms=pMgr->ReadSms(0);
	//pPacket->total_sms=pMgr->DetectSmsDevice();
	//pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
/* ------------------------------------------------------------------------- */
int SmsRpc::json_to_bin_sms_list_update(JsonDataCommObj* pReq)
{
	BBOXSMS_SMS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,BBOXSMS_SMS_PACKET,RPC_SRV_ACT_WRITE,EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE);
	return 0;
}
int SmsRpc::bin_to_json_sms_list_update(JsonDataCommObj* pReq)
{
	//PREPARE_JSON_RESP(RPC_SRV_REQ,BBOXSMS_SMS_PACKET);
	PREPARE_JSON_RESP_IN_PROG(RPC_SRV_REQ,BBOXSMS_SMS_PACKET,RPCMGR_RPC_TASK_STS_ARGID);
	return 0;
}
int SmsRpc::process_sms_list_update(JsonDataCommObj* pReq,ADJsonRpcMgrProducer* pObj)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	BBOXSMS_SMS_PACKET* pPacket;
	pPacket=(BBOXSMS_SMS_PACKET*)pPanelReq->dataRef;

	BBOXSMS_SMS_PACKET* pWorkData=NULL;
	OBJECT_MEM_NEW(pWorkData,BBOXSMS_SMS_PACKET);//delete this object in run_work() callback function
	if(pWorkData == NULL)
	{
		pPanelReq->result=RPC_SRV_RESULT_MEM_ERROR;
		return -1;
	}
	//pWorkData->operation=pPacket->operation;
	pPanelReq->result=pObj->PushAsyncTask(EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE,(unsigned char*)pWorkData,&pPacket->taskID,WORK_CMD_AFTER_DONE_PRESERVE);//WORK_CMD_AFTER_DONE_DELETE);
	if(pPanelReq->result!=RPC_SRV_RESULT_IN_PROG)
		OBJ_MEM_DELETE(pWorkData);
	//pPanelReq->result=RPC_SRV_RESULT_SUCCESS;//although, this is async type, but internally taskID object is auto-deleted after completion
	return 0;
}
RPC_SRV_RESULT SmsRpc::process_async_list_update(BBOXSMS_SMS_PACKET* pPacket)
{
	usleep(ASYNC_TASK_EVENT_DELAY);//needed because event might go too fast to subscriber before being handled in a proper way
	SmsMgr *pMgr=(SmsMgr*)pDataCache->pSmsMgr;
	pMgr->LogFlag=get_debug_log_flag();
	if(pMgr->ReadSms(0)<0)
		return RPC_SRV_RESULT_FAIL;
	else
		return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
int SmsRpc::json_to_bin_ident_device(JsonDataCommObj* pReq)
{
	BBOXSMS_SMS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,BBOXSMS_SMS_PACKET,RPC_SRV_ACT_WRITE,EJSON_BBOXSMS_RPC_SMS_IDENTIFY_DEV);
	return 0;
}
int SmsRpc::bin_to_json_ident_device(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_IN_PROG(RPC_SRV_REQ,BBOXSMS_SMS_PACKET,RPCMGR_RPC_TASK_STS_ARGID);
	return 0;
}
int SmsRpc::process_ident_device(JsonDataCommObj* pReq,ADJsonRpcMgrProducer* pObj)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	BBOXSMS_SMS_PACKET* pPacket;
	pPacket=(BBOXSMS_SMS_PACKET*)pPanelReq->dataRef;
	BBOXSMS_SMS_PACKET* pWorkData=NULL;
	OBJECT_MEM_NEW(pWorkData,BBOXSMS_SMS_PACKET);//delete this object in run_work() callback function
	if(pWorkData == NULL)
	{
		pPanelReq->result=RPC_SRV_RESULT_MEM_ERROR;
		return -1;
	}
	//pWorkData->operation=pPacket->operation;
	pPanelReq->result=pObj->PushAsyncTask(EJSON_BBOXSMS_RPC_SMS_IDENTIFY_DEV,(unsigned char*)pWorkData,&pPacket->taskID,WORK_CMD_AFTER_DONE_PRESERVE);
	if(pPanelReq->result!=RPC_SRV_RESULT_IN_PROG)
		OBJ_MEM_DELETE(pWorkData);
	return 0;
}
RPC_SRV_RESULT SmsRpc::process_async_ident_device(BBOXSMS_SMS_PACKET* pPacket)
{
	usleep(ASYNC_TASK_EVENT_DELAY);//needed because event might go too fast to subscriber before being handled in a proper way
	SmsMgr *pMgr=(SmsMgr*)pDataCache->pSmsMgr;
	pMgr->LogFlag=get_debug_log_flag();
	if(pMgr->DetectSmsDevice()==0)
		return RPC_SRV_RESULT_SUCCESS;
	else
		return RPC_SRV_RESULT_FAIL;
}
/* ------------------------------------------------------------------------- */
int SmsRpc::json_to_bin_send_sms(JsonDataCommObj* pReq)
{
	BBOXSMS_SMS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,BBOXSMS_SMS_PACKET,RPC_SRV_ACT_WRITE,EJSON_BBOXSMS_RPC_SMS_SEND);
	JSON_STRING_TO_STRING(BBOXSMS_RPC_SMS_ARG_DEST,pPanelCmdObj->destNum);
	JSON_STRING_TO_STRING(BBOXSMS_RPC_SMS_ARG_MSG,pPanelCmdObj->sms);
	return 0;
}
int SmsRpc::bin_to_json_send_sms(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_IN_PROG(RPC_SRV_REQ,BBOXSMS_SMS_PACKET,RPCMGR_RPC_TASK_STS_ARGID);
	return 0;
}
int SmsRpc::process_send_sms(JsonDataCommObj* pReq,ADJsonRpcMgrProducer* pObj)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	BBOXSMS_SMS_PACKET* pPacket;
	pPacket=(BBOXSMS_SMS_PACKET*)pPanelReq->dataRef;

	BBOXSMS_SMS_PACKET* pWorkData=NULL;
	OBJECT_MEM_NEW(pWorkData,BBOXSMS_SMS_PACKET);//delete this object in run_work() callback function
	if(pWorkData == NULL)
	{
		pPanelReq->result=RPC_SRV_RESULT_MEM_ERROR;
		return -1;
	}
	strcpy(pWorkData->destNum,pPacket->destNum);
	strcpy(pWorkData->sms,pPacket->sms);
	pPanelReq->result=pObj->PushAsyncTask(EJSON_BBOXSMS_RPC_SMS_SEND,(unsigned char*)pWorkData,&pPacket->taskID,WORK_CMD_AFTER_DONE_PRESERVE);
	if(pPanelReq->result!=RPC_SRV_RESULT_IN_PROG)
		OBJ_MEM_DELETE(pWorkData);
	return 0;
}
RPC_SRV_RESULT SmsRpc::process_async_send_sms(BBOXSMS_SMS_PACKET* pPacket)
{
	usleep(ASYNC_TASK_EVENT_DELAY);//needed because event might go too fast to subscriber before being handled in a proper way
	SmsMgr *pMgr=(SmsMgr*)pDataCache->pSmsMgr;
	pMgr->LogFlag=get_debug_log_flag();
	if(pMgr->SendSms(pPacket->destNum,pPacket->sms)!=0)
	//if(pMgr->DialUSSDCode(pPacket->destNum,pPacket->sms)!=0)
		return RPC_SRV_RESULT_FAIL;
	else
		return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
int SmsRpc::json_to_bin_dial_voice(JsonDataCommObj* pReq)
{
	BBOXSMS_SMS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,BBOXSMS_SMS_PACKET,RPC_SRV_ACT_WRITE,EJSON_BBOXSMS_RPC_DIAL_VOICE);
	JSON_STRING_TO_STRING(BBOXSMS_RPC_SMS_ARG_DEST,pPanelCmdObj->destNum);
	return 0;
}
int SmsRpc::bin_to_json_dial_voice(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_IN_PROG(RPC_SRV_REQ,BBOXSMS_SMS_PACKET,RPCMGR_RPC_TASK_STS_ARGID);
	return 0;
}
int SmsRpc::process_dial_voice(JsonDataCommObj* pReq,ADJsonRpcMgrProducer* pObj)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	BBOXSMS_SMS_PACKET* pPacket;
	pPacket=(BBOXSMS_SMS_PACKET*)pPanelReq->dataRef;

	BBOXSMS_SMS_PACKET* pWorkData=NULL;
	OBJECT_MEM_NEW(pWorkData,BBOXSMS_SMS_PACKET);//delete this object in run_work() callback function
	if(pWorkData == NULL)
	{
		pPanelReq->result=RPC_SRV_RESULT_MEM_ERROR;
		return -1;
	}
	strcpy(pWorkData->destNum,pPacket->destNum);
	pPanelReq->result=pObj->PushAsyncTask(EJSON_BBOXSMS_RPC_DIAL_VOICE,(unsigned char*)pWorkData,&pPacket->taskID,WORK_CMD_AFTER_DONE_PRESERVE);
	if(pPanelReq->result!=RPC_SRV_RESULT_IN_PROG)
		OBJ_MEM_DELETE(pWorkData);
	return 0;
}
RPC_SRV_RESULT SmsRpc::process_async_dial_voice(BBOXSMS_SMS_PACKET* pPacket)
{
	usleep(ASYNC_TASK_EVENT_DELAY);//needed because event might go too fast to subscriber before being handled in a proper way
	//char ret_val[1024];
	SmsMgr *pMgr=(SmsMgr*)pDataCache->pSmsMgr;
	pMgr->LogFlag=get_debug_log_flag();
	if(pMgr->DialVoice(pPacket->destNum)!=0)
	//if(pMgr->DialUSSDCode(pPacket->destNum,ret_val)!=0)
		return RPC_SRV_RESULT_FAIL;
	else
		return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
int SmsRpc::json_to_bin_dial_ussd(JsonDataCommObj* pReq)
{
	BBOXSMS_SMS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,BBOXSMS_SMS_PACKET,RPC_SRV_ACT_WRITE,EJSON_BBOXSMS_RPC_DIAL_USSD);
	JSON_STRING_TO_STRING(BBOXSMS_RPC_SMS_ARG_DEST,pPanelCmdObj->destNum);
	return 0;
}
int SmsRpc::bin_to_json_dial_ussd(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_IN_PROG(RPC_SRV_REQ,BBOXSMS_SMS_PACKET,RPCMGR_RPC_TASK_STS_ARGID);
	return 0;
}
int SmsRpc::process_dial_ussd(JsonDataCommObj* pReq,ADJsonRpcMgrProducer* pObj)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	BBOXSMS_SMS_PACKET* pPacket;
	pPacket=(BBOXSMS_SMS_PACKET*)pPanelReq->dataRef;

	BBOXSMS_SMS_PACKET* pWorkData=NULL;
	OBJECT_MEM_NEW(pWorkData,BBOXSMS_SMS_PACKET);//delete this object in run_work() callback function
	if(pWorkData == NULL)
	{
		pPanelReq->result=RPC_SRV_RESULT_MEM_ERROR;
		return -1;
	}
	strcpy(pWorkData->destNum,pPacket->destNum);
	pPanelReq->result=pObj->PushAsyncTask(EJSON_BBOXSMS_RPC_DIAL_USSD,(unsigned char*)pWorkData,&pPacket->taskID,WORK_CMD_AFTER_DONE_PRESERVE);
	if(pPanelReq->result!=RPC_SRV_RESULT_IN_PROG)
		OBJ_MEM_DELETE(pWorkData);
	return 0;
}
RPC_SRV_RESULT SmsRpc::process_async_dial_ussd(BBOXSMS_SMS_PACKET* pPacket)
{
	usleep(ASYNC_TASK_EVENT_DELAY);//needed because event might go too fast to subscriber before being handled in a proper way
	char ret_val[1024];
	SmsMgr *pMgr=(SmsMgr*)pDataCache->pSmsMgr;
	pMgr->LogFlag=get_debug_log_flag();
	//if(pMgr->DialVoice(pPacket->destNum)!=0)
	if(pMgr->DialUSSDCode(pPacket->destNum,ret_val)!=0)
		return RPC_SRV_RESULT_FAIL;
	else
		return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
int SmsRpc::json_to_bin_get_ussd(JsonDataCommObj* pReq)
{
	BBOXSMS_SMS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,BBOXSMS_SMS_PACKET,RPC_SRV_ACT_READ,EJSON_BBOXSMS_RPC_USSD_GET);
	return 0;
}
int SmsRpc::bin_to_json_get_ussd(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,BBOXSMS_SMS_PACKET,BBOXSMS_RPC_SMS_ARG_MSG,sms);
	return 0;
}
int SmsRpc::process_get_ussd(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	BBOXSMS_SMS_PACKET* pPacket;
	pPacket=(BBOXSMS_SMS_PACKET*)pPanelReq->dataRef;
	SmsMgr *pMgr=(SmsMgr*)pDataCache->pSmsMgr;
	pMgr->LogFlag=get_debug_log_flag();
	pPanelReq->result=pMgr->GetLatestUSSDReply(pPacket->sms);	
	return 0;
}
/* ------------------------------------------------------------------------- */


