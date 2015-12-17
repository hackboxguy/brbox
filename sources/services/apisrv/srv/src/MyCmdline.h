#ifndef __MY_CMDLINE_H_
#define __MY_CMDLINE_H_
#include "ADCmdlineHelper.hpp"
#include "ADCmnStringProcessor.hpp"
#define APISRV_DEFAULT_HTTP_PORT 8080

class MyCmdline:public ADCmdlineHelperConsumer ,public ADCmnStringProcessor
{
	int http_port;
	int port_number;
	char version_number[255];
	ADCmdlineHelper CmdlineHelper;
	//char LoginFilePath[512];


	//Chain-callback functions	
	virtual int parse_my_cmdline_options(int arg, char* sub_arg);
	virtual int run_my_commands(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker);//char* ip);
	virtual int run_my_autotest(char* ip,int interval_us,int max_loop,int test_num);
	virtual int print_my_version();
	virtual int get_my_server_port();
public:
	MyCmdline(CMDLINE_HELPER_MODE cmdline_mode,int portnum,char* version_str);
	~MyCmdline();
	int parse_cmdline_arguments(int argc, char **argv);
	bool get_emulation_mode();
	int is_help_printed();
	int get_socket_log();
	bool get_debug_log();
	int get_port_number();
	int get_dev_info(ADCMN_DEV_INFO *pInfo);
	//int get_login_filepath(char* filepath);
	int get_http_port();
	//service specific part
};
#endif
