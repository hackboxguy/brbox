#include "SeaBreezeAccess.h"
//#include "DispAccess.h"
//#include "ADEvntNotifier.hpp"//for eventing
/* ------------------------------------------------------------------------- */
SeaBreezeAccess::SeaBreezeAccess(std::string DevNode,std::string DevType):LightSensor(DevNode)
{
	SensorInitialized=false;
	DeviceID=0;
	SensorNodeOpen=false;
	TrigMode=LS_TRIG_MODE_UNKNOWN;
	spectroIdentified=false;
	IntegrationTime=100000;//default integration time is 100ms
}
/* ------------------------------------------------------------------------- */
SeaBreezeAccess::~ SeaBreezeAccess()
{
	int error = 0;
	if(SensorNodeOpen)
		sbapi_close_device(DeviceID, &error);
	if(SensorInitialized)
		sbapi_shutdown();

	FormattedSpectrum.clear();

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

	if(identify_spectrometer(DeviceID,SpectroID)==RPC_SRV_RESULT_SUCCESS)
		spectroIdentified=true;

	//set trigger-mode
	if(set_trigger_mode(DeviceID,SpectroID,LS_TRIG_MODE_NORMAL)!=RPC_SRV_RESULT_SUCCESS)
		return RPC_SRV_RESULT_FAIL;

	//read the wavelength table
	if(read_wavelength(DeviceID,SpectroID,&WaveLength)!=RPC_SRV_RESULT_SUCCESS)
		return RPC_SRV_RESULT_FAIL;


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
RPC_SRV_RESULT SeaBreezeAccess::identify_spectrometer(long DevID,long &spectro_id)
{
	long mySpectro;
	int error;
	if(sbapi_get_number_of_spectrometer_features(DevID, &error)<1)
	{
		printf("SeaBreezeAccess::identify_spectrometer: spectro feature not Found!!!\n");
		return RPC_SRV_RESULT_FAIL;
	}
	if(sbapi_get_spectrometer_features(DevID, &error, &mySpectro,1)<1)
	{
		printf("SeaBreezeAccess::identify_spectrometer: spectro not Found!!!\n");
		return RPC_SRV_RESULT_FAIL;
	}
	spectro_id=mySpectro;
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT SeaBreezeAccess::get_trigger_mode(long DevID,LS_TRIG_MODE &mode)
{
	mode=TrigMode;
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT SeaBreezeAccess::set_trigger_mode(long DevID,long SpectroID,LS_TRIG_MODE mode)
{
	int error;
	if(mode!=LS_TRIG_MODE_NORMAL)//dont support anything other than mode-normal
		return RPC_SRV_RESULT_FEATURE_UNSUPPORTED;
	/*long SpectroID;
	if(sbapi_get_number_of_spectrometer_features(DevID, &error)<1)
	{
		printf("SeaBreezeAccess::set_trigger_mode: feature numbers not Found!!!\n");
		return RPC_SRV_RESULT_FAIL;
	}
	if(sbapi_get_spectrometer_features(DevID, &error, &SpectroID,1)<1)
	{
		printf("SeaBreezeAccess::set_trigger_mode: feature not Found!!!\n");
		return RPC_SRV_RESULT_FAIL;
	}*/
	
        sbapi_spectrometer_set_trigger_mode(DevID,SpectroID, &error, 0);//LS_TRIG_MODE_NORMAL);
	TrigMode=LS_TRIG_MODE_NORMAL;
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT SeaBreezeAccess::get_max_intensity(long DevID,long SpectroID,double &intensity)
{
	int error;
	/*long SpectroID;
	if(sbapi_get_number_of_spectrometer_features(DevID, &error)<1)
	{
		printf("SeaBreezeAccess::get_max_intensity: feature numbers not Found!!!\n");
		return RPC_SRV_RESULT_FAIL;
	}
	if(sbapi_get_spectrometer_features(DevID, &error, &SpectroID,1)<1)
	{
		printf("SeaBreezeAccess::get_max_intensity: feature not Found!!!\n");
		return RPC_SRV_RESULT_FAIL;
	}*/
        intensity=sbapi_spectrometer_get_maximum_intensity(DevID,SpectroID, &error);
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT SeaBreezeAccess::get_min_integration_time(long DevID,long SpectroID,long &time)
{
	int error;
	/*long SpectroID;
	if(sbapi_get_number_of_spectrometer_features(DevID, &error)<1)
	{
		printf("SeaBreezeAccess::get_min_integration_time: feature numbers not Found!!!\n");
		return RPC_SRV_RESULT_FAIL;
	}
	if(sbapi_get_spectrometer_features(DevID, &error, &SpectroID,1)<1)
	{
		printf("SeaBreezeAccess::get_min_integration_time: feature not Found!!!\n");
		return RPC_SRV_RESULT_FAIL;
	}*/
        time=sbapi_spectrometer_get_minimum_integration_time_micros(DevID,SpectroID, &error);
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT SeaBreezeAccess::set_integration_time(long DevID,long SpectroID,long time)
{
	int error;
	/*long SpectroID;
	if(sbapi_get_number_of_spectrometer_features(DevID, &error)<1)
	{
		printf("SeaBreezeAccess::set_integration_time: feature numbers not Found!!!\n");
		return RPC_SRV_RESULT_FAIL;
	}
	if(sbapi_get_spectrometer_features(DevID, &error, &SpectroID,1)<1)
	{
		printf("SeaBreezeAccess::set_integration_time: feature not Found!!!\n");
		return RPC_SRV_RESULT_FAIL;
	}*/
        sbapi_spectrometer_set_integration_time_micros(DevID,SpectroID, &error,time);
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT SeaBreezeAccess::read_formatted_spectrum(long DevID,long SpectroID,std::deque<double> *pList)
{
	int length;
	double *doubleBuffer = 0;
	int error;
	/*long SpectroID;
	if(sbapi_get_number_of_spectrometer_features(DevID, &error)<1)
	{
		printf("SeaBreezeAccess::read_formatted_spectrum: feature numbers not Found!!!\n");
		return RPC_SRV_RESULT_FAIL;
	}
	if(sbapi_get_spectrometer_features(DevID, &error, &SpectroID,1)<1)
	{
		printf("SeaBreezeAccess::read_formatted_spectrum: feature not Found!!!\n");
		return RPC_SRV_RESULT_FAIL;
	}*/

	length = sbapi_spectrometer_get_formatted_spectrum_length(DevID,SpectroID, &error);
	if(length>10000) //max allowed spectrum array is 10k
		return RPC_SRV_RESULT_FAIL;
	if(length<1) //no spectrum is available
		return RPC_SRV_RESULT_FAIL;

	doubleBuffer = (double *)calloc(length, sizeof(double));
	length = sbapi_spectrometer_get_formatted_spectrum(DevID,SpectroID,&error,doubleBuffer,length);

	pList->clear();
	for(int i=0;i<length;i++)
		pList->push_back (doubleBuffer[i]);

	free(doubleBuffer);
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT SeaBreezeAccess::read_unformatted_spectrum(long DevID,long SpectroID,std::deque<unsigned char> *pList,std::deque<double> *pWList)
{
	int length;
	double *doubleBuffer = 0;
	unsigned char *byteBuffer = 0;

	int error;

	length = sbapi_spectrometer_get_unformatted_spectrum_length(DevID,SpectroID, &error);
	if(length>10000) //max allowed spectrum array is 10k
		return RPC_SRV_RESULT_FAIL;
	if(length<1) //no spectrum is available
		return RPC_SRV_RESULT_FAIL;

	//read unformatter spectrum array
	byteBuffer = (unsigned char *)calloc(length, sizeof(unsigned char));
	length = sbapi_spectrometer_get_unformatted_spectrum(DevID,SpectroID,&error,byteBuffer,length);
	pList->clear();
	for(int i=0;i<length;i++)
		pList->push_back (byteBuffer[i]);
	free(byteBuffer);

	//read wavelength array
	doubleBuffer = (double *)calloc(length, sizeof(double));
	length = sbapi_spectrometer_get_wavelengths(DevID,SpectroID,&error,doubleBuffer,length);
	pWList->clear();
	for(int j=0;j<length;j++)
		pWList->push_back (doubleBuffer[j]);
	free(doubleBuffer);

	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT SeaBreezeAccess::read_wavelength(long DevID,long SpectroID,std::deque<double> *pWList)
{
	int length;
	double *doubleBuffer = 0;
	unsigned char *byteBuffer = 0;
	int error;

	length = sbapi_spectrometer_get_formatted_spectrum_length(DevID,SpectroID, &error);
	if(length>10000) //max allowed spectrum array is 10k
		return RPC_SRV_RESULT_FAIL;
	if(length<1) //no spectrum is available
		return RPC_SRV_RESULT_FAIL;

	//read wavelength array
	doubleBuffer = (double *)calloc(length, sizeof(double));
	length = sbapi_spectrometer_get_wavelengths(DevID,SpectroID,&error,doubleBuffer,length);
	pWList->clear();
	for(int j=0;j<length;j++)
		pWList->push_back (doubleBuffer[j]);
	free(doubleBuffer);

	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT SeaBreezeAccess::trigger_measurement()
{
	if(spectroIdentified==false || SensorInitialized==false)
		return RPC_SRV_RESULT_DEV_NOT_ACCESSIBLE;

	//set trigger-mode
	//if(set_trigger_mode(DeviceID,SpectroID,LS_TRIG_MODE_NORMAL)!=RPC_SRV_RESULT_SUCCESS)
	//	return RPC_SRV_RESULT_FAIL;

	//read the wavelength table
	//if(read_wavelength(DeviceID,SpectroID,&WaveLength)!=RPC_SRV_RESULT_SUCCESS)
	//	return RPC_SRV_RESULT_FAIL;


	//if(get_max_intensity(DeviceID,SpectroID,MaxIntensity)!=RPC_SRV_RESULT_SUCCESS)
	//	return RPC_SRV_RESULT_FAIL;
	//long IntgrTime;
	//if(get_min_integration_time(DeviceID,SpectroID,IntgrTime)!=RPC_SRV_RESULT_SUCCESS)
	//	return RPC_SRV_RESULT_FAIL;

	//set integration time
	if(set_integration_time(DeviceID,SpectroID,IntegrationTime)!=RPC_SRV_RESULT_SUCCESS)
		return RPC_SRV_RESULT_FAIL;

	if(read_formatted_spectrum(DeviceID,SpectroID,&FormattedSpectrum)!=RPC_SRV_RESULT_SUCCESS)
		return RPC_SRV_RESULT_FAIL;

	//if(read_unformatted_spectrum(DeviceID,SpectroID,&UnformattedSpectrum,&WaveLength)!=RPC_SRV_RESULT_SUCCESS)
	//	return RPC_SRV_RESULT_FAIL;
	//for(int i=0;i<500;i++)
	//	printf("%d\n",UnformattedSpectrum.at(i));//FormattedSpectrum.at(i));

	//for(int i=0;i<500;i++)
	//	printf("%lf\n",FormattedSpectrum.at(i));//FormattedSpectrum.at(i));

	//printf("trigger_measurement: success\n");
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
//sequence:
//int pixels = seabreeze_get_formatted_spectrum_length(index, &error);
//seabreeze_set_integration_time_microsec(index, &error, gArgs.integrationTimeUS);
//seabreeze_get_formatted_spectrum(index, &error, spectrum, pixels);
//seabreeze_get_formatted_spectrum(index, &error, spectrum, pixels);
RPC_SRV_RESULT SeaBreezeAccess::get_integration_time(uint32_t &time)
{
	time=IntegrationTime;
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT SeaBreezeAccess::set_integration_time(uint32_t time)
{
	IntegrationTime=time;
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT SeaBreezeAccess::get_spectrum_count(uint32_t &count)
{
	count=FormattedSpectrum.size();
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT SeaBreezeAccess::get_spectrum_value(uint32_t index,double &value)
{
	if(index>=FormattedSpectrum.size())
		return RPC_SRV_RESULT_ARG_ERROR;
	value=FormattedSpectrum.at(index);
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT SeaBreezeAccess::get_wavelength_count(uint32_t &count)
{
	count=WaveLength.size();
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT SeaBreezeAccess::get_wavelength_value(uint32_t index,double &value)
{
	if(index>=WaveLength.size())
		return RPC_SRV_RESULT_ARG_ERROR;
	value=WaveLength.at(index);
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
/*TODO:
SeaBreezeAccess::init_sensor: DeviceID      = 0x02
SeaBreezeAccess::init_sensor: DeviceType    = STS
SeaBreezeAccess::init_sensor: DeviceSerial  = S02700
SeaBreezeAccess::init_sensor: HwRevision    = 0x5
SeaBreezeAccess::init_sensor: FmwRevision   = 0x43
SeaBreezeAccess::get_optprop: BenchID       = 
SeaBreezeAccess::get_optprop: BenchSerial   = 
SeaBreezeAccess::get_optprop: BenchCoating  = OOI
SeaBreezeAccess::get_optprop: BenchFilter   = 305
SeaBreezeAccess::get_optprop: BenchGrating  = VIS
SeaBreezeAccess::get_optprop: BenchSlitWidth= 0
SeaBreezeAccess::get_optprop: BenchFiberDia = 0
*/






