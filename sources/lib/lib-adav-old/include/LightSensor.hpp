#ifndef __LIGHT_SENSOR_H_
#define __LIGHT_SENSOR_H_
/*interface class for color and brightness sensors*/
#include <iostream>
#include "ADCommon.hpp"
#include <stdint.h>
#include "I2CBusAccess.h"
class LightSensor : public I2CBusAccess
{
public:
	LightSensor(std::string devnode);
	~LightSensor();
	virtual RPC_SRV_RESULT init_sensor()=0;
};
#endif
