#ifndef __GPIOCTL_JSON_DEF_H_
#define __GPIOCTL_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#include "ADCmnPortList.h"
#define GPIOCTL_JSON_PORT_NUMBER    ADCMN_PORT_GPIOCTL
/* ------------------------------------------------------------------------- */
#define GPIOCTL_OMXACT_START_FILE    "/home/pi/videos/start.mp4"
#define GPIOCTL_OMXACT_STOP_FILE     "/home/pi/videos/stop.mp4"
#define GPIOCTL_OMXACT_INTR_FILE     "/home/pi/videos/interrupt.mp4"
/* ------------------------------------------------------------------------- */
typedef enum EJSON_GPIOCTL_RPC_TYPES_T
{
	EJSON_GPIOCTL_RPC_IO_GET=0,
	EJSON_GPIOCTL_RPC_IO_SET,
	EJSON_GPIOCTL_RPC_IO_CONFIG,

	EJSON_GPIOCTL_RPC_OMXACT_GET,
	EJSON_GPIOCTL_RPC_OMXACT_SET,

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
	unsigned int addr;
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
//keep all the data related to gpioctl-service here
typedef struct GPIOCTL_CMN_DATA_CACHE_T
{
	void *pDevInfo;//device-info-struct(typecast in rpc handlers)
	unsigned int tmpData;
	GPIOCTL_OMXACT_TYPE ActType;
}GPIOCTL_CMN_DATA_CACHE;
/* ------------------------------------------------------------------------- */

#endif

