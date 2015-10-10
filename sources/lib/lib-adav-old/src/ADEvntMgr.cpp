#include "ADEvntMgr.hpp"
ADEvntMgr::ADEvntMgr() :AckToken(0)
{
}
ADEvntMgr::~ADEvntMgr()
{
	for(int i = 0; i < eventList.size(); ++i)
		delete eventList[i];//in distructor, free all elements from heap
}
int ADEvntMgr::register_event_subscription(EventEntry *pEvent,int *ack_token)
{
	//TODO:check if mutex-locking is needed
	pEvent->srvToken=*ack_token=++AckToken;
	eventList.push_back(pEvent);
	return 0;
}
int ADEvntMgr::unregister_event_subscription(int srv_token)
{
	//TODO: removed the element with srv_token from vector and delete it.
	return 0;
}
//following function to be called by all those worker rpc's when they need to notify subscribers
int ADEvntMgr::notify_event(int event)//enumerated event-list(check the service header).
{
	//TODO:decouple with thread
	//if pEvent->eventNum==-1, then ignore event value and just notify
	//if pEvent->eventNum>=0,  then check event value, if (pEvent->eventNum==event) then notify
	//when notifying, check if connection is possible, if not, then remove the EventSubscription entry from my list
	return 0;
}

