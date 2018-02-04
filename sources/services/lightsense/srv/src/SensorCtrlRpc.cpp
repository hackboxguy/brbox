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
		case EJSON_LIGHTSENSE_RPC_SENSOR_INIT     :return json_to_bin_sensor_init(pReq);
		case EJSON_LIGHTSENSE_RPC_START_MEASURE   :return json_to_bin_start_measure(pReq);
		case EJSON_LIGHTSENSE_GET_INTEGRATION_TIME:return json_to_bin_get_integration_time(pReq);
		case EJSON_LIGHTSENSE_SET_INTEGRATION_TIME:return json_to_bin_set_integration_time(pReq);
		case EJSON_LIGHTSENSE_SYNC_EDGE_GET       :return json_to_bin_get_sync_edge(pReq);
		case EJSON_LIGHTSENSE_SYNC_EDGE_SET       :return json_to_bin_set_sync_edge(pReq);
		case EJSON_LIGHTSENSE_INTEGRATION_MODE_GET:return json_to_bin_get_integration_mode(pReq);
		case EJSON_LIGHTSENSE_INTEGRATION_MODE_SET:return json_to_bin_set_integration_mode(pReq);
		case EJSON_LIGHTSENSE_INTR_STOP_STS_GET   :return json_to_bin_get_intrstop_sts(pReq);
		case EJSON_LIGHTSENSE_INTR_STOP_STS_SET   :return json_to_bin_set_intrstop_sts(pReq);
		case EJSON_LIGHTSENSE_INTR_MODE_GET       :return json_to_bin_get_intr_mode(pReq);
		case EJSON_LIGHTSENSE_INTR_MODE_SET       :return json_to_bin_set_intr_mode(pReq);
		case EJSON_LIGHTSENSE_INTR_RATE_GET       :return json_to_bin_get_intr_rate(pReq);
		case EJSON_LIGHTSENSE_INTR_RATE_SET       :return json_to_bin_set_intr_rate(pReq);
		case EJSON_LIGHTSENSE_INTR_SOURCE_GET     :return json_to_bin_get_intr_source(pReq);
		case EJSON_LIGHTSENSE_INTR_SOURCE_SET     :return json_to_bin_set_intr_source(pReq);
		case EJSON_LIGHTSENSE_GAIN_MODE_GET       :return json_to_bin_get_gain_mode(pReq);
		case EJSON_LIGHTSENSE_GAIN_MODE_SET       :return json_to_bin_set_gain_mode(pReq);
		case EJSON_LIGHTSENSE_PRESCALER_GET       :return json_to_bin_get_prescaler(pReq);
		case EJSON_LIGHTSENSE_PRESCALER_SET       :return json_to_bin_set_prescaler(pReq);
		case EJSON_LIGHTSENSE_RGBWCOUNT_GET       :return json_to_bin_get_rgbwcount(pReq);
		case EJSON_LIGHTSENSE_WAVELENGTH_COUNT_GET:return json_to_bin_get_wavelength_count(pReq);
		case EJSON_LIGHTSENSE_WAVELENGTH_ITEM_GET :return json_to_bin_get_wavelength_item(pReq);
		case EJSON_LIGHTSENSE_SPECTRUM_COUNT_GET  :return json_to_bin_get_spectrum_count(pReq);
		case EJSON_LIGHTSENSE_SPECTRUM_ITEM_GET   :return json_to_bin_get_spectrum_item(pReq);
		case EJSON_LIGHTSENSE_FIRST_LEASED_IP_GET :return json_to_bin_get_leased_ip(pReq);
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
		case EJSON_LIGHTSENSE_RPC_SENSOR_INIT     :return bin_to_json_sensor_init(pReq);
		case EJSON_LIGHTSENSE_RPC_START_MEASURE   :return bin_to_json_start_measure(pReq);
		case EJSON_LIGHTSENSE_GET_INTEGRATION_TIME:return bin_to_json_get_integration_time(pReq);
		case EJSON_LIGHTSENSE_SET_INTEGRATION_TIME:return bin_to_json_set_integration_time(pReq);
		case EJSON_LIGHTSENSE_SYNC_EDGE_GET       :return bin_to_json_get_sync_edge(pReq);
		case EJSON_LIGHTSENSE_SYNC_EDGE_SET       :return bin_to_json_set_sync_edge(pReq);
		case EJSON_LIGHTSENSE_INTEGRATION_MODE_GET:return bin_to_json_get_integration_mode(pReq);
		case EJSON_LIGHTSENSE_INTEGRATION_MODE_SET:return bin_to_json_set_integration_mode(pReq);
		case EJSON_LIGHTSENSE_INTR_STOP_STS_GET   :return bin_to_json_get_intrstop_sts(pReq);
		case EJSON_LIGHTSENSE_INTR_STOP_STS_SET   :return bin_to_json_set_intrstop_sts(pReq);
		case EJSON_LIGHTSENSE_INTR_MODE_GET       :return bin_to_json_get_intr_mode(pReq);
		case EJSON_LIGHTSENSE_INTR_MODE_SET       :return bin_to_json_set_intr_mode(pReq);
		case EJSON_LIGHTSENSE_INTR_RATE_GET       :return bin_to_json_get_intr_rate(pReq);
		case EJSON_LIGHTSENSE_INTR_RATE_SET       :return bin_to_json_set_intr_rate(pReq);
		case EJSON_LIGHTSENSE_INTR_SOURCE_GET     :return bin_to_json_get_intr_source(pReq);
		case EJSON_LIGHTSENSE_INTR_SOURCE_SET     :return bin_to_json_set_intr_source(pReq);
		case EJSON_LIGHTSENSE_GAIN_MODE_GET       :return bin_to_json_get_gain_mode(pReq);
		case EJSON_LIGHTSENSE_GAIN_MODE_SET       :return bin_to_json_set_gain_mode(pReq);
		case EJSON_LIGHTSENSE_PRESCALER_GET       :return bin_to_json_get_prescaler(pReq);
		case EJSON_LIGHTSENSE_PRESCALER_SET       :return bin_to_json_set_prescaler(pReq);
		case EJSON_LIGHTSENSE_RGBWCOUNT_GET       :return bin_to_json_get_rgbwcount(pReq);
		case EJSON_LIGHTSENSE_WAVELENGTH_COUNT_GET:return bin_to_json_get_wavelength_count(pReq);
		case EJSON_LIGHTSENSE_WAVELENGTH_ITEM_GET :return bin_to_json_get_wavelength_item(pReq);
		case EJSON_LIGHTSENSE_SPECTRUM_COUNT_GET  :return bin_to_json_get_spectrum_count(pReq);
		case EJSON_LIGHTSENSE_SPECTRUM_ITEM_GET   :return bin_to_json_get_spectrum_item(pReq);
		case EJSON_LIGHTSENSE_FIRST_LEASED_IP_GET :return bin_to_json_get_leased_ip(pReq);
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
		case EJSON_LIGHTSENSE_RPC_SENSOR_INIT     :return process_sensor_init(pReq,pDataCache);
		case EJSON_LIGHTSENSE_RPC_START_MEASURE   :return process_start_measure(pReq,pDataCache);
		case EJSON_LIGHTSENSE_GET_INTEGRATION_TIME:return process_get_integration_time(pReq,pDataCache);
		case EJSON_LIGHTSENSE_SET_INTEGRATION_TIME:return process_set_integration_time(pReq,pDataCache);
		case EJSON_LIGHTSENSE_SYNC_EDGE_GET       :return process_get_sync_edge(pReq,pDataCache);
		case EJSON_LIGHTSENSE_SYNC_EDGE_SET       :return process_set_sync_edge(pReq,pDataCache);
		case EJSON_LIGHTSENSE_INTEGRATION_MODE_GET:return process_get_integration_mode(pReq,pDataCache);
		case EJSON_LIGHTSENSE_INTEGRATION_MODE_SET:return process_set_integration_mode(pReq,pDataCache);
		case EJSON_LIGHTSENSE_INTR_STOP_STS_GET   :return process_get_intrstop_sts(pReq,pDataCache);
		case EJSON_LIGHTSENSE_INTR_STOP_STS_SET   :return process_set_intrstop_sts(pReq,pDataCache);
		case EJSON_LIGHTSENSE_INTR_MODE_GET       :return process_get_intr_mode(pReq,pDataCache);
		case EJSON_LIGHTSENSE_INTR_MODE_SET       :return process_set_intr_mode(pReq,pDataCache);
		case EJSON_LIGHTSENSE_INTR_RATE_GET       :return process_get_intr_rate(pReq,pDataCache);
		case EJSON_LIGHTSENSE_INTR_RATE_SET       :return process_set_intr_rate(pReq,pDataCache);
		case EJSON_LIGHTSENSE_INTR_SOURCE_GET     :return process_get_intr_source(pReq,pDataCache);
		case EJSON_LIGHTSENSE_INTR_SOURCE_SET     :return process_set_intr_source(pReq,pDataCache);
		case EJSON_LIGHTSENSE_GAIN_MODE_GET       :return process_get_gain_mode(pReq,pDataCache);
		case EJSON_LIGHTSENSE_GAIN_MODE_SET       :return process_set_gain_mode(pReq,pDataCache);
		case EJSON_LIGHTSENSE_PRESCALER_GET       :return process_get_prescaler(pReq,pDataCache);
		case EJSON_LIGHTSENSE_PRESCALER_SET       :return process_set_prescaler(pReq,pDataCache);
		case EJSON_LIGHTSENSE_RGBWCOUNT_GET       :return process_get_rgbwcount(pReq,pDataCache);
		case EJSON_LIGHTSENSE_WAVELENGTH_COUNT_GET:return process_get_wavelength_count(pReq,pDataCache);
		case EJSON_LIGHTSENSE_WAVELENGTH_ITEM_GET :return process_get_wavelength_item(pReq,pDataCache);
		case EJSON_LIGHTSENSE_SPECTRUM_COUNT_GET  :return process_get_spectrum_count(pReq,pDataCache);
		case EJSON_LIGHTSENSE_SPECTRUM_ITEM_GET   :return process_get_spectrum_item(pReq,pDataCache);
		case EJSON_LIGHTSENSE_FIRST_LEASED_IP_GET :return process_get_leased_ip(pReq,pDataCache);
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
//EJSON_DISPSRV_RPC_SENSOR_INIT
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
//EJSON_LIGHTSENSE_RPC_START_MEASURE
int SensorCtrlRpc::json_to_bin_start_measure(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_READ,EJSON_LIGHTSENSE_RPC_START_MEASURE);
	return 0;
}
int SensorCtrlRpc::bin_to_json_start_measure(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET);
	return 0;
}
int SensorCtrlRpc::process_start_measure(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->trigger_measurement();
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
//EJSON_LIGHTSENSE_INTEGRATION_MODE_GET
int SensorCtrlRpc::json_to_bin_get_integration_mode(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_READ,EJSON_LIGHTSENSE_INTEGRATION_MODE_GET);
	return 0;
}
int SensorCtrlRpc::bin_to_json_get_integration_mode(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,LIGHTSENSE_RPC_INTEGRATION_MODE_ARG,integration_mode,LS_INTEG_MODE_TABL,LS_INTEG_MODE_UNKNOWN);
	return 0;
}
int SensorCtrlRpc::process_get_integration_mode(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->get_integration_mode(pPacket->integration_mode);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_INTEGRATION_MODE_SET
int SensorCtrlRpc::json_to_bin_set_integration_mode(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
        PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_WRITE,EJSON_LIGHTSENSE_INTEGRATION_MODE_SET);
	JSON_STRING_TO_ENUM(LIGHTSENSE_RPC_INTEGRATION_MODE_ARG,LS_INTEG_MODE_TABL,LS_INTEG_MODE,LS_INTEG_MODE_UNKNOWN,pPanelCmdObj->integration_mode);
	return 0;
}
int SensorCtrlRpc::bin_to_json_set_integration_mode(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET);
	return 0;
}
int SensorCtrlRpc::process_set_integration_mode(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->set_integration_mode(pPacket->integration_mode);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_INTR_STOP_STS_GET
int SensorCtrlRpc::json_to_bin_get_intrstop_sts(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_READ,EJSON_LIGHTSENSE_INTR_STOP_STS_GET);
	return 0;
}
int SensorCtrlRpc::bin_to_json_get_intrstop_sts(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,LIGHTSENSE_RPC_INTR_STOP_STS_ARG,intr_stop_sts,ADLIB_STATUS_FLAG_TYPE_TABL,ADLIB_STATUS_FLAG_TYPE_UNKNOWN);
	return 0;
}
int SensorCtrlRpc::process_get_intrstop_sts(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->get_intr_stop_sts(pPacket->intr_stop_sts);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_INTR_STOP_STS_SET
int SensorCtrlRpc::json_to_bin_set_intrstop_sts(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
        PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_WRITE,EJSON_LIGHTSENSE_INTR_STOP_STS_SET);
	JSON_STRING_TO_ENUM(LIGHTSENSE_RPC_INTR_STOP_STS_ARG,ADLIB_STATUS_FLAG_TYPE_TABL,ADLIB_STATUS_FLAG_TYPE,ADLIB_STATUS_FLAG_TYPE_UNKNOWN,pPanelCmdObj->intr_stop_sts);
	return 0;
}
int SensorCtrlRpc::bin_to_json_set_intrstop_sts(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET);
	return 0;
}
int SensorCtrlRpc::process_set_intrstop_sts(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->set_intr_stop_sts(pPacket->intr_stop_sts);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_INTR_MODE_GET
int SensorCtrlRpc::json_to_bin_get_intr_mode(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_READ,EJSON_LIGHTSENSE_INTR_MODE_GET);
	return 0;
}
int SensorCtrlRpc::bin_to_json_get_intr_mode(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,LIGHTSENSE_RPC_INTR_MODE_ARG,intr_mode,LS_INTR_MODE_TABL,LS_INTR_MODE_UNKNOWN);
	return 0;
}
int SensorCtrlRpc::process_get_intr_mode(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->get_intr_mode(pPacket->intr_mode);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_INTR_MODE_SET
int SensorCtrlRpc::json_to_bin_set_intr_mode(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
        PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_WRITE,EJSON_LIGHTSENSE_INTR_MODE_SET);
	JSON_STRING_TO_ENUM(LIGHTSENSE_RPC_INTR_MODE_ARG,LS_INTR_MODE_TABL,LS_INTR_MODE,LS_INTR_MODE_UNKNOWN,pPanelCmdObj->intr_mode);
	return 0;
}
int SensorCtrlRpc::bin_to_json_set_intr_mode(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET);
	return 0;
}
int SensorCtrlRpc::process_set_intr_mode(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->set_intr_mode(pPacket->intr_mode);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_INTR_RATE_GET
int SensorCtrlRpc::json_to_bin_get_intr_rate(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_READ,EJSON_LIGHTSENSE_INTR_RATE_GET);
	return 0;
}
int SensorCtrlRpc::bin_to_json_get_intr_rate(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,LIGHTSENSE_RPC_INTR_RATE_ARG,intr_rate,LS_INTR_RATE_TABL,LS_INTR_RATE_UNKNOWN);
	return 0;
}
int SensorCtrlRpc::process_get_intr_rate(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->get_intr_rate(pPacket->intr_rate);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_INTR_RATE_SET
int SensorCtrlRpc::json_to_bin_set_intr_rate(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
        PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_WRITE,EJSON_LIGHTSENSE_INTR_RATE_SET);
	JSON_STRING_TO_ENUM(LIGHTSENSE_RPC_INTR_RATE_ARG,LS_INTR_RATE_TABL,LS_INTR_RATE,LS_INTR_RATE_UNKNOWN,pPanelCmdObj->intr_rate);
	return 0;
}
int SensorCtrlRpc::bin_to_json_set_intr_rate(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET);
	return 0;
}
int SensorCtrlRpc::process_set_intr_rate(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->set_intr_rate(pPacket->intr_rate);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_INTR_SOURCE_GET
int SensorCtrlRpc::json_to_bin_get_intr_source(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_READ,EJSON_LIGHTSENSE_INTR_SOURCE_GET);
	return 0;
}
int SensorCtrlRpc::bin_to_json_get_intr_source(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,LIGHTSENSE_RPC_INTR_SOURCE_ARG,intr_source,LS_INTR_SOURCE_TABL,LS_INTR_SOURCE_UNKNOWN);
	return 0;
}
int SensorCtrlRpc::process_get_intr_source(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->get_intr_source(pPacket->intr_source);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_INTR_SOURCE_SET
int SensorCtrlRpc::json_to_bin_set_intr_source(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
        PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_WRITE,EJSON_LIGHTSENSE_INTR_SOURCE_SET);
	JSON_STRING_TO_ENUM(LIGHTSENSE_RPC_INTR_SOURCE_ARG,LS_INTR_SOURCE_TABL,LS_INTR_SOURCE,LS_INTR_SOURCE_UNKNOWN,pPanelCmdObj->intr_source);
	return 0;
}
int SensorCtrlRpc::bin_to_json_set_intr_source(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET);
	return 0;
}
int SensorCtrlRpc::process_set_intr_source(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->set_intr_source(pPacket->intr_source);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_GAIN_MODE_GET
int SensorCtrlRpc::json_to_bin_get_gain_mode(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_READ,EJSON_LIGHTSENSE_GAIN_MODE_GET);
	return 0;
}
int SensorCtrlRpc::bin_to_json_get_gain_mode(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,LIGHTSENSE_RPC_GAIN_MODE_ARG,gain_mode,LS_GAIN_MODE_TABL,LS_GAIN_MODE_UNKNOWN);
	return 0;
}
int SensorCtrlRpc::process_get_gain_mode(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->get_gain_mode(pPacket->gain_mode);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_GAIN_MODE_SET
int SensorCtrlRpc::json_to_bin_set_gain_mode(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
        PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_WRITE,EJSON_LIGHTSENSE_GAIN_MODE_SET);
	JSON_STRING_TO_ENUM(LIGHTSENSE_RPC_GAIN_MODE_ARG,LS_GAIN_MODE_TABL,LS_GAIN_MODE,LS_GAIN_MODE_UNKNOWN,pPanelCmdObj->gain_mode);
	return 0;
}
int SensorCtrlRpc::bin_to_json_set_gain_mode(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET);
	return 0;
}
int SensorCtrlRpc::process_set_gain_mode(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->set_gain_mode(pPacket->gain_mode);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_PRESCALER_GET
int SensorCtrlRpc::json_to_bin_get_prescaler(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_READ,EJSON_LIGHTSENSE_PRESCALER_GET);
	return 0;
}
int SensorCtrlRpc::bin_to_json_get_prescaler(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,LIGHTSENSE_RPC_PRESCALER_ARG,prescaler,LS_PRESCALER_TABL,LS_PRESCALER_UNKNOWN);
	return 0;
}
int SensorCtrlRpc::process_get_prescaler(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->get_prescaler(pPacket->prescaler);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_PRESCALER_SET
int SensorCtrlRpc::json_to_bin_set_prescaler(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
        PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_WRITE,EJSON_LIGHTSENSE_PRESCALER_SET);
	JSON_STRING_TO_ENUM(LIGHTSENSE_RPC_PRESCALER_ARG,LS_PRESCALER_TABL,LS_PRESCALER,LS_PRESCALER_UNKNOWN,pPanelCmdObj->prescaler);
	return 0;
}
int SensorCtrlRpc::bin_to_json_set_prescaler(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET);
	return 0;
}
int SensorCtrlRpc::process_set_prescaler(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->set_prescaler(pPacket->prescaler);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_RGBWCOUNT_GET
int SensorCtrlRpc::json_to_bin_get_rgbwcount(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_READ,EJSON_LIGHTSENSE_RGBWCOUNT_GET);
	return 0;
}
int SensorCtrlRpc::bin_to_json_get_rgbwcount(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	pPanelCmdObj=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->result!=RPC_SRV_RESULT_SUCCESS)
		prepare_result_string(pPanelReq->result,pReq);
	else
	{
		prepare_result_string(pPanelReq->result,pReq,(char*)LIGHTSENSE_RPC_RGBWCOUNT_ARGR,pPanelCmdObj->red,
				                             (char*)LIGHTSENSE_RPC_RGBWCOUNT_ARGG,pPanelCmdObj->green,
				                             (char*)LIGHTSENSE_RPC_RGBWCOUNT_ARGB,pPanelCmdObj->blue,
				                             (char*)LIGHTSENSE_RPC_RGBWCOUNT_ARGW,pPanelCmdObj->white);
				                             
	}
	OBJ_MEM_DELETE(pPanelCmdObj);
	return 0;
}
int SensorCtrlRpc::process_get_rgbwcount(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->get_rgbw_count(pPacket->red,pPacket->green,pPacket->blue,pPacket->white);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
int SensorCtrlRpc::json_to_bin_get_wavelength_count(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_READ,EJSON_LIGHTSENSE_WAVELENGTH_COUNT_GET);
	return 0;
}
int SensorCtrlRpc::bin_to_json_get_wavelength_count(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_INT(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,LIGHTSENSE_RPC_COUNT_ARG,sample_count);
	return 0;
}
int SensorCtrlRpc::process_get_wavelength_count(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->get_wavelength_count(pPacket->sample_count);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
int SensorCtrlRpc::json_to_bin_get_wavelength_item(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_READ,EJSON_LIGHTSENSE_WAVELENGTH_ITEM_GET);
	JSON_STRING_TO_INT(LIGHTSENSE_RPC_ITEM_INDX_ARG,pPanelCmdObj->sample_index);
	return 0;
}
int SensorCtrlRpc::bin_to_json_get_wavelength_item(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,LIGHTSENSE_RPC_ITEM_VAL_ARG,str_sample_value);
	return 0;
}
int SensorCtrlRpc::process_get_wavelength_item(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
	{
		pPacket->sample_value=0.0;
		pPanelReq->result=pSensr->get_wavelength_value(pPacket->sample_index,pPacket->sample_value);
		sprintf(pPacket->str_sample_value,"%.2lf",pPacket->sample_value);
	}
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
int SensorCtrlRpc::json_to_bin_get_spectrum_count(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_READ,EJSON_LIGHTSENSE_SPECTRUM_COUNT_GET);
	return 0;
}
int SensorCtrlRpc::bin_to_json_get_spectrum_count(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_INT(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,LIGHTSENSE_RPC_COUNT_ARG,sample_count);
	return 0;
}
int SensorCtrlRpc::process_get_spectrum_count(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
		pPanelReq->result=pSensr->get_spectrum_count(pPacket->sample_count);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
int SensorCtrlRpc::json_to_bin_get_spectrum_item(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_READ,EJSON_LIGHTSENSE_SPECTRUM_ITEM_GET);
	JSON_STRING_TO_INT(LIGHTSENSE_RPC_ITEM_INDX_ARG,pPanelCmdObj->sample_index);
	return 0;
}
int SensorCtrlRpc::bin_to_json_get_spectrum_item(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,LIGHTSENSE_RPC_ITEM_VAL_ARG,str_sample_value);
	return 0;
}
int SensorCtrlRpc::process_get_spectrum_item(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	LightSensor *pSensr=pData->pSensor;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pData->pSensor!=NULL)
	{
		pPacket->sample_value=0.0;
		pPanelReq->result=pSensr->get_spectrum_value(pPacket->sample_index,pPacket->sample_value);
		sprintf(pPacket->str_sample_value,"%.2lf",pPacket->sample_value);
	}
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cerrno>

//EJSON_LIGHTSENSE_FIRST_LEASED_IP_GET
int SensorCtrlRpc::json_to_bin_get_leased_ip(JsonDataCommObj* pReq)
{
	LIGHTSENSE_MEASUREMENT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,RPC_SRV_ACT_READ,EJSON_LIGHTSENSE_FIRST_LEASED_IP_GET);
	return 0;
}
int SensorCtrlRpc::bin_to_json_get_leased_ip(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,LIGHTSENSE_MEASUREMENT_PACKET,LIGHTSENSE_RPC_FIRST_LEASED_IP_ARG,leased_ip_addr);
	return 0;
}
int SensorCtrlRpc::process_get_leased_ip(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	LIGHTSENSE_MEASUREMENT_PACKET* pPacket;
	pPacket=(LIGHTSENSE_MEASUREMENT_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->action!=RPC_SRV_ACT_READ)
	{
		pPanelReq->result=RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
		return 0;
	}


	if(pData->LeaseFilePath.size()<=0)
	{
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
		return 0;
	}
	std::ifstream infile(pData->LeaseFilePath.c_str());
	std::string line;
	while (std::getline(infile, line))
	{
		//1517049089 28:d2:44:89:b3:4f 192.168.1.205 my-linux-machine *
		stringstream iss(line);
		std::string uid,mac,ip;
		iss >> uid;
		iss >> mac;
		iss >> ip;
		if(ip.size()>0)
		{
			strcpy(pPacket->leased_ip_addr,ip.c_str());
			pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
		}
		else
		{
			pPanelReq->result=RPC_SRV_RESULT_FAIL;
		}
	}
	return 0;
}
/* ------------------------------------------------------------------------- */



