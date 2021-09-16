#ifndef __MONITSRV_JSON_DEF_H_
#define __MONITSRV_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#include "ADCmnPortList.h"
#include "ADCommon.hpp"
#define MONITSRV_JSON_PORT_NUMBER    ADCMN_PORT_MONITSRV
/* ------------------------------------------------------------------------- */
typedef enum EJSON_MONITSRV_RPC_TYPES_T
{
	EJSON_MONITSRV_RPC_END,
	EJSON_MONITSRV_RPC_NONE
}EJSON_MONITSRV_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//keep all the data related to modbus-server here
typedef struct MONITSRV_CMN_DATA_CACHE_T
{
	void *pDevInfo;//device-info-struct(typecast in rpc handlers)
	void *pEventNotifier;//event notifier object
	void *pDevAccess;//modbus device access node

	//int energy_param;
	MONITSRV_CMN_DATA_CACHE_T() //constructor(initializer)
	{
		pDevAccess=NULL;
		pDevInfo=NULL;
		pEventNotifier=NULL;
	};
}MONITSRV_CMN_DATA_CACHE;
/* ------------------------------------------------------------------------- */
typedef enum MONITSRV_EVENT_TYPE_T
{
	MONITSRV_EVENT_TYPE_OUTPUT_CHANGED=ADLIB_EVENT_NUM_END,
	MONITSRV_EVENT_TYPE_END,
	MONITSRV_EVENT_TYPE_NONE
}MONITSRV_EVENT_TYPE;
#define SERVER_JSON_PORT_NUM MONITSRV_JSON_PORT_NUMBER
#endif

