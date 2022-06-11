#include "NetRpc.h"
#include "ADSysInfo.hpp"
//#include "SysmgrJsonDef.h"
/* ------------------------------------------------------------------------- */
NetRpc:: NetRpc(std::string rpcName,int myIndex,bool emu,bool log,SYSMGR_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
	pDataCache=pData;
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
		case EJSON_SYSMGR_RPC_GET_IP_ADDR  :return json_to_bin_get_ip_addr(pReq);
		case EJSON_SYSMGR_RPC_SET_IP_ADDR  :return json_to_bin_set_ip_addr(pReq);
		case EJSON_SYSMGR_RPC_GET_NETMASK  :return json_to_bin_get_netmask(pReq);
		case EJSON_SYSMGR_RPC_SET_NETMASK  :return json_to_bin_set_netmask(pReq);
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
		case EJSON_SYSMGR_RPC_GET_IP_ADDR  :return bin_to_json_get_ip_addr(pReq);
		case EJSON_SYSMGR_RPC_SET_IP_ADDR  :return bin_to_json_set_ip_addr(pReq);
		case EJSON_SYSMGR_RPC_GET_NETMASK  :return bin_to_json_get_netmask(pReq);
		case EJSON_SYSMGR_RPC_SET_NETMASK  :return bin_to_json_set_netmask(pReq);
	}
	return -1;
}
/* ------------------------------------------------------------------------- */
int NetRpc::ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj)
{
	//printf("NetRpc::ProcessWork called\n");
	EJSON_SYSMGR_RPC_TYPES command =(EJSON_SYSMGR_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_SYSMGR_RPC_GET_MAC_ADDR :return process_get_mac_addr(pReq);
		case EJSON_SYSMGR_RPC_SET_MAC_ADDR :return process_set_mac_addr(pReq);
		case EJSON_SYSMGR_RPC_GET_ETH_COUNT:return process_get_eth_count(pReq);
		case EJSON_SYSMGR_RPC_GET_ETH_NAME :return process_get_eth_name(pReq);
		case EJSON_SYSMGR_RPC_GET_IP_ADDR  :return process_get_ip_addr(pReq);
		case EJSON_SYSMGR_RPC_SET_IP_ADDR  :return process_set_ip_addr(pReq);
		case EJSON_SYSMGR_RPC_GET_NETMASK  :return process_get_netmask(pReq);
		case EJSON_SYSMGR_RPC_SET_NETMASK  :return process_set_netmask(pReq);
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT NetRpc::ProcessWorkAsync(int index,unsigned char* pWorkData)
{
	RPC_SRV_RESULT ret_val=RPC_SRV_RESULT_FAIL;

	return ret_val;
}
/* ------------------------------------------------------------------------- */
int NetRpc::json_to_bin_get_mac_addr(JsonDataCommObj* pReq)
{
	SYSMGR_MAC_ADDR_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_MAC_ADDR_PACKET,RPC_SRV_ACT_READ,EJSON_SYSMGR_RPC_GET_MAC_ADDR);
	//after assigning pointer to pPanelReq->dataRef, modify cmd-req-obj with correct client arguments
//JSON_STRING_TO_ENUM(SYSMGR_RPC_MAC_ADDR_ARG_IFACE,SYSMGR_RPC_MAC_ADDR_ARG_IFACE_TABL,EJSON_SYSMGR_IFACE_TYPE,EJSON_SYSMGR_IFACE_TYPE_UNKNOWN,pPanelCmdObj->eth_type);
	JSON_STRING_TO_STRING(SYSMGR_RPC_IP_ADDR_ARG_IFACE,pPanelCmdObj->eth_name);//changed from fixed enum list to string
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
	//if(SysInfo.read_network_info((char*)table[pPacket->eth_type],pPacket->mac_addr,ip,netmask)==0)
	if(SysInfo.read_network_info(pPacket->eth_name,pPacket->mac_addr,ip,netmask)==0)
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	else
	{
		//if network is not connected, use ifconfig method
		//if(SysInfo.read_network_info_ifconfig((char*)table[pPacket->eth_type],pPacket->mac_addr,ip,netmask)==0)
		if(SysInfo.read_network_info_ifconfig(pPacket->eth_name,pPacket->mac_addr,ip,netmask)==0)
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

	ADSysInfo SysInfo;
	pPanelReq->result=SysInfo.get_total_eth_count(pPacket->EthCountIndx);

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
	ADSysInfo SysInfo;int tmpCount;
	pPanelReq->result=SysInfo.get_total_eth_count(tmpCount);
	if(pPanelReq->result!=RPC_SRV_RESULT_SUCCESS)
		return 0;
	if(tmpCount==0)
	{
		pPanelReq->result=RPC_SRV_RESULT_FAIL;//no eth interfaces are available
		return 0;
	}
	//zero based index must be passed by the client
	if(pPacket->EthCountIndx > (tmpCount-1) )
	{
		pPanelReq->result=RPC_SRV_RESULT_ARG_ERROR;
		return 0;
	}
	pPanelReq->result=SysInfo.get_nth_eth_name(pPacket->EthCountIndx+1,pPacket->eth_name);
	return 0;
}
/* ------------------------------------------------------------------------- */
int NetRpc::json_to_bin_get_ip_addr(JsonDataCommObj* pReq)
{
	SYSMGR_NET_INFO_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_NET_INFO_PACKET,RPC_SRV_ACT_READ,EJSON_SYSMGR_RPC_GET_IP_ADDR);
	//after assigning pointer to pPanelReq->dataRef, modify cmd-req-obj with correct client arguments
//JSON_STRING_TO_ENUM(SYSMGR_RPC_IP_ADDR_ARG_IFACE,SYSMGR_RPC_IP_ADDR_ARG_IFACE_TABL,EJSON_SYSMGR_IFACE_TYPE,EJSON_SYSMGR_IFACE_TYPE_UNKNOWN,pPanelCmdObj->eth_type);
	JSON_STRING_TO_STRING(SYSMGR_RPC_IP_ADDR_ARG_IFACE,pPanelCmdObj->eth_name);//changed from fixed enum list to string
	return 0;
}
int NetRpc::bin_to_json_get_ip_addr(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,SYSMGR_NET_INFO_PACKET,SYSMGR_RPC_IP_ADDR_ARG,addr);
	return 0;
}
int NetRpc::process_get_ip_addr(JsonDataCommObj* pReq)
{
	ADSysInfo SysInfo;//lib-class for reading cpu-info and system-info
	char netmask[512];char mac[512];

	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SYSMGR_NET_INFO_PACKET* pPacket;
	pPacket=(SYSMGR_NET_INFO_PACKET*)pPanelReq->dataRef;

	const char *table[]   = SYSMGR_RPC_MAC_ADDR_ARG_IFACE_TABL;
	//if network is connected,then read details from SysInfo
	//if(SysInfo.read_network_info((char*)table[pPacket->eth_type],mac,pPacket->addr,netmask)==0)
	if(SysInfo.read_network_info(pPacket->eth_name,mac,pPacket->addr,netmask)==0)
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	else
	{
		//if network is not connected, use ifconfig method
		//if(SysInfo.read_network_info_ifconfig((char*)table[pPacket->eth_type],mac,pPacket->addr,netmask)==0)
		if(SysInfo.read_network_info_ifconfig(pPacket->eth_name,mac,pPacket->addr,netmask)==0)
			pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
		else
			pPanelReq->result=RPC_SRV_RESULT_FAIL;
	}
	return 0;
}
int NetRpc::json_to_bin_set_ip_addr(JsonDataCommObj* pReq)
{
	SYSMGR_NET_INFO_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_NET_INFO_PACKET,RPC_SRV_ACT_WRITE,EJSON_SYSMGR_RPC_SET_IP_ADDR);
	//after assigning pointer to pPanelReq->dataRef, modify cmd-req-obj with correct client arguments
JSON_STRING_TO_ENUM(SYSMGR_RPC_IP_ADDR_ARG_IFACE,SYSMGR_RPC_IP_ADDR_ARG_IFACE_TABL,EJSON_SYSMGR_IFACE_TYPE,EJSON_SYSMGR_IFACE_TYPE_UNKNOWN,pPanelCmdObj->eth_type);
	return 0;
}
int NetRpc::bin_to_json_set_ip_addr(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,SYSMGR_NET_INFO_PACKET);
	return 0;
}
int NetRpc::process_set_ip_addr(JsonDataCommObj* pReq)
{
	//TODO:dont supprt mac writing yet
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	pPanelReq->result=RPC_SRV_RESULT_FEATURE_UNSUPPORTED;
	return 0;
}
/* ------------------------------------------------------------------------- */
int NetRpc::json_to_bin_get_netmask(JsonDataCommObj* pReq)
{
	SYSMGR_NET_INFO_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_NET_INFO_PACKET,RPC_SRV_ACT_READ,EJSON_SYSMGR_RPC_GET_NETMASK);
	//after assigning pointer to pPanelReq->dataRef, modify cmd-req-obj with correct client arguments
//JSON_STRING_TO_ENUM(SYSMGR_RPC_NETMASK_ARG_IFACE,SYSMGR_RPC_NETMASK_ARG_IFACE_TABL,EJSON_SYSMGR_IFACE_TYPE,EJSON_SYSMGR_IFACE_TYPE_UNKNOWN,pPanelCmdObj->eth_type);
	JSON_STRING_TO_STRING(SYSMGR_RPC_IP_ADDR_ARG_IFACE,pPanelCmdObj->eth_name);//changed from fixed enum list to string
	return 0;
}
int NetRpc::bin_to_json_get_netmask(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,SYSMGR_NET_INFO_PACKET,SYSMGR_RPC_NETMASK_ARG,addr);
	return 0;
}
int NetRpc::process_get_netmask(JsonDataCommObj* pReq)
{
	ADSysInfo SysInfo;//lib-class for reading cpu-info and system-info
	char ip[512];char mac[512];

	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SYSMGR_NET_INFO_PACKET* pPacket;
	pPacket=(SYSMGR_NET_INFO_PACKET*)pPanelReq->dataRef;

	const char *table[]   = SYSMGR_RPC_MAC_ADDR_ARG_IFACE_TABL;
	//if network is connected,then read details from SysInfo
	//if(SysInfo.read_network_info((char*)table[pPacket->eth_type],mac,ip,pPacket->addr)==0)
	if(SysInfo.read_network_info(pPacket->eth_name,mac,ip,pPacket->addr)==0)
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	else
	{
		//if network is not connected, use ifconfig method
		//if(SysInfo.read_network_info_ifconfig((char*)table[pPacket->eth_type],mac,ip,pPacket->addr)==0)
		if(SysInfo.read_network_info_ifconfig(pPacket->eth_name,mac,ip,pPacket->addr)==0)
			pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
		else
			pPanelReq->result=RPC_SRV_RESULT_FAIL;
	}
	return 0;
}
int NetRpc::json_to_bin_set_netmask(JsonDataCommObj* pReq)
{
	SYSMGR_NET_INFO_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_NET_INFO_PACKET,RPC_SRV_ACT_WRITE,EJSON_SYSMGR_RPC_SET_NETMASK);
	//after assigning pointer to pPanelReq->dataRef, modify cmd-req-obj with correct client arguments
JSON_STRING_TO_ENUM(SYSMGR_RPC_NETMASK_ARG_IFACE,SYSMGR_RPC_NETMASK_ARG_IFACE_TABL,EJSON_SYSMGR_IFACE_TYPE,EJSON_SYSMGR_IFACE_TYPE_UNKNOWN,pPanelCmdObj->eth_type);
	return 0;
}
int NetRpc::bin_to_json_set_netmask(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,SYSMGR_NET_INFO_PACKET);
	return 0;
}
int NetRpc::process_set_netmask(JsonDataCommObj* pReq)
{
	//TODO:dont supprt mac writing yet
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	pPanelReq->result=RPC_SRV_RESULT_FEATURE_UNSUPPORTED;
	return 0;
}
/* ------------------------------------------------------------------------- */
