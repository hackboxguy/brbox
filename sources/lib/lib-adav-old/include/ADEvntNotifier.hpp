#ifndef __ADEVNT_NOTIFIER_H_
#define __ADEVNT_NOTIFIER_H_
#include "ADThread.hpp"
#include <vector>
#include <deque>
#include <iostream>
using namespace std;
struct EvntNotifyEntry
{
	int eventNum;  //what is the event number
	int eventArg;  //used as storing the eventArg
	int port;      //on which port to send the event(typically self port of a server)
	int eventArg2; //used as storing second eventArg
public:
	EvntNotifyEntry(int event_num,int event_arg,int self_port,int event_arg2) :eventNum(event_num),eventArg(event_arg),port(self_port),eventArg2(event_arg2){}
};
/*****************************************************************************/
class ADEvntNotifier : public ADThreadConsumer
{
	std::deque<EvntNotifyEntry> NotifierList;//fifo for eventProcess worker thread
	ADThread NotifierThread;//thread for calling self-rpc
	//thread-callback functions
	virtual int monoshot_callback_function(void* pUserData,ADThreadProducer* pObj);
	virtual int thread_callback_function(void* pUserData,ADThreadProducer* pObj){return 0;};//we are not using this one..
public:
	ADEvntNotifier();
	~ADEvntNotifier();
	int NotifyEvent(int event,int eventArg,int port,int eventArg2);
};
#endif

