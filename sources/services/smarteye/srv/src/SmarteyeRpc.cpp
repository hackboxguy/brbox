#include "SmarteyeRpc.h"
#include "ImgIdentify.h"
/* ------------------------------------------------------------------------- */
SmarteyeRpc:: SmarteyeRpc(std::string rpcName,int myIndex,bool emu, bool log,SMARTEYE_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
	pDataCache=pData;	
}
/* ------------------------------------------------------------------------- */
SmarteyeRpc::~ SmarteyeRpc()
{
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::MapJsonToBinary(JsonDataCommObj* pReq,int index)
{
	EJSON_SMARTEYE_RPC_TYPES command =(EJSON_SMARTEYE_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN      :return json_to_bin_check_id_pattern(pReq);
		case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET     :return json_to_bin_get_debug_outfile(pReq);
		case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_SET     :return json_to_bin_set_debug_outfile(pReq);
		case EJSON_SMARTEYE_RPC_CAPTURE_JPG_IMG       :return json_to_bin_capture_jpg_img(pReq);
		case EJSON_SMARTEYE_RPC_CAPTURE_RESOLUTION_SET:return json_to_bin_set_capture_resolution(pReq);
		default:break;
	}
	return -1;//0;
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::MapBinaryToJson(JsonDataCommObj* pReq,int index)
{
	EJSON_SMARTEYE_RPC_TYPES command =(EJSON_SMARTEYE_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN      :return bin_to_json_check_id_pattern(pReq);
		case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET     :return bin_to_json_get_debug_outfile(pReq);
		case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_SET     :return bin_to_json_set_debug_outfile(pReq);
		case EJSON_SMARTEYE_RPC_CAPTURE_JPG_IMG       :return bin_to_json_capture_jpg_img(pReq);
		case EJSON_SMARTEYE_RPC_CAPTURE_RESOLUTION_SET:return bin_to_json_set_capture_resolution(pReq);
		default:break;
	}
	return -1;
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj)
{
	EJSON_SMARTEYE_RPC_TYPES command =(EJSON_SMARTEYE_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN      :return process_check_id_pattern(pReq);
		case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET     :return process_get_debug_outfile(pReq);
		case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_SET     :return process_set_debug_outfile(pReq);
		case EJSON_SMARTEYE_RPC_CAPTURE_JPG_IMG       :return process_capture_jpg_img(pReq);
		case EJSON_SMARTEYE_RPC_CAPTURE_RESOLUTION_SET:return process_set_capture_resolution(pReq);
		default:break;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT SmarteyeRpc::ProcessWorkAsync(int index,unsigned char* pWorkData)
{
	RPC_SRV_RESULT ret_val=RPC_SRV_RESULT_FAIL;

	return ret_val;
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::json_to_bin_check_id_pattern(JsonDataCommObj* pReq)
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
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::json_to_bin_get_debug_outfile(JsonDataCommObj* pReq)
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
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::json_to_bin_set_debug_outfile(JsonDataCommObj* pReq)
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
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::json_to_bin_capture_jpg_img(JsonDataCommObj* pReq)
{
	SMARTEYE_DEBUG_OUTFILE_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_DEBUG_OUTFILE_PACKET,RPC_SRV_ACT_WRITE,EJSON_SMARTEYE_RPC_CAPTURE_JPG_IMG);
	//JSON_STRING_TO_STRING(SMARTEYE_RPC_DEBUG_OUTFILE_ARG,pPanelCmdObj->filepath);
	return 0;
}
int SmarteyeRpc::bin_to_json_capture_jpg_img(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,SMARTEYE_DEBUG_OUTFILE_PACKET);
	//PREPARE_JSON_RESP_IN_PROG(RPC_SRV_REQ,SMARTEYE_DEBUG_OUTFILE_PACKET,RPCMGR_RPC_TASK_STS_ARGID);
	return 0;
}
int SmarteyeRpc::process_capture_jpg_img(JsonDataCommObj* pReq)
{
	ImgIdentify ImgId;//opencv based class for image processing
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_DEBUG_OUTFILE_PACKET* pPacket;
	pPacket=(SMARTEYE_DEBUG_OUTFILE_PACKET*)pPanelReq->dataRef;
	//ImgId.logflag=get_debug_log_flag();//get_debug_log_flag() is a function f parent class ADJsonRpcMgrConsumer
	pPanelReq->result=ImgId.capture_jpg_image(pDataCache->StrImgIdDebugFile,pDataCache->captureRes.pixels,pDataCache->captureRes.lines);
	return 0;
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::json_to_bin_set_capture_resolution(JsonDataCommObj* pReq)
{
	SMARTEYE_CAPTURE_RESOLUTION_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_CAPTURE_RESOLUTION_PACKET,RPC_SRV_ACT_WRITE,EJSON_SMARTEYE_RPC_CAPTURE_RESOLUTION_SET);
	JSON_STRING_TO_INT(SMARTEYE_RPC_CAPTURE_RESOLUTION_ARGH,pPanelCmdObj->pixels);
	JSON_STRING_TO_INT(SMARTEYE_RPC_CAPTURE_RESOLUTION_ARGV,pPanelCmdObj->lines);
	return 0;
}
int SmarteyeRpc::bin_to_json_set_capture_resolution(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,SMARTEYE_CAPTURE_RESOLUTION_PACKET);
	return 0;
}
int SmarteyeRpc::process_set_capture_resolution(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_CAPTURE_RESOLUTION_PACKET* pPacket;
	pPacket=(SMARTEYE_CAPTURE_RESOLUTION_PACKET*)pPanelReq->dataRef;
	pDataCache->captureRes.pixels=pPacket->pixels;
	pDataCache->captureRes.lines =pPacket->lines;
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
/* ------------------------------------------------------------------------- */





