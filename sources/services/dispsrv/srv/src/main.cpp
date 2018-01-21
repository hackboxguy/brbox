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
#include "DispsrvJsonDef.h"
#include "DispAccess.h"
#include "DispCtrlRpc.h"
#include "I2CDualPcfLcd.hpp"
#include "I2CPcfLcd.hpp"
#include "I2CSsd1306.hpp"
/* ------------------------------------------------------------------------- */
using namespace std;
DisplayDevice* create_display_device(std::string DevNode,std::string Type);
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
	DISPSRV_CMN_DATA_CACHE DataCache;
	ADEvntNotifier EventNotifier;//global event notification object
	DataCache.pDevInfo=(void*)&DevInfo;//rpc's needs to know board or device type
	DataCache.pEventNotifier=(void*)&EventNotifier;
	//I2CDualPcfLcd lcd(CmdLine.get_dev_node(),CmdLine.get_disp_type());//"/dev/i2c-0");
	DataCache.pDisplay=create_display_device(CmdLine.get_dev_node(),CmdLine.get_disp_type());
	//if(DataCache.pDisplay!=NULL)
	//	DataCache.pDisplay->print_line((char*)"BBROX",DISPLAY_LINE_1);//,1);
	
	//attach rpc classes to ADJsonRpcMgr
	ADJsonRpcMgr RpcMgr(SRC_CONTROL_VERSION,dbglog,&DevInfo); //main rpc handler

	/****************************RPC list*************************************/
	DispCtrlRpc DisplayInit (DISPSRV_RPC_DISP_INIT ,EJSON_DISPSRV_RPC_DISP_INIT ,emulat,dbglog,&DataCache);
	DispCtrlRpc DisplayClear(DISPSRV_RPC_DISP_CLEAR,EJSON_DISPSRV_RPC_DISP_CLEAR,emulat,dbglog,&DataCache);
	DispCtrlRpc DisplayPrint(DISPSRV_RPC_DISP_PRINT,EJSON_DISPSRV_RPC_DISP_PRINT,emulat,dbglog,&DataCache);
	DispCtrlRpc DisplayBklGet(DISPSRV_RPC_DISP_GET_BKLT,EJSON_DISPSRV_RPC_DISP_GET_BLKT,emulat,dbglog,&DataCache);
	DispCtrlRpc DisplayBklSet(DISPSRV_RPC_DISP_SET_BKLT,EJSON_DISPSRV_RPC_DISP_SET_BLKT,emulat,dbglog,&DataCache);
	RpcMgr.AttachRpc(&DisplayInit);
	RpcMgr.AttachRpc(&DisplayClear);
	RpcMgr.AttachRpc(&DisplayPrint);
	RpcMgr.AttachRpc(&DisplayBklGet);
	RpcMgr.AttachRpc(&DisplayBklSet);

	//start listening for rpc-commands
	RpcMgr.AttachHeartBeat(&AppTimer);//attach 100ms heartbeat to ADJsonRpcMgr
	RpcMgr.Start(CmdLine.get_port_number(),CmdLine.get_socket_log(),CmdLine.get_emulation_mode());
	//server is ready to serve rpc's
	RpcMgr.SetServiceReadyFlag(EJSON_RPCGMGR_READY_STATE_READY);

	//wait for sigkill or sigterm signal
	AppTimer.wait_for_exit_signal();//loop till KILL or TERM signal is received
	AppTimer.stop_timer();//stop sending heart-beats to other objects
	if(DataCache.pDisplay!=NULL)
		delete DataCache.pDisplay;
	return 0;
}
//following function creates a display object based on user passed cmd argument
DisplayDevice* create_display_device(std::string DevNode,std::string Type)
{
	DisplayDevice* pDevice=NULL;
	ADLIB_DISPLAY_TYPE disp_type;
	const char *dispTbl[] = ADLIB_DISPLAY_TYPE_TABL;
	ADCmnStringProcessor string_proc;
	disp_type=(ADLIB_DISPLAY_TYPE)string_proc.string_to_enum(dispTbl,(char*)Type.c_str(),ADLIB_DISPLAY_TYPE_UNKNOWN);
	if(disp_type>=ADLIB_DISPLAY_TYPE_UNKNOWN)
		return NULL;//unable to determine which display object is needed
	switch(disp_type)
	{
		case ADLIB_DISPLAY_TYPE_SSD1306_128x32   :break;
		case ADLIB_DISPLAY_TYPE_SSD1306_128x64   :pDevice = new I2CSsd1306(DevNode,Type);break;
		case ADLIB_DISPLAY_TYPE_SSD1306_128x32_PI:
		case ADLIB_DISPLAY_TYPE_SSD1306_128x64_PI:pDevice = new DispAccess(DevNode,Type);break;
		case ADLIB_DISPLAY_TYPE_1602_DUAL_PCF    ://pDevice = new I2CPcfLcd(DevNode,Type);break;//I2CDualPcfLcd
		case ADLIB_DISPLAY_TYPE_1602_PCF         ://pDevice = new I2CPcfLcd(DevNode,Type);break;
		case ADLIB_DISPLAY_TYPE_2002_PCF         ://pDevice = new I2CPcfLcd(DevNode,Type);break;
		case ADLIB_DISPLAY_TYPE_2004_PCF         ://pDevice = new I2CPcfLcd(DevNode,Type);break;
		case ADLIB_DISPLAY_TYPE_1604_PCF         :pDevice = new I2CPcfLcd(DevNode,Type);break;
		default: break;
	}
	//"SSD1306_128x32"
	//"SSD1306_128x64"
	//"SSD1306_128x32_PI"
	//"SSD1306_128x64_PI"
	//"DISPLAY_TYPE_1602_DUAL_PCF"
	//"DISPLAY_TYPE_1602_PCF"
	return pDevice;
}

