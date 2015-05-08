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
	CmdlineHelper.insert_options_entry((char*)"ethname" ,optional_argument,EJSON_SYSMGR_RPC_GET_ETH_NAME);
	CmdlineHelper.insert_help_entry((char*)"--ethname=index            [read ethname of a given zero-based-index]");

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
		case EJSON_SYSMGR_RPC_GET_ETH_NAME:
			push_get_string_of_index(sub_arg,(char*)SYSMGR_RPC_ETH_NAME_GET,EJSON_SYSMGR_RPC_GET_ETH_NAME,
			(char*)SYSMGR_RPC_ETH_NAME_ARG_INDEX,(char*)SYSMGR_RPC_ETH_NAME_ARG);
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
		case EJSON_SYSMGR_RPC_GET_ETH_NAME:
			run_get_string_of_index(pCmdObj,pSrvSockConn,pOutMsgList,pWorker);
			break;
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
int SysmgrCltCmdline::push_get_string_of_index(char* subarg,char* rpc_name,int rpc_index,char* arg_name,char* result_name)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	strcpy(pCmdObj->get_rpc_name,rpc_name);
	if(CmdlineHelper.get_next_subargument(&subarg)==0)//user must specify string index(zero based)
	{
		OBJ_MEM_DELETE(pCmdObj);
		printf("please specify correct index number\n");
		return -1;
	}
	else 
	{
			strcpy(pCmdObj->first_arg_param_name,arg_name);
			strcpy(pCmdObj->first_arg_param_value,subarg);//index
			strcpy(pCmdObj->second_arg_param_name,result_name);
			pCmdObj->command=rpc_index;
			pCmdObj->action=RPC_SRV_ACT_READ;
	}
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_USER_DEFINED;
	//put the request into chain
	if(CmdlineHelper.CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_get_string_of_index: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
int SysmgrCltCmdline::run_get_string_of_index(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	ADThreadedSockClient *pOrig = (ADThreadedSockClient*)pWorker;
	if(pCmdObj->action == RPC_SRV_ACT_READ)
	{
	//following command has different name for req_arg and for resp_arg.
	pCmdObj->result=pSrvSockConn->get_string_type_with_string_para(pCmdObj->get_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,pCmdObj->second_arg_param_value,pCmdObj->second_arg_param_name);
	pOrig->log_print_message(pSrvSockConn,pCmdObj->get_rpc_name,RPC_SRV_ACT_READ,pCmdObj->result,pOutMsgList,pCmdObj->second_arg_param_value);
	}
	else
	{
		pOrig->my_log_print_message(pSrvSockConn,(char*)"run_get_log_msg_command",RPC_SRV_ACT_UNKNOWN,(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		return -1;
	}
//	printf("run_get_log_msg_command returning\n");
	return 0;
}
/*****************************************************************************/


