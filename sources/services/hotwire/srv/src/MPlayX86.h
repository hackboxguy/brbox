#ifndef __MPLAY_X86_H_
#define __MPLAY_X86_H_
//#include <stdint.h>
//#include "I2CBusAccess.h"
//#include "I2CTAOS3414Reg.h"
#include "MPlayer.h"
#include "ADCommon.hpp"
class MPlayX86 : public MPlayer
{
	RPC_SRV_RESULT remove_existing_image();
	bool is_media_playing();
	RPC_SRV_RESULT send_char_to_xutility(std::string utility, char ch);

public:
	MPlayX86(std::string DevNode);//,std::string DevType);
	~MPlayX86();
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

