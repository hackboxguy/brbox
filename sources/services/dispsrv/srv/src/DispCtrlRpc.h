#ifndef __DISPCTRL_RPC_H_
#define __DISPCTRL_RPC_H_
#include "ADJsonRpcMgr.hpp"
#include "DispsrvJsonDef.h"
#include "ADCmnStringProcessor.hpp"
/* ------------------------------------------------------------------------- */
class  DispCtrlRpc : public ADJsonRpcMgrConsumer, public ADCmnStringProcessor
{
	DISPSRV_CMN_DATA_CACHE *pDataCache;
public:
	 DispCtrlRpc(std::string rpcName,int myIndex,bool emu,bool log,DISPSRV_CMN_DATA_CACHE *pData);
	~ DispCtrlRpc();
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index);
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index);
	virtual int ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj);
	virtual RPC_SRV_RESULT ProcessWorkAsync(int index,unsigned char* pWorkData);
	virtual void ReceiveEvent(int cltToken,int evntNum,int evntArg,int evntArg2){};
	virtual RPC_SRV_RESULT ProcessCommonRpc(int index,unsigned char* pWorkData){return RPC_SRV_RESULT_SUCCESS;};

	int json_to_bin_disp_init(JsonDataCommObj* pReq);
	int bin_to_json_disp_init(JsonDataCommObj* pReq);
	int process_disp_init(JsonDataCommObj* pReq,DISPSRV_CMN_DATA_CACHE *pData);

	int json_to_bin_clear_display(JsonDataCommObj* pReq);
	int bin_to_json_clear_display(JsonDataCommObj* pReq);
	int process_clear_display(JsonDataCommObj* pReq,DISPSRV_CMN_DATA_CACHE *pData);

	int json_to_bin_print_display(JsonDataCommObj* pReq);
	int bin_to_json_print_display(JsonDataCommObj* pReq);
	int process_print_display(JsonDataCommObj* pReq,DISPSRV_CMN_DATA_CACHE *pData);

	int json_to_bin_backlight_get(JsonDataCommObj* pReq);
	int bin_to_json_backlight_get(JsonDataCommObj* pReq);
	int process_backlight_get(JsonDataCommObj* pReq,DISPSRV_CMN_DATA_CACHE *pData);
	int json_to_bin_backlight_set(JsonDataCommObj* pReq);
	int bin_to_json_backlight_set(JsonDataCommObj* pReq);
	int process_backlight_set(JsonDataCommObj* pReq,DISPSRV_CMN_DATA_CACHE *pData);

};
/* ------------------------------------------------------------------------- */

#endif
