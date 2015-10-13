#ifndef __AD_CMN_CLT_CMD_PROCESSOR_H_
#define __AD_CMN_CLT_CMD_PROCESSOR_H_
#include "ADCommon.hpp"
#include "JsonCmnDef.h"
#include "ADJsonRpcClient.hpp"
#include "ADGenericChain.hpp"
#include "ADCmnStringProcessor.hpp"

//#define JSON_RPC_METHOD_NAME_MAX_LENGTH 256
//#define JSON_RPC_METHOD_REQ_MAX_LENGTH  1100 //considering 1400 MTU limit
//#define JSON_RPC_METHOD_RESP_MAX_LENGTH 1100 //considering 1400 MTU limit


#define CLIENT_CMD_RESULT_FAIL        "failed!!!"
#define CLIENT_CMD_RESULT_SUCCESS     "success"
#define CLIENT_CMD_RESULT_INVALID_ACT "invalid action!!!"

typedef enum CLIENT_CMD_TYPE_T
{
	CLIENT_CMD_TYPE_ACTION_NO_ARG=0,
	CLIENT_CMD_TYPE_ENUM_GET_SET,
	CLIENT_CMD_TYPE_INT_GET_SET,
	CLIENT_CMD_TYPE_INT_GET_SET_WITH_DEV_ADDR,
	CLIENT_CMD_TYPE_ENUM_GET_SET_WITH_DEV_ADDR,
	CLIENT_CMD_TYPE_STRING_GET_SET_WITH_DEV_ADDR,
	CLIENT_CMD_TYPE_STRING_GET_SET_WITH_STRING_PARA,
	CLIENT_CMD_TYPE_STRING_GET_SET_WITH_ENUM_PARA,
	CLIENT_CMD_TYPE_GET_TASK_STATUS,
	CLIENT_CMD_TYPE_STRING_GET_SET,
	CLIENT_CMD_TYPE_USER_DEFINED,
	CLIENT_CMD_TYPE_GET_TASK_STATUS_WITH_DEV_ADDR,
	CLIENT_CMD_TYPE_DOUBLE_INT_SET,
	CLIENT_CMD_TYPE_UNKNOWN,
	CLIENT_CMD_TYPE_NONE
}CLIENT_CMD_TYPE;
//ADAV: this object is getting nasty, we need a better and dynamic way of handling different kind of objects using chains.
typedef struct CmdExecutionObj_t
{
	int command;
	CLIENT_CMD_TYPE cmd_type;
	RPC_SRV_ACT action;
	char get_rpc_name[JSON_RPC_METHOD_NAME_MAX_LENGTH];
	char set_rpc_name[JSON_RPC_METHOD_NAME_MAX_LENGTH];

	int cmd_int_val;//first_arg_int_value
	int cmd_enum_val;//first_arg_enum_value

	//char set_rpc_cmd_single_param_name[1024];//for int type set cmd with single parameter type
	char first_arg_param_name[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument
	char first_arg_param_value[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument value as string
	int  first_arg_param_int_value;	
    	double  first_arg_param_double_value;
	unsigned long first_arg_param_ulong_value;

	char second_arg_param_name[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument
	char second_arg_param_value[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument value as string
	int  second_arg_param_int_value;	
    	double  second_arg_param_double_value;
	unsigned long second_arg_param_ulong_value;


	char third_arg_param_name[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument
	char third_arg_param_value[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument value as string
	int  third_arg_param_int_value;	
    	double  third_arg_param_double_value;
	unsigned long third_arg_param_ulong_value;


	char fourth_arg_param_name[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument
	char fourth_arg_param_value[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument value as string
	int  fourth_arg_param_int_value;	
    	double  fourth_arg_param_double_value;
	unsigned long fourth_arg_param_ulong_value;


	char fifth_arg_param_name[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument
	char fifth_arg_param_value[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument value as string
	int  fifth_arg_param_int_value;	
    	double  fifth_arg_param_double_value;

	char sixth_arg_param_name[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument
	char sixth_arg_param_value[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument value as string
	int  sixth_arg_param_int_value;	
        double  sixth_arg_param_double_value;

	char seventh_arg_param_name[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument
	char seventh_arg_param_value[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument value as string
        int  seventh_arg_param_int_value;
        double  seventh_arg_param_double_value;

	char eighth_arg_param_name[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument
	char eighth_arg_param_value[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument value as string
	int  eighth_arg_param_int_value;
        double  eighth_arg_param_double_value;
	
	char ninth_arg_param_name[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument
	char ninth_arg_param_value[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument value as string
	int  ninth_arg_param_int_value;
        double  ninth_arg_param_double_value;
	
	char tenth_arg_param_name[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument
	char tenth_arg_param_value[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument value as string
	int  tenth_arg_param_int_value;
        double  tenth_arg_param_double_value;
	
	char eleventh_arg_param_name[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument
	char eleventh_arg_param_value[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument value as string
	int  eleventh_arg_param_int_value;
        double  eleventh_arg_param_double_value;

	char twelfth_arg_param_name[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument
	char twelfth_arg_param_value[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument value as string
	int  twelfth_arg_param_int_value;
        double  twelfth_arg_param_double_value;

	char thirteen_arg_param_name[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument
	char thirteen_arg_param_value[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument value as string
	int  thirteen_arg_param_int_value;
    double thirteen_arg_param_double_value;
	
	char fourteen_arg_param_name[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument
	char fourteen_arg_param_value[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument value as string
	int  fourteen_arg_param_int_value;
	
	char fifteen_arg_param_name[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument
	char fifteen_arg_param_value[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument value as string
	int  fifteen_arg_param_int_value;

	char sixteen_arg_param_name[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument
	char sixteen_arg_param_value[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument value as string
	float  sixteen_arg_param_float_value;

	char seventeen_arg_param_name[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument
	char seventeen_arg_param_value[JSON_RPC_METHOD_RESP_MAX_LENGTH];//for rpc types with argument value as string
	float  seventeen_arg_param_float_value;
	
	RPC_SRV_RESULT result;
	char rpc_resp_result[JSON_RPC_METHOD_RESP_MAX_LENGTH];//server response string
	//add further for other data types
}CmdExecutionObj;
//!!!!!!!!important: when ever adding removing members to CmdExecutionObj, please modify following function as well.
//int ADCmnCltCmdProcessor::copy_cmd_exec_obj(CmdExecutionObj *pCmdObjDest,CmdExecutionObj *pCmdObjSrc)



typedef struct OutputPrintMsgObj_t
{
	string ip;
	char rpc_method_name[JSON_RPC_METHOD_NAME_MAX_LENGTH];
	RPC_SRV_ACT action;
	char result[JSON_RPC_METHOD_RESP_MAX_LENGTH];
	char result_parameter[JSON_RPC_METHOD_RESP_MAX_LENGTH];
	double exec_time;//command execution time
	//char value;
}OutputPrintMsgObj;

class ADCmnCltCmdProcessor : public ADCmnStringProcessor
{
	//int print_ip(string ip);
public:
	ADCmnCltCmdProcessor();
	~ADCmnCltCmdProcessor();
	int copy_cmd_exec_obj(CmdExecutionObj *pCmdObjDest,CmdExecutionObj *pCmdObjSrc);
	int run_cmd_type_enum_get_set(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList);
	int run_cmd_type_int_get_set(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList);
	int run_cmd_type_int_get_set_with_dev_addr(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList);
	int run_cmd_type_enum_get_set_with_dev_addr(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList);
	int run_get_task_progress_command(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList);//,ADThreadedSockClientProducer *pWorker);

	int run_cmd_type_string_get_set_with_dev_addr(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList);
	int run_cmd_type_string_get_set(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList);
	int run_get_task_progress_command_with_dev_addr(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList);

	int run_cmd_type_action_noarg(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList);
	int run_cmd_type_string_get_set_with_enum_para(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList);

	int log_print_message(ADJsonRpcClient *pSrvSockConn/*string ip*/,char* rpc_name,RPC_SRV_ACT action,char* result,ADGenericChain *pOutMsgList);
	int log_print_message(ADJsonRpcClient *pSrvSockConn,char* rpc_method_name,RPC_SRV_ACT action,RPC_SRV_RESULT result,ADGenericChain *pOutMsgList,char* result_val);
	int run_cmd_type_double_int_set(CmdExecutionObj *pCmdObj,ADJsonRpcClient *pSrvSockConn,ADGenericChain *pOutMsgList);

};
#endif
