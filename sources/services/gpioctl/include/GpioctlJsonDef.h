#ifndef __GPIOCTL_JSON_DEF_H_
#define __GPIOCTL_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#include "ADCmnPortList.h"
#define GPIOCTL_JSON_PORT_NUMBER    ADCMN_PORT_GPIOCTL
#define GPIOCTL_MAX_GPIO_PINS       64
/* ------------------------------------------------------------------------- */
typedef enum EJSON_GPIOCTL_RPC_TYPES_T
{
	EJSON_GPIOCTL_RPC_IO_GET=0,
	EJSON_GPIOCTL_RPC_IO_SET,
	EJSON_GPIOCTL_RPC_IO_CONFIG,
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
//keep all the data related to gpioctl-service here
typedef struct GPIOCTL_CMN_DATA_CACHE_T
{
	void *pDevInfo;//device-info-struct(typecast in rpc handlers)
	unsigned int gpio_data[GPIOCTL_MAX_GPIO_PINS];//allow max 64gpio addresses(0 to 63)
}GPIOCTL_CMN_DATA_CACHE;
/* ------------------------------------------------------------------------- */

#endif

