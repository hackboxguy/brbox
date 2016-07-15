#include "SensorCtrlRpc.h"
//#include "DispAccess.h"
#include "ADEvntNotifier.hpp"//for eventing
/* ------------------------------------------------------------------------- */
SensorCtrlRpc:: SensorCtrlRpc(std::string rpcName,int myIndex,bool emu, bool log,LIGHTSENSE_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
	pDataCache=pData;	
}
/* ------------------------------------------------------------------------- */
SensorCtrlRpc::~ SensorCtrlRpc()
{
}
/* ------------------------------------------------------------------------- */
int SensorCtrlRpc::MapJsonToBinary(JsonDataCommObj* pReq,int index)
{
	EJSON_LIGHTSENSE_RPC_TYPES command =(EJSON_LIGHTSENSE_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_LIGHTSENSE_RPC_SENSOR_INIT :return json_to_bin_sensor_init(pReq);
		case EJSON_LIGHTSENSE_RPC_READ_XYZ    :return json_to_bin_read_xyz(pReq);
		default:break;
	}
	return -1;//0;
}
/* ------------------------------------------------------------------------- */
int SensorCtrlRpc::MapBinaryToJson(JsonDataCommObj* pReq,int index)
{
	EJSON_LIGHTSENSE_RPC_TYPES command =(EJSON_LIGHTSENSE_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_LIGHTSENSE_RPC_SENSOR_INIT :return bin_to_json_sensor_init(pReq);
		case EJSON_LIGHTSENSE_RPC_READ_XYZ    :return bin_to_json_read_xyz(pReq);
		default:break;
	}
	return -1;
}
/* ------------------------------------------------------------------------- */
int SensorCtrlRpc::ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj)
{
	EJSON_LIGHTSENSE_RPC_TYPES command =(EJSON_LIGHTSENSE_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_LIGHTSENSE_RPC_SENSOR_INIT :return process_sensor_init(pReq,pDataCache);
		case EJSON_LIGHTSENSE_RPC_READ_XYZ    :return process_read_xyz(pReq,pDataCache);
		default:break;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT SensorCtrlRpc::ProcessWorkAsync(int index,unsigned char* pWorkData)
{
	RPC_SRV_RESULT ret_val=RPC_SRV_RESULT_FAIL;

	return ret_val;
}
/* ------------------------------------------------------------------------- */
//EJSON_DISPSRV_RPC_DISP_INIT
int SensorCtrlRpc::json_to_bin_sensor_init(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_WRITE,EJSON_LIGHTSENSE_RPC_SENSOR_INIT);
	return 0;
}
int SensorCtrlRpc::bin_to_json_sensor_init(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET);
	return 0;
}
int SensorCtrlRpc::process_sensor_init(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->init_sensor();
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_RPC_READ_XYZ
int SensorCtrlRpc::json_to_bin_read_xyz(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_READ,EJSON_LIGHTSENSE_RPC_READ_XYZ);
	return 0;
}
int SensorCtrlRpc::bin_to_json_read_xyz(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET);
	return 0;
}
int SensorCtrlRpc::process_read_xyz(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->read_xyz();
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_DISPSRV_RPC_DISP_CLEAR
/*int SensorCtrlRpc::json_to_bin_clear_display(JsonDataCommObj* pReq)
{
	DISPSRV_PRINT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,DISPSRV_PRINT_PACKET,RPC_SRV_ACT_WRITE,EJSON_DISPSRV_RPC_DISP_CLEAR);
	return 0;
}
int SensorCtrlRpc::bin_to_json_clear_display(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,DISPSRV_PRINT_PACKET);
	return 0;
}
int SensorCtrlRpc::process_clear_display(JsonDataCommObj* pReq,DISPSRV_CMN_DATA_CACHE *pData)
{
	DisplayDevice *pDisp=pData->pDisplay;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	DISPSRV_PRINT_PACKET* pPacket;
	pPacket=(DISPSRV_PRINT_PACKET*)pPanelReq->dataRef;
	if(pData->pDisplay!=NULL)
		pPanelReq->result=pDisp->clear_display();
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}*/
/* ------------------------------------------------------------------------- */

