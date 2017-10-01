#include "MyCmdline.h"
using namespace std;
/*****************************************************************************/
typedef enum MPLAYSRV_CMDLINE_OPT_T
{
	MPLAYSRV_CMDLINE_OPT_EDID_DVI = 100,
	MPLAYSRV_CMDLINE_OPT_EDID_HDMI,
	MPLAYSRV_CMDLINE_OPT_EDID_DP,
	MPLAYSRV_CMDLINE_OPT_UNKNOWN,
	MPLAYSRV_CMDLINE_OPT_NONE
}MPLAYSRV_CMDLINE_OPT;
/*****************************************************************************/
MyCmdline::MyCmdline(CMDLINE_HELPER_MODE cmdline_mode,int portnum,char* version_str):CmdlineHelper(cmdline_mode)
{
	edid_dvi="none";edid_hdmi="none";edid_dp="none";

	port_number=portnum;
	strcpy(version_number,version_str);
	CmdlineHelper.attach_helper(this);
	//note:"hviptdln" are already used by the producer class in library
	CmdlineHelper.insert_options_entry((char*)"edid_dvi" ,optional_argument,MPLAYSRV_CMDLINE_OPT_EDID_DVI);
	CmdlineHelper.insert_help_entry((char*)"--edid_dvi=/dev/node       (i2c device node of dvi edid)");
	CmdlineHelper.insert_options_entry((char*)"edid_hdmi" ,optional_argument,MPLAYSRV_CMDLINE_OPT_EDID_HDMI);
	CmdlineHelper.insert_help_entry((char*)"--edid_hdmi=/dev/node      (i2c device node of hdmi edid)");
	CmdlineHelper.insert_options_entry((char*)"edid_dp" ,optional_argument,MPLAYSRV_CMDLINE_OPT_EDID_DP);
	CmdlineHelper.insert_help_entry((char*)"--edid_dp=/dev/node        (i2c device node of dp edid)");
}
/*****************************************************************************/
MyCmdline::~MyCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int MyCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	MPLAYSRV_CMDLINE_OPT command =(MPLAYSRV_CMDLINE_OPT)arg;
	switch(command)
	{
		case MPLAYSRV_CMDLINE_OPT_EDID_DVI:
			if(CmdlineHelper.get_next_subargument(&sub_arg)==0)//no argument passed by user
				edid_dvi="none";
			else
				edid_dvi=sub_arg;
			break;
		case MPLAYSRV_CMDLINE_OPT_EDID_HDMI:
			if(CmdlineHelper.get_next_subargument(&sub_arg)==0)//no argument passed by user
				edid_hdmi="none";
			else
				edid_hdmi=sub_arg;
			break;
		case MPLAYSRV_CMDLINE_OPT_EDID_DP:
			if(CmdlineHelper.get_next_subargument(&sub_arg)==0)//no argument passed by user
				edid_dp="none";
			else
				edid_dp=sub_arg;
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
std::string MyCmdline::get_edid_dvi()
{
	return edid_dvi;
}
std::string MyCmdline::get_edid_hdmi()
{
	return edid_hdmi;
}
std::string MyCmdline::get_edid_dp()
{
	return edid_dp;
}
/*****************************************************************************/

