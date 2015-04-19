#ifndef __ADJSONRPCPROXY_H_
#define __ADJSONRPCPROXY_H_
#include <vector>
#include <typeinfo>
#include <string>
#include <iostream>

//#include <json-c/json.h>
#include "ADJsonChecker.hpp"
#include "ADGenericChain.hpp"
#include "ADNetServer.hpp"
//#include "DisplayPanelCmnDef.h"
#include "JsonCmnDef.h"
using namespace std;
/*****************************************************************************/
typedef enum cmd_task_access_t
{
	CMD_TASK_ACCESS_SYNC,
	CMD_TASK_ACCESS_ASYNC,
	CMD_TASK_ACCESS_NONE
}CMD_TASK_ACC;
/*typedef enum cmd_task_requestor_t
{
	CMD_TASK_REQUESTOR_JSON_RPC,
	CMD_TASK_REQUESTOR_BINARY,
	CMD_TASK_REQUESTOR_XMPP,
	CMD_TASK_REQUESTOR_INTERNAL,
	CMD_TASK_REQUESTOR_NONE
}CMD_TASK_REQUESTOR;*/

#define JSON_RPC_VERSION         "2.0"
#define JSON_RPC_PARSE_ERR_MSG        "Parse error."
#define JSON_RPC_INVALID_REQ_MSG      "Invalid Request."
#define JSON_RPC_METHOD_NOT_FOUND_MSG "Method not found."
#define JSON_RPC_INVALID_PARAMS_MSG   "Invalid params."
#define JSON_RPC_INTERNAL_ERR_MSG     "Internal error."
typedef enum JSON_RPC_ERR_TYPE_T
{
	//invalid JSON was received by server.
	//or an error occured on server while parsing the JSON text
	JSON_RPC_ERR_PARSE_ERROR     = -32700,
	JSON_RPC_ERR_INVALID_REQ     = -32600,//the JSON sent is not a valid request obj
	JSON_RPC_ERR_METHOD_NOT_FOUND= -32601,//method doesnot exist or not available
	JSON_RPC_ERR_INVALID_PARAMS  = -32602,//invalid method parameters
	JSON_RPC_ERR_INTERNAL_ERROR  = -32603,//internal JSON-RPC error
	//RESERVER ERRORS by Json spec: -32000 to -32099
	JSON_RPC_ERR_UNKNOWN_ERROR   = -33000 //
}JSON_RPC_ERR_TYPE;
#define JSON_RESULT_SUCCESS "OK"
#define JSON_RESULT_FAIL    "FAIL"
#define JSON_RESULT_UNKNOWN "UNKNOWN"

#define TASK_RESULT_STRING_NAME     "result"
#define TASK_RESULT_STRING_FAIL     "Fail"
#define TASK_RESULT_STRING_SUCCESS  "Success"

/*****************************************************************************/
struct api_task_obj;//forward declaration
class ADJsonRpcProducer; //subject
class ADJsonRpcConsumer //observer
{
public:
	virtual int rpc_call_notification(api_task_obj* pReqRespObj)=0;//int method_index)=0;
	virtual ~ADJsonRpcConsumer(){};
};
/*****************************************************************************/
struct ADJsonRpcCommand
{	
	ADJsonRpcConsumer* pParent;
	char method[JSON_RPC_METHOD_NAME_MAX_LENGTH];
	int method_index;
	ADJsonRpcProducer* pProducer;//with this pointer, consumer can callback producer's function(ex:for sending response).
public:
	ADJsonRpcCommand(){};//cout<<"ADJsonRpcCommand::Constructor"<<endl;};
	~ADJsonRpcCommand(){};//cout<<"ADJsonRpcCommand::Destructor"<<endl;};
};
/*****************************************************************************/
class ADJsonRpcProducer
{
	std::vector<ADJsonRpcCommand*> rpc_call_list;
protected:
	/*
	void notify_subscriber(char *rpc_method)
	{
		std::vector<ADJsonRpcCommand*>::iterator iter;
		for(iter=rpc_call_list.begin();iter != rpc_call_list.end();++iter)
		{
			if(strcmp((*iter)->method,rpc_method)==0)
				(*iter)->pParent->rpc_call_notification((*iter)->method_index);
		}
	}
	*/
	ADJsonRpcCommand* is_this_method_registered(char *rpc_method)
	{
		std::vector<ADJsonRpcCommand*>::iterator iter;
		for(iter=rpc_call_list.begin();iter != rpc_call_list.end();++iter)
		{
			if(strcmp((*iter)->method,rpc_method)==0)
				return (*iter); 				
				//(*iter)->pParent->rpc_call_notification((*iter)->method_index);
		}
		return NULL;
	}

public:
	virtual ~ADJsonRpcProducer()
	{
		//delete all dynamically created json-rpc call elements
		std::vector<ADJsonRpcCommand*>::iterator iter;
		for(iter=rpc_call_list.begin();iter != rpc_call_list.end();++iter)
			OBJ_MEM_DELETE((*iter));//delete (*iter);

	};
	int attach_rpc_method(ADJsonRpcConsumer* pConsumer,char* method_name,int method_indx)
	{
		ADJsonRpcCommand* pList=NULL;
		if(strlen(method_name)>=JSON_RPC_METHOD_NAME_MAX_LENGTH)
			return -1;//dont handle lengthy method-names
		//pList=new ADJsonRpcCommand;
		OBJECT_MEM_NEW(pList,ADJsonRpcCommand);
		if(pList!=NULL)
		{
			pList->pParent=pConsumer;
			strcpy(pList->method,method_name);
			pList->method_index=method_indx;
			pList->pProducer=this;//consumer can call me back;
			rpc_call_list.push_back(pList);
			return 0;
		}
		else 
			return -1;
	}
};
/*****************************************************************************/
struct api_task_obj
{
	int ident;//for internal use
	int req_id;//json-req-id from user's tcp rquest packet
	net_data_obj* pNetData;//data from tcp-socket
	ADJsonRpcCommand* pRpcMethod;//consumer's api-method handler
	CMD_TASK_ACC access;
	CMD_TASK_RESULT task_result;
	RPC_SRV_RESULT  rpc_code;//as per unified error code b/w client and server
	RPC_SRV_ACT     rpc_action;//as per unified error code b/w client and server


	int result_code;//json_error_code: typically -38000 range of error code
	json_object *json_resp_obj;
	char *json_resp_string;//response string to be prepared for sending the reply
	int json_resp_string_len;//
	char custom_result_string[JSON_RPC_METHOD_RESP_MAX_LENGTH];
public:
	api_task_obj(){};
	~api_task_obj(){};
};
/*****************************************************************************/
class ADJsonRpcProxy: public ADJsonRpcProducer, public ADNetConsumer, public ADChainConsumer, public ADThreadConsumer
{
	//pthread_mutex_t request_lock;
	char request_timestamp[255];
	int socketlog;//enable/disable socket req/resp logging
	int total_req_received;
	int total_res_sent;
	//std::vector<ADTimerConsumer*> subscribers;
	ADNetServer ServerSocket;

	//creation/deletion of this chain element is ADJsonRpcProxy's responsibility
	ADGenericChain ReqRespChain;int ReqRespChainID;

	//used for sending response
	ADGenericChain RespChain;
	int RespChainID;
	ADThread RespThread;


	/*********************************************************************/
	//helper callback functions from ADNetServer,
	virtual int on_data_arrival(ADGenericChain* pRxChain,ADNetProducer* pObj);

	//Chain-callback functions	
	virtual int identify_chain_element(void* element,int ident,ADChainProducer* pObj);
	virtual int double_identify_chain_element(void* element,int ident1,int ident2,ADChainProducer* pObj);
	virtual int free_chain_element_data(void* element,ADChainProducer* pObj);

	//thread-callback functions
	virtual int monoshot_callback_function(void* pUserData,ADThreadProducer* pObj);//{return 0;};
	virtual int thread_callback_function(void* pUserData,ADThreadProducer* pObj);//{return 0;};


	/*********************************************************************/
	char* get_timestamp();
	int json_checker_function(char *string);
	void print_json_value(json_object *jobj,json_type type);
	void json_parse(json_object * obj);
	void json_parse_array( json_object *jobj, char *key);
	int  check_jsonrpc_2_0(struct json_object *new_obj);
	ADJsonRpcCommand* get_jsonrpc_method(struct json_object *new_obj);//check if method is registered
	CMD_TASK_ACC get_jsonrpc_acc(struct json_object *new_obj);
	int get_jsonrpc_id(struct json_object *new_obj,int* req_id);
	int json_send_error_response_string(int id,int sock_descriptor,JSON_RPC_ERR_TYPE err_type);
	int json_send_result_response_string(int id,int sock_descriptor,char* result,api_task_obj* task_obj);
	int json_single_string_response_sender(api_task_obj *resp_obj);//char* result_string,api_task_obj *resp_obj);
	int json_send_back_result(api_task_obj *resp_obj);
	int json_process_request(net_data_obj *req_obj);
	int json_process_rsponse();

	int json_send_new_result(api_task_obj *resp_obj);

public:
	ADJsonRpcProxy();
	~ADJsonRpcProxy();
	int start_listening(int port,int socket_log);
	int json_send_response(struct api_task_obj* pTaskObj);

};
#endif

