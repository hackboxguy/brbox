#ifndef __LIGHT_SENSOR_H_
#define __LIGHT_SENSOR_H_
/*interface class for color and brightness sensors*/
#include <iostream>
#include "ADCommon.hpp"
#include <stdint.h>
#include "I2CBusAccess.h"
/*****************************************************************************/
typedef enum LIGHT_SENSOR_TYPE_T
{
	LIGHT_SENSOR_TYPE_TAOS3414,
	LIGHT_SENSOR_TYPE_OOSTS, //spectrometer
	LIGHT_SENSOR_TYPE_UNKNOWN,
	LIGHT_SENSOR_TYPE_NONE
}LIGHT_SENSOR_TYPE;
#define LIGHT_SENSOR_TYPE_TABL  {"taos3414","oosts","unknown","none","\0"}
/*****************************************************************************/
typedef enum LS_INTEG_MODE_T
{
	LS_INTEG_MODE_FREE,
	LS_INTEG_MODE_MANUAL,
	LS_INTEG_MODE_SYNC_SINGLE,
	LS_INTEG_MODE_SYNC_MULTI,
	LS_INTEG_MODE_UNKNOWN,
	LS_INTEG_MODE_NONE
}LS_INTEG_MODE;
#define LS_INTEG_MODE_TABL  {"free","manual","syncsingle","syncmulti","unknown","none","\0"}
/*****************************************************************************/
typedef enum LS_INTR_MODE_T
{
	LS_INTR_MODE_DISABLE,
	LS_INTR_MODE_LEVEL,
	LS_INTR_MODE_SMB_ALERT,
	LS_INTR_MODE_SMB_ALERT_SIMULATE,
	LS_INTR_MODE_UNKNOWN,
	LS_INTR_MODE_NONE
}LS_INTR_MODE;
#define LS_INTR_MODE_TABL  {"disable","level","smb","smbsimulate","unknown","none","\0"}
/*****************************************************************************/
typedef enum LS_INTR_RATE_T
{
	LS_INTR_RATE_EVERY,
	LS_INTR_RATE_SINGLE,
	LS_INTR_RATE_100MS,
	LS_INTR_RATE_SECOND,
	LS_INTR_RATE_UNKNOWN,
	LS_INTR_RATE_NONE
}LS_INTR_RATE;
#define LS_INTR_RATE_TABL  {"every","single","100ms","second","unknown","none","\0"}
/*****************************************************************************/
typedef enum LS_INTR_SOURCE_T
{
	LS_INTR_SOURCE_GREEN,
	LS_INTR_SOURCE_RED,
	LS_INTR_SOURCE_BLUE,
	LS_INTR_SOURCE_WHITE,
	LS_INTR_SOURCE_UNKNOWN,
	LS_INTR_SOURCE_NONE
}LS_INTR_SOURCE;
#define LS_INTR_SOURCE_TABL  {"green","red","blue","white","unknown","none","\0"}
/*****************************************************************************/
typedef enum LS_GAIN_MODE_T
{
	LS_GAIN_MODE_1,
	LS_GAIN_MODE_4,
	LS_GAIN_MODE_16,
	LS_GAIN_MODE_64,
	LS_GAIN_MODE_UNKNOWN,
	LS_GAIN_MODE_NONE
}LS_GAIN_MODE;
#define LS_GAIN_MODE_TABL  {"1","4","16","64","unknown","none","\0"}
/*****************************************************************************/
typedef enum LS_PRESCALER_T
{
	LS_PRESCALER_DIV1,
	LS_PRESCALER_DIV2,
	LS_PRESCALER_DIV4,
	LS_PRESCALER_DIV8,
	LS_PRESCALER_DIV16,
	LS_PRESCALER_DIV32,
	LS_PRESCALER_DIV64,
	LS_PRESCALER_UNKNOWN,
	LS_PRESCALER_NONE
}LS_PRESCALER;
#define LS_PRESCALER_TABL  {"div1","div2","div4","div8","div16","div32","div64","unknown","none","\0"}
/*****************************************************************************/
class LightSensor : public I2CBusAccess
{
public:
	LightSensor(std::string devnode);
	~LightSensor();
	virtual RPC_SRV_RESULT init_sensor()=0;//must-have function for derived class
	//virtual RPC_SRV_RESULT read_xyz()=0;
	virtual RPC_SRV_RESULT trigger_measurement()=0;//must-have function for derived class
	virtual RPC_SRV_RESULT get_integration_time(uint32_t &time){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	virtual RPC_SRV_RESULT set_integration_time(uint32_t time){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	//taos3414 specific functions
	virtual RPC_SRV_RESULT get_sync_edge(ADLIB_STATUS_FLAG_TYPE &edge){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	virtual RPC_SRV_RESULT set_sync_edge(ADLIB_STATUS_FLAG_TYPE edge){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	virtual RPC_SRV_RESULT get_integration_mode(LS_INTEG_MODE &mode){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	virtual RPC_SRV_RESULT set_integration_mode(LS_INTEG_MODE mode){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	virtual RPC_SRV_RESULT get_intr_stop_sts(ADLIB_STATUS_FLAG_TYPE &sts){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	virtual RPC_SRV_RESULT set_intr_stop_sts(ADLIB_STATUS_FLAG_TYPE sts){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	virtual RPC_SRV_RESULT get_intr_mode(LS_INTR_MODE &mode){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	virtual RPC_SRV_RESULT set_intr_mode(LS_INTR_MODE mode){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	virtual RPC_SRV_RESULT get_intr_rate(LS_INTR_RATE &rate){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	virtual RPC_SRV_RESULT set_intr_rate(LS_INTR_RATE rate){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	virtual RPC_SRV_RESULT get_intr_source(LS_INTR_SOURCE &src){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	virtual RPC_SRV_RESULT set_intr_source(LS_INTR_SOURCE src){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	virtual RPC_SRV_RESULT get_gain_mode(LS_GAIN_MODE &mode){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	virtual RPC_SRV_RESULT set_gain_mode(LS_GAIN_MODE mode){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	virtual RPC_SRV_RESULT get_prescaler(LS_PRESCALER &mode){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	virtual RPC_SRV_RESULT set_prescaler(LS_PRESCALER mode){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	virtual RPC_SRV_RESULT get_rgbw_count(int32_t &red,int32_t &green,int32_t &blue,int32_t &white){return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};

};
#endif
