#include "EventHandler.h"
#include "ADJsonRpcClient.hpp"
#include "ADCmnPortList.h"
#define EVENT_GPIOSRV ADCMN_PORT_GPIOCTL //40003
/* ------------------------------------------------------------------------- */
EventHandler:: EventHandler(std::string rpcName,int myIndex,bool emu,bool log,SYSMGR_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
	gpioEventActive=false;
	srvToken=-1;
	pDataCache=pData;
	SUBSCRIBE_EVENT("127.0.0.1",EVENT_GPIOSRV,&srvToken,EVENT_GPIOSRV,-1,40001);
	if(srvToken!=-1)
		gpioEventActive=true;//subscription is active
//	std::cout<<"srvToken = "<<srvToken<<endl;
	//second arg:40003 : port number of gpio-server
	//thirdarg:srvToken: on success, gpio-server returns a unique token ID to sysmgr
	//fourtharg: 40003 : sysmgr is giving out a unique ID, where gpiosrv will return this ID on event so that sysmgr knows source of event
	//fiftharg: -1     : sysmgr is requested to receive all events from gpio-srv
	//sixth:40001      : sysmgr is telling its port number where gpio-srv will send events
}
/* ------------------------------------------------------------------------- */
EventHandler::~ EventHandler()
{
	if(gpioEventActive==true)//unsubscribe only if subscription is active
		UNSUBSCRIBE_EVENT("127.0.0.1",EVENT_GPIOSRV,srvToken);
}
/* ------------------------------------------------------------------------- */
void EventHandler::ReceiveEvent(int cltToken,int evntNum,int evntArg)
{
//	std::cout << "EventHandler::ReceiveEvent: clt_token = " <<cltToken<<" evnt_num = "<<evntNum<<" evnt_arg = "<<evntArg <<endl;
	if(cltToken==EVENT_GPIOSRV && evntNum==ADLIB_EVENT_NUM_SHUT_DOWN)
		gpioEventActive=false;//gpio-server is dead, subscription is not active any more

}
/* ------------------------------------------------------------------------- */
//SUBSCRIBE_EVENT(evntNum,cltToken,"127.0.0.1",port)


