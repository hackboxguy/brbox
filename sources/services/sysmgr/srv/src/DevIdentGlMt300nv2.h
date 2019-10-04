#ifndef __DEVIDENT_GLMT300NV2_H_
#define __DEVIDENT_GLMT300NV2_H_
#include "DevIdent.h"
using namespace std;
class DevIdentGlMt300nv2: public DevIdent
{
public:
	DevIdentGlMt300nv2();
	~DevIdentGlMt300nv2();
	RPC_SRV_RESULT device_identify();//virtual override
};
#endif
