#include "NetRpc.h"
//#include "SysmgrJsonDef.h"

/* ------------------------------------------------------------------------- */
NetRpc:: NetRpc(std::string rpcName,int myIndex):ADJsonRpcMgrConsumer(rpcName,myIndex)
{
}
/* ------------------------------------------------------------------------- */
NetRpc::~ NetRpc()
{
}
/* ------------------------------------------------------------------------- */
int NetRpc::MapJsonToBinary(JsonDataCommObj* pReq,int index)
{
	//printf("NetRpc::MapJsonToBinary called\n");
	EJSON_SYSMGR_RPC_TYPES command =(EJSON_SYSMGR_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_SYSMGR_RPC_GET_MAC_ADDR:return json_to_bin_get_mac_addr(pReq);
		case EJSON_SYSMGR_RPC_SET_MAC_ADDR:break;
	}
	return -1;//0;
}
/* ------------------------------------------------------------------------- */
int NetRpc::MapBinaryToJson(JsonDataCommObj* pReq,int index)
{
	//printf("NetRpc::MapBinaryToJson called\n");
	EJSON_SYSMGR_RPC_TYPES command =(EJSON_SYSMGR_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_SYSMGR_RPC_GET_MAC_ADDR:return bin_to_json_get_mac_addr(pReq);
		case EJSON_SYSMGR_RPC_SET_MAC_ADDR:break;
	}
	return -1;
}
/* ------------------------------------------------------------------------- */
int NetRpc::ProcessWork(JsonDataCommObj* pReq,int index)
{
	//printf("NetRpc::ProcessWork called\n");
	EJSON_SYSMGR_RPC_TYPES command =(EJSON_SYSMGR_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_SYSMGR_RPC_GET_MAC_ADDR:return process_get_mac_addr(pReq);
		case EJSON_SYSMGR_RPC_SET_MAC_ADDR:break;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int NetRpc::ProcessWorkAsync(unsigned char* pWorkData)
{
	
	return 0;
}
/* ------------------------------------------------------------------------- */
int NetRpc::json_to_bin_get_mac_addr(JsonDataCommObj* pReq)
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
/* ------------------------------------------------------------------------- */
int NetRpc::process_get_mac_addr(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SYSMGR_MAC_ADDR_PACKET* pPacket;
	pPacket=(SYSMGR_MAC_ADDR_PACKET*)pPanelReq->dataRef;
	strcpy(pPacket->mac_addr,"00:11:22:33:44:55");
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
/* ------------------------------------------------------------------------- */


