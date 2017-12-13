//note this implementation is taken from gloox library example:message_example.cpp
//it depends on gloox library
#ifndef __CXMPP_PROXY_H_
#define __CXMPP_PROXY_H_
#include "ADThread.hpp"
#include <deque>
#include <vector>
#include <iostream>
#include <map>
using namespace std;
/* ------------------------------------------------------------------------- */
class CXmppProducer; //subject
class CXmppConsumer //observer
{
public:
	virtual int onXmppMessage(std::string msg,std::string sender,CXmppProducer* pObj)=0;
	virtual ~CXmppConsumer(){};
};
class CXmppProducer
{
	static int IDGenerator;
	CXmppConsumer *pConsumer;//consumer object where notification goes to
	int id;//id number for consumer to distinguish between many producers

protected:
	int onXmppMessage(std::string msg,std::string sender="")
	{
		if(pConsumer!=NULL)		
			return pConsumer->onXmppMessage(msg,sender,this);
		return -1;
	}
public:
	CXmppProducer() {id=IDGenerator++;pConsumer=NULL;}
	virtual ~CXmppProducer(){};
	int attach_callback(CXmppConsumer* c)
	{
		//allow only one Consumer to be attached		
		if(pConsumer==NULL)
		{
			pConsumer=c;
			return id;
		}
		else
			return -1;//some other Consumer has already been attached
	}
	int getID(){return id;}
};
/* ------------------------------------------------------------------------- */
class CXmppProxy : public CXmppProducer,public ADThreadConsumer
{
public:
	CXmppProxy();
	~CXmppProxy();//{}
	int disconnect();
	int connect(char* user,char* password);
	int send_reply(std::string reply,std::string sender="");
	int receive_request(std::string request,std::string sender);
	bool get_connect_sts(){return connected;};
	void SetDebugLog(bool log);
	void send_client_alive_ping();
	const std::string currentDateTime();
	bool getForcedDisconnect(){return DisconnectNow;}
	void setForcedDisconnect(){DisconnectNow=true;}
	//for sending asyc-event to a buddy
	bool SendMessageToBuddy(std::string address, const std::string & body, const std::string & subject);
	int get_buddy_list(std::string &returnval);

	/*virtual void handleEvent( const Event& event );// = 0;
	virtual void onConnect();
	virtual void onDisconnect( ConnectionError e );
	virtual bool onTLSConnect( const CertInfo& info );
	virtual void handleMessage( const Message& msg, MessageSession *  );
	virtual void handleMessageEvent( const JID& from, MessageEventType event );
	virtual void handleChatState( const JID& from, ChatStateType state );
	virtual void handleMessageSession( MessageSession *session );
	virtual void handleLog( LogLevel level, LogArea area, const std::string& message );

	//roster related overrider
	virtual void onResourceBindError( ResourceBindError error );
	virtual void onSessionCreateError( SessionCreateError error );
	virtual void handleItemSubscribed( const JID& jid );
	virtual void handleItemAdded( const JID& jid );
	virtual void handleItemUnsubscribed( const JID& jid );
	virtual void handleItemRemoved( const JID& jid );
	virtual void handleItemUpdated( const JID& jid );
	virtual void handleRoster( const Roster& roster );
	virtual void handleRosterError( const IQ&  );
	virtual void handleRosterPresence( const RosterItem& item, const std::string& resource,
	Presence::PresenceType presence, const std::string&  );
	virtual void handleSelfPresence( const RosterItem& item, const std::string& resource,
		Presence::PresenceType presence, const std::string&  );
	virtual bool handleSubscriptionRequest( const JID& jid, const std::string&  );
	virtual bool handleUnsubscriptionRequest( const JID& jid, const std::string&  );
	virtual void handleNonrosterPresence( const Presence& presence );*/

	std::deque<int> PingPipe;//fifo for processing xmpp ping requests
	ADThread PingThread;
	virtual int monoshot_callback_function(void* pUserData,ADThreadProducer* pObj);//{return 0;};
	virtual int thread_callback_function(void* pUserData,ADThreadProducer* pObj){return 0;};
	
private:
	vector<std::string> BuddyList;//authorized accounts that can contact me
	bool iConnect;//shows status of onConnect/onDisconnect
	bool DisconnectNow;
	bool DebugLog;
	bool failed_authorization;
	bool connected;
	int  HeartBeat;
	/*Client *j;
	struct Session
	{
		gloox::MessageSession* m_session;
		gloox::ChatStateFilter* m_chatStateFilter;
		gloox::MessageEventFilter *m_messageEventFilter;
	};
	typedef std::map<std::string, Session> Sessions;
	Sessions mySessions;*/
};

#endif
