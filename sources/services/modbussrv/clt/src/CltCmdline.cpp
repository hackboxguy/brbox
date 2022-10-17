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
	CmdlineHelper.insert_options_entry((char*)"iostate" ,optional_argument,EJSON_MODBUSSRV_RPC_IOSTATE_GET);
	CmdlineHelper.insert_help_entry((char*)"--iostate=slaveaddr,ioaddr,<val> [read/write iostate of given slave and ioaddr, val[on/off]");
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
			push_energy_param_command(sub_arg);
			break;
		case EJSON_MODBUSSRV_RPC_IOSTATE_GET:
			push_modbus_iostatecmd(sub_arg);
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
		case EJSON_MODBUSSRV_RPC_ENRGYPARM_GET:
			run_energy_param_command(pCmdObj,pSrvSockConn,pOutMsgList,pWorker);
			break;
		case EJSON_MODBUSSRV_RPC_IOSTATE_GET:
		case EJSON_MODBUSSRV_RPC_IOSTATE_SET:
			run_modbus_iostatecmd(pCmdObj,pSrvSockConn,pOutMsgList,pWorker);
			break;
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
/* ------------------------------------------------------------------------- */
int CltCmdline::push_energy_param_command(char* subarg)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	strcpy(pCmdObj->get_rpc_name,MODBUSSRV_RPC_ENRGYPARM_GET);
	if(CmdlineHelper.get_next_subargument(&subarg)==0)//user must specify either eth0 or eth1
	{
		OBJ_MEM_DELETE(pCmdObj);
		printf("please specify correct energy parameter type []!!!\n");
		return -1;
	}
	else 
	{
		const char *table[]   = MODBUSSRV_RPC_ENRGYPARM_ARG_TABL;
		EJSON_ENRGYPARM module=(EJSON_ENRGYPARM)string_to_enum(table,subarg,EJSON_ENRGYPARM_UNKNOWN);
		if(module>=EJSON_ENRGYPARM_UNKNOWN)
		{
			printf("energy parameters type must be specified\n");
			OBJ_MEM_DELETE(pCmdObj);
			return -1;
		}
		else
		{	
			strcpy(pCmdObj->first_arg_param_name,MODBUSSRV_RPC_ENRGYPARM_ARG);
			strcpy(pCmdObj->first_arg_param_value,table[module]);
			strcpy(pCmdObj->second_arg_param_name,MODBUSSRV_RPC_ENRGYPARM_RESP_ARG);
			pCmdObj->command=EJSON_MODBUSSRV_RPC_ENRGYPARM_GET;
			pCmdObj->action=RPC_SRV_ACT_READ;
		}
	}
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_USER_DEFINED;
	//put the request into chain
	if(CmdlineHelper.CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_energy_param_command: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
int CltCmdline::run_energy_param_command(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
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
		pOrig->my_log_print_message(pSrvSockConn,(char*)"run_energy_param_command",RPC_SRV_ACT_UNKNOWN,(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		return -1;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int CltCmdline::push_modbus_iostatecmd(char* subarg)//,EJSON_MODBUSSRV_RPC_TYPES rpc_cmd)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}

	strcpy(pCmdObj->get_rpc_name,MODBUSSRV_RPC_IOSTATE_GET);
	strcpy(pCmdObj->set_rpc_name,MODBUSSRV_RPC_IOSTATE_SET);

	if(CmdlineHelper.get_next_subargument(&subarg)==0)
	{	
		printf("for modbus iostate get/set, slaveaddr must be specified\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	else 
	{
		strcpy(pCmdObj->first_arg_param_name,MODBUSSRV_RPC_IOSTATE_SLAVEADDR_ARG);
		strcpy(pCmdObj->first_arg_param_value,subarg);

		if(CmdlineHelper.get_next_subargument(&subarg)==0)
		{	
			printf("for modbus iostate get/set, ioaddr must be specified\n");
			OBJ_MEM_DELETE(pCmdObj);
			return -1;
		}
		strcpy(pCmdObj->second_arg_param_name,MODBUSSRV_RPC_IOSTATE_IOADDR_ARG);
		strcpy(pCmdObj->second_arg_param_value,subarg);

		if(CmdlineHelper.get_next_subargument(&subarg)==0)
		{
			//GET cmd
			pCmdObj->command=EJSON_MODBUSSRV_RPC_IOSTATE_GET;
			pCmdObj->action=RPC_SRV_ACT_READ;
			strcpy(pCmdObj->third_arg_param_name,MODBUSSRV_RPC_IOSTATE_ARG);
		}
		else
		{
			//SET cmd
			pCmdObj->command=EJSON_MODBUSSRV_RPC_IOSTATE_SET;
			pCmdObj->action=RPC_SRV_ACT_WRITE;
			strcpy(pCmdObj->third_arg_param_name,MODBUSSRV_RPC_IOSTATE_ARG);
			strcpy(pCmdObj->third_arg_param_value,subarg);
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
/* ------------------------------------------------------------------------- */
int CltCmdline::run_modbus_iostatecmd(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	ADThreadedSockClient *pOrig = (ADThreadedSockClient*)pWorker;
	if(pCmdObj->action == RPC_SRV_ACT_WRITE)
	{
	pCmdObj->result=pSrvSockConn->set_tripple_string_type(pCmdObj->set_rpc_name,
			pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,
			pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value,
			pCmdObj->third_arg_param_name,pCmdObj->third_arg_param_value);
	pOrig->log_print_message(pSrvSockConn,pCmdObj->set_rpc_name,RPC_SRV_ACT_READ,pCmdObj->result,
				pOutMsgList,pCmdObj->third_arg_param_value);
	//pCmdObj->my_log_print_message(pSrvSockConn,pCmdObj->set_rpc_name,RPC_SRV_ACT_WRITE,pCmdObj->result,pOutMsgList);
	}
	else
	{
		pOrig->my_log_print_message(pSrvSockConn,(char*)"run_modbus_iostatecmd",RPC_SRV_ACT_UNKNOWN,
					(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		return -1;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */

