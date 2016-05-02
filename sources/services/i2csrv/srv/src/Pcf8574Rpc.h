#ifndef __PCF8574_RPC_H_
#define __PCF8574_RPC_H_
#include "ADJsonRpcMgr.hpp"
#include "I2CSrvJsonDef.h"
#include "ADCmnStringProcessor.hpp"
/* ------------------------------------------------------------------------- */
class  Pcf8574Rpc : public ADJsonRpcMgrConsumer, public ADCmnStringProcessor
{
	I2CSRV_CMN_DATA_CACHE *pDataCache;
public:
	 Pcf8574Rpc(std::string rpcName,int myIndex,bool emu,bool log,I2CSRV_CMN_DATA_CACHE *pData);
	~ Pcf8574Rpc();
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index);
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index);
	virtual int ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj);
	virtual RPC_SRV_RESULT ProcessWorkAsync(int index,unsigned char* pWorkData);
	virtual void ReceiveEvent(int cltToken,int evntNum,int evntArg){};

	int json_to_bin_pcf_get(JsonDataCommObj* pReq);
	int bin_to_json_pcf_get(JsonDataCommObj* pReq);
	int process_pcf_get(JsonDataCommObj* pReq);

	int json_to_bin_pcf_set(JsonDataCommObj* pReq);
	int bin_to_json_pcf_set(JsonDataCommObj* pReq);
	int process_pcf_set(JsonDataCommObj* pReq);
};
/* ------------------------------------------------------------------------- */

#endif
