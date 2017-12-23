#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "SysmgrJsonDef.h"
#include "ADJsonRpcMgr.hpp"
#include "MyCmdline.h"
#include "SrcControlVersion.h"
#include "ADTimer.hpp"
#include "DevIdent.h"
#include "DevIdentRaspiAZero.h"
#include "DevIdentNexx3020.h"
/* ------------------------------------------------------------------------- */
#include "NetRpc.h"
#include "SysRpc.h"
#include "EventHandler.h"
#include "EventMonitor.h"
#include "LogHandler.h"
#include "CmnRpc.h"
#define SERVER_JSON_PORT_NUM SYSMGR_JSON_PORT_NUMBER
#define LOG_FILE_PATH         "/tmp/messages"
/* ------------------------------------------------------------------------- */
using namespace std;
DevIdent* create_dev_ident_object(ADCMN_BOARD_TYPE board_type);
/* ------------------------------------------------------------------------- */
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
	SYSMGR_CMN_DATA_CACHE DataCache;
	DataCache.pDevInfo=(void*)&DevInfo;//rpc's needs to know board or device type

	//prepare logger
	LogHandler Logger(LOG_FILE_PATH);//log handler
	DataCache.pLogger =(void*)&Logger;//rpc's needs to know the object pointer of sms-handling-object

	//prepare device-identify object
	DataCache.pDevIdent=(void*)create_dev_ident_object(DevInfo.BoardType);

	//attach rpc classes to ADJsonRpcMgr
	ADJsonRpcMgr RpcMgr(SRC_CONTROL_VERSION,dbglog,&DevInfo); //main rpc handler

 	/****************Prepare event receiver to receive events*****************/
	//EventHandler EvntReceiver("dummy_rpc",0,emulat,dbglog,&DataCache);
	//RpcMgr.AttachEventReceiver(&EvntReceiver);
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
	NetRpc IpGet  (SYSMGR_RPC_IP_ADDR_GET ,EJSON_SYSMGR_RPC_GET_IP_ADDR ,emulat,dbglog,&DataCache);  //network related rpc handler class
	RpcMgr.AttachRpc(&IpGet);
	NetRpc IpSet  (SYSMGR_RPC_IP_ADDR_SET ,EJSON_SYSMGR_RPC_SET_IP_ADDR ,emulat,dbglog,&DataCache);  //network related rpc handler class
	RpcMgr.AttachRpc(&IpSet);
	NetRpc NetmaskGet(SYSMGR_RPC_NETMASK_GET ,EJSON_SYSMGR_RPC_GET_NETMASK ,emulat,dbglog,&DataCache);//network related rpc handler class
	RpcMgr.AttachRpc(&NetmaskGet);
	NetRpc NetmaskSet(SYSMGR_RPC_NETMASK_SET ,EJSON_SYSMGR_RPC_SET_NETMASK ,emulat,dbglog,&DataCache);//network related rpc handler class
	RpcMgr.AttachRpc(&NetmaskSet);

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
	SysRpc HostnameGet(SYSMGR_RPC_HOSTNAME_GET    ,EJSON_SYSMGR_RPC_GET_HOSTNAME   ,emulat,dbglog,&DataCache); 
	RpcMgr.AttachRpc(&HostnameGet);
	SysRpc HostnameSet(SYSMGR_RPC_HOSTNAME_SET    ,EJSON_SYSMGR_RPC_SET_HOSTNAME   ,emulat,dbglog,&DataCache); 
	RpcMgr.AttachRpc(&HostnameSet);
	SysRpc MyIp(SYSMGR_RPC_MY_PUBLIC_IP_GET    ,EJSON_SYSMGR_RPC_GET_MY_PUBLIC_IP   ,emulat,dbglog,&DataCache); 
	RpcMgr.AttachRpc(&MyIp);
	SysRpc DefHostname(SYSMGR_RPC_DEFAULT_HOSTNAME_SET,EJSON_SYSMGR_RPC_SET_DEFAULT_HOSTNAME,emulat,dbglog,&DataCache); 
	RpcMgr.AttachRpc(&DefHostname);
	SysRpc UpdtLogList(SYSMGR_RPC_UPDATE_LOG_SET,EJSON_SYSMGR_RPC_SET_UPDATE_LOG,emulat,dbglog,&DataCache); 
	RpcMgr.AttachRpc(&UpdtLogList);
	SysRpc GetLogCount(SYSMGR_RPC_LOG_COUNT_GET,EJSON_SYSMGR_RPC_GET_LOG_COUNT,emulat,dbglog,&DataCache); 
	RpcMgr.AttachRpc(&GetLogCount);
	SysRpc GetLogLine(SYSMGR_RPC_LOG_LINE_GET,EJSON_SYSMGR_RPC_GET_LOG_LINE,emulat,dbglog,&DataCache); 
	RpcMgr.AttachRpc(&GetLogLine);
	SysRpc RunShellCmd(SYSMGR_RPC_RUN_SHELLCMD,EJSON_SYSMGR_RPC_RUN_SHELLCMD,emulat,dbglog,&DataCache); 
	RpcMgr.AttachRpc(&RunShellCmd);
	SysRpc DevIdent (SYSMGR_RPC_DEVIDENT,EJSON_SYSMGR_RPC_DEVIDENT,emulat,dbglog,&DataCache);
	RpcMgr.AttachRpc(&DevIdent);
	SysRpc EvntSubscr (SYSMGR_RPC_EVNT_SUBSCRIBE,EJSON_SYSMGR_RPC_EVNT_SUBSCRIBE,emulat,dbglog,&DataCache);
	RpcMgr.AttachRpc(&EvntSubscr);

	//common rpc hadler object(eg: trigger-data-save/store-factory/restore-factory..etc)
	CmnRpc CmnRpcHandler("cmnrpc",0,emulat,dbglog,&DataCache);//common rpc-handler(name and index are ignored)
	RpcMgr.AttachRpc(&CmnRpcHandler);

	//start listening for rpc-commands
	RpcMgr.AttachHeartBeat(&AppTimer);//attach 100ms heartbeat to ADJsonRpcMgr
	RpcMgr.SupportShutdownRpc(false);//this is a system-manager, needs to be alive all the time, hence dont support shutdown via rpc
	RpcMgr.Start(CmdLine.get_port_number(),CmdLine.get_socket_log(),CmdLine.get_emulation_mode());

 	/****************Prepare event receiver to receive events*****************/
	//TODO: wait for event-sending-service to be ready
	//monitor the events(for debug purpose)
	DataCache.EvntMonitorConfigFile=CmdLine.get_monit_cfg_file();
	EventHandler* pEventHandler=NULL;
	EventMonitor* pEventMonitor=NULL;
	if(DataCache.EvntMonitorConfigFile=="") //if event monitor config file is not passed by the user, then do internal event handler
	{
		//EventHandler EvntReceiver("dummy_rpc",0,emulat,dbglog,&DataCache);
		pEventHandler = new EventHandler("dummy_rpc",0,emulat,dbglog,&DataCache);
		RpcMgr.AttachEventReceiver(pEventHandler);//default event handler
		DataCache.pEventDefault=(void*)pEventHandler;
	}
	else
	{
		//EventMonitor EventMonitor("dummy_rpc",0,emulat,dbglog,&DataCache);
		pEventMonitor = new EventMonitor("dummy_rpc",0,emulat,dbglog,&DataCache);
		RpcMgr.AttachEventReceiver(pEventMonitor);//event monitor for debug purpose
		DataCache.pEventCustom=(void*)pEventMonitor;
	}

	//server is ready to serve rpc's
	RpcMgr.SetServiceReadyFlag(EJSON_RPCGMGR_READY_STATE_READY);
	//wait for sigkill or sigterm signal
	AppTimer.wait_for_exit_signal();//loop till KILL or TERM signal is received
	AppTimer.stop_timer();//stop sending heart-beats to other objects
	if(pEventHandler!=NULL)delete pEventHandler;
	if(pEventMonitor!=NULL)delete pEventMonitor;
	return 0;
}
/* ------------------------------------------------------------------------- */
//following function creates device identify object for board-identify-with-led-blink-functionality
DevIdent* create_dev_ident_object(ADCMN_BOARD_TYPE board_type)
{
	DevIdent* pDevice=NULL;
	switch(board_type)
	{
		case ADCMN_BOARD_TYPE_RASPI_A      :pDevice = new DevIdentRaspiAZero;break;
		case ADCMN_BOARD_TYPE_RASPI_APLUS  :pDevice = new DevIdentRaspiAZero;break;
		case ADCMN_BOARD_TYPE_RASPI_B      :pDevice = new DevIdentRaspiAZero;break;//break;
		case ADCMN_BOARD_TYPE_RASPI_BPLUS  :pDevice = new DevIdentRaspiAZero;break;//break;
		case ADCMN_BOARD_TYPE_RASPI_B2     :break;
		case ADCMN_BOARD_TYPE_BAYTRAIL     :break;
		case ADCMN_BOARD_TYPE_BAYTRAIL_I210:break;
		case ADCMN_BOARD_TYPE_BBB          :break;
		case ADCMN_BOARD_TYPE_RASPI_0      :pDevice = new DevIdentRaspiAZero;break;
		case ADCMN_BOARD_TYPE_RASPI_3      :pDevice = new DevIdentRaspiAZero;break;
		case ADCMN_BOARD_TYPE_A5_V11       :break;
		case ADCMN_BOARD_TYPE_NEXX_3020    :pDevice = new DevIdentNexx3020;break;
		default: break;
	}
	return pDevice;
}
/* ------------------------------------------------------------------------- */
