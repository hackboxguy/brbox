#ifndef __GPIOCTRL_RPC_H_
#define __GPIOCTRL_RPC_H_
#include "ADJsonRpcMgr.hpp"
#include "GpioctlJsonDef.h"
#include "ADCmnStringProcessor.hpp"
/* ------------------------------------------------------------------------- */
class  GpioCtrlRpc : public ADJsonRpcMgrConsumer, public ADCmnStringProcessor
{
	GPIOCTL_CMN_DATA_CACHE *pDataCache;
public:
	 GpioCtrlRpc(std::string rpcName,int myIndex,bool emu,bool log,GPIOCTL_CMN_DATA_CACHE *pData);
	~ GpioCtrlRpc();
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index);
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index);
	virtual int ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj);
	virtual RPC_SRV_RESULT ProcessWorkAsync(int index,unsigned char* pWorkData);
	virtual void ReceiveEvent(int cltToken,int evntNum,int evntArg,int evntArg2){};
	virtual RPC_SRV_RESULT ProcessCommonRpc(int index,unsigned char* pWorkData){return RPC_SRV_RESULT_SUCCESS;};

	int json_to_bin_gpio_get(JsonDataCommObj* pReq);
	int bin_to_json_gpio_get(JsonDataCommObj* pReq);
	int process_gpio_get(JsonDataCommObj* pReq);

	int json_to_bin_gpio_set(JsonDataCommObj* pReq);
	int bin_to_json_gpio_set(JsonDataCommObj* pReq);
	int process_gpio_set(JsonDataCommObj* pReq);
};
/* ------------------------------------------------------------------------- */

#endif
