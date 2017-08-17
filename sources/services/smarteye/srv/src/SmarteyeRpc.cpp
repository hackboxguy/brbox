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
		case EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN       :return json_to_bin_check_id_pattern(pReq);
		case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET      :return json_to_bin_get_debug_outfile(pReq);
		case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_SET      :return json_to_bin_set_debug_outfile(pReq);
		case EJSON_SMARTEYE_RPC_CAPTURE_JPG_IMG        :return json_to_bin_capture_jpg_img(pReq);
		case EJSON_SMARTEYE_RPC_CAPTURE_RESOLUTION_SET :return json_to_bin_set_capture_resolution(pReq);
		case EJSON_SMARTEYE_RPC_CHECKWALL_FILE_GET     :return json_to_bin_get_checkwall_file(pReq);
		case EJSON_SMARTEYE_RPC_CHECKWALL_FILE_SET     :return json_to_bin_set_checkwall_file(pReq);
		case EJSON_SMARTEYE_RPC_CHECKWALL_BASE_FILE_GET:return json_to_bin_get_checkwallbase_file(pReq);
		case EJSON_SMARTEYE_RPC_CHECKWALL_BASE_FILE_SET:return json_to_bin_set_checkwallbase_file(pReq);
		case EJSON_SMARTEYE_RPC_SCAN_QRSTRING          :return json_to_bin_scan_qrstring(pReq);
		case EJSON_SMARTEYE_RPC_COMPARE_IMG            :return json_to_bin_compare_img(pReq);
		case EJSON_SMARTEYE_RPC_AUTO_EXPOSURE_GET      :return json_to_bin_auto_exposure_get(pReq);
		case EJSON_SMARTEYE_RPC_AUTO_EXPOSURE_SET      :return json_to_bin_auto_exposure_set(pReq);
		case EJSON_SMARTEYE_RPC_EXPOSURE_GET           :return json_to_bin_get_exposure(pReq);
		case EJSON_SMARTEYE_RPC_EXPOSURE_SET           :return json_to_bin_set_exposure(pReq);
		case EJSON_SMARTEYE_RPC_AUTO_FOCUS_GET         :return json_to_bin_auto_focus_get(pReq);
		case EJSON_SMARTEYE_RPC_AUTO_FOCUS_SET         :return json_to_bin_auto_focus_set(pReq);
		case EJSON_SMARTEYE_RPC_FOCUS_GET              :return json_to_bin_get_focus(pReq);
		case EJSON_SMARTEYE_RPC_FOCUS_SET              :return json_to_bin_set_focus(pReq);;
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
		case EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN       :return bin_to_json_check_id_pattern(pReq);
		case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET      :return bin_to_json_get_debug_outfile(pReq);
		case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_SET      :return bin_to_json_set_debug_outfile(pReq);
		case EJSON_SMARTEYE_RPC_CAPTURE_JPG_IMG        :return bin_to_json_capture_jpg_img(pReq);
		case EJSON_SMARTEYE_RPC_CAPTURE_RESOLUTION_SET :return bin_to_json_set_capture_resolution(pReq);
		case EJSON_SMARTEYE_RPC_CHECKWALL_FILE_GET     :return bin_to_json_get_checkwall_file(pReq);
		case EJSON_SMARTEYE_RPC_CHECKWALL_FILE_SET     :return bin_to_json_set_checkwall_file(pReq);
		case EJSON_SMARTEYE_RPC_CHECKWALL_BASE_FILE_GET:return bin_to_json_get_checkwallbase_file(pReq);
		case EJSON_SMARTEYE_RPC_CHECKWALL_BASE_FILE_SET:return bin_to_json_set_checkwallbase_file(pReq);
		case EJSON_SMARTEYE_RPC_SCAN_QRSTRING          :return bin_to_json_scan_qrstring(pReq);
		case EJSON_SMARTEYE_RPC_COMPARE_IMG            :return bin_to_json_compare_img(pReq);
		case EJSON_SMARTEYE_RPC_AUTO_EXPOSURE_GET      :return bin_to_json_auto_exposure_get(pReq);
		case EJSON_SMARTEYE_RPC_AUTO_EXPOSURE_SET      :return bin_to_json_auto_exposure_set(pReq);
		case EJSON_SMARTEYE_RPC_EXPOSURE_GET           :return bin_to_json_get_exposure(pReq);
		case EJSON_SMARTEYE_RPC_EXPOSURE_SET           :return bin_to_json_set_exposure(pReq);
		case EJSON_SMARTEYE_RPC_AUTO_FOCUS_GET         :return bin_to_json_auto_focus_get(pReq);
		case EJSON_SMARTEYE_RPC_AUTO_FOCUS_SET         :return bin_to_json_auto_focus_set(pReq);
		case EJSON_SMARTEYE_RPC_FOCUS_GET              :return bin_to_json_get_focus(pReq);
		case EJSON_SMARTEYE_RPC_FOCUS_SET              :return bin_to_json_set_focus(pReq);
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
		case EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN       :return process_check_id_pattern(pReq);
		case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET      :return process_get_debug_outfile(pReq);
		case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_SET      :return process_set_debug_outfile(pReq);
		case EJSON_SMARTEYE_RPC_CAPTURE_JPG_IMG        :return process_capture_jpg_img(pReq);
		case EJSON_SMARTEYE_RPC_CAPTURE_RESOLUTION_SET :return process_set_capture_resolution(pReq);
		case EJSON_SMARTEYE_RPC_CHECKWALL_FILE_GET     :return process_get_checkwall_file(pReq);
		case EJSON_SMARTEYE_RPC_CHECKWALL_FILE_SET     :return process_set_checkwall_file(pReq);
		case EJSON_SMARTEYE_RPC_CHECKWALL_BASE_FILE_GET:return process_get_checkwallbase_file(pReq);
		case EJSON_SMARTEYE_RPC_CHECKWALL_BASE_FILE_SET:return process_set_checkwallbase_file(pReq);
		case EJSON_SMARTEYE_RPC_SCAN_QRSTRING          :return process_scan_qrstring(pReq);
		case EJSON_SMARTEYE_RPC_COMPARE_IMG            :return process_compare_img(pReq);
		case EJSON_SMARTEYE_RPC_AUTO_EXPOSURE_GET      :return process_auto_exposure_get(pReq);
		case EJSON_SMARTEYE_RPC_AUTO_EXPOSURE_SET      :return process_auto_exposure_set(pReq);
		case EJSON_SMARTEYE_RPC_EXPOSURE_GET           :return process_get_exposure(pReq);
		case EJSON_SMARTEYE_RPC_EXPOSURE_SET           :return process_set_exposure(pReq);
		case EJSON_SMARTEYE_RPC_AUTO_FOCUS_GET         :return process_auto_focus_get(pReq);
		case EJSON_SMARTEYE_RPC_AUTO_FOCUS_SET         :return process_auto_focus_set(pReq);
		case EJSON_SMARTEYE_RPC_FOCUS_GET              :return process_get_focus(pReq);
		case EJSON_SMARTEYE_RPC_FOCUS_SET              :return process_set_focus(pReq);
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

	pPanelReq->result=ImgId.IdentifyPattern(pPacket->pattern_type,pDataCache->StrImgIdDebugFile,pDataCache->StrImgIdCheckWallFile,pDataCache->StrImgIdCheckWallBaseFile);
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
int SmarteyeRpc::json_to_bin_get_checkwall_file(JsonDataCommObj* pReq)
{
	SMARTEYE_DEBUG_OUTFILE_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_DEBUG_OUTFILE_PACKET,RPC_SRV_ACT_READ,EJSON_SMARTEYE_RPC_CHECKWALL_FILE_GET);
	return 0;
}
int SmarteyeRpc::bin_to_json_get_checkwall_file(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,SMARTEYE_DEBUG_OUTFILE_PACKET,SMARTEYE_RPC_DEBUG_OUTFILE_ARG,filepath);
	return 0;
}
int SmarteyeRpc::process_get_checkwall_file(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_DEBUG_OUTFILE_PACKET* pPacket;
	pPacket=(SMARTEYE_DEBUG_OUTFILE_PACKET*)pPanelReq->dataRef;
	strcpy(pPacket->filepath,pDataCache->StrImgIdCheckWallFile.c_str());
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
int SmarteyeRpc::json_to_bin_set_checkwall_file(JsonDataCommObj* pReq)
{
	SMARTEYE_DEBUG_OUTFILE_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_DEBUG_OUTFILE_PACKET,RPC_SRV_ACT_WRITE,EJSON_SMARTEYE_RPC_CHECKWALL_FILE_SET);
	JSON_STRING_TO_STRING(SMARTEYE_RPC_DEBUG_OUTFILE_ARG,pPanelCmdObj->filepath);
	return 0;
}
int SmarteyeRpc::bin_to_json_set_checkwall_file(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,SMARTEYE_DEBUG_OUTFILE_PACKET);
	return 0;
}
int SmarteyeRpc::process_set_checkwall_file(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_DEBUG_OUTFILE_PACKET* pPacket;
	pPacket=(SMARTEYE_DEBUG_OUTFILE_PACKET*)pPanelReq->dataRef;
	pDataCache->StrImgIdCheckWallFile="";///tmp/test.txt";
	pDataCache->StrImgIdCheckWallFile.append(pPacket->filepath);
	if(pDataCache->StrImgIdCheckWallFile==" ") //dont accept empty file
		pPanelReq->result=RPC_SRV_RESULT_ARG_ERROR;
	else if(pDataCache->StrImgIdCheckWallFile=="") //dont accept empty file
		pPanelReq->result=RPC_SRV_RESULT_ARG_ERROR;
	else
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::json_to_bin_get_checkwallbase_file(JsonDataCommObj* pReq)
{
	SMARTEYE_DEBUG_OUTFILE_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_DEBUG_OUTFILE_PACKET,RPC_SRV_ACT_READ,EJSON_SMARTEYE_RPC_CHECKWALL_BASE_FILE_GET);
	return 0;
}
int SmarteyeRpc::bin_to_json_get_checkwallbase_file(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,SMARTEYE_DEBUG_OUTFILE_PACKET,SMARTEYE_RPC_DEBUG_OUTFILE_ARG,filepath);
	return 0;
}
int SmarteyeRpc::process_get_checkwallbase_file(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_DEBUG_OUTFILE_PACKET* pPacket;
	pPacket=(SMARTEYE_DEBUG_OUTFILE_PACKET*)pPanelReq->dataRef;
	strcpy(pPacket->filepath,pDataCache->StrImgIdCheckWallBaseFile.c_str());
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
int SmarteyeRpc::json_to_bin_set_checkwallbase_file(JsonDataCommObj* pReq)
{
	SMARTEYE_DEBUG_OUTFILE_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_DEBUG_OUTFILE_PACKET,RPC_SRV_ACT_WRITE,EJSON_SMARTEYE_RPC_CHECKWALL_BASE_FILE_SET);
	JSON_STRING_TO_STRING(SMARTEYE_RPC_DEBUG_OUTFILE_ARG,pPanelCmdObj->filepath);
	return 0;
}
int SmarteyeRpc::bin_to_json_set_checkwallbase_file(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,SMARTEYE_DEBUG_OUTFILE_PACKET);
	return 0;
}
int SmarteyeRpc::process_set_checkwallbase_file(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_DEBUG_OUTFILE_PACKET* pPacket;
	pPacket=(SMARTEYE_DEBUG_OUTFILE_PACKET*)pPanelReq->dataRef;
	pDataCache->StrImgIdCheckWallBaseFile="";///tmp/test.txt";
	pDataCache->StrImgIdCheckWallBaseFile.append(pPacket->filepath);
	if(pDataCache->StrImgIdCheckWallBaseFile==" ") //dont accept empty file
		pPanelReq->result=RPC_SRV_RESULT_ARG_ERROR;
	else if(pDataCache->StrImgIdCheckWallBaseFile=="") //dont accept empty file
		pPanelReq->result=RPC_SRV_RESULT_ARG_ERROR;
	else
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::json_to_bin_scan_qrstring(JsonDataCommObj* pReq)
{
	SMARTEYE_SCAN_QRSTRING_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_SCAN_QRSTRING_PACKET,RPC_SRV_ACT_WRITE,EJSON_SMARTEYE_RPC_SCAN_QRSTRING);
	JSON_STRING_TO_STRING(SMARTEYE_RPC_SCAN_QRSTRING_ARGFPATH,pPanelCmdObj->filepath);
	return 0;
}
int SmarteyeRpc::bin_to_json_scan_qrstring(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,SMARTEYE_SCAN_QRSTRING_PACKET,SMARTEYE_RPC_SCAN_QRSTRING_ARGQRSTR,qrstring);
	return 0;
}
int SmarteyeRpc::process_scan_qrstring(JsonDataCommObj* pReq)
{
	ImgIdentify ImgId;//opencv based class for image processing

	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_SCAN_QRSTRING_PACKET* pPacket;
	pPacket=(SMARTEYE_SCAN_QRSTRING_PACKET*)pPanelReq->dataRef;
	pDataCache->StrQrFilePath="";///tmp/test.txt";
	pDataCache->StrQrFilePath.append(pPacket->filepath);
	if(pDataCache->StrQrFilePath==" ") //dont accept empty file
		pPanelReq->result=RPC_SRV_RESULT_ARG_ERROR;
	else if(pDataCache->StrQrFilePath=="") //dont accept empty file
		pPanelReq->result=RPC_SRV_RESULT_ARG_ERROR;
	else
	{
		pPanelReq->result=ImgId.scan_qr_string(pDataCache->StrQrFilePath,pDataCache->StrQrString);
		strcpy(pPacket->qrstring,pDataCache->StrQrString.c_str());
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::json_to_bin_compare_img(JsonDataCommObj* pReq)
{
	SMARTEYE_COMPARE_IMG_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_COMPARE_IMG_PACKET,RPC_SRV_ACT_WRITE,EJSON_SMARTEYE_RPC_COMPARE_IMG);
	JSON_STRING_TO_STRING(SMARTEYE_RPC_COMPARE_IMG_ARGIMG1,pPanelCmdObj->filepath1);
	JSON_STRING_TO_STRING(SMARTEYE_RPC_COMPARE_IMG_ARGIMG2,pPanelCmdObj->filepath2);
	return 0;
}
int SmarteyeRpc::bin_to_json_compare_img(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,SMARTEYE_COMPARE_IMG_PACKET,SMARTEYE_RPC_COMPARE_IMG_ARGDIFF,imgdiff);//TODO
	return 0;
}
int SmarteyeRpc::process_compare_img(JsonDataCommObj* pReq)
{
	ImgIdentify ImgId;//opencv based class for image processing
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_COMPARE_IMG_PACKET* pPacket;
	pPacket=(SMARTEYE_COMPARE_IMG_PACKET*)pPanelReq->dataRef;

	pDataCache->CmpImgFilePath1="";
	pDataCache->CmpImgFilePath1.append(pPacket->filepath1);
	pDataCache->CmpImgFilePath2="";
	pDataCache->CmpImgFilePath2.append(pPacket->filepath2);

	if(pDataCache->CmpImgFilePath1==" " || pDataCache->CmpImgFilePath2==" ") //dont accept empty file
		pPanelReq->result=RPC_SRV_RESULT_ARG_ERROR;
	else if(pDataCache->CmpImgFilePath1=="" || pDataCache->CmpImgFilePath2=="") //dont accept empty file
		pPanelReq->result=RPC_SRV_RESULT_ARG_ERROR;
	else
	{
		pPanelReq->result=ImgId.compare_images(pDataCache->CmpImgFilePath1,pDataCache->CmpImgFilePath2,pDataCache->CmpImgResult);
		strcpy(pPacket->imgdiff,pDataCache->CmpImgResult.c_str());
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::json_to_bin_auto_exposure_get(JsonDataCommObj* pReq)
{
	SMARTEYE_EXPOSURE_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_EXPOSURE_PACKET,RPC_SRV_ACT_READ,EJSON_SMARTEYE_RPC_AUTO_EXPOSURE_GET);
	return 0;
}
int SmarteyeRpc::bin_to_json_auto_exposure_get(JsonDataCommObj* pReq)
{
PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,SMARTEYE_EXPOSURE_PACKET,SMARTEYE_RPC_AUTO_EXPOSURE_ARG,AutoExposure,SMARTEYE_RPC_AUTO_EXPOSURE_ARG_TABL,SMARTEYE_AUTO_EXPOSURE_UNKNOWN);
	return 0;
}
int SmarteyeRpc::process_auto_exposure_get(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_EXPOSURE_PACKET* pPacket;
	pPacket=(SMARTEYE_EXPOSURE_PACKET*)pPanelReq->dataRef;
	//pPacket->AutoExposure=pDataCache->AutoExposure;//TODO: do v4l2-ctl call
	//pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	pPanelReq->result=get_auto_exposure(pPacket->AutoExposure);
	return 0;
}
int SmarteyeRpc::json_to_bin_auto_exposure_set(JsonDataCommObj* pReq)
{
	SMARTEYE_EXPOSURE_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_EXPOSURE_PACKET,RPC_SRV_ACT_WRITE,EJSON_SMARTEYE_RPC_AUTO_EXPOSURE_SET);
	JSON_STRING_TO_ENUM(SMARTEYE_RPC_AUTO_EXPOSURE_ARG,SMARTEYE_RPC_AUTO_EXPOSURE_ARG_TABL,SMARTEYE_AUTO_EXPOSURE_TYPE,SMARTEYE_AUTO_EXPOSURE_UNKNOWN,pPanelCmdObj->AutoExposure);
	return 0;
}
int SmarteyeRpc::bin_to_json_auto_exposure_set(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,SMARTEYE_EXPOSURE_PACKET);
	return 0;
}
int SmarteyeRpc::process_auto_exposure_set(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_EXPOSURE_PACKET* pPacket;
	pPacket=(SMARTEYE_EXPOSURE_PACKET*)pPanelReq->dataRef;
	//pDataCache->AutoExposure=pPacket->AutoExposure;
	//pPanelReq->result=RPC_SRV_RESULT_SUCCESS;//process_show_pattern(pDataCache->pattern);//TODO: do v4l2-ctl call
	pPanelReq->result=set_auto_exposure(pPacket->AutoExposure);
	return 0;		
}
RPC_SRV_RESULT SmarteyeRpc::get_auto_exposure(SMARTEYE_AUTO_EXPOSURE_TYPE &exposure)
{
	//v4l2-ctl --set-ctrl=exposure_auto
	char command[1024];
	sprintf(command,"v4l2-ctl --get-ctrl=exposure_auto | awk '{print $2}' > %s","/tmp/temp-v4l2-result.txt");
	if(system(command)!=0)
		return RPC_SRV_RESULT_FILE_OPEN_ERR;
	char temp_str[255];
	FILE *shell;
	shell= fopen("/tmp/temp-v4l2-result.txt","r");
	if(shell == NULL )
		return RPC_SRV_RESULT_FILE_OPEN_ERR;
	size_t read_bytes = fread(command,1,100,shell);
	fclose(shell);
	if(read_bytes>0)
	{
		command[read_bytes]='\0';
		if(command[strlen(command)-1]=='\n')//remove the carriage return line
			command[strlen(command)-1]='\0';
		if(strlen(command)>=1)
		{
			int exp=atoi(command);
			if(exp==3)
				exposure=SMARTEYE_AUTO_EXPOSURE_ENABLE;
			else if(exp==1)
				exposure=SMARTEYE_AUTO_EXPOSURE_DISABLE;
			else
				exposure=SMARTEYE_AUTO_EXPOSURE_UNKNOWN;
			return RPC_SRV_RESULT_SUCCESS;
		}
		else
			return RPC_SRV_RESULT_FAIL;
	}
	else
		return RPC_SRV_RESULT_FILE_READ_ERR;	
}
RPC_SRV_RESULT SmarteyeRpc::set_auto_exposure(SMARTEYE_AUTO_EXPOSURE_TYPE exposure)
{
	//v4l2-ctl --set-ctrl=exposure_auto=3 //enable
	//v4l2-ctl --set-ctrl=exposure_auto=1 //disable
	char command[512];
	if(exposure==SMARTEYE_AUTO_EXPOSURE_DISABLE)
		sprintf(command,"v4l2-ctl --set-ctrl=exposure_auto=1>/dev/null");
	else if(exposure==SMARTEYE_AUTO_EXPOSURE_ENABLE)
		sprintf(command,"v4l2-ctl --set-ctrl=exposure_auto=3>/dev/null");
	else
		return RPC_SRV_RESULT_ARG_ERROR;

	if (system(command)==0)
		return RPC_SRV_RESULT_SUCCESS;
	else
		return RPC_SRV_RESULT_FAIL;
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::json_to_bin_get_exposure(JsonDataCommObj* pReq)
{
	SMARTEYE_EXPOSURE_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_EXPOSURE_PACKET,RPC_SRV_ACT_READ,EJSON_SMARTEYE_RPC_EXPOSURE_GET);
	return 0;
}
int SmarteyeRpc::bin_to_json_get_exposure(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
        SMARTEYE_EXPOSURE_PACKET* pPanelCmdObj;
        pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
        pPanelCmdObj=(SMARTEYE_EXPOSURE_PACKET*)pPanelReq->dataRef;
        if(pPanelReq->result!=RPC_SRV_RESULT_SUCCESS)
              prepare_result_string(pPanelReq->result,pReq);
	else
	      prepare_result_string(pPanelReq->result,pReq,(char*)SMARTEYE_RPC_EXPOSURE_ARG,pPanelCmdObj->Exposure);
	OBJ_MEM_DELETE(pPanelCmdObj);
	return 0;
}
int SmarteyeRpc::process_get_exposure(JsonDataCommObj* pReq)//,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_EXPOSURE_PACKET* pPacket;
	pPacket=(SMARTEYE_EXPOSURE_PACKET*)pPanelReq->dataRef;
	//pPacket->Exposure=pDataCache->Exposure;//TODO: do v4l2-ctl call
	//pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	pPanelReq->result=get_exposure(&pPacket->Exposure);
	return 0;
}
int SmarteyeRpc::json_to_bin_set_exposure(JsonDataCommObj* pReq)
{
	SMARTEYE_EXPOSURE_PACKET* pPanelCmdObj=NULL;
        PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_EXPOSURE_PACKET,RPC_SRV_ACT_WRITE,EJSON_SMARTEYE_RPC_EXPOSURE_SET);
	JSON_STRING_TO_INT(SMARTEYE_RPC_EXPOSURE_ARG,pPanelCmdObj->Exposure);
	return 0;
}
int SmarteyeRpc::bin_to_json_set_exposure(JsonDataCommObj* pReq)
{
        PREPARE_JSON_RESP(RPC_SRV_REQ,SMARTEYE_EXPOSURE_PACKET);
	return 0;
}
int SmarteyeRpc:: process_set_exposure(JsonDataCommObj* pReq)//,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	//LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_EXPOSURE_PACKET* pPacket;
	pPacket=(SMARTEYE_EXPOSURE_PACKET*)pPanelReq->dataRef;
	//pDataCache->Exposure=pPacket->Exposure;//TODO: do v4l2-ctl call
	//pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	pPanelReq->result=set_exposure(pPacket->Exposure);
	return 0;
}
RPC_SRV_RESULT SmarteyeRpc::get_exposure(int* exposure)
{
	char command[1024];
	sprintf(command,"v4l2-ctl --get-ctrl=exposure_absolute | awk '{print $2}' > %s","/tmp/temp-v4l2-result.txt");
	if(system(command)!=0)
		return RPC_SRV_RESULT_FILE_OPEN_ERR;
	char temp_str[255];
	FILE *shell;
	shell= fopen("/tmp/temp-v4l2-result.txt","r");
	if(shell == NULL )
		return RPC_SRV_RESULT_FILE_OPEN_ERR;
	size_t read_bytes = fread(command,1,100,shell);
	fclose(shell);
	if(read_bytes>0)
	{
		command[read_bytes]='\0';
		if(command[strlen(command)-1]=='\n')//remove the carriage return line
			command[strlen(command)-1]='\0';
		if(strlen(command)>=1)
		{
			*exposure=atoi(command);
			return RPC_SRV_RESULT_SUCCESS;
		}
		else
			return RPC_SRV_RESULT_FAIL;
	}
	else
		return RPC_SRV_RESULT_FILE_READ_ERR;	
}
RPC_SRV_RESULT SmarteyeRpc::set_exposure(int exposure)
{
	//v4l2-ctl --set-ctrl=exposure_absolute=200
	char command[512];
	sprintf(command,"v4l2-ctl --set-ctrl=exposure_absolute=%d>/dev/null",exposure);
	if (system(command)==0)
		return RPC_SRV_RESULT_SUCCESS;
	else
		return RPC_SRV_RESULT_FAIL;
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::json_to_bin_auto_focus_get(JsonDataCommObj* pReq)
{
	SMARTEYE_FOCUS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_FOCUS_PACKET,RPC_SRV_ACT_READ,EJSON_SMARTEYE_RPC_AUTO_FOCUS_GET);
	return 0;
}
int SmarteyeRpc::bin_to_json_auto_focus_get(JsonDataCommObj* pReq)
{
PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,SMARTEYE_FOCUS_PACKET,SMARTEYE_RPC_AUTO_FOCUS_ARG,AutoFocus,SMARTEYE_RPC_AUTO_FOCUS_ARG_TABL,SMARTEYE_AUTO_FOCUS_UNKNOWN);
	return 0;
}
int SmarteyeRpc::process_auto_focus_get(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_FOCUS_PACKET* pPacket;
	pPacket=(SMARTEYE_FOCUS_PACKET*)pPanelReq->dataRef;
	//pPacket->AutoFocus=pDataCache->AutoFocus;//TODO: do v4l2-ctl call
	//pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	pPanelReq->result=get_auto_focus(pPacket->AutoFocus);
	return 0;
}
int SmarteyeRpc::json_to_bin_auto_focus_set(JsonDataCommObj* pReq)
{
	SMARTEYE_FOCUS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_FOCUS_PACKET,RPC_SRV_ACT_WRITE,EJSON_SMARTEYE_RPC_AUTO_FOCUS_SET);
	JSON_STRING_TO_ENUM(SMARTEYE_RPC_AUTO_FOCUS_ARG,SMARTEYE_RPC_AUTO_FOCUS_ARG_TABL,SMARTEYE_AUTO_FOCUS_TYPE,SMARTEYE_AUTO_FOCUS_UNKNOWN,pPanelCmdObj->AutoFocus);
	return 0;
}
int SmarteyeRpc::bin_to_json_auto_focus_set(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,SMARTEYE_FOCUS_PACKET);
	return 0;
}
int SmarteyeRpc::process_auto_focus_set(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_FOCUS_PACKET* pPacket;
	pPacket=(SMARTEYE_FOCUS_PACKET*)pPanelReq->dataRef;
	//pDataCache->AutoFocus=pPacket->AutoFocus;
	//pPanelReq->result=RPC_SRV_RESULT_SUCCESS;//process_show_pattern(pDataCache->pattern);//TODO: do v4l2-ctl call
	pPanelReq->result=set_auto_focus(pPacket->AutoFocus);
	return 0;		
}
RPC_SRV_RESULT SmarteyeRpc::get_auto_focus(SMARTEYE_AUTO_FOCUS_TYPE &focus)
{
	//v4l2-ctl --set-ctrl=exposure_auto
	char command[1024];
	sprintf(command,"v4l2-ctl --get-ctrl=focus_auto | awk '{print $2}' > %s","/tmp/temp-v4l2-result.txt");
	if(system(command)!=0)
		return RPC_SRV_RESULT_FILE_OPEN_ERR;
	char temp_str[255];
	FILE *shell;
	shell= fopen("/tmp/temp-v4l2-result.txt","r");
	if(shell == NULL )
		return RPC_SRV_RESULT_FILE_OPEN_ERR;
	size_t read_bytes = fread(command,1,100,shell);
	fclose(shell);
	if(read_bytes>0)
	{
		command[read_bytes]='\0';
		if(command[strlen(command)-1]=='\n')//remove the carriage return line
			command[strlen(command)-1]='\0';
		if(strlen(command)>=1)
		{
			int foc=atoi(command);
			if(foc==1)
				focus=SMARTEYE_AUTO_FOCUS_ENABLE;
			else if(foc==0)
				focus=SMARTEYE_AUTO_FOCUS_DISABLE;
			else
				focus=SMARTEYE_AUTO_FOCUS_UNKNOWN;
			return RPC_SRV_RESULT_SUCCESS;
		}
		else
			return RPC_SRV_RESULT_FAIL;
	}
	else
		return RPC_SRV_RESULT_FILE_READ_ERR;	
}
RPC_SRV_RESULT SmarteyeRpc::set_auto_focus(SMARTEYE_AUTO_FOCUS_TYPE focus)
{
	//v4l2-ctl --set-ctrl=focus_auto=1 //enable
	//v4l2-ctl --set-ctrl=focus_auto=0 //disable
	char command[512];
	if(focus==SMARTEYE_AUTO_FOCUS_DISABLE)
		sprintf(command,"v4l2-ctl --set-ctrl=focus_auto=0>/dev/null");
	else if(focus==SMARTEYE_AUTO_FOCUS_ENABLE)
		sprintf(command,"v4l2-ctl --set-ctrl=focus_auto=1>/dev/null");
	else
		return RPC_SRV_RESULT_ARG_ERROR;

	if (system(command)==0)
		return RPC_SRV_RESULT_SUCCESS;
	else
		return RPC_SRV_RESULT_FAIL;
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::json_to_bin_get_focus(JsonDataCommObj* pReq)
{
	SMARTEYE_EXPOSURE_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_EXPOSURE_PACKET,RPC_SRV_ACT_READ,EJSON_SMARTEYE_RPC_EXPOSURE_GET);
	return 0;
}
int SmarteyeRpc::bin_to_json_get_focus(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
        SMARTEYE_FOCUS_PACKET* pPanelCmdObj;
        pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
        pPanelCmdObj=(SMARTEYE_FOCUS_PACKET*)pPanelReq->dataRef;
        if(pPanelReq->result!=RPC_SRV_RESULT_SUCCESS)
              prepare_result_string(pPanelReq->result,pReq);
	else
	      prepare_result_string(pPanelReq->result,pReq,(char*)SMARTEYE_RPC_FOCUS_ARG,pPanelCmdObj->Focus);
	OBJ_MEM_DELETE(pPanelCmdObj);
	return 0;
}
int SmarteyeRpc::process_get_focus(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_FOCUS_PACKET* pPacket;
	pPacket=(SMARTEYE_FOCUS_PACKET*)pPanelReq->dataRef;
	//pPacket->Focus=pDataCache->Focus;//TODO: do v4l2-ctl call
	//pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	pPanelReq->result=get_focus(&pPacket->Focus);
	return 0;
}
int SmarteyeRpc::json_to_bin_set_focus(JsonDataCommObj* pReq)
{
	SMARTEYE_FOCUS_PACKET* pPanelCmdObj=NULL;
        PREPARE_JSON_REQUEST(RPC_SRV_REQ,SMARTEYE_FOCUS_PACKET,RPC_SRV_ACT_WRITE,EJSON_SMARTEYE_RPC_FOCUS_SET);
	JSON_STRING_TO_INT(SMARTEYE_RPC_FOCUS_ARG,pPanelCmdObj->Focus);
	return 0;
}
int SmarteyeRpc::bin_to_json_set_focus(JsonDataCommObj* pReq)
{
        PREPARE_JSON_RESP(RPC_SRV_REQ,SMARTEYE_FOCUS_PACKET);
	return 0;
}
int SmarteyeRpc:: process_set_focus(JsonDataCommObj* pReq)
{
	//LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SMARTEYE_FOCUS_PACKET* pPacket;
	pPacket=(SMARTEYE_FOCUS_PACKET*)pPanelReq->dataRef;
	//pDataCache->Focus=pPacket->Focus;//TODO: do v4l2-ctl call
	//pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	pPanelReq->result=set_focus(pPacket->Focus);
	return 0;
}
RPC_SRV_RESULT SmarteyeRpc::get_focus(int* focus)
{
	//v4l2-ctl --get-ctrl=focus_absolute
	char command[1024];
	sprintf(command,"v4l2-ctl --get-ctrl=focus_absolute | awk '{print $2}' > %s","/tmp/temp-v4l2-result.txt");
	if(system(command)!=0)
		return RPC_SRV_RESULT_FILE_OPEN_ERR;
	char temp_str[255];
	FILE *shell;
	shell= fopen("/tmp/temp-v4l2-result.txt","r");
	if(shell == NULL )
		return RPC_SRV_RESULT_FILE_OPEN_ERR;
	size_t read_bytes = fread(command,1,100,shell);
	fclose(shell);
	if(read_bytes>0)
	{
		command[read_bytes]='\0';
		if(command[strlen(command)-1]=='\n')//remove the carriage return line
			command[strlen(command)-1]='\0';
		if(strlen(command)>=1)
		{
			*focus=atoi(command);
			return RPC_SRV_RESULT_SUCCESS;
		}
		else
			return RPC_SRV_RESULT_FAIL;
	}
	else
		return RPC_SRV_RESULT_FILE_READ_ERR;	
}
RPC_SRV_RESULT SmarteyeRpc::set_focus(int focus)
{
	//v4l2-ctl --set-ctrl=exposure_absolute=200
	char command[512];
	sprintf(command,"v4l2-ctl --set-ctrl=focus_absolute=%d>/dev/null",focus);
	if (system(command)==0)
		return RPC_SRV_RESULT_SUCCESS;
	else
		return RPC_SRV_RESULT_FAIL;
}

/* ------------------------------------------------------------------------- */




