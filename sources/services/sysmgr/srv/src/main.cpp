#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "SysmgrJsonDef.h"
#include "NetRpc.h"
#include "ADJsonRpcMgr.hpp"
#include "MyCmdline.h"
//#include "SrcControlVersion.h"

using namespace std;
int main(int argc, const char* argv[])
{
	MyCmdline CmdLine(CMDLINE_HELPER_MODE_SERVER,46000,(char*)"00.01");//TYSS_VERSION);
	CmdLine.parse_cmdline_arguments(argc,(char**)argv);

	ADJsonRpcMgr     RpcMgr; //main rpc handler
	NetRpc IpAddr(SYSMGR_RPC_IP_ADDR_GET,0);

	RpcMgr.AttachRpc(&IpAddr);

	return 0;
}
