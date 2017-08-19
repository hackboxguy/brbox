#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ADJsonRpcMgr.hpp"
#include "MyCmdline.h"
#include "SrcControlVersion.h"
#include "ADTimer.hpp"

/* ------------------------------------------------------------------------- */
#include "HotwireJsonDef.h"
#include "HotwireRpc.h"
#define SERVER_JSON_PORT_NUM ADCMN_PORT_MPLAYSRV
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
	GPIOCTL_CMN_DATA_CACHE DataCache;
	DataCache.pDevInfo=(void*)&DevInfo;//rpc's needs to know board or device type

	//attach rpc classes to ADJsonRpcMgr
	ADJsonRpcMgr RpcMgr(SRC_CONTROL_VERSION,dbglog,&DevInfo); //main rpc handler

	/****************************RPC list*************************************/
	GpioCtrlRpc OmxActGet(GPIOCTL_RPC_OMXACT_GET ,EJSON_GPIOCTL_RPC_OMXACT_GET ,emulat,dbglog,&DataCache);
	GpioCtrlRpc OmxActSet(GPIOCTL_RPC_OMXACT_SET ,EJSON_GPIOCTL_RPC_OMXACT_SET ,emulat,dbglog,&DataCache);
	GpioCtrlRpc ShowImgGet(MPLAYSRV_RPC_SHOWFBIMG_GET ,EJSON_MPLAYSRV_RPC_SHOWFBIMG_GET ,emulat,dbglog,&DataCache);
	GpioCtrlRpc ShowImgSet(MPLAYSRV_RPC_SHOWFBIMG_SET ,EJSON_MPLAYSRV_RPC_SHOWFBIMG_SET ,emulat,dbglog,&DataCache);
	GpioCtrlRpc QrCodeImgSet(MPLAYSRV_RPC_QRCODEIMG_SET ,EJSON_MPLAYSRV_RPC_QRCODEIMG_SET ,emulat,dbglog,&DataCache);
	GpioCtrlRpc PatternGet(MPLAYSRV_RPC_PATTERN_GET ,EJSON_MPLAYSRV_RPC_PATTERN_GET ,emulat,dbglog,&DataCache);
	GpioCtrlRpc PatternSet(MPLAYSRV_RPC_PATTERN_SET ,EJSON_MPLAYSRV_RPC_PATTERN_SET ,emulat,dbglog,&DataCache);

	GpioCtrlRpc MFileTypeGet(MPLAYSRV_RPC_MEDIAFILE_TYPE_GET,EJSON_MPLAYSRV_RPC_MEDIAFILE_TYPE_GET,emulat,dbglog,&DataCache);
	GpioCtrlRpc MFileTypeSet(MPLAYSRV_RPC_MEDIAFILE_TYPE_SET,EJSON_MPLAYSRV_RPC_MEDIAFILE_TYPE_SET,emulat,dbglog,&DataCache);
	GpioCtrlRpc MFileGet(MPLAYSRV_RPC_MEDIAFILE_GET,EJSON_MPLAYSRV_RPC_MEDIAFILE_GET,emulat,dbglog,&DataCache);
	GpioCtrlRpc MFileSet(MPLAYSRV_RPC_MEDIAFILE_SET,EJSON_MPLAYSRV_RPC_MEDIAFILE_SET,emulat,dbglog,&DataCache);
	GpioCtrlRpc MActionSet(MPLAYSRV_RPC_MEDIA_ACTION_SET,EJSON_MPLAYSRV_RPC_MEDIA_ACTION_SET,emulat,dbglog,&DataCache);
	GpioCtrlRpc ScreenStsGet(MPLAYSRV_RPC_SCREENSTS_GET,EJSON_MPLAYSRV_RPC_SCREENSTS_GET,emulat,dbglog,&DataCache);

	RpcMgr.AttachRpc(&OmxActGet);
	RpcMgr.AttachRpc(&OmxActSet);
	RpcMgr.AttachRpc(&ShowImgGet);
	RpcMgr.AttachRpc(&ShowImgSet);
	RpcMgr.AttachRpc(&QrCodeImgSet);
	RpcMgr.AttachRpc(&PatternGet);
	RpcMgr.AttachRpc(&PatternSet);
	RpcMgr.AttachRpc(&MFileTypeGet);
	RpcMgr.AttachRpc(&MFileTypeSet);
	RpcMgr.AttachRpc(&MFileGet);
	RpcMgr.AttachRpc(&MFileSet);
	RpcMgr.AttachRpc(&MActionSet);
	RpcMgr.AttachRpc(&ScreenStsGet);

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
