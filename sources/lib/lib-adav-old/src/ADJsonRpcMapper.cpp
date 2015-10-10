#include "ADJsonRpcMapper.hpp"
using namespace std;
/*****************************************************************************/
int ADJsonRpcMapper::rpc_call_notification(api_task_obj* pReqRespObj)
{
	if(ReqChain.chain_put((void *)pReqRespObj)!=0)//push the request to chain, and wake-up worker thread
	{
		printf("failed! unable to push responseID to chain!\n");
		return -1;
	}
	ReqThread.wakeup_thread();//monoshot_callback_function() gets called
	return 0;
}
/*****************************************************************************/
//request thread-callback function
int ADJsonRpcMapper::monoshot_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	api_task_obj* pApiObj=NULL;
	pApiObj=(api_task_obj*)ReqChain.chain_get();
	if(pApiObj==NULL)
		return -1;

	ADJsonRpcProxy* pJson;
	pJson = (ADJsonRpcProxy*)pApiObj->pRpcMethod->pProducer;

	JsonDataCommObj DataObj;
	DataObj.req_id                    = pApiObj->req_id;//client provided req id through json string
	DataObj.cmd_index                 = pApiObj->pRpcMethod->method_index;
	DataObj.mapper_result             = CMD_TASK_RESULT_UNKNOWN;
	DataObj.rpc_code                  = RPC_SRV_RESULT_NOT_STARTED;
	DataObj.worker_result             = -1;//different workers will have different return value
	DataObj.socket_data               = pApiObj->pNetData->data_buffer;//actual text data received on json-socket
	DataObj.json_resp_obj             = pApiObj->json_resp_obj;   //for returning more than one value
	DataObj.custom_result_string      = pApiObj->custom_result_string;
	DataObj.dataObjLen                = 0;
	DataObj.pDataObj                  = NULL;

	//client connection info(port,ip,ident) - needed for eventing;
	DataObj.cltport                   = pApiObj->pNetData->port ;//client info port
	strcpy(DataObj.ip,                  pApiObj->pNetData->ip);//client info ip
	DataObj.sock_id                   = pApiObj->pNetData->cltid; //client connection's info chain-id
	DataObj.sock_descr                = pApiObj->pNetData->sock_descriptor;//client connection socket descriptor

	if(process_json_to_binary(&DataObj)!=0)
	{
		pApiObj->task_result   = DataObj.mapper_result;
		pApiObj->rpc_code      = DataObj.rpc_code;
		pApiObj->json_resp_obj = DataObj.json_resp_obj;//in case if mapper wants to send its own result
		pJson->json_send_response(pApiObj);
		return 0;
	}

	//do the work
	process_work(&DataObj);

	//do the mapping again for returning result: binary-to-json
	process_binary_to_json(&DataObj);

	//put the mapper's result back into original request-object
	pApiObj->task_result   = DataObj.mapper_result;
	pApiObj->rpc_code      = DataObj.rpc_code;
	pApiObj->json_resp_obj = DataObj.json_resp_obj;//in case if mapper wants to send its own result

	//send the json response
	pJson->json_send_response(pApiObj);//(pJson pointer is supplied along with request-object)
	return 0;
}
/*****************************************************************************/
ADJsonRpcMapper::ADJsonRpcMapper()
{
	pJsonProxy=NULL;
	//init my chain
	ReqChain.disable_auto_remove();
	ReqChain.attach_helper(this);//attach callbacks to request-chain
	//init my thread
	ReqThread.subscribe_thread_callback(this);
	ReqThread.set_thread_properties(THREAD_TYPE_MONOSHOT,(void *)this);
	ReqThread.start_thread();

}
ADJsonRpcMapper::~ADJsonRpcMapper()
{
	//stop the thread and clean-up chain
	ReqThread.stop_thread();
	ReqChain.remove_all();//remove all of my hooks(but dont remove elements,because i have disabled_auto_remove)
}
/*****************************************************************************/
int ADJsonRpcMapper::AttachProxy(ADJsonRpcProxy* pJProxy)
{
	pJsonProxy=pJProxy;//remember parents pointer
	return 0;
}
/*****************************************************************************/
int ADJsonRpcMapper::attach_rpc_method(int index,char* method_name)
{
	if(pJsonProxy==NULL)
	{
		cout<<"ADJsonRpcMapper::attach_rpc_method: Error!! parent is not attached"<<endl;
		return -1;
	}

	pJsonProxy->attach_rpc_method(this,method_name,index);//attach rpc methods to JsonProxy
	return 0;
}
/*****************************************************************************/
