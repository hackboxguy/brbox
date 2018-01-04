#include "MyCmdline.h"
using namespace std;
/*****************************************************************************/
typedef enum SYSMGR_CMDLINE_OPT_T
{
	SYSMGR_CMDLINE_OPT_MONITCFG = 100,
	SYSMGR_CMDLINE_OPT_SYSCFG,
	SYSMGR_CMDLINE_OPT_DISPTYPE,
	SYSMGR_CMDLINE_OPT_UNKNOWN,
	SYSMGR_CMDLINE_OPT_NONE
}SYSMGR_CMDLINE_OPT;
/*****************************************************************************/
MyCmdline::MyCmdline(CMDLINE_HELPER_MODE cmdline_mode,int portnum,char* version_str):CmdlineHelper(cmdline_mode)
{
	port_number=portnum;
	strcpy(version_number,version_str);
	CmdlineHelper.attach_helper(this);
	MonitCfgFile="";
	SystemConfig="";
	DisplayType="";
	//note:"hviptdln" are already used by the producer class in library
	CmdlineHelper.insert_options_entry((char*)"evntmonitcfg" ,optional_argument,SYSMGR_CMDLINE_OPT_MONITCFG);
	CmdlineHelper.insert_help_entry((char*)"--evntmonitcfg=filepath         (config file for monitoring the event)");
	CmdlineHelper.insert_options_entry((char*)"syscfg" ,optional_argument,SYSMGR_CMDLINE_OPT_SYSCFG);
	CmdlineHelper.insert_help_entry((char*)"--syscfg=sys_config_str         (specify system type ex:a5v11-xmpp/a5v11-base/3020f-base)");
	CmdlineHelper.insert_options_entry((char*)"disptype" ,optional_argument,SYSMGR_CMDLINE_OPT_DISPTYPE);
	CmdlineHelper.insert_help_entry((char*)"--disptype=type                 (display-type [ssd1306_128x64/ssd1306_128x32])");
}
/*****************************************************************************/
MyCmdline::~MyCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int MyCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	SYSMGR_CMDLINE_OPT command =(SYSMGR_CMDLINE_OPT)arg;
	switch(command)
	{
		case SYSMGR_CMDLINE_OPT_MONITCFG:
			if(CmdlineHelper.get_next_subargument(&sub_arg)==0)//no /dev/tty option passed by user
				MonitCfgFile="";
			else
				MonitCfgFile=sub_arg;
			break;
		case SYSMGR_CMDLINE_OPT_SYSCFG:
			if(CmdlineHelper.get_next_subargument(&sub_arg)==0)//no system-config passed by user
				SystemConfig="none";
			else
				SystemConfig=sub_arg;
			break;
		case SYSMGR_CMDLINE_OPT_DISPTYPE:
			if(CmdlineHelper.get_next_subargument(&sub_arg)==0)//display type to support
				DisplayType="none";
			else
				DisplayType=sub_arg;
			break;
		default:return 0;break;	
	}
	return 0;
}
/*****************************************************************************/
//following function will not be called for server_mode_cmdline_helper
int MyCmdline::run_my_commands(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
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
std::string MyCmdline::get_monit_cfg_file()
{
	return MonitCfgFile;
}
/*****************************************************************************/
std::string MyCmdline::get_sys_config()
{
	return SystemConfig;
}
/*****************************************************************************/
std::string MyCmdline::get_disp_type()
{
	return DisplayType;
}
/*****************************************************************************/

