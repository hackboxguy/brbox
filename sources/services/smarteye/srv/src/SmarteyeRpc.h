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
	virtual int ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj);
	virtual RPC_SRV_RESULT ProcessWorkAsync(int index,unsigned char* pWorkData);
	virtual void ReceiveEvent(int cltToken,int evntNum,int evntArg,int evntArg2){};

	int json_to_bin_check_id_pattern(JsonDataCommObj* pReq);
	int bin_to_json_check_id_pattern(JsonDataCommObj* pReq);
	int process_check_id_pattern(JsonDataCommObj* pReq);

	int json_to_bin_get_debug_outfile(JsonDataCommObj* pReq);
	int bin_to_json_get_debug_outfile(JsonDataCommObj* pReq);
	int process_get_debug_outfile(JsonDataCommObj* pReq);

	int json_to_bin_set_debug_outfile(JsonDataCommObj* pReq);
	int bin_to_json_set_debug_outfile(JsonDataCommObj* pReq);
	int process_set_debug_outfile(JsonDataCommObj* pReq);

	int json_to_bin_capture_jpg_img(JsonDataCommObj* pReq);
	int bin_to_json_capture_jpg_img(JsonDataCommObj* pReq);
	int process_capture_jpg_img(JsonDataCommObj* pReq);

	int json_to_bin_set_capture_resolution(JsonDataCommObj* pReq);
	int bin_to_json_set_capture_resolution(JsonDataCommObj* pReq);
	int process_set_capture_resolution(JsonDataCommObj* pReq);

	int json_to_bin_get_checkwall_file(JsonDataCommObj* pReq);
	int bin_to_json_get_checkwall_file(JsonDataCommObj* pReq);
	int process_get_checkwall_file(JsonDataCommObj* pReq);
	int json_to_bin_set_checkwall_file(JsonDataCommObj* pReq);
	int bin_to_json_set_checkwall_file(JsonDataCommObj* pReq);
	int process_set_checkwall_file(JsonDataCommObj* pReq);

	int json_to_bin_get_checkwallbase_file(JsonDataCommObj* pReq);
	int bin_to_json_get_checkwallbase_file(JsonDataCommObj* pReq);
	int process_get_checkwallbase_file(JsonDataCommObj* pReq);
	int json_to_bin_set_checkwallbase_file(JsonDataCommObj* pReq);
	int bin_to_json_set_checkwallbase_file(JsonDataCommObj* pReq);
	int process_set_checkwallbase_file(JsonDataCommObj* pReq);

	int json_to_bin_scan_qrstring(JsonDataCommObj* pReq);
	int bin_to_json_scan_qrstring(JsonDataCommObj* pReq);
	int process_scan_qrstring(JsonDataCommObj* pReq);

	int json_to_bin_compare_img(JsonDataCommObj* pReq);
	int bin_to_json_compare_img(JsonDataCommObj* pReq);
	int process_compare_img(JsonDataCommObj* pReq);

	int json_to_bin_auto_exposure_get(JsonDataCommObj* pReq);
	int bin_to_json_auto_exposure_get(JsonDataCommObj* pReq);
	int process_auto_exposure_get(JsonDataCommObj* pReq);
	int json_to_bin_auto_exposure_set(JsonDataCommObj* pReq);
	int bin_to_json_auto_exposure_set(JsonDataCommObj* pReq);
	int process_auto_exposure_set(JsonDataCommObj* pReq);
	RPC_SRV_RESULT get_auto_exposure(SMARTEYE_AUTO_EXPOSURE_TYPE &exposure);
	RPC_SRV_RESULT set_auto_exposure(SMARTEYE_AUTO_EXPOSURE_TYPE exposure);

	int json_to_bin_get_exposure(JsonDataCommObj* pReq);
	int bin_to_json_get_exposure(JsonDataCommObj* pReq);
	int process_get_exposure(JsonDataCommObj* pReq);
	int json_to_bin_set_exposure(JsonDataCommObj* pReq);
	int bin_to_json_set_exposure(JsonDataCommObj* pReq);
	int process_set_exposure(JsonDataCommObj* pReq);
	RPC_SRV_RESULT get_exposure(int* exposure);
	RPC_SRV_RESULT set_exposure(int exposure);

	int json_to_bin_auto_focus_get(JsonDataCommObj* pReq);
	int bin_to_json_auto_focus_get(JsonDataCommObj* pReq);
	int process_auto_focus_get(JsonDataCommObj* pReq);
	int json_to_bin_auto_focus_set(JsonDataCommObj* pReq);
	int bin_to_json_auto_focus_set(JsonDataCommObj* pReq);
	int process_auto_focus_set(JsonDataCommObj* pReq);

	int json_to_bin_get_focus(JsonDataCommObj* pReq);
	int bin_to_json_get_focus(JsonDataCommObj* pReq);
	int process_get_focus(JsonDataCommObj* pReq);
	int json_to_bin_set_focus(JsonDataCommObj* pReq);
	int bin_to_json_set_focus(JsonDataCommObj* pReq);
	int process_set_focus(JsonDataCommObj* pReq);
	RPC_SRV_RESULT get_focus(int* focus);
	RPC_SRV_RESULT set_focus(int focus);
};
/* ------------------------------------------------------------------------- */

#endif
