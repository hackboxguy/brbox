#ifndef __SMARTEYE_RPC_H_
#define __SMARTEYE_RPC_H_
#include "ADJsonRpcMgr.hpp"
#include "SmarteyeJsonDef.h"
#include "ADCmnStringProcessor.hpp"
/* ------------------------------------------------------------------------- */
class  SmarteyeRpc : public ADJsonRpcMgrConsumer, public ADCmnStringProcessor
{
	SMARTEYE_CMN_DATA_CACHE *pDataCache;
public:
	 SmarteyeRpc(std::string rpcName,int myIndex,bool emu,bool log,SMARTEYE_CMN_DATA_CACHE *pData);
	~ SmarteyeRpc();
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index);
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index);
	virtual int ProcessWork(JsonDataCommObj* pReq,int index);
	virtual int ProcessWorkAsync(unsigned char* pWorkData);

	int json_to_bin_check_id_pattern(JsonDataCommObj* pReq);
	int bin_to_json_check_id_pattern(JsonDataCommObj* pReq);
	int process_check_id_pattern(JsonDataCommObj* pReq);

	int json_to_bin_get_debug_outfile(JsonDataCommObj* pReq);
	int bin_to_json_get_debug_outfile(JsonDataCommObj* pReq);
	int process_get_debug_outfile(JsonDataCommObj* pReq);

	int json_to_bin_set_debug_outfile(JsonDataCommObj* pReq);
	int bin_to_json_set_debug_outfile(JsonDataCommObj* pReq);
	int process_set_debug_outfile(JsonDataCommObj* pReq);

};
/* ------------------------------------------------------------------------- */

#endif
