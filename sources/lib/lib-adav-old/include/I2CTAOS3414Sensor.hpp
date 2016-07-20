//this code is based on GroveColorSensor: https://github.com/Seeed-Studio/Grove_I2C_Color_Sensor/blob/master/GroveColorSensor.h
#ifndef __I2C_TAOS_3414_SENSOR_H_
#define __I2C_TAOS_3414_SENSOR_H_
#include <stdint.h>
#include "I2CBusAccess.h"
#include "I2CTAOS3414Reg.h"
#include "LightSensor.hpp"
class I2CTAOS3414Sensor : public LightSensor
{
	uint8_t sensorI2CAddr;
	// Used for storing the colour data
	uint8_t readingdata_[8];
	int green_;
	int red_;
	int blue_;
	int clear_;
	int triggerMode_;	
	int interruptSource_;
	int interruptMode_;
	int gainAndPrescaler_;
	int sensorAddress_;
	// Set trigger mode. Including free mode, manually mode, single synchronization mode or so.
	void setTimingReg();
	// Set interrupt source
	void setInterruptSourceReg();
	// Set interrupt mode
	void setInterruptControlReg();
	// Set gain value and pre-scaler value
	void setGainReg();
	// Start ADC of the colour sensor
	void setEnableADC();
	void clearInterrupt();
	RPC_SRV_RESULT calculateCoordinate();
	void readRGB(int *red, int *green, int *blue);
	RPC_SRV_RESULT readRGB();

	RPC_SRV_RESULT setTimingReg(uint8_t val);
	RPC_SRV_RESULT getTimingReg(uint8_t &val);
	RPC_SRV_RESULT setInterruptSourceReg(uint8_t val);
	RPC_SRV_RESULT getInterruptSourceReg(uint8_t &val);
	RPC_SRV_RESULT setInterruptControlReg(uint8_t val);
	RPC_SRV_RESULT getInterruptControlReg(uint8_t &val);
	RPC_SRV_RESULT setGainReg(uint8_t val);
	RPC_SRV_RESULT getGainReg(uint8_t &val);
public:
	I2CTAOS3414Sensor(std::string DevNode,std::string DevType);
	~I2CTAOS3414Sensor();
	RPC_SRV_RESULT init_sensor();//{return RPC_SRV_RESULT_SUCCESS;};
	RPC_SRV_RESULT read_xyz(){return readRGB();};//
	RPC_SRV_RESULT trigger_measurement();
	RPC_SRV_RESULT get_integration_time(uint32_t &time);//{return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	RPC_SRV_RESULT set_integration_time(uint32_t time);//{return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	RPC_SRV_RESULT get_sync_edge(ADLIB_STATUS_FLAG_TYPE &edge);//{return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	RPC_SRV_RESULT set_sync_edge(ADLIB_STATUS_FLAG_TYPE edge);//{return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	RPC_SRV_RESULT get_integration_mode(LS_INTEG_MODE &mode);//{return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	RPC_SRV_RESULT set_integration_mode(LS_INTEG_MODE mode);//{return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	RPC_SRV_RESULT get_intr_stop_sts(ADLIB_STATUS_FLAG_TYPE &sts);//{return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	RPC_SRV_RESULT set_intr_stop_sts(ADLIB_STATUS_FLAG_TYPE sts);//{return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	RPC_SRV_RESULT get_intr_mode(LS_INTR_MODE &mode);//{return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	RPC_SRV_RESULT set_intr_mode(LS_INTR_MODE mode);//{return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	RPC_SRV_RESULT get_intr_rate(LS_INTR_RATE &rate);//{return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	RPC_SRV_RESULT set_intr_rate(LS_INTR_RATE rate);//{return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	RPC_SRV_RESULT get_intr_source(LS_INTR_SOURCE &src);//{return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	RPC_SRV_RESULT set_intr_source(LS_INTR_SOURCE src);//{return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	RPC_SRV_RESULT get_gain_mode(LS_GAIN_MODE &mode);//{return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	RPC_SRV_RESULT set_gain_mode(LS_GAIN_MODE mode);//{return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	RPC_SRV_RESULT get_prescaler(LS_PRESCALER &mode);//{return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	RPC_SRV_RESULT set_prescaler(LS_PRESCALER mode);//{return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;};
	RPC_SRV_RESULT get_rgbw_count(int32_t &red,int32_t &green,int32_t &blue,int32_t &white);
	int ledStatus;
};
#endif

