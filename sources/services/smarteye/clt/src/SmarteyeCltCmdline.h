#ifndef __SMARTEYECLT_CMDLINE_H_
#define __SMARTEYECLT_CMDLINE_H_
#include "ADCmdlineHelper.hpp"
#include "SmarteyeJsonDef.h"
#include "ADCmnStringProcessor.hpp"
class SmarteyeCltCmdline:public ADCmdlineHelperConsumer ,public ADCmnStringProcessor
{
	ADCmdlineHelper CmdlineHelper;

	//Chain-callback functions	
	virtual int parse_my_cmdline_options(int arg, char* sub_arg);
	virtual int run_my_commands(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker);//char* ip);
	virtual int run_my_autotest(char* ip,int interval_us,int max_loop,int test_num);
	virtual int print_my_version();
	virtual int get_my_server_port();
public:
	SmarteyeCltCmdline();
	~SmarteyeCltCmdline();
	int parse_cmdline_arguments(int argc, char **argv);
	int push_scan_qrcode_command(char* subarg,char* rpc_name,int rpc_index,char* arg_name,char* result_name);
	int run_scan_qrcode_command(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,
					ADThreadedSockClientProducer *pWorker);
	int push_compare_img_command(char* subarg,char* rpc_name,int rpc_index,char* arg_name,char* arg_name2,char* result_name);
	int run_compare_img_command(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,
				        ADThreadedSockClientProducer *pWorker);
};
#endif
