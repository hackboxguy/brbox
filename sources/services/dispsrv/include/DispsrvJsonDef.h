#ifndef __DISPSRV_JSON_DEF_H_
#define __DISPSRV_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#include "ADCmnPortList.h"
#include "ADCommon.hpp"
#define DISPSRV_JSON_PORT_NUMBER    ADCMN_PORT_DISPSRV
/* ------------------------------------------------------------------------- */
typedef enum EJSON_DISPSRV_RPC_TYPES_T
{
	//EJSON_DISPSRV_RPC_IO_GET=0,
	//EJSON_DISPSRV_RPC_IO_SET,
	//EJSON_DISPSRV_RPC_IO_CONFIG,
	EJSON_DISPSRV_RPC_END,
	EJSON_DISPSRV_RPC_NONE
}EJSON_DISPSRV_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//#define GPIOCTL_RPC_IO_GET       "gpio_get"
//#define GPIOCTL_RPC_IO_SET       "gpio_set"
//#define GPIOCTL_RPC_IO_ADDR_ARG  "addr"
//#define GPIOCTL_RPC_IO_DATA_ARG  "data"
//typedef struct GPIOCTL_IO_ACCESS_PACKET_T
//{
//	unsigned int addr;
//	unsigned int data;
//}GPIOCTL_IO_ACCESS_PACKET;
/* ------------------------------------------------------------------------- */
//keep all the data related to gpioctl-service here
typedef struct DISPSRV_CMN_DATA_CACHE_T
{
	void *pDevInfo;//device-info-struct(typecast in rpc handlers)
	void *pEventNotifier;//event notifier object
	//unsigned int gpio_data[GPIOCTL_MAX_GPIO_PINS];//allow max 64gpio addresses(0 to 63)
	//unsigned int gpio_data_prev[GPIOCTL_MAX_GPIO_PINS];//comparing the last value for eventing
	DISPSRV_CMN_DATA_CACHE_T() //constructor(initializer)
	{
		pDevInfo=NULL;
		pEventNotifier=NULL;
		//for(int i=0;i<GPIOCTL_MAX_GPIO_PINS;i++)
		//	gpio_data[i]=gpio_data_prev[i]=2;//init with invalid value so that comparision fails and event can be sent
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

