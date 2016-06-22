#ifndef __DISPSRV_JSON_DEF_H_
#define __DISPSRV_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#include "ADCmnPortList.h"
#include "ADCommon.hpp"
#define DISPSRV_JSON_PORT_NUMBER    ADCMN_PORT_DISPSRV
/* ------------------------------------------------------------------------- */
typedef enum EJSON_DISPSRV_RPC_TYPES_T
{
	EJSON_DISPSRV_RPC_DISP_INIT,
	EJSON_DISPSRV_RPC_DISP_CLEAR,
	EJSON_DISPSRV_RPC_DISP_PRINT,
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
typedef struct DISPSRV_PRINT_PACKET_T
{
	int line;
	char msg[1024];
}DISPSRV_PRINT_PACKET;
/* ------------------------------------------------------------------------- */
//keep all the data related to gpioctl-service here
typedef struct DISPSRV_CMN_DATA_CACHE_T
{
	void *pDevInfo;//device-info-struct(typecast in rpc handlers)
	void *pEventNotifier;//event notifier object
	void *pDispAccess;//display control object
	//unsigned int gpio_data[GPIOCTL_MAX_GPIO_PINS];//allow max 64gpio addresses(0 to 63)
	//unsigned int gpio_data_prev[GPIOCTL_MAX_GPIO_PINS];//comparing the last value for eventing
	DISPSRV_CMN_DATA_CACHE_T() //constructor(initializer)
	{
		pDevInfo=NULL;
		pEventNotifier=NULL;
		pDispAccess=NULL;
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

