#ifndef __DISPCTRL_RPC_H_
#define __DISPCTRL_RPC_H_
#include "ADJsonRpcMgr.hpp"
#include "SysmgrJsonDef.h"
#include "ADCmnStringProcessor.hpp"
/* ------------------------------------------------------------------------- */
class  DispCtrlRpc : public ADJsonRpcMgrConsumer, public ADCmnStringProcessor
{
	SYSMGR_CMN_DATA_CACHE *pDataCache;
public:
	 DispCtrlRpc(std::string rpcName,int myIndex,bool emu,bool log,SYSMGR_CMN_DATA_CACHE *pData);//getRpc,std::string setRpc);
	~ DispCtrlRpc();
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index);
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index);
	virtual int ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj);
	virtual RPC_SRV_RESULT ProcessWorkAsync(int index,unsigned char* pWorkData);
	virtual void ReceiveEvent(int cltToken,int evntNum,int evntArg,int evntArg2){};
	//virtual RPC_SRV_RESULT ProcessCommonRpc(int index,unsigned char* pWorkData){return RPC_SRV_RESULT_SUCCESS;};

	int json_to_bin_disp_clear(JsonDataCommObj* pReq);
	int bin_to_json_disp_clear(JsonDataCommObj* pReq);
	int process_disp_clear(JsonDataCommObj* pReq,SYSMGR_CMN_DATA_CACHE *pData);

	int json_to_bin_disp_print(JsonDataCommObj* pReq);
	int bin_to_json_disp_print(JsonDataCommObj* pReq);
	int process_disp_print(JsonDataCommObj* pReq,SYSMGR_CMN_DATA_CACHE *pData);

	int json_to_bin_backlight_get(JsonDataCommObj* pReq);
	int bin_to_json_backlight_get(JsonDataCommObj* pReq);
	int process_backlight_get(JsonDataCommObj* pReq,SYSMGR_CMN_DATA_CACHE *pData);
	int json_to_bin_backlight_set(JsonDataCommObj* pReq);
	int bin_to_json_backlight_set(JsonDataCommObj* pReq);
	int process_backlight_set(JsonDataCommObj* pReq,SYSMGR_CMN_DATA_CACHE *pData);

};
/* ------------------------------------------------------------------------- */

#endif
