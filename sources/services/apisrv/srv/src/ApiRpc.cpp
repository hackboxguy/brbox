#include "ApiRpc.h"
//#include "SmsMgr.h"
/* ------------------------------------------------------------------------- */
ApiRpc:: ApiRpc(std::string rpcName,int myIndex,bool emu, bool log,APISRV_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
	pDataCache=pData;	
}
/* ------------------------------------------------------------------------- */
ApiRpc::~ ApiRpc()
{
}
/* ------------------------------------------------------------------------- */
int ApiRpc::MapJsonToBinary(JsonDataCommObj* pReq,int index)
{
	EJSON_APISRV_RPC_TYPES command =(EJSON_APISRV_RPC_TYPES)index;
	switch(command)
	{
		//case EJSON_XMPROXY_RPC_GET_ASYNCTASK   :return json_to_bin_get_async_task_in_progress(pReq);
		//case EJSON_BBOXSMS_RPC_SMS_DELETE_ALL  :return json_to_bin_delete_all(pReq);
		//case EJSON_BBOXSMS_RPC_SMS_DELETE      :break;
		//case EJSON_BBOXSMS_RPC_SMS_TOTAL_GET   :return json_to_bin_get_total_sms(pReq);
		//case EJSON_BBOXSMS_RPC_SMS_GET         :return json_to_bin_get_sms(pReq);
		//case EJSON_BBOXSMS_RPC_SMS_SEND        :break;
		//case EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE :return json_to_bin_sms_list_update(pReq);
		//case EJSON_BBOXSMS_RPC_SMS_IDENTIFY_DEV:return json_to_bin_ident_device(pReq);
		default:break;
	}
	return -1;//0;
}
/* ------------------------------------------------------------------------- */
int ApiRpc::MapBinaryToJson(JsonDataCommObj* pReq,int index)
{
	EJSON_APISRV_RPC_TYPES command =(EJSON_APISRV_RPC_TYPES)index;
	switch(command)
	{
		//case EJSON_XMPROXY_RPC_GET_ASYNCTASK   :return bin_to_json_get_async_task_in_progress(pReq);
		//case EJSON_BBOXSMS_RPC_SMS_DELETE_ALL  :return bin_to_json_delete_all(pReq);
		//case EJSON_BBOXSMS_RPC_SMS_DELETE      :break;
		//case EJSON_BBOXSMS_RPC_SMS_TOTAL_GET   :return bin_to_json_get_total_sms(pReq);
		//case EJSON_BBOXSMS_RPC_SMS_GET         :return bin_to_json_get_sms(pReq);
		//case EJSON_BBOXSMS_RPC_SMS_SEND        :break;
		//case EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE :return bin_to_json_sms_list_update(pReq);
		//case EJSON_BBOXSMS_RPC_SMS_IDENTIFY_DEV:return bin_to_json_ident_device(pReq);
		default:break;
	}
	return -1;//0;
}
/* ------------------------------------------------------------------------- */
int ApiRpc::ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj)
{
	EJSON_APISRV_RPC_TYPES command =(EJSON_APISRV_RPC_TYPES)index;
	switch(command)
	{
		//case EJSON_XMPROXY_RPC_GET_ASYNCTASK   :return process_get_async_task_in_progress(pReq);
		//case EJSON_BBOXSMS_RPC_SMS_DELETE_ALL  :return process_delete_all(pReq,pObj);
		//case EJSON_BBOXSMS_RPC_SMS_DELETE      :break;
		//case EJSON_BBOXSMS_RPC_SMS_TOTAL_GET   :return process_get_total_sms(pReq);
		//case EJSON_BBOXSMS_RPC_SMS_GET         :return process_get_sms(pReq);
		//case EJSON_BBOXSMS_RPC_SMS_SEND        :break;
		//case EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE :return process_sms_list_update(pReq,pObj);
		//case EJSON_BBOXSMS_RPC_SMS_IDENTIFY_DEV:return process_ident_device(pReq,pObj);
		default:break;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
/*XMPROXY_ASYNCTASK_TYPE XmppRpc::get_async_task_in_progress()
{
	//!!!!!!!important: when a new async task is added to ProcessWorkAsync(),
	//ensure that it is also added in this function
	XMPROXY_ASYNCTASK_TYPE task=XMPROXY_ASYNCTASK_UNKNOWN;
	switch(pDataCache->AsyncCmdInProgress)
	{
		//case EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE :task=BBOXSMS_ASYNCTASK_LIST_UPDATE;break;
		//case EJSON_BBOXSMS_RPC_SMS_IDENTIFY_DEV:task=BBOXSMS_ASYNCTASK_IDENTIFY_DEV;break;
		//case EJSON_BBOXSMS_RPC_SMS_DELETE_ALL  :task=BBOXSMS_ASYNCTASK_DELETE_ALL;break;
		default                               :break;
	}
	return task;
}*/
RPC_SRV_RESULT ApiRpc::ProcessWorkAsync(int cmd,unsigned char* pWorkData)
{
	//!!!!!!!important: when a new async task is added in this function,
	//ensure that it is also added in get_async_task_in_progress()
	RPC_SRV_RESULT ret_val=RPC_SRV_RESULT_FAIL;
	pDataCache->AsyncCmdInProgress=(EJSON_APISRV_RPC_TYPES)cmd;//To know which async command is in progress
	switch(cmd)
	{
		/*case EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE:
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
				OBJ_MEM_DELETE(pWorkData);
			}
			break;*/
		default:
			break;
	}
	pDataCache->AsyncCmdInProgress=EJSON_APISRV_RPC_NONE;
	return ret_val;
}
/* ------------------------------------------------------------------------- */
/*int ApiRpc::json_to_bin_get_async_task_in_progress(JsonDataCommObj* pReq)
{
	XMPROXY_ASYNCTASK_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,XMPROXY_ASYNCTASK_PACKET,RPC_SRV_ACT_READ,EJSON_XMPROXY_RPC_GET_ASYNCTASK);
	return 0;
}
int ApiRpc::bin_to_json_get_async_task_in_progress(JsonDataCommObj* pReq)
{
PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,XMPROXY_ASYNCTASK_PACKET,XMPROXY_RPC_ASYNCTASK_ARG,task,XMPROXY_RPC_ASYNCTASK_ARG_TABL,XMPROXY_ASYNCTASK_UNKNOWN);
	return 0;
}
int ApiRpc::process_get_async_task_in_progress(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	XMPROXY_ASYNCTASK_PACKET* pPacket;
	pPacket=(XMPROXY_ASYNCTASK_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->action!=RPC_SRV_ACT_READ)
	{
		pPanelReq->result=RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
		return 0;
	}
	pPacket->task=get_async_task_in_progress();
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}*/
/* ------------------------------------------------------------------------- */


