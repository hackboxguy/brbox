#ifndef __API_RPC_H_
#define __API_RPC_H_
#include "ADJsonRpcMgr.hpp"
#include "ApisrvJsonDef.h"
#include "ADCmnStringProcessor.hpp"
/* ------------------------------------------------------------------------- */
class  ApiRpc : public ADJsonRpcMgrConsumer , public ADCmnStringProcessor
{
	APISRV_CMN_DATA_CACHE *pDataCache;
public:
	 ApiRpc(std::string rpcName,int myIndex,bool emu,bool log,APISRV_CMN_DATA_CACHE *pData);
	~ ApiRpc();
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index);
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index);
	virtual int ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj);
	virtual RPC_SRV_RESULT ProcessWorkAsync(int index,unsigned char* pWorkData);
	virtual void ReceiveEvent(int cltToken,int evntNum,int evntArg){};
	//XMPROXY_ASYNCTASK_TYPE get_async_task_in_progress();

	//int json_to_bin_get_async_task_in_progress(JsonDataCommObj* pReq);
	//int bin_to_json_get_async_task_in_progress(JsonDataCommObj* pReq);
	//int process_get_async_task_in_progress(JsonDataCommObj* pReq);
};
/* ------------------------------------------------------------------------- */

#endif
