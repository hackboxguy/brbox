#ifndef __I2C_DS3231_H_
#define __I2C_DS3231_H_
#include <stdint.h>
#include "I2CBusAccess.h"
#include "RTCDevice.hpp"
class I2CDS3231 : public RTCDevice
{
public:
	I2CDS3231(std::string DevNode,std::string DevType);
	~I2CDS3231();
	RPC_SRV_RESULT get_date_time(struct tm &time);
	RPC_SRV_RESULT set_date_time(struct tm time);
};
#endif

