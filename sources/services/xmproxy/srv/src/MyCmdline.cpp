#include "MyCmdline.h"
using namespace std;
/*****************************************************************************/
typedef enum XMPROXY_CMDLINE_OPT_T
{
	XMPROXY_CMDLINE_OPT_LOGIN_FILE = 100,
	XMPROXY_CMDLINE_OPT_UNKNOWN,
	XMPROXY_CMDLINE_OPT_NONE
}XMPROXY_CMDLINE_OPT;
#define XMPROXY_DEFAULT_LOGIN_FILE_PATH "/mnt/settings/etc/xmpp.login"
/*****************************************************************************/
MyCmdline::MyCmdline(CMDLINE_HELPER_MODE cmdline_mode,int portnum,char* version_str):CmdlineHelper(cmdline_mode)
{
	port_number=portnum;
	strcpy(version_number,version_str);
	CmdlineHelper.attach_helper(this);
	//note:"hviptdln" are already used by the producer class in library
	CmdlineHelper.insert_options_entry((char*)"loginfile" ,optional_argument,XMPROXY_CMDLINE_OPT_LOGIN_FILE);
	CmdlineHelper.insert_help_entry((char*)"--loginfile=filepath (path to the file having xmpp user/pw details)");
	strcpy(LoginFilePath,XMPROXY_DEFAULT_LOGIN_FILE_PATH);
}
/*****************************************************************************/
MyCmdline::~MyCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int MyCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	XMPROXY_CMDLINE_OPT command =(XMPROXY_CMDLINE_OPT)arg;
	switch(command)
	{
		case XMPROXY_CMDLINE_OPT_LOGIN_FILE:
			if(CmdlineHelper.get_next_subargument(&sub_arg)==0)//no login filepath passed by user
				strcpy(LoginFilePath,XMPROXY_DEFAULT_LOGIN_FILE_PATH);
			else
				strcpy(LoginFilePath,sub_arg);
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
		case XMPROXY_CMDLINE_OPT_LOGIN_FILE:break;
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
int MyCmdline::get_login_filepath(char* filepath)
{
	strcpy(filepath,LoginFilePath);
	return 0;
}
/*****************************************************************************/


