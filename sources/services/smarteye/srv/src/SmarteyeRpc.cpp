#include "SmarteyeRpc.h"
#include "ImgIdentify.h"

//#include "ADSysInfo.hpp"
//#include "SysmgrJsonDef.h"
/* ------------------------------------------------------------------------- */
SmarteyeRpc:: SmarteyeRpc(std::string rpcName,int myIndex,bool emu, bool log):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
}
/* ------------------------------------------------------------------------- */
SmarteyeRpc::~ SmarteyeRpc()
{
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::MapJsonToBinary(JsonDataCommObj* pReq,int index)
{
	//printf("NetRpc::MapJsonToBinary called\n");
	EJSON_SMARTEYE_RPC_TYPES command =(EJSON_SMARTEYE_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN :return json_to_bin_check_id_pattern(pReq);
		//case EJSON_SYSMGR_RPC_SET_MAC_ADDR :return json_to_bin_set_mac_addr(pReq);
		//case EJSON_SYSMGR_RPC_GET_ETH_COUNT:return json_to_bin_get_eth_count(pReq);
		//case EJSON_SYSMGR_RPC_GET_ETH_NAME :return json_to_bin_get_eth_name(pReq);
		default:break;
	}
	return -1;//0;
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::MapBinaryToJson(JsonDataCommObj* pReq,int index)
{
	//printf("NetRpc::MapBinaryToJson called\n");
	EJSON_SMARTEYE_RPC_TYPES command =(EJSON_SMARTEYE_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN :return bin_to_json_check_id_pattern(pReq);
		//case EJSON_SYSMGR_RPC_SET_MAC_ADDR :return bin_to_json_set_mac_addr(pReq);
		//case EJSON_SYSMGR_RPC_GET_ETH_COUNT:return bin_to_json_get_eth_count(pReq);
		//case EJSON_SYSMGR_RPC_GET_ETH_NAME :return bin_to_json_get_eth_name(pReq);
		default:break;
	}
	return -1;
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::ProcessWork(JsonDataCommObj* pReq,int index)
{
	//printf("NetRpc::ProcessWork called\n");
	EJSON_SMARTEYE_RPC_TYPES command =(EJSON_SMARTEYE_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN :return process_check_id_pattern(pReq);
		//case EJSON_SYSMGR_RPC_SET_MAC_ADDR :return process_set_mac_addr(pReq);
		//case EJSON_SYSMGR_RPC_GET_ETH_COUNT:return process_get_eth_count(pReq);
		//case EJSON_SYSMGR_RPC_GET_ETH_NAME :return process_get_eth_name(pReq);
		default:break;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::ProcessWorkAsync(unsigned char* pWorkData)
{
	return 0;
}
/* ------------------------------------------------------------------------- */
int SmarteyeRpc::json_to_bin_check_id_pattern(JsonDataCommObj* pReq)
{
	SYSMGR_ID_PATTERN_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_ID_PATTERN_PACKET,RPC_SRV_ACT_WRITE,EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN);
	JSON_STRING_TO_ENUM(SMARTEYE_RPC_ID_PATTERN_CHECK_ARG,SMARTEYE_RPC_ID_PATTERN_CHECK_ARG_TABL,EJSON_SYSMGR_IFACE_TYPE,EJSON_SMARTEYE_IDPATTERN_UNKNOWN,pPanelCmdObj->pattern_type);
	return 0;
}
int SmarteyeRpc::bin_to_json_check_id_pattern(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,SYSMGR_ID_PATTERN_PACKET);
	return 0;
}
int SmarteyeRpc::process_check_id_pattern(JsonDataCommObj* pReq)
{
	ImgIdentify ImgId;//opencv based class for image processing
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SYSMGR_ID_PATTERN_PACKET* pPacket;
	pPacket=(SYSMGR_ID_PATTERN_PACKET*)pPanelReq->dataRef;
	pPanelReq->result=ImgId.IdentifyPattern(pPacket->pattern_type);
	return 0;
}
/* ------------------------------------------------------------------------- */

