#ifndef __GPIOCTRL_RPC_H_
#define __GPIOCTRL_RPC_H_
#include "ADJsonRpcMgr.hpp"
#include "HotwireJsonDef.h"
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
	//virtual RPC_SRV_RESULT ProcessCommonRpc(int index,unsigned char* pWorkData){return RPC_SRV_RESULT_SUCCESS;};

	int json_to_bin_gpio_get(JsonDataCommObj* pReq);
	int bin_to_json_gpio_get(JsonDataCommObj* pReq);
	int process_gpio_get(JsonDataCommObj* pReq);

	int json_to_bin_gpio_set(JsonDataCommObj* pReq);
	int bin_to_json_gpio_set(JsonDataCommObj* pReq);
	int process_gpio_set(JsonDataCommObj* pReq);


	int json_to_bin_omxact_get(JsonDataCommObj* pReq);
	int bin_to_json_omxact_get(JsonDataCommObj* pReq);
	int process_omxact_get(JsonDataCommObj* pReq);
	int json_to_bin_omxact_set(JsonDataCommObj* pReq);
	int bin_to_json_omxact_set(JsonDataCommObj* pReq);
	int process_omxact_set(JsonDataCommObj* pReq,ADJsonRpcMgrProducer* pObj);
	RPC_SRV_RESULT process_omx_action(GPIOCTL_OMXACT_TYPE act);
	bool is_omx_running();

	int json_to_bin_showfbimg_get(JsonDataCommObj* pReq);
	int bin_to_json_showfbimg_get(JsonDataCommObj* pReq);
	int process_showfbimg_get(JsonDataCommObj* pReq);//,ADJsonRpcMgrProducer* pObj);

	int json_to_bin_showfbimg_set(JsonDataCommObj* pReq);
	int bin_to_json_showfbimg_set(JsonDataCommObj* pReq);
	int process_showfbimg_set(JsonDataCommObj* pReq);//,ADJsonRpcMgrProducer* pObj);
	RPC_SRV_RESULT process_show_image(std::string imgfile);
	bool is_screen_image_active();

	int json_to_bin_qrcodeimg_set(JsonDataCommObj* pReq);
	int bin_to_json_qrcodeimg_set(JsonDataCommObj* pReq);
	int process_qrcodeimg_set(JsonDataCommObj* pReq);
	RPC_SRV_RESULT create_qrcode_image(char* qrfilepath,char* qrstring);
	RPC_SRV_RESULT auto_scale_qr_size(int qrstrlen,int* qr_size);

	int json_to_bin_pattern_get(JsonDataCommObj* pReq);
	int bin_to_json_pattern_get(JsonDataCommObj* pReq);
	int process_pattern_get(JsonDataCommObj* pReq);

	int json_to_bin_pattern_set(JsonDataCommObj* pReq);
	int bin_to_json_pattern_set(JsonDataCommObj* pReq);
	int process_pattern_set(JsonDataCommObj* pReq);
	RPC_SRV_RESULT process_show_pattern(MPLAYSRV_PATTERN_TYPE pat);

	int json_to_bin_mediafile_type_get(JsonDataCommObj* pReq);
	int bin_to_json_mediafile_type_get(JsonDataCommObj* pReq);
	int process_mediafile_type_get(JsonDataCommObj* pReq);
	int json_to_bin_mediafile_type_set(JsonDataCommObj* pReq);
	int bin_to_json_mediafile_type_set(JsonDataCommObj* pReq);
	int process_mediafile_type_set(JsonDataCommObj* pReq);

	int json_to_bin_mediafile_get(JsonDataCommObj* pReq);
	int bin_to_json_mediafile_get(JsonDataCommObj* pReq);
	int process_mediafile_get(JsonDataCommObj* pReq);
	int json_to_bin_mediafile_set(JsonDataCommObj* pReq);
	int bin_to_json_mediafile_set(JsonDataCommObj* pReq);
	int process_mediafile_set(JsonDataCommObj* pReq);

	int json_to_bin_media_action_set(JsonDataCommObj* pReq);
	int bin_to_json_media_action_set(JsonDataCommObj* pReq);
	int process_media_action_set(JsonDataCommObj* pReq);
	RPC_SRV_RESULT process_media_action(MPLAYSRV_MEDIA_ACTION act);

	int json_to_bin_graphics_out_get(JsonDataCommObj* pReq);
	int bin_to_json_graphics_out_get(JsonDataCommObj* pReq);
	int process_graphics_out_get(JsonDataCommObj* pReq);
	int json_to_bin_graphics_out_set(JsonDataCommObj* pReq);
	int bin_to_json_graphics_out_set(JsonDataCommObj* pReq);
	int process_graphics_out_set(JsonDataCommObj* pReq);

	RPC_SRV_RESULT UpdateScreenStatus();

};
/* ------------------------------------------------------------------------- */

#endif
