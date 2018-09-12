#include "ModbusRpc.h"
//#include "RaspiIo.h"
#include "ADEvntNotifier.hpp"//for eventing
#include "MODBusAccess.h"
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
		case EJSON_MODBUSSRV_RPC_ENRGYPARM_GET :return json_to_bin_energyparm_get(pReq);
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
		case EJSON_MODBUSSRV_RPC_ENRGYPARM_GET :return bin_to_json_energyparm_get(pReq);
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
		case EJSON_MODBUSSRV_RPC_ENRGYPARM_GET :return process_energyparm_get(pReq);
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
int ModbusRpc::json_to_bin_energyparm_get(JsonDataCommObj* pReq)
{
	MODBUSSRV_ENRGYPARM_ACCESS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,MODBUSSRV_ENRGYPARM_ACCESS_PACKET,RPC_SRV_ACT_READ,EJSON_MODBUSSRV_RPC_ENRGYPARM_GET);
	//extract "parameter" argument from json string
JSON_STRING_TO_ENUM(MODBUSSRV_RPC_ENRGYPARM_ARG,MODBUSSRV_RPC_ENRGYPARM_ARG_TABL,EJSON_ENRGYPARM,EJSON_ENRGYPARM_UNKNOWN,pPanelCmdObj->param_type);

	//MODBUSSRV_ENRGYPARM_ACCESS_PACKET* pPanelCmdObj=NULL;
	//PREPARE_JSON_REQUEST(RPC_SRV_REQ,MODBUSSRV_ENRGYPARM_ACCESS_PACKET,RPC_SRV_ACT_READ,EJSON_MODBUSSRV_RPC_ENRGYPARM_GET);
	//JSON_STRING_TO_INT(I2CSRV_RPC_PCF8574_ADDR_ARG,pPanelCmdObj->devaddr);
	return 0;
}
int ModbusRpc::bin_to_json_energyparm_get(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,MODBUSSRV_ENRGYPARM_ACCESS_PACKET,MODBUSSRV_RPC_ENRGYPARM_RESP_ARG,parameter);
	//PREPARE_JSON_RESP_INT(RPC_SRV_REQ,MODBUSSRV_ENRGYPARM_ACCESS_PACKET,MODBUSSRV_RPC_ENRGYPARM_ARG,parameter);
	return 0;
}
int ModbusRpc::process_energyparm_get(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	MODBUSSRV_ENRGYPARM_ACCESS_PACKET* pPacket;
	pPacket=(MODBUSSRV_ENRGYPARM_ACCESS_PACKET*)pPanelReq->dataRef;

	strcpy(pPacket->parameter,"uninitialized");
	if(get_emulation_flag())//no h/w present, just simulate
	{
		//pPacket->parameter=pDataCache->energy_param;//[pPacket->devaddr];
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	}
	else
	{
		MODBusAccess *pMODBus=(MODBusAccess*)pDataCache->pDevAccess;
		if(pDataCache->pDevAccess==NULL)
		{
			pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
			return 0;		
		}
		//getEnergyParam(EJSON_ENRGYPARM,std::string &result)
		std::string tmpStr;
		pPanelReq->result=pMODBus->getEnergyParam(pPacket->param_type,tmpStr);
		if(pPanelReq->result==RPC_SRV_RESULT_SUCCESS)
			strcpy(pPacket->parameter,tmpStr.c_str());

		//TODO: do tha actual device modbus access 
		//pPacket->parameter=231;//pDataCache->voltage;//[pPacket->devaddr];
		//pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
		//pPanelReq->result=RPC_SRV_RESULT_FAIL;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */



