#include "LightSensor.hpp"
using namespace std;
/*****************************************************************************/
LightSensor::LightSensor(std::string devnode):I2CBusAccess(devnode)
{
}
/*****************************************************************************/
LightSensor::~LightSensor()
{
}
/*****************************************************************************/

