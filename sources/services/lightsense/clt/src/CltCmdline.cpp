#include "CltCmdline.h"

using namespace std;
/*****************************************************************************/
CltCmdline::CltCmdline()
{
	CmdlineHelper.attach_helper(this);
	CmdlineHelper.insert_options_entry((char*)"sensorinit" ,optional_argument,EJSON_LIGHTSENSE_RPC_SENSOR_INIT);
	CmdlineHelper.insert_help_entry((char*)"--sensorinit               [initializes light sensor]");
	CmdlineHelper.insert_options_entry((char*)"startmeasure" ,optional_argument,EJSON_LIGHTSENSE_RPC_START_MEASURE);
	CmdlineHelper.insert_help_entry((char*)"--startmeasure             [triggers the measurement]");
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
	CmdlineHelper.insert_options_entry((char*)"prescaler" ,optional_argument,EJSON_LIGHTSENSE_PRESCALER_GET);
	CmdlineHelper.insert_help_entry((char*)"--prescaler=value          [read/write prescaler=<div1/div2/div4/div8/div16/div32/div64>]");
	CmdlineHelper.insert_options_entry((char*)"rgbwcount" ,optional_argument,EJSON_LIGHTSENSE_RGBWCOUNT_GET);
	CmdlineHelper.insert_help_entry((char*)"--rgbwcount                [read r/g/b/w count value of tri-stimulus sensor]");
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
		case EJSON_LIGHTSENSE_RPC_START_MEASURE:
			CmdlineHelper.push_action_type_noarg_command(EJSON_LIGHTSENSE_RPC_START_MEASURE,(char*)LIGHTSENSE_RPC_START_MEASURE);
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
		case EJSON_LIGHTSENSE_PRESCALER_GET:
		case EJSON_LIGHTSENSE_PRESCALER_SET:
			{
			const char *table[]   = LS_PRESCALER_TABL;
			CmdlineHelper.push_single_enum_get_set_command(EJSON_LIGHTSENSE_PRESCALER_GET,
			EJSON_LIGHTSENSE_PRESCALER_SET,LIGHTSENSE_RPC_PRESCALER_GET,LIGHTSENSE_RPC_PRESCALER_SET,
			&table[0],LS_PRESCALER_UNKNOWN,(char*)LIGHTSENSE_RPC_PRESCALER_ARG,sub_arg);
			}
			break;
		case EJSON_LIGHTSENSE_RGBWCOUNT_GET:
			push_get_set_rgbwcount(sub_arg);
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
		case EJSON_LIGHTSENSE_RGBWCOUNT_GET:
			run_get_set_rgbwcount(pCmdObj,pSrvSockConn,pOutMsgList,pWorker);
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
//EJSON_SDMS_SET_WALLCONF
int CltCmdline::push_get_set_rgbwcount(char* subarg){
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed to create pCmdObj!!!\n");
		return -1;
	}
	strcpy(pCmdObj->get_rpc_name,LIGHTSENSE_RPC_RGBWCOUNT_GET);
	//strcpy(pCmdObj->set_rpc_name,LIGHTSENSE_RPC_RGBWCOUNT_SET);
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_USER_DEFINED;

	if(CmdlineHelper.get_next_subargument(&subarg)==0)//read
	{
		pCmdObj->command=EJSON_LIGHTSENSE_RGBWCOUNT_GET;
		pCmdObj->action=RPC_SRV_ACT_READ;
		strcpy(pCmdObj->first_arg_param_name,LIGHTSENSE_RPC_RGBWCOUNT_ARGR);
		strcpy(pCmdObj->second_arg_param_name,LIGHTSENSE_RPC_RGBWCOUNT_ARGG);
		strcpy(pCmdObj->third_arg_param_name,LIGHTSENSE_RPC_RGBWCOUNT_ARGB);
		strcpy(pCmdObj->fourth_arg_param_name,LIGHTSENSE_RPC_RGBWCOUNT_ARGW);
	}
	else
	{
		printf("set_rgbwcount command is not allowed\n");
		return -1;
	}
	//put the request into chain
	if(CmdlineHelper.CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("CltCmdline::push_get_set_rgbwcount: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
int CltCmdline::run_get_set_rgbwcount(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	ADThreadedSockClient *pOrig = (ADThreadedSockClient*)pWorker;
	/*if(pCmdObj->action == RPC_SRV_ACT_WRITE)
	{
		pCmdObj->result=pSrvSockConn->set_four_int_type(pCmdObj->set_rpc_name,
				pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_int_value,
				pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_int_value,
				pCmdObj->third_arg_param_name,pCmdObj->third_arg_param_int_value,
				pCmdObj->fourth_arg_param_name,pCmdObj->fourth_arg_param_int_value);
		pOrig->log_print_message(pSrvSockConn,pCmdObj->set_rpc_name,RPC_SRV_ACT_WRITE,pCmdObj->result,pOutMsgList,(char*)"");
	}*/
	if(pCmdObj->action == RPC_SRV_ACT_READ)
	{
		//get_four_int_type
		pCmdObj->result=pSrvSockConn->get_four_int_type(pCmdObj->get_rpc_name,
						pCmdObj->first_arg_param_name,&pCmdObj->first_arg_param_int_value,
						pCmdObj->second_arg_param_name,&pCmdObj->second_arg_param_int_value,
						pCmdObj->third_arg_param_name,&pCmdObj->third_arg_param_int_value,
						pCmdObj->fourth_arg_param_name,&pCmdObj->fourth_arg_param_int_value);
		sprintf(pCmdObj->first_arg_param_value,"rgbwcount_params=%d,%d,%d,%d",pCmdObj->first_arg_param_int_value,
								     pCmdObj->second_arg_param_int_value,
								     pCmdObj->third_arg_param_int_value,
								     pCmdObj->fourth_arg_param_int_value);
		pOrig->log_print_message(pSrvSockConn,pCmdObj->get_rpc_name,RPC_SRV_ACT_READ,
					pCmdObj->result,pOutMsgList,pCmdObj->first_arg_param_value);
	}
	else
	{
		pOrig->my_log_print_message(pSrvSockConn,(char*)"rgbwcount_params",
			RPC_SRV_ACT_UNKNOWN,(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		return -1;
	}
	return 0;
}
/*****************************************************************************/

