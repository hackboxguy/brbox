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
/* ------------------------------------------------------------------------- */
using namespace std;
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
	//DispAccess Disp(CmdLine.get_disp_type());
	//DataCache.pDispAccess=(void*)&Disp;
	//Disp.print_line(1,(char*)"Hello World!!!");
	I2CDualPcfLcd lcd(CmdLine.get_dev_node());//"/dev/i2c-0");
	//DataCache.pDispAccess=(void*)&lcd;
	DataCache.pDisplay=&lcd;
	//lcd.print_center(LCD_DISP_LINE_1,"helloworld");
	DataCache.pDisplay->print_line("hello world!");//,1);
	
	//attach rpc classes to ADJsonRpcMgr
	ADJsonRpcMgr RpcMgr(SRC_CONTROL_VERSION,dbglog,&DevInfo); //main rpc handler

	/****************************RPC list*************************************/
	DispCtrlRpc DisplayInit (DISPSRV_RPC_DISP_INIT ,EJSON_DISPSRV_RPC_DISP_INIT ,emulat,dbglog,&DataCache);
	DispCtrlRpc DisplayClear(DISPSRV_RPC_DISP_CLEAR,EJSON_DISPSRV_RPC_DISP_CLEAR,emulat,dbglog,&DataCache);
	DispCtrlRpc DisplayPrint(DISPSRV_RPC_DISP_PRINT,EJSON_DISPSRV_RPC_DISP_PRINT,emulat,dbglog,&DataCache);
	RpcMgr.AttachRpc(&DisplayInit);
	RpcMgr.AttachRpc(&DisplayClear);
	RpcMgr.AttachRpc(&DisplayPrint);

	//start listening for rpc-commands
	RpcMgr.AttachHeartBeat(&AppTimer);//attach 100ms heartbeat to ADJsonRpcMgr
	RpcMgr.Start(CmdLine.get_port_number(),CmdLine.get_socket_log(),CmdLine.get_emulation_mode());
	//server is ready to serve rpc's
	RpcMgr.SetServiceReadyFlag(EJSON_RPCGMGR_READY_STATE_READY);

	//wait for sigkill or sigterm signal
	AppTimer.wait_for_exit_signal();//loop till KILL or TERM signal is received
	AppTimer.stop_timer();//stop sending heart-beats to other objects
	return 0;
}
