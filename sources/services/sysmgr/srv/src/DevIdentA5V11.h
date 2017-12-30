#ifndef __DEVIDENT_A5V11_H_
#define __DEVIDENT_A5V11_H_
#include "DevIdent.h"
using namespace std;
class DevIdentA5V11: public DevIdent
{
public:
	DevIdentA5V11();
	~DevIdentA5V11();
	RPC_SRV_RESULT device_identify();//virtual override
};
#endif
