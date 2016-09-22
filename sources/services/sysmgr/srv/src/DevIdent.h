#ifndef __DEVIDENT_H_
#define __DEVIDENT_H_
#include <iostream>
#include "ADCommon.hpp"
#include <stdint.h>
using namespace std;
class DevIdent
{
public:
	DevIdent();
	~DevIdent();
	virtual RPC_SRV_RESULT device_identify()=0;
};
#endif
