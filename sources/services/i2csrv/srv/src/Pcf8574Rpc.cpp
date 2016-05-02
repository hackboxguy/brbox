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
	//GPIOCTL_IO_ACCESS_PACKET* pPanelCmdObj=NULL;
	//PREPARE_JSON_REQUEST(RPC_SRV_REQ,GPIOCTL_IO_ACCESS_PACKET,RPC_SRV_ACT_READ,EJSON_GPIOCTL_RPC_IO_GET);
	//JSON_STRING_TO_INT(GPIOCTL_RPC_IO_ADDR_ARG,pPanelCmdObj->addr);
	return 0;
}
int Pcf8574Rpc::bin_to_json_pcf_get(JsonDataCommObj* pReq)
{
	//PREPARE_JSON_RESP_INT(RPC_SRV_REQ,GPIOCTL_IO_ACCESS_PACKET,GPIOCTL_RPC_IO_DATA_ARG,data);
	return 0;
}
int Pcf8574Rpc::process_pcf_get(JsonDataCommObj* pReq)
{
	/*RaspiIo Raspi;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	GPIOCTL_IO_ACCESS_PACKET* pPacket;
	pPacket=(GPIOCTL_IO_ACCESS_PACKET*)pPanelReq->dataRef;

	if(pPacket->addr>=GPIOCTL_MAX_GPIO_PINS)
	{
		pPanelReq->result=RPC_SRV_RESULT_ARG_ERROR;//max allowed gpio pins or 0 to 63
		return 0;
	}

	pPacket->data=pDataCache->gpio_data[pPacket->addr];
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	//pPanelReq->result=Raspi.ReadGPIO (pPacket->addr,pPacket->data);

	//set debug logging flag
	//ImgId.logflag=get_debug_log_flag();//get_debug_log_flag() is a function f parent class ADJsonRpcMgrConsumer
	//pPanelReq->result=ImgId.IdentifyPattern(pPacket->pattern_type,pDataCache->StrImgIdDebugFile);*/
	return 0;
}
/* ------------------------------------------------------------------------- */
int Pcf8574Rpc::json_to_bin_pcf_set(JsonDataCommObj* pReq)
{
	/*GPIOCTL_IO_ACCESS_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,GPIOCTL_IO_ACCESS_PACKET,RPC_SRV_ACT_WRITE,EJSON_GPIOCTL_RPC_IO_SET);
	JSON_STRING_TO_INT(GPIOCTL_RPC_IO_ADDR_ARG,pPanelCmdObj->addr);
	JSON_STRING_TO_INT(GPIOCTL_RPC_IO_DATA_ARG,pPanelCmdObj->data);*/
	return 0;
}
int Pcf8574Rpc::bin_to_json_pcf_set(JsonDataCommObj* pReq)
{
	//PREPARE_JSON_RESP(RPC_SRV_REQ,GPIOCTL_IO_ACCESS_PACKET);
	return 0;
}
int Pcf8574Rpc::process_pcf_set(JsonDataCommObj* pReq)
{
	/*RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	GPIOCTL_IO_ACCESS_PACKET* pPacket;
	pPacket=(GPIOCTL_IO_ACCESS_PACKET*)pPanelReq->dataRef;
	if(pPacket->addr>=GPIOCTL_MAX_GPIO_PINS)
	{
		pPanelReq->result=RPC_SRV_RESULT_ARG_ERROR;//max allowed gpio pins or 0 to 63
		return 0;
	}
	RaspiIo Raspi;
	if(get_emulation_flag())
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;//dummy write to memory
	else
		pPanelReq->result=Raspi.WriteGPIO(pPacket->addr,pPacket->data);
	if(pPanelReq->result==RPC_SRV_RESULT_SUCCESS)
	{
		pDataCache->gpio_data[pPacket->addr]=pPacket->data;//store last set gpio value in my local-cache
		if(pDataCache->gpio_data[pPacket->addr]!=pDataCache->gpio_data_prev[pPacket->addr])
		{
			pDataCache->gpio_data_prev[pPacket->addr]=pDataCache->gpio_data[pPacket->addr];
			//gpio value changed, notify subscribers
			ADEvntNotifier* pNotifier=(ADEvntNotifier*)pDataCache->pEventNotifier;
			pNotifier->NotifyEvent(EGPIOCTL_EVENT_TYPE_OUTPUT_CHANGED,pPacket->addr,SERVER_JSON_PORT_NUM);
			//NOTIFY_EVENT(EGPIOCTL_EVENT_TYPE_OUTPUT_CHANGED,pPacket->addr,SERVER_JSON_PORT_NUM);
		}
	}*/
	return 0;
}
/* ------------------------------------------------------------------------- */


