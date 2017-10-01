#include "MPlayX86.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/*****************************************************************************/
MPlayX86::MPlayX86(std::string DevNode):MPlayer(DevNode)//,I2CBusAccess(DevNode)
{
	//sensorI2CAddr=COLOR_SENSOR_ADDR;
	//ledStatus = 0;//flashlight led on the grove color sensor
}
MPlayX86::~MPlayX86()
{
}
/*****************************************************************************/
RPC_SRV_RESULT MPlayX86::read_edid(std::string filepath,std::string devnode)//pPacket->filepath,"/dev/i2c-3");
{
	char command[1024];
	if(devnode=="none")
		return RPC_SRV_RESULT_ARG_ERROR;
	if(devnode=="")
		return RPC_SRV_RESULT_ARG_ERROR;
	sprintf(command,"eeprog %s 0x50 -f -q -r 0:256 -o %s",devnode.c_str(),filepath.c_str());
	if (system(command)==0)
		return RPC_SRV_RESULT_SUCCESS;
	else
		return RPC_SRV_RESULT_FAIL;

}
/*****************************************************************************/
RPC_SRV_RESULT MPlayX86::get_graphics_out_ctrl(MPLAYSRV_GRAPHICS_OUT& sts)
{
	sts=graphics_out_sts;
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT MPlayX86::set_graphics_out_ctrl(MPLAYSRV_GRAPHICS_OUT sts)
{
	char command[1024];
	if(sts==MPLAYSRV_GRAPHICS_OUT_DISABLE)
		sprintf(command,"export DISPLAY=:0;xset dpms force off");
	else
		sprintf(command,"export DISPLAY=:0;xset dpms force on");
	
	if (system(command)==0)
	{
		graphics_out_sts=sts;
		return RPC_SRV_RESULT_SUCCESS;
	}
	else
		return RPC_SRV_RESULT_FAIL;
}
/*****************************************************************************/

