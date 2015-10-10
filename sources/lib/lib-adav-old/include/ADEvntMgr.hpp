#ifndef __ADEVNT_MGR_H_
#define __ADEVNT_MGR_H_
#include "ADThread.hpp"
#include <vector>
typedef struct EventEntry_t
{
	int cltToken; //event receiver wants to know who sent the event
	int portNum;  //port number where events to be sent.
	int eventNum; //if event-num is -1=then, notify all events
	int srvToken; //subscription token(every subscriber will get a token-number)
	int sock_descr;//socket descriptor(incase of event to be notified on same connection)
	int sock_id;   //unique client connection ident(needed in case of client connection breaks)
	char ip[512];
}EventEntry;

class ADEvntMgr : public ADThreadConsumer
{
//private:
	int AckToken;
	std::vector<EventEntry*> eventList;
	//thread-callback functions
	virtual int monoshot_callback_function(void* pUserData,ADThreadProducer* pObj){return 0;};
	virtual int thread_callback_function(void* pUserData,ADThreadProducer* pObj){return 0;};//we are not using this one..

public:
	ADEvntMgr();
	~ADEvntMgr();
	int register_event_subscription(EventEntry *pEvent,int *ack_token);
	int unregister_event_subscription(int srv_token);
	int notify_event(int event);
};
#endif

