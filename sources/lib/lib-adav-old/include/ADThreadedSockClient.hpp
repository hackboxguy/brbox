#ifndef __AD_THREADED_SOCK_CLIENT_H_
#define __AD_THREADED_SOCK_CLIENT_H_
#include "ADGenericChain.hpp"
#include "ADThread.hpp"
#include "ADCmnCltCmdProcessor.hpp"
#include "ADJsonRpcClient.hpp"

/*****************************************************************************/
//to understand this, read C++ subject observer pattern
class ADThreadedSockClientProducer; //subject
class ADThreadedSockClientConsumer //observer
{
public:
	virtual int run_user_command(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)=0;
	virtual ~ADThreadedSockClientConsumer(){};
};
class ADThreadedSockClientProducer
{
	static int IDGenerator;
	ADThreadedSockClientConsumer *pConsumer;//consumer object where notification goes to
	int id;//id number for consumer to distinguish between many producers
protected:
	int is_helper_attached(void)
	{
		if(pConsumer==NULL)		
			return -1;
		return 0;
	}
	int run_user_command(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList)
	{
		if(pConsumer!=NULL)		
			return pConsumer->run_user_command(pCmdObj,pSrvSockConn,pOutMsgList,this);
		return -1;
	}
public:
	ADThreadedSockClientProducer() {id=IDGenerator++;pConsumer=NULL;}
	virtual ~ADThreadedSockClientProducer(){};
	int attach_helper(ADThreadedSockClientConsumer* c)
	{
		//allow only one Consumer to be attached		
		if(pConsumer==NULL)
		{
			pConsumer=c;
			return id;
		}
		else
			return -1;//some other Consumer has already been attached
	}
	int getID(){return id;}
};
/*****************************************************************************/
class ADThreadedSockClient:public ADThreadedSockClientProducer, public ADChainConsumer, public ADThreadConsumer, public ADCmnCltCmdProcessor
{
	char ip_addr[255];
	int port;
	unsigned char running;//run status
	unsigned char stop_running;//signal to end thread

	ADThread cmd_thread;//thread for running client commands
	ADGenericChain cmd_chain;//all the commands to run are in this chain.
	ADGenericChain output_msg_chain;//for synchronized cout of messages
	int output_msg_chain_id;

	//ADGenericChain callbacks
	virtual int identify_chain_element(void* element,int ident,ADChainProducer* pObj);
	virtual int double_identify_chain_element(void* element,int ident1,int ident2,ADChainProducer* pObj);
	virtual int free_chain_element_data(void* element,ADChainProducer* pObj);

	//ADThread callbacks
	virtual int monoshot_callback_function(void* pUserData,ADThreadProducer* pObj);
	virtual int thread_callback_function(void* pUserData,ADThreadProducer* pObj);

	int initialize_helpers(void);
	//int set_server_addr_info(char* srv_ip,int srv_port);
	int start_command_execution();
	int stop_command_execution();
	int copy_command_chain(ADGenericChain *pDest,ADGenericChain *pSrc);
	int print_output_message(OutputPrintMsgObj* pOutput);

public:
	ADThreadedSockClient();
	//ADThreadedSockClient(char* srv_ip,int srv_port);
	~ADThreadedSockClient();
	int run_commands(char* srv_ip,int srv_port,ADGenericChain *pSrcChain);
	int is_running();
	int print_command_result();
	int my_log_print_message(ADJsonRpcClient *pSrvSockConn,char* rpc_method_name,RPC_SRV_ACT action,char* result,ADGenericChain *pOutMsgList);

};
#endif

