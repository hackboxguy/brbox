#include "MyCmdline.h"
using namespace std;
/*****************************************************************************/
typedef enum APISRV_CMDLINE_OPT_T
{
	APISRV_CMDLINE_OPT_HTTPPORT = 100,
	APISRV_CMDLINE_OPT_UNKNOWN,
	APISRV_CMDLINE_OPT_NONE
}APISRV_CMDLINE_OPT;
//#define XMPROXY_DEFAULT_LOGIN_FILE_PATH "/mnt/settings/etc/xmpp.login"
/*****************************************************************************/
MyCmdline::MyCmdline(CMDLINE_HELPER_MODE cmdline_mode,int portnum,char* version_str):CmdlineHelper(cmdline_mode)
{
	http_port  =APISRV_DEFAULT_HTTP_PORT;
	port_number=portnum;
	strcpy(version_number,version_str);
	CmdlineHelper.attach_helper(this);
	//note:"hviptdln" are already used by the producer class in library
//	CmdlineHelper.insert_options_entry((char*)"loginfile" ,optional_argument,XMPROXY_CMDLINE_OPT_LOGIN_FILE);
//	CmdlineHelper.insert_help_entry((char*)"--loginfile=filepath (path to the file having xmpp user/pw details)");
//	strcpy(LoginFilePath,XMPROXY_DEFAULT_LOGIN_FILE_PATH);
	CmdlineHelper.insert_options_entry((char*)"httpport" ,optional_argument,APISRV_CMDLINE_OPT_HTTPPORT);
	CmdlineHelper.insert_help_entry((char*)"--httpport=port             (Http listening port number)");

}
/*****************************************************************************/
MyCmdline::~MyCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int MyCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	APISRV_CMDLINE_OPT command =(APISRV_CMDLINE_OPT)arg;
	switch(command)
	{
		case APISRV_CMDLINE_OPT_HTTPPORT:
			if(CmdlineHelper.get_next_subargument(&sub_arg)==0)//no /dev/tty option passed by user
				http_port=APISRV_DEFAULT_HTTP_PORT;
			else
				http_port=atoi(sub_arg);
			break;
		default:return 0;break;	
	}
	return 0;
}
/*****************************************************************************/
//following function will not be called for server_mode_cmdline_helper
int MyCmdline::run_my_commands(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	switch(pCmdObj->command)
	{
		//case XMPROXY_CMDLINE_OPT_LOGIN_FILE:break;
		default:return -1;
			break;
	}
	return 0;
}
/*****************************************************************************/
//following function will not be called for server_mode_cmdline_helper
int MyCmdline::run_my_autotest(char* ip,int interval_us,int max_loop,int test_num)
{
	return 0;
}
/*****************************************************************************/
int MyCmdline::print_my_version()
{
	printf("version - %s\n",version_number);
	return 0;
}
//default server port number
int MyCmdline::get_my_server_port()
{
	return port_number;//SDSMS_JSON_PORT_NUMBER;
}
/*****************************************************************************/
int MyCmdline::parse_cmdline_arguments(int argc, char **argv)
{
	return CmdlineHelper.parse_cmdline_arguments(argc,argv);
}
/*****************************************************************************/
bool MyCmdline::get_emulation_mode()
{
	if(CmdlineHelper.get_emulation_mode()==CMDLINE_OPT_TYPE_YES)
		return true;
	else
		return false;
}
/*****************************************************************************/
int MyCmdline::is_help_printed()
{
	return CmdlineHelper.is_help_printed();
}
/*****************************************************************************/
int MyCmdline::get_socket_log()
{
	return CmdlineHelper.get_socket_log_opt();
}
bool MyCmdline::get_debug_log()
{
	return CmdlineHelper.get_debug_log_opt();
}
/*****************************************************************************/
int MyCmdline::get_port_number()
{
	return CmdlineHelper.get_port_number();
}
/*****************************************************************************/
int MyCmdline::get_dev_info(ADCMN_DEV_INFO *pInfo)
{
	return CmdlineHelper.get_dev_info(pInfo);
}
/*****************************************************************************/
/*int MyCmdline::get_login_filepath(char* filepath)
{
	strcpy(filepath,LoginFilePath);
	return 0;
}*/
/*****************************************************************************/
int MyCmdline::get_http_port()
{
	return http_port;
}
/*****************************************************************************/



