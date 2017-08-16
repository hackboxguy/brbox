#include "SmarteyeCltCmdline.h"

using namespace std;
/*****************************************************************************/
SmarteyeCltCmdline::SmarteyeCltCmdline()
{
	CmdlineHelper.attach_helper(this);

	CmdlineHelper.insert_options_entry((char*)"imgcheck" ,optional_argument,EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN);
	CmdlineHelper.insert_help_entry((char*)"--imgcheck=imgtype         [checks captured image against<ident/red/green/blue/white>]");
	CmdlineHelper.insert_options_entry((char*)"debugimgfile" ,optional_argument,EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET);
	CmdlineHelper.insert_help_entry((char*)"--debugimgfile=filepath    [read/write debug image file path]");
	CmdlineHelper.insert_options_entry((char*)"capturejpg" ,optional_argument,EJSON_SMARTEYE_RPC_CAPTURE_JPG_IMG);
	CmdlineHelper.insert_help_entry((char*)"--captureimg               [capture camera frame and save it debugimgfile_path]");
	CmdlineHelper.insert_options_entry((char*)"captureRes" ,optional_argument,EJSON_SMARTEYE_RPC_CAPTURE_RESOLUTION_SET);
	CmdlineHelper.insert_help_entry((char*)"--captureRes=lines,pixels  [set jpg image capture resolution]");
	CmdlineHelper.insert_options_entry((char*)"chkwallfile",optional_argument,EJSON_SMARTEYE_RPC_CHECKWALL_FILE_GET);
	CmdlineHelper.insert_help_entry((char*)"--chkwallfile=filepath     [read/write check wall file path]");
	CmdlineHelper.insert_options_entry((char*)"chkwallbasefile" ,optional_argument,EJSON_SMARTEYE_RPC_CHECKWALL_BASE_FILE_GET);
	CmdlineHelper.insert_help_entry((char*)"--chkwallbasefile=filepath [read/write check wall base file path]");
	CmdlineHelper.insert_options_entry((char*)"scanqrstr" ,optional_argument,EJSON_SMARTEYE_RPC_SCAN_QRSTRING);
	CmdlineHelper.insert_help_entry((char*)"--scanqrstr=filepath       [scans the qrcode encoded in a given file path]");
	CmdlineHelper.insert_options_entry((char*)"cmpimg" ,optional_argument,EJSON_SMARTEYE_RPC_COMPARE_IMG);
	CmdlineHelper.insert_help_entry((char*)"--cmpimg=imgpath1,imgpath2 [compares two images and returns result in %]");

	CmdlineHelper.insert_options_entry((char*)"autoexposure" ,optional_argument,EJSON_SMARTEYE_RPC_AUTO_EXPOSURE_GET);
	CmdlineHelper.insert_help_entry((char*)"--autoexposure=type        [get/set auto-exposure, type=<enable/disable>]");
	CmdlineHelper.insert_options_entry((char*)"exposure" ,optional_argument,EJSON_SMARTEYE_RPC_EXPOSURE_GET);
	CmdlineHelper.insert_help_entry((char*)"--exposure=type            [get/set exposure-value<0 to 250, steps-1]");
	CmdlineHelper.insert_options_entry((char*)"autofocus" ,optional_argument,EJSON_SMARTEYE_RPC_AUTO_FOCUS_GET);
	CmdlineHelper.insert_help_entry((char*)"--autofocus=type           [get/set auto-focus, type=<enable/disable>]");
	CmdlineHelper.insert_options_entry((char*)"focus" ,optional_argument,EJSON_SMARTEYE_RPC_FOCUS_GET);
	CmdlineHelper.insert_help_entry((char*)"--focus=type               [get/set vocus-value<0 to 250, steps-5]");

}
/*****************************************************************************/
SmarteyeCltCmdline::~SmarteyeCltCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int SmarteyeCltCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	EJSON_SMARTEYE_RPC_TYPES command =(EJSON_SMARTEYE_RPC_TYPES)arg;
	switch(command)
	{
		case EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN:
			{
			const char *table[]   = SMARTEYE_RPC_ID_PATTERN_CHECK_ARG_TABL;
			CmdlineHelper.push_single_enum_get_set_command(
							EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN,EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN,
							SMARTEYE_RPC_ID_PATTERN_CHECK,SMARTEYE_RPC_ID_PATTERN_CHECK,
							&table[0],EJSON_SMARTEYE_IDPATTERN_UNKNOWN,
							(char*)SMARTEYE_RPC_ID_PATTERN_CHECK_ARG,sub_arg);
			}
			break;
		case EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET:
			CmdlineHelper.push_string_get_set_command(EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET,EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_SET,
			SMARTEYE_RPC_DEBUG_OUTFILE_GET,SMARTEYE_RPC_DEBUG_OUTFILE_SET,
			(char*)SMARTEYE_RPC_DEBUG_OUTFILE_ARG,sub_arg);
			break;
		case EJSON_SMARTEYE_RPC_CAPTURE_JPG_IMG:
			CmdlineHelper.push_action_type_noarg_command(EJSON_SMARTEYE_RPC_CAPTURE_JPG_IMG,(char*)SMARTEYE_RPC_CAPTURE_JPG_IMG);
			break;
		case EJSON_SMARTEYE_RPC_CAPTURE_RESOLUTION_SET:
			CmdlineHelper.push_int_get_set_with_dev_addr_arg_command(EJSON_SMARTEYE_RPC_CAPTURE_RESOLUTION_SET,
						EJSON_SMARTEYE_RPC_CAPTURE_RESOLUTION_SET,
						SMARTEYE_RPC_CAPTURE_RESOLUTION_SET,
						SMARTEYE_RPC_CAPTURE_RESOLUTION_SET,
						(char*)SMARTEYE_RPC_CAPTURE_RESOLUTION_ARGH,
						(char*)SMARTEYE_RPC_CAPTURE_RESOLUTION_ARGV,-1,sub_arg);
			break;
		case EJSON_SMARTEYE_RPC_CHECKWALL_FILE_GET:
			CmdlineHelper.push_string_get_set_command(EJSON_SMARTEYE_RPC_CHECKWALL_FILE_GET,EJSON_SMARTEYE_RPC_CHECKWALL_FILE_SET,
			SMARTEYE_RPC_CHECKWALL_FILE_GET,SMARTEYE_RPC_CHECKWALL_FILE_SET,
			(char*)SMARTEYE_RPC_DEBUG_OUTFILE_ARG,sub_arg);
			break;
		case EJSON_SMARTEYE_RPC_CHECKWALL_BASE_FILE_GET:
			CmdlineHelper.push_string_get_set_command(EJSON_SMARTEYE_RPC_CHECKWALL_BASE_FILE_GET,EJSON_SMARTEYE_RPC_CHECKWALL_BASE_FILE_SET,
			SMARTEYE_RPC_CHECKWALL_BASE_FILE_GET,SMARTEYE_RPC_CHECKWALL_BASE_FILE_SET,
			(char*)SMARTEYE_RPC_DEBUG_OUTFILE_ARG,sub_arg);
			break;
		case EJSON_SMARTEYE_RPC_SCAN_QRSTRING:
			push_scan_qrcode_command(sub_arg,(char*)SMARTEYE_RPC_SCAN_QRSTRING,EJSON_SMARTEYE_RPC_SCAN_QRSTRING,
						     (char*)SMARTEYE_RPC_SCAN_QRSTRING_ARGFPATH,(char*)SMARTEYE_RPC_SCAN_QRSTRING_ARGQRSTR);
			break;
		case EJSON_SMARTEYE_RPC_COMPARE_IMG:
			push_compare_img_command(sub_arg,(char*)SMARTEYE_RPC_COMPARE_IMG,EJSON_SMARTEYE_RPC_COMPARE_IMG,
						     (char*)SMARTEYE_RPC_COMPARE_IMG_ARGIMG1,(char*)SMARTEYE_RPC_COMPARE_IMG_ARGIMG2,
						     (char*)SMARTEYE_RPC_COMPARE_IMG_ARGDIFF);
			break;
		case EJSON_SMARTEYE_RPC_AUTO_EXPOSURE_GET:
		case EJSON_SMARTEYE_RPC_AUTO_EXPOSURE_SET:
			{
			const char *table[]   = SMARTEYE_RPC_AUTO_EXPOSURE_ARG_TABL;
			CmdlineHelper.push_single_enum_get_set_command( EJSON_SMARTEYE_RPC_AUTO_EXPOSURE_GET,
			EJSON_SMARTEYE_RPC_AUTO_EXPOSURE_SET,SMARTEYE_RPC_AUTO_EXPOSURE_GET,
			SMARTEYE_RPC_AUTO_EXPOSURE_SET,&table[0],SMARTEYE_AUTO_EXPOSURE_UNKNOWN,
			(char*)SMARTEYE_RPC_AUTO_EXPOSURE_ARG,sub_arg);
			}
			break;
		case EJSON_SMARTEYE_RPC_EXPOSURE_GET:
		case EJSON_SMARTEYE_RPC_EXPOSURE_SET:
			CmdlineHelper.push_single_int_get_set_command(EJSON_SMARTEYE_RPC_EXPOSURE_GET,EJSON_SMARTEYE_RPC_EXPOSURE_SET,
					SMARTEYE_RPC_EXPOSURE_GET,SMARTEYE_RPC_EXPOSURE_SET,
					(char*)SMARTEYE_RPC_EXPOSURE_ARG,sub_arg,1);
			break;
		case EJSON_SMARTEYE_RPC_AUTO_FOCUS_GET:
		case EJSON_SMARTEYE_RPC_AUTO_FOCUS_SET:
			{
			const char *table[]   = SMARTEYE_RPC_AUTO_FOCUS_ARG_TABL;
			CmdlineHelper.push_single_enum_get_set_command( EJSON_SMARTEYE_RPC_AUTO_FOCUS_GET,
			EJSON_SMARTEYE_RPC_AUTO_FOCUS_SET,SMARTEYE_RPC_AUTO_FOCUS_GET,
			SMARTEYE_RPC_AUTO_FOCUS_SET,&table[0],SMARTEYE_AUTO_FOCUS_UNKNOWN,
			(char*)SMARTEYE_RPC_AUTO_FOCUS_ARG,sub_arg);
			}
			break;
		case EJSON_SMARTEYE_RPC_FOCUS_GET:
		case EJSON_SMARTEYE_RPC_FOCUS_SET:
			CmdlineHelper.push_single_int_get_set_command(EJSON_SMARTEYE_RPC_FOCUS_GET,EJSON_SMARTEYE_RPC_FOCUS_SET,
					SMARTEYE_RPC_FOCUS_GET,SMARTEYE_RPC_FOCUS_SET,
					(char*)SMARTEYE_RPC_FOCUS_ARG,sub_arg,1);
			break;
		default:
			return 0;
			break;	
	}
	return 0;
}

int SmarteyeCltCmdline::run_my_commands(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	switch(pCmdObj->command)
	{
		case EJSON_SMARTEYE_RPC_SCAN_QRSTRING:
			run_scan_qrcode_command(pCmdObj,pSrvSockConn,pOutMsgList,pWorker);
			break;
		case EJSON_SMARTEYE_RPC_COMPARE_IMG:
			run_compare_img_command(pCmdObj,pSrvSockConn,pOutMsgList,pWorker);
		default:return -1;
			break;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int SmarteyeCltCmdline::run_my_autotest(char* ip,int interval_us,int max_loop,int test_num)
{
	return 0;
}
/* ------------------------------------------------------------------------- */
#include "SrcControlVersion.h"
int SmarteyeCltCmdline::print_my_version()
{
	printf("version - %05d\n",SRC_CONTROL_VERSION);
	return 0;
}
int SmarteyeCltCmdline::get_my_server_port()
{
	return SMARTEYE_JSON_PORT_NUMBER;
}
/* ------------------------------------------------------------------------- */
int SmarteyeCltCmdline::parse_cmdline_arguments(int argc, char **argv)
{
	return CmdlineHelper.parse_cmdline_arguments(argc,argv);
}
/* ------------------------------------------------------------------------- */
int SmarteyeCltCmdline::push_scan_qrcode_command(char* subarg,char* rpc_name,int rpc_index,char* arg_name,char* result_name)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	if(CmdlineHelper.get_next_subargument(&subarg)==0)//user must specify png/jpg file path 
	{
		OBJ_MEM_DELETE(pCmdObj);
		printf("please specify correct filepath on target\n");
		//cout<<"SmarteyeCltCmdline::run_my_commands"<<endl;
		return -1;
	}
	//else 
	//	return -1;//filepath is a must
	strcpy(pCmdObj->get_rpc_name,rpc_name);
	strcpy(pCmdObj->second_arg_param_name,result_name);
	strcpy(pCmdObj->first_arg_param_name,arg_name);
	strcpy(pCmdObj->first_arg_param_value,subarg);
	pCmdObj->command=rpc_index;
	//pCmdObj->action=RPC_SRV_ACT_WRITE;
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_USER_DEFINED;
	//put the request into chain
	if(CmdlineHelper.CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_scan_qrcode_command: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
int SmarteyeCltCmdline::run_scan_qrcode_command(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	ADThreadedSockClient *pOrig = (ADThreadedSockClient*)pWorker;
	{
	pCmdObj->result=pSrvSockConn->get_string_type_with_string_para(pCmdObj->get_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,pCmdObj->second_arg_param_value,pCmdObj->second_arg_param_name);
	pOrig->log_print_message(pSrvSockConn,pCmdObj->get_rpc_name,RPC_SRV_ACT_READ,pCmdObj->result,pOutMsgList,pCmdObj->second_arg_param_value);
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int SmarteyeCltCmdline::push_compare_img_command(char* subarg,char* rpc_name,int rpc_index,char* arg_name,char* arg_name2,char* result_name)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	if(CmdlineHelper.get_next_subargument(&subarg)==0)//user must specify png/jpg file path 
	{
		OBJ_MEM_DELETE(pCmdObj);
		printf("please specify correct filepath of first image on target\n");
		return -1;
	}
	else
		strcpy(pCmdObj->first_arg_param_value,subarg);
	if(CmdlineHelper.get_next_subargument(&subarg)==0)//user must specify png/jpg file path 
	{
		OBJ_MEM_DELETE(pCmdObj);
		printf("please specify correct filepath of second image on target\n");
		return -1;
	}
	else
		strcpy(pCmdObj->second_arg_param_value,subarg);

	strcpy(pCmdObj->get_rpc_name,rpc_name);
	strcpy(pCmdObj->first_arg_param_name,arg_name);
	strcpy(pCmdObj->second_arg_param_name,arg_name2);
	strcpy(pCmdObj->third_arg_param_name,result_name);
	pCmdObj->command=rpc_index;
	//pCmdObj->action=RPC_SRV_ACT_WRITE;
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_USER_DEFINED;
	//put the request into chain
	if(CmdlineHelper.CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_compare_img_command: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
int SmarteyeCltCmdline::run_compare_img_command(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	ADThreadedSockClient *pOrig = (ADThreadedSockClient*)pWorker;
	{
	pCmdObj->result=pSrvSockConn->set_double_string_get_single_string_type(pCmdObj->get_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value,pCmdObj->third_arg_param_name,pCmdObj->third_arg_param_value);
	pOrig->log_print_message(pSrvSockConn,pCmdObj->get_rpc_name,RPC_SRV_ACT_READ,pCmdObj->result,pOutMsgList,pCmdObj->third_arg_param_value);
	}
	return 0;
}
/* ------------------------------------------------------------------------- */

