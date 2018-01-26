//this file is for seabreeze based spectrometer access
#ifndef __SEABREEZE_ACCESS_H_
#define __SEABREEZE_ACCESS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <api/seabreezeapi/SeaBreezeAPI.h>
#include <unistd.h>
#include <stdint.h>
#include <deque>
#include "LightSensor.hpp"
#define DEVICE_SERIAL_NUM_MAX_LEN 255
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
	long SpectroID;
	bool spectroIdentified;
	char DeviceType[80];
	char DeviceSerialNum[DEVICE_SERIAL_NUM_MAX_LEN];
	OpticalBenchProps BenchProps;
	LS_TRIG_MODE TrigMode;
	std::deque<double> FormattedSpectrum;//fifo for storing spectrometer spectrum
	std::deque<double> WaveLength;
	std::deque<unsigned char> UnformattedSpectrum;
	double MaxIntensity;
	uint32_t IntegrationTime;

	unsigned int HwRevision,FmwRevision;
	RPC_SRV_RESULT get_device_serial_num(long DevID,char* SerialNum,int MaxLen);
	RPC_SRV_RESULT get_device_revisions(long DevID,unsigned int* HwRev,unsigned int* FwRev);
	RPC_SRV_RESULT get_optical_features(long DevID,OpticalBenchProps *pProps);
	RPC_SRV_RESULT identify_spectrometer(long DevID,long &spectro_id);
	RPC_SRV_RESULT get_trigger_mode(long DevID,LS_TRIG_MODE &mode);
	RPC_SRV_RESULT set_trigger_mode(long DevID,long SpectroID,LS_TRIG_MODE mode);
	RPC_SRV_RESULT get_max_intensity(long DevID,long SpectroID,double &intensity);
	RPC_SRV_RESULT get_min_integration_time(long DevID,long SpectroID,long &time);
	RPC_SRV_RESULT set_integration_time(long DevID,long SpectroID,long time);
	RPC_SRV_RESULT read_formatted_spectrum(long DevID,long SpectroID,std::deque<double> *pList);//read_formatted_spectrum(long DevID);
	//read_formatted_spectrum(DevId,SpectroID,&FormattedSpectrum);
	RPC_SRV_RESULT read_unformatted_spectrum(long DevID,long SpectroID,std::deque<unsigned char> *pList,std::deque<double> *pWList);
	RPC_SRV_RESULT read_wavelength(long DevID,long SpectroID,std::deque<double> *pWList);

public:
	SeaBreezeAccess(std::string DevNode,std::string DevType);
	~SeaBreezeAccess();
	RPC_SRV_RESULT init_sensor();
	RPC_SRV_RESULT trigger_measurement();
	RPC_SRV_RESULT get_integration_time(uint32_t &time);
	RPC_SRV_RESULT set_integration_time(uint32_t time);

	RPC_SRV_RESULT get_spectrum_count(uint32_t &count);//max number of samples
	RPC_SRV_RESULT get_spectrum_value(uint32_t index,double &value);//read the sample of given index
	RPC_SRV_RESULT get_wavelength_count(uint32_t &count);//max number of wavelengths
	RPC_SRV_RESULT get_wavelength_value(uint32_t index,double &value);//read the wavelength value of given index

};
/* ------------------------------------------------------------------------- */
#endif
