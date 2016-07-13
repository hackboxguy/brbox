#ifndef __LIGHTSENSE_JSON_DEF_H_
#define __LIGHTSENSE_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#include "ADCmnPortList.h"
#include "ADCommon.hpp"
#include "LightSensor.hpp"
#define LIGHTSENSE_JSON_PORT_NUMBER    ADCMN_PORT_LIGHTSENSE
/* ------------------------------------------------------------------------- */
typedef enum EJSON_LIGHTSENSE_RPC_TYPES_T
{
	EJSON_LIGHTSENSE_RPC_SENSOR_INIT,
	//LIGHTSENSE_DISPSRV_RPC_DISP_CLEAR,
	//LIGHTSENSE_DISPSRV_RPC_DISP_PRINT,
	EJSON_LIGHTSENSE_RPC_END,
	EJSON_LIGHTSENSE_RPC_NONE
}EJSON_LIGHTSENSE_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_RPC_DISP_INIT,
#define	LIGHTSENSE_RPC_SENSOR_INIT            "sensor_init"
//#define	DISPSRV_RPC_DISP_CLEAR           "display_clear"
//#define	DISPSRV_RPC_DISP_PRINT           "display_print"
//#define DISPSRV_RPC_DISP_PRINT_LINE_ARG  "line"
//#define DISPSRV_RPC_DISP_PRINT_MESG_ARG  "msg"
//#define DISPSRV_RPC_DISP_LINE_ARG_TABL    {"line1","line2","line3","line4","lineall","none","none","\0"} //
//important: keep enum DISPLAY_LINE and EJSON_DISPSRV_LINE same
/*typedef enum EJSON_DISPSRV_LINE_T
{
	EJSON_DISPSRV_LINE_1,
	EJSON_DISPSRV_LINE_2,
	EJSON_DISPSRV_LINE_3,
	EJSON_DISPSRV_LINE_4,
	EJSON_DISPSRV_LINE_FULL,
	EJSON_DISPSRV_LINE_UNKNOWN,
	EJSON_DISPSRV_LINE_NONE
}EJSON_DISPSRV_LINE;
typedef struct LIGHTSENSE_PRINT_PACKET_T
{
	EJSON_LIGHTSENSE_LINE line;
	char msg[1024];
}LIGHTSENSE_PRINT_PACKET;*/
/* ------------------------------------------------------------------------- */
//keep all the data related to gpioctl-service here
typedef struct LIGHTSENSE_CMN_DATA_CACHE_T
{
	void *pDevInfo;//device-info-struct(typecast in rpc handlers)
	void *pEventNotifier;//event notifier object
	//void *pDispAccess;//display control object
	//unsigned int gpio_data[GPIOCTL_MAX_GPIO_PINS];//allow max 64gpio addresses(0 to 63)
	//unsigned int gpio_data_prev[GPIOCTL_MAX_GPIO_PINS];//comparing the last value for eventing
	LightSensor *pSensor;
	LIGHTSENSE_CMN_DATA_CACHE_T() //constructor(initializer)
	{
		pDevInfo=NULL;
		pEventNotifier=NULL;
		//pDispAccess=NULL;
		pSensor=NULL;
	};
}LIGHTSENSE_CMN_DATA_CACHE;
/* ------------------------------------------------------------------------- */
typedef enum LIGHTSENSE_EVENT_TYPE_T
{
	LIGHTSENSE_EVENT_TYPE_SENSOR_INITIALIZED=ADLIB_EVENT_NUM_END,
	LIGHTSENSE_EVENT_TYPE_END,
	LIGHTSENSE_EVENT_TYPE_NONE
}LIGHTSENSE_EVENT_TYPE;
#define SERVER_JSON_PORT_NUM LIGHTSENSE_JSON_PORT_NUMBER
#endif

