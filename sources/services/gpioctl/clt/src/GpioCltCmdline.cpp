#include "GpioCltCmdline.h"

using namespace std;
/*****************************************************************************/
GpioCltCmdline::GpioCltCmdline()
{
	CmdlineHelper.attach_helper(this);

	//CmdlineHelper.insert_options_entry((char*)"imgcheck" ,optional_argument,EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN);
	//CmdlineHelper.insert_help_entry((char*)"--imgcheck=imgtype         [checks captured image against<ident/red/green/blue/white>]");
	//CmdlineHelper.insert_options_entry((char*)"debugimgfile" ,optional_argument,EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET);
	//CmdlineHelper.insert_help_entry((char*)"--debugimgfile=filepath    [read/write debug image file path]");
}
/*****************************************************************************/
GpioCltCmdline::~GpioCltCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int GpioCltCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	EJSON_GPIOCTL_RPC_TYPES command =(EJSON_GPIOCTL_RPC_TYPES)arg;
	switch(command)
	{
		case EJSON_GPIOCTL_RPC_IO_CONFIG:
			//{
			//const char *table[]   = SMARTEYE_RPC_ID_PATTERN_CHECK_ARG_TABL;
			//CmdlineHelper.push_single_enum_get_set_command(
			//				EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN,EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN,
			//				SMARTEYE_RPC_ID_PATTERN_CHECK,SMARTEYE_RPC_ID_PATTERN_CHECK,
			//				&table[0],EJSON_SMARTEYE_IDPATTERN_UNKNOWN,
			//				(char*)SMARTEYE_RPC_ID_PATTERN_CHECK_ARG,sub_arg);
			//}
			break;
		case EJSON_GPIOCTL_RPC_IO_GET:
			//CmdlineHelper.push_string_get_set_command(EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET,EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_SET,
			//SMARTEYE_RPC_DEBUG_OUTFILE_GET,SMARTEYE_RPC_DEBUG_OUTFILE_SET,
			//(char*)SMARTEYE_RPC_DEBUG_OUTFILE_ARG,sub_arg);
			break;
		default:
			return 0;
			break;	
	}
	return 0;
}

int GpioCltCmdline::run_my_commands(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	switch(pCmdObj->command)
	{
		default:return -1;
			break;
	}
	return 0;
}
/*****************************************************************************/
int GpioCltCmdline::run_my_autotest(char* ip,int interval_us,int max_loop,int test_num)
{
	return 0;
}
/*****************************************************************************/
#include "SrcControlVersion.h"
int GpioCltCmdline::print_my_version()
{
	printf("version - %05d\n",SRC_CONTROL_VERSION);
	return 0;
}
int GpioCltCmdline::get_my_server_port()
{
	return GPIOCTL_JSON_PORT_NUMBER;
}
/*****************************************************************************/
int GpioCltCmdline::parse_cmdline_arguments(int argc, char **argv)
{
	return CmdlineHelper.parse_cmdline_arguments(argc,argv);
}
/*****************************************************************************/

