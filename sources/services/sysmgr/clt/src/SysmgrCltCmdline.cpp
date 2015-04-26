#include "SysmgrCltCmdline.h"

using namespace std;
/*****************************************************************************/
SysmgrCltCmdline::SysmgrCltCmdline()
{
	CmdlineHelper.attach_helper(this);
	//note:"hvitdln" are already used by the producer class in library
	//here sdfc specific commandline options can be added
	//CmdlineHelper.insert_options_entry((char*)"tasksts" ,optional_argument,EJSON_SDES_GET_TASK_STS);
	//CmdlineHelper.insert_help_entry((char*)"--tasksts=taskID          (read the status of the task which was in progress)");

	//CmdlineHelper.insert_options_entry((char*)"keypush" ,optional_argument,EJSON_SDES_PUSH_IR_PB_KEY);
	//CmdlineHelper.insert_help_entry((char*)"--keypush=key              [push IR/PB key code via rpc instead of sensing from H/W]");


}
/*****************************************************************************/
SysmgrCltCmdline::~SysmgrCltCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int SysmgrCltCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	/*EJSON_SDES_CMD command =(EJSON_SDES_CMD)arg;
	switch(command)
	{
		case EJSON_SDES_GET_TASK_STS:
	CmdlineHelper.push_get_task_progress_command((char*)SDES_RPC_NAME_GET_TASK_STATUS,(char*)SDES_RPC_ARG_TASK_STS_ID_PARAM,(char*)SDES_RPC_ARG_TASK_STS_PARAM,EJSON_SDES_GET_TASK_STS,sub_arg);
			break;
		case EJSON_SDES_PUSH_IR_PB_KEY:
	CmdlineHelper.push_single_int_get_set_command(EJSON_SDES_PUSH_IR_PB_KEY,EJSON_SDES_PUSH_IR_PB_KEY,SDES_RPC_NAME_PUSH_IR_PB_KEY,SDES_RPC_NAME_PUSH_IR_PB_KEY,(char*)SDES_RPC_KEYVAL_INTG_NAME,sub_arg);
		default:
			return 0;
			break;	
	}*/
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

