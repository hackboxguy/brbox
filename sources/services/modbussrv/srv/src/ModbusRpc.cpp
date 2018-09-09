#include "ModbusRpc.h"
//#include "RaspiIo.h"
#include "ADEvntNotifier.hpp"//for eventing
/* ------------------------------------------------------------------------- */
ModbusRpc:: ModbusRpc(std::string rpcName,int myIndex,bool emu, bool log,MODBUSSRV_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
	pDataCache=pData;	
}
/* ------------------------------------------------------------------------- */
ModbusRpc::~ ModbusRpc()
{
}
/* ------------------------------------------------------------------------- */
int ModbusRpc::MapJsonToBinary(JsonDataCommObj* pReq,int index)
{
	EJSON_MODBUSSRV_RPC_TYPES command =(EJSON_MODBUSSRV_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_MODBUSSRV_RPC_VOLTAGE_GET :return json_to_bin_voltage_get(pReq);
		//case EJSON_I2CSRV_RPC_PCF8574_SET :return json_to_bin_pcf_set(pReq);
		default:break;
	}
	return -1;//0;
}
/* ------------------------------------------------------------------------- */
int ModbusRpc::MapBinaryToJson(JsonDataCommObj* pReq,int index)
{
	EJSON_MODBUSSRV_RPC_TYPES command =(EJSON_MODBUSSRV_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_MODBUSSRV_RPC_VOLTAGE_GET :return bin_to_json_voltage_get(pReq);
		//case EJSON_I2CSRV_RPC_PCF8574_SET :return bin_to_json_pcf_set(pReq);
		default:break;
	}
	return -1;
}
/* ------------------------------------------------------------------------- */
int ModbusRpc::ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj)
{
	EJSON_MODBUSSRV_RPC_TYPES command =(EJSON_MODBUSSRV_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_MODBUSSRV_RPC_VOLTAGE_GET :return process_voltage_get(pReq);
		//case EJSON_I2CSRV_RPC_PCF8574_SET :return process_pcf_set(pReq);
		default:break;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT ModbusRpc::ProcessWorkAsync(int index,unsigned char* pWorkData)
{
	RPC_SRV_RESULT ret_val=RPC_SRV_RESULT_FAIL;

	return ret_val;
}
/* ------------------------------------------------------------------------- */
int ModbusRpc::json_to_bin_voltage_get(JsonDataCommObj* pReq)
{
	MODBUSSRV_VOLTAGE_ACCESS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,MODBUSSRV_VOLTAGE_ACCESS_PACKET,RPC_SRV_ACT_READ,EJSON_MODBUSSRV_RPC_VOLTAGE_GET);
	//JSON_STRING_TO_INT(I2CSRV_RPC_PCF8574_ADDR_ARG,pPanelCmdObj->devaddr);
	return 0;
}
int ModbusRpc::bin_to_json_voltage_get(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_INT(RPC_SRV_REQ,MODBUSSRV_VOLTAGE_ACCESS_PACKET,MODBUSSRV_RPC_VOLTAGE_ARG,voltage);
	return 0;
}
int ModbusRpc::process_voltage_get(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	MODBUSSRV_VOLTAGE_ACCESS_PACKET* pPacket;
	pPacket=(MODBUSSRV_VOLTAGE_ACCESS_PACKET*)pPanelReq->dataRef;
	//if(pPacket->devaddr<0x2) //i2c device cannot have address as 0 or 1 //TODO: more checks for address
	//{
	//	pPanelReq->result=RPC_SRV_RESULT_ARG_ERROR;//out of valid address range
	//	return 0;
	//}
	if(get_emulation_flag())//no h/w present, just simulate
	{
		pPacket->voltage=pDataCache->voltage;//[pPacket->devaddr];
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



