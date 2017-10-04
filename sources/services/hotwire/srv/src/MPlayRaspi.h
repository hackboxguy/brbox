#ifndef __MPLAY_RASPI_H_
#define __MPLAY_RASPI_H_
//#include <stdint.h>
//#include "I2CBusAccess.h"
//#include "I2CTAOS3414Reg.h"
#include "MPlayer.h"
#include "ADCommon.hpp"
class MPlayRaspi : public MPlayer
{
	bool is_omx_running();

public:
	MPlayRaspi(std::string DevNode);//,std::string DevType);
	~MPlayRaspi();
	RPC_SRV_RESULT read_edid(std::string filepath,std::string devnode="");
	RPC_SRV_RESULT get_graphics_out_ctrl(MPLAYSRV_GRAPHICS_OUT& sts);
	RPC_SRV_RESULT set_graphics_out_ctrl(MPLAYSRV_GRAPHICS_OUT sts);
	RPC_SRV_RESULT show_image(std::string imgfile);
	RPC_SRV_RESULT get_mediafile_type(MPLAYSRV_MEDIAFILE_TYPE& type);
	RPC_SRV_RESULT set_mediafile_type(MPLAYSRV_MEDIAFILE_TYPE type);
	RPC_SRV_RESULT get_mediafile(char* file);
	RPC_SRV_RESULT set_mediafile(char* file);
	RPC_SRV_RESULT get_media_loop(MPLAYSRV_MEDIA_LOOP& loop);
	RPC_SRV_RESULT set_media_loop(MPLAYSRV_MEDIA_LOOP loop);
	RPC_SRV_RESULT get_seamless_loop(MPLAYSRV_MEDIA_LOOP& loop);
	RPC_SRV_RESULT set_seamless_loop(MPLAYSRV_MEDIA_LOOP loop);
	RPC_SRV_RESULT set_media_action(MPLAYSRV_MEDIA_ACTION act);

};
#endif

