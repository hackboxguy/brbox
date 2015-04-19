//#include <json.h>
#include <string.h>
#include "ADCmdlineHelper.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;
/*****************************************************************************/
int ADCmdlineHelperProducer::IDGenerator = 0;//generate Unique ID for every consumer object
/*****************************************************************************/
ADCmdlineHelper::ADCmdlineHelper(CMDLINE_HELPER_MODE cmdline_mode)
{
	my_mode=cmdline_mode;
	init_myself();//when user creates my object with specific mode
}
ADCmdlineHelper::ADCmdlineHelper()
{
	my_mode=CMDLINE_HELPER_MODE_CLIENT;
	init_myself();//by default i am in client mode
}
/*****************************************************************************/
ADCmdlineHelper::~ADCmdlineHelper()
{
}
int ADCmdlineHelper::init_myself()
{
	CmdChain.attach_helper(this);
	OptionsChain.attach_helper(this);
	HelpMsgChainID=HelpMsgChain.attach_helper(this);//this ID is needed to delete dynamically created char array
	ipChain.attach_helper(this);
	ClientWorkersList.attach_helper(this);

	complete_subargument = NULL;
	subarg_ptr = NULL;
	help_printed=0;//info for consumer

	//my own default options
	autotest=0;
	interval_us=1000;//1ms resolution
	interval_ms_delay=1;
	max_loop=1;
	test_num=0;
	strcpy(ip,"127.0.0.1");
	port=-1;
	emulation_mode=CMDLINE_OPT_TYPE_NO;
	socket_log=CMDLINE_OPT_TYPE_NO;
	settings[0]='\0';

	//internal default arguments available for all the clients
		strcpy(short_options,"hvpiegtdlnskwrabxzqcof");//--help,--version,--ip,--autotest,--delay,--loopcount,--testnum, --settings
		insert_options_entry((char*)"help"      ,optional_argument,'h',1);

		insert_options_entry((char*)"version"   ,no_argument,'v',1);
		insert_help_entry((char*)"--version                  (print the version number of this tool)");

		insert_options_entry((char*)"port"        ,optional_argument,'p',1);
		insert_help_entry((char*)"--port                     (server's listening tcp port number)");

	if(my_mode==CMDLINE_HELPER_MODE_CLIENT)
	{
		insert_options_entry((char*)"ip"        ,optional_argument,'i',1);
		insert_help_entry((char*)"--ip=ip1,ip2,ip3,ipN       (server ip address: default is localhost, multiple ip's can be specified)");

		insert_options_entry((char*)"autotest"  ,optional_argument,'t',1);
		//insert_help_entry((char*)"--autotest                 (perform autotest of server)"); //for future use

		insert_options_entry((char*)"delay"     ,optional_argument,'d',1);
		insert_help_entry((char*)"--delay                    (delay between loop iteration: default is 1ms)");

		insert_options_entry((char*)"loopcount" ,optional_argument,'l',1);
		insert_help_entry((char*)"--loopcount                (repeatation of a test: default is 1)");

		insert_options_entry((char*)"testnum"   ,optional_argument,'n',1);
		//insert_help_entry((char*)"--testnum                  (test-code: type of test to perform)");

		insert_options_entry((char*)"tasksts" ,optional_argument,'k',1);
		insert_help_entry((char*)"--tasksts=taskID           (read the status of the task which was in progress)");

		insert_options_entry((char*)"shutdown" ,optional_argument,'w',1);
		insert_help_entry((char*)"--shutdown                 (shuts down the service)");

		insert_options_entry((char*)"serversion" ,optional_argument,'r',1);
		insert_help_entry((char*)"--serversion               (read the rpc server version)");

		insert_options_entry((char*)"savedata" ,optional_argument,'a',1);
		insert_help_entry((char*)"--savedata                 (trigger saving of server settings)");

		insert_options_entry((char*)"datasts" ,optional_argument,'b',1);
		insert_help_entry((char*)"--datasts                  (read the status of settings save/load action)");

		insert_options_entry((char*)"rsttasksts" ,optional_argument,'x',1);
		insert_help_entry((char*)"--rsttasksts               (resets task status chain and ID to 0)");

		insert_options_entry((char*)"readysts" ,optional_argument,'z',1);
		insert_help_entry((char*)"--readysts                 (read the ready state of service)");

	}
	else if(my_mode==CMDLINE_HELPER_MODE_SERVER)
	{
		insert_options_entry((char*)"emulation" ,optional_argument,'e',1);
		insert_help_entry((char*)"--emulation=[yes/no]       (disable actual h/w access, default is no)");

		insert_options_entry((char*)"logsocket" ,optional_argument,'g',1);
		insert_help_entry((char*)"--logsocket=[yes/no]       (log json req/resp strings on socket, default is no)");

		insert_options_entry((char*)"settings" ,optional_argument,'s',1);
		insert_help_entry((char*)"--settings=filepath        (settings text filepath to be loaded and stored)");
		
		//deviceInfo related cmdline arguments
		insert_options_entry((char*)"devtype" ,optional_argument,'q',1);
		insert_help_entry((char*)"--devtype=[OVD/KVD/MAGIC/NAUTILUS] (pass device type to service)");
		insert_options_entry((char*)"dvcvariant" ,optional_argument,'c',1);
		insert_help_entry((char*)"--dvcvariant=[XX21/XX15/XX13/XX08] (pass device variant type to service)");
		insert_options_entry((char*)"dvcdetvariant" ,optional_argument,'o',1);
		insert_help_entry((char*)"--dvcdetvariant=[4621/5521] (pass detailed device variant type to service)");
		insert_options_entry((char*)"cpuboardtype" ,optional_argument,'f',1);
		insert_help_entry((char*)"--cpuboardtype=[MERCURY1_0/MERCURY1_2/FREDDY1_0/MERCURY2_0] (pass cpu board type to service)");


	}
	return 0;
}
/*****************************************************************************/
//override virtual functions of ADGeneric Chain
int ADCmdlineHelper::identify_chain_element(void* element,int ident,ADChainProducer* pObj){return -1;}
int ADCmdlineHelper::double_identify_chain_element(void* element,int ident1,int ident2,ADChainProducer* pObj){return -1;}
int ADCmdlineHelper::free_chain_element_data(void* element,ADChainProducer* pObj)
{
	if(pObj->getID() == HelpMsgChainID)
	{
		HelpInfoEntryObj* pEntry;
		pEntry=(HelpInfoEntryObj*)element;
		if(pEntry->help_msg!=NULL)
			MEM_DELETE(pEntry->help_msg);
	}
	return 0;
}
/*****************************************************************************/
int ADCmdlineHelper::is_help_printed()
{
	return help_printed;
}

int ADCmdlineHelper::set_new_subargument(char *arg)
{
        int subarg_num = 0;
        if(complete_subargument != NULL)
        {
                free(complete_subargument);
                complete_subargument = NULL;
                subarg_ptr = NULL;
        }
        if((arg == NULL) || (strlen(arg) <= 0))
        {
                return subarg_num;
        }
        complete_subargument = strdup(arg);
        // count number of subarguments
        subarg_ptr = complete_subargument;
        subarg_num++;
        while(subarg_ptr != NULL)
        {
                subarg_ptr = strchr(subarg_ptr, ',');
                if(subarg_ptr != NULL)
                {
                        subarg_num++;
                        subarg_ptr = subarg_ptr + 1;
                }
	}
        subarg_ptr = complete_subargument;
        return subarg_num;
}
/*****************************************************************************/
int ADCmdlineHelper::get_next_subargument(char **subarg)
{
        char *c;
        static char *str = NULL;
        // cleanup str
        if(str != NULL) {
                free(str);
                str = NULL;
        }
        // check complete_subargument
        if(complete_subargument == NULL) {
                *subarg = NULL;
                return 0;
        }
        if(strlen(complete_subargument) <= 0) {
                *subarg = NULL;
                return 0;
        }
        // check subarg pointer
        if(subarg_ptr == NULL) {
                *subarg = NULL;
                return 0;
        }
        c = strchr(subarg_ptr, ',');
        if(c == NULL) { //no more or last subargument
                str = strdup(subarg_ptr);
                subarg_ptr = NULL;
        } else {
                *c = '\0';
                str = strdup(subarg_ptr);
                subarg_ptr = (c + 1);
        }
        *subarg = str;
        if(subarg_ptr == NULL)
	{
                return 1;       // last subargument
        }
	else
	{
                return 2;       // more subarguments are available
        }
}
/*****************************************************************************/
int ADCmdlineHelper::parse_cmdline_arguments(int argc, char **argv)
{
	//printf("running ADCmdlineHelper::parse_cmdline_arguments\n");
	int arg;//,subarg_num;
	char *subarg;
	if(fill_long_options_table()!=0)
		return -1;
	do
	{
		arg = getopt_long(argc, argv, short_options, long_options_table, 0);
		if( arg == -1 )
		{
			break;
		}
		//subarg_num = 
		set_new_subargument(optarg);
		switch(arg)
		{
			case '?':print_help();return 0;break;//printf("unknown argument found\n");return -1;break;//found something, but invalid/incomplete argument
			case 'h':print_help();return 0;break;
			case 'v':print_subscribers_version();help_printed=1;break;//print_version();return 0;break;
			case 'p':parse_port_number_opt(subarg);
				break;
			case 'i':parse_ip_list_opt(subarg);//push all user supplied ip list to the chain.
				break;
			case 'e':
				//printf("cmdline_helper:emulation mode arg found\n");
				parse_emulation_mode_opt(subarg);
				break;
			case 'g':parse_socket_log_opt(subarg);
				break;
			case 't'://auto-test
				autotest=1;
				break;
			case 'd'://delay interval
				if(get_next_subargument(&subarg)!=0)
					interval_ms_delay=atoi(subarg);
				break;
			case 'l'://loop count
				if(get_next_subargument(&subarg)!=0)
					max_loop=atoi(subarg);
				break;
			case 'n'://test number
				if(get_next_subargument(&subarg)!=0)
					test_num=atoi(subarg);
				break;
			case 's':
				if(get_next_subargument(&subarg)!=0)
					strcpy(settings,subarg);
				break;
			case 'k'://indx:0:read task status
				push_get_task_progress_command((char*)ADLIB_RPC_NAME_GET_TASK_STATUS,(char*)ADLIB_RPC_PARM_TASK_STS_ID,(char*)ADLIB_RPC_PARM_TASK_STS,ADLIB_RPC_INDX_GET_TASK_STATUS,subarg);
				break;
			case 'w'://indx:1:shut-down rpc command
				push_action_type_noarg_command(ADLIB_RPC_INDX_SHUTDOWN_SERVICE,ADLIB_RPC_NAME_SHUTDOWN_SERVICE,(char*)ADLIB_RPC_PARM_TASK_STS_ID);
				break;
			case 'r'://indx:2:read server's version rpc command
				push_single_int_get_set_command(ADLIB_RPC_INDX_GET_SRV_VER,ADLIB_RPC_INDX_GET_SRV_VER,ADLIB_RPC_NAME_GET_SRV_VER,ADLIB_RPC_NAME_GET_SRV_VER,(char*)ADLIB_RPC_PARM_GET_SRV_VER,subarg,1);
				break;
			case 'a'://indx:3:server's data save rpc command
				push_action_type_noarg_command(ADLIB_RPC_INDX_TRIGGER_DATASAVE,(char*)ADLIB_RPC_NAME_TRIGGER_DATASAVE,(char*)ADLIB_RPC_PARM_TASK_STS_ID);
				//push_action_type_noarg_command((int)EJSON_SDSMS_TRIGGER_SENSE6,SDSMS_RPC_NAME_SET_TRIGGER_SENSE6,(char*)SDSMS_RPC_ARG_TASK_STS_ID_PARAM);
				break;
			case 'b'://indx:3:server's data save rpc command
				push_string_get_set_command(ADLIB_RPC_INDX_GET_SETTINGS_STATUS,ADLIB_RPC_INDX_GET_SETTINGS_STATUS,ADLIB_RPC_NAME_GET_SETTINGS_STATUS,ADLIB_RPC_NAME_GET_SETTINGS_STATUS,
					(char*)ADLIB_RPC_PARM_GET_SETTINGS_STATUS,subarg,RPC_SRV_ACT_READONLY);
				break;
			case 'x':
				push_action_type_noarg_command(ADLIB_RPC_INDX_RESET_TASK_STS,(char*)ADLIB_RPC_NAME_RESET_TASK_STS);
				break;
			case 'z':
				push_string_get_set_command(ADLIB_RPC_INDX_GET_READY_STS,ADLIB_RPC_INDX_GET_READY_STS,ADLIB_RPC_NAME_GET_READY_STS,ADLIB_RPC_NAME_GET_READY_STS,
					(char*)ADLIB_RPC_PARM_GET_READY_STS,subarg,RPC_SRV_ACT_READONLY);
				break;
			default:if(parse_subscribers_cmdline((arg-CONSUMERS_OPTIONS_ARG_START_BOUNDARY),subarg)!=0) return -1;//remove the offset which was added by me
				break;
		}
	}while(arg!= -1);

	if(port<=0) //if not provided by user, then ask the consumer-class for default port
		port=get_subscribers_server_port();

	if(my_mode==CMDLINE_HELPER_MODE_CLIENT)
	{
		interval_us*=interval_ms_delay;
		if(autotest==0)//either one of them runs
		{
			run_commands();
		}
		else
			run_subscribers_autotest(ip,interval_us,max_loop,test_num);
	}
	else //do nothing for server mode(just parse and initialize variables).
	{
		;
	}
	return 0;
}
int ADCmdlineHelper::get_port_number()
{
	return	port;
}
int ADCmdlineHelper::get_ip_addr(char* addr)
{
	strcpy(addr,ip);
	return	0;
}
CMDLINE_OPT_TYPE ADCmdlineHelper::get_emulation_mode()
{
	return emulation_mode;
}
int ADCmdlineHelper::get_loop_count()
{
	return	max_loop;
}
int ADCmdlineHelper::get_interval_delay_ms()
{
	return	interval_ms_delay;
}
char* ADCmdlineHelper::get_settings_file()
{
	return	settings;
}
/*****************************************************************************/
int ADCmdlineHelper::fill_long_options_table()
{
	int i=0;	
	OptionsEntryObj* pEntry;
	int total_items=OptionsChain.get_chain_size();
	if(total_items==0)
		return -1;//no options are entered

	if( total_items >= MAX_LONG_OPT_ITEMS )
		return -1;//allow maximum 99 and remaining one entry for null terminated struct table
	
	for(i=0;i<total_items;i++)
	{
		pEntry=(OptionsEntryObj*)OptionsChain.chain_get_by_index(i);
		if(pEntry!=NULL)
		{
			long_options_table[i].name    = pEntry->cmd_option.name;
			long_options_table[i].has_arg = pEntry->cmd_option.has_arg;
			long_options_table[i].flag    = pEntry->cmd_option.flag;
			long_options_table[i].val     = pEntry->cmd_option.val;
		}	
	}

	//last entry for null termination
	long_options_table[i].name    = 0;
	long_options_table[i].has_arg = 0;
	long_options_table[i].flag    = 0;
	long_options_table[i].val     = 0;
	return 0;
}
/*****************************************************************************/
int ADCmdlineHelper::insert_options_entry(/*ADGenericChain *pOptionsChain,*/char* opt_name,int has_arg,int val,int my_own_options)
{
	OptionsEntryObj* pEntry=NULL;
	OBJECT_MEM_NEW(pEntry,OptionsEntryObj);//in anycase, deleting will be done by the ADGenericChain destructor
	if(pEntry==NULL)
		return -1;

	if(my_own_options==0)//consumer's options entry
		val=val+CONSUMERS_OPTIONS_ARG_START_BOUNDARY;//add extra boundary so that user's option doesnt conflict with my ascii value

	strcpy(pEntry->cmd_name,opt_name);
	pEntry->cmd_option.name   = pEntry->cmd_name;
	pEntry->cmd_option.has_arg= has_arg;
	pEntry->cmd_option.flag   = NULL;
	pEntry->cmd_option.val    = val;
	if(OptionsChain.chain_put((void *)pEntry)!=0)
	{
		printf("failed! unable to push options entry to chain!\n");
		OBJ_MEM_DELETE(pEntry);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
int ADCmdlineHelper::insert_help_entry(char* HelpMsg)
{
	HelpInfoEntryObj* pEntry=NULL;
	OBJECT_MEM_NEW(pEntry,HelpInfoEntryObj);
	if(pEntry==NULL)
		return -1;

	pEntry->help_msg=NULL;
	ARRAY_MEM_NEW(pEntry->help_msg,strlen(HelpMsg)+1);//in distructor, ensure that pEntry->help_msg is un-allocated	
	if(pEntry->help_msg==NULL)
	{
		printf("failed! unable to allocate memory for HelpMsg!\n");
		OBJ_MEM_DELETE(pEntry);
		return -1;
	}
	
	strcpy(pEntry->help_msg,HelpMsg);
	if(HelpMsgChain.chain_put((void *)pEntry)!=0)
	{
		printf("failed! unable to push options entry to chain!\n");
		OBJ_MEM_DELETE(pEntry->help_msg);
		OBJ_MEM_DELETE(pEntry);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
int ADCmdlineHelper::print_help()
{
	int i=0;	
	HelpInfoEntryObj* pEntry;
	int total_items=HelpMsgChain.get_chain_size();
	if(total_items==0)
		return -1;//no options are entered
	for(i=0;i<total_items;i++)
	{
		pEntry=(HelpInfoEntryObj*)HelpMsgChain.chain_get_by_index(i);
		if(pEntry!=NULL)
		{
			printf("%s",pEntry->help_msg);printf("\n");
		}	
	}
	help_printed=1;//let user know that help has been printed(server or client may not need to proceess further)
	return 0;
}
/*****************************************************************************/
//note: readonly argument is optional(by default it is disabled)
int ADCmdlineHelper::push_int_type_command(int rpc_cmd, const char* get_rpc_name,const char* set_rpc_name,char* subarg,int readonly)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	pCmdObj->command=rpc_cmd;
	strcpy(pCmdObj->get_rpc_name,get_rpc_name);
	strcpy(pCmdObj->set_rpc_name,set_rpc_name);

	if(readonly)
		pCmdObj->action=RPC_SRV_ACT_READ;
	else if(get_next_subargument(&subarg)==0)//read
	{	
		pCmdObj->action=RPC_SRV_ACT_READ;
	}
	else //write
	{
		pCmdObj->cmd_int_val=atoi(subarg);
		pCmdObj->action=RPC_SRV_ACT_WRITE;
	}
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_INT_GET_SET;

	//put the request into chain
	if(CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	//printf("successfully inserted pCmdObj!!!\n");
	return 0;
}
/*****************************************************************************/
int ADCmdlineHelper::push_action_type_noarg_command(int rpc_cmd,const char* rpc_name,char* ret_string)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	pCmdObj->command=rpc_cmd;
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_ACTION_NO_ARG;
	pCmdObj->action=RPC_SRV_ACT_WRITE;//action-type commands are considered write operations

	if(ret_string!=NULL)
		strcpy(pCmdObj->first_arg_param_name,ret_string);
	else
		pCmdObj->first_arg_param_name[0]='\0';


	strcpy(pCmdObj->set_rpc_name,rpc_name);
	//put the request into chain
	if(CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_action_type_noarg_command: unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
int ADCmdlineHelper::push_single_int_get_set_command(int getcmd,int setcmd,const char* get_rpc_name,const char* set_rpc_name,char* param_name, char* subarg,int readonly)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	strcpy(pCmdObj->get_rpc_name,get_rpc_name);
	strcpy(pCmdObj->set_rpc_name,set_rpc_name);

	if(readonly)//user forced readonly command
	{
		pCmdObj->command=getcmd;
		pCmdObj->action=RPC_SRV_ACT_READ;
		strcpy(pCmdObj->first_arg_param_name,param_name);//"action"
	}
	else if(get_next_subargument(&subarg)==0)//read
	{	
		pCmdObj->command=getcmd;
		pCmdObj->action=RPC_SRV_ACT_READ;
		strcpy(pCmdObj->first_arg_param_name,param_name);//"action"
	}
	else //write
	{
		pCmdObj->command=setcmd;
		pCmdObj->cmd_int_val=atoi(subarg);
		pCmdObj->first_arg_param_int_value=atoi(subarg);
		strcpy(pCmdObj->first_arg_param_name,param_name);//"action"
		pCmdObj->action=RPC_SRV_ACT_WRITE;
	}
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_INT_GET_SET;
	//put the request into chain
	if(CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_int_type_get_set_command: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
//in this case, first param_value is read from the commandline argument.
int ADCmdlineHelper::push_int_get_set_with_dev_addr_arg_command(int getcmd,int setcmd,const char* get_rpc_name,const char* set_rpc_name,char* int_param_name,char* addr_param_name,int addr_param_val,char* subarg,RPC_SRV_ACT forced_act)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	strcpy(pCmdObj->get_rpc_name,get_rpc_name);
	strcpy(pCmdObj->set_rpc_name,set_rpc_name);

	if(addr_param_val>=0)//user doesnt want to parse address via commandline argument
	{
		if(get_next_subargument(&subarg)==0)//read
		{
			if(forced_act==RPC_SRV_ACT_WRITEONLY)
			{
				printf("this is a write only command!!!\n");
				return -1;
			}
			//addr		
			strcpy(pCmdObj->first_arg_param_name,addr_param_name);//"addr"
			pCmdObj->first_arg_param_int_value=addr_param_val;

			strcpy(pCmdObj->second_arg_param_name,int_param_name);//"value"

			pCmdObj->command=getcmd;
			pCmdObj->action=RPC_SRV_ACT_READ;
		}
		else //write
		{
			if(forced_act==RPC_SRV_ACT_READONLY)
			{
				printf("this is a read only command!!!\n");
				return -1;
			}
			//addr		
			strcpy(pCmdObj->first_arg_param_name,addr_param_name);//"addr"
			pCmdObj->first_arg_param_int_value=addr_param_val;

			//value
			strcpy(pCmdObj->second_arg_param_name,int_param_name);//"value"
			pCmdObj->second_arg_param_int_value=atoi(subarg);

			pCmdObj->command=setcmd;
			pCmdObj->action=RPC_SRV_ACT_WRITE;
		}
	}
	else //user wants to parse address and data both from cmdline
	{
		if(get_next_subargument(&subarg)==0)//no address is given
		{
				printf("please specify address!!!\n");
				return -1;
		}
		//addr		
		strcpy(pCmdObj->first_arg_param_name,addr_param_name);
		pCmdObj->first_arg_param_int_value=atoi(subarg);//get the user passed address
		strcpy(pCmdObj->second_arg_param_name,int_param_name);//"data"
		if(get_next_subargument(&subarg)==0)//read command
		{
			pCmdObj->command=getcmd;
			pCmdObj->action=RPC_SRV_ACT_READ;
		}
		else
		{
			//data
			pCmdObj->second_arg_param_int_value=atoi(subarg);//get the user passed data
			pCmdObj->command=setcmd;
			pCmdObj->action=RPC_SRV_ACT_WRITE;
		}
	}


	pCmdObj->cmd_type=CLIENT_CMD_TYPE_INT_GET_SET_WITH_DEV_ADDR;
	//put the request into chain
	if(CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_int_type_get_set_command: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
//in this case, first param_value is read from the commandline argument.
int ADCmdlineHelper::push_enum_get_set_with_dev_addr_arg_command(int getcmd,int setcmd,const char* get_rpc_name,const char* set_rpc_name,const char** enum_string_table,int enum_max_val,char* enum_param_name,char* addr_param_name,int addr_param_val,char* subarg,RPC_SRV_ACT forced_act)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	strcpy(pCmdObj->get_rpc_name,get_rpc_name);
	strcpy(pCmdObj->set_rpc_name,set_rpc_name);

	if(get_next_subargument(&subarg)==0)//read
	{
		if(forced_act==RPC_SRV_ACT_WRITEONLY)
		{
			printf("this is a write only command!!!\n");
			return -1;
		}
		//addr		
		strcpy(pCmdObj->first_arg_param_name,addr_param_name);//"addr"
		pCmdObj->first_arg_param_int_value=addr_param_val;

		strcpy(pCmdObj->second_arg_param_name,enum_param_name);//"value"

		pCmdObj->command=getcmd;
		pCmdObj->action=RPC_SRV_ACT_READ;
	}
	else //write
	{
		if(forced_act==RPC_SRV_ACT_READONLY)
		{
			printf("this is a read only command!!!\n");
			return -1;
		}
		//addr		
		strcpy(pCmdObj->first_arg_param_name,addr_param_name);//"addr"
		pCmdObj->first_arg_param_int_value=addr_param_val;


		pCmdObj->cmd_enum_val=string_to_enum(enum_string_table,subarg,enum_max_val);
		if(pCmdObj->cmd_enum_val>=enum_max_val)
		{
			pCmdObj->command=getcmd;
			pCmdObj->action=RPC_SRV_ACT_READ;
			strcpy(pCmdObj->second_arg_param_name,enum_param_name);//"action"
		}
		else
		{	
			pCmdObj->command=setcmd;
			pCmdObj->action=RPC_SRV_ACT_WRITE;
			strcpy(pCmdObj->second_arg_param_name,enum_param_name);//"action"
			strcpy(pCmdObj->second_arg_param_value,enum_string_table[pCmdObj->cmd_enum_val]);//"config"		
		}

		//value
		//strcpy(pCmdObj->second_arg_param_name,int_param_name);//"value"
		//pCmdObj->second_arg_param_int_value=atoi(subarg);
		//pCmdObj->command=setcmd;
		//pCmdObj->action=RPC_SRV_ACT_WRITE;
	}
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_ENUM_GET_SET_WITH_DEV_ADDR;
	//put the request into chain
	if(CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_enum_type_get_set_command: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
//in this case, first param_value is read from the commandline argument.
//int ADCmdlineHelper::push_string_get_set_with_dev_addr_arg_command(int getcmd,int setcmd,const char* get_rpc_name,const char* set_rpc_name,char* int_param_name,char* addr_param_name,int addr_param_val,char* subarg,RPC_SRV_ACT forced_act)
int ADCmdlineHelper::push_string_get_set_with_dev_addr_arg_command(int getcmd,int setcmd,const char* get_rpc_name,const char* set_rpc_name,char* string_param_name,char* addr_param_name,int addr_param_val,char* subarg,RPC_SRV_ACT forced_act)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	strcpy(pCmdObj->get_rpc_name,get_rpc_name);
	strcpy(pCmdObj->set_rpc_name,set_rpc_name);

	if(get_next_subargument(&subarg)==0)//read
	{
		if(forced_act==RPC_SRV_ACT_WRITEONLY)
		{
			printf("this is a write only command!!!\n");
			return -1;
		}
		//addr		
		strcpy(pCmdObj->first_arg_param_name,addr_param_name);//"addr"
		pCmdObj->first_arg_param_int_value=addr_param_val;//0

		strcpy(pCmdObj->second_arg_param_name,string_param_name);//"string"

		pCmdObj->command=getcmd;
		pCmdObj->action=RPC_SRV_ACT_READ;
	}
	else //write
	{
		if(forced_act==RPC_SRV_ACT_READONLY)
		{
			printf("this is a read only command!!!\n");
			return -1;
		}
		//addr		
		strcpy(pCmdObj->first_arg_param_name,addr_param_name);//"addr"
		pCmdObj->first_arg_param_int_value=addr_param_val;

		//value(string type)
		strcpy(pCmdObj->second_arg_param_name,string_param_name);//"value"
		strcpy(pCmdObj->second_arg_param_value,subarg);//string type

		pCmdObj->command=setcmd;
		pCmdObj->action=RPC_SRV_ACT_WRITE;
	}
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_STRING_GET_SET_WITH_DEV_ADDR;
	//put the request into chain
	if(CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_string_type_get_set_command: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
int ADCmdlineHelper::push_single_enum_get_set_command(int getcmd,int setcmd,const char* get_rpc_name,const char* set_rpc_name,const char** enum_string_table,int enum_max_val,char* param_name, char* subarg, char* ret_string)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	strcpy(pCmdObj->get_rpc_name,get_rpc_name);
	strcpy(pCmdObj->set_rpc_name,set_rpc_name);
	if(get_next_subargument(&subarg)==0)//get enum command
	{	
		pCmdObj->command=getcmd;
		pCmdObj->action=RPC_SRV_ACT_READ;
		strcpy(pCmdObj->first_arg_param_name,param_name);//"action"
		//printf("push_single_enum_get_set_command: read\n");
	}
	else //set enum
	{
		pCmdObj->cmd_enum_val=string_to_enum(enum_string_table,subarg,enum_max_val);
		if(pCmdObj->cmd_enum_val>=enum_max_val)
		{
			pCmdObj->command=getcmd;
			pCmdObj->action=RPC_SRV_ACT_READ;
			strcpy(pCmdObj->first_arg_param_name,param_name);//"action"
			//pCmdObj->first_arg_param_value// for get action, server will fill this value
		}
		else
		{	
			pCmdObj->command=setcmd;
			pCmdObj->action=RPC_SRV_ACT_WRITE;
			strcpy(pCmdObj->first_arg_param_name,param_name);//"action"
			strcpy(pCmdObj->first_arg_param_value,enum_string_table[pCmdObj->cmd_enum_val]);//"config"		
			//printf("push_single_enum_get_set_command: write\n");

			if(ret_string!=NULL)
				strcpy(pCmdObj->second_arg_param_name,ret_string);
			else
				pCmdObj->second_arg_param_name[0]='\0';
		}
	}
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_ENUM_GET_SET;

	//put the request into chain
	if(CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_single_enum_get_set_command: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}

/*****************************************************************************/
#include <sys/stat.h>
int ADCmdlineHelper::Is_it_valid_file(char* filepath)
{
	struct stat buffer;
	if(stat(filepath,&buffer)!=0)
		return -1;
	if(buffer.st_mode & S_IFREG)
		return 0;
	return -1;//it could be a directory
}
int ADCmdlineHelper::parse_ip_list_opt(char* subarg)
{
	bool FileChecked=false;
	while(get_next_subargument(&subarg)!=0)
	{
		if(FileChecked==false)//do this just in the beginning of the loop
		{
			if(Is_it_valid_file(subarg)!=0)
				FileChecked=true;//this is not a ip-list-file, its quamma separated iplist
			else
			{
				push_ip_to_list_from_file(subarg);
				break;
			}
		}
		strcpy(ip,subarg);//keep last ip in my cache(needed for jtclt client)
		push_ip_to_list(subarg);
	}
	return 0;
}
int ADCmdlineHelper::parse_port_number_opt(char* subarg)
{
	if(get_next_subargument(&subarg)!=0)
		port=atoi(subarg);
	return 0;
}
int ADCmdlineHelper::parse_emulation_mode_opt(char* subarg)
{
	if(get_next_subargument(&subarg)!=0)
	{
		if(strcmp(subarg,"yes")==0)
			emulation_mode=CMDLINE_OPT_TYPE_YES;
	}
	return 0;
}
int ADCmdlineHelper::parse_socket_log_opt(char* subarg)
{
	if(get_next_subargument(&subarg)!=0)
	{
		if(strcmp(subarg,"yes")==0)
			socket_log=CMDLINE_OPT_TYPE_YES;
	}
	return 0;
}
int ADCmdlineHelper::get_socket_log_opt()
{
	if(socket_log==CMDLINE_OPT_TYPE_YES)
		return 1;
	else
		return 0;
}
/*****************************************************************************/
//note: caller must allocate atleast 256 bytes for "ip" pointer before calling this function
int ADCmdlineHelper::grep_ip_line(char* ip_filepath,int line,char* ip)
{
	char command[1024];
	char temp_str[256];
	sprintf(command,"sed '%d,%d!d' %s",line,line,ip_filepath);//grep the ip-line
	FILE *shell;
	shell= popen(command,"r");
	if(shell == NULL)
		return -1;
	if(fgets(temp_str,254,shell)==NULL)
	{
		pclose(shell);
		return -1;
	}
	if(temp_str[strlen(temp_str)-1]=='\n')
		temp_str[strlen(temp_str)-1]='\0';
	temp_str[255]='\0';
	strcpy(ip,temp_str);
	pclose(shell);
	return 0;
}
int ADCmdlineHelper::count_total_ip_from_file(char* ip_filepath)
{
	int count=0;
	char command[1024];
	char temp_str[256];
	sprintf(command,"sed -n '$=' %s",ip_filepath);//count number of lines
	FILE *shell;
	shell= popen(command,"r");
	if(shell == NULL)
		return -1;//RPC_SRV_RESULT_FILE_OPEN_ERR;
	if(fgets(temp_str,250,shell)==NULL)
	{
		pclose(shell);
		return -1;
	}
	temp_str[255]='\0';
	count=atoi(temp_str);
	pclose(shell);
	return count;
}
int ADCmdlineHelper::push_ip_to_list_from_file(char* file)
{
	char ip[256];
	int total_ip=count_total_ip_from_file(file);
	for(int i=0;i<total_ip;i++)
	{
		if(grep_ip_line(file,i+1,ip)==0)
			push_ip_to_list(ip);
	}
	return 0;
}
/*****************************************************************************/
int ADCmdlineHelper::push_ip_to_list(char* ip)
{
	IpAddrEntryObj* pIPEntry=NULL;
	OBJECT_MEM_NEW(pIPEntry,IpAddrEntryObj);
	if(pIPEntry==NULL)
	{
		printf("failed! unable to allocate memory for ipEntry!\n");
		return -1;
	}
	strcpy(pIPEntry->ip_addr,ip);
	if(ipChain.chain_put((void *)pIPEntry)!=0)
	{
		printf("failed! unable to push ip entry to chain!\n");
		OBJ_MEM_DELETE(pIPEntry);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
#define MAX_SRV_RESPONSE_TIMEOUT 120000 //60sec
int ADCmdlineHelper::wait_for_client_workers_to_finish()
{
	ADThreadedSockClient* pWorker=NULL;
	int commands=ClientWorkersList.get_chain_size();
	for(int i=0;i<commands;i++)
	{
		pWorker=NULL;
		pWorker=(ADThreadedSockClient*)ClientWorkersList.chain_get_by_index(i);
		if(pWorker!=NULL)
		{
			int SrvRespTimeout=0;
			while( pWorker->is_running() && (SrvRespTimeout++ < MAX_SRV_RESPONSE_TIMEOUT) )
			{
                //printf("SrvRespTimeout=%d\n",SrvRespTimeout);
                usleep(5000);//5ms
			}
		}
		pWorker->print_command_result();
	}
	return 0;
}
int ADCmdlineHelper::delete_client_workers()
{
	ADThreadedSockClient* pWorker=NULL;
	while((pWorker=(ADThreadedSockClient*)ClientWorkersList.chain_get()) != NULL)
	{
		OBJ_MEM_DELETE(pWorker);
	}
	return 0;
}
/*****************************************************************************/
//override function callback from ADThreadedSockClient
int ADCmdlineHelper::run_user_command(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList,ADThreadedSockClientProducer *pWorker)
{
	//ADThreadedSockClient doesnt know how to process this command, hence just call user's command processor(SdfcCmdline).
	return run_subscribers_commands(pCmdObj,pSrvSockConn,pOutMsgList,pWorker);
}
/*****************************************************************************/
int ADCmdlineHelper::start_new_client_worker(char* ip_addr,int port)
{
	ADThreadedSockClient* pWorker=NULL;
	OBJECT_MEM_NEW(pWorker,ADThreadedSockClient);
	if(pWorker==NULL)
	{
		printf("failed! unable to allocate memory for pWorker!\n");
		return -1;
	}
	pWorker->attach_helper(this);//attach for callback
	pWorker->run_commands(ip_addr,port,&CmdChain);//this starts the thread
	if(ClientWorkersList.chain_put((void *)pWorker)!=0)
	{
		printf("failed! unable to push pWorker entry to chain!\n");
		OBJ_MEM_DELETE(pWorker);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
int ADCmdlineHelper::run_commands() //runs all the commands on list of ip's provided by user
{
	IpAddrEntryObj  *pIpObj=NULL;
	int commands=CmdChain.get_chain_size();
	if(commands==0)
	{
		printf("no command execution is requested!!! try with --help\n");
		return -1;//no commands are requested
	}
	if(ipChain.get_chain_size()==0)
		push_ip_to_list(ip);//default local host ip

	while((pIpObj=(IpAddrEntryObj*)ipChain.chain_get()) != NULL)
	{
		start_new_client_worker(pIpObj->ip_addr,port/*get_subscribers_server_port()*/);//ADThreadedSockClient
		OBJ_MEM_DELETE(pIpObj);//delete IpEntry
	}
	wait_for_client_workers_to_finish();
	delete_client_workers();//after finishing the task, delete all workers
	return 0;
}
/*****************************************************************************/
int ADCmdlineHelper::push_string_get_set_with_string_arg_command(int getcmd,int setcmd,const char* get_rpc_name,const char* set_rpc_name,char* first_par_name,char* first_par_val,char* sec_para_name,char* sec_para_val,char* subarg,RPC_SRV_ACT forced_act)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	strcpy(pCmdObj->get_rpc_name,get_rpc_name);
	strcpy(pCmdObj->set_rpc_name,set_rpc_name);

	strcpy(pCmdObj->first_arg_param_name ,first_par_name);
	strcpy(pCmdObj->first_arg_param_value,first_par_val);
	strcpy(pCmdObj->second_arg_param_name,sec_para_name);
	strcpy(pCmdObj->second_arg_param_value,sec_para_val);//string type

	if(get_next_subargument(&subarg)==0)//read
	{
		if(forced_act==RPC_SRV_ACT_WRITEONLY)
		{
			printf("this is a write only command!!!\n");
			return -1;
		}
		pCmdObj->command=getcmd;
		pCmdObj->action=RPC_SRV_ACT_READ;
	}
	else //write
	{
		if(forced_act==RPC_SRV_ACT_READONLY)
		{
			printf("this is a read only command!!!\n");
			return -1;
		}
		pCmdObj->command=setcmd;
		pCmdObj->action=RPC_SRV_ACT_WRITE;
	}
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_STRING_GET_SET_WITH_STRING_PARA;
	//put the request into chain
	if(CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_string_type_get_set_command: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
int ADCmdlineHelper::push_get_task_progress_command_with_dev_addr(char* get_rpc_name,char *addr, int addr_val,char* taskID_name,char*taskSts_name,int command,char* subarg)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}

	strcpy(pCmdObj->get_rpc_name,get_rpc_name);//SDSMS_RPC_NAME_GET_TASK_STATUS);
	strcpy(pCmdObj->set_rpc_name,get_rpc_name);//SDSMS_RPC_NAME_GET_TASK_STATUS);

	if(get_next_subargument(&subarg)==0)
	{
		OBJ_MEM_DELETE(pCmdObj);
		printf("please specify correct task ID!!!\n");
		return -1;
	}
	else 
	{
			strcpy(pCmdObj->first_arg_param_name,addr);//SDSMS_RPC_ARG_TASK_STS_ID_PARAM);//"taskID"
			pCmdObj->first_arg_param_int_value=addr_val;

			strcpy(pCmdObj->second_arg_param_name,taskID_name);//SDSMS_RPC_ARG_TASK_STS_ID_PARAM);//"taskID"
			strcpy(pCmdObj->second_arg_param_value,subarg);//taskID
			pCmdObj->command=command;//EJSON_SDSMS_GET_TASK_STS;
			pCmdObj->action=RPC_SRV_ACT_READ;
			strcpy(pCmdObj->third_arg_param_name,taskSts_name);//SDSMS_RPC_ARG_TASK_STS_PARAM);//"taskSts"
			//strcpy(pCmdObj->second_arg_param_value,subarg);//00:01:02:03:04:05
	}
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_GET_TASK_STATUS_WITH_DEV_ADDR;//CLIENT_CMD_TYPE_USER_DEFINED;
	//put the request into chain
	if(CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_get_task_progress_command: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
int ADCmdlineHelper::push_get_task_progress_command(char* get_rpc_name,char* taskID_name,char*taskSts_name,int command,char* subarg)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}

	strcpy(pCmdObj->get_rpc_name,get_rpc_name);//SDSMS_RPC_NAME_GET_TASK_STATUS);
	strcpy(pCmdObj->set_rpc_name,get_rpc_name);//SDSMS_RPC_NAME_GET_TASK_STATUS);

	if(get_next_subargument(&subarg)==0)//user must specify either eth0 or eth1
	{
		OBJ_MEM_DELETE(pCmdObj);
		printf("please specify correct task ID!!!\n");
		return -1;
	}
	else 
	{
			strcpy(pCmdObj->first_arg_param_name,taskID_name);//SDSMS_RPC_ARG_TASK_STS_ID_PARAM);//"taskID"
			strcpy(pCmdObj->first_arg_param_value,subarg);//taskID
			pCmdObj->command=command;//EJSON_SDSMS_GET_TASK_STS;
			pCmdObj->action=RPC_SRV_ACT_READ;
			strcpy(pCmdObj->second_arg_param_name,taskSts_name);//SDSMS_RPC_ARG_TASK_STS_PARAM);//"taskSts"
			//strcpy(pCmdObj->second_arg_param_value,subarg);//00:01:02:03:04:05
	}
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_GET_TASK_STATUS;//CLIENT_CMD_TYPE_USER_DEFINED;
	//put the request into chain
	if(CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_get_task_progress_command: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
int ADCmdlineHelper::push_string_get_set_with_enum_arg(int getcmd,int setcmd,const char* get_rpc,const char* set_rpc,
			const char* enum_name,int enum_max,const char** enum_table, const char* string_name,char* subarg,RPC_SRV_ACT forced_act)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	strcpy(pCmdObj->get_rpc_name,get_rpc);
	strcpy(pCmdObj->set_rpc_name,set_rpc);

	if(get_next_subargument(&subarg)==0)//user must specify either eth0 or eth1
	{
		OBJ_MEM_DELETE(pCmdObj);
		printf("please specify correct enum value!!!\n");
		return -1;
	}
	else 
	{
		int temp_val=string_to_enum(enum_table,subarg,enum_max);
		if(temp_val>=enum_max)
		{
			OBJ_MEM_DELETE(pCmdObj);
			printf("please specify correct first enum type!!\n");
			return -1;
		}
		else
		{	
			strcpy(pCmdObj->first_arg_param_name,enum_name);
			strcpy(pCmdObj->first_arg_param_value,enum_table[temp_val]);
			strcpy(pCmdObj->second_arg_param_name,string_name);//"macaddr"
			//strcpy(pCmdObj->third_arg_param_name,string_name);//for read, grep return value in third string
			if(forced_act==RPC_SRV_ACT_READ_AND_WRITE)
			{
				if(get_next_subargument(&subarg)==0)//read
				{	
					pCmdObj->command=getcmd;
					pCmdObj->action=RPC_SRV_ACT_READ;
				}
				else
				{
					pCmdObj->command=setcmd;
					pCmdObj->action=RPC_SRV_ACT_WRITE;
					strcpy(pCmdObj->second_arg_param_name,string_name);//"macaddr"
					strcpy(pCmdObj->second_arg_param_value,subarg);//00:01:02:03:04:05
				}
			}
		}
	}
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_STRING_GET_SET_WITH_ENUM_PARA;
	//put the request into chain
	if(CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_string_get_set_with_enum_arg: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
int ADCmdlineHelper::push_string_get_set_command(int getcmd,int setcmd,const char* get_rpc_name,const char* set_rpc_name,char* string_param_name,char* subarg,RPC_SRV_ACT forced_act)
{
	CmdExecutionObj *pCmdObj=NULL;
	OBJECT_MEM_NEW(pCmdObj,CmdExecutionObj);
	if(pCmdObj==NULL)
	{
		printf("failed create pCmdObj!!!\n");
		return -1;
	}
	strcpy(pCmdObj->get_rpc_name,get_rpc_name);
	strcpy(pCmdObj->set_rpc_name,set_rpc_name);

	if(get_next_subargument(&subarg)==0)//read
	{
		if(forced_act==RPC_SRV_ACT_WRITEONLY)
		{
			printf("this is a write only command!!!\n");
			return -1;
		}
		strcpy(pCmdObj->first_arg_param_name,string_param_name);
		pCmdObj->command=getcmd;
		pCmdObj->action=RPC_SRV_ACT_READ;
	}
	else //write
	{
		if(forced_act==RPC_SRV_ACT_READONLY)
		{
			printf("this is a read only command!!!\n");
			return -1;
		}
		strcpy(pCmdObj->first_arg_param_name,string_param_name);
		strcpy(pCmdObj->first_arg_param_value,subarg);
		pCmdObj->command=setcmd;
		pCmdObj->action=RPC_SRV_ACT_WRITE;
	}
	pCmdObj->cmd_type=CLIENT_CMD_TYPE_STRING_GET_SET;
	//put the request into chain
	if(CmdChain.chain_put((void *)pCmdObj)!=0)
	{
		printf("push_string_type_get_set_command: failed! unable to push json-req-task-obj to chain!\n");
		OBJ_MEM_DELETE(pCmdObj);
		return -1;
	}
	return 0;
}
/*****************************************************************************/

