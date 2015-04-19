#include "ADCmnCltCmdProcessor.hpp"
//#include <json.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
using namespace std;
/*****************************************************************************/
ADCmnCltCmdProcessor::ADCmnCltCmdProcessor()
{
}
/*****************************************************************************/
ADCmnCltCmdProcessor::~ADCmnCltCmdProcessor()
{
}
/*****************************************************************************/
int ADCmnCltCmdProcessor::copy_cmd_exec_obj(CmdExecutionObj *pCmdObjDest,CmdExecutionObj *pCmdObjSrc)
{
	pCmdObjDest->command  = pCmdObjSrc->command;
	pCmdObjDest->cmd_type = pCmdObjSrc->cmd_type;
	pCmdObjDest->action   = pCmdObjSrc->action;

	strncpy(pCmdObjDest->get_rpc_name, pCmdObjSrc->get_rpc_name, sizeof(pCmdObjSrc->get_rpc_name) );
	strncpy(pCmdObjDest->set_rpc_name, pCmdObjSrc->set_rpc_name, sizeof(pCmdObjSrc->set_rpc_name) );
	pCmdObjDest->cmd_int_val = pCmdObjSrc->cmd_int_val;//first_arg_int_value
	pCmdObjDest->cmd_enum_val= pCmdObjSrc->cmd_enum_val;//first_arg_enum_value

	strncpy(pCmdObjDest->first_arg_param_name, pCmdObjSrc->first_arg_param_name, sizeof(pCmdObjSrc->first_arg_param_name) );
	strncpy(pCmdObjDest->first_arg_param_value, pCmdObjSrc->first_arg_param_value, sizeof(pCmdObjSrc->first_arg_param_value) );
	pCmdObjDest->first_arg_param_int_value= pCmdObjSrc->first_arg_param_int_value;
    pCmdObjDest->first_arg_param_double_value=pCmdObjSrc->first_arg_param_double_value;


	strncpy(pCmdObjDest->second_arg_param_name, pCmdObjSrc->second_arg_param_name, sizeof(pCmdObjSrc->second_arg_param_name) );
	strncpy(pCmdObjDest->second_arg_param_value, pCmdObjSrc->second_arg_param_value, sizeof(pCmdObjSrc->second_arg_param_value) );
	pCmdObjDest->second_arg_param_int_value= pCmdObjSrc->second_arg_param_int_value;
    pCmdObjDest->second_arg_param_double_value=pCmdObjSrc->second_arg_param_double_value;


	strncpy(pCmdObjDest->third_arg_param_name , pCmdObjSrc->third_arg_param_name, sizeof(pCmdObjSrc->third_arg_param_name) );
	strncpy(pCmdObjDest->third_arg_param_value, pCmdObjSrc->third_arg_param_value, sizeof(pCmdObjSrc->third_arg_param_value) );
	pCmdObjDest->third_arg_param_int_value= pCmdObjSrc->third_arg_param_int_value;
    pCmdObjDest->third_arg_param_double_value=pCmdObjSrc->third_arg_param_double_value;

	strncpy(pCmdObjDest->fourth_arg_param_name , pCmdObjSrc->fourth_arg_param_name, sizeof(pCmdObjSrc->fourth_arg_param_name) );
	strncpy(pCmdObjDest->fourth_arg_param_value, pCmdObjSrc->fourth_arg_param_value, sizeof(pCmdObjSrc->fourth_arg_param_value) );
	pCmdObjDest->fourth_arg_param_int_value= pCmdObjSrc->fourth_arg_param_int_value;
    pCmdObjDest->fourth_arg_param_double_value=pCmdObjSrc->fourth_arg_param_double_value;

	strncpy(pCmdObjDest->fifth_arg_param_name , pCmdObjSrc->fifth_arg_param_name, sizeof(pCmdObjSrc->fifth_arg_param_name) );
	strncpy(pCmdObjDest->fifth_arg_param_value, pCmdObjSrc->fifth_arg_param_value, sizeof(pCmdObjSrc->fifth_arg_param_value) );
	pCmdObjDest->fifth_arg_param_int_value= pCmdObjSrc->fifth_arg_param_int_value;
    pCmdObjDest->fifth_arg_param_double_value=pCmdObjSrc->fifth_arg_param_double_value;

	strncpy(pCmdObjDest->sixth_arg_param_name , pCmdObjSrc->sixth_arg_param_name, sizeof(pCmdObjSrc->sixth_arg_param_name) );
	strncpy(pCmdObjDest->sixth_arg_param_value, pCmdObjSrc->sixth_arg_param_value, sizeof(pCmdObjSrc->sixth_arg_param_value) );
	pCmdObjDest->sixth_arg_param_int_value= pCmdObjSrc->sixth_arg_param_int_value;
    pCmdObjDest->sixth_arg_param_double_value=pCmdObjSrc->sixth_arg_param_double_value;

	strncpy(pCmdObjDest->seventh_arg_param_name , pCmdObjSrc->seventh_arg_param_name, sizeof(pCmdObjSrc->seventh_arg_param_name) );
	strncpy(pCmdObjDest->seventh_arg_param_value, pCmdObjSrc->seventh_arg_param_value, sizeof(pCmdObjSrc->seventh_arg_param_value) );
	pCmdObjDest->seventh_arg_param_int_value= pCmdObjSrc->seventh_arg_param_int_value;
    pCmdObjDest->seventh_arg_param_double_value=pCmdObjSrc->seventh_arg_param_double_value;

	strncpy(pCmdObjDest->eighth_arg_param_name , pCmdObjSrc->eighth_arg_param_name, sizeof(pCmdObjSrc->eighth_arg_param_name) );
	strncpy(pCmdObjDest->eighth_arg_param_value, pCmdObjSrc->eighth_arg_param_value, sizeof(pCmdObjSrc->eighth_arg_param_value) );
	pCmdObjDest->eighth_arg_param_int_value= pCmdObjSrc->eighth_arg_param_int_value;
    pCmdObjDest->eighth_arg_param_double_value=pCmdObjSrc->eighth_arg_param_double_value;

	strncpy(pCmdObjDest->ninth_arg_param_name , pCmdObjSrc->ninth_arg_param_name, sizeof(pCmdObjSrc->ninth_arg_param_name) );
	strncpy(pCmdObjDest->ninth_arg_param_value, pCmdObjSrc->ninth_arg_param_value, sizeof(pCmdObjSrc->ninth_arg_param_value) );
	pCmdObjDest->ninth_arg_param_int_value= pCmdObjSrc->ninth_arg_param_int_value;
    pCmdObjDest->ninth_arg_param_double_value=pCmdObjSrc->ninth_arg_param_double_value;

	strncpy(pCmdObjDest->tenth_arg_param_name , pCmdObjSrc->tenth_arg_param_name, sizeof(pCmdObjSrc->tenth_arg_param_name) );
	strncpy(pCmdObjDest->tenth_arg_param_value, pCmdObjSrc->tenth_arg_param_value, sizeof(pCmdObjSrc->tenth_arg_param_value) );
	pCmdObjDest->tenth_arg_param_int_value= pCmdObjSrc->tenth_arg_param_int_value;
    pCmdObjDest->tenth_arg_param_double_value=pCmdObjSrc->tenth_arg_param_double_value;

	strncpy(pCmdObjDest->eleventh_arg_param_name , pCmdObjSrc->eleventh_arg_param_name, sizeof(pCmdObjSrc->eleventh_arg_param_name) );
	strncpy(pCmdObjDest->eleventh_arg_param_value, pCmdObjSrc->eleventh_arg_param_value, sizeof(pCmdObjSrc->eleventh_arg_param_value) );
	pCmdObjDest->eleventh_arg_param_int_value= pCmdObjSrc->eleventh_arg_param_int_value;
    pCmdObjDest->eleventh_arg_param_double_value=pCmdObjSrc->eleventh_arg_param_double_value;

	strncpy(pCmdObjDest->twelfth_arg_param_name , pCmdObjSrc->twelfth_arg_param_name, sizeof(pCmdObjSrc->twelfth_arg_param_name) );
    strncpy(pCmdObjDest->twelfth_arg_param_value, pCmdObjSrc->twelfth_arg_param_value, sizeof(pCmdObjSrc->twelfth_arg_param_value) );
	pCmdObjDest->twelfth_arg_param_int_value= pCmdObjSrc->twelfth_arg_param_int_value;
    pCmdObjDest->twelfth_arg_param_double_value=pCmdObjSrc->twelfth_arg_param_double_value;

	strncpy(pCmdObjDest->thirteen_arg_param_name , pCmdObjSrc->thirteen_arg_param_name, sizeof(pCmdObjSrc->thirteen_arg_param_name) );
	strncpy(pCmdObjDest->thirteen_arg_param_value, pCmdObjSrc->thirteen_arg_param_value, sizeof(pCmdObjSrc->thirteen_arg_param_value) );
	pCmdObjDest->thirteen_arg_param_int_value= pCmdObjSrc->thirteen_arg_param_int_value;
    pCmdObjDest->thirteen_arg_param_double_value=pCmdObjSrc->thirteen_arg_param_double_value;

	strncpy(pCmdObjDest->fourteen_arg_param_name , pCmdObjSrc->fourteen_arg_param_name, sizeof(pCmdObjSrc->fourteen_arg_param_name) );
	strncpy(pCmdObjDest->fourteen_arg_param_value, pCmdObjSrc->fourteen_arg_param_value, sizeof(pCmdObjSrc->fourteen_arg_param_value) );
	pCmdObjDest->fourteen_arg_param_int_value= pCmdObjSrc->fourteen_arg_param_int_value;

	strncpy(pCmdObjDest->fifteen_arg_param_name , pCmdObjSrc->fifteen_arg_param_name, sizeof(pCmdObjSrc->fifteen_arg_param_name) );
	strncpy(pCmdObjDest->fifteen_arg_param_value, pCmdObjSrc->fifteen_arg_param_value, sizeof(pCmdObjSrc->fifteen_arg_param_value) );
	pCmdObjDest->fifteen_arg_param_int_value= pCmdObjSrc->fifteen_arg_param_int_value;

	strncpy(pCmdObjDest->sixteen_arg_param_name , pCmdObjSrc->sixteen_arg_param_name, sizeof(pCmdObjSrc->sixteen_arg_param_name) );
	strncpy(pCmdObjDest->sixteen_arg_param_value, pCmdObjSrc->sixteen_arg_param_value, sizeof(pCmdObjSrc->sixteen_arg_param_value) );
	pCmdObjDest->sixteen_arg_param_float_value= pCmdObjSrc->sixteen_arg_param_float_value;

	strncpy(pCmdObjDest->seventeen_arg_param_name , pCmdObjSrc->seventeen_arg_param_name, sizeof(pCmdObjSrc->seventeen_arg_param_name) );
	strncpy(pCmdObjDest->seventeen_arg_param_value, pCmdObjSrc->seventeen_arg_param_value, sizeof(pCmdObjSrc->seventeen_arg_param_value) );
	pCmdObjDest->seventeen_arg_param_float_value= pCmdObjSrc->seventeen_arg_param_float_value;

	strncpy(pCmdObjDest->rpc_resp_result, pCmdObjSrc->rpc_resp_result, sizeof(pCmdObjSrc->rpc_resp_result) );
	return 0;
}
/*****************************************************************************/
int ADCmnCltCmdProcessor::log_print_message(ADJsonRpcClient *pSrvSockConn,char* rpc_method_name,RPC_SRV_ACT action,char* result,ADGenericChain *pOutMsgList)
{
	OutputPrintMsgObj *pOutMsg=NULL;
	OBJECT_MEM_NEW(pOutMsg,OutputPrintMsgObj);
	if(pOutMsg==NULL)
	{
		printf("failed! unable to allocate memory for pOutMsg!\n");
		return -1;
	}
	pOutMsg->ip=pSrvSockConn->get_ip_addr();
	strcpy(pOutMsg->rpc_method_name,rpc_method_name);
	pOutMsg->action=action;
	strcpy(pOutMsg->result,result);
	pOutMsg->exec_time=pSrvSockConn->get_command_execution_time();

	if(pOutMsgList->chain_put((void *)pOutMsg)!=0)
	{
		printf("failed! unable to push pOutMsg entry to chain!\n");
		OBJ_MEM_DELETE(pOutMsg);
		return -1;
	}
	return 0;
}
int ADCmnCltCmdProcessor::log_print_message(ADJsonRpcClient *pSrvSockConn,char* rpc_method_name,RPC_SRV_ACT action,RPC_SRV_RESULT result,ADGenericChain *pOutMsgList,char* result_value)
{
	OutputPrintMsgObj *pOutMsg=NULL;
	OBJECT_MEM_NEW(pOutMsg,OutputPrintMsgObj);
	if(pOutMsg==NULL)
	{
		printf("failed! unable to allocate memory for pOutMsg!\n");
		return -1;
	}
	pOutMsg->ip=pSrvSockConn->get_ip_addr();
	strcpy(pOutMsg->rpc_method_name,rpc_method_name);
	pOutMsg->action=action;
	//strcpy(pOutMsg->result,result);
	convert_server_result_to_string(result,pOutMsg->result);
	strcpy(pOutMsg->result_parameter,result_value);
	pOutMsg->exec_time=pSrvSockConn->get_command_execution_time();
	if(pOutMsgList->chain_put((void *)pOutMsg)!=0)
	{
		printf("failed! unable to push pOutMsg entry to chain!\n");
		OBJ_MEM_DELETE(pOutMsg);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
//todo: fix this for fpga-version read as per new implementation
int ADCmnCltCmdProcessor::run_cmd_type_int_get_set(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList)
{
	//int tmp_int;
	if(pCmdObj->action == RPC_SRV_ACT_WRITE)
	{
		pCmdObj->result=pSrvSockConn->set_integer_type(pCmdObj->set_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_int_value);
		log_print_message(pSrvSockConn,pCmdObj->set_rpc_name,RPC_SRV_ACT_WRITE,pCmdObj->result,pOutMsgList,(char*)"");
	}
	else if(pCmdObj->action == RPC_SRV_ACT_READ)
	{
		pCmdObj->result=pSrvSockConn->get_integer_type(pCmdObj->get_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value);
		log_print_message(pSrvSockConn,pCmdObj->get_rpc_name,RPC_SRV_ACT_READ,pCmdObj->result,pOutMsgList,pCmdObj->first_arg_param_value);
	}
	else
	{
		log_print_message(pSrvSockConn,(char*)"run_cmd_type_int_get_set",RPC_SRV_ACT_UNKNOWN,(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
int ADCmnCltCmdProcessor::run_cmd_type_int_get_set_with_dev_addr(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList)
{
	if(pCmdObj->action == RPC_SRV_ACT_WRITE)
	{
		pCmdObj->result=pSrvSockConn->set_integer_type_with_addr_para(pCmdObj->set_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_int_value,pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_int_value);
		log_print_message(pSrvSockConn,pCmdObj->set_rpc_name,RPC_SRV_ACT_WRITE,pCmdObj->result,pOutMsgList,(char*)"");

	}
	else if(pCmdObj->action == RPC_SRV_ACT_READ)
	{
		pCmdObj->result=pSrvSockConn->get_integer_type_with_addr_para(pCmdObj->get_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_int_value,pCmdObj->second_arg_param_name,pCmdObj->first_arg_param_value);//&tmp_int);
		log_print_message(pSrvSockConn,pCmdObj->get_rpc_name,RPC_SRV_ACT_READ,pCmdObj->result,pOutMsgList,pCmdObj->first_arg_param_value);
	}
	else
	{
		log_print_message(pSrvSockConn,(char*)"run_cmd_type_int_get_set",RPC_SRV_ACT_UNKNOWN,(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
int ADCmnCltCmdProcessor::run_cmd_type_enum_get_set_with_dev_addr(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList)
{
	if(pCmdObj->action == RPC_SRV_ACT_WRITE)
	{
pCmdObj->result=pSrvSockConn->set_enum_type_with_addr_para(pCmdObj->set_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_int_value,pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value);
		log_print_message(pSrvSockConn,pCmdObj->set_rpc_name,RPC_SRV_ACT_WRITE,pCmdObj->result,pOutMsgList,(char*)"");
	}
	else if(pCmdObj->action == RPC_SRV_ACT_READ)
	{
pCmdObj->result=pSrvSockConn->get_enum_type_with_addr_para(pCmdObj->get_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_int_value,pCmdObj->second_arg_param_name,pCmdObj->first_arg_param_value);
		log_print_message(pSrvSockConn,pCmdObj->get_rpc_name,RPC_SRV_ACT_READ,pCmdObj->result,pOutMsgList,pCmdObj->first_arg_param_value);
	}
	else
	{
		log_print_message(pSrvSockConn,(char*)"run_cmd_type_enum_get_set_with_dev_addr",RPC_SRV_ACT_UNKNOWN,(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		return -1;
	}
	return 0;
}

/*****************************************************************************/
int ADCmnCltCmdProcessor::run_cmd_type_string_get_set_with_dev_addr(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList)
{
	run_cmd_type_enum_get_set_with_dev_addr(pCmdObj,pSrvSockConn,pOutMsgList);
	return 0;
}
int ADCmnCltCmdProcessor::run_cmd_type_string_get_set(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList)
{
	run_cmd_type_enum_get_set(pCmdObj,pSrvSockConn,pOutMsgList);
	return 0;
}
/*****************************************************************************/
int ADCmnCltCmdProcessor::run_cmd_type_action_noarg(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList)
{
	if(pCmdObj->action == RPC_SRV_ACT_WRITE)
	{
		if(strlen(pCmdObj->first_arg_param_name)==0)
		{
			pCmdObj->result=pSrvSockConn->set_action_noarg_type(pCmdObj->set_rpc_name);
			log_print_message(pSrvSockConn,pCmdObj->set_rpc_name,RPC_SRV_ACT_WRITE,pCmdObj->result,pOutMsgList,(char*)"");
		}
		else
		{
			pCmdObj->result=pSrvSockConn->set_action_noarg_get_single_string_type(pCmdObj->set_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value);
			log_print_message(pSrvSockConn,pCmdObj->set_rpc_name,RPC_SRV_ACT_WRITE,pCmdObj->result,pOutMsgList,pCmdObj->first_arg_param_value);
		}
	}
	else
	{
		log_print_message(pSrvSockConn,(char*)"run_cmd_type_action_noarg",RPC_SRV_ACT_UNKNOWN,(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
int ADCmnCltCmdProcessor::run_cmd_type_enum_get_set(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList)
{
	if(pCmdObj->action == RPC_SRV_ACT_WRITE)
	{
		if(strlen(pCmdObj->first_arg_param_name)==0)
		{
			pCmdObj->result=pSrvSockConn->set_single_string_type(pCmdObj->set_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value);
			log_print_message(pSrvSockConn,pCmdObj->set_rpc_name,RPC_SRV_ACT_WRITE,pCmdObj->result,pOutMsgList,(char*)"");
		}
		else
		{
			//pCmdObj->result=pSrvSockConn->set_action_noarg_type(pCmdObj->set_rpc_name);
			pCmdObj->result=pSrvSockConn->set_single_string_get_single_string_type(pCmdObj->set_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,
							pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value);
			log_print_message(pSrvSockConn,pCmdObj->set_rpc_name,RPC_SRV_ACT_WRITE,pCmdObj->result,pOutMsgList,pCmdObj->second_arg_param_value);//(char*)"");
		}
	}
	else if(pCmdObj->action == RPC_SRV_ACT_READ)
	{
		pCmdObj->result=pSrvSockConn->get_string_type(pCmdObj->get_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value);
		log_print_message(pSrvSockConn,pCmdObj->get_rpc_name,RPC_SRV_ACT_READ,pCmdObj->result,pOutMsgList,pCmdObj->first_arg_param_value);
	}
	else
	{
		log_print_message(pSrvSockConn,(char*)"run_cmd_type_enum_get_set",RPC_SRV_ACT_UNKNOWN,(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
int ADCmnCltCmdProcessor::run_get_task_progress_command(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList) //,ADThreadedSockClientProducer *pWorker)
{
	//ADThreadedSockClient *pOrig = (ADThreadedSockClient*)pWorker;
	pCmdObj->result=pSrvSockConn->get_string_type_with_string_para(pCmdObj->get_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,pCmdObj->second_arg_param_value,pCmdObj->second_arg_param_name);
	log_print_message(pSrvSockConn,pCmdObj->get_rpc_name,RPC_SRV_ACT_READ,pCmdObj->result,pOutMsgList,pCmdObj->second_arg_param_value);
	return 0;
}
/*****************************************************************************/
int ADCmnCltCmdProcessor::run_get_task_progress_command_with_dev_addr(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList)
{

	pCmdObj->result=pSrvSockConn->get_string_type_with_string_para_with_addr_para(pCmdObj->get_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_int_value,pCmdObj->second_arg_param_value,pCmdObj->second_arg_param_name,pCmdObj->third_arg_param_value,pCmdObj->third_arg_param_name);
	log_print_message(pSrvSockConn,pCmdObj->get_rpc_name,RPC_SRV_ACT_READ,pCmdObj->result,pOutMsgList,pCmdObj->third_arg_param_value);
	return 0;
}
/*****************************************************************************/
int ADCmnCltCmdProcessor::run_cmd_type_string_get_set_with_enum_para(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList)
{
	if(pCmdObj->action == RPC_SRV_ACT_WRITE)
	{
	pCmdObj->result=pSrvSockConn->set_double_string_type(pCmdObj->set_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value);
		log_print_message(pSrvSockConn,pCmdObj->set_rpc_name,RPC_SRV_ACT_WRITE,pCmdObj->result,pOutMsgList,(char*)"");
	}
	else if(pCmdObj->action == RPC_SRV_ACT_READ)
	{
		pCmdObj->result=pSrvSockConn->get_string_type_with_string_para(pCmdObj->get_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,pCmdObj->second_arg_param_value,pCmdObj->second_arg_param_name);
		log_print_message(pSrvSockConn,pCmdObj->get_rpc_name,RPC_SRV_ACT_READ,pCmdObj->result,pOutMsgList,pCmdObj->second_arg_param_value);
	}
	else
	{
		//pOrig->my_log_print_message(pSrvSockConn,(char*)"run_mac_get_set_command",RPC_SRV_ACT_UNKNOWN,(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		log_print_message(pSrvSockConn,(char*)"run_cmd_type_string_get_set_with_enum_para",RPC_SRV_ACT_UNKNOWN,(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		return -1;
	}
	return 0;
}
/*****************************************************************************/



