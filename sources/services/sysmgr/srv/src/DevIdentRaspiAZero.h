#ifndef __DEVIDENT_RASPIAZERO_H_
#define __DEVIDENT_RASPIAZERO_H_
//#include <iostream>
//#include "ADCommon.hpp"
//#include <stdint.h>
#include "DevIdent.h"
using namespace std;
class DevIdentRaspiAZero: public DevIdent
{
public:
	DevIdentRaspiAZero();
	~DevIdentRaspiAZero();
	RPC_SRV_RESULT device_identify();//virtual override
};
#endif
