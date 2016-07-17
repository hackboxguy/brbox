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
	EJSON_LIGHTSENSE_RPC_READ_XYZ,
	EJSON_LIGHTSENSE_GET_INTEGRATION_TIME,
	EJSON_LIGHTSENSE_SET_INTEGRATION_TIME,
	EJSON_LIGHTSENSE_SYNC_EDGE_GET,
	EJSON_LIGHTSENSE_SYNC_EDGE_SET,
	EJSON_LIGHTSENSE_GET_INTEGRATION_MODE,
	EJSON_LIGHTSENSE_SET_INTEGRATION_MODE,
	EJSON_LIGHTSENSE_GET_INTR_STOP_STS,
	EJSON_LIGHTSENSE_SET_INTR_STOP_STS,
	EJSON_LIGHTSENSE_GET_INTR_MODE,//get interrupt-mode
	EJSON_LIGHTSENSE_SET_INTR_MODE,//set interrupt-mode
	EJSON_LIGHTSENSE_GET_INTR_RATE,
	EJSON_LIGHTSENSE_SET_INTR_RATE,
	EJSON_LIGHTSENSE_GET_INTR_SOURCE,
	EJSON_LIGHTSENSE_SET_INTR_SOURCE,
	EJSON_LIGHTSENSE_GET_GAIN_MODE,
	EJSON_LIGHTSENSE_SET_GAIN_MODE,
	EJSON_LIGHTSENSE_GET_PRESCALER,
	EJSON_LIGHTSENSE_SET_PRESCALER,
	EJSON_LIGHTSENSE_RPC_END,
	EJSON_LIGHTSENSE_RPC_NONE
}EJSON_LIGHTSENSE_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_RPC_DISP_INIT
#define	LIGHTSENSE_RPC_SENSOR_INIT         "sensor_init"
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_RPC_READ_XYZ
#define	LIGHTSENSE_RPC_READ_XYZ            "read_xyz"
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_GET_INTEGRATION_TIME,
//EJSON_LIGHTSENSE_SET_INTEGRATION_TIME,
#define	LIGHTSENSE_RPC_GET_INTEGRATION_TIME      "get_integration_time"
#define	LIGHTSENSE_RPC_SET_INTEGRATION_TIME      "set_integration_time"
#define LIGHTSENSE_RPC_SET_INTEGRATION_TIME_ARG  "time"
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_SYNC_EDGE_GET,
//EJSON_LIGHTSENSE_SYNC_EDGE_SET,
#define	LIGHTSENSE_RPC_SYNC_EDGE_GET  "get_sync_edge"
#define	LIGHTSENSE_RPC_SYNC_EDGE_SET  "set_sync_edge"
#define LIGHTSENSE_RPC_SYNC_EDGE_ARG  "edge"
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_GET_INTEGRATION_MODE,
//EJSON_LIGHTSENSE_SET_INTEGRATION_MODE,
#define	LIGHTSENSE_RPC_GET_INTEGRATION_MODE "get_integration_mode"
#define	LIGHTSENSE_RPC_SET_INTEGRATION_MODE "set_integration_mode"
#define	LIGHTSENSE_RPC_SET_INTEGRATION_MODE_ARG "mode"
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_GET_INTR_STOP_STS,
//EJSON_LIGHTSENSE_SET_INTR_STOP_STS,
#define	LIGHTSENSE_RPC_GET_INTR_STOP_STS "get_intr_stop_sts"
#define	LIGHTSENSE_RPC_SET_INTR_STOP_STS "set_intr_stop_sts"
#define	LIGHTSENSE_RPC_SET_INTR_STOP_STS_ARG "status"
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_GET_INTR_MODE,//get interrupt-mode
//EJSON_LIGHTSENSE_SET_INTR_MODE,//set interrupt-mode
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_GET_INTR_RATE,
//EJSON_LIGHTSENSE_SET_INTR_RATE,
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_GET_INTR_SOURCE,
//EJSON_LIGHTSENSE_SET_INTR_SOURCE,
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_GET_GAIN_MODE,
//EJSON_LIGHTSENSE_SET_GAIN_MODE,
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_GET_PRESCALER,
//EJSON_LIGHTSENSE_SET_PRESCALER,
/* ------------------------------------------------------------------------- */
typedef struct LIGHTSENSE_MEASUREMENT_PACKET_T
{
	uint16_t red;
	uint16_t green;
	uint16_t blue;
	uint32_t integration_time;
	ADLIB_STATUS_FLAG_TYPE sync_edge;
	LS_INTEG_MODE integration_mode;
	ADLIB_STATUS_FLAG_TYPE intr_stop_sts;
	LS_INTR_MODE intr_mode;
	LS_INTR_RATE intr_rate;
	LS_INTR_SOURCE intr_source;
	LS_GAIN_MODE gain_mode;
	LS_PRESCALER prescaler;
}LIGHTSENSE_MEASUREMENT_PACKET;
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

