#include "ADThreadedSockClient.hpp"
#include <string.h>
#include <iostream>
#include <iomanip>
using namespace std;
/*****************************************************************************/
int ADThreadedSockClientProducer::IDGenerator = 0;//generate Unique ID for every consumer object
/*****************************************************************************/
//ADGenericChain callbacks
int ADThreadedSockClient::identify_chain_element(void* element,int ident,ADChainProducer* pObj)
{
	return -1;
}
int ADThreadedSockClient::double_identify_chain_element(void* element,int ident1,int ident2,ADChainProducer* pObj)
{
	return -1;
}
int ADThreadedSockClient::free_chain_element_data(void* element,ADChainProducer* pObj)
{
	return 0;
}
/*****************************************************************************/
//ADThread callbacks(this thread function sends the response data back to client)
int ADThreadedSockClient::monoshot_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	return 0;
}
int ADThreadedSockClient::thread_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	CmdExecutionObj *pCmdObj=NULL;
	ADJsonRpcClient SrvSockConn;
	int commands=cmd_chain.get_chain_size();
	if(commands==0)
	{
		//printf("no command execution is requested!!! try with --help\n");
		running=0;
		return -1;//no commands are requested
	}
	if(SrvSockConn.rpc_server_connect(ip_addr,port)!=0)
	{
		running=0;
		return -1;
	}
	while((pCmdObj=(CmdExecutionObj*)cmd_chain.chain_get()) != NULL)
	{
		switch(pCmdObj->cmd_type)
		{
			case CLIENT_CMD_TYPE_ACTION_NO_ARG:
				run_cmd_type_action_noarg(pCmdObj,&SrvSockConn,&output_msg_chain);
				break;
			case CLIENT_CMD_TYPE_ENUM_GET_SET:
				run_cmd_type_enum_get_set(pCmdObj,&SrvSockConn,&output_msg_chain);
				break;
			case CLIENT_CMD_TYPE_INT_GET_SET:
				run_cmd_type_int_get_set(pCmdObj,&SrvSockConn,&output_msg_chain);
				break;
			case CLIENT_CMD_TYPE_INT_GET_SET_WITH_DEV_ADDR:
				run_cmd_type_int_get_set_with_dev_addr(pCmdObj,&SrvSockConn,&output_msg_chain);
				break;
			case CLIENT_CMD_TYPE_STRING_GET_SET_WITH_DEV_ADDR:
				run_cmd_type_string_get_set_with_dev_addr(pCmdObj,&SrvSockConn,&output_msg_chain);
				break;
			case CLIENT_CMD_TYPE_ENUM_GET_SET_WITH_DEV_ADDR:
				run_cmd_type_enum_get_set_with_dev_addr(pCmdObj,&SrvSockConn,&output_msg_chain);
				break;
			case CLIENT_CMD_TYPE_STRING_GET_SET_WITH_ENUM_PARA:
				run_cmd_type_string_get_set_with_enum_para(pCmdObj,&SrvSockConn,&output_msg_chain);
				break;
			case CLIENT_CMD_TYPE_GET_TASK_STATUS:
				run_get_task_progress_command(pCmdObj,&SrvSockConn,&output_msg_chain);
				break;
			case CLIENT_CMD_TYPE_GET_TASK_STATUS_WITH_DEV_ADDR:
				run_get_task_progress_command_with_dev_addr(pCmdObj,&SrvSockConn,&output_msg_chain);
				break;
			case CLIENT_CMD_TYPE_STRING_GET_SET:
				run_cmd_type_string_get_set(pCmdObj,&SrvSockConn,&output_msg_chain);
				break;
			case CLIENT_CMD_TYPE_DOUBLE_INT_SET:
				run_cmd_type_double_int_set(pCmdObj,&SrvSockConn,&output_msg_chain);
				break;
			//case CLIENT_CMD_TYPE_USER_DEFINED:
			//	run_subscribers_commands(pCmdObj,&SrvSockConn,&output_msg_chain);
			//	break;
			default:
				run_user_command(pCmdObj,&SrvSockConn,&output_msg_chain);
				//run_subscribers_commands(pCmdObj,&SrvSockConn,&output_msg_chain);
				break;
		}
		OBJ_MEM_DELETE(pCmdObj);//delete cmd entry
	}
	SrvSockConn.rpc_server_disconnect();
	running=0;
	return 0;
}
/*****************************************************************************/
int ADThreadedSockClient::is_running()
{
	return (int)running;
}
/*****************************************************************************/
int ADThreadedSockClient::print_output_message(OutputPrintMsgObj* pOutput)
{
	if(strlen(pOutput->result_parameter)==0)
		cout<<setw(15)<<left<<pOutput->ip<<" : "<<setw(15)<<pOutput->exec_time*1000<<" : " <<setw(30)<<left<<pOutput->rpc_method_name<<" : return="<<pOutput->result<<endl;
	else
		cout<<setw(15)<<left<<pOutput->ip<<" : "<<setw(15)<<pOutput->exec_time*1000<<" : " <<setw(30)<<left<<pOutput->rpc_method_name<<" : return="<<pOutput->result<<" : result="<<pOutput->result_parameter<<endl;
	return 0;
}
/*****************************************************************************/
int ADThreadedSockClient::print_command_result()
{
	OutputPrintMsgObj* pOutput=NULL;
	while((pOutput=(OutputPrintMsgObj*)output_msg_chain.chain_get()) != NULL)
	{
		print_output_message(pOutput);
		OBJ_MEM_DELETE(pOutput);
	}
	return 0;
}
/*****************************************************************************/
ADThreadedSockClient::ADThreadedSockClient()
{
	running=0;
	stop_running = 0;
	strcpy(ip_addr,"127.0.0.1");
	port = 0;	
	initialize_helpers();
}
/*****************************************************************************/
ADThreadedSockClient::~ADThreadedSockClient()
{
	stop_command_execution();
	//start_command_execution();
}
/*****************************************************************************/
int ADThreadedSockClient::initialize_helpers(void)
{
	cmd_chain.attach_helper(this);
	output_msg_chain_id=output_msg_chain.attach_helper(this);
	cmd_thread.subscribe_thread_callback(this);
	cmd_thread.set_thread_properties(THREAD_TYPE_NOBLOCK,(void *)this);
	//printf("helpers are initialized\n");
	return 0;
}
/*****************************************************************************/
int ADThreadedSockClient::start_command_execution()//char* srv_ip,int srv_port)
{
	if(running)
	{
		printf("worker is already running\n");
		return -1;//already running
	}
	stop_running = 0;
	cmd_thread.start_thread();
	running=1;//this should have been in thread function, but it is done to ensure that 
		  //is_running() function returns correct status to the caller
	return 0;
}
/*****************************************************************************/
int ADThreadedSockClient::stop_command_execution()//char* srv_ip,int srv_port)
{
	if(running)//no need to stop if not started
	{
		stop_running = 1;//signal thread to stop
		cmd_thread.stop_thread();
	}
	cmd_chain.remove_all();
	return 0;
}
/*****************************************************************************/
int ADThreadedSockClient::run_commands(char* srv_ip,int srv_port,ADGenericChain *pSrcChain)
{
	strcpy(ip_addr,srv_ip);
	port = srv_port;
	copy_command_chain(&cmd_chain,pSrcChain);
	start_command_execution();
	return 0;
}
/*****************************************************************************/
int ADThreadedSockClient::copy_command_chain(ADGenericChain *pDest,ADGenericChain *pSrc)
{
	int commands=pSrc->get_chain_size();
	if(commands==0)
	{
		printf("no command execution is requested!!! try with --help\n");
		return -1;//no commands are requested
	}
	for(int i=0;i<commands;i++)//run all the commands on this ip
	{
		CmdExecutionObj* pCmdObjSrc=NULL;
		pCmdObjSrc=(CmdExecutionObj*)pSrc->chain_get_by_index(i);

		CmdExecutionObj* pCmdObjDest=NULL;
		OBJECT_MEM_NEW(pCmdObjDest,CmdExecutionObj);
		if(pCmdObjDest==NULL)
		{
			printf("failed! unable to allocate memory for cmdObject!\n");
			return -1;
		}
		copy_cmd_exec_obj(pCmdObjDest,pCmdObjSrc);//ADCmnCltCmdProcessor
		if(pDest->chain_put((void *)pCmdObjDest)!=0)
		{
			printf("failed! unable to push cmdObject to chain!\n");
			OBJ_MEM_DELETE(pCmdObjDest);
			return -1;
		}
	}
	return 0;
}
/*****************************************************************************/
int ADThreadedSockClient::my_log_print_message(ADJsonRpcClient *pSrvSockConn/*string ip*/,char* rpc_method_name,RPC_SRV_ACT action,char* result,ADGenericChain *pOutMsgList)
{
	return log_print_message(pSrvSockConn,rpc_method_name,action,result,pOutMsgList);
}



