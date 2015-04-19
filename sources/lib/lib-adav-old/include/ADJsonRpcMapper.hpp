#ifndef __AD_JSON_RPC_MAPPER_H_
#define __AD_JSON_RPC_MAPPER_H_
#include "ADJsonRpcProxy.hpp"
#include "ADThread.hpp"
#include "ADGenericChain.hpp"
/*****************************************************************************/
//common macros for all the mappers of different services

/*****************************************************************************/
//to understand this, read C++ subject observer pattern
class ADJsonRpcMapProducer; //subject
class ADJsonRpcMapConsumer //observer
{
public:
	virtual int process_json_to_binary(JsonDataCommObj* pReq)=0;
	virtual int process_work(JsonDataCommObj* pReq)=0;
	virtual int process_binary_to_json(JsonDataCommObj* pReq)=0;
	virtual ~ADJsonRpcMapConsumer(){};
};
/*****************************************************************************/
class ADJsonRpcMapProducer
{
	ADJsonRpcMapConsumer *pConsumerMapper;//callback of mapper
	ADJsonRpcMapConsumer *pConsumerWorker;//callback of worker
protected:
	int process_json_to_binary(JsonDataCommObj* pReq)
	{
		if(pConsumerMapper!=NULL)		
			return pConsumerMapper->process_json_to_binary(pReq);
		return -1;
	}
	int process_work(JsonDataCommObj* pReq)
	{
		if(pConsumerWorker!=NULL)		
			return pConsumerWorker->process_work(pReq);
		return -1;
	}
	int process_binary_to_json(JsonDataCommObj* pReq)
	{
		if(pConsumerMapper!=NULL)		
			return pConsumerMapper->process_binary_to_json(pReq);
		return -1;
	}
public:
	ADJsonRpcMapProducer() {pConsumerMapper=NULL;pConsumerWorker=NULL;}
	virtual ~ADJsonRpcMapProducer(){};
	int AttachMapper(ADJsonRpcMapConsumer* c)
	{
		//allow only one mapper to be attached
		if(pConsumerMapper==NULL)
		{
			pConsumerMapper=c;
			return 0;
		}
		else
			return -1;//some other mapper has already been attached
	}
	int AttachWorker(ADJsonRpcMapConsumer* c)
	{
		//allow only one worker to be attached
		if(pConsumerWorker==NULL)
		{
			pConsumerWorker=c;
			return 0;
		}
		else
			return -1;//some other worker has already been attached
	}
};
/*****************************************************************************/
class ADJsonRpcMapper : public ADJsonRpcConsumer, public ADThreadConsumer, public ADChainConsumer, public ADJsonRpcMapProducer
{
	ADJsonRpcProxy* pJsonProxy;
	ADThread        ReqThread;//decoupler for incoming requests from ADJsonRpcProxy
	ADGenericChain  ReqChain;//chain for storing incoming requests

	//json proxy callback(json-proxy will call this function on external rpc call)
	virtual int rpc_call_notification(api_task_obj* pReqRespObj);//used in this class

	//thread-callback functions(This manager needs a thread to process incoming requests)
	virtual int monoshot_callback_function(void* pUserData,ADThreadProducer* pObj);//used in this class
	virtual int thread_callback_function(void* pUserData,ADThreadProducer* pObj){return 0;};

	//Chain-callback functions(this manager keeps the incoming requests in this chain).
	virtual int identify_chain_element(void* element,int ident,ADChainProducer* pObj){return -1;};
	virtual int double_identify_chain_element(void* element,int ident1,int ident2,ADChainProducer* pObj){return -1;};
	virtual int free_chain_element_data(void* element,ADChainProducer* pObj){return 0;};
public:
	ADJsonRpcMapper();
	~ADJsonRpcMapper();
	int AttachProxy(ADJsonRpcProxy* pJProxy);
	int attach_rpc_method(int index,char* method_name);
};

#endif
