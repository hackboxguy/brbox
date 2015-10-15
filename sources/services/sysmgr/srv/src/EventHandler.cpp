#include "EventHandler.h"
#include "ADJsonRpcClient.hpp"
/* ------------------------------------------------------------------------- */
EventHandler:: EventHandler(std::string rpcName,int myIndex,bool emu,bool log,SYSMGR_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
	pDataCache=pData;
	int srvToken=-1;
	SUBSCRIBE_EVENT(40002,-1,"127.0.0.1",40001,srvToken);
}
/* ------------------------------------------------------------------------- */
EventHandler::~ EventHandler()
{
}
/* ------------------------------------------------------------------------- */
void EventHandler::ReceiveEvent(int cltToken,int evntNum,int evntArg)
{
	std::cout << "EventHandler::ReceiveEvent: clt_token = " <<cltToken<<" evnt_num = "<<evntNum<<" evnt_arg = "<<evntArg <<endl;
}
/* ------------------------------------------------------------------------- */
//SUBSCRIBE_EVENT(evntNum,cltToken,"127.0.0.1",port)


