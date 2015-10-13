#include "ADEvntMgr.hpp"
#include "ADJsonRpcClient.hpp"

#include <algorithm>
//#include <functional>
//using namespace std;

ADEvntMgr::ADEvntMgr() :AckToken(0)
{
	notifyThreadID=EventNotifyThread.subscribe_thread_callback(this);
	EventNotifyThread.set_thread_properties(THREAD_TYPE_MONOSHOT,(void *)this);
	EventNotifyThread.start_thread();

	processThreadID=EventProcessThread.subscribe_thread_callback(this);
	EventProcessThread.set_thread_properties(THREAD_TYPE_MONOSHOT,(void *)this);
	EventProcessThread.start_thread();
}
ADEvntMgr::~ADEvntMgr()
{
	EventProcessThread.stop_thread();
	EventNotifyThread.stop_thread();
	for(int i = 0; i < eventList.size(); ++i)
		delete eventList[i];//in distructor, free all elements from heap
}
//
int ADEvntMgr::monoshot_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	int call_from=pObj->getID();
	if(call_from == notifyThreadID)
	{
		while (!notifyEvent.empty())
		{
			EventProcEntry entry = notifyEvent.front();
			//when notifying, check if connection is possible, if not, then remove the EventSubscription entry from my list
			//everytime after subscribing, if the subscriber goes aways(no listining on port), then notify entry will be removed
			//in such case, subscriber has to subscribe again after restart
			for(int i = 0; i < eventList.size(); ++i)
			{
				if(eventList[i]->eventNum==-1)//notify all events
				{
					if (send_event(eventList[i],entry.eventNum,entry.eventArg) == -1)
						eventList[i]->deleteFlag=true;//after this loop, delete the entry(subscriber is not listening)
				}				
				else if(eventList[i]->eventNum == entry.eventNum)//notify specific event number
				{
					if(send_event(eventList[i],entry.eventNum,entry.eventArg) == -1)
						eventList[i]->deleteFlag=true;//after this loop, delete the entry(subscriber is not listening)

				}
			}
			//TODO: remove the entries in a thread safe way(coz subscribe or unscribe rpc may clash with this chain)
			RemoveNonListenerEntry Remover;
			eventList.erase(remove_if(eventList.begin(), eventList.end(), Remover) , eventList.end());
			notifyEvent.pop_front();
		}
	}
	else//must be for processThreadID
	{
		while (!processEvent.empty())
		{
			EventProcEntry entry = processEvent.front();
			//TODO://callback of the subscriber.
	std::cout << "process_event: clt_token = " <<entry.cltToken<<" evnt_num = "<<entry.eventNum<<" evnt_arg = "<<entry.eventArg <<endl;
			processEvent.pop_front();
		}
	}
	return 0;
}
int ADEvntMgr::register_event_subscription(EventEntry *pEvent,int *ack_token)
{
	//TODO:check if mutex-locking is needed
	//check if there is already an entry with same cltToken/Port/evntNum, if exists, then return fail to avoid duplicate entries
	if (find_if(eventList.begin(), eventList.end(), FindDuplicateEntry(pEvent)) == eventList.end())
	{
		pEvent->srvToken=*ack_token=++AckToken;
		eventList.push_back(pEvent);
		//Note: following lines are just for testing the container elements.
		//after event subscription, print the whole event vector list(notice the use of functor object)
		PrintEventEntries PrintEvent;
		for_each (eventList.begin(), eventList.end(),PrintEvent);
		cout<<"##############################################"<<endl;
		return 0;
	}
	else
		return -1;
}
int ADEvntMgr::unregister_event_subscription(int srv_token)
{
	//bool found = any_of(eventList.begin(), eventList.end(), FindEventEntry(srv_token)); //c++11
	//if(found == false)
	//	return 1;//1 indicates that item is not found in our eventEntryList having this srv_token
	if (find_if(eventList.begin(), eventList.end(), FindEventEntry(srv_token)) == eventList.end())
		return -1;//if item with srv_token is not found, then return -1 to indicate that itemNotFound

	//removed the element with srv_token from vector and delete it.(be careful, anyone can delete event notification)
	eventList.erase(remove_if(eventList.begin(), eventList.end(), RemoveEventEntry(srv_token)) , eventList.end());

	//following solution works when only entry needs to be deleted, not the entry-pointer
	//http://stackoverflow.com/questions/991335/how-to-erase-delete-pointers-to-objects-stored-in-a-vector
	//using functor object, delete the entry having srv_token
	//for_each(eventList.begin(), eventList.end(), RemoveEventEntry(srv_token));
	//scan all the entries which have NULL pointer, and remove that pointer
	//vector<EventEntry*>::iterator new_end = remove(eventList.begin(), eventList.end(), static_cast<EventEntry*>(NULL));
	//eventList.erase(new_end, eventList.end());
	return 0;
}
//sequence-1:following function is called by server's own internal code via rpc, notifyThread will then notify other as per subscription entry
int ADEvntMgr::notify_event(int eventNum,int eventArg)//enumerated event-list(check the service header).
{
	//notifyEvent.push_back(event);
	notifyEvent.push_back(EventProcEntry(eventNum,eventArg,0));
	EventNotifyThread.wakeup_thread();//tell the worker to start working
	//after this context is de-coupled and later monoshot_callback_function will be called
	return 0;
}
//sequence-2:thread wakes-up and according subscription list calls following functions defined by sequence-3
//sequence-3:following function sends events to other services in the context of sender service
int ADEvntMgr::send_event(EventEntry *pEvent,int event_num,int event_arg)
{
	//TODO: in case of port=-1, send the response on existing socket connection
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(pEvent->ip,pEvent->portNum)!=0)
		return -1;//RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	if(Client.set_three_int_type((char*)RPCMGR_RPC_EVENT_PROCESS, //set_integer_type_with_addr_para
				     (char*)RPCMGR_RPC_EVENT_ARG_CLTTOK,pEvent->cltToken,
				     (char*)RPCMGR_RPC_EVENT_ARG_EVENTNUM,event_num,
				     (char*)RPCMGR_RPC_EVENT_ARG_EXTRA,event_arg)!=RPC_SRV_RESULT_SUCCESS)

		;//TODO: unable to send event:TODO://send eventARG param
	Client.rpc_server_disconnect();
	return 0;
}
//sequence-4:following function receives cltToken,evntNum,evntArg in the context of receiver service
int ADEvntMgr::process_event(int event_num,int event_arg,int clt_token)
{
	processEvent.push_back(EventProcEntry(event_num,event_arg,clt_token));
	EventProcessThread.wakeup_thread();//tell the worker to start working
	//after this context is de-coupled and later monoshot_callback_function will be called
	return 0;
}
//sequence-5:a thread wakes-up and does the call-back to attached object with cltToken,evntNum,evntArg

