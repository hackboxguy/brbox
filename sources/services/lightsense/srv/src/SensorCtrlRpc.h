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
};
/* ------------------------------------------------------------------------- */

#endif
