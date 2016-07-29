#include "ADJsonRpcProxy.hpp"
#include "ADCommon.hpp"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include <json.h>
using namespace std;
/*****************************************************************************/
//callback function from ADNetServer, upon arrival of tcp socket data, this
//function is called
int ADJsonRpcProxy::on_data_arrival(ADGenericChain* pRxChain,ADNetProducer* pObj)
{
	//int i=0;
	net_data_obj *req_obj;
	//once the element is removed, then ADJsonRpcProxy is responsible for freeing the memory
	while( (req_obj=(net_data_obj*)pRxChain->chain_get())!=NULL)
	//if(req_obj!=NULL)
	{
		//printf("ADJsonRpcProxy::on_data_arrival found request %d\n",i++);
		//cout<<req_obj->data_buffer<<endl;
		//req_obj->data_buffer[req_obj->data_buffer_len]='\0';//already done by ADNetServer

		//printf("req_frame = %s\n",req_obj->data_buffer);

		//notify_subscriber(req_obj->data_buffer);//char *rpc_method);
		if(json_process_request(req_obj)!=0)//req_obj->sock_descriptor,req_obj->data_buffer, req_obj->data_buffer_len)!=0)
		{
			//echo back data
			//Server.schedule_response(req_obj->sock_descriptor,req_obj->data_buffer, req_obj->data_buffer_len);
			//free(req_obj->data_buffer);
			ARRAY_MEM_DELETE(req_obj->data_buffer);
			//free(req_obj);
			OBJ_MEM_DELETE(req_obj);
		}
		//else, json_process_request() is responsible for keeping removed req_obj pointer in different chain
	}
	//dont know why this callback is called when there is no data in chain
	return 0;
}
/*****************************************************************************/
//chain-callback functions
int ADJsonRpcProxy::identify_chain_element(void* element,int ident,ADChainProducer* pObj)
{
	if(pObj->getID() == ReqRespChainID)//RespChainID)
	{
		struct api_task_obj* pTaskObj;
		pTaskObj=(api_task_obj*)element;
		if(pTaskObj->ident==ident)
			return 0;
	}
	return -1;
}
int ADJsonRpcProxy::double_identify_chain_element(void* element,int ident1,int ident2,ADChainProducer* pObj)
{
	return -1;
}
int ADJsonRpcProxy::free_chain_element_data(void* element,ADChainProducer* pObj)
{
	if(pObj->getID() == ReqRespChainID)
	{
		api_task_obj *objData;
		objData=(api_task_obj*)element;
		if(objData->pNetData->data_buffer!=NULL)
			MEM_DELETE(objData->pNetData->data_buffer);
		if(objData->pNetData!=NULL)
			MEM_DELETE(objData->pNetData);

		if(objData->json_resp_obj!=NULL)
			MEM_DELETE(objData->json_resp_obj);

		if(objData->json_resp_string!=NULL)
			MEM_DELETE(objData->json_resp_string);
	}
	return 0;
}
/*****************************************************************************/
//thread-callback functions
int ADJsonRpcProxy::monoshot_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	
	json_process_rsponse();
	
	return 0;
}
//blocking thread-callback not used here
int ADJsonRpcProxy::thread_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	return 0;	
}
/*****************************************************************************/
ADJsonRpcProxy::ADJsonRpcProxy()
{
	//pthread_mutex_init(&request_lock,NULL);
	socketlog=0;//by default, disable logging
	total_req_received=0;
	total_res_sent=0;


	//my-callback function to be called upon data arrival on tcp listener socket
	ServerSocket.attach_on_data_arrival(this);//attach myself
	ReqRespChainID=ReqRespChain.attach_helper(this);

	//initialize response-thread and chain
	RespChainID=RespChain.attach_helper(this);//this chain stores integer element
	RespThread.subscribe_thread_callback(this);
	RespThread.set_thread_properties(THREAD_TYPE_MONOSHOT,(void *)this);
	RespThread.start_thread();
}
ADJsonRpcProxy::~ADJsonRpcProxy()
{
	cout<<"ADJsonRpcProxy: total_req_received="<<total_req_received<<" total_res_sent="<<total_res_sent<<endl;
	RespThread.stop_thread();
	RespChain.remove_all();
	//clean-up my ReqRespChain elements
}


/*****************************************************************************/
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
char* ADJsonRpcProxy::get_timestamp()
{
	char buffer[30];
	struct timeval tv;
	time_t curtime;
	request_timestamp[0]='\0';
	gettimeofday(&tv, NULL); 
	curtime=tv.tv_sec;
	//strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));
	strftime(buffer,30,"%T.",localtime(&curtime));
	sprintf(request_timestamp,"%s%03ld",buffer,(tv.tv_usec/1000));
	return request_timestamp;
}
/*****************************************************************************/
int ADJsonRpcProxy::start_listening(int port,int socket_log)
{
	socketlog=socket_log;
	return ServerSocket.start_listening(port,socket_log);
}
/*****************************************************************************/
int ADJsonRpcProxy::json_checker_function(char *string)
{
	//Read STDIN. Exit with a message if the input is not well-formed JSON text.
	//jc will contain a JSON_checker with a maximum depth of 20.
	int i,str_len;
	str_len=strlen(string);	
	JSON_checker jc = new_JSON_checker(20);
	for (i=0;i<str_len;i++)
	{
		int next_char = string[i];//getchar();
		if (next_char <= 0)
		{
			break;
		}
		if (!JSON_checker_char(jc, next_char))
			return -1;			
	}
	if (!JSON_checker_done(jc))
		return -1;
	return 0;
}
/*****************************************************************************/
void ADJsonRpcProxy::print_json_value(json_object *jobj,json_type type)
{
	printf("type: ");
	switch (type)
	{
		case json_type_boolean: printf("json_type_boolean\n");
			printf("value: %s\n", json_object_get_boolean(jobj)? "true": "false");
			break;
		case json_type_double: printf("json_type_double\n");
			printf("          value: %lf\n", json_object_get_double(jobj));
			break;
		case json_type_int: printf("json_type_int\n");
			printf("          value: %d\n", json_object_get_int(jobj));
			break;
		case json_type_string: printf("json_type_string\n");
			printf("          value: %s\n", json_object_get_string(jobj));
			break;
		case json_type_null:
		case json_type_object:
		case json_type_array:
			break;
	}
}
/*****************************************************************************/
void ADJsonRpcProxy::json_parse(json_object * obj)
{
	char *key; struct json_object *val; struct lh_entry *entry; 
	enum json_type type;
	for(entry = json_object_get_object(obj)->head; (entry ? (key = (char*)entry->k, val = (struct json_object*)entry->v, entry) : 0); entry = entry->next)
	{ 
		/*Passing through every array element*/
		printf("type: ");//,type);
		type = json_object_get_type(val);
		switch (type)
		{
			case json_type_boolean:
			case json_type_double:
			case json_type_int:
			case json_type_string: 
				print_json_value(val,type);
				break;
			case json_type_object:
				{
				printf("json_type_object\n");
				json_object *jobj2 = json_object_object_get(obj, key);
				json_parse(jobj2);
				}
				break;
			case json_type_array:
				printf("type: json_type_array, ");
				json_parse_array(obj, key);
				break;
			case json_type_null:
				break;
			default:
				break;
		}
	}
}
/*****************************************************************************/
void ADJsonRpcProxy::json_parse_array( json_object *jobj, char *key)
{
	//void json_parse(json_object * jobj); //Forward Declaration
	enum json_type type;
	int i;
	int arraylen;

	json_object *jarray = jobj; //Simply get the array
	if(key)
	{
		jarray = json_object_object_get(jobj, key); //Getting the array if it is a key value pair
	}

	arraylen = json_object_array_length(jarray); //Getting the length of the array
	printf("Array Length: %d\n",arraylen);
	json_object * jvalue;

	for (i=0; i< arraylen; i++)
	{
		jvalue = json_object_array_get_idx(jarray, i); //Getting the array element at position i
		type = json_object_get_type(jvalue);
		if (type == json_type_array)
		{
			json_parse_array(jvalue, NULL);
		}
		else if (type != json_type_object)
		{
			printf("value[%d]: ",i);
			print_json_value(jvalue,type);
		}
		else
		{
			json_parse(jvalue);
		}
	}
}
/*****************************************************************************/
int ADJsonRpcProxy::check_jsonrpc_2_0(struct json_object *new_obj)
{
	char tmp_arr[256];
	new_obj = json_object_object_get(new_obj, "jsonrpc");
	
	if(json_object_get_string(new_obj)==NULL)
		return -1;

	sprintf(tmp_arr,"%s",json_object_get_string(new_obj));
	//printf("jsonrpc: %s\n",tmp_arr);			
	if(strcmp(tmp_arr,"2.0")==0)
		return 0;
	else
		return -1;
}
/*****************************************************************************/
ADJsonRpcCommand* ADJsonRpcProxy::get_jsonrpc_method(struct json_object *new_obj)
{
	char tmp_arr[JSON_RPC_METHOD_NAME_MAX_LENGTH];
	//int i=0,items=0;
	//json_rpc_method *pRpcLocal;

	new_obj = json_object_object_get(new_obj, "method");
	if(json_object_get_string(new_obj)==NULL)
		return NULL;
	sprintf(tmp_arr,"%s",json_object_get_string(new_obj));
	
	//check in my list if this method is registred by any consumerObject
	return(is_this_method_registered(tmp_arr));

	/*items=chain_size(&json_proc_obj->api_chain);
	for(i=0;i<items;i++)
	{
		pRpcLocal=(json_rpc_method *)chain_get_by_index(&json_proc_obj->api_chain,i);
		if(pRpcLocal==NULL)
			return NULL;//method not found
		
		if(strcmp(pRpcLocal->method_name,tmp_arr)==0)
		{
			return pRpcLocal;//method is registered, handle this	
		}
	}
	return NULL;*/
}
/*****************************************************************************/
CMD_TASK_ACC ADJsonRpcProxy::get_jsonrpc_acc(struct json_object *new_obj)
{
	char tmp_arr[256];
	new_obj = json_object_object_get(new_obj, "access");
	if(json_object_get_string(new_obj)==NULL)
		return CMD_TASK_ACCESS_SYNC;//if not specified explicitly, then this call is sync(blocking).
	sprintf(tmp_arr,"%s",json_object_get_string(new_obj));
	printf("access : %s\n",tmp_arr);
	if(strcmp(tmp_arr,"sync")==0)
		return CMD_TASK_ACCESS_SYNC;
	else if(strcmp(tmp_arr,"async")==0)
		return CMD_TASK_ACCESS_ASYNC;
	else
		return CMD_TASK_ACCESS_SYNC;//if requested json_rpc doesnt specify access_type, then default is sync(blocking)
}
/*****************************************************************************/
int ADJsonRpcProxy::get_jsonrpc_id(struct json_object *new_obj,int* req_id)
{
	char tmp_arr[256];
	new_obj = json_object_object_get(new_obj, "id");
	if(json_object_get_string(new_obj)==NULL)
		return -1;
	sprintf(tmp_arr,"%s",json_object_get_string(new_obj));
	//printf("id     : %s\n",tmp_arr);
	*req_id=atoi(tmp_arr);
	return 0;
}
/*****************************************************************************/
/*
{"jsonrpc": "2.0", "error": {"code": -32700, "message": "Parse error."}, "id": null}
{"jsonrpc": "2.0", "error": {"code": -32600, "message": "Invalid Request."}, "id": null}
{"jsonrpc": "2.0", "error": {"code": -32601, "message": "Method not found."}, "id": 1}
{"jsonrpc": "2.0", "error": {"code": -32600, "message": "Invalid params."}, "id": 1}
{"jsonrpc": "2.0", "error": {"code": -32700, "message": "Internal error."}, "id": null}
*/
/*****************************************************************************/
int ADJsonRpcProxy::json_send_error_response_string(int id,int sock_descriptor,JSON_RPC_ERR_TYPE err_type)//,ADCJsonProxyObj* json_proc_obj)
{
	char message[AD_NET_SERVER_MAX_BUFFER_SIZE];//65k
	json_object *intrnl_obj;
	json_object *error_obj;
	intrnl_obj = json_object_new_object();
	error_obj  = json_object_new_object();

	json_object_object_add(error_obj, "code"   , json_object_new_int(err_type));
	switch(err_type)
	{
		case JSON_RPC_ERR_PARSE_ERROR:
			json_object_object_add(error_obj, "message", json_object_new_string(JSON_RPC_PARSE_ERR_MSG));
			break;
		case JSON_RPC_ERR_INVALID_REQ:
			json_object_object_add(error_obj, "message", json_object_new_string(JSON_RPC_INVALID_REQ_MSG));
			break;
		case JSON_RPC_ERR_METHOD_NOT_FOUND:
			//internal_id=id;//in this case, return requestor's id
			json_object_object_add(error_obj, "message", json_object_new_string(JSON_RPC_METHOD_NOT_FOUND_MSG));
			break;
		case JSON_RPC_ERR_INVALID_PARAMS:
			//internal_id=id;//in this case, return requestor's id
			json_object_object_add(error_obj, "message", json_object_new_string(JSON_RPC_INVALID_PARAMS_MSG));
			break;
		default:		
			json_object_object_add(error_obj, "message", json_object_new_string(JSON_RPC_INTERNAL_ERR_MSG));
			break;
	}

	json_object_object_add(intrnl_obj,"jsonrpc", json_object_new_string(JSON_RPC_VERSION));
	json_object_object_add(intrnl_obj,"error", error_obj);
	
	switch(err_type)
	{
		case JSON_RPC_ERR_METHOD_NOT_FOUND:
		case JSON_RPC_ERR_INVALID_PARAMS:
			json_object_object_add(intrnl_obj, "id", json_object_new_int(id));
			break;
		default:		
			json_object_object_add(intrnl_obj, "id", NULL);
			break;
	}
	sprintf(message,"%s",json_object_to_json_string(intrnl_obj));//return the json formatted string
	//network_new_schedule_response(&json_proc_obj->NetserverObj,sock_descriptor,message,strlen(message));
	ServerSocket.schedule_response(sock_descriptor,message, strlen(message));//use my socket to send data
//#ifdef SOCKET_DEBUG_MESSAGE
	if(socketlog)
		printf("%s<--%s\n",get_timestamp(),message);
//#endif
	json_object_put(intrnl_obj);
	json_object_put(error_obj);
	return 0;	
}
/*****************************************************************************/
int ADJsonRpcProxy::json_send_result_response_string(int id,int sock_descriptor,char* result,api_task_obj* task_obj)
{
	char message[AD_NET_SERVER_MAX_BUFFER_SIZE];
	json_object *intrnl_obj;
	json_object *tmp_obj=NULL;
	intrnl_obj = json_object_new_object();
	
	json_object_object_add(intrnl_obj,"jsonrpc", json_object_new_string(JSON_RPC_VERSION));

	if(task_obj->json_resp_obj!=NULL) //json_object used for sending multiple result values
	{
		json_object *result_obj;
		result_obj=(json_object *)task_obj->json_resp_obj;
		json_object_object_add(intrnl_obj,"result", result_obj);
	}	
	else
	{
		tmp_obj=json_object_new_object();
		json_object_object_add(tmp_obj,"return",json_object_new_string("fail"));
		json_object_object_add(tmp_obj,"errorType",json_object_new_string(result));
		json_object_object_add(intrnl_obj,"result", tmp_obj);

		//json_object_object_add(intrnl_obj,"result", json_object_new_string(result)); //only this line existing in else condition before KP's change request
	}
	json_object_object_add(intrnl_obj,"id", json_object_new_int(id));

	sprintf(message,"%s",json_object_to_json_string(intrnl_obj));//return the json formatted string
	ServerSocket.schedule_response(sock_descriptor,message, strlen(message));
	if(socketlog)
		printf("%s<--%s\n",get_timestamp(),message);	

	if(tmp_obj!=NULL)
		json_object_put(tmp_obj);

	json_object_put(intrnl_obj);
	return 0;
}
/*****************************************************************************/
int ADJsonRpcProxy::json_single_string_response_sender(api_task_obj *resp_obj)//char* result_string,api_task_obj *resp_obj)//,ADCJsonProxyObj* json_proc_obj)
{
	if(resp_obj->task_result!=CMD_TASK_RESULT_SUCCESS)
		json_send_error_response_string (resp_obj->req_id,resp_obj->pNetData->sock_descriptor,JSON_RPC_ERR_INTERNAL_ERROR);//,json_proc_obj);
	if(resp_obj->json_resp_string_len>0)//task_datalen>0)
		json_send_result_response_string(resp_obj->req_id,resp_obj->pNetData->sock_descriptor,resp_obj->json_resp_string/*result_string*/,resp_obj);
	else
		json_send_result_response_string(resp_obj->req_id,resp_obj->pNetData->sock_descriptor,(char*)JSON_RESULT_UNKNOWN,resp_obj);
	return 0;
}
/*****************************************************************************/
int ADJsonRpcProxy::json_send_back_result(api_task_obj *resp_obj)//,ADCJsonProxyObj* json_proc_obj)
{
	char result_string[32];
	if(resp_obj->task_result==CMD_TASK_RESULT_SUCCESS)
		sprintf(result_string,JSON_RESULT_SUCCESS);//"OK");
	else if(resp_obj->task_result<0)
		sprintf(result_string,JSON_RESULT_FAIL);//"FAIL");
	else
		sprintf(result_string,JSON_RESULT_UNKNOWN);//"UNKNOWN");
	json_send_result_response_string(resp_obj->req_id,resp_obj->pNetData->sock_descriptor,result_string,resp_obj);
	return 0;
}
/*****************************************************************************/
int ADJsonRpcProxy::json_process_request(net_data_obj *req_obj)
{
	struct api_task_obj* pTaskObj=NULL;
	ADJsonRpcCommand* pRpcMethod=NULL;
	int json_req_id;//,req_res;
	struct json_object *new_obj=NULL;
	char* json_string=req_obj->data_buffer;

	//printf("json_proc_req: %s\n",req_obj->data_buffer);

	//do not support batchmode this is causing crashes when json string contains only "[]" or [{}]"
	if(json_string[0]=='[')
	{
		json_send_error_response_string(0,req_obj->sock_descriptor,JSON_RPC_ERR_INVALID_REQ);
		return -1;
	}
	
	//check if any syntax errors in the json object(return parse_error).
	if(json_checker_function(json_string)!=0)
	{
//#ifdef SOCKET_DEBUG_MESSAGE
		if(socketlog)
			printf("%s-->%s\n",get_timestamp(),json_string);
//#endif
		json_send_error_response_string(0,req_obj->sock_descriptor,JSON_RPC_ERR_PARSE_ERROR);
		return -1;
	}
	
	
	//!!!!!!!!!!!!!warning: parsing "[]" or "[{}]" is crasing the server due to segmentation error! 
	//TODO: handle these cases properly
	new_obj = json_tokener_parse(json_string);
	if(new_obj==NULL)
	{
//#ifdef SOCKET_DEBUG_MESSAGE
		if(socketlog)
			printf("%s-->%s\n",get_timestamp(),json_string);
//#endif
		json_send_error_response_string(0,req_obj->sock_descriptor,JSON_RPC_ERR_PARSE_ERROR);
		return -1;
	}	

	//check if its a valid jsonrpc_2.0 object
	if(check_jsonrpc_2_0(new_obj)==0)
		;//printf("its a valid jsonrpc 2.0\n");
	else //for time-being support both 1.0 and 2.0 versions
		;//printf("invalid json rpc\n");

	//get the requested method ID
	if(get_jsonrpc_id(new_obj,&json_req_id)!=0)
	{
		if(socketlog)
			printf("%s-->%s\n",get_timestamp(),json_string);
		json_send_error_response_string(0,req_obj->sock_descriptor,JSON_RPC_ERR_INVALID_REQ);
		json_object_put(new_obj);
		return -1;
	}

	pRpcMethod=get_jsonrpc_method(new_obj);
	if(pRpcMethod==NULL)
	{
		if(socketlog)
			printf("%s-->%s\n",get_timestamp(),json_string);
		json_send_error_response_string(json_req_id,req_obj->sock_descriptor,JSON_RPC_ERR_METHOD_NOT_FOUND);
		json_object_put(new_obj);
		return -1;
	}

//#ifdef SOCKET_DEBUG_MESSAGE
	if(socketlog)
		printf("%s-->%s\n",get_timestamp(),json_string);
//#endif
	//TODO: lock here to avoid other request:
	//pthread_mutex_lock(&request_lock);


	OBJECT_MEM_NEW(pTaskObj,api_task_obj);
	if(pTaskObj==NULL)
	{
		json_send_error_response_string(json_req_id,req_obj->sock_descriptor,JSON_RPC_ERR_INTERNAL_ERROR);
		json_object_put(new_obj);
		//TODO: unlock
		//pthread_mutex_unlock(&request_lock);
		return -1;
	}

	pTaskObj->ident=ReqRespChain.chain_generate_ident();
	pTaskObj->req_id=json_req_id;//caller's requestID
	pTaskObj->pNetData=req_obj;//keep original tcp-sockets reference in json-req-resp chain
	pTaskObj->pRpcMethod=pRpcMethod;//user's rpc-method-handler
	//int task_datalen;//network packet datalen
	pTaskObj->access          = get_jsonrpc_acc(new_obj);//if api_call doesnt specify access_type, then default is sync(blocking);
	pTaskObj->task_result     = CMD_TASK_RESULT_NOT_STARTED;
	pTaskObj->rpc_code        = RPC_SRV_RESULT_NOT_STARTED;//as per new unified error coding

	pTaskObj->result_code     = -32603;///internal json-rpc error
	pTaskObj->json_resp_obj   = NULL;
	pTaskObj->json_resp_string= NULL;//initially set response as null, mapper will fill this value
	pTaskObj->json_resp_string_len=0;//response string length
	pTaskObj->custom_result_string[0]='\0';

	//put the request into chain
	if(ReqRespChain.chain_put((void *)pTaskObj)!=0)
	{
		printf("failed! unable to push json-req-task-obj to chain!\n");
		json_send_error_response_string(json_req_id,req_obj->sock_descriptor,JSON_RPC_ERR_INTERNAL_ERROR);
		json_object_put(new_obj);
		OBJ_MEM_DELETE(pTaskObj);
		//TODO: unlock
		//pthread_mutex_unlock(&request_lock);
		return -1;
	}
	pTaskObj->pRpcMethod->pParent->rpc_call_notification(pTaskObj);//user registered callback function
	json_object_put(new_obj);//free json_object memory
	total_req_received++;
	//pthread_mutex_unlock(&request_lock);
	//TODO: unlock
	return 0;
}
/*****************************************************************************/
int ADJsonRpcProxy::json_send_response(struct api_task_obj* pTaskObj)
{
	//cout<<"in : ADJsonRpcProxy::json_send_response"<<endl;
	//inserts the ID of pTaskObj into chain and wakes-up json_process_rsponse() function

	//pthread_mutex_lock(&request_lock);

	int* pRespID=NULL;
	OBJECT_MEM_NEW(pRespID,int);
	if(pRespID==NULL)
	{
		//pthread_mutex_unlock(&request_lock);
		return -1;
	}

	*pRespID=pTaskObj->ident;
	if(RespChain.chain_put((void *)pRespID)!=0)
	{
		printf("failed! unable to push responseID to chain!\n");
		OBJ_MEM_DELETE(pRespID);
		//pthread_mutex_unlock(&request_lock);
		return -1;
	}
	//pthread_mutex_unlock(&request_lock);

	RespThread.wakeup_thread();//call json_process_rsponse(); in monoshot thread context
	return 0;
}
/*****************************************************************************/
int ADJsonRpcProxy::json_process_rsponse()
{
	//cout<<"in : ADJsonRpcProxy::json_process_rsponse"<<endl;

	struct api_task_obj* pTaskObj=NULL;
	int* pRespID=NULL;
	pRespID=(int*)RespChain.chain_get();
	if(pRespID==NULL)
	{
		//cout<<"in : ADJsonRpcProxy::json_process_rsponse returning because pRespID=NULL"<<endl;
		return -1;
	}
	
	pTaskObj=(api_task_obj*)ReqRespChain.chain_remove_by_ident(*pRespID);
	if(pTaskObj==NULL)
	{
		//cout<<"in : ADJsonRpcProxy::json_process_rsponse returning because chain-remove-by-ident failed"<<endl;
		OBJ_MEM_DELETE(pRespID);
		return -1;
	}
	//once data is removed, send the response and delete pTaskObj and its other memory references

	//TODO:send response()
	//json_send_back_result(pTaskObj);
	json_send_new_result(pTaskObj);
	
	//delete api_task_obj data
	if(pTaskObj->pNetData->data_buffer!=NULL)
		MEM_DELETE(pTaskObj->pNetData->data_buffer);
	if(pTaskObj->pNetData!=NULL)
		MEM_DELETE(pTaskObj->pNetData);
	if(pTaskObj->json_resp_obj!=NULL)
	{
		;//json_object_put(pTaskObj->json_resp_obj);
		//MEM_DELETE(pTaskObj->json_resp_obj);
	}
	if(pTaskObj->json_resp_string!=NULL)
		MEM_DELETE(pTaskObj->json_resp_string);

	OBJ_MEM_DELETE(pTaskObj);//delete the main holder

	OBJ_MEM_DELETE(pRespID);//delete this integer

	total_res_sent++;
	return 0;
}
/*****************************************************************************/
int ADJsonRpcProxy::json_send_new_result(api_task_obj *resp_obj)
{
	char result_string[512];

	switch(resp_obj->task_result)
	{
		case CMD_TASK_RESULT_FAIL         :sprintf(result_string,"%s",(char*)MSG_CMD_TASK_RESULT_FAIL);break;
		case CMD_TASK_RESULT_SUCCESS      :sprintf(result_string,"%s",(char*)MSG_CMD_TASK_RESULT_SUCCESS);break;
		case CMD_TASK_RESULT_INVALID_PARAM:sprintf(result_string,"%s",(char*)MSG_CMD_TASK_RESULT_INVALID_PARAM);break;
		case CMD_TASK_RESULT_DEV_TIMEOUT  :sprintf(result_string,"%s",(char*)MSG_CMD_TASK_RESULT_DEV_TIMEOUT);break;
		case CMD_TASK_RESULT_DEV_ERR      :sprintf(result_string,"%s",(char*)MSG_CMD_TASK_RESULT_DEV_ERR);break;
		case CMD_TASK_RESULT_MEM_FAIL     :sprintf(result_string,"%s",(char*)MSG_CMD_TASK_RESULT_MEM_FAIL);break;
		case CMD_TASK_RESULT_IN_PROGRESS  :sprintf(result_string,"%s",(char*)MSG_CMD_TASK_RESULT_IN_PROGRESS);break;
		case CMD_TASK_RESULT_ACTUAL_VALUE :sprintf(result_string,"%s",resp_obj->custom_result_string);break;
		case CMD_TASK_RESULT_UNKNOWN      :sprintf(result_string,"%s",(char*)MSG_CMD_TASK_RESULT_UNKNOWN);break;
		default:sprintf(result_string,"%s",(char*)MSG_CMD_TASK_RESULT_UNKNOWN);break;
	}
	json_send_result_response_string(resp_obj->req_id,resp_obj->pNetData->sock_descriptor,result_string,resp_obj);
	return 0;
}



