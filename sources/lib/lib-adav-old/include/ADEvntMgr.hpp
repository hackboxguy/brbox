#ifndef __ADEVNT_MGR_H_
#define __ADEVNT_MGR_H_
#include "ADThread.hpp"
#include <vector>
#include <deque>
#include <iostream>
using namespace std;

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

struct EventProcEntry
{
	int cltToken;  //who sent the event
	int eventNum;  //what is the event number
public:
	EventProcEntry(int clt_tok,int event_num) :cltToken(clt_tok),eventNum(event_num){}
};


//following is just a functor object used as predicator for vector to print values of entry
class PrintEventEntries
{
public:
	void operator()(EventEntry* Entry) const 
	{
	cout << "cltToken="<<Entry->cltToken<<" portNum="<<Entry->portNum<<" eventNum="<<Entry->eventNum<<" srvToken="<<Entry->srvToken;
	cout << " sock_descr="<<Entry->sock_descr<<" sock_id="<<Entry->sock_id<<" ip="<<Entry->ip<<endl;
	}
};
//following functor object is used as predicator for vector to remove entry based on srv_token
class RemoveEventEntry
{
	const int srv_token;
public:
	RemoveEventEntry(const int token) :srv_token(token){}
	bool operator()(EventEntry* pEntry) 
	{
		if(srv_token == pEntry->srvToken)
		{
			delete pEntry; //delete the object and set it to null
			pEntry = NULL;
			return true;
		}
		return false;
	}
};
//following functor object is used as predicator for finding a specific vector element entry based on srvToken
class FindEventEntry
{
	const int srv_token;
public:
	FindEventEntry(const int token) :srv_token(token){}
	bool operator()(EventEntry* pEntry) const
	{
		if(srv_token == pEntry->srvToken)
			return true;
		else
			return false;
	}
};

class ADEvntMgr : public ADThreadConsumer
{
//private:
	int AckToken;
	int notifyThreadID;
	int processThreadID;

	std::vector<EventEntry*> eventList;
	std::deque<int> notifyEvent;//fifo for eventNotify worker thread
	std::deque<EventProcEntry> processEvent;//fifo for eventNotify worker thread

	ADThread EventNotifyThread;//thread for sending the events to subscribers
	ADThread EventProcessThread;//thread for processing the events from other services

	//thread-callback functions
	virtual int monoshot_callback_function(void* pUserData,ADThreadProducer* pObj);
	virtual int thread_callback_function(void* pUserData,ADThreadProducer* pObj){return 0;};//we are not using this one..

public:
	ADEvntMgr();
	~ADEvntMgr();
	int register_event_subscription(EventEntry *pEvent,int *ack_token);
	int unregister_event_subscription(int srv_token);
	int notify_event(int event);
	int process_event(int clt_token,int event_num);
};
#endif

