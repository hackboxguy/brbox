#include "BboxsmsCltCmdline.h"
#include "ADJsonRpcMgr.hpp"
using namespace std;
/*****************************************************************************/
BboxsmsCltCmdline::BboxsmsCltCmdline()
{
	CmdlineHelper.attach_helper(this);
	CmdlineHelper.insert_options_entry((char*)"asynctasksts" ,optional_argument,EJSON_BBOXSMS_RPC_GET_ASYNCTASK);
	CmdlineHelper.insert_help_entry((char*)"--asynctasksts             [read async-task-in-progress if any]");
	CmdlineHelper.insert_options_entry((char*)"deleteall" ,optional_argument,EJSON_BBOXSMS_RPC_SMS_DELETE_ALL);
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
	CmdlineHelper.insert_options_entry((char*)"sendsms" ,optional_argument,EJSON_BBOXSMS_RPC_SMS_SEND);
	CmdlineHelper.insert_help_entry((char*)"--sendsms=destNum,msg      [send sms msg to destNum]");
	CmdlineHelper.insert_options_entry((char*)"dialvoice" ,optional_argument,EJSON_BBOXSMS_RPC_DIAL_VOICE);
	CmdlineHelper.insert_help_entry((char*)"--dialvoice=destNum        [dial given destNum]");
	CmdlineHelper.insert_options_entry((char*)"dialussd" ,optional_argument,EJSON_BBOXSMS_RPC_DIAL_USSD);
	CmdlineHelper.insert_help_entry((char*)"--dialussd=destNum         [dial ussd code to given destNum]");
	CmdlineHelper.insert_options_entry((char*)"getussdmsg" ,optional_argument,EJSON_BBOXSMS_RPC_USSD_GET);
	CmdlineHelper.insert_help_entry((char*)"--getussdmsg               [read latest updated ussd reply]");
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
		case EJSON_BBOXSMS_RPC_GET_ASYNCTASK:
			{
			const char *table[]   = BBOXSMS_RPC_ASYNCTASK_ARG_TABL;
			CmdlineHelper.push_single_enum_get_set_command( EJSON_BBOXSMS_RPC_GET_ASYNCTASK,
			EJSON_BBOXSMS_RPC_GET_ASYNCTASK,BBOXSMS_RPC_ASYNCTASK_GET,
			BBOXSMS_RPC_ASYNCTASK_GET,&table[0],BBOXSMS_ASYNCTASK_UNKNOWN,
			(char*)BBOXSMS_RPC_ASYNCTASK_ARG,sub_arg);
			}
			break;
		case EJSON_BBOXSMS_RPC_SMS_DELETE_ALL:
			CmdlineHelper.push_action_type_noarg_command(EJSON_BBOXSMS_RPC_SMS_DELETE_ALL,
				(char*)BBOXSMS_RPC_SMS_DELETE_ALL,(char*)RPCMGR_RPC_TASK_STS_ARGID);
			break;
		case EJSON_BBOXSMS_RPC_SMS_DELETE:
			//CmdlineHelper.push_string_get_set_command(EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET,EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_SET,
			//SMARTEYE_RPC_DEBUG_OUTFILE_GET,SMARTEYE_RPC_DEBUG_OUTFILE_SET,
			//(char*)SMARTEYE_RPC_DEBUG_OUTFILE_ARG,sub_arg);
			break;
		case EJSON_BBOXSMS_RPC_SMS_TOTAL_GET:
			CmdlineHelper.push_single_int_get_set_command(EJSON_BBOXSMS_RPC_SMS_TOTAL_GET,EJSON_BBOXSMS_RPC_SMS_TOTAL_GET,
					BBOXSMS_RPC_SMS_TOTAL_GET,BBOXSMS_RPC_SMS_TOTAL_GET,(char*)BBOXSMS_RPC_SMS_ARG_TOTAL,sub_arg,1);
			break;
		case EJSON_BBOXSMS_RPC_SMS_GET:
			push_get_indexed_msg_command(sub_arg,(char*)BBOXSMS_RPC_SMS_GET,EJSON_BBOXSMS_RPC_SMS_GET,
						     (char*)BBOXSMS_RPC_SMS_ARG_INDX,(char*)BBOXSMS_RPC_SMS_ARG_MSG);
			break;
		case EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE:
			//CmdlineHelper.push_action_type_noarg_command(EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE,(char*)BBOXSMS_RPC_SMS_LIST_UPDATE);
			//no inprogress returned for above command
			CmdlineHelper.push_action_type_noarg_command(EJSON_BBOXSMS_RPC_SMS_LIST_UPDATE,
				(char*)BBOXSMS_RPC_SMS_LIST_UPDATE,(char*)RPCMGR_RPC_TASK_STS_ARGID);
			break;
		case EJSON_BBOXSMS_RPC_SMS_IDENTIFY_DEV:
			CmdlineHelper.push_action_type_noarg_command(EJSON_BBOXSMS_RPC_SMS_IDENTIFY_DEV,
				(char*)BBOXSMS_RPC_SMS_IDENTIFY_DEV,(char*)RPCMGR_RPC_TASK_STS_ARGID);
			break;
		case EJSON_BBOXSMS_RPC_SMS_SEND:
			push_send_sms(sub_arg);
			break;
		case EJSON_BBOXSMS_RPC_DIAL_VOICE:
			push_dial_cmd(sub_arg,EJSON_BBOXSMS_RPC_DIAL_VOICE,(char*)BBOXSMS_RPC_DIAL_VOICE);
			break;
		case EJSON_BBOXSMS_RPC_DIAL_USSD:
			push_dial_cmd(sub_arg,EJSON_BBOXSMS_RPC_DIAL_USSD,(char*)BBOXSMS_RPC_DIAL_USSD);
			break;
		case EJSON_BBOXSMS_RPC_USSD_GET:
			CmdlineHelper.push_string_get_set_command(EJSON_BBOXSMS_RPC_USSD_GET,EJSON_BBOXSMS_RPC_USSD_GET,
			BBOXSMS_RPC_USSD_GET,BBOXSMS_RPC_USSD_GET,(char*)BBOXSMS_RPC_SMS_ARG_MSG,sub_arg);
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
		case EJSON_BBOXSMS_RPC_SMS_GET:
			run_get_indexed_msg_command(pCmdObj,pSrvSockConn,pOutMsgList,pWorker);
			break;
		case EJSON_BBOXSMS_RPC_SMS_SEND:
			run_send_sms(pCmdObj,pSrvSockConn,pOutMsgList,pWorker);
			break;
		case EJSON_BBOXSMS_RPC_DIAL_VOICE:
		case EJSON_BBOXSMS_RPC_DIAL_USSD:
			run_dial_cmd(pCmdObj,pSrvSockConn,pOutMsgList,pWorker);
			break;
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
int BboxsmsCltCmdline::push_get_indexed_msg_command(char* subarg,char* rpc_name,int rpc_index,char* arg_name,char* result_name)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	strcpy(pCmdObj->get_rpc_name,rpc_name);
	if(CmdlineHelper.get_next_subargument(&subarg)==0)//user must specify index number
	{
		OBJ_MEM_DELETE(pCmdObj);
		printf("please specify correct msg index number\n");
		return -1;
	}
	else 
	{
			strcpy(pCmdObj->first_arg_param_name,arg_name);
			//strcpy(pCmdObj->first_arg_param_value,subarg);
			pCmdObj->first_arg_param_int_value=atoi(subarg);
			strcpy(pCmdObj->second_arg_param_name,result_name);
			pCmdObj->command=rpc_index;
			pCmdObj->action=RPC_SRV_ACT_READ;
	}
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_USER_DEFINED;
	//put the request into chain
	if(CmdlineHelper.CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_get_indexed_msg_command: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
int BboxsmsCltCmdline::run_get_indexed_msg_command(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	ADThreadedSockClient *pOrig = (ADThreadedSockClient*)pWorker;
	if(pCmdObj->action == RPC_SRV_ACT_READ)
	{
	//following command has different name for req_arg and for resp_arg.
	pCmdObj->result=pSrvSockConn->get_int_type_with_string_para(pCmdObj->get_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_int_value,pCmdObj->second_arg_param_value,pCmdObj->second_arg_param_name);
	pOrig->log_print_message(pSrvSockConn,pCmdObj->get_rpc_name,RPC_SRV_ACT_READ,pCmdObj->result,pOutMsgList,pCmdObj->second_arg_param_value);
	}
	else
	{
		pOrig->my_log_print_message(pSrvSockConn,(char*)"run_get_indexed_msg_command",RPC_SRV_ACT_UNKNOWN,(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		return -1;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int BboxsmsCltCmdline::push_send_sms(char* subarg)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	strcpy(pCmdObj->get_rpc_name,BBOXSMS_RPC_SMS_SEND);
	strcpy(pCmdObj->set_rpc_name,BBOXSMS_RPC_SMS_SEND);
	pCmdObj->command=EJSON_BBOXSMS_RPC_SMS_SEND;
	pCmdObj->action=RPC_SRV_ACT_WRITE;

	if(CmdlineHelper.get_next_subargument(&subarg)==0)
	{	
		printf("for sms-send,  destination phone number must be specified\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	else 
	{
		strcpy(pCmdObj->first_arg_param_name,BBOXSMS_RPC_SMS_ARG_DEST);
		strcpy(pCmdObj->first_arg_param_value,subarg);

		if(CmdlineHelper.get_next_subargument(&subarg)==0)
		{	
			printf("for sms-send,  please specify message\n");
			OBJ_MEM_DELETE(pCmdObj);
			return -1;
		}
		strcpy(pCmdObj->second_arg_param_name,BBOXSMS_RPC_SMS_ARG_MSG);
		strcpy(pCmdObj->second_arg_param_value,subarg);
		strcpy(pCmdObj->third_arg_param_name,RPCMGR_RPC_TASK_STS_ARGID);//taskID
	}
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_USER_DEFINED;
	//put the request into chain
	if(CmdlineHelper.CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push-send-sms: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
int BboxsmsCltCmdline::run_send_sms(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	ADThreadedSockClient *pOrig = (ADThreadedSockClient*)pWorker;
	if(pCmdObj->action == RPC_SRV_ACT_WRITE)
	{
		pCmdObj->result=pSrvSockConn->set_double_string_get_single_string_type(pCmdObj->set_rpc_name,
				pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,
				pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value,
				pCmdObj->third_arg_param_name,pCmdObj->third_arg_param_value);
		pOrig->log_print_message(pSrvSockConn,pCmdObj->set_rpc_name,RPC_SRV_ACT_READ,pCmdObj->result,
				pOutMsgList,pCmdObj->third_arg_param_value);
	}
	else
	{
		pOrig->my_log_print_message(pSrvSockConn,(char*)"run_send_sms",RPC_SRV_ACT_UNKNOWN,
					(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		return -1;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int BboxsmsCltCmdline::push_dial_cmd(char* subarg,EJSON_BBOXSMS_RPC_TYPES cmd,char* cmdname)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	strcpy(pCmdObj->get_rpc_name,cmdname);
	strcpy(pCmdObj->set_rpc_name,cmdname);
	pCmdObj->command=cmd;//EJSON_BBOXSMS_RPC_SMS_SEND;
	pCmdObj->action=RPC_SRV_ACT_WRITE;

	if(CmdlineHelper.get_next_subargument(&subarg)==0)
	{	
		printf("for dialling,  destination phone number must be specified\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	else 
	{
		strcpy(pCmdObj->first_arg_param_name,BBOXSMS_RPC_SMS_ARG_DEST);
		strcpy(pCmdObj->first_arg_param_value,subarg);
		strcpy(pCmdObj->second_arg_param_name,RPCMGR_RPC_TASK_STS_ARGID);//taskID
	}
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_USER_DEFINED;
	//put the request into chain
	if(CmdlineHelper.CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_dial_cmd: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
int BboxsmsCltCmdline::run_dial_cmd(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	ADThreadedSockClient *pOrig = (ADThreadedSockClient*)pWorker;
	if(pCmdObj->action == RPC_SRV_ACT_WRITE)
	{
		pCmdObj->result=pSrvSockConn->set_single_string_get_single_string_type(pCmdObj->set_rpc_name,
				pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,
				pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value);
		pOrig->log_print_message(pSrvSockConn,pCmdObj->set_rpc_name,RPC_SRV_ACT_WRITE,pCmdObj->result,
				pOutMsgList,pCmdObj->second_arg_param_value);
	}
	else
	{
		pOrig->my_log_print_message(pSrvSockConn,(char*)"run_dial_cmd",RPC_SRV_ACT_UNKNOWN,
					(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		return -1;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */

