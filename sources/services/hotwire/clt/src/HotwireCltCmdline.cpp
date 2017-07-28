#include "HotwireCltCmdline.h"

using namespace std;
/*****************************************************************************/
GpioCltCmdline::GpioCltCmdline()
{
	CmdlineHelper.attach_helper(this);
	//CmdlineHelper.insert_options_entry((char*)"gpio" ,optional_argument,EJSON_GPIOCTL_RPC_IO_GET);
	//CmdlineHelper.insert_help_entry((char*)"--gpio=addr,data             (read/write gpio bit value)");

	//CmdlineHelper.insert_options_entry((char*)"imgcheck" ,optional_argument,EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN);
	//CmdlineHelper.insert_help_entry((char*)"--imgcheck=imgtype         [checks captured image against<ident/red/green/blue/white>]");
	//CmdlineHelper.insert_options_entry((char*)"debugimgfile" ,optional_argument,EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET);
	//CmdlineHelper.insert_help_entry((char*)"--debugimgfile=filepath    [read/write debug image file path]");

	CmdlineHelper.insert_options_entry((char*)"omxact" ,optional_argument,EJSON_GPIOCTL_RPC_OMXACT_GET);
	CmdlineHelper.insert_help_entry((char*)"--omxact=type              [get/set omx player action, type=<start/intr/stop/warn/idle>]");
	CmdlineHelper.insert_options_entry((char*)"showimg" ,optional_argument,EJSON_MPLAYSRV_RPC_SHOWFBIMG_GET);
	CmdlineHelper.insert_help_entry((char*)"--showimg=imagefilepath    [get/set image showing on the screen]");

	CmdlineHelper.insert_help_entry((char*)"--qrcode=imgfile,qr-string [converts given qr-string to qrcode-png-image file]");
	CmdlineHelper.insert_options_entry((char*)"qrcode" ,optional_argument,EJSON_MPLAYSRV_RPC_QRCODEIMG_SET);

}
/*****************************************************************************/
GpioCltCmdline::~GpioCltCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int GpioCltCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	EJSON_GPIOCTL_RPC_TYPES command =(EJSON_GPIOCTL_RPC_TYPES)arg;
	switch(command)
	{
		//case EJSON_GPIOCTL_RPC_IO_GET:
		//	CmdlineHelper.push_int_get_set_with_dev_addr_arg_command(EJSON_GPIOCTL_RPC_IO_GET,EJSON_GPIOCTL_RPC_IO_SET,
		//	GPIOCTL_RPC_IO_GET,GPIOCTL_RPC_IO_SET,(char*)GPIOCTL_RPC_IO_DATA_ARG,(char*)GPIOCTL_RPC_IO_ADDR_ARG,-1,sub_arg);
		//	break;
		case EJSON_GPIOCTL_RPC_OMXACT_GET:
		case EJSON_GPIOCTL_RPC_OMXACT_SET:
			{
			const char *table[]   = GPIOCTL_RPC_OMXACT_ARG_TABL;
			CmdlineHelper.push_single_enum_get_set_command( EJSON_GPIOCTL_RPC_OMXACT_GET,
			EJSON_GPIOCTL_RPC_OMXACT_SET,GPIOCTL_RPC_OMXACT_GET,
			GPIOCTL_RPC_OMXACT_SET,&table[0],GPIOCTL_OMXACT_UNKNOWN,
			(char*)GPIOCTL_RPC_OMXACT_ARG,sub_arg);
			}
			break;
		case EJSON_MPLAYSRV_RPC_SHOWFBIMG_GET:
		case EJSON_MPLAYSRV_RPC_SHOWFBIMG_SET:
			CmdlineHelper.push_string_get_set_command(EJSON_MPLAYSRV_RPC_SHOWFBIMG_GET,EJSON_MPLAYSRV_RPC_SHOWFBIMG_SET,
			MPLAYSRV_RPC_SHOWFBIMG_GET,MPLAYSRV_RPC_SHOWFBIMG_SET,(char*)MPLAYSRV_RPC_SHOWFBIMG_ARG,sub_arg);
			break;
		case EJSON_MPLAYSRV_RPC_QRCODEIMG_SET:
			push_qrcode_image_cmd(sub_arg);//,command);
			break;
		default:
			return 0;
			break;	
	}
	return 0;
}

int GpioCltCmdline::run_my_commands(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	switch(pCmdObj->command)
	{
		case EJSON_MPLAYSRV_RPC_QRCODEIMG_SET:
			run_qrcode_image_cmd(pCmdObj,pSrvSockConn,pOutMsgList,pWorker);
			break;
		default:return -1;
			break;
	}
	return 0;
}
/*****************************************************************************/
int GpioCltCmdline::run_my_autotest(char* ip,int interval_us,int max_loop,int test_num)
{
	return 0;
}
/*****************************************************************************/
#include "SrcControlVersion.h"
int GpioCltCmdline::print_my_version()
{
	printf("version - %05d\n",SRC_CONTROL_VERSION);
	return 0;
}
/* ------------------------------------------------------------------------- */
int GpioCltCmdline::get_my_server_port()
{
	return GPIOCTL_JSON_PORT_NUMBER;
}
/* ------------------------------------------------------------------------- */
int GpioCltCmdline::parse_cmdline_arguments(int argc, char **argv)
{
	return CmdlineHelper.parse_cmdline_arguments(argc,argv);
}
/* ------------------------------------------------------------------------- */
int GpioCltCmdline::push_qrcode_image_cmd(char* subarg)//,EJSON_SYSMGR_RPC_TYPES rpc_cmd)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	strcpy(pCmdObj->get_rpc_name,MPLAYSRV_RPC_QRCODEIMG_SET);
	strcpy(pCmdObj->set_rpc_name,MPLAYSRV_RPC_QRCODEIMG_SET);
	pCmdObj->command=EJSON_MPLAYSRV_RPC_QRCODEIMG_SET;
	pCmdObj->action=RPC_SRV_ACT_WRITE;

	if(CmdlineHelper.get_next_subargument(&subarg)==0)
	{	
		printf("for qrcode generation,  image file path must be specified\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	else 
	{
		strcpy(pCmdObj->first_arg_param_name,MPLAYSRV_RPC_QRCODEIMG_ARG_FILEPATH);
		strcpy(pCmdObj->first_arg_param_value,subarg);

		if(CmdlineHelper.get_next_subargument(&subarg)==0)
		{	
			printf("for qrcode generation,  qr-string must be specified\n");
			OBJ_MEM_DELETE(pCmdObj);
			return -1;
		}
		strcpy(pCmdObj->second_arg_param_name,MPLAYSRV_RPC_QRCODEIMG_ARG_QRSTRING);
		strcpy(pCmdObj->second_arg_param_value,subarg);
		//strcpy(pCmdObj->third_arg_param_name,RPCMGR_RPC_TASK_STS_ARGID);//taskID

		//if(rpc_cmd==EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP)
		//{
		//	if(CmdlineHelper.get_next_subargument(&subarg)==0)
		//	{	
		//		printf("for file download,  tftp server's filename must be specified\n");
		//		OBJ_MEM_DELETE(pCmdObj);
		//		return -1;
		//	}
		//	else 
		//	{
		//		strcpy(pCmdObj->third_arg_param_name,SYSMGR_RPC_DOWNLOADFILE_ARG_SRCFILE);
		//		strcpy(pCmdObj->third_arg_param_value,subarg);
				//shift taskID string to last as fourth
		//		strcpy(pCmdObj->fourth_arg_param_name,RPCMGR_RPC_TASK_STS_ARGID);//taskID
		//	}

		//}
	}
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_USER_DEFINED;
	//put the request into chain
	if(CmdlineHelper.CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int GpioCltCmdline::run_qrcode_image_cmd(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	ADThreadedSockClient *pOrig = (ADThreadedSockClient*)pWorker;
	if(pCmdObj->action == RPC_SRV_ACT_WRITE)
	{
		pCmdObj->result=pSrvSockConn->set_double_string_type(pCmdObj->set_rpc_name,
				pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,
				pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value);//,
				//pCmdObj->third_arg_param_name,pCmdObj->third_arg_param_value);
		pOrig->log_print_message(pSrvSockConn,pCmdObj->set_rpc_name,RPC_SRV_ACT_READ,pCmdObj->result,
				pOutMsgList,pCmdObj->third_arg_param_value);
	}
	else
	{
		pOrig->my_log_print_message(pSrvSockConn,(char*)"run_qrcode_image_cmd",RPC_SRV_ACT_UNKNOWN,
					(char*)CLIENT_CMD_RESULT_INVALID_ACT,pOutMsgList);
		return -1;
	}
	return 0;
}

