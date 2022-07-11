#ifndef __XMPROXY_CMDLINE_H_
#define __XMPROXY_CMDLINE_H_
#include "ADCmdlineHelper.hpp"
#include "XmproxyJsonDef.h"
#include "ADCmnStringProcessor.hpp"
class XmproxyCltCmdline:public ADCmdlineHelperConsumer ,public ADCmnStringProcessor
{
	ADCmdlineHelper CmdlineHelper;

	//Chain-callback functions
	virtual int parse_my_cmdline_options(int arg, char* sub_arg);
	virtual int run_my_commands(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker);//char* ip);
	virtual int run_my_autotest(char* ip,int interval_us,int max_loop,int test_num);
	virtual int print_my_version();
	virtual int get_my_server_port();

	int push_get_indexed_msg_command(char* subarg,char* rpc_name,int rpc_index,char* arg_name,char* result_name);
	int run_get_indexed_msg_command(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker);

	int push_send_msg_command(char* subarg,char* rpc_name,int rpc_index,char* arg_name,char* arg_name2);//,char* result_name);
	int run_send_msg_command(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker);

public:
	XmproxyCltCmdline();
	~XmproxyCltCmdline();
	int parse_cmdline_arguments(int argc, char **argv);
};
#endif
