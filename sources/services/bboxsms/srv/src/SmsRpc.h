#ifndef __SMS_RPC_H_
#define __SMS_RPC_H_
#include "ADJsonRpcMgr.hpp"
#include "BboxsmsJsonDef.h"
#include "ADCmnStringProcessor.hpp"
/* ------------------------------------------------------------------------- */
class  SmsRpc : public ADJsonRpcMgrConsumer , public ADCmnStringProcessor
{
	BBOXSMS_CMN_DATA_CACHE *pDataCache;
public:
	 SmsRpc(std::string rpcName,int myIndex,bool emu,bool log,BBOXSMS_CMN_DATA_CACHE *pData);
	~ SmsRpc();
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index);
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index);
	virtual int ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj);
	virtual RPC_SRV_RESULT ProcessWorkAsync(int index,unsigned char* pWorkData);
	virtual void ReceiveEvent(int cltToken,int evntNum,int evntArg){};

	int json_to_bin_delete_all(JsonDataCommObj* pReq);
	int bin_to_json_delete_all(JsonDataCommObj* pReq);
	int process_delete_all(JsonDataCommObj* pReq);

	int json_to_bin_get_sms(JsonDataCommObj* pReq);
	int bin_to_json_get_sms(JsonDataCommObj* pReq);
	int process_get_sms(JsonDataCommObj* pReq);
};
/* ------------------------------------------------------------------------- */

#endif
