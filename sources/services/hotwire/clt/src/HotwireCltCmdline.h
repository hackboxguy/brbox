#ifndef __GPIOCLT_CMDLINE_H_
#define __GPIOCLT_CMDLINE_H_
#include "ADCmdlineHelper.hpp"
#include "HotwireJsonDef.h"
#include "ADCmnStringProcessor.hpp"
class GpioCltCmdline:public ADCmdlineHelperConsumer ,public ADCmnStringProcessor
{
	ADCmdlineHelper CmdlineHelper;

	//Chain-callback functions	
	virtual int parse_my_cmdline_options(int arg, char* sub_arg);
	virtual int run_my_commands(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker);//char* ip);
	virtual int run_my_autotest(char* ip,int interval_us,int max_loop,int test_num);
	virtual int print_my_version();
	virtual int get_my_server_port();
public:
	GpioCltCmdline();
	~GpioCltCmdline();
	int parse_cmdline_arguments(int argc, char **argv);
	int push_qrcode_image_cmd(char* subarg);
	int run_qrcode_image_cmd(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,
				ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker);
};
#endif
