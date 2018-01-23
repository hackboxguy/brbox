#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ADJsonRpcMgr.hpp"
#include "MyCmdline.h"
#include "SrcControlVersion.h"
#include "ADTimer.hpp"
#include "ADEvntNotifier.hpp"
/* ------------------------------------------------------------------------- */
#include "LightsenseJsonDef.h"
#include "SensorCtrlRpc.h"
#include "LightSensor.hpp"
#include "I2CTAOS3414Sensor.hpp"
#include "SeaBreezeAccess.h"
/* ------------------------------------------------------------------------- */
using namespace std;
LightSensor* create_sensor_device(std::string DevNode,std::string Type);
int main(int argc, const char* argv[])
{
	//cmdline parsing
	char ver[255];snprintf(ver,254,"%05d",SRC_CONTROL_VERSION);
	MyCmdline CmdLine(CMDLINE_HELPER_MODE_SERVER,SERVER_JSON_PORT_NUM,ver);//40000 is the portnumber
	CmdLine.parse_cmdline_arguments(argc,(char**)argv);
	if(CmdLine.is_help_printed())
		return -1;//user just requested to print the help info

	//read the board-type info from cmdline argument
	ADCMN_DEV_INFO DevInfo;
	CmdLine.get_dev_info(&DevInfo);

	bool dbglog = CmdLine.get_debug_log();
	bool emulat = CmdLine.get_emulation_mode();

	//start 100ms timer
	ADTimer AppTimer(100,CmdLine.get_port_number());//only one instance per application(or process) must exist
	//create a common data Cache of the service
	LIGHTSENSE_CMN_DATA_CACHE DataCache;
	ADEvntNotifier EventNotifier;//global event notification object
	DataCache.pDevInfo=(void*)&DevInfo;//rpc's needs to know board or device type
	DataCache.pEventNotifier=(void*)&EventNotifier;
	DataCache.pSensor=create_sensor_device(CmdLine.get_dev_node(),CmdLine.get_sensor_type());

	//attach rpc classes to ADJsonRpcMgr
	ADJsonRpcMgr RpcMgr(SRC_CONTROL_VERSION,dbglog,&DevInfo); //main rpc handler

	/****************************RPC list*************************************/
	SensorCtrlRpc SensorInit    (LIGHTSENSE_RPC_SENSOR_INIT ,EJSON_LIGHTSENSE_RPC_SENSOR_INIT ,emulat,dbglog,&DataCache);
	SensorCtrlRpc StartMeasure  (LIGHTSENSE_RPC_START_MEASURE,EJSON_LIGHTSENSE_RPC_START_MEASURE,emulat,dbglog,&DataCache);
	SensorCtrlRpc IntegrTimeGet (LIGHTSENSE_RPC_GET_INTEGRATION_TIME,EJSON_LIGHTSENSE_GET_INTEGRATION_TIME,emulat,dbglog,&DataCache);
	SensorCtrlRpc IntegrTimeSet (LIGHTSENSE_RPC_SET_INTEGRATION_TIME,EJSON_LIGHTSENSE_SET_INTEGRATION_TIME,emulat,dbglog,&DataCache);
	SensorCtrlRpc SyncEdgeGet   (LIGHTSENSE_RPC_SYNC_EDGE_GET,EJSON_LIGHTSENSE_SYNC_EDGE_GET,emulat,dbglog,&DataCache);
	SensorCtrlRpc SyncEdgeSet   (LIGHTSENSE_RPC_SYNC_EDGE_SET,EJSON_LIGHTSENSE_SYNC_EDGE_SET,emulat,dbglog,&DataCache);
	SensorCtrlRpc IntgrModeGet  (LIGHTSENSE_RPC_INTEGRATION_MODE_GET,EJSON_LIGHTSENSE_INTEGRATION_MODE_GET,emulat,dbglog,&DataCache);
	SensorCtrlRpc IntgrModeSet  (LIGHTSENSE_RPC_INTEGRATION_MODE_SET,EJSON_LIGHTSENSE_INTEGRATION_MODE_SET,emulat,dbglog,&DataCache);
	SensorCtrlRpc IntrStopStsGet(LIGHTSENSE_RPC_INTR_STOP_STS_GET,EJSON_LIGHTSENSE_INTR_STOP_STS_GET,emulat,dbglog,&DataCache);
	SensorCtrlRpc IntrStopStsSet(LIGHTSENSE_RPC_INTR_STOP_STS_SET,EJSON_LIGHTSENSE_INTR_STOP_STS_SET,emulat,dbglog,&DataCache);
	SensorCtrlRpc IntrModeGet   (LIGHTSENSE_RPC_INTR_MODE_GET,EJSON_LIGHTSENSE_INTR_MODE_GET,emulat,dbglog,&DataCache);
	SensorCtrlRpc IntrModeSet   (LIGHTSENSE_RPC_INTR_MODE_SET,EJSON_LIGHTSENSE_INTR_MODE_SET,emulat,dbglog,&DataCache);
	SensorCtrlRpc IntrRateGet   (LIGHTSENSE_RPC_INTR_RATE_GET,EJSON_LIGHTSENSE_INTR_RATE_GET,emulat,dbglog,&DataCache);
	SensorCtrlRpc IntrRateSet   (LIGHTSENSE_RPC_INTR_RATE_SET,EJSON_LIGHTSENSE_INTR_RATE_SET,emulat,dbglog,&DataCache);
	SensorCtrlRpc IntrSourceGet (LIGHTSENSE_RPC_INTR_SOURCE_GET,EJSON_LIGHTSENSE_INTR_SOURCE_GET,emulat,dbglog,&DataCache);
	SensorCtrlRpc IntrSourceSet (LIGHTSENSE_RPC_INTR_SOURCE_SET,EJSON_LIGHTSENSE_INTR_SOURCE_SET,emulat,dbglog,&DataCache);
	SensorCtrlRpc GainModeGet   (LIGHTSENSE_RPC_GAIN_MODE_GET,EJSON_LIGHTSENSE_GAIN_MODE_GET,emulat,dbglog,&DataCache);
	SensorCtrlRpc GainModeSet   (LIGHTSENSE_RPC_GAIN_MODE_SET,EJSON_LIGHTSENSE_GAIN_MODE_SET,emulat,dbglog,&DataCache);
	SensorCtrlRpc PrescalerGet  (LIGHTSENSE_RPC_PRESCALER_GET,EJSON_LIGHTSENSE_PRESCALER_GET,emulat,dbglog,&DataCache);
	SensorCtrlRpc PrescalerSet  (LIGHTSENSE_RPC_PRESCALER_SET,EJSON_LIGHTSENSE_PRESCALER_SET,emulat,dbglog,&DataCache);
	SensorCtrlRpc RGBWCountGet  (LIGHTSENSE_RPC_RGBWCOUNT_GET,EJSON_LIGHTSENSE_RGBWCOUNT_GET,emulat,dbglog,&DataCache);

	RpcMgr.AttachRpc(&SensorInit);
	RpcMgr.AttachRpc(&StartMeasure);
	RpcMgr.AttachRpc(&IntegrTimeGet);
	RpcMgr.AttachRpc(&IntegrTimeSet);
	RpcMgr.AttachRpc(&SyncEdgeGet);
	RpcMgr.AttachRpc(&SyncEdgeSet);
	RpcMgr.AttachRpc(&IntgrModeGet);
	RpcMgr.AttachRpc(&IntgrModeSet);
	RpcMgr.AttachRpc(&IntrStopStsGet);
	RpcMgr.AttachRpc(&IntrStopStsSet);
	RpcMgr.AttachRpc(&IntrModeGet);
	RpcMgr.AttachRpc(&IntrModeSet);
	RpcMgr.AttachRpc(&IntrRateGet);
	RpcMgr.AttachRpc(&IntrRateSet);
	RpcMgr.AttachRpc(&IntrSourceGet);
	RpcMgr.AttachRpc(&IntrSourceSet);
	RpcMgr.AttachRpc(&GainModeGet);
	RpcMgr.AttachRpc(&GainModeSet);
	RpcMgr.AttachRpc(&PrescalerGet);
	RpcMgr.AttachRpc(&PrescalerSet);
	RpcMgr.AttachRpc(&RGBWCountGet);
	//start listening for rpc-commands
	RpcMgr.AttachHeartBeat(&AppTimer);//attach 100ms heartbeat to ADJsonRpcMgr
	RpcMgr.Start(CmdLine.get_port_number(),CmdLine.get_socket_log(),CmdLine.get_emulation_mode());
	//server is ready to serve rpc's
	RpcMgr.SetServiceReadyFlag(EJSON_RPCGMGR_READY_STATE_READY);

	//wait for sigkill or sigterm signal
	AppTimer.wait_for_exit_signal();//loop till KILL or TERM signal is received
	AppTimer.stop_timer();//stop sending heart-beats to other objects
	if(DataCache.pSensor!=NULL)
		delete DataCache.pSensor;
	return 0;
}
//following function creates a sensor object based on user passed cmd argument
LightSensor* create_sensor_device(std::string DevNode,std::string Type)
{
	LightSensor* pDevice=NULL;
	LIGHT_SENSOR_TYPE sensor_type;
	const char *sensorTbl[] = LIGHT_SENSOR_TYPE_TABL;
	ADCmnStringProcessor string_proc;
	sensor_type=(LIGHT_SENSOR_TYPE)string_proc.string_to_enum(sensorTbl,(char*)Type.c_str(),LIGHT_SENSOR_TYPE_UNKNOWN);
	if(sensor_type>=LIGHT_SENSOR_TYPE_UNKNOWN)
		return NULL;//unable to determine which display object is needed
	switch(sensor_type)
	{
		case LIGHT_SENSOR_TYPE_TAOS3414:pDevice = new I2CTAOS3414Sensor(DevNode,Type);break;
		case LIGHT_SENSOR_TYPE_OOSTS   :pDevice = new SeaBreezeAccess(DevNode,Type);break;
		//case LIGHT_SENSOR_TYPE_OOSTS   :pDevice = new I2CDualPcfLcd(DevNode,Type);break;//spectrometer
		default: break;
	}
	return pDevice;
}

