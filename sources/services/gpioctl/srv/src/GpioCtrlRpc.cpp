#include "GpioCtrlRpc.h"
/* ------------------------------------------------------------------------- */
GpioCtrlRpc:: GpioCtrlRpc(std::string rpcName,int myIndex,bool emu, bool log,GPIOCTL_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
	pDataCache=pData;	
}
/* ------------------------------------------------------------------------- */
GpioCtrlRpc::~ GpioCtrlRpc()
{
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::MapJsonToBinary(JsonDataCommObj* pReq,int index)
{
	EJSON_GPIOCTL_RPC_TYPES command =(EJSON_GPIOCTL_RPC_TYPES)index;
	switch(command)
	{
		//case EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN :return json_to_bin_check_id_pattern(pReq);
		//case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET:return json_to_bin_get_debug_outfile(pReq);
		//case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_SET:return json_to_bin_set_debug_outfile(pReq);
		default:break;
	}
	return -1;//0;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::MapBinaryToJson(JsonDataCommObj* pReq,int index)
{
	EJSON_GPIOCTL_RPC_TYPES command =(EJSON_GPIOCTL_RPC_TYPES)index;
	switch(command)
	{
		//case EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN :return bin_to_json_check_id_pattern(pReq);
		//case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET:return bin_to_json_get_debug_outfile(pReq);
		//case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_SET:return bin_to_json_set_debug_outfile(pReq);
		default:break;
	}
	return -1;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::ProcessWork(JsonDataCommObj* pReq,int index)
{
	EJSON_GPIOCTL_RPC_TYPES command =(EJSON_GPIOCTL_RPC_TYPES)index;
	switch(command)
	{
		//case EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN :return process_check_id_pattern(pReq);
		//case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET:return process_get_debug_outfile(pReq);
		//case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_SET:return process_set_debug_outfile(pReq);
		default:break;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::ProcessWorkAsync(unsigned char* pWorkData)
{
	return 0;
}
/* ------------------------------------------------------------------------- */
/*int SmarteyeRpc::json_to_bin_check_id_pattern(JsonDataCommObj* pReq)
{
	SMARTEYE_ID_PATTERN_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_ID_PATTERN_PACKET,RPC_SRV_ACT_WRITE,EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN);
	JSON_STRING_TO_ENUM(SMARTEYE_RPC_ID_PATTERN_CHECK_ARG,SMARTEYE_RPC_ID_PATTERN_CHECK_ARG_TABL,EJSON_SMARTEYE_IFACE_TYPE,EJSON_SMARTEYE_IDPATTERN_UNKNOWN,pPanelCmdObj->pattern_type);
	return 0;
}
int SmarteyeRpc::bin_to_json_check_id_pattern(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,SMARTEYE_ID_PATTERN_PACKET);
	return 0;
}
int SmarteyeRpc::process_check_id_pattern(JsonDataCommObj* pReq)
{
	ImgIdentify ImgId;//opencv based class for image processing
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_ID_PATTERN_PACKET* pPacket;
	pPacket=(SMARTEYE_ID_PATTERN_PACKET*)pPanelReq->dataRef;

	//set debug logging flag
	ImgId.logflag=get_debug_log_flag();//get_debug_log_flag() is a function f parent class ADJsonRpcMgrConsumer

	pPanelReq->result=ImgId.IdentifyPattern(pPacket->pattern_type,pDataCache->StrImgIdDebugFile);
	return 0;
}*/
/* ------------------------------------------------------------------------- */
/*int SmarteyeRpc::json_to_bin_get_debug_outfile(JsonDataCommObj* pReq)
{
	SMARTEYE_DEBUG_OUTFILE_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_DEBUG_OUTFILE_PACKET,RPC_SRV_ACT_READ,EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET);
	return 0;
}
int SmarteyeRpc::bin_to_json_get_debug_outfile(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,SMARTEYE_DEBUG_OUTFILE_PACKET,SMARTEYE_RPC_DEBUG_OUTFILE_ARG,filepath);
	return 0;
}
int SmarteyeRpc::process_get_debug_outfile(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_DEBUG_OUTFILE_PACKET* pPacket;
	pPacket=(SMARTEYE_DEBUG_OUTFILE_PACKET*)pPanelReq->dataRef;
	strcpy(pPacket->filepath,pDataCache->StrImgIdDebugFile.c_str());
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}*/
/* ------------------------------------------------------------------------- */
/*int SmarteyeRpc::json_to_bin_set_debug_outfile(JsonDataCommObj* pReq)
{
	SMARTEYE_DEBUG_OUTFILE_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_DEBUG_OUTFILE_PACKET,RPC_SRV_ACT_WRITE,EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_SET);
	JSON_STRING_TO_STRING(SMARTEYE_RPC_DEBUG_OUTFILE_ARG,pPanelCmdObj->filepath);
	return 0;
}
int SmarteyeRpc::bin_to_json_set_debug_outfile(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,SMARTEYE_DEBUG_OUTFILE_PACKET);
	return 0;
}
int SmarteyeRpc::process_set_debug_outfile(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_DEBUG_OUTFILE_PACKET* pPacket;
	pPacket=(SMARTEYE_DEBUG_OUTFILE_PACKET*)pPanelReq->dataRef;
	pDataCache->StrImgIdDebugFile="";///tmp/test.txt";
	pDataCache->StrImgIdDebugFile.append(pPacket->filepath);
	if(pDataCache->StrImgIdDebugFile==" ") //use single space as null string(due to limitation of client)
		pDataCache->StrImgIdDebugFile="";
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}*/
/* ------------------------------------------------------------------------- */


