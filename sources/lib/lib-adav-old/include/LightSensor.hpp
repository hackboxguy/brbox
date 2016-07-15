#ifndef __LIGHT_SENSOR_H_
#define __LIGHT_SENSOR_H_
/*interface class for color and brightness sensors*/
#include <iostream>
#include "ADCommon.hpp"
#include <stdint.h>
#include "I2CBusAccess.h"
typedef enum LIGHT_SENSOR_TYPE_T
{
	LIGHT_SENSOR_TYPE_TAOS3414,
	LIGHT_SENSOR_TYPE_OOSTS, //spectrometer
	LIGHT_SENSOR_TYPE_UNKNOWN,
	LIGHT_SENSOR_TYPE_NONE
}LIGHT_SENSOR_TYPE;
#define LIGHT_SENSOR_TYPE_TABL  {"taos3414","oosts","unknown","none","\0"}

class LightSensor : public I2CBusAccess
{
public:
	LightSensor(std::string devnode);
	~LightSensor();
	virtual RPC_SRV_RESULT init_sensor()=0;
	virtual RPC_SRV_RESULT read_xyz()=0;
};
#endif
