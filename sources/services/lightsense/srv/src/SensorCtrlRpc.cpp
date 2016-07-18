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
		case EJSON_LIGHTSENSE_GET_INTEGRATION_TIME:return json_to_bin_get_integration_time(pReq);
		case EJSON_LIGHTSENSE_SET_INTEGRATION_TIME:return json_to_bin_set_integration_time(pReq);
		case EJSON_LIGHTSENSE_SYNC_EDGE_GET   :return json_to_bin_get_sync_edge(pReq);
		case EJSON_LIGHTSENSE_SYNC_EDGE_SET   :return json_to_bin_set_sync_edge(pReq);
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
		case EJSON_LIGHTSENSE_GET_INTEGRATION_TIME:return bin_to_json_get_integration_time(pReq);
		case EJSON_LIGHTSENSE_SET_INTEGRATION_TIME:return bin_to_json_set_integration_time(pReq);
		case EJSON_LIGHTSENSE_SYNC_EDGE_GET   :return bin_to_json_get_sync_edge(pReq);
		case EJSON_LIGHTSENSE_SYNC_EDGE_SET   :return bin_to_json_set_sync_edge(pReq);
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
		case EJSON_LIGHTSENSE_GET_INTEGRATION_TIME:return process_get_integration_time(pReq,pDataCache);
		case EJSON_LIGHTSENSE_SET_INTEGRATION_TIME:return process_set_integration_time(pReq,pDataCache);
		case EJSON_LIGHTSENSE_SYNC_EDGE_GET   :return process_get_sync_edge(pReq,pDataCache);
		case EJSON_LIGHTSENSE_SYNC_EDGE_SET   :return process_set_sync_edge(pReq,pDataCache);
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
//EJSON_LIGHTSENSE_GET_INTEGRATION_TIME
int SensorCtrlRpc::json_to_bin_get_integration_time(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_READ,EJSON_LIGHTSENSE_GET_INTEGRATION_TIME);
	return 0;
}
int SensorCtrlRpc::bin_to_json_get_integration_time(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
        LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj;
        pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
        pPanelCmdObj=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
        if(pPanelReq->result!=RPC_SRV_RESULT_SUCCESS)
                prepare_result_string(pPanelReq->result,pReq);
	else
		prepare_result_string(pPanelReq->result,pReq,(char*)LIGHTSENSE_RPC_SET_INTEGRATION_TIME_ARG,pPanelCmdObj->integration_time);
	OBJ_MEM_DELETE(pPanelCmdObj);
	return 0;
}
int SensorCtrlRpc::process_get_integration_time(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
	{
		uint32_t time;
		pPanelReq->result=pSensr->get_integration_time(time);
		pPacket->integration_time=time;
	}
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_SET_INTEGRATION_TIME
int SensorCtrlRpc::json_to_bin_set_integration_time(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
        PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_WRITE,EJSON_LIGHTSENSE_SET_INTEGRATION_TIME);
	JSON_STRING_TO_INT(LIGHTSENSE_RPC_SET_INTEGRATION_TIME_ARG,pPanelCmdObj->integration_time);
	return 0;
}
int SensorCtrlRpc::bin_to_json_set_integration_time(JsonDataCommObj* pReq)
{
        PREPARE_JSON_RESP(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET);
	return 0;
}
int SensorCtrlRpc:: process_set_integration_time(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->set_integration_time(pPacket->integration_time);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_SYNC_EDGE_GET
int SensorCtrlRpc::json_to_bin_get_sync_edge(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_READ,EJSON_LIGHTSENSE_SYNC_EDGE_GET);
	return 0;
}
int SensorCtrlRpc::bin_to_json_get_sync_edge(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,LIGHTSENSE_RPC_SYNC_EDGE_ARG,sync_edge,ADLIB_STATUS_FLAG_TYPE_TABL,ADLIB_STATUS_FLAG_TYPE_UNKNOWN);
	return 0;
}
int SensorCtrlRpc::process_get_sync_edge(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->get_sync_edge(pPacket->sync_edge);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_SYNC_EDGE_SET
int SensorCtrlRpc::json_to_bin_set_sync_edge(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
        PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_WRITE,EJSON_LIGHTSENSE_SYNC_EDGE_SET);
	JSON_STRING_TO_ENUM(LIGHTSENSE_RPC_SYNC_EDGE_ARG,ADLIB_STATUS_FLAG_TYPE_TABL,ADLIB_STATUS_FLAG_TYPE,ADLIB_STATUS_FLAG_TYPE_UNKNOWN,pPanelCmdObj->sync_edge);
	return 0;
}
int SensorCtrlRpc::bin_to_json_set_sync_edge(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET);
	return 0;
}
int SensorCtrlRpc::process_set_sync_edge(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->set_sync_edge(pPacket->sync_edge);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */

