#include "MPlayRaspi.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/*****************************************************************************/
MPlayRaspi::MPlayRaspi(std::string DevNode):MPlayer(DevNode)//,I2CBusAccess(DevNode)
{
	//sensorI2CAddr=COLOR_SENSOR_ADDR;
	//ledStatus = 0;//flashlight led on the grove color sensor
	char command[512];
	sprintf(command,"dd if=/dev/zero of=/dev/fb0;setterm -cursor off >/dev/tty1");
	system(command);
}
MPlayRaspi::~MPlayRaspi()
{
}
/*****************************************************************************/
RPC_SRV_RESULT MPlayRaspi::read_edid(std::string filepath,std::string devnode)
{
	char command[1024];
	sprintf(command,"tvservice -d %s",filepath.c_str());
	if (system(command)==0)
		return RPC_SRV_RESULT_SUCCESS;
	else
		return RPC_SRV_RESULT_FAIL;
}
/*****************************************************************************/
RPC_SRV_RESULT MPlayRaspi::get_graphics_out_ctrl(MPLAYSRV_GRAPHICS_OUT& sts)
{
	sts=graphics_out_sts;
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT MPlayRaspi::set_graphics_out_ctrl(MPLAYSRV_GRAPHICS_OUT sts)
{
	char command[1024];
	if(sts==MPLAYSRV_GRAPHICS_OUT_DISABLE)
		sprintf(command,"tvservice -o");
	else
		sprintf(command,"tvservice -p;fbset -depth 8 && fbset -depth 16");
	
	if (system(command)==0)
	{
		graphics_out_sts=sts;
		return RPC_SRV_RESULT_SUCCESS;
	}
	else
		return RPC_SRV_RESULT_FAIL;
}
/*****************************************************************************/

