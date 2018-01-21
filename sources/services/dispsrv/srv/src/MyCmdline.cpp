#include "MyCmdline.h"
using namespace std;
/*****************************************************************************/
typedef enum DISPSRV_CMDLINE_OPT_T
{
	DISPSRV_CMDLINE_OPT_DISPTYPE = 100,
	DISPSRV_CMDLINE_OPT_DEVICE, //device node
	DISPSRV_CMDLINE_OPT_UNKNOWN,
	DISPSRV_CMDLINE_OPT_NONE
}DISPSRV_CMDLINE_OPT;
/*****************************************************************************/
MyCmdline::MyCmdline(CMDLINE_HELPER_MODE cmdline_mode,int portnum,char* version_str):CmdlineHelper(cmdline_mode)
{
	DispType="none";
	DevNode="none";
	port_number=portnum;
	strcpy(version_number,version_str);
	CmdlineHelper.attach_helper(this);
	//note:"hviptdln" are already used by the producer class in library
	CmdlineHelper.insert_options_entry((char*)"disptype" ,optional_argument,DISPSRV_CMDLINE_OPT_DISPTYPE);
	CmdlineHelper.insert_help_entry((char*)"--disptype=<type>           (display type[ssd1306_128x64/ssd1306_128x64_pi/1602_pcf/1604_pcf/2002_pcf/2004_pcf])");
	CmdlineHelper.insert_options_entry((char*)"device" ,optional_argument,DISPSRV_CMDLINE_OPT_DEVICE);
	CmdlineHelper.insert_help_entry((char*)"--device=/dev/i2c-x         (i2c device node if applicable)");
}
/*****************************************************************************/
MyCmdline::~MyCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int MyCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	DISPSRV_CMDLINE_OPT command =(DISPSRV_CMDLINE_OPT)arg;
	switch(command)
	{
		case DISPSRV_CMDLINE_OPT_DISPTYPE:
			if(CmdlineHelper.get_next_subargument(&sub_arg)==0)//no display type option passed by user
				DispType="none";
			else
				DispType=sub_arg;
			break;
		case DISPSRV_CMDLINE_OPT_DEVICE:
			if(CmdlineHelper.get_next_subargument(&sub_arg)==0)//no /dev/i2c-x option passed by user
				DevNode="none";
			else
				DevNode=sub_arg;
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
std::string MyCmdline::get_disp_type()
{
	return DispType;
}
/*****************************************************************************/
std::string MyCmdline::get_dev_node()
{
	return DevNode;
}
/*****************************************************************************/


