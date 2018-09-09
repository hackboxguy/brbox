#ifndef __MODBUS_RPC_H_
#define __MODBUS_RPC_H_
#include "ADJsonRpcMgr.hpp"
#include "ModBusSrvJsonDef.h"
#include "ADCmnStringProcessor.hpp"
/* ------------------------------------------------------------------------- */
class  ModbusRpc : public ADJsonRpcMgrConsumer, public ADCmnStringProcessor
{
	MODBUSSRV_CMN_DATA_CACHE *pDataCache;
public:
	 ModbusRpc(std::string rpcName,int myIndex,bool emu,bool log,MODBUSSRV_CMN_DATA_CACHE *pData);
	~ ModbusRpc();
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index);
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index);
	virtual int ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj);
	virtual RPC_SRV_RESULT ProcessWorkAsync(int index,unsigned char* pWorkData);
	virtual void ReceiveEvent(int cltToken,int evntNum,int evntArg,int evntArg2){};
	virtual RPC_SRV_RESULT ProcessCommonRpc(int index,unsigned char* pWorkData){return RPC_SRV_RESULT_SUCCESS;};

	int json_to_bin_voltage_get(JsonDataCommObj* pReq);
	int bin_to_json_voltage_get(JsonDataCommObj* pReq);
	int process_voltage_get(JsonDataCommObj* pReq);

	//int json_to_bin_pcf_set(JsonDataCommObj* pReq);
	//int bin_to_json_pcf_set(JsonDataCommObj* pReq);
	//int process_pcf_set(JsonDataCommObj* pReq);
};
/* ------------------------------------------------------------------------- */

#endif
