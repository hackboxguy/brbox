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
		case EJSON_MODBUSSRV_RPC_IOSTATE_GET   :return json_to_bin_iostate_get(pReq);
		case EJSON_MODBUSSRV_RPC_IOSTATE_SET   :return json_to_bin_iostate_set(pReq);
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
		case EJSON_MODBUSSRV_RPC_IOSTATE_GET   :return bin_to_json_iostate_get(pReq);
		case EJSON_MODBUSSRV_RPC_IOSTATE_SET   :return bin_to_json_iostate_set(pReq);
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
		case EJSON_MODBUSSRV_RPC_IOSTATE_GET   :return process_iostate_get(pReq);
		case EJSON_MODBUSSRV_RPC_IOSTATE_SET   :return process_iostate_set(pReq);
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
	return 0;
}
int ModbusRpc::bin_to_json_energyparm_get(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,MODBUSSRV_ENRGYPARM_ACCESS_PACKET,MODBUSSRV_RPC_ENRGYPARM_RESP_ARG,parameter);
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
int ModbusRpc::json_to_bin_iostate_get(JsonDataCommObj* pReq)
{
	MODBUSSRV_IO_ACCESS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,MODBUSSRV_IO_ACCESS_PACKET,RPC_SRV_ACT_READ,EJSON_MODBUSSRV_RPC_IOSTATE_GET);
	JSON_STRING_TO_INT(MODBUSSRV_RPC_IOSTATE_SLAVEADDR_ARG,pPanelCmdObj->slaveaddr);
	JSON_STRING_TO_INT(MODBUSSRV_RPC_IOSTATE_IOADDR_ARG,pPanelCmdObj->ioaddr);
	return 0;
}
int ModbusRpc::bin_to_json_iostate_get(JsonDataCommObj* pReq)
{
PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,MODBUSSRV_IO_ACCESS_PACKET,MODBUSSRV_RPC_IOSTATE_ARG,state,MODBUSSRV_RPC_IOSTATE_ARG_TABL,EJSON_IOSTATE_UNKNOWN);
	return 0;
}
int ModbusRpc::process_iostate_get(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	MODBUSSRV_IO_ACCESS_PACKET* pPacket;
	pPacket=(MODBUSSRV_IO_ACCESS_PACKET*)pPanelReq->dataRef;

	if(get_emulation_flag())//no h/w present, just simulate
	{
		pPacket->state=EJSON_IOSTATE_OFF;//just simulate state as off
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	}

	MODBusAccess *pMODBus=(MODBusAccess*)pDataCache->pDevAccess;
	if(pDataCache->pDevAccess==NULL)
	{
		pPanelReq->result=RPC_SRV_RESULT_FAIL;//SUCCESS;
		return 0;		
	}

	uint16_t value=2;
	pPanelReq->result=pMODBus->read_register(pMODBus->ctx,pPacket->slaveaddr,pPacket->ioaddr,&value);
	if(pPanelReq->result==RPC_SRV_RESULT_SUCCESS)
	{
		if(value==0)
			pPacket->state=EJSON_IOSTATE_OFF;
		else if(value==1)
			pPacket->state=EJSON_IOSTATE_ON;
		else
			pPacket->state=EJSON_IOSTATE_UNKNOWN;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int ModbusRpc::json_to_bin_iostate_set(JsonDataCommObj* pReq)
{
	MODBUSSRV_IO_ACCESS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,MODBUSSRV_IO_ACCESS_PACKET,RPC_SRV_ACT_READ,EJSON_MODBUSSRV_RPC_IOSTATE_GET);
	JSON_STRING_TO_INT(MODBUSSRV_RPC_IOSTATE_SLAVEADDR_ARG,pPanelCmdObj->slaveaddr);
	JSON_STRING_TO_INT(MODBUSSRV_RPC_IOSTATE_IOADDR_ARG,pPanelCmdObj->ioaddr);
JSON_STRING_TO_ENUM(MODBUSSRV_RPC_IOSTATE_ARG,MODBUSSRV_RPC_IOSTATE_ARG_TABL,EJSON_IOSTATE,EJSON_IOSTATE_UNKNOWN,pPanelCmdObj->state);
	return 0;
}
int ModbusRpc::bin_to_json_iostate_set(JsonDataCommObj* pReq)
{
PREPARE_JSON_RESP(RPC_SRV_REQ,MODBUSSRV_IO_ACCESS_PACKET);
	return 0;
}
int ModbusRpc::process_iostate_set(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	MODBUSSRV_IO_ACCESS_PACKET* pPacket;
	pPacket=(MODBUSSRV_IO_ACCESS_PACKET*)pPanelReq->dataRef;

	if(get_emulation_flag())//no h/w present, just simulate
	{
		//just accept value, dont do anything
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	}

	MODBusAccess *pMODBus=(MODBusAccess*)pDataCache->pDevAccess;
	if(pDataCache->pDevAccess==NULL)
	{
		pPanelReq->result=RPC_SRV_RESULT_FAIL;//SUCCESS;
		return 0;		
	}

	uint16_t value;
	if(pPacket->state==EJSON_IOSTATE_OFF)
		value=2;
	else if(pPacket->state==EJSON_IOSTATE_ON)
		value=1;
	else
	{
		pPanelReq->result=RPC_SRV_RESULT_ARG_ERROR;
		return 0;
	}
	value<<=8; //lsb must be first in the packet
	pPanelReq->result=pMODBus->write_register(pMODBus->ctx,pPacket->slaveaddr,pPacket->ioaddr,value);
	return 0;
}
/* ------------------------------------------------------------------------- */


