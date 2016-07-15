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
	void setGain();
	// Start ADC of the colour sensor
	void setEnableADC();
	void clearInterrupt();
	RPC_SRV_RESULT calculateCoordinate();
	void readRGB(int *red, int *green, int *blue);
	RPC_SRV_RESULT readRGB();
public:
	I2CTAOS3414Sensor(std::string DevNode,std::string DevType);
	~I2CTAOS3414Sensor();
	RPC_SRV_RESULT init_sensor();//{return RPC_SRV_RESULT_SUCCESS;};
	RPC_SRV_RESULT read_xyz(){return readRGB();};//
	int ledStatus;
};
#endif

