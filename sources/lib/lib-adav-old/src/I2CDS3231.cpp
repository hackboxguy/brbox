#include "I2CDS3231.hpp"
#include <unistd.h>
#include <string.h>
/*****************************************************************************/
I2CDS3231::I2CDS3231(std::string DevNode,std::string DevType):RTCDevice(DevNode)
{
}
I2CDS3231::~I2CDS3231()
{
}
/*****************************************************************************/
RPC_SRV_RESULT I2CDS3231::get_date_time(struct tm &time)
{
	return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;
}
RPC_SRV_RESULT I2CDS3231::set_date_time(struct tm time)
{
	return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;
}
/*****************************************************************************/

