#include "SysmgrCltCmdline.h"
#include "ADJsonRpcMgr.hpp"
using namespace std;
/* ------------------------------------------------------------------------- */
SysmgrCltCmdline::SysmgrCltCmdline()
{
	CmdlineHelper.attach_helper(this);
	CmdlineHelper.insert_options_entry((char*)"mac" ,optional_argument,EJSON_SYSMGR_RPC_GET_MAC_ADDR);
	CmdlineHelper.insert_help_entry((char*)"--mac=ethN,<addr>          [get/set mac address]");
	CmdlineHelper.insert_options_entry((char*)"ethcount" ,optional_argument,EJSON_SYSMGR_RPC_GET_ETH_COUNT);
	CmdlineHelper.insert_help_entry((char*)"--ethcount                 [read total available eth interfaces]");
	CmdlineHelper.insert_options_entry((char*)"ethname" ,optional_argument,EJSON_SYSMGR_RPC_GET_ETH_NAME);
	CmdlineHelper.insert_help_entry((char*)"--ethname=index            [read ethname of a given zero-based-index]");
	CmdlineHelper.insert_options_entry((char*)"loadinfo" ,optional_argument,EJSON_SYSMGR_RPC_GET_LOADINFO);
	CmdlineHelper.insert_help_entry((char*)"--loadinfo                 [read system's load and uptime info]");
	CmdlineHelper.insert_options_entry((char*)"meminfo" ,optional_argument,EJSON_SYSMGR_RPC_GET_MEMINFO);
	CmdlineHelper.insert_help_entry((char*)"--meminfo                  [read system's memory info]");
	CmdlineHelper.insert_options_entry((char*)"cpuinfo" ,optional_argument,EJSON_SYSMGR_RPC_GET_CPUINFO);
	CmdlineHelper.insert_help_entry((char*)"--cpuinfo                  [read system's cpu info]");
	CmdlineHelper.insert_options_entry((char*)"devop" ,optional_argument,EJSON_SYSMGR_RPC_GET_DEV_OP);
	CmdlineHelper.insert_help_entry((char*)"--devop=state              [read/write device operation state=<idle/on/standby/reboot>]");
	CmdlineHelper.insert_options_entry((char*)"fversion" ,optional_argument,EJSON_SYSMGR_RPC_GET_FMWVER);
	CmdlineHelper.insert_help_entry((char*)"--fversion=module          [get fmw module version, module=<current/backup/kernel/project>]");
	CmdlineHelper.insert_options_entry((char*)"bootdev" ,optional_argument,EJSON_SYSMGR_RPC_GET_BOOT_SYSTEM);
	CmdlineHelper.insert_help_entry((char*)"--bootdev                  [read currently booted system<brbox1/brbox2>]");
	CmdlineHelper.insert_options_entry((char*)"fupdate" ,optional_argument,EJSON_SYSMGR_RPC_SET_FMWUPDATE);
	CmdlineHelper.insert_help_entry((char*)"--fupdate=module,filepath  [trigger fmw update of defined module=<project>]");
	CmdlineHelper.insert_options_entry((char*)"ftget" ,optional_argument,EJSON_SYSMGR_RPC_SET_DOWNLOADFTP);
	CmdlineHelper.insert_help_entry((char*)"--ftget=srcURL,trgtpath    [download file from the ftp server to defined target_file_path]");
	CmdlineHelper.insert_options_entry((char*)"tftget" ,optional_argument,EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP);
	CmdlineHelper.insert_help_entry((char*)"--tftget=srvip,trgt,src    [download file from tftp server's(srvip) to target_file_path]");
	CmdlineHelper.insert_options_entry((char*)"asynctasksts" ,optional_argument,EJSON_SYSMGR_RPC_GET_ASYNCTASK);
	CmdlineHelper.insert_help_entry((char*)"--asynctasksts             [read async-task-in-progress if any]");
	CmdlineHelper.insert_options_entry((char*)"devtype" ,optional_argument,EJSON_SYSMGR_RPC_GET_DEVTYPE);
	CmdlineHelper.insert_help_entry((char*)"--devtype                  [read device type]");
	CmdlineHelper.insert_options_entry((char*)"hostname" ,optional_argument,EJSON_SYSMGR_RPC_GET_HOSTNAME);
	CmdlineHelper.insert_help_entry((char*)"--hostname                 [read/write hostname]");
	CmdlineHelper.insert_options_entry((char*)"myip" ,optional_argument,EJSON_SYSMGR_RPC_GET_MY_PUBLIC_IP);
	CmdlineHelper.insert_help_entry((char*)"--myip                     [read my internet ip]");
	CmdlineHelper.insert_options_entry((char*)"defhostname" ,optional_argument,EJSON_SYSMGR_RPC_SET_DEFAULT_HOSTNAME);
	CmdlineHelper.insert_help_entry((char*)"--defhostname              [reset hostname to default]");
}
/* ------------------------------------------------------------------------- */
SysmgrCltCmdline::~SysmgrCltCmdline()
{
}
/* ------------------------------------------------------------------------- */
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
		case EJSON_SYSMGR_RPC_GET_LOADINFO:
			push_get_info_command(EJSON_SYSMGR_RPC_GET_LOADINFO,(char*)SYSMGR_RPC_LOADINFO_GET,
			(char*)SYSMGR_RPC_LOADINFO_ARG_CURRENT,(char*)SYSMGR_RPC_LOADINFO_ARG_AVERAGE,
			(char*)SYSMGR_RPC_LOADINFO_ARG_UPTIME);
			break;
		case EJSON_SYSMGR_RPC_GET_MEMINFO:
			push_get_info_command(EJSON_SYSMGR_RPC_GET_MEMINFO,(char*)SYSMGR_RPC_MEMINFO_GET,
			(char*)SYSMGR_RPC_MEMINFO_ARG_MEM,(char*)SYSMGR_RPC_MEMINFO_ARG_MEMFREE,
			(char*)SYSMGR_RPC_MEMINFO_ARG_MEMUSED);
			break;
		case EJSON_SYSMGR_RPC_GET_CPUINFO:
			push_get_info_command(EJSON_SYSMGR_RPC_GET_CPUINFO,(char*)SYSMGR_RPC_CPUINFO_GET,
			(char*)SYSMGR_RPC_CPUINFO_ARG_MODEL,(char*)SYSMGR_RPC_CPUINFO_ARG_CORES,
			(char*)SYSMGR_RPC_CPUINFO_ARG_FREQ);
			break;
		case EJSON_SYSMGR_RPC_GET_DEV_OP://is used of set type as well
			{
			const char *table[]   = SYSMGR_RPC_DEV_OP_ARG_TABL;
			CmdlineHelper.push_single_enum_get_set_command(EJSON_SYSMGR_RPC_GET_DEV_OP,EJSON_SYSMGR_RPC_SET_DEV_OP,
			SYSMGR_RPC_DEV_OP_GET,SYSMGR_RPC_DEV_OP_SET,&table[0],EJSON_SYSMGR_DEV_OP_UNKNOWN,
			(char*)SYSMGR_RPC_DEV_OP_ARG,sub_arg,(char*)RPCMGR_RPC_TASK_STS_ARGID);
			}
			break;
		case EJSON_SYSMGR_RPC_GET_FMWVER:
			push_fmw_version_read_command(sub_arg);
			break;
		case EJSON_SYSMGR_RPC_GET_BOOT_SYSTEM:
			{
			const char *table[]   = SYSMGR_RPC_BOOT_SYSTEM_ARG_TABL;
			CmdlineHelper.push_single_enum_get_set_command(EJSON_SYSMGR_RPC_GET_BOOT_SYSTEM,
			EJSON_SYSMGR_RPC_GET_BOOT_SYSTEM,SYSMGR_RPC_BOOT_SYSTEM_GET,SYSMGR_RPC_BOOT_SYSTEM_GET,
			&table[0],SYSMGR_BOOT_SYSTEM_UNKNOWN,(char*)SYSMGR_RPC_BOOT_SYSTEM_ARG,sub_arg);
			}
			break;
		case EJSON_SYSMGR_RPC_SET_FMWUPDATE:
			push_fmw_update_command(sub_arg);
			break;
		case EJSON_SYSMGR_RPC_SET_DOWNLOADFTP:
		case EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP:
			push_file_download(sub_arg,command);
			break;
		case EJSON_SYSMGR_RPC_GET_ASYNCTASK:
			{
			const char *table[]   = SYSMGR_RPC_ASYNCTASK_ARG_TABL;
			CmdlineHelper.push_single_enum_get_set_command( EJSON_SYSMGR_RPC_GET_ASYNCTASK,
			EJSON_SYSMGR_RPC_GET_ASYNCTASK,SYSMGR_RPC_ASYNCTASK_GET,
			SYSMGR_RPC_ASYNCTASK_GET,&table[0],SYSMGR_ASYNCTASK_UNKNOWN,
			(char*)SYSMGR_RPC_ASYNCTASK_ARG,sub_arg);
			}
			break;
		case EJSON_SYSMGR_RPC_GET_DEVTYPE:
			{
			const char *table[]   = SYSMGR_RPC_DEVTYPE_ARG_TABL;
			CmdlineHelper.push_single_enum_get_set_command( EJSON_SYSMGR_RPC_GET_DEVTYPE,
			EJSON_SYSMGR_RPC_GET_DEVTYPE,SYSMGR_RPC_DEVTYPE_GET,
			SYSMGR_RPC_DEVTYPE_GET,&table[0],ADCMN_BOARD_TYPE_UNKNOWN,
			(char*)SYSMGR_RPC_DEVTYPE_ARG,sub_arg);
			}
			break;
		case EJSON_SYSMGR_RPC_GET_HOSTNAME:
			CmdlineHelper.push_string_get_set_command(EJSON_SYSMGR_RPC_GET_HOSTNAME,EJSON_SYSMGR_RPC_SET_HOSTNAME,
			SYSMGR_RPC_HOSTNAME_GET,SYSMGR_RPC_HOSTNAME_SET,(char*)SYSMGR_RPC_HOSTNAME_ARG,sub_arg);
			break;
		case EJSON_SYSMGR_RPC_GET_MY_PUBLIC_IP:
			CmdlineHelper.push_string_get_set_command(EJSON_SYSMGR_RPC_GET_MY_PUBLIC_IP,EJSON_SYSMGR_RPC_GET_MY_PUBLIC_IP,
			SYSMGR_RPC_MY_PUBLIC_IP_GET,SYSMGR_RPC_MY_PUBLIC_IP_GET,(char*)SYSMGR_RPC_MY_PUBLIC_IP_ARG,sub_arg);
			break;

		case EJSON_SYSMGR_RPC_SET_DEFAULT_HOSTNAME:
			CmdlineHelper.push_action_type_noarg_command(EJSON_SYSMGR_RPC_SET_DEFAULT_HOSTNAME,(char*)SYSMGR_RPC_DEFAULT_HOSTNAME_SET);
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
		case EJSON_SYSMGR_RPC_GET_LOADINFO:
		case EJSON_SYSMGR_RPC_GET_MEMINFO:
		case EJSON_SYSMGR_RPC_GET_CPUINFO:
			run_get_info_command(pCmdObj,pSrvSockConn,pOutMsgList,pWorker);
			break;
		case EJSON_SYSMGR_RPC_GET_FMWVER:
			run_fmw_version_command(pCmdObj,pSrvSockConn,pOutMsgList,pWorker);
			break;
		case EJSON_SYSMGR_RPC_SET_FMWUPDATE:
			run_fmw_update_command(pCmdObj,pSrvSockConn,pOutMsgList,pWorker);
			break;
		case EJSON_SYSMGR_RPC_SET_DOWNLOADFTP:
		case EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP:
			run_file_download(pCmdObj,pSrvSockConn,pOutMsgList,pWorker);
			break;
		default:return -1;
			break;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int SysmgrCltCmdline::run_my_autotest(char* ip,int interval_us,int max_loop,int test_num)
{
	return 0;
}
/* ------------------------------------------------------------------------- */
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
/* ------------------------------------------------------------------------- */
int SysmgrCltCmdline::parse_cmdline_arguments(int argc, char **argv)
{
	return CmdlineHelper.parse_cmdline_arguments(argc,argv);
}
/* ------------------------------------------------------------------------- */
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
/* ------------------------------------------------------------------------- */
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
/* ------------------------------------------------------------------------- */
int SysmgrCltCmdline::push_get_info_command(int cmd,char* rpc_name,char* arg1_name,char* arg2_name,char* arg3_name)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	pCmdObj->command=cmd;
	strcpy(pCmdObj->get_rpc_name,rpc_name);
	pCmdObj->action=RPC_SRV_ACT_READ;
	strcpy(pCmdObj->first_arg_param_name,arg1_name);
	strcpy(pCmdObj->second_arg_param_name,arg2_name);
	strcpy(pCmdObj->third_arg_param_name,arg3_name);
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_USER_DEFINED;
	//put the request into chain
	if(CmdlineHelper.CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_get_info_command: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
int SysmgrCltCmdline::run_get_info_command(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	ADThreadedSockClient *pOrig = (ADThreadedSockClient*)pWorker;
	if(pCmdObj->action != RPC_SRV_ACT_READ)
	{
		pOrig->my_log_print_message(pSrvSockConn,(char*)"get_info_command",RPC_SRV_ACT_UNKNOWN,(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		return -1;
	}
	pCmdObj->result=pSrvSockConn->get_three_string_type(pCmdObj->get_rpc_name,pCmdObj->first_arg_param_name,
			pCmdObj->first_arg_param_value,pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value,
			pCmdObj->third_arg_param_name,pCmdObj->third_arg_param_value);
	sprintf(pCmdObj->fourth_arg_param_value,"%s:%s,%s:%s,%s:%s",pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,
							  pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value,
							  pCmdObj->third_arg_param_name,pCmdObj->third_arg_param_value);
	pOrig->log_print_message(pSrvSockConn,pCmdObj->get_rpc_name,RPC_SRV_ACT_READ,pCmdObj->result,
			pOutMsgList,pCmdObj->fourth_arg_param_value);
	return 0;
}
/* ------------------------------------------------------------------------- */
int SysmgrCltCmdline::push_fmw_version_read_command(char* subarg)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	strcpy(pCmdObj->get_rpc_name,SYSMGR_RPC_FMW_VER_GET);
	if(CmdlineHelper.get_next_subargument(&subarg)==0)//user must specify either eth0 or eth1
	{
		OBJ_MEM_DELETE(pCmdObj);
		printf("please specify correct module type []!!!\n");
		return -1;
	}
	else 
	{
		const char *table[]   = SYSMGR_RPC_FMW_VER_ARG_TABL;
		SYSMGR_FMW_MODULE_TYPE module=(SYSMGR_FMW_MODULE_TYPE)string_to_enum(table,subarg,SYSMGR_FMW_MODULE_UNKNOWN);
		if(module>=SYSMGR_FMW_MODULE_UNKNOWN)
		{
			printf("For fmw version read, module_type must be specified\n");
			OBJ_MEM_DELETE(pCmdObj);
			return -1;
		}
		else
		{	
			strcpy(pCmdObj->first_arg_param_name,SYSMGR_RPC_FMW_VER_ARG_MODULE);//"module"
			strcpy(pCmdObj->first_arg_param_value,table[module]);//"MrcSpiImg"
			strcpy(pCmdObj->second_arg_param_name,SYSMGR_RPC_FMW_VER_ARG);//"version"
			pCmdObj->command=EJSON_SYSMGR_RPC_GET_FMWVER;
			pCmdObj->action=RPC_SRV_ACT_READ;
		}
	}
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_USER_DEFINED;
	//put the request into chain
	if(CmdlineHelper.CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_fmw_version_read_command: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
int SysmgrCltCmdline::run_fmw_version_command(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
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
		pOrig->my_log_print_message(pSrvSockConn,(char*)"run_fmw_version_command",RPC_SRV_ACT_UNKNOWN,(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		return -1;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int SysmgrCltCmdline::push_fmw_update_command(char* subarg)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	strcpy(pCmdObj->set_rpc_name,SYSMGR_RPC_FMWUPDATE_SET);
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_USER_DEFINED;

	if(CmdlineHelper.get_next_subargument(&subarg)==0)//read
	{	
		printf("for fmw update module_type and file_path must be specified\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	else //set
	{
		const char *table[]   = SYSMGR_RPC_FMWUPDATE_ARG_TABL;
		SYSMGR_FMW_MODULE_TYPE module=(SYSMGR_FMW_MODULE_TYPE)string_to_enum(table,subarg,SYSMGR_FMW_MODULE_UNKNOWN);
		if(module>=SYSMGR_FMW_MODULE_UNKNOWN)
		{
			printf("For fmw update, module_type must be specified\n");
			OBJ_MEM_DELETE(pCmdObj);
			return -1;
		}
		else
		{	
			strcpy(pCmdObj->first_arg_param_name,SYSMGR_RPC_FMWUPDATE_ARG);//"module"
			strcpy(pCmdObj->first_arg_param_value,table[module]);//"module-type"		
			pCmdObj->command=EJSON_SYSMGR_RPC_SET_FMWUPDATE;
			pCmdObj->action=RPC_SRV_ACT_WRITE;
			if(CmdlineHelper.get_next_subargument(&subarg)==0)//read
			{	
				printf("For fmw update, file_path must be specified\n");
				OBJ_MEM_DELETE(pCmdObj);
				return -1;
			}
			strcpy(pCmdObj->second_arg_param_name,SYSMGR_RPC_FMWUPDATE_ARG_FILEPATH);
			strcpy(pCmdObj->second_arg_param_value,subarg);
			strcpy(pCmdObj->third_arg_param_name,RPCMGR_RPC_TASK_STS_ARGID);//taskID
		}
	}
	//put the request into chain
	if(CmdlineHelper.CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
int SysmgrCltCmdline::run_fmw_update_command(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	ADThreadedSockClient *pOrig = (ADThreadedSockClient*)pWorker;
	if(pCmdObj->action == RPC_SRV_ACT_WRITE)
	{
		pCmdObj->result=pSrvSockConn->set_double_string_get_single_string_type(pCmdObj->set_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,
									pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value,
									pCmdObj->third_arg_param_name,pCmdObj->third_arg_param_value);
		pOrig->log_print_message(pSrvSockConn,pCmdObj->set_rpc_name,RPC_SRV_ACT_READ,pCmdObj->result,pOutMsgList,pCmdObj->third_arg_param_value);
	}
	else
	{
		pOrig->my_log_print_message(pSrvSockConn,(char*)"run_fmw_update_command",RPC_SRV_ACT_UNKNOWN,(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		return -1;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int SysmgrCltCmdline::push_file_download(char* subarg,EJSON_SYSMGR_RPC_TYPES rpc_cmd)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	if(rpc_cmd==EJSON_SYSMGR_RPC_SET_DOWNLOADFTP)
	{
		strcpy(pCmdObj->get_rpc_name,SYSMGR_RPC_DOWNLOADFTP_SET);
		strcpy(pCmdObj->set_rpc_name,SYSMGR_RPC_DOWNLOADFTP_SET);
		pCmdObj->command=EJSON_SYSMGR_RPC_SET_DOWNLOADFTP;
	}
	else
	{
		strcpy(pCmdObj->get_rpc_name,SYSMGR_RPC_DOWNLOADTFTP_SET);
		strcpy(pCmdObj->set_rpc_name,SYSMGR_RPC_DOWNLOADTFTP_SET);
		pCmdObj->command=EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP;
	}
	pCmdObj->action=RPC_SRV_ACT_WRITE;

	if(CmdlineHelper.get_next_subargument(&subarg)==0)
	{	
		printf("for file download,  ftp server's URL(or tftp server ip) must be specified\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	else 
	{
		strcpy(pCmdObj->first_arg_param_name,SYSMGR_RPC_DOWNLOADFILE_ARG_SRCADDR);
		strcpy(pCmdObj->first_arg_param_value,subarg);

		if(CmdlineHelper.get_next_subargument(&subarg)==0)
		{	
			printf("for file download,  please specify target download location\n");
			OBJ_MEM_DELETE(pCmdObj);
			return -1;
		}
		strcpy(pCmdObj->second_arg_param_name,SYSMGR_RPC_DOWNLOADFILE_ARG_TARFILE);
		strcpy(pCmdObj->second_arg_param_value,subarg);
		strcpy(pCmdObj->third_arg_param_name,RPCMGR_RPC_TASK_STS_ARGID);//taskID

		if(rpc_cmd==EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP)
		{
			if(CmdlineHelper.get_next_subargument(&subarg)==0)
			{	
				printf("for file download,  tftp server's filename must be specified\n");
				OBJ_MEM_DELETE(pCmdObj);
				return -1;
			}
			else 
			{
				strcpy(pCmdObj->third_arg_param_name,SYSMGR_RPC_DOWNLOADFILE_ARG_SRCFILE);
				strcpy(pCmdObj->third_arg_param_value,subarg);
				//shift taskID string to last as fourth
				strcpy(pCmdObj->fourth_arg_param_name,RPCMGR_RPC_TASK_STS_ARGID);//taskID
			}

		}
	}
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_USER_DEFINED;
	//put the request into chain
	if(CmdlineHelper.CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
int SysmgrCltCmdline::run_file_download(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	ADThreadedSockClient *pOrig = (ADThreadedSockClient*)pWorker;
	if(pCmdObj->action == RPC_SRV_ACT_WRITE)
	{
		if(pCmdObj->command==EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP)
		{
			pCmdObj->result=pSrvSockConn->set_tripple_string_get_single_string_type(pCmdObj->set_rpc_name,
					pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,
					pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value,
					pCmdObj->third_arg_param_name,pCmdObj->third_arg_param_value,
					pCmdObj->fourth_arg_param_name,pCmdObj->fourth_arg_param_value);
					pOrig->log_print_message(pSrvSockConn,pCmdObj->set_rpc_name,RPC_SRV_ACT_READ,
					pCmdObj->result,pOutMsgList,pCmdObj->fourth_arg_param_value);
		}
		else
		{
			pCmdObj->result=pSrvSockConn->set_double_string_get_single_string_type(pCmdObj->set_rpc_name,
					pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,
					pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value,
					pCmdObj->third_arg_param_name,pCmdObj->third_arg_param_value);
			pOrig->log_print_message(pSrvSockConn,pCmdObj->set_rpc_name,RPC_SRV_ACT_READ,pCmdObj->result,
					pOutMsgList,pCmdObj->third_arg_param_value);
		}
	}
	else
	{
		pOrig->my_log_print_message(pSrvSockConn,(char*)"run_ftp_file_download",RPC_SRV_ACT_UNKNOWN,
					(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		return -1;
	}
	return 0;
}

