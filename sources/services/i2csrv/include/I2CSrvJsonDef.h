#ifndef __I2CSRV_JSON_DEF_H_
#define __I2CSRV_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#include "ADCmnPortList.h"
#include "ADCommon.hpp"
#define I2CSRV_JSON_PORT_NUMBER    ADCMN_PORT_I2CSRV
//#define I2CSRV_MAX_GPIO_PINS       64
/* ------------------------------------------------------------------------- */
typedef enum EJSON_I2CSRV_RPC_TYPES_T
{
	EJSON_I2CSRV_RPC_PCF8574_GET=0,
	EJSON_I2CSRV_RPC_PCF8574_SET,
	EJSON_I2CSRV_RPC_END,
	EJSON_I2CSRV_RPC_NONE
}EJSON_I2CSRV_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//EJSON_I2CSRV_RPC_PCF8574_GET
//EJSON_I2CSRV_RPC_PCF8574_SET
#define I2CSRV_RPC_PCF8574_GET       "pcf8574_get"
#define I2CSRV_RPC_PCF8574_SET       "pcf8574_set"
#define I2CSRV_RPC_PCF8574_ADDR_ARG  "devaddr"
#define I2CSRV_RPC_PCF8574_DATA_ARG  "data"
typedef struct I2CSRV_PCF8574_ACCESS_PACKET_T
{
	unsigned char devaddr;
	unsigned char data;
}I2CSRV_PCF8574_ACCESS_PACKET;
/* ------------------------------------------------------------------------- */
//keep all the data related to gpioctl-service here
typedef struct I2CSRV_CMN_DATA_CACHE_T
{
	void *pDevInfo;//device-info-struct(typecast in rpc handlers)
	void *pEventNotifier;//event notifier object
	void *pDevAccess;//i2c device access node

	unsigned char pcf8574cache[256];
	unsigned char pcf8574cache_prev[256];
	I2CSRV_CMN_DATA_CACHE_T() //constructor(initializer)
	{
		pDevInfo=NULL;
		pEventNotifier=NULL;
		pDevAccess=NULL;
		for(int i=0;i<256;i++)
		{
			pcf8574cache[i]=0;
			pcf8574cache_prev[i]=0xff;
		}
	};
}I2CSRV_CMN_DATA_CACHE;
/* ------------------------------------------------------------------------- */
typedef enum EI2CSRV_EVENT_TYPE_T
{
	EI2CSRV_EVENT_TYPE_PCF8574_CHANGED=ADLIB_EVENT_NUM_END,
	EI2CSRV_EVENT_TYPE_END,
	EI2CSRV_EVENT_TYPE_NONE
}EI2CSRV_EVENT_TYPE;
#define SERVER_JSON_PORT_NUM I2CSRV_JSON_PORT_NUMBER
#endif

