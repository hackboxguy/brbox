#ifndef __EVENT_HANDLER_H_
#define __EVENT_HANDLER_H_
#include "ADJsonRpcMgr.hpp"
#include "SysmgrJsonDef.h"
#include "ADCmnStringProcessor.hpp"
/* ------------------------------------------------------------------------- */
class  EventHandler : public ADJsonRpcMgrConsumer, public ADCmnStringProcessor
{
	int srvToken;//=-1;
	SYSMGR_CMN_DATA_CACHE *pDataCache;
public:
	 EventHandler(std::string rpcName,int myIndex,bool emu,bool log,SYSMGR_CMN_DATA_CACHE *pData);
	~ EventHandler();
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index){};
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index){};
	virtual int ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj){};
	virtual RPC_SRV_RESULT ProcessWorkAsync(int index,unsigned char* pWorkData){};
	virtual void ReceiveEvent(int cltToken,int evntNum,int evntArg);
};
/* ------------------------------------------------------------------------- */

#endif
