#include "CltCmdline.h"

using namespace std;
/*****************************************************************************/
CltCmdline::CltCmdline()
{
	CmdlineHelper.attach_helper(this);
	CmdlineHelper.insert_options_entry((char*)"sensorinit" ,optional_argument,EJSON_LIGHTSENSE_RPC_SENSOR_INIT);
	CmdlineHelper.insert_help_entry((char*)"--sensorinit               [initializes light sensor]");
	CmdlineHelper.insert_options_entry((char*)"readxyz" ,optional_argument,EJSON_LIGHTSENSE_RPC_READ_XYZ);
	CmdlineHelper.insert_help_entry((char*)"--readxyz                  [reads XYZ values of color]");
	CmdlineHelper.insert_options_entry((char*)"integrtime" ,optional_argument,EJSON_LIGHTSENSE_GET_INTEGRATION_TIME);
	CmdlineHelper.insert_help_entry((char*)"--integrtime=time          [read/write integration time of measurement]");
	CmdlineHelper.insert_options_entry((char*)"syncedge" ,optional_argument,EJSON_LIGHTSENSE_SYNC_EDGE_GET);
	CmdlineHelper.insert_help_entry((char*)"--syncedge=edge            [read/write sync-edge=<lo/hi>]");
	CmdlineHelper.insert_options_entry((char*)"integrmode" ,optional_argument,EJSON_LIGHTSENSE_INTEGRATION_MODE_GET);
	CmdlineHelper.insert_help_entry((char*)"--integrmode=mode          [read/write integration-mode=<free/manual/syncsingle/syncmulti>]");
	CmdlineHelper.insert_options_entry((char*)"intrstopsts" ,optional_argument,EJSON_LIGHTSENSE_INTR_STOP_STS_GET);
	CmdlineHelper.insert_help_entry((char*)"--intrstopsts=sts          [read/write interrupt-stop-status=<lo/hi>]");
	CmdlineHelper.insert_options_entry((char*)"intrmode" ,optional_argument,EJSON_LIGHTSENSE_INTR_MODE_GET);
	CmdlineHelper.insert_help_entry((char*)"--intrmode=mode            [read/write interrupt-mode=<disable/level/smb/smbsimulate>]");
	CmdlineHelper.insert_options_entry((char*)"intrrate" ,optional_argument,EJSON_LIGHTSENSE_INTR_RATE_GET);
	CmdlineHelper.insert_help_entry((char*)"--intrrate=rate            [read/write interrupt-rate=<every/single/100ms/second>]");
	CmdlineHelper.insert_options_entry((char*)"intrsrc" ,optional_argument,EJSON_LIGHTSENSE_INTR_SOURCE_GET);
	CmdlineHelper.insert_help_entry((char*)"--intrsrc=src              [read/write interrupt-source=<green/red/blue/white>]");
	CmdlineHelper.insert_options_entry((char*)"gainmode" ,optional_argument,EJSON_LIGHTSENSE_GAIN_MODE_GET);
	CmdlineHelper.insert_help_entry((char*)"--gainmode=mode            [read/write gain-mode=<1/4/16/64>]");

}
/*****************************************************************************/
CltCmdline::~CltCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int CltCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	EJSON_LIGHTSENSE_RPC_TYPES command =(EJSON_LIGHTSENSE_RPC_TYPES)arg;
	switch(command)
	{
		case EJSON_LIGHTSENSE_RPC_SENSOR_INIT:
			CmdlineHelper.push_action_type_noarg_command(EJSON_LIGHTSENSE_RPC_SENSOR_INIT,(char*)LIGHTSENSE_RPC_SENSOR_INIT);
			break;
		case EJSON_LIGHTSENSE_RPC_READ_XYZ:
			CmdlineHelper.push_action_type_noarg_command(EJSON_LIGHTSENSE_RPC_READ_XYZ,(char*)LIGHTSENSE_RPC_READ_XYZ);
			break;
		case EJSON_LIGHTSENSE_GET_INTEGRATION_TIME:
		case EJSON_LIGHTSENSE_SET_INTEGRATION_TIME:
                        {
                        	CmdlineHelper.push_string_get_set_command(EJSON_LIGHTSENSE_GET_INTEGRATION_TIME,
					EJSON_LIGHTSENSE_SET_INTEGRATION_TIME,(char*)LIGHTSENSE_RPC_GET_INTEGRATION_TIME,
					(char*)LIGHTSENSE_RPC_SET_INTEGRATION_TIME,
				        (char*)LIGHTSENSE_RPC_SET_INTEGRATION_TIME_ARG,sub_arg,RPC_SRV_ACT_READ_AND_WRITE);
                        }
                        break;
		case EJSON_LIGHTSENSE_SYNC_EDGE_GET:
		case EJSON_LIGHTSENSE_SYNC_EDGE_SET:
			{
			const char *table[]   = ADLIB_STATUS_FLAG_TYPE_TABL;
			CmdlineHelper.push_single_enum_get_set_command(EJSON_LIGHTSENSE_SYNC_EDGE_GET,
			EJSON_LIGHTSENSE_SYNC_EDGE_SET,LIGHTSENSE_RPC_SYNC_EDGE_GET,LIGHTSENSE_RPC_SYNC_EDGE_SET,
			&table[0],ADLIB_STATUS_FLAG_TYPE_UNKNOWN,(char*)LIGHTSENSE_RPC_SYNC_EDGE_ARG,sub_arg);
			}
			break;
		case EJSON_LIGHTSENSE_INTEGRATION_MODE_GET:
		case EJSON_LIGHTSENSE_INTEGRATION_MODE_SET:
			{
			const char *table[]   = LS_INTEG_MODE_TABL;
			CmdlineHelper.push_single_enum_get_set_command(EJSON_LIGHTSENSE_INTEGRATION_MODE_GET,
			EJSON_LIGHTSENSE_INTEGRATION_MODE_SET,LIGHTSENSE_RPC_INTEGRATION_MODE_GET,LIGHTSENSE_RPC_INTEGRATION_MODE_SET,
			&table[0],LS_INTEG_MODE_UNKNOWN,(char*)LIGHTSENSE_RPC_INTEGRATION_MODE_ARG,sub_arg);
			}
			break;
		case EJSON_LIGHTSENSE_INTR_STOP_STS_GET:
		case EJSON_LIGHTSENSE_INTR_STOP_STS_SET:
			{
			const char *table[]   = ADLIB_STATUS_FLAG_TYPE_TABL;
			CmdlineHelper.push_single_enum_get_set_command(EJSON_LIGHTSENSE_INTR_STOP_STS_GET,
			EJSON_LIGHTSENSE_INTR_STOP_STS_SET,LIGHTSENSE_RPC_INTR_STOP_STS_GET,LIGHTSENSE_RPC_INTR_STOP_STS_SET,
			&table[0],ADLIB_STATUS_FLAG_TYPE_UNKNOWN,(char*)LIGHTSENSE_RPC_INTR_STOP_STS_ARG,sub_arg);
			}
			break;
		case EJSON_LIGHTSENSE_INTR_MODE_GET:
		case EJSON_LIGHTSENSE_INTR_MODE_SET:
			{
			const char *table[]   = LS_INTR_MODE_TABL;
			CmdlineHelper.push_single_enum_get_set_command(EJSON_LIGHTSENSE_INTR_MODE_GET,
			EJSON_LIGHTSENSE_INTR_MODE_SET,LIGHTSENSE_RPC_INTR_MODE_GET,LIGHTSENSE_RPC_INTR_MODE_SET,
			&table[0],LS_INTR_MODE_UNKNOWN,(char*)LIGHTSENSE_RPC_INTR_MODE_ARG,sub_arg);
			}
			break;
		case EJSON_LIGHTSENSE_INTR_RATE_GET:
		case EJSON_LIGHTSENSE_INTR_RATE_SET:
			{
			const char *table[]   = LS_INTR_RATE_TABL;
			CmdlineHelper.push_single_enum_get_set_command(EJSON_LIGHTSENSE_INTR_RATE_GET,
			EJSON_LIGHTSENSE_INTR_RATE_SET,LIGHTSENSE_RPC_INTR_RATE_GET,LIGHTSENSE_RPC_INTR_RATE_SET,
			&table[0],LS_INTR_RATE_UNKNOWN,(char*)LIGHTSENSE_RPC_INTR_RATE_ARG,sub_arg);
			}
			break;
		case EJSON_LIGHTSENSE_INTR_SOURCE_GET:
		case EJSON_LIGHTSENSE_INTR_SOURCE_SET:
			{
			const char *table[]   = LS_INTR_SOURCE_TABL;
			CmdlineHelper.push_single_enum_get_set_command(EJSON_LIGHTSENSE_INTR_SOURCE_GET,
			EJSON_LIGHTSENSE_INTR_SOURCE_SET,LIGHTSENSE_RPC_INTR_SOURCE_GET,LIGHTSENSE_RPC_INTR_SOURCE_SET,
			&table[0],LS_INTR_SOURCE_UNKNOWN,(char*)LIGHTSENSE_RPC_INTR_SOURCE_ARG,sub_arg);
			}
			break;
		case EJSON_LIGHTSENSE_GAIN_MODE_GET:
		case EJSON_LIGHTSENSE_GAIN_MODE_SET:
			{
			const char *table[]   = LS_GAIN_MODE_TABL;
			CmdlineHelper.push_single_enum_get_set_command(EJSON_LIGHTSENSE_GAIN_MODE_GET,
			EJSON_LIGHTSENSE_GAIN_MODE_SET,LIGHTSENSE_RPC_GAIN_MODE_GET,LIGHTSENSE_RPC_GAIN_MODE_SET,
			&table[0],LS_GAIN_MODE_UNKNOWN,(char*)LIGHTSENSE_RPC_GAIN_MODE_ARG,sub_arg);
			}
			break;
		default:
			return 0;
			break;	
	}
	return 0;
}

int CltCmdline::run_my_commands(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	switch(pCmdObj->command)
	{
		default:return -1;
			break;
	}
	return 0;
}
/*****************************************************************************/
int CltCmdline::run_my_autotest(char* ip,int interval_us,int max_loop,int test_num)
{
	return 0;
}
/*****************************************************************************/
#include "SrcControlVersion.h"
int CltCmdline::print_my_version()
{
	printf("version - %05d\n",SRC_CONTROL_VERSION);
	return 0;
}
int CltCmdline::get_my_server_port()
{
	return LIGHTSENSE_JSON_PORT_NUMBER;
}
/*****************************************************************************/
int CltCmdline::parse_cmdline_arguments(int argc, char **argv)
{
	return CmdlineHelper.parse_cmdline_arguments(argc,argv);
}
/*****************************************************************************/

