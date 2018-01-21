#include "DispCltCmdline.h"

using namespace std;
/*****************************************************************************/
DispCltCmdline::DispCltCmdline()
{
	CmdlineHelper.attach_helper(this);
	//CmdlineHelper.insert_options_entry((char*)"gpio" ,optional_argument,EJSON_GPIOCTL_RPC_IO_GET);
	//CmdlineHelper.insert_help_entry((char*)"--gpio=addr,data             (read/write gpio bit value)");

	CmdlineHelper.insert_options_entry((char*)"dispinit" ,optional_argument,EJSON_DISPSRV_RPC_DISP_INIT);
	CmdlineHelper.insert_help_entry((char*)"--dispinit                 [initializes display]");
	CmdlineHelper.insert_options_entry((char*)"dispclear" ,optional_argument,EJSON_DISPSRV_RPC_DISP_CLEAR);
	CmdlineHelper.insert_help_entry((char*)"--dispclear                [clears the display content]");
	CmdlineHelper.insert_options_entry((char*)"printline" ,optional_argument,EJSON_DISPSRV_RPC_DISP_PRINT);
	CmdlineHelper.insert_help_entry((char*)"--printline=line,msg       [print message on display]");
	CmdlineHelper.insert_options_entry((char*)"backlight" ,optional_argument,EJSON_DISPSRV_RPC_DISP_GET_BLKT);
	CmdlineHelper.insert_help_entry((char*)"--backlight=<on/off>       [get/set display backlight status]");

}
/*****************************************************************************/
DispCltCmdline::~DispCltCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int DispCltCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	EJSON_DISPSRV_RPC_TYPES command =(EJSON_DISPSRV_RPC_TYPES)arg;
	switch(command)
	{
		case EJSON_DISPSRV_RPC_DISP_INIT:
			CmdlineHelper.push_action_type_noarg_command(EJSON_DISPSRV_RPC_DISP_INIT,(char*)DISPSRV_RPC_DISP_INIT);
			break;
		case EJSON_DISPSRV_RPC_DISP_CLEAR:
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
			break;
		case EJSON_DISPSRV_RPC_DISP_GET_BLKT:
		case EJSON_DISPSRV_RPC_DISP_SET_BLKT:
			{
			const char *table[]   = DISPSRV_RPC_DISP_BKLT_ARG_TABL;
			CmdlineHelper.push_single_enum_get_set_command(EJSON_DISPSRV_RPC_DISP_GET_BLKT,EJSON_DISPSRV_RPC_DISP_SET_BLKT,
			DISPSRV_RPC_DISP_GET_BKLT,DISPSRV_RPC_DISP_SET_BKLT,&table[0],DISPSRV_BKLT_STS_UNKNOWN,
			(char*)DISPSRV_RPC_DISP_BKLT_ARG,sub_arg);
			}
			break;
		default:
			return 0;
			break;	
	}
	return 0;
}

int DispCltCmdline::run_my_commands(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	switch(pCmdObj->command)
	{
		default:return -1;
			break;
	}
	return 0;
}
/*****************************************************************************/
int DispCltCmdline::run_my_autotest(char* ip,int interval_us,int max_loop,int test_num)
{
	return 0;
}
/*****************************************************************************/
#include "SrcControlVersion.h"
int DispCltCmdline::print_my_version()
{
	printf("version - %05d\n",SRC_CONTROL_VERSION);
	return 0;
}
int DispCltCmdline::get_my_server_port()
{
	return DISPSRV_JSON_PORT_NUMBER;
}
/*****************************************************************************/
int DispCltCmdline::parse_cmdline_arguments(int argc, char **argv)
{
	return CmdlineHelper.parse_cmdline_arguments(argc,argv);
}
/*****************************************************************************/

