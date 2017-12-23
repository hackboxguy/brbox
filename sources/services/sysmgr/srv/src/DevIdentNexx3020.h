#ifndef __DEVIDENT_NEXX3020_H_
#define __DEVIDENT_NEXX3020_H_
#include "DevIdent.h"
using namespace std;
class DevIdentNexx3020: public DevIdent
{
public:
	DevIdentNexx3020();
	~DevIdentNexx3020();
	RPC_SRV_RESULT device_identify();//virtual override
};
#endif
