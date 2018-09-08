#ifndef __MODBUSSRV_JSON_DEF_H_
#define __MODBUSSRV_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#include "ADCmnPortList.h"
#include "ADCommon.hpp"
//#include "DisplayDevice.hpp"
#define MODBUSSRV_JSON_PORT_NUMBER    ADCMN_PORT_MODBUSSRV
/* ------------------------------------------------------------------------- */
typedef enum EJSON_MODBUSSRV_RPC_TYPES_T
{
	EJSON_MODBUSSRV_RPC_MODBUS_INIT,
	EJSON_MODBUSSRV_RPC_END,
	EJSON_MODBUSSRV_RPC_NONE
}EJSON_MODBUSSRV_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//keep all the data related to modbus-server here
typedef struct MODBUSSRV_CMN_DATA_CACHE_T
{
	void *pDevInfo;//device-info-struct(typecast in rpc handlers)
	void *pEventNotifier;//event notifier object
	//DisplayDevice *pDisplay;
	MODBUSSRV_CMN_DATA_CACHE_T() //constructor(initializer)
	{
		pDevInfo=NULL;
		pEventNotifier=NULL;
		//pDisplay=NULL;
	};
}MODBUSSRV_CMN_DATA_CACHE;
/* ------------------------------------------------------------------------- */
typedef enum MODBUSSRV_EVENT_TYPE_T
{
	MODBUSSRV_EVENT_TYPE_OUTPUT_CHANGED=ADLIB_EVENT_NUM_END,
	MODBUSSRV_EVENT_TYPE_END,
	MODBUSSRV_EVENT_TYPE_NONE
}MODBUSSRV_EVENT_TYPE;
#define SERVER_JSON_PORT_NUM MODBUSSRV_JSON_PORT_NUMBER
#endif

