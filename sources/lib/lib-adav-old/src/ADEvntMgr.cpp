#include "ADEvntMgr.hpp"
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
		//cout<<"ADEvntMgr::monoshot_callback_function called"<<endl;
		//usleep(100000);usleep(100000);usleep(100000);usleep(100000);usleep(100000);
		while (!notifyEvent.empty())
		{
			//if pEvent->eventNum==-1, then ignore event value and just notify
			//if pEvent->eventNum>=0,  then check event value, if (pEvent->eventNum==event) then notify
			//when notifying, check if connection is possible, if not, then remove the EventSubscription entry from my list
			std::cout << "event_val = " << notifyEvent.front()<<endl;
			notifyEvent.pop_front();
		}
	}
	else//must be for processThreadID
	{
		while (!processEvent.empty())
		{
			EventProcEntry entry = processEvent.front();
			//TODO://callback of the subscriber.
			std::cout << "clt_token = " <<entry.cltToken <<" evnt_num = "<<entry.eventNum <<endl;
			processEvent.pop_front();
		}
	}
	return 0;
}
int ADEvntMgr::register_event_subscription(EventEntry *pEvent,int *ack_token)
{
	//TODO:check if mutex-locking is needed
	pEvent->srvToken=*ack_token=++AckToken;
	eventList.push_back(pEvent);

	//Note: following lines are just for testing the container elements.
	//after event subscription, print the whole event vector list(notice the use of functor object)
	PrintEventEntries PrintEvent;
	for_each (eventList.begin(), eventList.end(),PrintEvent);
	return 0;
}
int ADEvntMgr::unregister_event_subscription(int srv_token)
{
	//bool found = any_of(eventList.begin(), eventList.end(), FindEventEntry(srv_token)); //c++11
	//if(found == false)
	//	return 1;//1 indicates that item is not found in our eventEntryList having this srv_token
	if (find_if(eventList.begin(), eventList.end(), FindEventEntry(srv_token)) == eventList.end())
		return 1;//if item with srv_token is not found, then return 1 to indicate that itemNotFound

	//TODO: removed the element with srv_token from vector and delete it.(be careful, anyone can delete event notification)
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
//following function is called by server's own internal code via rpc, notifyThread will then notify other as per subscription entry
int ADEvntMgr::notify_event(int event)//enumerated event-list(check the service header).
{
	notifyEvent.push_back(event);
	EventNotifyThread.wakeup_thread();//tell the worker to start working
	//after this context is de-coupled and later monoshot_callback_function will be called
	return 0;
}

int ADEvntMgr::process_event(int clt_token,int event_num)
{
	processEvent.push_back(EventProcEntry(clt_token,event_num));
	EventProcessThread.wakeup_thread();//tell the worker to start working
	//after this context is de-coupled and later monoshot_callback_function will be called
	return 0;
}

