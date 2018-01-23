//this file is for seabreeze based spectrometer access
#ifndef __SEABREEZE_ACCESS_H_
#define __SEABREEZE_ACCESS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <api/seabreezeapi/SeaBreezeAPI.h>
#include <unistd.h>
#include <stdint.h>
#include "LightSensor.hpp"

//#include "ADJsonRpcMgr.hpp"
//#include "LightsenseJsonDef.h"
//#include "ADCmnStringProcessor.hpp"
/* ------------------------------------------------------------------------- */
class  SeaBreezeAccess : public LightSensor
{
public:
	SeaBreezeAccess(std::string DevNode,std::string DevType);
	~SeaBreezeAccess();

	// SeaBreezeAccess();
	//~ SeaBreezeAccess();
	RPC_SRV_RESULT init_sensor(){return RPC_SRV_RESULT_SUCCESS;};
	//RPC_SRV_RESULT read_xyz(){return readRGB();};//
	RPC_SRV_RESULT trigger_measurement(){return RPC_SRV_RESULT_SUCCESS;};
};
/* ------------------------------------------------------------------------- */
#endif
