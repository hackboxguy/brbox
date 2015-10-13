#ifndef __ADEVNT_MGR_H_
#define __ADEVNT_MGR_H_
#include "ADThread.hpp"
#include <vector>
#include <deque>
#include <iostream>
using namespace std;
#define RPCMGR_RPC_EVENT_SUBSCRIBE    "subscribe_event"
#define RPCMGR_RPC_EVENT_UNSUBSCRIBE  "unsubscribe_event"
#define RPCMGR_RPC_EVENT_NOTIFY       "notify_event"  //for self calling within server
#define RPCMGR_RPC_EVENT_PROCESS      "process_event" //notification from other services

#define RPCMGR_RPC_EVENT_ARG_CLTTOK   "cltToken" //sent from clt to srv, but srv will return this with events
#define RPCMGR_RPC_EVENT_ARG_PORT     "port"
#define RPCMGR_RPC_EVENT_ARG_EVENTNUM "evntNum"  //which of the events
#define RPCMGR_RPC_EVENT_ARG_SRVTOK   "srvToken" //sent from server to client as a subscription token
#define RPCMGR_RPC_EVENT_ARG_EXTRA    "evntArg"  //optional extra argument sent with eventNum

typedef struct EventEntry_t
{
	int cltToken; //event receiver wants to know who sent the event
	int portNum;  //port number where events to be sent.
	int eventNum; //if event-num is -1=then, notify all events
	int srvToken; //subscription token(every subscriber will get a token-number)
	int sock_descr;//socket descriptor(incase of event to be notified on same connection)
	int sock_id;   //unique client connection ident(needed in case of client connection breaks)
	char ip[512];
	bool deleteFlag;//during iteration, if socket connection is not possible, then delete the entry based on this flag
}EventEntry;
//common set of events
//1) 0 - Last inProgress command completed(send ident?)
//2) 1 - ready notification?? to indicate service has restarted
//3) 2 - shutting down event to to subscriber?
// 
struct EventProcEntry
{
	int eventNum;  //what is the event number
	int eventArg;  //used as storing the eventArg
	int cltToken;  //who sent the event
public:
	EventProcEntry(int event_num,int event_arg,int clt_token) :eventNum(event_num),eventArg(event_arg),cltToken(clt_token){}
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
//following functor object is used as predicator for vector to remove entry based on deleteFlag 
class RemoveNonListenerEntry
{
	//const int srv_token;
public:
	//RemoveEventEntry(const int token) :srv_token(token){}
	bool operator()(EventEntry* pEntry) 
	{
		if( pEntry->deleteFlag == true)
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

//following functor object is used as predicator for finding an existing vector element(duplicate)
class FindDuplicateEntry
{
	EventEntry* myEntry;
public:
	FindDuplicateEntry(EventEntry* entry) :myEntry(entry){}
	bool operator()(EventEntry* pEntry) const
	{
		//check if there is an exact match with three items(cltToken/Port/evntNum)
		if(myEntry->cltToken != pEntry->cltToken)
			return false;
		if(myEntry->portNum != pEntry->portNum)
			return false;
		if(myEntry->eventNum != pEntry->eventNum)
			return false;
		return true;//all are matching, duplicate found!!!!
	}
};


class ADEvntMgr : public ADThreadConsumer
{
//private:
	int AckToken;
	int notifyThreadID;
	int processThreadID;

	std::vector<EventEntry*> eventList;
	//std::deque<int> notifyEvent;//fifo for eventNotify worker thread
	std::deque<EventProcEntry> notifyEvent;//fifo for eventNotify worker thread
	std::deque<EventProcEntry> processEvent;//fifo for eventProcess worker thread

	ADThread EventNotifyThread;//thread for sending the events to subscribers
	ADThread EventProcessThread;//thread for processing the events from other services

	//thread-callback functions
	virtual int monoshot_callback_function(void* pUserData,ADThreadProducer* pObj);
	virtual int thread_callback_function(void* pUserData,ADThreadProducer* pObj){return 0;};//we are not using this one..

	int send_event(EventEntry *pEvent,int event_num,int event_arg=-1);//event arg is optional, it could be ident of the inprogress rpc

public:
	ADEvntMgr();
	~ADEvntMgr();
	int register_event_subscription(EventEntry *pEvent,int *ack_token);
	int unregister_event_subscription(int srv_token);
	int notify_event(int eventNum,int eventArg);
	int process_event(int event_num,int event_arg,int clt_token);
};
#endif

//eventing todo;
//Done:3)ADEvntMgr.cpp: check if already there is an existing same subscription to avoid duplication.
//1)client-cmd: evntSubscribe, shall support third arg for registering specific event(add library-function for event subscription).
//2)client-cmd: evntSubscribe, shall print srvToken along with result.
//4)reply on same connection
//5)subscribe/unsubscribe/notification shall handle listContainer in a multi-thread safe way.
//6)in processEvent: add callback to the attached objects
//7)add a macro for sending events from within the rpc code
