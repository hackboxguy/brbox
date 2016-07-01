#ifndef __CMN_RPC_H_
#define __CMN_RPC_H_
#include "ADJsonRpcMgr.hpp"
#include "I2CSrvJsonDef.h"
#include "ADCmnStringProcessor.hpp"
/* ------------------------------------------------------------------------- */
class  CmnRpc : public ADJsonRpcMgrConsumer, public ADCmnStringProcessor
{
	I2CSRV_CMN_DATA_CACHE *pDataCache;
	int process_get_mw_byte(JsonDataCommObj* pReq);
	int process_set_mw_byte(JsonDataCommObj* pReq);
	int process_get_mw_word(JsonDataCommObj* pReq);
	int process_set_mw_word(JsonDataCommObj* pReq);
	int process_get_mw_dword(JsonDataCommObj* pReq);
	int process_set_mw_dword(JsonDataCommObj* pReq);
public:
	 CmnRpc(std::string rpcName,int myIndex,bool emu,bool log,I2CSRV_CMN_DATA_CACHE *pData);//getRpc,std::string setRpc);
	~ CmnRpc();
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index){return 0;};
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index){return 0;};
	virtual int ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj);
	virtual RPC_SRV_RESULT ProcessWorkAsync(int index,unsigned char* pWorkData);
	virtual void ReceiveEvent(int cltToken,int evntNum,int evntArg,int evntArg2){};
	//virtual RPC_SRV_RESULT ProcessCommonRpc(int index,unsigned char* pWorkData){return RPC_SRV_RESULT_SUCCESS;};
};
/* ------------------------------------------------------------------------- */

#endif
