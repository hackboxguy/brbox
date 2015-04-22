#ifndef __ADJSON_RPC_H_
#define __ADJSON_RPC_H_
#include <vector>
#include <typeinfo>
#include <string>
#include <iostream>
#include <stdint.h>
#include "ADCommon.hpp"
#include "JsonCmnDef.h"
#include "ADJsonRpcMapper.hpp"
#include "ADTimer.hpp"
#include "ADTaskWorker.hpp"


using namespace std;
/*****************************************************************************/
//to understand this, read C++ subject observer pattern
class ADJsonRpcMgrProducer; //subject
class ADJsonRpcMgrConsumer  //observer
{
//protected:
//	std::string strGetRpc;
//	std::string strSetRpc;
public:
	int index;
	std::string strRpcName;
	//std::string strSetRpc;

	std::string GetRpcName(){return strRpcName;};
	//std::string GetRpcNameSetType(){return strGetRpc;};
	//std::string GetPageTag(){return Tag;};
	//std::string GetPageOverrideName(){return OvrName;};
	//int GetMenuInfo(mnuTypes_t &menuType,std::string &menuName,std::string &menuTag)//,std::string &NameOveride="none")
	//{
	//	menuType=Menu;
	//	menuName=Name;
	//	menuTag=Tag;
	//	return 0;
	//};
	//ADJsonRpcMgrConsumer(std::string getrpc,std::string setrpc){strGetRpc=getrpc;strSetRpc=setrpc;};
	ADJsonRpcMgrConsumer(std::string rpc_name,int consumer_index){strRpcName=rpc_name;index=consumer_index;};
	virtual ~ADJsonRpcMgrConsumer(){};
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index)=0;
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index)=0;
	//virtual int WorkRpc(RPC_SRV_REQ *pPanelReq/*data-cache?*/)=0;
	virtual int ProcessWork(JsonDataCommObj* pReq,int index)=0;
	virtual int ProcessWorkAsync(unsigned char* pWorkData)=0;

};
/*****************************************************************************/
class ADJsonRpcMgrProducer
{
	std::vector<ADJsonRpcMgrConsumer*> rpclist;
protected:
	ADJsonRpcMgrConsumer* getRpcHandler(std::string rpcName) //based on rpc-name string, returns the object ptr
	{
		std::vector<ADJsonRpcMgrConsumer*>::iterator iter;
		for(iter=rpclist.begin();iter != rpclist.end();++iter)
		{
			if((*iter)->GetRpcName() == rpcName)
				return (*iter);
			//else if((*iter)->GetRpcNameSetType() == rpcName)
			//	return (*iter);
		}
		return NULL;
	}
	ADJsonRpcMgrConsumer* getRpcHandler(int index) //based on index, returns the object ptr
	{
		std::vector<ADJsonRpcMgrConsumer*>::iterator iter = rpclist.begin() + index;
		return (*iter);
	}
public:
	virtual ~ADJsonRpcMgrProducer(){};
	void AttachRpc(ADJsonRpcMgrConsumer* pRpc)
	{
		rpclist.push_back(pRpc);
	}
	int get_total_attached_rpcs()
	{
		return rpclist.size();
	}
	int MapJsonToBinary(JsonDataCommObj* pReq)
	{
		if(pReq->cmd_index>=get_total_attached_rpcs())
			return -1;//not my call, i dont have an object of this index
		ADJsonRpcMgrConsumer* pPageHandler = getRpcHandler(pReq->cmd_index);
		if(pPageHandler==NULL)
			return -1;
		else
			return pPageHandler->MapJsonToBinary(pReq,pPageHandler->index);
	}
	int MapBinaryToJson(JsonDataCommObj* pReq)
	{
		if(pReq->cmd_index>=get_total_attached_rpcs())
			return -1;//not my call, i dont have an object of this index
		ADJsonRpcMgrConsumer* pPageHandler = getRpcHandler(pReq->cmd_index);
		if(pPageHandler==NULL)
			return -1;
		else
			return pPageHandler->MapBinaryToJson(pReq,pPageHandler->index);
	}
	//int WorkRpc(RPC_SRV_REQ *pPanelReq)
	int ProcessWork(JsonDataCommObj* pReq)
	{
		if(pReq->cmd_index>=get_total_attached_rpcs())
			return -1;//not my call, i dont have an object of this index
		ADJsonRpcMgrConsumer* pPageHandler = getRpcHandler(pReq->cmd_index);
		if(pPageHandler==NULL)
			return -1;
		else
			return pPageHandler->ProcessWork(pReq,pPageHandler->index);
	}
	int ProcessWorkAsync(unsigned char* pWorkData)
	{
		return -1;
	}

};
/*****************************************************************************/
class ADJsonRpcMgr : public ADJsonRpcMgrProducer, public ADJsonRpcMapConsumer, public ADTaskWorkerConsumer, public ADTimerConsumer
{
	ADTimer* myTimer;
	ADJsonRpcProxy  Proxy;//json-net-proxy
	ADJsonRpcMapper	JMapper;

	//ADJsonRpcMapConsumer overrides
	virtual int process_json_to_binary(JsonDataCommObj* pReq);//{return 0;};//only applicable for mapper
	virtual int process_binary_to_json(JsonDataCommObj* pReq);//{return 0;};//only applicable for mapper
	virtual int process_work(JsonDataCommObj* pReq);//{return 0;};

	//ADTaskWorkerConsumer override
	virtual RPC_SRV_RESULT run_work(int cmd,unsigned char* pWorkData,ADTaskWorkerProducer *pTaskWorker){return RPC_SRV_RESULT_SUCCESS;};

	//ADTimerConsumer overrides: 100ms timer and sigio 
	virtual int sigio_notification(){return 0;};
	virtual int timer_notification(){return 0;};
	virtual int custom_sig_notification(int signum){return 0;};

public:
	ADJsonRpcMgr();
	~ADJsonRpcMgr();
	int AttachHeartBeat(ADTimer* pTimer);
	int Start(int port,int socket_log,int emulation);

};
/*****************************************************************************/

#endif
