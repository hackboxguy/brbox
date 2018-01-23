#include "SeaBreezeAccess.h"
//#include "DispAccess.h"
//#include "ADEvntNotifier.hpp"//for eventing
/* ------------------------------------------------------------------------- */
SeaBreezeAccess::SeaBreezeAccess(std::string DevNode,std::string DevType):LightSensor(DevNode)
{
	//pDataCache=pData;

	sbapi_initialize();
	sbapi_probe_devices();
	//number_of_devices = sbapi_get_number_of_device_ids();
	//printf("Device count is %d\n", number_of_devices);
	//if(0 == number_of_devices) {
	//return 0;
	//}

}
/* ------------------------------------------------------------------------- */
SeaBreezeAccess::~ SeaBreezeAccess()
{
}
/* ------------------------------------------------------------------------- */

