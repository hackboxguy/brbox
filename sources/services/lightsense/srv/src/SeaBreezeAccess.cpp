#include "SeaBreezeAccess.h"
//#include "DispAccess.h"
//#include "ADEvntNotifier.hpp"//for eventing
/* ------------------------------------------------------------------------- */
SeaBreezeAccess::SeaBreezeAccess(std::string DevNode,std::string DevType):LightSensor(DevNode)
{
	SensorInitialized=false;
	DeviceID=0;
	SensorNodeOpen=false;
}
/* ------------------------------------------------------------------------- */
SeaBreezeAccess::~ SeaBreezeAccess()
{
	int error = 0;
	if(SensorNodeOpen)
		sbapi_close_device(DeviceID, &error);
	if(SensorInitialized)
		sbapi_shutdown();
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT SeaBreezeAccess::init_sensor()
{
	if(SensorInitialized)
		return RPC_SRV_RESULT_SUCCESS;

	sbapi_initialize();
	sbapi_probe_devices();
	int number_of_devices = sbapi_get_number_of_device_ids();
	if(number_of_devices<1)
			return RPC_SRV_RESULT_FAIL;

	number_of_devices = sbapi_get_device_ids(&DeviceID, 1);
	printf("SeaBreezeAccess::init_sensor: DeviceID      = 0x%02lX\n", DeviceID);

	int error = 0;
	int flag = sbapi_get_device_type(DeviceID, &error, DeviceType, 79);
	if(flag > 0)
		printf("SeaBreezeAccess::init_sensor: DeviceType    = %s\n", DeviceType);
	else
		printf("SeaBreezeAccess::init_sensor: DeviceType    = NotFound!!!!\n");
	//SeaBreezeAccess::init_sensor: DeviceID   = 0x02
	//SeaBreezeAccess::init_sensor: DeviceType = STS

	if(sbapi_open_device(DeviceID, &error)==0)
		SensorNodeOpen=true;

	if(get_device_serial_num(DeviceID,DeviceSerialNum,DEVICE_SERIAL_NUM_MAX_LEN)==RPC_SRV_RESULT_SUCCESS)
		printf("SeaBreezeAccess::init_sensor: DeviceSerial  = %s\n",DeviceSerialNum);
	
	if(get_device_revisions(DeviceID,&HwRevision,&FmwRevision)==RPC_SRV_RESULT_SUCCESS)
	{
		printf("SeaBreezeAccess::init_sensor: HwRevision    = 0x%X\n",HwRevision);
		printf("SeaBreezeAccess::init_sensor: FmwRevision   = 0x%X\n",FmwRevision);
	}

	get_optical_features(DeviceID,&BenchProps);

	SensorInitialized=true;
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT SeaBreezeAccess::get_device_serial_num(long DevID,char* SerialNum,int MaxLen)
{
	int error;
	int Length=0;
	long SerialNumID;

	if(sbapi_get_number_of_serial_number_features(DevID, &error)<1)
	{
		printf("SeaBreezeAccess::get_device_serial_num: Not Found!!!\n");
		return RPC_SRV_RESULT_FAIL;
	}

	sbapi_get_serial_number_features(DevID, &error,&SerialNumID, 1);
	Length=sbapi_get_serial_number_maximum_length(DevID, SerialNumID, &error);
	if(Length>=MaxLen)
		return RPC_SRV_RESULT_FAIL;
	SerialNum[0]='\0';SerialNum[MaxLen-1]='\0';//just to be safe
	sbapi_get_serial_number(DevID, SerialNumID, &error, SerialNum, Length);
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT SeaBreezeAccess::get_device_revisions(long DevID,unsigned int* HwRev,unsigned int* FwRev)
{
	int error;
	long RevID;

	if(sbapi_get_number_of_revision_features(DevID, &error)<1)
	{
		printf("SeaBreezeAccess::get_device_revisions: Not Found!!!\n");
		return RPC_SRV_RESULT_FAIL;
	}
	sbapi_get_revision_features(DevID, &error, &RevID,1);

	*HwRev=sbapi_revision_hardware_get(DevID, RevID, &error);
	*FwRev=sbapi_revision_firmware_get(DevID, RevID, &error);

	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT SeaBreezeAccess::get_optical_features(long DevID,OpticalBenchProps *pProps)
{
	int error;
	long OpticalID;

	if(sbapi_get_number_of_optical_bench_features(DevID, &error)<1)
	{
		printf("SeaBreezeAccess::get_optical_features: Not Found!!!\n");
		return RPC_SRV_RESULT_FAIL;
	}
	sbapi_get_optical_bench_features(DevID, &error, &OpticalID,1);

	pProps->BenchID[0]='\0';pProps->BenchID[79]='\0';
	sbapi_optical_bench_get_id(DevID, OpticalID, &error, pProps->BenchID, 79);
	sbapi_optical_bench_get_serial_number(DevID, OpticalID, &error, pProps->BenchSerialNum, 79);
	sbapi_optical_bench_get_coating(DevID, OpticalID, &error, pProps->BenchCoating, 79);
	sbapi_optical_bench_get_filter(DevID, OpticalID, &error, pProps->BenchFilter, 79);
	sbapi_optical_bench_get_grating(DevID, OpticalID, &error, pProps->BenchGrating, 79);
	pProps->SlitWidth=sbapi_optical_bench_get_slit_width_microns(DevID, OpticalID, &error);
	pProps->FiberDia=sbapi_optical_bench_get_fiber_diameter_microns(DevID, OpticalID, &error);

	printf("SeaBreezeAccess::get_optprop: BenchID       = %s\n",pProps->BenchID);
	printf("SeaBreezeAccess::get_optprop: BenchSerial   = %s\n",pProps->BenchSerialNum);
	printf("SeaBreezeAccess::get_optprop: BenchCoating  = %s\n",pProps->BenchCoating);
	printf("SeaBreezeAccess::get_optprop: BenchFilter   = %s\n",pProps->BenchFilter);
	printf("SeaBreezeAccess::get_optprop: BenchGrating  = %s\n",pProps->BenchGrating);
	printf("SeaBreezeAccess::get_optprop: BenchSlitWidth= %d\n",pProps->SlitWidth);
	printf("SeaBreezeAccess::get_optprop: BenchFiberDia = %d\n",pProps->FiberDia);
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */




