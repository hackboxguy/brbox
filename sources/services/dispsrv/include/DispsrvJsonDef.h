#ifndef __DISPSRV_JSON_DEF_H_
#define __DISPSRV_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#include "ADCmnPortList.h"
#include "ADCommon.hpp"
#include "DisplayDevice.hpp"
#define DISPSRV_JSON_PORT_NUMBER    ADCMN_PORT_DISPSRV
/* ------------------------------------------------------------------------- */
typedef enum EJSON_DISPSRV_RPC_TYPES_T
{
	EJSON_DISPSRV_RPC_DISP_INIT,
	EJSON_DISPSRV_RPC_DISP_CLEAR,
	EJSON_DISPSRV_RPC_DISP_PRINT,
	EJSON_DISPSRV_RPC_DISP_GET_BLKT,
	EJSON_DISPSRV_RPC_DISP_SET_BLKT,
	EJSON_DISPSRV_RPC_END,
	EJSON_DISPSRV_RPC_NONE
}EJSON_DISPSRV_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//EJSON_DISPSRV_RPC_DISP_INIT,
//EJSON_DISPSRV_RPC_DISP_CLEAR,
//EJSON_DISPSRV_RPC_DISP_PRINT,
#define	DISPSRV_RPC_DISP_INIT            "display_init"
#define	DISPSRV_RPC_DISP_CLEAR           "display_clear"
#define	DISPSRV_RPC_DISP_PRINT           "display_print"
#define DISPSRV_RPC_DISP_PRINT_LINE_ARG  "line"
#define DISPSRV_RPC_DISP_PRINT_MESG_ARG  "msg"
#define DISPSRV_RPC_DISP_LINE_ARG_TABL    {"line1","line2","line3","line4","line5","line6","line7","line8","lineall","none","none","\0"} //
//important: keep enum DISPLAY_LINE and EJSON_DISPSRV_LINE same
typedef enum EJSON_DISPSRV_LINE_T
{
	EJSON_DISPSRV_LINE_1,
	EJSON_DISPSRV_LINE_2,
	EJSON_DISPSRV_LINE_3,
	EJSON_DISPSRV_LINE_4,
	EJSON_DISPSRV_LINE_5,
	EJSON_DISPSRV_LINE_6,
	EJSON_DISPSRV_LINE_7,
	EJSON_DISPSRV_LINE_8,
	EJSON_DISPSRV_LINE_FULL,
	EJSON_DISPSRV_LINE_UNKNOWN,
	EJSON_DISPSRV_LINE_NONE
}EJSON_DISPSRV_LINE;
/*typedef enum TEXT_ALIGNMENT_T
{
	TEXT_ALIGNMENT_LEFT,
	TEXT_ALIGNMENT_RIGHT,
	TEXT_ALIGNMENT_CENTER,
	TEXT_ALIGNMENT_UNKNOWN,
	TEXT_ALIGNMENT_NONE
}TEXT_ALIGNMENT;*/
/* ------------------------------------------------------------------------- */
//EJSON_DISPSRV_RPC_DISP_GET_BLKT,
//EJSON_DISPSRV_RPC_DISP_SET_BLKT,
#define	DISPSRV_RPC_DISP_GET_BKLT        "display_backlight_get"
#define	DISPSRV_RPC_DISP_SET_BKLT        "display_backlight_set"
#define DISPSRV_RPC_DISP_BKLT_ARG        "status"
#define DISPSRV_RPC_DISP_BKLT_ARG_TABL   {"off","on","unknown","none","\0"} 
typedef enum DISPSRV_BKLT_STS_T
{
	DISPSRV_BKLT_STS_OFF,
	DISPSRV_BKLT_STS_ON,
	DISPSRV_BKLT_STS_UNKNOWN,
	DISPSRV_BKLT_STS_NONE
}DISPSRV_BKLT_STS;
typedef struct DISPSRV_PRINT_PACKET_T
{
	//int line;
	//DISPLAY_LINE line;
	EJSON_DISPSRV_LINE line;
	char msg[1024];
	DISPSRV_BKLT_STS bklsts;//backlight status
}DISPSRV_PRINT_PACKET;
/* ------------------------------------------------------------------------- */
//keep all the data related to gpioctl-service here
typedef struct DISPSRV_CMN_DATA_CACHE_T
{
	void *pDevInfo;//device-info-struct(typecast in rpc handlers)
	void *pEventNotifier;//event notifier object
	//void *pDispAccess;//display control object
	//unsigned int gpio_data[GPIOCTL_MAX_GPIO_PINS];//allow max 64gpio addresses(0 to 63)
	//unsigned int gpio_data_prev[GPIOCTL_MAX_GPIO_PINS];//comparing the last value for eventing
	DisplayDevice *pDisplay;
	DISPSRV_CMN_DATA_CACHE_T() //constructor(initializer)
	{
		pDevInfo=NULL;
		pEventNotifier=NULL;
		//pDispAccess=NULL;
		pDisplay=NULL;
	};
}DISPSRV_CMN_DATA_CACHE;
/* ------------------------------------------------------------------------- */
typedef enum DISPSRV_EVENT_TYPE_T
{
	DISPSRV_EVENT_TYPE_OUTPUT_CHANGED=ADLIB_EVENT_NUM_END,
	DISPSRV_EVENT_TYPE_END,
	DISPSRV_EVENT_TYPE_NONE
}DISPSRV_EVENT_TYPE;
#define SERVER_JSON_PORT_NUM DISPSRV_JSON_PORT_NUMBER
#endif

