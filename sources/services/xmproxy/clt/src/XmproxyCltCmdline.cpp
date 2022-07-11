#include "XmproxyCltCmdline.h"
#include "ADJsonRpcMgr.hpp"
using namespace std;
/*****************************************************************************/
XmproxyCltCmdline::XmproxyCltCmdline()
{
	CmdlineHelper.attach_helper(this);
	CmdlineHelper.insert_options_entry((char*)"asynctasksts" ,optional_argument,EJSON_XMPROXY_RPC_GET_ASYNCTASK);
	CmdlineHelper.insert_help_entry((char*)"--asynctasksts             [read async-task-in-progress if any]");
	CmdlineHelper.insert_options_entry((char*)"connectsts" ,optional_argument,EJSON_XMPROXY_RPC_GET_ONLINE_STATUS);
	CmdlineHelper.insert_help_entry((char*)"--connectsts               [get the online status of xmpp connection]");
	CmdlineHelper.insert_options_entry((char*)"relaymsg" ,optional_argument,EJSON_XMPROXY_RPC_SET_SEND_MESSAGE);
	CmdlineHelper.insert_help_entry((char*)"--relaymsg=<to>,<msg>      [send the message to a recipient's address]");
	/*CmdlineHelper.insert_options_entry((char*)"deleteall" ,optional_argument,EJSON_BBOXSMS_RPC_SMS_DELETE_ALL);
	CmdlineHelper.insert_help_entry((char*)"--deleteall                [delete all sms]");
	CmdlineHelper.insert_options_entry((char*)"delete" ,optional_argument,EJSON_BBOXSMS_RPC_SMS_DELETE);
	CmdlineHelper.insert_help_entry((char*)"--delete=index             [delete an sms of a given index]");
	CmdlineHelper.insert_options_entry((char*)"gettotal" ,optional_argument,EJSON_BBOXSMS_RPC_SMS_TOTAL_GET);
	CmdlineHelper.insert_help_entry((char*)"--gettotal                 [check how many sms are available in sim]");
	CmdlineHelper.insert_options_entry((char*)"getsms" ,optional_argument,EJSON_BBOXSMS_RPC_SMS_GET);
	CmdlineHelper.insert_help_entry((char*)"--getsms=index             [read the sms of zero based index]");
	CmdlineHelper.insert_options_entry((char*)"updatelist" ,optional_argument,EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE);
	CmdlineHelper.insert_help_entry((char*)"--updatelist               [check for any new messages and update sms inbox ]");
	CmdlineHelper.insert_options_entry((char*)"devident" ,optional_argument,EJSON_BBOXSMS_RPC_SMS_IDENTIFY_DEV);
	CmdlineHelper.insert_help_entry((char*)"--devident                 [identify if sms modem is accessible]");
	*/
}
/*****************************************************************************/
XmproxyCltCmdline::~XmproxyCltCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int XmproxyCltCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	EJSON_XMPROXY_RPC_TYPES command =(EJSON_XMPROXY_RPC_TYPES)arg;
	switch(command)
	{
		case EJSON_XMPROXY_RPC_GET_ASYNCTASK:
			{
			const char *table[]   = XMPROXY_RPC_ASYNCTASK_ARG_TABL;
			CmdlineHelper.push_single_enum_get_set_command( EJSON_XMPROXY_RPC_GET_ASYNCTASK,
			EJSON_XMPROXY_RPC_GET_ASYNCTASK,XMPROXY_RPC_ASYNCTASK_GET,
			XMPROXY_RPC_ASYNCTASK_GET,&table[0],XMPROXY_ASYNCTASK_UNKNOWN,
			(char*)XMPROXY_RPC_ASYNCTASK_ARG,sub_arg);
			}
			break;
		case EJSON_XMPROXY_RPC_GET_ONLINE_STATUS:
			{
				const char *table[]   = XMPROXY_RPC_ONLINE_STATUS_ARG_TABL;
				CmdlineHelper.push_single_enum_get_set_command( EJSON_XMPROXY_RPC_GET_ONLINE_STATUS,
				EJSON_XMPROXY_RPC_SET_ONLINE_STATUS,XMPROXY_RPC_ONLINE_STATUS_GET,
				XMPROXY_RPC_ONLINE_STATUS_SET,&table[0],XMPROXY_ONLINESTS_UNKNOWN,
				(char*)XMPROXY_RPC_ONLINE_STATUS_ARG,sub_arg);
			}
			break;
		case EJSON_XMPROXY_RPC_SET_SEND_MESSAGE:
			push_send_msg_command(sub_arg,(char*)XMPROXY_RPC_SEND_MESSAGE_SET,EJSON_XMPROXY_RPC_SET_SEND_MESSAGE,
						     (char*)XMPROXY_RPC_SEND_MESSAGE_TO_ARG,(char*)XMPROXY_RPC_SEND_MESSAGE_MSG_ARG);//,
						     //(char*)SMARTEYE_RPC_COMPARE_IMG_ARGDIFF);
			break;
		default:
			return 0;
			break;
	}
	return 0;
}

int XmproxyCltCmdline::run_my_commands(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	switch(pCmdObj->command)
	{
		case EJSON_XMPROXY_RPC_SET_SEND_MESSAGE:
			run_send_msg_command(pCmdObj,pSrvSockConn,pOutMsgList,pWorker);
			break;
		default:return -1;
			break;
	}
	return 0;
}
/*****************************************************************************/
int XmproxyCltCmdline::run_my_autotest(char* ip,int interval_us,int max_loop,int test_num)
{
	return 0;
}
/*****************************************************************************/
#include "SrcControlVersion.h"
int XmproxyCltCmdline::print_my_version()
{
	printf("version - %05d\n",SRC_CONTROL_VERSION);
	return 0;
}
int XmproxyCltCmdline::get_my_server_port()
{
	return XMPROXY_JSON_PORT_NUMBER;
}
/*****************************************************************************/
int XmproxyCltCmdline::parse_cmdline_arguments(int argc, char **argv)
{
	return CmdlineHelper.parse_cmdline_arguments(argc,argv);
}
/*****************************************************************************/
int XmproxyCltCmdline::push_get_indexed_msg_command(char* subarg,char* rpc_name,int rpc_index,char* arg_name,char* result_name)
{
	return 0;
}
int XmproxyCltCmdline::run_get_indexed_msg_command(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	return 0;
}
/*****************************************************************************/
int XmproxyCltCmdline::push_send_msg_command(char* subarg,char* rpc_name,int rpc_index,char* arg_name,char* arg_name2)//,char* result_name)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	if(CmdlineHelper.get_next_subargument(&subarg)==0)//user must specify to address
	{
		OBJ_MEM_DELETE(pCmdObj);
		printf("please specify to address to whom to send the message\n");
		return -1;
	}
	else
		strcpy(pCmdObj->first_arg_param_value,subarg);
	if(CmdlineHelper.get_next_subargument(&subarg)==0)//user must specify message
	{
		OBJ_MEM_DELETE(pCmdObj);
		printf("please specify the message\n");
		return -1;
	}
	else
		strcpy(pCmdObj->second_arg_param_value,subarg);

	strcpy(pCmdObj->get_rpc_name,rpc_name);
	strcpy(pCmdObj->first_arg_param_name,arg_name);
	strcpy(pCmdObj->second_arg_param_name,arg_name2);
	//strcpy(pCmdObj->third_arg_param_name,result_name);
	pCmdObj->command=rpc_index;
	pCmdObj->action=RPC_SRV_ACT_WRITE;
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_USER_DEFINED;
	//put the request into chain
	if(CmdlineHelper.CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_compare_img_command: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
int XmproxyCltCmdline::run_send_msg_command(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	ADThreadedSockClient *pOrig = (ADThreadedSockClient*)pWorker;
	{
	pCmdObj->result=pSrvSockConn->set_double_string_type(pCmdObj->get_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value);//,pCmdObj->third_arg_param_name,pCmdObj->third_arg_param_value);
	//pOrig->log_print_message(pSrvSockConn,pCmdObj->get_rpc_name,RPC_SRV_ACT_READ,pCmdObj->result,pOutMsgList);//,pCmdObj->third_arg_param_value);
	pOrig->log_print_message(pSrvSockConn,pCmdObj->get_rpc_name,RPC_SRV_ACT_WRITE,pCmdObj->result,pOutMsgList,"");
	}
	return 0;
}
/*****************************************************************************/