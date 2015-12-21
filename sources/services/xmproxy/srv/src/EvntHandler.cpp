#include "EvntHandler.h"
#include "ADJsonRpcClient.hpp"
#include "ADCmnPortList.h"
#include "XmppMgr.h"
#define EVENT_SYSMGR ADCMN_PORT_SYSMGR  //40001
#define EVENT_BBXSMS ADCMN_PORT_BBOXSMS 
/* ------------------------------------------------------------------------- */
EvntHandler:: EvntHandler(std::string rpcName,int myIndex,bool emu,bool log,XMPROXY_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
	pDataCache=pData;

	sysmgrEventActive=false;
	sysMgrSrvToken=-1;
	SUBSCRIBE_EVENT("127.0.0.1",EVENT_SYSMGR,&sysMgrSrvToken,EVENT_SYSMGR,-1,XMPROXY_JSON_PORT_NUMBER);
	if(sysMgrSrvToken!=-1)
		sysmgrEventActive=true;//subscription is active
//	std::cout<<"sysMgrSrvToken = "<<sysMgrSrvToken<<endl;
	//second arg:40001 : port number of sysmgr
	//thirdarg:sysMgrSrvToken: on success, sysmgr returns a unique token ID to xmproxy
	//fourtharg: 40001 : xmproxy is giving out a unique ID, where sysmgr will return this ID on event so that xmproxy knows source of event
	//fiftharg: -1     : xmproxy is requested to receive all events from gpio-srv
	//sixth:4000x      : xmproxy is telling its port number where sysmgr will send events

	bboxSmsEventActive=false;
	bboxSmsSrvToken=-1;
	SUBSCRIBE_EVENT("127.0.0.1",EVENT_BBXSMS,&bboxSmsSrvToken,EVENT_BBXSMS,-1,XMPROXY_JSON_PORT_NUMBER);
	if(bboxSmsSrvToken!=-1)
		bboxSmsEventActive=true;//subscription is active
//	std::cout<<"bboxSmsSrvToken = "<<bboxSmsSrvToken<<endl;

}
/* ------------------------------------------------------------------------- */
EvntHandler::~ EvntHandler()
{
	if(sysmgrEventActive==true)//unsubscribe only if subscription is active
		UNSUBSCRIBE_EVENT("127.0.0.1",EVENT_SYSMGR,sysMgrSrvToken);
	if(bboxSmsEventActive==true)//unsubscribe only if subscription is active
		UNSUBSCRIBE_EVENT("127.0.0.1",EVENT_BBXSMS,bboxSmsSrvToken);
}
/* ------------------------------------------------------------------------- */
void EvntHandler::ReceiveEvent(int cltToken,int evntNum,int evntArg)
{
//	std::cout << "EvntHandler::ReceiveEvent: clt_token = " <<cltToken<<" evnt_num = "<<evntNum<<" evnt_arg = "<<evntArg <<endl;
	if(cltToken==EVENT_SYSMGR && evntNum==ADLIB_EVENT_NUM_SHUT_DOWN)
		sysmgrEventActive=false;//sysmgr is dead, subscription is not active any more
	if(cltToken==EVENT_BBXSMS && evntNum==ADLIB_EVENT_NUM_SHUT_DOWN)
		bboxSmsEventActive=false;//bboxsms-srv is dead, subscription is not active any more

	if(evntNum==ADLIB_EVENT_NUM_INPROG_DONE)
	{
		char taskIDString[255];char taskIDResult[255];taskIDResult[254]='\0';
		sprintf(taskIDString,"%d",evntArg);
		XmppMgr *pXmpp=(XmppMgr*)pDataCache->pXmpMgr;
		if(pXmpp->IsItMyAsyncTaskResp(evntArg,cltToken)==RPC_SRV_RESULT_SUCCESS)
		{
			ADJsonRpcClient Client;
			if(Client.rpc_server_connect("127.0.0.1",cltToken)!=0)
				return;// RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
			RPC_SRV_RESULT result=Client.get_string_type_with_string_para((char*)ADLIB_RPC_NAME_GET_TASK_STATUS,
						(char*)ADLIB_RPC_PARM_TASK_STS_ID,taskIDString,taskIDResult,(char*)ADLIB_RPC_PARM_TASK_STS);
			Client.rpc_server_disconnect();
			std::string finalRes=taskIDResult;
			pXmpp->RpcResponseCallback(finalRes,evntArg);
		}
	}
}
/* ------------------------------------------------------------------------- */


