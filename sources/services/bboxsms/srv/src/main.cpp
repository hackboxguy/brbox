#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "BboxsmsJsonDef.h"
#include "SmsRpc.h"
#include "ADJsonRpcMgr.hpp"
#include "MyCmdline.h"
#include "SrcControlVersion.h"
#include "ADTimer.hpp"
#include "SmsMgr.h"
/* ------------------------------------------------------------------------- */
using namespace std;
int main(int argc, const char* argv[])
{
	//cmdline parsing
	char ver[255];snprintf(ver,254,"%05d",SRC_CONTROL_VERSION);
	MyCmdline CmdLine(CMDLINE_HELPER_MODE_SERVER,BBOXSMS_JSON_PORT_NUMBER,ver);//40000 is the portnumber
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
	BBOXSMS_CMN_DATA_CACHE DataCache;
	DataCache.pDevInfo=(void*)&DevInfo   ;//rpc's needs to know board or device type
	SmsMgr SmsManager;//sms handler
	DataCache.pSmsMgr =(void*)&SmsManager;//rpc's needs to know the object pointer of sms-handling-object

	//attach rpc classes to ADJsonRpcMgr
	ADJsonRpcMgr RpcMgr(SRC_CONTROL_VERSION,dbglog,&DevInfo); //main rpc handler

	SmsRpc DeletAll(BBOXSMS_RPC_SMS_DELETE_ALL,EJSON_BBOXSMS_RPC_SMS_DELETE_ALL,emulat,dbglog,&DataCache);
	SmsRpc Delete  (BBOXSMS_RPC_SMS_DELETE    ,EJSON_BBOXSMS_RPC_SMS_DELETE    ,emulat,dbglog,&DataCache);
	SmsRpc TotalGet(BBOXSMS_RPC_SMS_TOTAL_GET ,EJSON_BBOXSMS_RPC_SMS_TOTAL_GET ,emulat,dbglog,&DataCache);
	SmsRpc SmsGet  (BBOXSMS_RPC_SMS_GET       ,EJSON_BBOXSMS_RPC_SMS_GET       ,emulat,dbglog,&DataCache);
	SmsRpc SmsSend (BBOXSMS_RPC_SMS_SEND      ,EJSON_BBOXSMS_RPC_SMS_SEND      ,emulat,dbglog,&DataCache);

	RpcMgr.AttachRpc(&DeletAll);
	RpcMgr.AttachRpc(&Delete);
	RpcMgr.AttachRpc(&TotalGet);
	RpcMgr.AttachRpc(&SmsGet);
	RpcMgr.AttachRpc(&SmsSend);

	//start listening for rpc-commands
	RpcMgr.AttachHeartBeat(&AppTimer);//attach 100ms heartbeat to ADJsonRpcMgr
	//RpcMgr.SupportShutdownRpc(false);//this is a system-manager, needs to be alive all the time, hence dont support shutdown via rpc
	RpcMgr.Start(CmdLine.get_port_number(),CmdLine.get_socket_log(),CmdLine.get_emulation_mode());
	//server is ready to serve rpc's
	RpcMgr.SetServiceReadyFlag(EJSON_RPCGMGR_READY_STATE_READY);


	//wait for sigkill or sigterm signal
	AppTimer.wait_for_exit_signal();//loop till KILL or TERM signal is received
	AppTimer.stop_timer();//stop sending heart-beats to other objects
	return 0;
}
