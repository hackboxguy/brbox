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
	virtual int ProcessWork(JsonDataCommObj* pReq,int index);
	virtual int ProcessWorkAsync(unsigned char* pWorkData);

	//int json_to_bin_check_id_pattern(JsonDataCommObj* pReq);
	//int bin_to_json_check_id_pattern(JsonDataCommObj* pReq);
	//int process_check_id_pattern(JsonDataCommObj* pReq);

	//int json_to_bin_get_debug_outfile(JsonDataCommObj* pReq);
	//int bin_to_json_get_debug_outfile(JsonDataCommObj* pReq);
	//int process_get_debug_outfile(JsonDataCommObj* pReq);

	//int json_to_bin_set_debug_outfile(JsonDataCommObj* pReq);
	//int bin_to_json_set_debug_outfile(JsonDataCommObj* pReq);
	//int process_set_debug_outfile(JsonDataCommObj* pReq);
};
/* ------------------------------------------------------------------------- */

#endif
