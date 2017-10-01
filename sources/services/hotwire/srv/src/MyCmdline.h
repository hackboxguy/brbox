#ifndef __MY_CMDLINE_H_
#define __MY_CMDLINE_H_
#include "ADCmdlineHelper.hpp"
#include "ADCmnStringProcessor.hpp"

class MyCmdline:public ADCmdlineHelperConsumer ,public ADCmnStringProcessor
{
	int port_number;
	char version_number[255];
	ADCmdlineHelper CmdlineHelper;
	std::string edid_dvi,edid_hdmi,edid_dp;
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

	//service specific part
	std::string get_edid_dvi();
	std::string get_edid_hdmi();
	std::string get_edid_dp();

};
#endif
