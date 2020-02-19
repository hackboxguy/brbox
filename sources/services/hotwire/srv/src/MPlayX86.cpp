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

	//defaults
	graphics_out_sts=MPLAYSRV_GRAPHICS_OUT_ENABLE;//upon boot, graphics is always enabled.
	MediaFileType=MPLAYSRV_MEDIAFILE_TYPE_MEDIA;//MPLAYSRV_MEDIAFILE_TYPE_UNKNOWN;
	MediaFile="/opt/fmw/misc_binaries/sample-video.mkv";//"none";
	MediaLoop=MPLAYSRV_MEDIA_LOOP_DISABLE;
	SeamlessLoop=MPLAYSRV_MEDIA_LOOP_ENABLE;
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
std::string MPlayX86::get_image_geometry()
{
	char command[1024];
	sprintf(command,"fbset | grep geometry |awk '{print $2\"x\"$3'} > %s","/tmp/temp-x11-feh-session.txt");
	if(system(command)!=0)
		return "1920x1080";//RPC_SRV_RESULT_FILE_OPEN_ERR;

	FILE *shell;
	shell= fopen("/tmp/temp-x11-feh-session.txt","r");
	if(shell == NULL) 
		return "1920x1080";//RPC_SRV_RESULT_FILE_OPEN_ERR;

	size_t read_bytes = fread(command,1,100,shell);
	fclose(shell);
	if(read_bytes>0)
	{
		command[read_bytes]='\0';
		if(command[strlen(command)-1]=='\n')//remove the carriage return line
			command[strlen(command)-1]='\0';
		if(strlen(command)>=1)
		{
			std::string geo = command;
			return geo;
		}
	}
	return "1920x1080";
}
RPC_SRV_RESULT MPlayX86::show_image(std::string imgfile)
{
	char command[1024];
	//todo: check if already an existing 'feh' running in background

	remove_existing_image();

	if(imgfile=="none")
		return RPC_SRV_RESULT_SUCCESS;//last shown image successfully removed.
	std::string geometry = get_image_geometry();
	sprintf(command,"export DISPLAY=:0;feh -Z -g %s %s &",geometry.c_str(),imgfile.c_str());
	if (system(command)==0)
		return RPC_SRV_RESULT_SUCCESS;
	else
		return RPC_SRV_RESULT_FAIL;

	//export DISPLAY=:0;feh -F /tmp/image.png &
	//export DISPLAY=:0;xdotool search --class feh
	//xdotool key q --windowid 6291457
}
/*****************************************************************************/
RPC_SRV_RESULT MPlayX86::get_mediafile_type(MPLAYSRV_MEDIAFILE_TYPE& type)
{
	type=MediaFileType;
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT MPlayX86::set_mediafile_type(MPLAYSRV_MEDIAFILE_TYPE type)
{
	MediaFileType=type;
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT MPlayX86::get_mediafile(char* file)
{
	strcpy(file,MediaFile.c_str());
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT MPlayX86::set_mediafile(char* file)
{
	MediaFile=file;
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT MPlayX86::get_media_loop(MPLAYSRV_MEDIA_LOOP& loop)
{
	loop=MediaLoop;
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT MPlayX86::set_media_loop(MPLAYSRV_MEDIA_LOOP loop)
{
	MediaLoop=loop;
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT MPlayX86::get_seamless_loop(MPLAYSRV_MEDIA_LOOP& loop)
{
	loop=SeamlessLoop;
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT MPlayX86::set_seamless_loop(MPLAYSRV_MEDIA_LOOP loop)
{
	SeamlessLoop=loop;
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
bool MPlayX86::is_media_playing()
{
	char command[1024];
	sprintf(command,"ps cax | grep [g]st-play-1.0 >/dev/null");
	if (system(command)==0)
		return true;
	else
	{
		//sprintf(command,"ps cax | grep [h]ello_video >/dev/null");
		//if (system(command)==0)
		//	return true;
		//else
			return false;
	}
}

RPC_SRV_RESULT MPlayX86::set_media_action(MPLAYSRV_MEDIA_ACTION act)
{
	char command[1024];
	bool omx_sts=false;
	omx_sts=is_media_playing();
	switch(act)
	{
		case MPLAYSRV_MEDIA_ACTION_START :
				if(omx_sts==true)
					return RPC_SRV_RESULT_ACTION_NOT_ALLOWED;//video is already running, stop it first
				if(MediaFileType!=MPLAYSRV_MEDIAFILE_TYPE_MEDIA)
						return RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
				if(MediaFile=="")
						return RPC_SRV_RESULT_FILE_NOT_FOUND;//RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
				
				//sprintf(command,"mkfifo /tmp/omxplay.fifo;rm -rf /tmp/omxplay.finished");
				//sprintf(command,"rm -rf /tmp/omxplay.finished");
				system(command);

				if(MediaLoop==MPLAYSRV_MEDIA_LOOP_DISABLE)
				{
					//TODO: check if the media file exists, else return file-not-found
					//sprintf(command,"(export DISPLAY=:0; gst-play-1.0 %s --videosink=xvimagesink;touch /tmp/omxplay.finished) < /tmp/omxplay.fifo &",MediaFile.c_str());
					sprintf(command,"export DISPLAY=:0;(gst-play-1.0 %s --videosink=xvimagesink &)",MediaFile.c_str());
					system(command);
					//sprintf(command,"echo . > /tmp/omxplay.fifo");
				}
				else
				{
					//if(SeamlessLoop==MPLAYSRV_MEDIA_LOOP_ENABLE)
					//{
					//	sprintf(command,"(hello_video --loop %s;fbset -depth 8 && fbset -depth 16;touch /tmp/omxplay.finished) < /tmp/omxplay.fifo &",MediaFile.c_str());
					//	system(command);
					//	sprintf(command,"echo . > /tmp/omxplay.fifo");
					//}
					//else
					sprintf(command,"/opt/fmw/bin/gst-loop.sh %s &",MediaFile.c_str());
					system(command);
				}
				//system(command);
				//VideoPaused=false;
				return RPC_SRV_RESULT_SUCCESS;		
				break;
		case MPLAYSRV_MEDIA_ACTION_PAUSE :
				if(omx_sts==true)
				{
					//sprintf(command,"echo -n \" \" > /tmp/omxplay.fifo");
					//system(command);
					send_char_to_xutility("gst*", "space");
					return RPC_SRV_RESULT_SUCCESS;
				}
				else
					return RPC_SRV_RESULT_ACTION_NOT_ALLOWED;//video is already stopped;//RPC_SRV_RESULT_FAIL;
				break;//user for both pause or play(sending pause on paused video, will start playing)
		case MPLAYSRV_MEDIA_ACTION_STOP  :
				if(omx_sts==true)
				{
					if(MediaLoop==MPLAYSRV_MEDIA_LOOP_ENABLE)// && SeamlessLoop==MPLAYSRV_MEDIA_LOOP_DISABLE)
					{
						sprintf(command,"touch /tmp/omxplay.stoploop");
						system(command);
					}

					//sprintf(command,"echo -n q > /tmp/omxplay.fifo");
					//system(command);
					send_char_to_xutility("gst*", "q");
					return RPC_SRV_RESULT_SUCCESS;
				}
				else
					return RPC_SRV_RESULT_ACTION_NOT_ALLOWED;//video is already stopped

				break;
		default:break;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT MPlayX86::send_char_to_xutility(std::string utility, std::string ch)
{
	char command[1024];
	sprintf(command,"export DISPLAY=:0;xdotool key %s --windowid \"$(xdotool search --class %s)\"",ch.c_str(),utility.c_str());
	system(command);
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/

