#ifndef __MPLAYER_H_
#define __MPLAYER_H_
/*interface class for color and brightness sensors*/
#include <iostream>
#include "ADCommon.hpp"
//#include <stdint.h>
#include "HotwireJsonDef.h"
#define IMG_RENDER_FIFO_FILE "/tmp/img-renderer-process.fifo"
/*****************************************************************************/
class MPlayer
{
	std::string edid_dev_node;
protected:
	MPLAYSRV_GRAPHICS_OUT graphics_out_sts;
	MPLAYSRV_MEDIAFILE_TYPE MediaFileType;
	std::string MediaFile;
	MPLAYSRV_MEDIA_LOOP MediaLoop;
	MPLAYSRV_MEDIA_LOOP SeamlessLoop;//h264 formatter raw video only support
	bool VideoPaused;

public:
	MPlayer(std::string devnode);
	//{
		//graphics_out_sts=MPLAYSRV_GRAPHICS_OUT_ENABLE;//upon boot, graphics is always enabled.
		//MediaFileType=MPLAYSRV_MEDIAFILE_TYPE_MEDIA;//MPLAYSRV_MEDIAFILE_TYPE_UNKNOWN;
		//MediaFile="/opt/fmw/misc_binaries/sample-video.mkv";//"none";
		//MediaLoop=MPLAYSRV_MEDIA_LOOP_DISABLE;
		//SeamlessLoop=MPLAYSRV_MEDIA_LOOP_ENABLE;
	//};
	~MPlayer();
	virtual RPC_SRV_RESULT read_edid(std::string filepath,std::string devnode)=0;
	virtual RPC_SRV_RESULT get_graphics_out_ctrl(MPLAYSRV_GRAPHICS_OUT& sts)=0;
	virtual RPC_SRV_RESULT set_graphics_out_ctrl(MPLAYSRV_GRAPHICS_OUT sts)=0;
	virtual RPC_SRV_RESULT show_image(std::string imgfile)=0;
	virtual RPC_SRV_RESULT get_mediafile_type(MPLAYSRV_MEDIAFILE_TYPE& type)=0;
	virtual RPC_SRV_RESULT set_mediafile_type(MPLAYSRV_MEDIAFILE_TYPE type)=0;
	virtual RPC_SRV_RESULT get_mediafile(char* file)=0;
	virtual RPC_SRV_RESULT set_mediafile(char* file)=0;
	virtual RPC_SRV_RESULT get_media_loop(MPLAYSRV_MEDIA_LOOP& loop)=0;
	virtual RPC_SRV_RESULT set_media_loop(MPLAYSRV_MEDIA_LOOP loop)=0;
	virtual RPC_SRV_RESULT get_seamless_loop(MPLAYSRV_MEDIA_LOOP& loop)=0;
	virtual RPC_SRV_RESULT set_seamless_loop(MPLAYSRV_MEDIA_LOOP loop)=0;
        virtual RPC_SRV_RESULT set_media_action(MPLAYSRV_MEDIA_ACTION act)=0;


	//virtual RPC_SRV_RESULT init_sensor()=0;//must-have function for derived class
	//virtual RPC_SRV_RESULT trigger_measurement()=0;//must-have function for derived class
	//virtual RPC_SRV_RESULT get_integration_time(uint32_t &time){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	//virtual RPC_SRV_RESULT set_integration_time(uint32_t time){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
};
#endif
