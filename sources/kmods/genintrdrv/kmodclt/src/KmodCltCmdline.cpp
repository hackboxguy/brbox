#include "KmodCltCmdline.h"
#include <linux/genintrdrv.h>
using namespace std;
typedef enum EKMODCLT_CMD_TYPE_T
{
	EKMODCLT_CMD_TYPE_INCR=0,
	EKMODCLT_CMD_TYPE_DECR=0,
	EKMODCLT_CMD_TYPE_END,
	EKMODCLT_CMD_TYPE_NONE
}EKMODCLT_CMD_TYPE;
/*****************************************************************************/
KmodCltCmdline::KmodCltCmdline()
{
	CmdlineHelper.attach_helper(this);
	CmdlineHelper.insert_options_entry((char*)"incr" ,optional_argument,EKMODCLT_CMD_TYPE_INCR);
	CmdlineHelper.insert_help_entry((char*)"--incr=count               (increase kmod count via ioctl)");

	//CmdlineHelper.insert_options_entry((char*)"imgcheck" ,optional_argument,EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN);
	//CmdlineHelper.insert_help_entry((char*)"--imgcheck=imgtype         [checks captured image against<ident/red/green/blue/white>]");
	//CmdlineHelper.insert_options_entry((char*)"debugimgfile" ,optional_argument,EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET);
	//CmdlineHelper.insert_help_entry((char*)"--debugimgfile=filepath    [read/write debug image file path]");
}
/*****************************************************************************/
KmodCltCmdline::~KmodCltCmdline()
{
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int KmodCltCmdline::parse_my_cmdline_options(int arg, char* sub_arg)
{
	EKMODCLT_CMD_TYPE command =(EKMODCLT_CMD_TYPE)arg;
	switch(command)
	{
		case EKMODCLT_CMD_TYPE_INCR:
			cout<<"increase count called"<<endl;
			kmod_increase_count();
			//CmdlineHelper.push_int_get_set_with_dev_addr_arg_command(EJSON_GPIOCTL_RPC_IO_GET,EJSON_GPIOCTL_RPC_IO_SET,
			//GPIOCTL_RPC_IO_GET,GPIOCTL_RPC_IO_SET,(char*)GPIOCTL_RPC_IO_DATA_ARG,(char*)GPIOCTL_RPC_IO_ADDR_ARG,-1,sub_arg);
			break;
		default:
			return 0;
			break;	
	}
	return 0;
}

int KmodCltCmdline::run_my_commands(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	EKMODCLT_CMD_TYPE cmd =(EKMODCLT_CMD_TYPE)pCmdObj->command;
	switch(cmd)//pCmdObj->command)
	{
		case EKMODCLT_CMD_TYPE_INCR:
			//kmod_increase_count();
			return 0;
		default:return -1;
			break;
	}
	return 0;
}
/*****************************************************************************/
int KmodCltCmdline::run_my_autotest(char* ip,int interval_us,int max_loop,int test_num)
{
	return 0;
}
/*****************************************************************************/
#include "SrcControlVersion.h"
int KmodCltCmdline::print_my_version()
{
	printf("version - %05d\n",SRC_CONTROL_VERSION);
	return 0;
}
int KmodCltCmdline::get_my_server_port()
{
	return -1;//GPIOCTL_JSON_PORT_NUMBER;
}
/*****************************************************************************/
int KmodCltCmdline::parse_cmdline_arguments(int argc, char **argv)
{
	return CmdlineHelper.parse_cmdline_arguments(argc,argv);
}
/*****************************************************************************/
int KmodCltCmdline::kmod_open(char* device)
{
	int fd = open(device,'r');//"/dev/helloworld", 'w');
	if (fd < 0)
		return -fd;
	return fd;
}
int KmodCltCmdline::kmod_close(int fd)
{
	int ret = 0;
	ret = close(fd);
	return ret;
}
int KmodCltCmdline::kmod_increase_count(void)
{
	int fd = kmod_open((char*)"/dev/genintrdrv");
	if (fd < 0)
	{
		printf("There was an error opening genintrdrv.\n");
		return -1;
	}
	int ret = 0;	
	genintrdrv_ioctl_inc_t ioctl_args;
	memset(&ioctl_args, 0, sizeof(ioctl_args));
	ioctl_args.placeholder = GENINTRDRV_MAGIC;
	//ret = ioctl(fd, GENINTRDRV_IOCTL_INCREMENT, &ioctl_args);

	/*printf("INCREMENT=%d\n", Params.increment);
	for (i = 0; i < Params.increment; i++)
	{

		if (helloworld_inc(fd))
		{
			printf("There was an error calling helloworld_inc().\n");
			exit(EXIT_FAILURE);
		}
	}*/
	if (kmod_close(fd))
	{
		printf("There was an error closing genintrdrv.\n");
		return -1;
	}
	return ret;
}
/*****************************************************************************/


