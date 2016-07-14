#include "CltCmdline.h"

using namespace std;
/*****************************************************************************/
CltCmdline::CltCmdline()
{
	CmdlineHelper.attach_helper(this);
	//CmdlineHelper.insert_options_entry((char*)"gpio" ,optional_argument,EJSON_GPIOCTL_RPC_IO_GET);
	//CmdlineHelper.insert_help_entry((char*)"--gpio=addr,data             (read/write gpio bit value)");

	CmdlineHelper.insert_options_entry((char*)"sensorinit" ,optional_argument,EJSON_LIGHTSENSE_RPC_SENSOR_INIT);
	CmdlineHelper.insert_help_entry((char*)"--sensorinit               [initializes light sensor]");
	//CmdlineHelper.insert_options_entry((char*)"dispclear" ,optional_argument,EJSON_DISPSRV_RPC_DISP_CLEAR);
	//CmdlineHelper.insert_help_entry((char*)"--dispclear                [clears the display content]");
	//CmdlineHelper.insert_options_entry((char*)"printline" ,optional_argument,EJSON_DISPSRV_RPC_DISP_PRINT);
	//CmdlineHelper.insert_help_entry((char*)"--printline=line,msg       [print message on display]");
}
/*****************************************************************************/
CltCmdline::~CltCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int CltCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	EJSON_LIGHTSENSE_RPC_TYPES command =(EJSON_LIGHTSENSE_RPC_TYPES)arg;
	switch(command)
	{
		case EJSON_LIGHTSENSE_RPC_SENSOR_INIT:
			CmdlineHelper.push_action_type_noarg_command(EJSON_LIGHTSENSE_RPC_SENSOR_INIT,(char*)LIGHTSENSE_RPC_SENSOR_INIT);
			break;
		/*case EJSON_DISPSRV_RPC_DISP_CLEAR:
			CmdlineHelper.push_action_type_noarg_command(EJSON_DISPSRV_RPC_DISP_CLEAR,(char*)DISPSRV_RPC_DISP_CLEAR);
			break;
		case EJSON_DISPSRV_RPC_DISP_PRINT:
			{
			const char *table[]   = DISPSRV_RPC_DISP_LINE_ARG_TABL;
			CmdlineHelper.push_string_get_set_with_enum_arg
(EJSON_DISPSRV_RPC_DISP_PRINT,EJSON_DISPSRV_RPC_DISP_PRINT,DISPSRV_RPC_DISP_PRINT ,DISPSRV_RPC_DISP_PRINT ,
			DISPSRV_RPC_DISP_PRINT_LINE_ARG,EJSON_DISPSRV_LINE_UNKNOWN,&table[0],
			DISPSRV_RPC_DISP_PRINT_MESG_ARG,sub_arg);
			}
			break;*/
		default:
			return 0;
			break;	
	}
	return 0;
}

int CltCmdline::run_my_commands(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	switch(pCmdObj->command)
	{
		default:return -1;
			break;
	}
	return 0;
}
/*****************************************************************************/
int CltCmdline::run_my_autotest(char* ip,int interval_us,int max_loop,int test_num)
{
	return 0;
}
/*****************************************************************************/
#include "SrcControlVersion.h"
int CltCmdline::print_my_version()
{
	printf("version - %05d\n",SRC_CONTROL_VERSION);
	return 0;
}
int CltCmdline::get_my_server_port()
{
	return LIGHTSENSE_JSON_PORT_NUMBER;
}
/*****************************************************************************/
int CltCmdline::parse_cmdline_arguments(int argc, char **argv)
{
	return CmdlineHelper.parse_cmdline_arguments(argc,argv);
}
/*****************************************************************************/
