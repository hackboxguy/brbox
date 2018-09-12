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
	EJSON_MODBUSSRV_RPC_ENRGYPARM_GET, //read energy parameter

	EJSON_MODBUSSRV_RPC_END,
	EJSON_MODBUSSRV_RPC_NONE
}EJSON_MODBUSSRV_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//EJSON_MODBUSSRV_RPC_ENRGYPARM_GET
#define MODBUSSRV_RPC_ENRGYPARM_GET      "modbus_energy_param_get"
#define MODBUSSRV_RPC_ENRGYPARM_ARG      "parameter"
#define MODBUSSRV_RPC_ENRGYPARM_ARG_TABL {"voltage","current","power","actpwr","reactpwr","pfactor","phangle",\
					  "freq","import_act","export_act","import_react","export_react",\
					  "total_act","total_react","unknown","none","\0"}

#define MODBUSSRV_RPC_ENRGYPARM_RESP_ARG "value"
typedef enum EJSON_ENRGYPARM_T
{
	EJSON_ENRGYPARM_VOLTAGE,
	EJSON_ENRGYPARM_CURRENT,
	EJSON_ENRGYPARM_POWER,
	EJSON_ENRGYPARM_POWER_ACT,
	EJSON_ENRGYPARM_POWER_REACT,
	EJSON_ENRGYPARM_PFACTOR,
	EJSON_ENRGYPARM_PHASE_ANGLE,
	EJSON_ENRGYPARM_FREQ,
	EJSON_ENRGYPARM_IMPORT_ACT,
	EJSON_ENRGYPARM_EXPORT_ACT,
	EJSON_ENRGYPARM_IMPORT_REACT,
	EJSON_ENRGYPARM_EXPORT_REACT,
	EJSON_ENRGYPARM_TOTAL_ACT,
	EJSON_ENRGYPARM_TOTAL_REACT,
	EJSON_ENRGYPARM_UNKNOWN,
	EJSON_ENRGYPARM_NONE
}EJSON_ENRGYPARM;
typedef struct MODBUSSRV_ENRGYPARM_ACCESS_PACKET_T
{
	EJSON_ENRGYPARM param_type;
	char parameter[255];
}MODBUSSRV_ENRGYPARM_ACCESS_PACKET;
/* ------------------------------------------------------------------------- */
//keep all the data related to modbus-server here
typedef struct MODBUSSRV_CMN_DATA_CACHE_T
{
	void *pDevInfo;//device-info-struct(typecast in rpc handlers)
	void *pEventNotifier;//event notifier object
	void *pDevAccess;//modbus device access node

	int energy_param;
	//DisplayDevice *pDisplay;
	MODBUSSRV_CMN_DATA_CACHE_T() //constructor(initializer)
	{
		pDevAccess=NULL;
		pDevInfo=NULL;
		pEventNotifier=NULL;
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

