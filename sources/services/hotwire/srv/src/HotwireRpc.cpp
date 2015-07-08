#include "HotwireRpc.h"
/* ------------------------------------------------------------------------- */
GpioCtrlRpc:: GpioCtrlRpc(std::string rpcName,int myIndex,bool emu, bool log,GPIOCTL_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
	pDataCache=pData;
	pDataCache->ActType=GPIOCTL_OMXACT_IDLE;
}
/* ------------------------------------------------------------------------- */
GpioCtrlRpc::~ GpioCtrlRpc()
{
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::MapJsonToBinary(JsonDataCommObj* pReq,int index)
{
	EJSON_GPIOCTL_RPC_TYPES command =(EJSON_GPIOCTL_RPC_TYPES)index;
	switch(command)
	{
		//case EJSON_GPIOCTL_RPC_IO_GET :return json_to_bin_gpio_get(pReq);
		//case EJSON_GPIOCTL_RPC_IO_SET :return json_to_bin_gpio_set(pReq);
		case EJSON_GPIOCTL_RPC_OMXACT_GET:return json_to_bin_omxact_get(pReq);
		case EJSON_GPIOCTL_RPC_OMXACT_SET:return json_to_bin_omxact_set(pReq);
		default:break;
	}
	return -1;//0;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::MapBinaryToJson(JsonDataCommObj* pReq,int index)
{
	EJSON_GPIOCTL_RPC_TYPES command =(EJSON_GPIOCTL_RPC_TYPES)index;
	switch(command)
	{
		//case EJSON_GPIOCTL_RPC_IO_GET :return bin_to_json_gpio_get(pReq);
		//case EJSON_GPIOCTL_RPC_IO_SET :return bin_to_json_gpio_set(pReq);
		case EJSON_GPIOCTL_RPC_OMXACT_GET:return bin_to_json_omxact_get(pReq);
		case EJSON_GPIOCTL_RPC_OMXACT_SET:return bin_to_json_omxact_set(pReq);
		default:break;
	}
	return -1;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj)
{
	EJSON_GPIOCTL_RPC_TYPES command =(EJSON_GPIOCTL_RPC_TYPES)index;
	switch(command)
	{
		//case EJSON_GPIOCTL_RPC_IO_GET :return process_gpio_get(pReq);
		//case EJSON_GPIOCTL_RPC_IO_SET :return process_gpio_set(pReq);
		case EJSON_GPIOCTL_RPC_OMXACT_GET:return process_omxact_get(pReq);
		case EJSON_GPIOCTL_RPC_OMXACT_SET:return process_omxact_set(pReq,pObj);
		default:break;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT GpioCtrlRpc::ProcessWorkAsync(int cmd,unsigned char* pWorkData)
{
	RPC_SRV_RESULT ret_val=RPC_SRV_RESULT_FAIL;
	//pDataCache->AsyncCmdInProgress=(EJSON_SYSMGR_RPC_TYPES)cmd;//To know which async command is in progress
	switch(cmd)
	{
		case EJSON_GPIOCTL_RPC_OMXACT_SET:
			{
				GPIOCTL_OMXACT_PACKET *pPacket;
				pPacket=(GPIOCTL_OMXACT_PACKET*)pWorkData;
				ret_val=process_omx_action(pPacket->ActType);
				OBJ_MEM_DELETE(pWorkData);
			}
			break;
		default:
			break;
	}
	//pDataCache->AsyncCmdInProgress=EJSON_SYSMGR_RPC_NONE;
	return ret_val;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::json_to_bin_gpio_get(JsonDataCommObj* pReq)
{
	//GPIOCTL_IO_ACCESS_PACKET* pPanelCmdObj=NULL;
	//PREPARE_JSON_REQUEST(RPC_SRV_REQ,GPIOCTL_IO_ACCESS_PACKET,RPC_SRV_ACT_READ,EJSON_GPIOCTL_RPC_IO_GET);
	//JSON_STRING_TO_INT(GPIOCTL_RPC_IO_ADDR_ARG,pPanelCmdObj->addr);
	return 0;
}
int GpioCtrlRpc::bin_to_json_gpio_get(JsonDataCommObj* pReq)
{
	//PREPARE_JSON_RESP_INT(RPC_SRV_REQ,GPIOCTL_IO_ACCESS_PACKET,GPIOCTL_RPC_IO_DATA_ARG,data);
	return 0;
}
int GpioCtrlRpc::process_gpio_get(JsonDataCommObj* pReq)
{
	/*RaspiIo Raspi;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	GPIOCTL_IO_ACCESS_PACKET* pPacket;
	pPacket=(GPIOCTL_IO_ACCESS_PACKET*)pPanelReq->dataRef;

	//pPacket->data=pDataCache->tmpData;
	//pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	pPanelReq->result=Raspi.ReadGPIO (pPacket->addr,pPacket->data);
	//RPC_SRV_RESULT RaspiIo::WriteGPIO(unsigned int addr,unsigned int data)



	//set debug logging flag
	//ImgId.logflag=get_debug_log_flag();//get_debug_log_flag() is a function f parent class ADJsonRpcMgrConsumer
	//pPanelReq->result=ImgId.IdentifyPattern(pPacket->pattern_type,pDataCache->StrImgIdDebugFile);
	*/
	return 0;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::json_to_bin_gpio_set(JsonDataCommObj* pReq)
{
	//GPIOCTL_IO_ACCESS_PACKET* pPanelCmdObj=NULL;
	//PREPARE_JSON_REQUEST(RPC_SRV_REQ,GPIOCTL_IO_ACCESS_PACKET,RPC_SRV_ACT_WRITE,EJSON_GPIOCTL_RPC_IO_SET);
	//JSON_STRING_TO_INT(GPIOCTL_RPC_IO_ADDR_ARG,pPanelCmdObj->addr);
	//JSON_STRING_TO_INT(GPIOCTL_RPC_IO_DATA_ARG,pPanelCmdObj->data);
	return 0;
}
int GpioCtrlRpc::bin_to_json_gpio_set(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,GPIOCTL_IO_ACCESS_PACKET);
	return 0;
}
int GpioCtrlRpc::process_gpio_set(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	GPIOCTL_IO_ACCESS_PACKET* pPacket;
	pPacket=(GPIOCTL_IO_ACCESS_PACKET*)pPanelReq->dataRef;

	/*RaspiIo Raspi;
	pPanelReq->result=Raspi.WriteGPIO(pPacket->addr,pPacket->data);
	//pDataCache->tmpData=pPacket->data;
	//pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	*/
	return 0;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::json_to_bin_omxact_get(JsonDataCommObj* pReq)
{
	GPIOCTL_OMXACT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,GPIOCTL_OMXACT_PACKET,RPC_SRV_ACT_READ,EJSON_GPIOCTL_RPC_OMXACT_GET);
	return 0;
}
int GpioCtrlRpc::bin_to_json_omxact_get(JsonDataCommObj* pReq)
{
PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,GPIOCTL_OMXACT_PACKET,GPIOCTL_RPC_OMXACT_ARG,ActType,GPIOCTL_RPC_OMXACT_ARG_TABL,GPIOCTL_OMXACT_UNKNOWN);
	return 0;
}
int GpioCtrlRpc::process_omxact_get(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	GPIOCTL_OMXACT_PACKET* pPacket;
	pPacket=(GPIOCTL_OMXACT_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->action!=RPC_SRV_ACT_READ)
	{
		pPanelReq->result=RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
		return 0;
	}
	pPacket->ActType=pDataCache->ActType;
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::json_to_bin_omxact_set(JsonDataCommObj* pReq)
{
	GPIOCTL_OMXACT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,GPIOCTL_OMXACT_PACKET,RPC_SRV_ACT_WRITE,EJSON_GPIOCTL_RPC_OMXACT_SET);
	JSON_STRING_TO_ENUM(GPIOCTL_RPC_OMXACT_ARG,GPIOCTL_RPC_OMXACT_ARG_TABL,GPIOCTL_OMXACT_TYPE,GPIOCTL_OMXACT_UNKNOWN,pPanelCmdObj->ActType);
	return 0;
}
int GpioCtrlRpc::bin_to_json_omxact_set(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,GPIOCTL_OMXACT_PACKET);
	return 0;
}
int GpioCtrlRpc::process_omxact_set(JsonDataCommObj* pReq,ADJsonRpcMgrProducer* pObj)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	GPIOCTL_OMXACT_PACKET* pPacket;
	pPacket=(GPIOCTL_OMXACT_PACKET*)pPanelReq->dataRef;

	//create a copy packet and initialize with supplied values
	GPIOCTL_OMXACT_PACKET* pWorkData=NULL;
	OBJECT_MEM_NEW(pWorkData,GPIOCTL_OMXACT_PACKET);//delete this object in run_work() callback function
	if(pWorkData == NULL)
	{
		pPanelReq->result=RPC_SRV_RESULT_MEM_ERROR;
		return -1;
	}
	pWorkData->ActType=pPacket->ActType;
	pPanelReq->result=pObj->PushAsyncTask(EJSON_GPIOCTL_RPC_OMXACT_SET,(unsigned char*)pWorkData,&pPacket->taskID,WORK_CMD_AFTER_DONE_DELETE);//PRESERVE);
	if(pPanelReq->result!=RPC_SRV_RESULT_IN_PROG)
	{
		OBJ_MEM_DELETE(pWorkData);
		return 0;
	}
	else
	{
		//pDataCache->ActType=pPacket->ActType;
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	}
	return 0;		
}
//player window
//mkfifo /tmp/cmd
//omxplayer -o both start.mp4 < /tmp/cmd

//other window
//echo . > /tmp/cmd
//echo -n p > /tmp/cmd (pause)
//echo -n q > /tmp/cmd (quit)
RPC_SRV_RESULT GpioCtrlRpc::process_omx_action(GPIOCTL_OMXACT_TYPE act)
{
	char command[1024];
	if(pDataCache->ActType!=GPIOCTL_OMXACT_IDLE) //if not idle make it idle
	{
		sprintf(command,"echo -n q > /tmp/cmd");
		system(command);
	}
	switch(act)
	{
		case GPIOCTL_OMXACT_START : 
				sprintf(command,"mkfifo /tmp/cmd");
				system(command);
				sprintf(command,"omxplayer -o both %s < /tmp/cmd &",GPIOCTL_OMXACT_START_FILE);
				system(command);
				sprintf(command,"echo . > /tmp/cmd");
				system(command);
				pDataCache->ActType=GPIOCTL_OMXACT_START;//pPacket->ActType;
				break;
		case GPIOCTL_OMXACT_INTR  :
				sprintf(command,"mkfifo /tmp/cmd");
				system(command);
				sprintf(command,"omxplayer -o both %s < /tmp/cmd &",GPIOCTL_OMXACT_INTR_FILE);
				system(command);
				sprintf(command,"echo . > /tmp/cmd");
				system(command);
				pDataCache->ActType=GPIOCTL_OMXACT_INTR;//pPacket->ActType;
				break;
		case GPIOCTL_OMXACT_STOP  :
				sprintf(command,"mkfifo /tmp/cmd");
				system(command);
				sprintf(command,"omxplayer -o both %s < /tmp/cmd &",GPIOCTL_OMXACT_STOP_FILE);
				system(command);
				sprintf(command,"echo . > /tmp/cmd");
				system(command);
				pDataCache->ActType=GPIOCTL_OMXACT_STOP;//pPacket->ActType;
				break;
		case GPIOCTL_OMXACT_WARN  : break;
		case GPIOCTL_OMXACT_IDLE  :
				sprintf(command,"echo -n q > /tmp/cmd");
				system(command);
				pDataCache->ActType=GPIOCTL_OMXACT_IDLE;
				break;
		default:
			break;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */

