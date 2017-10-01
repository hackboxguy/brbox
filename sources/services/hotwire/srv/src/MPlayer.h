#ifndef __MPLAYER_H_
#define __MPLAYER_H_
/*interface class for color and brightness sensors*/
#include <iostream>
#include "ADCommon.hpp"
//#include <stdint.h>
#include "HotwireJsonDef.h"
/*****************************************************************************/
class MPlayer
{
	std::string edid_dev_node;
protected:
	MPLAYSRV_GRAPHICS_OUT graphics_out_sts;
public:
	MPlayer(std::string devnode);
	~MPlayer();
	virtual RPC_SRV_RESULT read_edid(std::string filepath,std::string devnode)=0;
	virtual RPC_SRV_RESULT get_graphics_out_ctrl(MPLAYSRV_GRAPHICS_OUT& sts)=0;
	virtual RPC_SRV_RESULT set_graphics_out_ctrl(MPLAYSRV_GRAPHICS_OUT sts)=0;

	//virtual RPC_SRV_RESULT init_sensor()=0;//must-have function for derived class
	//virtual RPC_SRV_RESULT trigger_measurement()=0;//must-have function for derived class
	//virtual RPC_SRV_RESULT get_integration_time(uint32_t &time){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	//virtual RPC_SRV_RESULT set_integration_time(uint32_t time){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
};
#endif
