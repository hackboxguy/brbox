#ifndef __SYS_RPC_H_
#define __SYS_RPC_H_
#include "ADJsonRpcMgr.hpp"
#include "SysmgrJsonDef.h"
#include "ADCmnStringProcessor.hpp"
/* ------------------------------------------------------------------------- */
class  SysRpc : public ADJsonRpcMgrConsumer, public ADCmnStringProcessor
{
	SYSMGR_CMN_DATA_CACHE *pDataCache;
public:
	 SysRpc(std::string rpcName,int myIndex,bool emu,bool log,SYSMGR_CMN_DATA_CACHE *pData);//getRpc,std::string setRpc);
	~ SysRpc();
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index);
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index);
	virtual int ProcessWork(JsonDataCommObj* pReq,int index);
	virtual int ProcessWorkAsync(unsigned char* pWorkData);

	int json_to_bin_get_loadinfo(JsonDataCommObj* pReq);
	int bin_to_json_get_loadinfo(JsonDataCommObj* pReq);
	int process_get_loadinfo(JsonDataCommObj* pReq);

	int json_to_bin_get_meminfo(JsonDataCommObj* pReq);
	int bin_to_json_get_meminfo(JsonDataCommObj* pReq);
	int process_get_meminfo(JsonDataCommObj* pReq);

	int json_to_bin_get_cpuinfo(JsonDataCommObj* pReq);
	int bin_to_json_get_cpuinfo(JsonDataCommObj* pReq);
	int process_get_cpuinfo(JsonDataCommObj* pReq);

	//get/set device operation
	int json_to_bin_get_devop(JsonDataCommObj* pReq);
	int bin_to_json_get_devop(JsonDataCommObj* pReq);
	int process_get_devop(JsonDataCommObj* pReq);
	int json_to_bin_set_devop(JsonDataCommObj* pReq);
	int bin_to_json_set_devop(JsonDataCommObj* pReq);
	int process_set_devop(JsonDataCommObj* pReq);

};
/* ------------------------------------------------------------------------- */

#endif
