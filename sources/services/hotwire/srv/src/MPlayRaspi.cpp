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

	//defaults
	graphics_out_sts=MPLAYSRV_GRAPHICS_OUT_ENABLE;//upon boot, graphics is always enabled.
	MediaFileType=MPLAYSRV_MEDIAFILE_TYPE_MEDIA;//MPLAYSRV_MEDIAFILE_TYPE_UNKNOWN;
	MediaFile="/opt/fmw/misc_binaries/sample-video.mkv";//"none";
	MediaLoop=MPLAYSRV_MEDIA_LOOP_DISABLE;
	SeamlessLoop=MPLAYSRV_MEDIA_LOOP_ENABLE;
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
/*****************************************************************************/
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
RPC_SRV_RESULT MPlayRaspi::show_image(std::string imgfile)
{
	char command[1024];
	if(imgfile=="none")
	{
		sprintf(command,"dd if=/dev/zero of=/dev/fb0;setterm -cursor off >/dev/tty1");
		system(command);
		return RPC_SRV_RESULT_SUCCESS;
	}

	//check if image file exists //TODO:
	sprintf(command,"mkfifo %s",IMG_RENDER_FIFO_FILE);
	system(command);

	//wake-up sleeping display-output and clear screen
	sprintf(command,"echo -ne \"\\033[9;2]\">/dev/tty1;dd if=/dev/zero of=/dev/fb0");
	system(command);

	//using fbv command render the image file
	sprintf(command,"fbv -f %s < %s &",imgfile.c_str(),IMG_RENDER_FIFO_FILE);
	system(command);
	//with following command, frame is rendered on the screen(need to check why?)
	sprintf(command,"echo . > %s",IMG_RENDER_FIFO_FILE);
	system(command);

	//pDataCache->ScreenStatus=MPLAYSRV_SCREENSTS_IMAGE;

	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT MPlayRaspi::get_mediafile_type(MPLAYSRV_MEDIAFILE_TYPE& type)
{
	type=MediaFileType;
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT MPlayRaspi::set_mediafile_type(MPLAYSRV_MEDIAFILE_TYPE type)
{
	MediaFileType=type;
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT MPlayRaspi::get_mediafile(char* file)
{
	strcpy(file,MediaFile.c_str());
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT MPlayRaspi::set_mediafile(char* file)
{
	MediaFile=file;
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT MPlayRaspi::get_media_loop(MPLAYSRV_MEDIA_LOOP& loop)
{
	loop=MediaLoop;
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT MPlayRaspi::set_media_loop(MPLAYSRV_MEDIA_LOOP loop)
{
	MediaLoop=loop;
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT MPlayRaspi::get_seamless_loop(MPLAYSRV_MEDIA_LOOP& loop)
{
	loop=SeamlessLoop;
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT MPlayRaspi::set_seamless_loop(MPLAYSRV_MEDIA_LOOP loop)
{
	SeamlessLoop=loop;
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
bool MPlayRaspi::is_omx_running()
{
	char command[1024];
	sprintf(command,"ps cax | grep [o]mxplayer >/dev/null");
	if (system(command)==0)
		return true;
	else
	{
		sprintf(command,"ps cax | grep [h]ello_video >/dev/null");
		if (system(command)==0)
			return true;
		else
			return false;
	}
}
RPC_SRV_RESULT MPlayRaspi::set_media_action(MPLAYSRV_MEDIA_ACTION act)
{
	char command[1024];
	bool omx_sts=false;
	omx_sts=is_omx_running();
	switch(act)
	{
		case MPLAYSRV_MEDIA_ACTION_START :
				if(omx_sts==true)
					return RPC_SRV_RESULT_ACTION_NOT_ALLOWED;//video is already running, stop it first
				if(MediaFileType!=MPLAYSRV_MEDIAFILE_TYPE_MEDIA)
						return RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
				if(MediaFile=="")
						return RPC_SRV_RESULT_FILE_NOT_FOUND;//RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
				sprintf(command,"mkfifo /tmp/omxplay.fifo;rm -rf /tmp/omxplay.finished");
				system(command);

				if(MediaLoop==MPLAYSRV_MEDIA_LOOP_DISABLE)
				{
					//TODO: check if the media file exists, else return file-not-found
					sprintf(command,"(omxplayer -b --layer 2 -r -o both %s;fbset -depth 8 && fbset -depth 16;touch /tmp/omxplay.finished) < /tmp/omxplay.fifo &",MediaFile.c_str());
					system(command);
					sprintf(command,"echo . > /tmp/omxplay.fifo");
				}
				else
				{
					if(SeamlessLoop==MPLAYSRV_MEDIA_LOOP_ENABLE)
					{
						sprintf(command,"(hello_video --loop %s;fbset -depth 8 && fbset -depth 16;touch /tmp/omxplay.finished) < /tmp/omxplay.fifo &",MediaFile.c_str());
						system(command);
						sprintf(command,"echo . > /tmp/omxplay.fifo");
					}
					else
						sprintf(command,"/opt/fmw/bin/omx-loop.sh %s &",MediaFile.c_str());
				}
				system(command);
				//VideoPaused=false;
				return RPC_SRV_RESULT_SUCCESS;		
				break;
		case MPLAYSRV_MEDIA_ACTION_PAUSE  ://user for both pause or play(sending pause on paused video, will start playing)
				if(omx_sts==true)
				{
					sprintf(command,"echo -n p > /tmp/omxplay.fifo");
					system(command);
					//VideoPaused=!pDataCache->VideoPaused;
					return RPC_SRV_RESULT_SUCCESS;
				}
				else
					return RPC_SRV_RESULT_ACTION_NOT_ALLOWED;//video is already stopped;//RPC_SRV_RESULT_FAIL;
				break;
		case MPLAYSRV_MEDIA_ACTION_STOP  :
				if(omx_sts==true)
				{
					if(MediaLoop==MPLAYSRV_MEDIA_LOOP_ENABLE && SeamlessLoop==MPLAYSRV_MEDIA_LOOP_DISABLE)
					{
						sprintf(command,"touch /tmp/omxplay.stoploop");
						system(command);
					}
					sprintf(command,"echo -n q > /tmp/omxplay.fifo");
					system(command);
					//pDataCache->VideoPaused=false;
					//pDataCache->ScreenStatus=MPLAYSRV_SCREENSTS_BLANK;
					return RPC_SRV_RESULT_SUCCESS;
				}
				else
					return RPC_SRV_RESULT_ACTION_NOT_ALLOWED;//video is already stopped
				break;
		default:
			break;
	}
	return RPC_SRV_RESULT_FAIL;
}
/*****************************************************************************/


