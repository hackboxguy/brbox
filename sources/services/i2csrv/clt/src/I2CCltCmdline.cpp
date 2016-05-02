#include "I2CCltCmdline.h"

using namespace std;
/*****************************************************************************/
I2CCltCmdline::I2CCltCmdline()
{
	CmdlineHelper.attach_helper(this);
	CmdlineHelper.insert_options_entry((char*)"pcf8574" ,optional_argument,EJSON_I2CSRV_RPC_PCF8574_GET);
	CmdlineHelper.insert_help_entry((char*)"--pcf8574=addr,data        (read/write pcf8574 bit value)");

	//CmdlineHelper.insert_options_entry((char*)"imgcheck" ,optional_argument,EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN);
	//CmdlineHelper.insert_help_entry((char*)"--imgcheck=imgtype         [checks captured image against<ident/red/green/blue/white>]");
	//CmdlineHelper.insert_options_entry((char*)"debugimgfile" ,optional_argument,EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET);
	//CmdlineHelper.insert_help_entry((char*)"--debugimgfile=filepath    [read/write debug image file path]");
}
/*****************************************************************************/
I2CCltCmdline::~I2CCltCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int I2CCltCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	EJSON_GPIOCTL_RPC_TYPES command =(EJSON_GPIOCTL_RPC_TYPES)arg;
	switch(command)
	{
		case EJSON_I2CSRV_RPC_PCF8574_GET://EJSON_GPIOCTL_RPC_IO_SET
			CmdlineHelper.push_int_get_set_with_dev_addr_arg_command(EJSON_I2CSRV_RPC_PCF8574_GET,EJSON_I2CSRV_RPC_PCF8574_SET,
			I2CSRV_RPC_PCF8574_GET,I2CSRV_RPC_PCF8574_SET,(char*)I2CSRV_RPC_PCF8574_DATA_ARG,(char*)I2CSRV_RPC_PCF8574_ADDR_ARG,-1,sub_arg);
			break;
		default:
			return 0;
			break;	
	}
	return 0;
}

int I2CCltCmdline::run_my_commands(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	switch(pCmdObj->command)
	{
		default:return -1;
			break;
	}
	return 0;
}
/*****************************************************************************/
int I2CCltCmdline::run_my_autotest(char* ip,int interval_us,int max_loop,int test_num)
{
	return 0;
}
/*****************************************************************************/
#include "SrcControlVersion.h"
int I2CCltCmdline::print_my_version()
{
	printf("version - %05d\n",SRC_CONTROL_VERSION);
	return 0;
}
int I2CCltCmdline::get_my_server_port()
{
	return I2CSRV_JSON_PORT_NUMBER;
}
/*****************************************************************************/
int I2CCltCmdline::parse_cmdline_arguments(int argc, char **argv)
{
	return CmdlineHelper.parse_cmdline_arguments(argc,argv);
}
/*****************************************************************************/

