#include "EvntHandler.h"
#include "ADJsonRpcClient.hpp"
#include "ADCmnPortList.h"
#include "XmppMgr.h"
#define EVENT_SYSMGR  ADCMN_PORT_SYSMGR  //40001
#define EVENT_BBXSMS  ADCMN_PORT_BBOXSMS 
#define EVENT_GPIOCTL ADCMN_PORT_GPIOCTL 
/* ------------------------------------------------------------------------- */
EvntHandler:: EvntHandler(std::string rpcName,int myIndex,bool emu,bool log,XMPROXY_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
	pDataCache=pData;

	//subscribe for events from sysmgr-service
	sysmgrEventActive=false;
	sysMgrSrvToken=-1;
	//SUBSCRIBE_EVENT("172.29.10.1",0,&sysMgrSrvToken,0,-1,42513);

	SUBSCRIBE_EVENT("127.0.0.1",EVENT_SYSMGR,&sysMgrSrvToken,EVENT_SYSMGR,-1,XMPROXY_JSON_PORT_NUMBER);
	if(sysMgrSrvToken!=-1)
		sysmgrEventActive=true;//subscription is active
	//std::cout<<"sysMgrSrvToken = "<<sysMgrSrvToken<<endl;
	//second arg:40001 : port number of sysmgr
	//thirdarg:sysMgrSrvToken: on success, sysmgr returns a unique token ID to xmproxy
	//fourtharg: 40001 : xmproxy is giving out a unique ID, where sysmgr will return this ID on event so that xmproxy knows source of event
	//fiftharg: -1     : xmproxy is requested to receive all events from gpio-srv
	//sixth:4000x      : xmproxy is telling its port number where sysmgr will send events

	//subscribe for events from bbox-sms-service
	bboxSmsEventActive=false;
	bboxSmsSrvToken=-1;
	SUBSCRIBE_EVENT("127.0.0.1",EVENT_BBXSMS,&bboxSmsSrvToken,EVENT_BBXSMS,-1,XMPROXY_JSON_PORT_NUMBER);
	if(bboxSmsSrvToken!=-1)
		bboxSmsEventActive=true;//subscription is active

	//subscribe for events from gpio-service
	gpioEventActive=false;
	gpioSrvToken=-1;
	SUBSCRIBE_EVENT("127.0.0.1",EVENT_GPIOCTL,&gpioSrvToken,EVENT_GPIOCTL,-1,XMPROXY_JSON_PORT_NUMBER);
	if(gpioSrvToken!=-1)
		gpioEventActive=true;//subscription is active

}
/* ------------------------------------------------------------------------- */
EvntHandler::~ EvntHandler()
{
	if(sysmgrEventActive==true)//unsubscribe only if subscription is active
		UNSUBSCRIBE_EVENT("127.0.0.1",EVENT_SYSMGR,sysMgrSrvToken);
	if(bboxSmsEventActive==true)//unsubscribe only if subscription is active
		UNSUBSCRIBE_EVENT("127.0.0.1",EVENT_BBXSMS,bboxSmsSrvToken);
	if(gpioEventActive==true)//unsubscribe only if subscription is active
		UNSUBSCRIBE_EVENT("127.0.0.1",EVENT_GPIOCTL,gpioSrvToken);
}
/* ------------------------------------------------------------------------- */
void EvntHandler::ReceiveEvent(int cltToken,int evntNum,int evntArg)
{
//	std::cout << "EvntHandler::ReceiveEvent: clt_token = " <<cltToken<<" evnt_num = "<<evntNum<<" evnt_arg = "<<evntArg <<endl;
	if(cltToken==EVENT_SYSMGR && evntNum==ADLIB_EVENT_NUM_SHUT_DOWN)
		sysmgrEventActive=false;//sysmgr is dead, subscription is not active any more
	if(cltToken==EVENT_BBXSMS && evntNum==ADLIB_EVENT_NUM_SHUT_DOWN)
		bboxSmsEventActive=false;//bboxsms-srv is dead, subscription is not active any more
	if(cltToken==EVENT_GPIOCTL && evntNum==ADLIB_EVENT_NUM_SHUT_DOWN)
		gpioEventActive=false;//gpio-srv is dead, subscription is not active any more


	if(evntNum==ADLIB_EVENT_NUM_INPROG_DONE)
	{
		char taskIDString[255];char taskIDResult[255];taskIDResult[254]='\0';
		sprintf(taskIDString,"%d",evntArg);
		XmppMgr *pXmpp=(XmppMgr*)pDataCache->pXmpMgr;
		//if(pXmpp->IsItMyAsyncTaskResp(evntArg,cltToken)==RPC_SRV_RESULT_SUCCESS)
		int xmpTID=-1;
		std::string to;
		if(pXmpp->AccessAsyncTaskList(evntArg,cltToken,false,&xmpTID,to)==RPC_SRV_RESULT_SUCCESS)
		{
			ADJsonRpcClient Client;
			if(Client.rpc_server_connect("127.0.0.1",cltToken)!=0)
			{
			LOG_DEBUG_MSG_1_ARG(true,"BRBOX:xmproxy","EvntHandler::ReceiveEvent::Conn Err, cltToken=%d",cltToken);
				return ;// RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
			}
			RPC_SRV_RESULT result=Client.get_string_type_with_string_para((char*)ADLIB_RPC_NAME_GET_TASK_STATUS,
						(char*)ADLIB_RPC_PARM_TASK_STS_ID,taskIDString,taskIDResult,(char*)ADLIB_RPC_PARM_TASK_STS);
			Client.rpc_server_disconnect();
			std::string finalRes=taskIDResult;
			pXmpp->RpcResponseCallback(finalRes,xmpTID,to);//evntArg);
		}
		else
		{
			LOG_DEBUG_MSG_2_ARG(true/*get_debug_log_flag()*/,"BRBOX:xmproxy","EvntHandler::ReceiveEvent::Entry not Found!!! evntArg=%d,cltToken=%d",evntArg,cltToken);
		}
	}
	else if(cltToken==EVENT_GPIOCTL && evntNum==ADLIB_EVENT_NUM_END) //TODO:correctly compare evntNum with actual enum of gpio-srv
	{
	//std::cout << "EvntHandler::ReceiveEvent: could be event from gpio-server(TODO) cltToken="<<cltToken<<" evntArg="<<evntArg<<endl;
		XmppMgr *pXmpp=(XmppMgr*)pDataCache->pXmpMgr;
		pXmpp->GpioEventCallback(evntNum,evntArg);
	}
}
/* ------------------------------------------------------------------------- */


