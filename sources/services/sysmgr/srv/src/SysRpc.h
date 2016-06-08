#ifndef __SYS_RPC_H_
#define __SYS_RPC_H_
#include "ADJsonRpcMgr.hpp"
#include "SysmgrJsonDef.h"
#include "ADCmnStringProcessor.hpp"
/* ------------------------------------------------------------------------- */
class  SysRpc : public ADJsonRpcMgrConsumer, public ADCmnStringProcessor
{
	bool crnt_fmwver_updated;
	bool bkup_fmwver_updated;
	bool krnl_fmwver_updated;
	bool boot_system_updated;
	SYSMGR_CMN_DATA_CACHE *pDataCache;
	SYSMGR_ASYNCTASK_TYPE get_async_task_in_progress();
public:
	 SysRpc(std::string rpcName,int myIndex,bool emu,bool log,SYSMGR_CMN_DATA_CACHE *pData);//getRpc,std::string setRpc);
	~ SysRpc();
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index);
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index);
	virtual int ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj);
	virtual RPC_SRV_RESULT ProcessWorkAsync(int index,unsigned char* pWorkData);
	virtual void ReceiveEvent(int cltToken,int evntNum,int evntArg,int evntArg2){};
	//virtual RPC_SRV_RESULT ProcessCommonRpc(int index,unsigned char* pWorkData){return RPC_SRV_RESULT_SUCCESS;};

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
	int process_set_devop(JsonDataCommObj* pReq,ADJsonRpcMgrProducer* pObj);
	RPC_SRV_RESULT process_async_set_devop(SYSMGR_DEV_OP_PACKET* pPacket);

	int json_to_bin_get_fmwver(JsonDataCommObj* pReq);
	int bin_to_json_get_fmwver(JsonDataCommObj* pReq);
	int process_get_fmwver(JsonDataCommObj* pReq);

	int json_to_bin_get_bootsys(JsonDataCommObj* pReq);
	int bin_to_json_get_bootsys(JsonDataCommObj* pReq);
	int process_get_bootsys(JsonDataCommObj* pReq);

	int json_to_bin_set_fmwupdate(JsonDataCommObj* pReq);
	int bin_to_json_set_fmwupdate(JsonDataCommObj* pReq);
	int process_set_fmwupdate(JsonDataCommObj* pReq,ADJsonRpcMgrProducer* pObj);
	RPC_SRV_RESULT process_async_set_fmwupdate(SYSMGR_FMWUPDATE_PACKET* pPacket);

	int json_to_bin_download_file(JsonDataCommObj* pReq,EJSON_SYSMGR_RPC_TYPES command);
	int bin_to_json_download_file(JsonDataCommObj* pReq,EJSON_SYSMGR_RPC_TYPES command);
	int process_download_file(JsonDataCommObj* pReq,EJSON_SYSMGR_RPC_TYPES command,ADJsonRpcMgrProducer* pObj);
	RPC_SRV_RESULT process_async_download_file(SYSMGR_DOWNLOAD_FILE_PACKET* pPacket,EJSON_SYSMGR_RPC_TYPES command);

	int json_to_bin_get_async_task_in_progress(JsonDataCommObj* pReq);
	int bin_to_json_get_async_task_in_progress(JsonDataCommObj* pReq);
	int process_get_async_task_in_progress(JsonDataCommObj* pReq);

	int json_to_bin_get_devtype(JsonDataCommObj* pReq);
	int bin_to_json_get_devtype(JsonDataCommObj* pReq);
	int process_get_devtype(JsonDataCommObj* pReq);

	int json_to_bin_get_hostname(JsonDataCommObj* pReq);
	int bin_to_json_get_hostname(JsonDataCommObj* pReq);
	int process_get_hostname(JsonDataCommObj* pReq);

	int json_to_bin_set_hostname(JsonDataCommObj* pReq);
	int bin_to_json_set_hostname(JsonDataCommObj* pReq);
	int process_set_hostname(JsonDataCommObj* pReq);

	int json_to_bin_get_myip(JsonDataCommObj* pReq);
	int bin_to_json_get_myip(JsonDataCommObj* pReq);
	int process_get_myip(JsonDataCommObj* pReq);

	int json_to_bin_set_def_hostname(JsonDataCommObj* pReq);
	int bin_to_json_set_def_hostname(JsonDataCommObj* pReq);
	int process_set_def_hostname(JsonDataCommObj* pReq);

	int json_to_bin_loglist_update(JsonDataCommObj* pReq);
	int bin_to_json_loglist_update(JsonDataCommObj* pReq);
	int process_loglist_update(JsonDataCommObj* pReq,ADJsonRpcMgrProducer* pObj);
	RPC_SRV_RESULT process_async_loglist_update(SYSMGR_LOG_PACKET* pPacket);

	int json_to_bin_get_logcount(JsonDataCommObj* pReq);
	int bin_to_json_get_logcount(JsonDataCommObj* pReq);
	int process_get_logcount(JsonDataCommObj* pReq);

	int json_to_bin_get_logline(JsonDataCommObj* pReq);
	int bin_to_json_get_logline(JsonDataCommObj* pReq);
	int process_get_logline(JsonDataCommObj* pReq);

};
/* ------------------------------------------------------------------------- */

#endif
