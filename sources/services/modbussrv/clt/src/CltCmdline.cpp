#include "CltCmdline.h"

using namespace std;
/*****************************************************************************/
CltCmdline::CltCmdline()
{
	CmdlineHelper.attach_helper(this);
	CmdlineHelper.insert_options_entry((char*)"enrgyparm" ,optional_argument,EJSON_MODBUSSRV_RPC_ENRGYPARM_GET);
	CmdlineHelper.insert_help_entry((char*)"--enrgyparm=param          [read energy parameter from energy meter]");
	CmdlineHelper.insert_help_entry((char*)"                           [param=voltage/current/power/actpwr/reactpwr/pfactor");
	CmdlineHelper.insert_help_entry((char*)"                            phangle/freq/import_act/export_act/import_react");
	CmdlineHelper.insert_help_entry((char*)"                            export_react/total_act/total_react]");
}
/*****************************************************************************/
CltCmdline::~CltCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int CltCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	EJSON_MODBUSSRV_RPC_TYPES command =(EJSON_MODBUSSRV_RPC_TYPES)arg;
	switch(command)
	{
		case EJSON_MODBUSSRV_RPC_ENRGYPARM_GET:
			CmdlineHelper.push_single_int_get_set_command
			(EJSON_MODBUSSRV_RPC_ENRGYPARM_GET,EJSON_MODBUSSRV_RPC_ENRGYPARM_GET,
			MODBUSSRV_RPC_ENRGYPARM_GET,MODBUSSRV_RPC_ENRGYPARM_GET,
			(char*)MODBUSSRV_RPC_ENRGYPARM_ARG,sub_arg,1);
			//CmdlineHelper.push_int_get_set_with_dev_addr_arg_command(EJSON_I2CSRV_RPC_PCF8574_GET,EJSON_I2CSRV_RPC_PCF8574_SET,
			//I2CSRV_RPC_PCF8574_GET,I2CSRV_RPC_PCF8574_SET,(char*)I2CSRV_RPC_PCF8574_DATA_ARG,(char*)I2CSRV_RPC_PCF8574_ADDR_ARG,-1,sub_arg);
			break;
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
	return MODBUSSRV_JSON_PORT_NUMBER;
}
/*****************************************************************************/
int CltCmdline::parse_cmdline_arguments(int argc, char **argv)
{
	return CmdlineHelper.parse_cmdline_arguments(argc,argv);
}
/*****************************************************************************/

