#ifndef __MPLAYSRV_JSON_DEF_H_
#define __MPLAYSRV_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#include "ADCmnPortList.h"
#define GPIOCTL_JSON_PORT_NUMBER    ADCMN_PORT_MPLAYSRV
/* ------------------------------------------------------------------------- */
#define GPIOCTL_OMXACT_START_FILE    "/home/pi/videos/start.mp4"
#define GPIOCTL_OMXACT_STOP_FILE     "/home/pi/videos/stop.mp4"
#define GPIOCTL_OMXACT_INTR_FILE     "/home/pi/videos/interrupt.mp4"
/* ------------------------------------------------------------------------- */
typedef enum EJSON_GPIOCTL_RPC_TYPES_T
{
	EJSON_GPIOCTL_RPC_OMXACT_GET=0,
	EJSON_GPIOCTL_RPC_OMXACT_SET,

	EJSON_MPLAYSRV_RPC_SHOWFBIMG_GET,
	EJSON_MPLAYSRV_RPC_SHOWFBIMG_SET, //frame buffer based image rendering

	EJSON_MPLAYSRV_RPC_QRCODEIMG_SET, //converts given string to qr-code-png-file

	EJSON_MPLAYSRV_RPC_PATTERN_GET,
	EJSON_MPLAYSRV_RPC_PATTERN_SET, //frame buffer based pattern rendering

	EJSON_MPLAYSRV_RPC_MEDIAFILE_TYPE_GET,
	EJSON_MPLAYSRV_RPC_MEDIAFILE_TYPE_SET,
	EJSON_MPLAYSRV_RPC_MEDIAFILE_GET,
	EJSON_MPLAYSRV_RPC_MEDIAFILE_SET,
	EJSON_MPLAYSRV_RPC_MEDIA_ACTION_SET, //start/stop/pause video
	EJSON_MPLAYSRV_RPC_SCREENSTS_GET,   //read the status of video/graphics output

	EJSON_GPIOCTL_RPC_END,
	EJSON_GPIOCTL_RPC_NONE
}EJSON_GPIOCTL_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//EJSON_GPIOCTL_RPC_IO_GET,
//EJSON_GPIOCTL_RPC_IO_SET,
#define GPIOCTL_RPC_IO_GET       "gpio_get"
#define GPIOCTL_RPC_IO_SET       "gpio_set"
#define GPIOCTL_RPC_IO_ADDR_ARG  "addr"
#define GPIOCTL_RPC_IO_DATA_ARG  "data"
typedef struct GPIOCTL_IO_ACCESS_PACKET_T
{
	unsigned int addr;//EJSON_MPLAYSRV_RPC_SHOWIMG_SET
//EJSON_MPLAYSRV_RPC_SHOWIMG_GET

	unsigned int data;
}GPIOCTL_IO_ACCESS_PACKET;
/* ------------------------------------------------------------------------- */
//omx action for hotwire application
//EJSON_GPIOCTL_RPC_OMXACT_SET
//EJSON_GPIOCTL_RPC_OMXACT_GET
#define GPIOCTL_RPC_OMXACT_GET         "get_omxact"
#define GPIOCTL_RPC_OMXACT_SET         "set_omxact"
#define GPIOCTL_RPC_OMXACT_ARG         "action"
#define GPIOCTL_RPC_OMXACT_ARG_TABL    {"start","intr","stop","warn","idle","unknown","none","\0"}
typedef enum GPIOCTL_OMXACT_TYPE_T
{
	GPIOCTL_OMXACT_START,//play start movie
	GPIOCTL_OMXACT_INTR, //play intr movie
	GPIOCTL_OMXACT_STOP, //play stop movie
	GPIOCTL_OMXACT_WARN, //play warning audio
	GPIOCTL_OMXACT_IDLE, //doing nothing(in idle mode)
	GPIOCTL_OMXACT_UNKNOWN,
	GPIOCTL_OMXACT_NONE
}GPIOCTL_OMXACT_TYPE;
typedef struct GPIOCTL_OMXACT_PACKET_T
{
	GPIOCTL_OMXACT_TYPE ActType;
	int taskID;
}GPIOCTL_OMXACT_PACKET;

/* ------------------------------------------------------------------------- */
//omx action for hotwire application
//EJSON_MPLAYSRV_RPC_SHOWFBIMG_SET
//EJSON_MPLAYSRV_RPC_SHOWFBIMG_GET
#define MPLAYSRV_RPC_SHOWFBIMG_GET         "get_show_fb_image"
#define MPLAYSRV_RPC_SHOWFBIMG_SET         "set_show_fb_image"
#define MPLAYSRV_RPC_SHOWFBIMG_ARG         "imgpath"
typedef struct MPLAYSRV_SHOWFBIMG_PACKET_T
{
	char fbimgpath[512];//"none" means remove-image or blank-screen(else "/some/path/imgfile.png")
	//int taskID;
}MPLAYSRV_SHOWFBIMG_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_MPLAYSRV_RPC_QRCODEIMG_SET
#define MPLAYSRV_RPC_QRCODEIMG_SET          "set_qrcode_image"
#define MPLAYSRV_RPC_QRCODEIMG_ARG_FILEPATH "filepath"
#define MPLAYSRV_RPC_QRCODEIMG_ARG_QRSTRING "qrstring"
typedef struct MPLAYSRV_QRCODEIMG_PACKET_T
{
	char qrfilepath[1024];
	char qrstring[1024];
	//int taskID;
}MPLAYSRV_QRCODEIMG_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_MPLAYSRV_RPC_PATTERN_GET,
//EJSON_MPLAYSRV_RPC_PATTERN_SET, //frame buffer based pattern rendering
#define MPLAYSRV_RPC_PATTERN_GET         "get_pattern"
#define MPLAYSRV_RPC_PATTERN_SET         "set_pattern"
#define MPLAYSRV_RPC_PATTERN_ARG         "pattern"
#define MPLAYSRV_RPC_PATTERN_ARG_TABL    {"red","green","blue","cyan","magenta","yellow","white","black","unknown","none","\0"}
typedef enum MPLAYSRV_PATTERN_TYPE_T
{
	MPLAYSRV_PATTERN_RED,
	MPLAYSRV_PATTERN_GREEN,
	MPLAYSRV_PATTERN_BLUE,
	MPLAYSRV_PATTERN_CYAN,
	MPLAYSRV_PATTERN_MAGENTA,
	MPLAYSRV_PATTERN_YELLOW,
	MPLAYSRV_PATTERN_WHITE,
	MPLAYSRV_PATTERN_BLACK,
	MPLAYSRV_PATTERN_UNKNOWN,
	MPLAYSRV_PATTERN_NONE
}MPLAYSRV_PATTERN_TYPE;
typedef struct MPLAYSRV_PATTERN_PACKET_T
{
	MPLAYSRV_PATTERN_TYPE PatType;
}MPLAYSRV_PATTERN_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_MPLAYSRV_RPC_MEDIAFILE_TYPE_GET,
//EJSON_MPLAYSRV_RPC_MEDIAFILE_TYPE_SET,
#define MPLAYSRV_RPC_MEDIAFILE_TYPE_GET         "get_media_file_type"
#define MPLAYSRV_RPC_MEDIAFILE_TYPE_SET         "set_media_file_type"
#define MPLAYSRV_RPC_MEDIAFILE_TYPE_ARG         "filetype"
#define MPLAYSRV_RPC_MEDIAFILE_TYPE_ARG_TABL    {"media","playlist","unknown","none","\0"}
typedef enum MPLAYSRV_MEDIAFILE_TYPE_T
{
	MPLAYSRV_MEDIAFILE_TYPE_MEDIA,
	MPLAYSRV_MEDIAFILE_TYPE_PLAYLIST,
	MPLAYSRV_MEDIAFILE_TYPE_UNKNOWN,
	MPLAYSRV_MEDIAFILE_TYPE_NONE
}MPLAYSRV_MEDIAFILE_TYPE;

//EJSON_MPLAYSRV_RPC_MEDIAFILE_GET,
//EJSON_MPLAYSRV_RPC_MEDIAFILE_SET,
#define MPLAYSRV_RPC_MEDIAFILE_GET         "get_media_file_path"
#define MPLAYSRV_RPC_MEDIAFILE_SET         "set_media_file_path"
#define MPLAYSRV_RPC_MEDIAFILE_ARG         "filepath"

//EJSON_MPLAYSRV_RPC_MEDIA_ACTION_SET, //start/stop/pause video
#define MPLAYSRV_RPC_MEDIA_ACTION_SET         "set_media_action"
#define MPLAYSRV_RPC_MEDIA_ACTION_ARG         "action"
#define MPLAYSRV_RPC_MEDIA_ACTION_ARG_TABL    {"start","pause_play","stop","unknown","none","\0"}
typedef enum MPLAYSRV_MEDIA_ACTION_T
{
	MPLAYSRV_MEDIA_ACTION_START,
	MPLAYSRV_MEDIA_ACTION_PAUSE,
	MPLAYSRV_MEDIA_ACTION_STOP,
	MPLAYSRV_MEDIA_ACTION_UNKNOWN,
	MPLAYSRV_MEDIA_ACTION_NONE
}MPLAYSRV_MEDIA_ACTION;

//mplay/mpause/pattern/image/blank/off
//EJSON_MPLAYSRV_RPC_SCREENSTS_GET,   //read the status of video/graphics output
#define MPLAYSRV_RPC_SCREENSTS_GET         "get_screen_status"
#define MPLAYSRV_RPC_SCREENSTS_ARG         "status"
#define MPLAYSRV_RPC_SCREENSTS_ARG_TABL    {"mplay","mpause","pattern","image","blank","off","unknown","none","\0"}
typedef enum MPLAYSRV_SCREENSTS_T
{
	MPLAYSRV_SCREENSTS_MPLAY,
	MPLAYSRV_SCREENSTS_MPAUSE,
	MPLAYSRV_SCREENSTS_PATTERN,
	MPLAYSRV_SCREENSTS_IMAGE,
	MPLAYSRV_SCREENSTS_BLANK,
	MPLAYSRV_SCREENSTS_OFF,
	MPLAYSRV_SCREENSTS_UNKNOWN,
	MPLAYSRV_SCREENSTS_NONE
}MPLAYSRV_SCREENSTS;

typedef struct MPLAYSRV_MEDIA_PACKET_T
{
	MPLAYSRV_MEDIAFILE_TYPE MediaFileType;
	char MediaFilePath[1023];
	MPLAYSRV_MEDIA_ACTION MediaAction;
	MPLAYSRV_SCREENSTS ScreenSts;
}MPLAYSRV_MEDIA_PACKET;
/* ------------------------------------------------------------------------- */
//keep all the data related to mplaysrv-service here
typedef struct GPIOCTL_CMN_DATA_CACHE_T
{
	void *pDevInfo;//device-info-struct(typecast in rpc handlers)
	unsigned int tmpData;
	GPIOCTL_OMXACT_TYPE ActType;
	std::string fbimgpath;
	int qr_density;
	int qr_size;
	MPLAYSRV_PATTERN_TYPE pattern;
	MPLAYSRV_PATTERN_TYPE StartupBkgnd;
	MPLAYSRV_MEDIAFILE_TYPE MediaFileType;
	std::string MediaFilePath;

	GPIOCTL_CMN_DATA_CACHE_T()
	{
		fbimgpath="none";
		qr_density=500;//TODO: configure qr-code-density via rpc
		qr_size=37;//TODO: configure qr-code-size via rpc
		pattern=MPLAYSRV_PATTERN_NONE;
		StartupBkgnd=MPLAYSRV_PATTERN_BLUE;//TODO: read from user setting
		MediaFileType=MPLAYSRV_MEDIAFILE_TYPE_MEDIA;//MPLAYSRV_MEDIAFILE_TYPE_UNKNOWN;
		MediaFilePath="/opt/fmw/misc_binaries/sample-video.mkv";//"none";
	};//initialize variables here
	~ GPIOCTL_CMN_DATA_CACHE_T(){};

}GPIOCTL_CMN_DATA_CACHE;
/* ------------------------------------------------------------------------- */

#endif

