#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ApisrvJsonDef.h"
#include "ApiRpc.h"
#include "ADJsonRpcMgr.hpp"
#include "MyCmdline.h"
#include "SrcControlVersion.h"
#include "ADTimer.hpp"
#include "SdapiHttpHandler.h"
//#include "XmppMgr.h" //ADXmppProxy.hpp"
/* ------------------------------------------------------------------------- */
using namespace std;
int main(int argc, const char* argv[])
{
	//cmdline parsing
	char ver[255];snprintf(ver,254,"%05d",SRC_CONTROL_VERSION);
	MyCmdline CmdLine(CMDLINE_HELPER_MODE_SERVER,APISRV_JSON_PORT_NUMBER,ver);
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
	APISRV_CMN_DATA_CACHE DataCache;
	DataCache.pDevInfo=(void*)&DevInfo   ;//rpc's needs to know board or device type

	//ADXmppProxy XmpManager;//xmpp handler
//	XmppMgr XmpManager;
//	DataCache.pApiMgr =(void*)&XmpManager;//rpc's needs to know the object pointer of xmpp-handling-object
//	XmpManager.SetDebugLog(dbglog);
	//attach rpc classes to ADJsonRpcMgr
	ADJsonRpcMgr RpcMgr(SRC_CONTROL_VERSION,dbglog,&DevInfo); //main rpc handler

//	XmppRpc AsyncGet (XMPROXY_RPC_ASYNCTASK_GET   ,EJSON_XMPROXY_RPC_GET_ASYNCTASK   ,emulat,dbglog,&DataCache); 
//	RpcMgr.AttachRpc(&AsyncGet);

	//start listening for rpc-commands
	RpcMgr.AttachHeartBeat(&AppTimer);//attach 100ms heartbeat to ADJsonRpcMgr
	//RpcMgr.SupportShutdownRpc(false);//uncomment if this service is required to ignore shutdown-rpc
	RpcMgr.Start(CmdLine.get_port_number(),CmdLine.get_socket_log(),CmdLine.get_emulation_mode());

//	char filepath[512];CmdLine.get_login_filepath(filepath);
//	std::string XmpAccountAuthFilePath(filepath);
//	XmpManager.Start(XmpAccountAuthFilePath);//connect the xmpp server
//	XmpManager.AttachHeartBeat(&AppTimer);//attach 100ms heartbeat to xmpp client manager for sending white-space-ping-to-server

	//server is ready to serve rpc's
	RpcMgr.SetServiceReadyFlag(EJSON_RPCGMGR_READY_STATE_READY);

	//hear starts the http server
	SdapiHttpHandler HttpSrv(CmdLine.get_http_port());
	HttpSrv.StartServer();

	//wait for sigkill or sigterm signal
//	AppTimer.wait_for_exit_signal();//loop till KILL or TERM signal is received
//	XmpManager.Stop();//disconnect the xmpp server
	AppTimer.stop_timer();//stop sending heart-beats to other objects
	//XmpManager.Stop();//disconnect the xmpp server
	return 0;
}
