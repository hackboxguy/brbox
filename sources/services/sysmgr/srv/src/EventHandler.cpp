#include "EventHandler.h"
#include "ADJsonRpcClient.hpp"
/* ------------------------------------------------------------------------- */
EventHandler:: EventHandler(std::string rpcName,int myIndex,bool emu,bool log,SYSMGR_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
	srvToken=-1;
	pDataCache=pData;
	SUBSCRIBE_EVENT("127.0.0.1",40002,&srvToken,40002,-1,40001);
	std::cout<<"srvToken = "<<srvToken<<endl;
}
/* ------------------------------------------------------------------------- */
EventHandler::~ EventHandler()
{
	UNSUBSCRIBE_EVENT("127.0.0.1",40002,srvToken);
}
/* ------------------------------------------------------------------------- */
void EventHandler::ReceiveEvent(int cltToken,int evntNum,int evntArg)
{
	std::cout << "EventHandler::ReceiveEvent: clt_token = " <<cltToken<<" evnt_num = "<<evntNum<<" evnt_arg = "<<evntArg <<endl;
}
/* ------------------------------------------------------------------------- */
//SUBSCRIBE_EVENT(evntNum,cltToken,"127.0.0.1",port)


