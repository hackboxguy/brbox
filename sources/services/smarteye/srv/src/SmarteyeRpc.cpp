#include "SmarteyeRpc.h"
//#include "ADSysInfo.hpp"
//#include "SysmgrJsonDef.h"
/* ------------------------------------------------------------------------- */
SmarteyeRpc:: SmarteyeRpc(std::string rpcName,int myIndex):ADJsonRpcMgrConsumer(rpcName,myIndex)
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
		//case EJSON_SYSMGR_RPC_GET_MAC_ADDR :return json_to_bin_get_mac_addr(pReq);
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
		//case EJSON_SYSMGR_RPC_GET_MAC_ADDR :return bin_to_json_get_mac_addr(pReq);
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
		//case EJSON_SYSMGR_RPC_GET_MAC_ADDR :return process_get_mac_addr(pReq);
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
/*int SmarteyeRpc::json_to_bin_get_mac_addr(JsonDataCommObj* pReq)
{
	SYSMGR_MAC_ADDR_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_MAC_ADDR_PACKET,RPC_SRV_ACT_READ,EJSON_SYSMGR_RPC_GET_MAC_ADDR);
	//after assigning pointer to pPanelReq->dataRef, modify cmd-req-obj with correct client arguments
JSON_STRING_TO_ENUM(SYSMGR_RPC_MAC_ADDR_ARG_IFACE,SYSMGR_RPC_MAC_ADDR_ARG_IFACE_TABL,EJSON_SYSMGR_IFACE_TYPE,EJSON_SYSMGR_IFACE_TYPE_UNKNOWN,pPanelCmdObj->eth_type);
	return 0;
}
int NetRpc::bin_to_json_get_mac_addr(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,SYSMGR_MAC_ADDR_PACKET,SYSMGR_RPC_MAC_ADDR_ARG,mac_addr);
	return 0;
}
int NetRpc::process_get_mac_addr(JsonDataCommObj* pReq)
{
	ADSysInfo SysInfo;//lib-class for reading cpu-info and system-info
	char ip[512];char netmask[512];

	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SYSMGR_MAC_ADDR_PACKET* pPacket;
	pPacket=(SYSMGR_MAC_ADDR_PACKET*)pPanelReq->dataRef;

	const char *table[]   = SYSMGR_RPC_MAC_ADDR_ARG_IFACE_TABL;
	//if network is connected,then read details from SysInfo
	if(SysInfo.read_network_info((char*)table[pPacket->eth_type],pPacket->mac_addr,ip,netmask)==0)
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	else
	{
		//if network is not connected, use ifconfig method
		if(SysInfo.read_network_info_ifconfig((char*)table[pPacket->eth_type],pPacket->mac_addr,ip,netmask)==0)
			pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
		else
			pPanelReq->result=RPC_SRV_RESULT_FAIL;
	}
	return 0;
}*/
/* ------------------------------------------------------------------------- */

