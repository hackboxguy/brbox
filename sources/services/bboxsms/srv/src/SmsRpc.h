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
	BBOXSMS_ASYNCTASK_TYPE get_async_task_in_progress();

	int json_to_bin_get_async_task_in_progress(JsonDataCommObj* pReq);
	int bin_to_json_get_async_task_in_progress(JsonDataCommObj* pReq);
	int process_get_async_task_in_progress(JsonDataCommObj* pReq);

	int json_to_bin_delete_all(JsonDataCommObj* pReq);
	int bin_to_json_delete_all(JsonDataCommObj* pReq);
	int process_delete_all(JsonDataCommObj* pReq,ADJsonRpcMgrProducer* pObj);
	RPC_SRV_RESULT process_async_delete_all(BBOXSMS_SMS_PACKET* pPacket);

	int json_to_bin_get_sms(JsonDataCommObj* pReq);
	int bin_to_json_get_sms(JsonDataCommObj* pReq);
	int process_get_sms(JsonDataCommObj* pReq);

	int json_to_bin_get_total_sms(JsonDataCommObj* pReq);
	int bin_to_json_get_total_sms(JsonDataCommObj* pReq);
	int process_get_total_sms(JsonDataCommObj* pReq);

	int json_to_bin_sms_list_update(JsonDataCommObj* pReq);
	int bin_to_json_sms_list_update(JsonDataCommObj* pReq);
	int process_sms_list_update(JsonDataCommObj* pReq,ADJsonRpcMgrProducer* pObj);
	RPC_SRV_RESULT process_async_list_update(BBOXSMS_SMS_PACKET* pPacket);

	int json_to_bin_ident_device(JsonDataCommObj* pReq);
	int bin_to_json_ident_device(JsonDataCommObj* pReq);
	int process_ident_device(JsonDataCommObj* pReq,ADJsonRpcMgrProducer* pObj);
	RPC_SRV_RESULT process_async_ident_device(BBOXSMS_SMS_PACKET* pPacket);
};
/* ------------------------------------------------------------------------- */

#endif
