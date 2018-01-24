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
#define DEVICE_SERIAL_NUM_MAX_LEN 255
//#include "ADJsonRpcMgr.hpp"
//#include "LightsenseJsonDef.h"
//#include "ADCmnStringProcessor.hpp"
struct OpticalBenchProps
{
	char BenchID[80];
	char BenchSerialNum[80];
	char BenchCoating[80];
	char BenchFilter[80];
	char BenchGrating[80];
	int SlitWidth; 
	int FiberDia;
//public:
//	OpticalBenchProps();
};

/* ------------------------------------------------------------------------- */
class  SeaBreezeAccess : public LightSensor
{
	bool SensorInitialized;
	bool SensorNodeOpen;
	long DeviceID;
	char DeviceType[80];
	char DeviceSerialNum[DEVICE_SERIAL_NUM_MAX_LEN];
	OpticalBenchProps BenchProps;

	unsigned int HwRevision,FmwRevision;
	RPC_SRV_RESULT get_device_serial_num(long DevID,char* SerialNum,int MaxLen);
	RPC_SRV_RESULT get_device_revisions(long DevID,unsigned int* HwRev,unsigned int* FwRev);
	RPC_SRV_RESULT get_optical_features(long DevID,OpticalBenchProps *pProps);

public:
	SeaBreezeAccess(std::string DevNode,std::string DevType);
	~SeaBreezeAccess();

	// SeaBreezeAccess();
	//~ SeaBreezeAccess();
	RPC_SRV_RESULT init_sensor();//{return RPC_SRV_RESULT_SUCCESS;};
	//RPC_SRV_RESULT read_xyz(){return readRGB();};//
	RPC_SRV_RESULT trigger_measurement(){return RPC_SRV_RESULT_SUCCESS;};
};
/* ------------------------------------------------------------------------- */
#endif
