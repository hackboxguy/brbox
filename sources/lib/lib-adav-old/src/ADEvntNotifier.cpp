#include "ADEvntNotifier.hpp"
#include "ADJsonRpcClient.hpp"
//#include <algorithm>

ADEvntNotifier::ADEvntNotifier()
{
	NotifierThread.subscribe_thread_callback(this);
	NotifierThread.set_thread_properties(THREAD_TYPE_MONOSHOT,(void *)this);
	NotifierThread.start_thread();
}
ADEvntNotifier::~ADEvntNotifier()
{
	NotifierThread.stop_thread();
}
int ADEvntNotifier::monoshot_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	while (!NotifierList.empty())
	{
		EvntNotifyEntry entry = NotifierList.front();
		ADJsonRpcClient Client;
		if(Client.rpc_server_connect("127.0.0.1",entry.port)!=0)
		{
			//LOG_DEBUG_MSG_1_ARG(true,"BRBOX:ADEvntNotifier","Unable connect on port = %d",entry.port);
			LOG_ERR_MSG_WITH_ARG("libadav:ADEvntNotifier","Unable connect on port = %d",entry.port);
		}
		else
		{
			if(Client.set_three_int_type((char*)RPCMGR_RPC_EVENT_NOTIFY,
						     (char*)RPCMGR_RPC_EVENT_ARG_EVENTNUM,entry.eventNum,
						     (char*)RPCMGR_RPC_EVENT_ARG_EXTRA,entry.eventArg,
						     (char*)RPCMGR_RPC_EVENT_ARG2_EXTRA,entry.eventArg2)!=RPC_SRV_RESULT_SUCCESS)
			{
				//LOG_DEBUG_MSG_2_ARG(true,"BRBOX:ADEvntNotifier","Unable send event = %d on port = %d",entry.eventNum,entry.port);
				LOG_ERR_MSG_WITH_ARG("libadav:ADEvntNotifier","Unable send event = %d",entry.eventNum);
				LOG_ERR_MSG_WITH_ARG("libadav:ADEvntNotifier","Unable send event on port = %d",entry.port);
			}
			Client.rpc_server_disconnect();
		}
		NotifierList.pop_front();
	}
	return 0;
}
int ADEvntNotifier::NotifyEvent(int eventNum,int eventArg,int port,int eventArg2)
{
	NotifierList.push_back(EvntNotifyEntry(eventNum,eventArg,port,eventArg2));
	NotifierThread.wakeup_thread();//tell the worker to start working
	//after this context is de-coupled and later monoshot_callback_function will be called
	return 0;
}

