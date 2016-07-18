#ifndef __SENSORCTRL_RPC_H_
#define __SENSORCTRL_RPC_H_
#include "ADJsonRpcMgr.hpp"
#include "LightsenseJsonDef.h"
#include "ADCmnStringProcessor.hpp"
/* ------------------------------------------------------------------------- */
class  SensorCtrlRpc : public ADJsonRpcMgrConsumer, public ADCmnStringProcessor
{
	LIGHTSENSE_CMN_DATA_CACHE *pDataCache;
public:
	 SensorCtrlRpc(std::string rpcName,int myIndex,bool emu,bool log,LIGHTSENSE_CMN_DATA_CACHE *pData);
	~ SensorCtrlRpc();
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index);
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index);
	virtual int ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj);
	virtual RPC_SRV_RESULT ProcessWorkAsync(int index,unsigned char* pWorkData);
	virtual void ReceiveEvent(int cltToken,int evntNum,int evntArg,int evntArg2){};
	virtual RPC_SRV_RESULT ProcessCommonRpc(int index,unsigned char* pWorkData){return RPC_SRV_RESULT_SUCCESS;};

	int json_to_bin_sensor_init(JsonDataCommObj* pReq);
	int bin_to_json_sensor_init(JsonDataCommObj* pReq);
	int process_sensor_init(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData);

	int json_to_bin_read_xyz(JsonDataCommObj* pReq);
	int bin_to_json_read_xyz(JsonDataCommObj* pReq);
	int process_read_xyz(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData);

	//EJSON_LIGHTSENSE_GET_INTEGRATION_TIME
	int json_to_bin_get_integration_time(JsonDataCommObj* pReq);
	int bin_to_json_get_integration_time(JsonDataCommObj* pReq);
	int process_get_integration_time(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData);
	//EJSON_LIGHTSENSE_SET_INTEGRATION_TIME
	int json_to_bin_set_integration_time(JsonDataCommObj* pReq);
	int bin_to_json_set_integration_time(JsonDataCommObj* pReq);
	int process_set_integration_time(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData);
	//EJSON_LIGHTSENSE_SYNC_EDGE_GET
	int json_to_bin_get_sync_edge(JsonDataCommObj* pReq);
	int bin_to_json_get_sync_edge(JsonDataCommObj* pReq);
	int process_get_sync_edge(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData);
	//EJSON_LIGHTSENSE_SYNC_EDGE_SET
	int json_to_bin_set_sync_edge(JsonDataCommObj* pReq);
	int bin_to_json_set_sync_edge(JsonDataCommObj* pReq);
	int process_set_sync_edge(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData);
	//EJSON_LIGHTSENSE_INTEGRATION_MODE_GET
	int json_to_bin_get_integration_mode(JsonDataCommObj* pReq);
	int bin_to_json_get_integration_mode(JsonDataCommObj* pReq);
	int process_get_integration_mode(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData);
	//EJSON_LIGHTSENSE_INTEGRATION_MODE_SET
	int json_to_bin_set_integration_mode(JsonDataCommObj* pReq);
	int bin_to_json_set_integration_mode(JsonDataCommObj* pReq);
	int process_set_integration_mode(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData);
	//EJSON_LIGHTSENSE_INTR_STOP_STS_GET
	int json_to_bin_get_intrstop_sts(JsonDataCommObj* pReq);
	int bin_to_json_get_intrstop_sts(JsonDataCommObj* pReq);
	int process_get_intrstop_sts(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData);
	//EJSON_LIGHTSENSE_INTR_STOP_STS_SET
	int json_to_bin_set_intrstop_sts(JsonDataCommObj* pReq);
	int bin_to_json_set_intrstop_sts(JsonDataCommObj* pReq);
	int process_set_intrstop_sts(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData);
	//EJSON_LIGHTSENSE_INTR_MODE_GET
	int json_to_bin_get_intr_mode(JsonDataCommObj* pReq);
	int bin_to_json_get_intr_mode(JsonDataCommObj* pReq);
	int process_get_intr_mode(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData);
	//EJSON_LIGHTSENSE_INTR_MODE_SET
	int json_to_bin_set_intr_mode(JsonDataCommObj* pReq);
	int bin_to_json_set_intr_mode(JsonDataCommObj* pReq);
	int process_set_intr_mode(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData);
	//EJSON_LIGHTSENSE_INTR_RATE_GET
	int json_to_bin_get_intr_rate(JsonDataCommObj* pReq);
	int bin_to_json_get_intr_rate(JsonDataCommObj* pReq);
	int process_get_intr_rate(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData);
	//EJSON_LIGHTSENSE_INTR_RATE_SET
	int json_to_bin_set_intr_rate(JsonDataCommObj* pReq);
	int bin_to_json_set_intr_rate(JsonDataCommObj* pReq);
	int process_set_intr_rate(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData);
	//EJSON_LIGHTSENSE_INTR_SOURCE_GET
	int json_to_bin_get_intr_source(JsonDataCommObj* pReq);
	int bin_to_json_get_intr_source(JsonDataCommObj* pReq);
	int process_get_intr_source(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData);
	//EJSON_LIGHTSENSE_INTR_SOURCE_SET
	int json_to_bin_set_intr_source(JsonDataCommObj* pReq);
	int bin_to_json_set_intr_source(JsonDataCommObj* pReq);
	int process_set_intr_source(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData);
	//EJSON_LIGHTSENSE_GAIN_MODE_GET
	int json_to_bin_get_gain_mode(JsonDataCommObj* pReq);
	int bin_to_json_get_gain_mode(JsonDataCommObj* pReq);
	int process_get_gain_mode(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData);
	//EJSON_LIGHTSENSE_GAIN_MODE_SET
	int json_to_bin_set_gain_mode(JsonDataCommObj* pReq);
	int bin_to_json_set_gain_mode(JsonDataCommObj* pReq);
	int process_set_gain_mode(JsonDataCommObj* pReq,LIGHTSENSE_CMN_DATA_CACHE *pData);

};
/* ------------------------------------------------------------------------- */

#endif
