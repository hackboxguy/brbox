#ifndef __MPLAY_RASPI_H_
#define __MPLAY_RASPI_H_
//#include <stdint.h>
//#include "I2CBusAccess.h"
//#include "I2CTAOS3414Reg.h"
#include "MPlayer.h"
#include "ADCommon.hpp"
class MPlayRaspi : public MPlayer
{
public:
	MPlayRaspi(std::string DevNode);//,std::string DevType);
	~MPlayRaspi();
	RPC_SRV_RESULT read_edid(std::string filepath,std::string devnode="");
	RPC_SRV_RESULT get_graphics_out_ctrl(MPLAYSRV_GRAPHICS_OUT& sts);
	RPC_SRV_RESULT set_graphics_out_ctrl(MPLAYSRV_GRAPHICS_OUT sts);
};
#endif

