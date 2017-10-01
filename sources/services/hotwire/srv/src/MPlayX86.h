#ifndef __MPLAY_X86_H_
#define __MPLAY_X86_H_
//#include <stdint.h>
//#include "I2CBusAccess.h"
//#include "I2CTAOS3414Reg.h"
#include "MPlayer.h"
#include "ADCommon.hpp"
class MPlayX86 : public MPlayer
{
public:
	MPlayX86(std::string DevNode);//,std::string DevType);
	~MPlayX86();
	RPC_SRV_RESULT read_edid(std::string filepath,std::string devnode="");
	RPC_SRV_RESULT get_graphics_out_ctrl(MPLAYSRV_GRAPHICS_OUT& sts);
	RPC_SRV_RESULT set_graphics_out_ctrl(MPLAYSRV_GRAPHICS_OUT sts);
};
#endif

