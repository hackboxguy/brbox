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
	EJSON_LIGHTSENSE_RPC_START_MEASURE,//READ_XYZ,
	EJSON_LIGHTSENSE_GET_INTEGRATION_TIME,
	EJSON_LIGHTSENSE_SET_INTEGRATION_TIME,
	EJSON_LIGHTSENSE_SYNC_EDGE_GET,
	EJSON_LIGHTSENSE_SYNC_EDGE_SET,
	EJSON_LIGHTSENSE_INTEGRATION_MODE_GET,
	EJSON_LIGHTSENSE_INTEGRATION_MODE_SET,
	EJSON_LIGHTSENSE_INTR_STOP_STS_GET,
	EJSON_LIGHTSENSE_INTR_STOP_STS_SET,
	EJSON_LIGHTSENSE_INTR_MODE_GET,//get interrupt-mode
	EJSON_LIGHTSENSE_INTR_MODE_SET,//set interrupt-mode
	EJSON_LIGHTSENSE_INTR_RATE_GET,
	EJSON_LIGHTSENSE_INTR_RATE_SET,
	EJSON_LIGHTSENSE_INTR_SOURCE_GET,
	EJSON_LIGHTSENSE_INTR_SOURCE_SET,
	EJSON_LIGHTSENSE_GAIN_MODE_GET,
	EJSON_LIGHTSENSE_GAIN_MODE_SET,
	EJSON_LIGHTSENSE_PRESCALER_GET,
	EJSON_LIGHTSENSE_PRESCALER_SET,
	EJSON_LIGHTSENSE_RGBWCOUNT_GET,
	EJSON_LIGHTSENSE_WAVELENGTH_COUNT_GET,
	EJSON_LIGHTSENSE_WAVELENGTH_ITEM_GET,
	EJSON_LIGHTSENSE_SPECTRUM_COUNT_GET,
	EJSON_LIGHTSENSE_SPECTRUM_ITEM_GET,
	EJSON_LIGHTSENSE_RPC_END,
	EJSON_LIGHTSENSE_RPC_NONE
}EJSON_LIGHTSENSE_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_RPC_SENSOR_INIT
#define	LIGHTSENSE_RPC_SENSOR_INIT         "sensor_init"
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_RPC_START_MEASURE
#define	LIGHTSENSE_RPC_START_MEASURE       "trigger_measurement"
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
//EJSON_LIGHTSENSE_INTEGRATION_MODE_GET,
//EJSON_LIGHTSENSE_INTEGRATION_MODE_SET,
#define	LIGHTSENSE_RPC_INTEGRATION_MODE_GET "get_integration_mode"
#define	LIGHTSENSE_RPC_INTEGRATION_MODE_SET "set_integration_mode"
#define	LIGHTSENSE_RPC_INTEGRATION_MODE_ARG "mode"
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_INTR_STOP_STS_GET,
//EJSON_LIGHTSENSE_INTR_STOP_STS_SET,
#define	LIGHTSENSE_RPC_INTR_STOP_STS_GET "get_intr_stop_sts"
#define	LIGHTSENSE_RPC_INTR_STOP_STS_SET "set_intr_stop_sts"
#define	LIGHTSENSE_RPC_INTR_STOP_STS_ARG "status"
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_INTR_MODE_GET,//get interrupt-mode
//EJSON_LIGHTSENSE_INTR_MODE_SET,//set interrupt-mode
#define	LIGHTSENSE_RPC_INTR_MODE_GET "get_intr_mode"
#define	LIGHTSENSE_RPC_INTR_MODE_SET "set_intr_mode"
#define	LIGHTSENSE_RPC_INTR_MODE_ARG "mode"
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_INTR_RATE_GET,
//EJSON_LIGHTSENSE_INTR_RATE_SET,
#define	LIGHTSENSE_RPC_INTR_RATE_GET "get_intr_rate"
#define	LIGHTSENSE_RPC_INTR_RATE_SET "set_intr_rate"
#define	LIGHTSENSE_RPC_INTR_RATE_ARG "rate"
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_INTR_SOURCE_GET,
//EJSON_LIGHTSENSE_INTR_SOURCE_SET,
#define	LIGHTSENSE_RPC_INTR_SOURCE_GET "get_intr_source"
#define	LIGHTSENSE_RPC_INTR_SOURCE_SET "set_intr_source"
#define	LIGHTSENSE_RPC_INTR_SOURCE_ARG "source"
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_GAIN_MODE_GET,
//EJSON_LIGHTSENSE_GAIN_MODE_SET,
#define	LIGHTSENSE_RPC_GAIN_MODE_GET "get_gain_mode"
#define	LIGHTSENSE_RPC_GAIN_MODE_SET "set_gain_mode"
#define	LIGHTSENSE_RPC_GAIN_MODE_ARG "mode"
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_PRESCALER_GET,
//EJSON_LIGHTSENSE_PRESCALER_SET,
#define	LIGHTSENSE_RPC_PRESCALER_GET "get_prescaler"
#define	LIGHTSENSE_RPC_PRESCALER_SET "set_prescaler"
#define	LIGHTSENSE_RPC_PRESCALER_ARG "value"
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_RGBWCOUNT_GET
#define	LIGHTSENSE_RPC_RGBWCOUNT_GET "get_rgbwcount"
#define	LIGHTSENSE_RPC_RGBWCOUNT_ARGR "R"
#define	LIGHTSENSE_RPC_RGBWCOUNT_ARGG "G"
#define	LIGHTSENSE_RPC_RGBWCOUNT_ARGB "B"
#define	LIGHTSENSE_RPC_RGBWCOUNT_ARGW "W"
/* ------------------------------------------------------------------------- */
//EJSON_LIGHTSENSE_WAVELENGTH_COUNT_GET,
//EJSON_LIGHTSENSE_WAVELENGTH_ITEM_GET,
//EJSON_LIGHTSENSE_SPECTRUM_COUNT_GET,
//EJSON_LIGHTSENSE_SPECTRUM_ITEM_GET,
#define	LIGHTSENSE_RPC_WAVELENGTH_COUNT_GET "get_wavelength_count"
#define	LIGHTSENSE_RPC_WAVELENGTH_ITEM_GET  "get_wavelength_item"
#define	LIGHTSENSE_RPC_SPECTRUM_COUNT_GET   "get_spectrum_count"
#define	LIGHTSENSE_RPC_SPECTRUM_ITEM_GET    "get_spectrum_item"
#define	LIGHTSENSE_RPC_COUNT_ARG            "count"
#define	LIGHTSENSE_RPC_ITEM_INDX_ARG        "index"
#define	LIGHTSENSE_RPC_ITEM_VAL_ARG         "value"
/* ------------------------------------------------------------------------- */
typedef struct LIGHTSENSE_MEASUREMENT_PACKET_T
{
	int32_t red;
	int32_t green;
	int32_t blue;
	int32_t white;
	uint32_t integration_time;
	ADLIB_STATUS_FLAG_TYPE sync_edge;
	LS_INTEG_MODE integration_mode;
	ADLIB_STATUS_FLAG_TYPE intr_stop_sts;
	LS_INTR_MODE intr_mode;
	LS_INTR_RATE intr_rate;
	LS_INTR_SOURCE intr_source;
	LS_GAIN_MODE gain_mode;
	LS_PRESCALER prescaler;
	uint32_t sample_count;//used for spectrometer
	uint32_t sample_index;//used for spectrometer
	double sample_value;//used for spectrometer
	char str_sample_value[255];
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

