#ifndef __I2C_BUS_ACCESS_H_
#define __I2C_BUS_ACCESS_H_
#include <iostream>
#include "ADCommon.hpp"
#include <stdint.h>
using namespace std;
//#include "ADCmdlineHelper.hpp"
//#include "ADCmnStringProcessor.hpp"
class I2CBusAccess
{
	int fd;
	bool DevOpened;
	std::string node;
public:
	I2CBusAccess(std::string DevNode);
	~I2CBusAccess();
	RPC_SRV_RESULT SetSlaveAddr(int fd, uint8_t addr);
};
#endif
