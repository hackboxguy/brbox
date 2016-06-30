#include "DisplayDevice.hpp"
using namespace std;
/*****************************************************************************/
DisplayDevice::DisplayDevice(std::string devnode):I2CBusAccess(devnode)
{
	//DispType=disptype;
	//DispOpened=init_display();
}
/*****************************************************************************/
DisplayDevice::~DisplayDevice()
{
}
/*****************************************************************************/

