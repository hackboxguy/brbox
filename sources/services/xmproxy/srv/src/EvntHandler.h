#ifndef __EVNT_HANDLER_H_
#define __EVNT_HANDLER_H_
#include "ADJsonRpcMgr.hpp"
#include "XmproxyJsonDef.h"
#include "ADCmnStringProcessor.hpp"
/* ------------------------------------------------------------------------- */
class  EvntHandler : public ADJsonRpcMgrConsumer, public ADCmnStringProcessor
{
	bool sysmgrEventActive;
	int  sysMgrSrvToken;//=-1;
	bool bboxSmsEventActive;
	int  bboxSmsSrvToken;
	XMPROXY_CMN_DATA_CACHE *pDataCache;
public:
	 EvntHandler(std::string rpcName,int myIndex,bool emu,bool log,XMPROXY_CMN_DATA_CACHE *pData);
	~ EvntHandler();
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index){};
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index){};
	virtual int ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj){};
	virtual RPC_SRV_RESULT ProcessWorkAsync(int index,unsigned char* pWorkData){};
	virtual void ReceiveEvent(int cltToken,int evntNum,int evntArg);
};
/* ------------------------------------------------------------------------- */

#endif
