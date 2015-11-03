#include "BboxsmsCltCmdline.h"

using namespace std;
/*****************************************************************************/
BboxsmsCltCmdline::BboxsmsCltCmdline()
{
	CmdlineHelper.attach_helper(this);
	CmdlineHelper.insert_options_entry((char*)"deleteall" ,optional_argument,EJSON_BBOXSMS_RPC_SMS_DELETE_ALL);
	CmdlineHelper.insert_help_entry((char*)"--deleteall                [delete all sms]");
	CmdlineHelper.insert_options_entry((char*)"delete" ,optional_argument,EJSON_BBOXSMS_RPC_SMS_DELETE);
	CmdlineHelper.insert_help_entry((char*)"--delete=index             [delete an sms of a given index]");
	CmdlineHelper.insert_options_entry((char*)"gettotal" ,optional_argument,EJSON_BBOXSMS_RPC_SMS_TOTAL_GET);
	CmdlineHelper.insert_help_entry((char*)"--gettotal                 [check how many sms are available in sim]");
}
/*****************************************************************************/
BboxsmsCltCmdline::~BboxsmsCltCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int BboxsmsCltCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	EJSON_BBOXSMS_RPC_TYPES command =(EJSON_BBOXSMS_RPC_TYPES)arg;
	switch(command)
	{
		case EJSON_BBOXSMS_RPC_SMS_DELETE_ALL:
			//{
			//const char *table[]   = SMARTEYE_RPC_ID_PATTERN_CHECK_ARG_TABL;
			//CmdlineHelper.push_single_enum_get_set_command(
			//				EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN,EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN,
			//				SMARTEYE_RPC_ID_PATTERN_CHECK,SMARTEYE_RPC_ID_PATTERN_CHECK,
			//				&table[0],EJSON_SMARTEYE_IDPATTERN_UNKNOWN,
			//				(char*)SMARTEYE_RPC_ID_PATTERN_CHECK_ARG,sub_arg);
			//}
			break;
		case EJSON_BBOXSMS_RPC_SMS_DELETE:
			//CmdlineHelper.push_string_get_set_command(EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET,EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_SET,
			//SMARTEYE_RPC_DEBUG_OUTFILE_GET,SMARTEYE_RPC_DEBUG_OUTFILE_SET,
			//(char*)SMARTEYE_RPC_DEBUG_OUTFILE_ARG,sub_arg);
			break;
		case EJSON_BBOXSMS_RPC_SMS_TOTAL_GET:
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

int BboxsmsCltCmdline::run_my_commands(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	switch(pCmdObj->command)
	{
		default:return -1;
			break;
	}
	return 0;
}
/*****************************************************************************/
int BboxsmsCltCmdline::run_my_autotest(char* ip,int interval_us,int max_loop,int test_num)
{
	return 0;
}
/*****************************************************************************/
#include "SrcControlVersion.h"
int BboxsmsCltCmdline::print_my_version()
{
	printf("version - %05d\n",SRC_CONTROL_VERSION);
	return 0;
}
int BboxsmsCltCmdline::get_my_server_port()
{
	return BBOXSMS_JSON_PORT_NUMBER;
}
/*****************************************************************************/
int BboxsmsCltCmdline::parse_cmdline_arguments(int argc, char **argv)
{
	return CmdlineHelper.parse_cmdline_arguments(argc,argv);
}
/*****************************************************************************/

