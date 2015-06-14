#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "SysmgrJsonDef.h"
#include "ADJsonRpcMgr.hpp"
#include "MyCmdline.h"
#include "SrcControlVersion.h"
#include "ADTimer.hpp"
/* ------------------------------------------------------------------------- */
#include "NetRpc.h"
#include "SysRpc.h"
#define SERVER_JSON_PORT_NUM SYSMGR_JSON_PORT_NUMBER
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
	ADTimer AppTimer(100);//only one instance per application(or process) must exist
	//create a common data Cache of the service
	SYSMGR_CMN_DATA_CACHE DataCache;
	DataCache.pDevInfo=(void*)&DevInfo;//rpc's needs to know board or device type

	//attach rpc classes to ADJsonRpcMgr
	ADJsonRpcMgr RpcMgr(SRC_CONTROL_VERSION,dbglog,&DevInfo); //main rpc handler
 
	/****************************RPC list*************************************/
	//network related rpc's
	NetRpc MacGet  (SYSMGR_RPC_MAC_ADDR_GET ,EJSON_SYSMGR_RPC_GET_MAC_ADDR ,emulat,dbglog,&DataCache);  //network related rpc handler class
	RpcMgr.AttachRpc(&MacGet);
	NetRpc MacSet  (SYSMGR_RPC_MAC_ADDR_SET ,EJSON_SYSMGR_RPC_SET_MAC_ADDR ,emulat,dbglog,&DataCache);  //network related rpc handler class
	RpcMgr.AttachRpc(&MacSet);
	NetRpc Ethcount(SYSMGR_RPC_ETH_COUNT_GET,EJSON_SYSMGR_RPC_GET_ETH_COUNT,emulat,dbglog,&DataCache);  //network related rpc handler class
	RpcMgr.AttachRpc(&Ethcount);
	NetRpc Ethname (SYSMGR_RPC_ETH_NAME_GET ,EJSON_SYSMGR_RPC_GET_ETH_NAME ,emulat,dbglog,&DataCache);  //network related rpc handler class
	RpcMgr.AttachRpc(&Ethname);
	//system related rpc's
	SysRpc LoadInfoGet(SYSMGR_RPC_LOADINFO_GET  ,EJSON_SYSMGR_RPC_GET_LOADINFO    ,emulat,dbglog,&DataCache);//sys related rpc handler class
	RpcMgr.AttachRpc(&LoadInfoGet);
	SysRpc MemInfoGet(SYSMGR_RPC_MEMINFO_GET    ,EJSON_SYSMGR_RPC_GET_MEMINFO     ,emulat,dbglog,&DataCache);//sys related rpc handler class
	RpcMgr.AttachRpc(&MemInfoGet);
	SysRpc CpuInfoGet(SYSMGR_RPC_CPUINFO_GET    ,EJSON_SYSMGR_RPC_GET_CPUINFO     ,emulat,dbglog,&DataCache);//sys related rpc handler class
	RpcMgr.AttachRpc(&CpuInfoGet);
	SysRpc DevOpGet(SYSMGR_RPC_DEV_OP_GET       ,EJSON_SYSMGR_RPC_GET_DEV_OP      ,emulat,dbglog,&DataCache); //get device operation
	RpcMgr.AttachRpc(&DevOpGet);
	SysRpc DevOpSet(SYSMGR_RPC_DEV_OP_SET       ,EJSON_SYSMGR_RPC_SET_DEV_OP      ,emulat,dbglog,&DataCache); //set device operation
	RpcMgr.AttachRpc(&DevOpSet);
	SysRpc FmwVerGet(SYSMGR_RPC_FMW_VER_GET     ,EJSON_SYSMGR_RPC_GET_FMWVER      ,emulat,dbglog,&DataCache); //get fmv version
	RpcMgr.AttachRpc(&FmwVerGet);
	SysRpc BootSysGet(SYSMGR_RPC_BOOT_SYSTEM_GET,EJSON_SYSMGR_RPC_GET_BOOT_SYSTEM ,emulat,dbglog,&DataCache);
	RpcMgr.AttachRpc(&BootSysGet);
	SysRpc FmwUpdateSet(SYSMGR_RPC_FMWUPDATE_SET,EJSON_SYSMGR_RPC_SET_FMWUPDATE   ,emulat,dbglog,&DataCache); 
	RpcMgr.AttachRpc(&FmwUpdateSet);
	SysRpc TftpGet(SYSMGR_RPC_DOWNLOADTFTP_SET  ,EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP,emulat,dbglog,&DataCache); 
	RpcMgr.AttachRpc(&TftpGet);
	SysRpc FtpGet(SYSMGR_RPC_DOWNLOADFTP_SET    ,EJSON_SYSMGR_RPC_SET_DOWNLOADFTP ,emulat,dbglog,&DataCache); 
	RpcMgr.AttachRpc(&FtpGet);
	SysRpc AsyncGet(SYSMGR_RPC_ASYNCTASK_GET    ,EJSON_SYSMGR_RPC_GET_ASYNCTASK   ,emulat,dbglog,&DataCache); 
	RpcMgr.AttachRpc(&AsyncGet);
	SysRpc DevTypeGet(SYSMGR_RPC_DEVTYPE_GET    ,EJSON_SYSMGR_RPC_GET_DEVTYPE   ,emulat,dbglog,&DataCache); 
	RpcMgr.AttachRpc(&DevTypeGet);

	//start listening for rpc-commands
	RpcMgr.AttachHeartBeat(&AppTimer);//attach 100ms heartbeat to ADJsonRpcMgr
	RpcMgr.SupportShutdownRpc(false);//this is a system-manager, needs to be alive all the time, hence dont support shutdown via rpc
	RpcMgr.Start(CmdLine.get_port_number(),CmdLine.get_socket_log(),CmdLine.get_emulation_mode());
	//server is ready to serve rpc's
	RpcMgr.SetServiceReadyFlag(EJSON_RPCGMGR_READY_STATE_READY);
	//wait for sigkill or sigterm signal
	AppTimer.wait_for_exit_signal();//loop till KILL or TERM signal is received
	AppTimer.stop_timer();//stop sending heart-beats to other objects
	return 0;
}
