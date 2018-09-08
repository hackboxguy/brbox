#include "Pcf8574Rpc.h"
//#include "RaspiIo.h"
#include "ADEvntNotifier.hpp"//for eventing
/* ------------------------------------------------------------------------- */
Pcf8574Rpc:: Pcf8574Rpc(std::string rpcName,int myIndex,bool emu, bool log,I2CSRV_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
	pDataCache=pData;	
}
/* ------------------------------------------------------------------------- */
Pcf8574Rpc::~ Pcf8574Rpc()
{
}
/* ------------------------------------------------------------------------- */
int Pcf8574Rpc::MapJsonToBinary(JsonDataCommObj* pReq,int index)
{
	EJSON_I2CSRV_RPC_TYPES command =(EJSON_I2CSRV_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_I2CSRV_RPC_PCF8574_GET :return json_to_bin_pcf_get(pReq);
		case EJSON_I2CSRV_RPC_PCF8574_SET :return json_to_bin_pcf_set(pReq);
		default:break;
	}
	return -1;//0;
}
/* ------------------------------------------------------------------------- */
int Pcf8574Rpc::MapBinaryToJson(JsonDataCommObj* pReq,int index)
{
	EJSON_I2CSRV_RPC_TYPES command =(EJSON_I2CSRV_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_I2CSRV_RPC_PCF8574_GET :return bin_to_json_pcf_get(pReq);
		case EJSON_I2CSRV_RPC_PCF8574_SET :return bin_to_json_pcf_set(pReq);
		default:break;
	}
	return -1;
}
/* ------------------------------------------------------------------------- */
int Pcf8574Rpc::ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj)
{
	EJSON_I2CSRV_RPC_TYPES command =(EJSON_I2CSRV_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_I2CSRV_RPC_PCF8574_GET :return process_pcf_get(pReq);
		case EJSON_I2CSRV_RPC_PCF8574_SET :return process_pcf_set(pReq);
		default:break;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT Pcf8574Rpc::ProcessWorkAsync(int index,unsigned char* pWorkData)
{
	RPC_SRV_RESULT ret_val=RPC_SRV_RESULT_FAIL;

	return ret_val;
}
/* ------------------------------------------------------------------------- */
int Pcf8574Rpc::json_to_bin_pcf_get(JsonDataCommObj* pReq)
{
	I2CSRV_PCF8574_ACCESS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,I2CSRV_PCF8574_ACCESS_PACKET,RPC_SRV_ACT_READ,EJSON_I2CSRV_RPC_PCF8574_GET);
	JSON_STRING_TO_INT(I2CSRV_RPC_PCF8574_ADDR_ARG,pPanelCmdObj->devaddr);
	return 0;
}
int Pcf8574Rpc::bin_to_json_pcf_get(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_INT(RPC_SRV_REQ,I2CSRV_PCF8574_ACCESS_PACKET,I2CSRV_RPC_PCF8574_DATA_ARG,data);
	return 0;
}
int Pcf8574Rpc::process_pcf_get(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	I2CSRV_PCF8574_ACCESS_PACKET* pPacket;
	pPacket=(I2CSRV_PCF8574_ACCESS_PACKET*)pPanelReq->dataRef;
	if(pPacket->devaddr<0x2) //i2c device cannot have address as 0 or 1 //TODO: more checks for address
	{
		pPanelReq->result=RPC_SRV_RESULT_ARG_ERROR;//out of valid address range
		return 0;
	}
	if(get_emulation_flag())//no h/w present, just simulate
	{
		pPacket->data=pDataCache->pcf8574cache[pPacket->devaddr];
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	}
	else
	{
		//TODO: do tha actual device driver(/dev/i2c-x) access 
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int Pcf8574Rpc::json_to_bin_pcf_set(JsonDataCommObj* pReq)
{
	I2CSRV_PCF8574_ACCESS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,I2CSRV_PCF8574_ACCESS_PACKET,RPC_SRV_ACT_WRITE,EJSON_I2CSRV_RPC_PCF8574_SET);
	JSON_STRING_TO_INT(I2CSRV_RPC_PCF8574_ADDR_ARG,pPanelCmdObj->devaddr);
	JSON_STRING_TO_INT(I2CSRV_RPC_PCF8574_DATA_ARG,pPanelCmdObj->data);
	return 0;
}
int Pcf8574Rpc::bin_to_json_pcf_set(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,I2CSRV_PCF8574_ACCESS_PACKET);
	return 0;
}
int Pcf8574Rpc::process_pcf_set(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	I2CSRV_PCF8574_ACCESS_PACKET* pPacket;
	pPacket=(I2CSRV_PCF8574_ACCESS_PACKET*)pPanelReq->dataRef;
	if(pPacket->devaddr<0x2) //i2c device cannot have address as 0 or 1 //TODO: more checks for address
	{
		pPanelReq->result=RPC_SRV_RESULT_ARG_ERROR;//out of valid address range
		return 0;
	}
	//RaspiIo Raspi;
	if(get_emulation_flag())
	{
		pDataCache->pcf8574cache[pPacket->devaddr]=pPacket->data;
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;//dummy write to memory
	}
	else
	{
		//TODO: do tha actual device driver(/dev/i2c-x) access 
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */


