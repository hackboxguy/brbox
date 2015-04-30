#include "SysmgrCltCmdline.h"

using namespace std;
/*****************************************************************************/
SysmgrCltCmdline::SysmgrCltCmdline()
{
	CmdlineHelper.attach_helper(this);

	CmdlineHelper.insert_options_entry((char*)"mac" ,optional_argument,EJSON_SYSMGR_RPC_GET_MAC_ADDR);
	CmdlineHelper.insert_help_entry((char*)"--mac=ethN,<addr>          [get/set mac address]");
	CmdlineHelper.insert_options_entry((char*)"ethcount" ,optional_argument,EJSON_SYSMGR_RPC_GET_ETH_COUNT);
	CmdlineHelper.insert_help_entry((char*)"--ethcount                 [read total available eth interfaces]");

}
/*****************************************************************************/
SysmgrCltCmdline::~SysmgrCltCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int SysmgrCltCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	EJSON_SYSMGR_RPC_TYPES command =(EJSON_SYSMGR_RPC_TYPES)arg;
	switch(command)
	{
		case EJSON_SYSMGR_RPC_GET_MAC_ADDR:
			{
			const char *table[]   = SYSMGR_RPC_MAC_ADDR_ARG_IFACE_TABL;
			CmdlineHelper.push_string_get_set_with_enum_arg
			(EJSON_SYSMGR_RPC_GET_MAC_ADDR,EJSON_SYSMGR_RPC_SET_MAC_ADDR,SYSMGR_RPC_MAC_ADDR_GET,SYSMGR_RPC_MAC_ADDR_SET,
			SYSMGR_RPC_MAC_ADDR_ARG_IFACE,EJSON_SYSMGR_IFACE_TYPE_UNKNOWN,&table[0],
			SYSMGR_RPC_MAC_ADDR_ARG,sub_arg);
			}
			break;
		case EJSON_SYSMGR_RPC_GET_ETH_COUNT:
			CmdlineHelper.push_single_int_get_set_command
			(EJSON_SYSMGR_RPC_GET_ETH_COUNT,EJSON_SYSMGR_RPC_GET_ETH_COUNT,SYSMGR_RPC_ETH_COUNT_GET,SYSMGR_RPC_ETH_COUNT_GET,
			(char*)SYSMGR_RPC_ETH_COUNT_ARG,sub_arg,1);
			break;
		default:
			return 0;
			break;	
	}
	return 0;
}

int SysmgrCltCmdline::run_my_commands(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	switch(pCmdObj->command)
	{
		default:return -1;
			break;
	}
	return 0;
}
/*****************************************************************************/
int SysmgrCltCmdline::run_my_autotest(char* ip,int interval_us,int max_loop,int test_num)
{
	return 0;
}
/*****************************************************************************/
#include "SrcControlVersion.h"
int SysmgrCltCmdline::print_my_version()
{
	printf("version - %05d\n",SRC_CONTROL_VERSION);
	return 0;
}
int SysmgrCltCmdline::get_my_server_port()
{
	return SYSMGR_JSON_PORT_NUMBER;
}
/*****************************************************************************/
int SysmgrCltCmdline::parse_cmdline_arguments(int argc, char **argv)
{
	return CmdlineHelper.parse_cmdline_arguments(argc,argv);
}
/*****************************************************************************/

