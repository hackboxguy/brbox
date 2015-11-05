#include "SmsRpc.h"
#include "SmsMgr.h"
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
		case EJSON_BBOXSMS_RPC_SMS_DELETE_ALL :return json_to_bin_delete_all(pReq);
		case EJSON_BBOXSMS_RPC_SMS_DELETE     :break;
		case EJSON_BBOXSMS_RPC_SMS_TOTAL_GET  :return json_to_bin_get_total_sms(pReq);
		case EJSON_BBOXSMS_RPC_SMS_GET        :return json_to_bin_get_sms(pReq);
		case EJSON_BBOXSMS_RPC_SMS_SEND       :break;
		case EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE:break;
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
		case EJSON_BBOXSMS_RPC_SMS_DELETE_ALL :return bin_to_json_delete_all(pReq);
		case EJSON_BBOXSMS_RPC_SMS_DELETE     :break;
		case EJSON_BBOXSMS_RPC_SMS_TOTAL_GET  :return bin_to_json_get_total_sms(pReq);
		case EJSON_BBOXSMS_RPC_SMS_GET        :return bin_to_json_get_sms(pReq);
		case EJSON_BBOXSMS_RPC_SMS_SEND       :break;
		case EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE:break;
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
		case EJSON_BBOXSMS_RPC_SMS_DELETE_ALL :return process_delete_all(pReq);
		case EJSON_BBOXSMS_RPC_SMS_DELETE     :break;
		case EJSON_BBOXSMS_RPC_SMS_TOTAL_GET  :return process_get_total_sms(pReq);
		case EJSON_BBOXSMS_RPC_SMS_GET        :return process_get_sms(pReq);
		case EJSON_BBOXSMS_RPC_SMS_SEND       :break;
		case EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE:break;
		default:break;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT SmsRpc::ProcessWorkAsync(int index,unsigned char* pWorkData)
{
	EJSON_BBOXSMS_RPC_TYPES command =(EJSON_BBOXSMS_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_BBOXSMS_RPC_SMS_DELETE_ALL :break;//return process_delete_all(pReq);
		case EJSON_BBOXSMS_RPC_SMS_DELETE     :break;
		case EJSON_BBOXSMS_RPC_SMS_TOTAL_GET  :break;//return process_get_total_sms(pReq);
		case EJSON_BBOXSMS_RPC_SMS_GET        :break;//return process_get_sms(pReq);
		case EJSON_BBOXSMS_RPC_SMS_SEND       :break;
		case EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE:break;
		default:break;
	}
	RPC_SRV_RESULT ret_val=RPC_SRV_RESULT_FAIL;
	return ret_val;
}
/* ------------------------------------------------------------------------- */
int SmsRpc::json_to_bin_delete_all(JsonDataCommObj* pReq)
{
	//SMARTEYE_ID_PATTERN_PACKET* pPanelCmdObj=NULL;
	//PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_ID_PATTERN_PACKET,RPC_SRV_ACT_WRITE,EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN);
	//JSON_STRING_TO_ENUM(SMARTEYE_RPC_ID_PATTERN_CHECK_ARG,SMARTEYE_RPC_ID_PATTERN_CHECK_ARG_TABL,EJSON_SMARTEYE_IFACE_TYPE,EJSON_SMARTEYE_IDPATTERN_UNKNOWN,pPanelCmdObj->pattern_type);
	return 0;
}
int SmsRpc::bin_to_json_delete_all(JsonDataCommObj* pReq)
{
	//PREPARE_JSON_RESP(RPC_SRV_REQ,SMARTEYE_ID_PATTERN_PACKET);
	return 0;
}
int SmsRpc::process_delete_all(JsonDataCommObj* pReq)
{
/*	ImgIdentify ImgId;//opencv based class for image processing
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_ID_PATTERN_PACKET* pPacket;
	pPacket=(SMARTEYE_ID_PATTERN_PACKET*)pPanelReq->dataRef;

	//set debug logging flag
	ImgId.logflag=get_debug_log_flag();//get_debug_log_flag() is a function f parent class ADJsonRpcMgrConsumer

	pPanelReq->result=ImgId.IdentifyPattern(pPacket->pattern_type,pDataCache->StrImgIdDebugFile);*/
	return 0;
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
	pPanelReq->result=pMgr->GetTotalSms(&pPacket->total_sms);

	//pPacket->total_sms=pMgr->ReadSms(0);
	//pPacket->total_sms=pMgr->DetectSmsDevice();
	//pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
/* ------------------------------------------------------------------------- */


