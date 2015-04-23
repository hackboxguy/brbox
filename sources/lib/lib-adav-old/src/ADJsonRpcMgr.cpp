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
	char rpc_name[1024];
	//TODO:myCmdLine.get_emulation_mode()
	Proxy.start_listening(port,socket_log);
	//attach first 7 rpc's which are common to all services
	JMapper.attach_rpc_method(EJSON_RPCGMGR_GET_TASK_STS       ,(char*)RPCMGR_RPC_TASK_STS_GET);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_GET_RPC_SRV_VERSION,(char*)RPCMGR_RPC_SER_VER_GET);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_TRIGGER_DATASAVE   ,(char*)RPCMGR_RPC_TRIG_SAVE);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_GET_SETTINGS_STS   ,(char*)RPCMGR_RPC_STTNG_STS_GET);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_SHUTDOWN_SERVICE   ,(char*)RPCMGR_RPC_TRIG_SHUTDOWN);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_RESET_TASK_STS     ,(char*)RPCMGR_RPC_RESET_TASKSTS);
	JMapper.attach_rpc_method(EJSON_RPCGMGR_GET_READY_STS      ,(char*)RPCMGR_RPC_READY_STS_GET);

	int total = get_total_attached_rpcs();
	for(int i=0;i<total;i++) 
	{
		ADJsonRpcMgrConsumer* rpc=getRpcHandler(i);//get the attached object pointer by index
		strcpy(rpc_name, rpc->strRpcName.c_str());
		JMapper.attach_rpc_method(i+EJSON_RPCGMGR_CMD_END,rpc_name);//attached with index offset of EJSON_RPCGMGR_CMD_END(7)
	}
	//TODO: TyWorker.SetReady();
	return 0;
}
/*****************************************************************************/
int ADJsonRpcMgr::process_json_to_binary(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	EJSON_RPCGMGR_CMD cmd = (EJSON_RPCGMGR_CMD)pPanelReq->cmd;
	if(cmd>=EJSON_RPCGMGR_CMD_END)
		return  MapJsonToBinary(pReq);//callback of the attached object from the list
	else
		return  MyMapJsonToBinary(pReq);//handle my own rpc calls
}
int ADJsonRpcMgr::process_binary_to_json(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	EJSON_RPCGMGR_CMD cmd = (EJSON_RPCGMGR_CMD)pPanelReq->cmd;
	if(cmd>=EJSON_RPCGMGR_CMD_END)
		return MapBinaryToJson(pReq);//callback of the attached object from the list
	else
		return MyMapBinaryToJson(pReq);//handle my own rpc calls
}
int ADJsonRpcMgr::process_work(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	EJSON_RPCGMGR_CMD cmd = (EJSON_RPCGMGR_CMD)pPanelReq->cmd;
	if(cmd>=EJSON_RPCGMGR_CMD_END)
		return ProcessWork(pReq);//callback of the attached object from the list
	else
		return MyProcessWork(pReq);//handle my own rpc calls
}
/*****************************************************************************/
int ADJsonRpcMgr::MyMapJsonToBinary(JsonDataCommObj* pReq)
{
	//TODO
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	EJSON_RPCGMGR_CMD cmd = (EJSON_RPCGMGR_CMD)pPanelReq->cmd;
	switch(cmd)
	{
		case EJSON_RPCGMGR_GET_TASK_STS       :break;
		case EJSON_RPCGMGR_GET_RPC_SRV_VERSION:break;
		case EJSON_RPCGMGR_TRIGGER_DATASAVE   :break;
		case EJSON_RPCGMGR_GET_SETTINGS_STS   :break;
		case EJSON_RPCGMGR_SHUTDOWN_SERVICE   :break;
		case EJSON_RPCGMGR_RESET_TASK_STS     :break;
		case EJSON_RPCGMGR_GET_READY_STS      :break;
		default:break;
	}
	return -1;
}
/*****************************************************************************/
int ADJsonRpcMgr::MyMapBinaryToJson(JsonDataCommObj* pReq)
{
	//TODO
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	EJSON_RPCGMGR_CMD cmd = (EJSON_RPCGMGR_CMD)pPanelReq->cmd;
	switch(cmd)
	{
		case EJSON_RPCGMGR_GET_TASK_STS       :break;
		case EJSON_RPCGMGR_GET_RPC_SRV_VERSION:break;
		case EJSON_RPCGMGR_TRIGGER_DATASAVE   :break;
		case EJSON_RPCGMGR_GET_SETTINGS_STS   :break;
		case EJSON_RPCGMGR_SHUTDOWN_SERVICE   :break;
		case EJSON_RPCGMGR_RESET_TASK_STS     :break;
		case EJSON_RPCGMGR_GET_READY_STS      :break;
		default:break;
	}
	return -1;
}
/*****************************************************************************/
int ADJsonRpcMgr::MyProcessWork(JsonDataCommObj* pReq)
{
	//TODO
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	EJSON_RPCGMGR_CMD cmd = (EJSON_RPCGMGR_CMD)pPanelReq->cmd;
	switch(cmd)
	{
		case EJSON_RPCGMGR_GET_TASK_STS       :break;
		case EJSON_RPCGMGR_GET_RPC_SRV_VERSION:break;
		case EJSON_RPCGMGR_TRIGGER_DATASAVE   :break;
		case EJSON_RPCGMGR_GET_SETTINGS_STS   :break;
		case EJSON_RPCGMGR_SHUTDOWN_SERVICE   :break;
		case EJSON_RPCGMGR_RESET_TASK_STS     :break;
		case EJSON_RPCGMGR_GET_READY_STS      :break;
		default:break;
	}
	return -1;
}
