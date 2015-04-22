#include "ADJsonRpcMgr.hpp"
/*****************************************************************************/
ADJsonRpcMgr::ADJsonRpcMgr()
{
	JMapper.AttachProxy(&Proxy);//let Proxy know who to call back for mapping
	JMapper.AttachMapper(this); //attach myself as mapper, jmapper shall call me back for mapper functions
	JMapper.AttachWorker(this); //attach myself as worker

}
/*****************************************************************************/
ADJsonRpcMgr::~ADJsonRpcMgr()
{
}
/*****************************************************************************/
int ADJsonRpcMgr::AttachHeartBeat(ADTimer* pTimer)
{
	//give 100ms heartbeat to ADDisplayMgr
	//this is needed for detecting 3sec timeout for uart communication	
	pTimer->subscribe_timer_notification(this);
	myTimer=pTimer;
	return 0;
}
/*****************************************************************************/
int ADJsonRpcMgr::Start(int port,int socket_log,int emulation)
{
	//TODO:myCmdLine.get_emulation_mode()
	Proxy.start_listening(port,socket_log);
	//TODO: attach my own first 6 common rpc's
	int total = get_total_attached_rpcs();
	for(int i=0;i<total;i++)
	{
		ADJsonRpcMgrConsumer* rpc=getRpcHandler(i);
		char rpc_name[1024];strcpy(rpc_name, rpc->strRpcName.c_str());
		JMapper.attach_rpc_method(i,rpc_name);
	}
	//TODO: TyWorker.SetReady();
	return 0;
}
/*****************************************************************************/
int ADJsonRpcMgr::process_json_to_binary(JsonDataCommObj* pReq)
{
	return  MapJsonToBinary(pReq);//callback of the attached object from the list
}
int ADJsonRpcMgr::process_binary_to_json(JsonDataCommObj* pReq)
{
	return MapBinaryToJson(pReq);//callback of the attached object from the list
}
int ADJsonRpcMgr::process_work(JsonDataCommObj* pReq)
{
	return ProcessWork(pReq);//callback of the attached object from the list
}
/*****************************************************************************/

