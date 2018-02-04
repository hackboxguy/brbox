#include "MyCmdline.h"
using namespace std;
#define DEFAULT_LEASEFILE_PATH "/tmp/dhcp.leases"
/*****************************************************************************/
typedef enum LIGHTSENSE_CMDLINE_OPT_T
{
	LIGHTSENSE_CMDLINE_OPT_SENSORTYPE = 100,
	LIGHTSENSE_CMDLINE_OPT_DEVICE, //device node
	LIGHTSENSE_CMDLINE_OPT_LEASEFILE, //sensor service needs to know connected display ip
	LIGHTSENSE_CMDLINE_OPT_UNKNOWN,
	LIGHTSENSE_CMDLINE_OPT_NONE
}LIGHTSENSE_CMDLINE_OPT;
/*****************************************************************************/
MyCmdline::MyCmdline(CMDLINE_HELPER_MODE cmdline_mode,int portnum,char* version_str):CmdlineHelper(cmdline_mode)
{
	SensorType="none";
	DevNode="none";
	LeaseFile=DEFAULT_LEASEFILE_PATH;

	port_number=portnum;
	strcpy(version_number,version_str);
	CmdlineHelper.attach_helper(this);
	//note:"hviptdln" are already used by the producer class in library
	CmdlineHelper.insert_options_entry((char*)"sensortype" ,optional_argument,LIGHTSENSE_CMDLINE_OPT_SENSORTYPE);
	CmdlineHelper.insert_help_entry((char*)"--sensortype=<type>         (light sesor type[taos3414/oosts])");
	CmdlineHelper.insert_options_entry((char*)"device" ,optional_argument,LIGHTSENSE_CMDLINE_OPT_DEVICE);
	CmdlineHelper.insert_help_entry((char*)"--device=/dev/i2c-x         (i2c device node if applicable)");
	CmdlineHelper.insert_options_entry((char*)"leasefile" ,optional_argument,LIGHTSENSE_CMDLINE_OPT_LEASEFILE);
	CmdlineHelper.insert_help_entry((char*)"--leasefile=/lease/file.txt (dhcp leasefile path)");
}
/*****************************************************************************/
MyCmdline::~MyCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int MyCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	LIGHTSENSE_CMDLINE_OPT command =(LIGHTSENSE_CMDLINE_OPT)arg;
	switch(command)
	{
		case LIGHTSENSE_CMDLINE_OPT_SENSORTYPE:
			if(CmdlineHelper.get_next_subargument(&sub_arg)==0)//no display type option passed by user
				SensorType="none";
			else
				SensorType=sub_arg;
			break;
		case LIGHTSENSE_CMDLINE_OPT_DEVICE:
			if(CmdlineHelper.get_next_subargument(&sub_arg)==0)//no /dev/i2c-x option passed by user
				DevNode="none";
			else
				DevNode=sub_arg;
			break;
		case LIGHTSENSE_CMDLINE_OPT_LEASEFILE:
			if(CmdlineHelper.get_next_subargument(&sub_arg)==0)//no /dev/i2c-x option passed by user
				LeaseFile=sub_arg;
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
std::string MyCmdline::get_sensor_type()
{
	return SensorType;
}
/*****************************************************************************/
std::string MyCmdline::get_dev_node()
{
	return DevNode;
}
/*****************************************************************************/
std::string MyCmdline::get_lease_filepath()
{
	return LeaseFile;
}
/*****************************************************************************/


