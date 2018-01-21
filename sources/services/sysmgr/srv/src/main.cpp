#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "SysmgrJsonDef.h"
#include "ADJsonRpcMgr.hpp"
#include "MyCmdline.h"
#include "SrcControlVersion.h"
#include "ADTimer.hpp"
#include "DevIdent.h"
#include "DevIdentRaspiAZero.h"
#include "DevIdentNexx3020.h"
#include "DevIdentA5V11.h"
/* ------------------------------------------------------------------------- */
#include "I2CDualPcfLcd.hpp"
#include "I2CPcfLcd.hpp"
#include "I2CSsd1306.hpp"
#include "ADSysInfo.hpp"
#include "DispCtrlRpc.h"
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
DisplayDevice* low_memory_device_special_action(std::string sysconf,std::string Type,std::string devnode);
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

	//sysmgr special usecase: if sysmgr is used on low-mem openwrt device, print the ip address using i2c dev.
	DataCache.pDisplay=(void*)low_memory_device_special_action(CmdLine.get_sys_config(),CmdLine.get_disp_type(),CmdLine.get_dev_node());//"ssd1306_128x64");//"a5v11-xmpp"

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

	//for low memory device, support display-clear and display-print commands.
	//if(CmdLine.get_sys_config()=="a5v11-xmpp")
	//{
	//}
	DispCtrlRpc DispClear(SYSMGR_RPC_DISP_CLEAR,EJSON_SYSMGR_RPC_DISP_CLEAR,emulat,dbglog,&DataCache); //clear-display
	RpcMgr.AttachRpc(&DispClear);
	DispCtrlRpc DispPrint(SYSMGR_RPC_DISP_PRINT,EJSON_SYSMGR_RPC_DISP_PRINT,emulat,dbglog,&DataCache); //print-display
	RpcMgr.AttachRpc(&DispPrint);
	DispCtrlRpc DispGetBklt(SYSMGR_RPC_DISP_GET_BKLT,EJSON_SYSMGR_RPC_DISP_GET_BKLT,emulat,dbglog,&DataCache); //print-display
	RpcMgr.AttachRpc(&DispGetBklt);
	DispCtrlRpc DispSetBklt(SYSMGR_RPC_DISP_SET_BKLT,EJSON_SYSMGR_RPC_DISP_SET_BKLT,emulat,dbglog,&DataCache); //print-display
	RpcMgr.AttachRpc(&DispSetBklt);


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

	DisplayDevice *disp=(DisplayDevice*)DataCache.pDisplay;
	if(disp!=NULL)delete disp;
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
		case ADCMN_BOARD_TYPE_A5_V11       :pDevice = new DevIdentA5V11;break;
		case ADCMN_BOARD_TYPE_NEXX_3020    :pDevice = new DevIdentNexx3020;break;
		default: break;
	}
	return pDevice;
}
/* ------------------------------------------------------------------------- */
//for a5-v11:xmpp-chatbot config, due to low memory, dispsrv could not be included.
//hence sysmgr will take care of showing ip address on display
DisplayDevice* low_memory_device_special_action(std::string sysconf,std::string Type,std::string devnode)
{
	if(sysconf!="a5v11-xmpp")
		return NULL;
	std::string DevNode = devnode;//"/dev/i2c-0";
	DisplayDevice* pDevice=NULL;
	ADLIB_DISPLAY_TYPE disp_type;
	const char *dispTbl[] = ADLIB_DISPLAY_TYPE_TABL;
	ADCmnStringProcessor string_proc;
	disp_type=(ADLIB_DISPLAY_TYPE)string_proc.string_to_enum(dispTbl,(char*)Type.c_str(),ADLIB_DISPLAY_TYPE_UNKNOWN);
	if(disp_type>=ADLIB_DISPLAY_TYPE_UNKNOWN)
		return NULL;//unable to determine which display object is needed
	switch(disp_type)
	{
		case ADLIB_DISPLAY_TYPE_SSD1306_128x64   :pDevice = new I2CSsd1306(DevNode,Type);break;
		case ADLIB_DISPLAY_TYPE_1602_DUAL_PCF    ://pDevice = new I2CPcfLcd(DevNode,Type);break;//I2CDualPcfLcd
		case ADLIB_DISPLAY_TYPE_1602_PCF         ://pDevice = new I2CPcfLcd(DevNode,Type);break;
		case ADLIB_DISPLAY_TYPE_2002_PCF         ://pDevice = new I2CPcfLcd(DevNode,Type);break;
		case ADLIB_DISPLAY_TYPE_2004_PCF         ://pDevice = new I2CPcfLcd(DevNode,Type);break;
		case ADLIB_DISPLAY_TYPE_1604_PCF         :pDevice = new I2CPcfLcd(DevNode,Type);break;
		default                                  :pDevice = new I2CSsd1306(DevNode,Type);break;//keep this as default
	}
	pDevice->init_display();
	pDevice->clear_display();
	char msg[255];

	//////////////line-1 ip-address///////////////
	ADSysInfo SysInfo;//lib-class for reading cpu-info and system-info
	char netmask[512];char mac[512];char ip[512];
	if(SysInfo.read_network_info((char*)"eth0",mac,ip,netmask)==0)
		sprintf(msg,"%s",ip);
	else
	{
		//if network is not connected, use ifconfig method
		if(SysInfo.read_network_info_ifconfig((char*)"eth0",mac,ip,netmask)==0)
			sprintf(msg,"ip-%s",ip);
		else
			sprintf(msg,"ip-not-available");
	}
	pDevice->print_line(msg,DISPLAY_LINE_1);

	//////////////line-2 software version///////////////
	std::ifstream file("/etc/version.txt");
	std::string version_num;
	std::getline(file, version_num);
	if(version_num.size()<=0)
		version_num="unknown";
	if(version_num.size()>=250) //something is wrong, file cannot be so big
		version_num="ver-error";
	sprintf(msg,"Ver-%s",version_num.c_str());//Ver-00.01.12345
	pDevice->print_line(msg,DISPLAY_LINE_2);

	//////////////line-3 a5v11-xmpp name///////////////
	if(ADLIB_DISPLAY_TYPE_1602_PCF!=disp_type)//disp-type has 3rd line
	{
		sprintf(msg,"a5v11-xmpp");
		pDevice->print_line(msg,DISPLAY_LINE_3);
	}
	return pDevice;
}
/* ------------------------------------------------------------------------- */

