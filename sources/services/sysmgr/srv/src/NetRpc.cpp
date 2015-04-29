#include "NetRpc.h"
#include "ADSysInfo.hpp"
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
		case EJSON_SYSMGR_RPC_GET_MAC_ADDR :return json_to_bin_get_mac_addr(pReq);
		case EJSON_SYSMGR_RPC_SET_MAC_ADDR :return json_to_bin_set_mac_addr(pReq);
		case EJSON_SYSMGR_RPC_GET_ETH_COUNT:return json_to_bin_get_eth_count(pReq);
		case EJSON_SYSMGR_RPC_GET_ETH_NAME :return json_to_bin_get_eth_name(pReq);
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
		case EJSON_SYSMGR_RPC_GET_MAC_ADDR :return bin_to_json_get_mac_addr(pReq);
		case EJSON_SYSMGR_RPC_SET_MAC_ADDR :return bin_to_json_set_mac_addr(pReq);
		case EJSON_SYSMGR_RPC_GET_ETH_COUNT:return bin_to_json_get_eth_count(pReq);
		case EJSON_SYSMGR_RPC_GET_ETH_NAME :return bin_to_json_get_eth_name(pReq);
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
		case EJSON_SYSMGR_RPC_GET_MAC_ADDR :return process_get_mac_addr(pReq);
		case EJSON_SYSMGR_RPC_SET_MAC_ADDR :return process_set_mac_addr(pReq);
		case EJSON_SYSMGR_RPC_GET_ETH_COUNT:return process_get_eth_count(pReq);
		case EJSON_SYSMGR_RPC_GET_ETH_NAME :return process_get_eth_name(pReq);
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
}
/* ------------------------------------------------------------------------- */
int NetRpc::json_to_bin_set_mac_addr(JsonDataCommObj* pReq)
{
	SYSMGR_MAC_ADDR_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_MAC_ADDR_PACKET,RPC_SRV_ACT_WRITE,EJSON_SYSMGR_RPC_SET_MAC_ADDR);
	//after assigning pointer to pPanelReq->dataRef, modify cmd-req-obj with correct client arguments
JSON_STRING_TO_ENUM(SYSMGR_RPC_MAC_ADDR_ARG_IFACE,SYSMGR_RPC_MAC_ADDR_ARG_IFACE_TABL,EJSON_SYSMGR_IFACE_TYPE,EJSON_SYSMGR_IFACE_TYPE_UNKNOWN,pPanelCmdObj->eth_type);
	return 0;
}
int NetRpc::bin_to_json_set_mac_addr(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,SYSMGR_MAC_ADDR_PACKET);
	return 0;
}
int NetRpc::process_set_mac_addr(JsonDataCommObj* pReq)
{
	//TODO:dont supprt mac writing yet
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	pPanelReq->result=RPC_SRV_RESULT_FEATURE_UNSUPPORTED;
	return 0;
}
/* ------------------------------------------------------------------------- */
int NetRpc::json_to_bin_get_eth_count(JsonDataCommObj* pReq)
{
	SYSMGR_ETH_COUNT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_ETH_COUNT_PACKET,RPC_SRV_ACT_READ,EJSON_SYSMGR_RPC_GET_ETH_COUNT);
	return 0;
}
int NetRpc::bin_to_json_get_eth_count(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_INT(RPC_SRV_REQ,SYSMGR_MAC_ADDR_PACKET,SYSMGR_RPC_ETH_COUNT_ARG,EthCountIndx);
	return 0;
}
int NetRpc::process_get_eth_count(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SYSMGR_ETH_COUNT_PACKET* pPacket;
	pPacket=(SYSMGR_ETH_COUNT_PACKET*)pPanelReq->dataRef;
	pPacket->EthCountIndx=0;

	char command[1024];
	char temp_str[256];
	FILE *shell;
	sprintf(command,"ifconfig | grep \"Link encap\" | grep -v \"Local Loopback\" >/dev/null");
	if(system(command)!=0) //only loopback interface is present, hence return EthCountIndx=0
	{
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
		return 0;
	}
	sprintf(command,"ifconfig | grep \"Link encap\" | grep -v \"Local Loopback\" | sed -n '$='");
	shell= popen(command,"r");
	if(shell == NULL)
	{
		pPanelReq->result=RPC_SRV_RESULT_FILE_OPEN_ERR;
		return 0;
	}
	if(fgets(temp_str,250,shell)!=NULL)
	{
		temp_str[255]='\0';
		pPacket->EthCountIndx=atoi(temp_str);
	}
	pclose(shell);
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
/* ------------------------------------------------------------------------- */
int NetRpc::json_to_bin_get_eth_name(JsonDataCommObj* pReq)
{
	SYSMGR_ETH_NAME_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_ETH_NAME_PACKET,RPC_SRV_ACT_READ,EJSON_SYSMGR_RPC_GET_ETH_NAME);
	JSON_STRING_TO_INT(SYSMGR_RPC_ETH_NAME_ARG_INDEX,pPanelCmdObj->EthCountIndx);
	return 0;
}
int NetRpc::bin_to_json_get_eth_name(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,SYSMGR_ETH_NAME_PACKET,SYSMGR_RPC_ETH_NAME_ARG,eth_name);
	return 0;
}
int NetRpc::process_get_eth_name(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SYSMGR_ETH_COUNT_PACKET* pPacket;
	pPacket=(SYSMGR_ETH_COUNT_PACKET*)pPanelReq->dataRef;
	//pPacket->EthCountIndx=0;
	//TODO:
	return 0;
}

