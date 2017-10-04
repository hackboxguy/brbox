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
RPC_SRV_RESULT MPlayX86::remove_existing_image()
{
	char command[1024];
	sprintf(command,"export DISPLAY=:0;xdotool search --class feh 2>&1 | wc -l > %s","/tmp/temp-x11-feh-session.txt");
	if(system(command)!=0)
		return RPC_SRV_RESULT_FILE_OPEN_ERR;

	//char temp_str[255];
	FILE *shell;
	shell= fopen("/tmp/temp-x11-feh-session.txt","r");
	if(shell == NULL )
		return RPC_SRV_RESULT_FILE_OPEN_ERR;

	size_t read_bytes = fread(command,1,100,shell);
	fclose(shell);
	if(read_bytes>0)
	{
		command[read_bytes]='\0';
		if(command[strlen(command)-1]=='\n')//remove the carriage return line
			command[strlen(command)-1]='\0';
		if(strlen(command)>=1)
		{
			int len=atoi(command);
			switch(len)
			{
				case 0: break;//no feh sessions is running(even the desktop screen is not visible - terminal only mode)
				case 1: break;//no image is shown, only desktop screen is visible
				case 2: 
					sprintf(command,"export DISPLAY=:0;xdotool key q --windowid \"$(xdotool search --class feh | tail -1)\"");
					system(command);
					break;//an image is drawn on top of desktop screen
				default:break;//something is wron, there are more than two feh sessions are running.
			}
			//return RPC_SRV_RESULT_SUCCESS;
		}
		return RPC_SRV_RESULT_SUCCESS;
	}
	else
		return RPC_SRV_RESULT_FILE_READ_ERR;	

}
RPC_SRV_RESULT MPlayX86::show_image(std::string imgfile)
{
	char command[1024];
	//todo: check if already an existing 'feh' running in background

	remove_existing_image();

	if(imgfile=="none")
		return RPC_SRV_RESULT_SUCCESS;//last shown image successfully removed.

	sprintf(command,"export DISPLAY=:0;feh -F %s &",imgfile.c_str());
	if (system(command)==0)
		return RPC_SRV_RESULT_SUCCESS;
	else
		return RPC_SRV_RESULT_FAIL;

	//export DISPLAY=:0;feh -F /tmp/image.png &
	//export DISPLAY=:0;xdotool search --class feh
	//xdotool key q --windowid 6291457
}
/*****************************************************************************/


