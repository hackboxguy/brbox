#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "SysmgrJsonDef.h"
#include "NetRpc.h"
#include "ADJsonRpcMgr.hpp"
#include "MyCmdline.h"
#include "SrcControlVersion.h"
#include "ADTimer.hpp"

using namespace std;
int main(int argc, const char* argv[])
{
	char ver[255];snprintf(ver,254,"%05d",SRC_CONTROL_VERSION);
	MyCmdline CmdLine(CMDLINE_HELPER_MODE_SERVER,46000,ver);//(char*)"00.01");//TYSS_VERSION);
	CmdLine.parse_cmdline_arguments(argc,(char**)argv);
	if(CmdLine.is_help_printed())
		return -1;//user just requested to print the help info
	//start 100ms timer
	ADTimer AppTimer(100);//only one instance per application(or process) must exist



	ADJsonRpcMgr RpcMgr; //main rpc handler
	NetRpc IpAddr(SYSMGR_RPC_IP_ADDR_GET,0);
	RpcMgr.AttachRpc(&IpAddr);
	RpcMgr.AttachHeartBeat(&AppTimer);
	RpcMgr.Start(CmdLine.get_port_number(),CmdLine.get_socket_log(),CmdLine.get_emulation_mode());


	//wait for sigkill or sigterm signal
	AppTimer.wait_for_exit_signal();//loop till KILL or TERM signal is received
	AppTimer.stop_timer();//stop sending heart-beats to other objects
	return 0;
}
