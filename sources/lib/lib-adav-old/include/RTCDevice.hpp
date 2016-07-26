#ifndef __RTCDEVICE_H_
#define __RTCDEVICE_H_
/*interface class for real-time-clock-calander device*/
#include <iostream>
#include <time.h>
#include "ADCommon.hpp"
#include <stdint.h>
#include "I2CBusAccess.h"
/*****************************************************************************/
class RTCDevice : public I2CBusAccess
{
public:
	RTCDevice(std::string devnode);
	~RTCDevice();
	virtual RPC_SRV_RESULT get_date_time(struct tm &time)=0;
	virtual RPC_SRV_RESULT set_date_time(struct tm time)=0;
};
#endif
