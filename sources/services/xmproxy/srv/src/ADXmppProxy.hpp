//note this implementation is taken from gloox library example:message_example.cpp
//it depends on gloox library
#ifndef __ADXMPP_PROXY_H_
#define __ADXMPP_PROXY_H_

#define CLIENT_TEST
#define CLIENTBASE_TEST

#include <gloox/client.h>
#include <gloox/messagesessionhandler.h>
#include <gloox/messageeventhandler.h>
#include <gloox/messageeventfilter.h>
#include <gloox/chatstatehandler.h>
#include <gloox/chatstatefilter.h>
#include <gloox/connectionlistener.h>
#include <gloox/disco.h>
#include <gloox/message.h>
#include <gloox/gloox.h>
#include <gloox/lastactivity.h>
#include <gloox/loghandler.h>
#include <gloox/logsink.h>
#include <gloox/connectiontcpclient.h>
#include <gloox/connectionsocks5proxy.h>
#include <gloox/connectionhttpproxy.h>
#include <gloox/messagehandler.h>
#include <gloox/eventhandler.h>
#include <gloox/rostermanager.h>
#include <gloox/connectionbosh.h>
#include <gloox/connectionhttpproxy.h>
#include "ADThread.hpp"
#include <deque>
#include <vector>
#include <iostream>
using namespace std;
using namespace gloox;
/* ------------------------------------------------------------------------- */
class ADXmppProducer; //subject
class ADXmppConsumer //observer
{
public:
	virtual int onXmppMessage(std::string msg,std::string sender,ADXmppProducer* pObj)=0;
	virtual ~ADXmppConsumer(){};
};
class ADXmppProducer
{
	static int IDGenerator;
	ADXmppConsumer *pConsumer;//consumer object where notification goes to
	int id;//id number for consumer to distinguish between many producers

protected:
	int onXmppMessage(std::string msg,std::string sender="")
	{
		if(pConsumer!=NULL)
			return pConsumer->onXmppMessage(msg,sender,this);
		return -1;
	}
public:
	ADXmppProducer() {id=IDGenerator++;pConsumer=NULL;}
	virtual ~ADXmppProducer(){};
	int attach_callback(ADXmppConsumer* c)
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
class ADXmppProxy : public MessageSessionHandler, ConnectionListener, LogHandler,
                    MessageEventHandler, MessageHandler, ChatStateHandler,EventHandler,RosterListener, public ADXmppProducer,public ADThreadConsumer
{
public:
	ADXmppProxy();
	~ADXmppProxy();//{}
	int disconnect();
	int connect(char* user,char* password,std::string adminbuddy="");
	int send_reply(std::string reply,std::string sender="");
	int receive_request(std::string request,std::string sender);
	bool get_connect_sts(){return connected;};
	void SetDebugLog(bool log);
	void send_client_alive_ping();
	const std::string currentDateTime();
	bool getForcedDisconnect(){return DisconnectNow;}
	void setForcedDisconnect(){DisconnectNow=true;}
	bool getOnDemandDisconnect(){return OnDemandDisconnect;}
	void setOnDemandDisconnect(bool flag){OnDemandDisconnect=flag;}

	//for sending asyc-event to a buddy
	bool SendMessageToBuddy(std::string address, const std::string & body, const std::string & subject);
	int get_buddy_list(std::string &returnval);
	bool get_connected_status();

	virtual void handleEvent( const Event& event );// = 0;
	virtual void onConnect();
	virtual void onDisconnect( ConnectionError e );
	virtual bool onTLSConnect( const CertInfo& info );
	virtual void handleMessage( const Message& msg, MessageSession * /*session*/ );
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
	virtual void handleRosterError( const IQ& /*iq*/ );
	virtual void handleRosterPresence( const RosterItem& item, const std::string& resource,
	Presence::PresenceType presence, const std::string& /*msg*/ );
	virtual void handleSelfPresence( const RosterItem& item, const std::string& resource,
		Presence::PresenceType presence, const std::string& /*msg*/ );
	virtual bool handleSubscriptionRequest( const JID& jid, const std::string& /*msg*/ );
	virtual bool handleUnsubscriptionRequest( const JID& jid, const std::string& /*msg*/ );
	virtual void handleNonrosterPresence( const Presence& presence );

	std::deque<int> PingPipe;//fifo for processing xmpp ping requests
	ADThread PingThread;
	virtual int monoshot_callback_function(void* pUserData,ADThreadProducer* pObj);//{return 0;};
	virtual int thread_callback_function(void* pUserData,ADThreadProducer* pObj){return 0;};
	std::string convert_presence_enum_to_str(Presence::PresenceType presence);
private:
	vector<std::string> BuddyList;//authorized accounts that can contact me
	bool iConnect;//shows status of onConnect/onDisconnect
	bool DisconnectNow;
	bool OnDemandDisconnect;
	bool DebugLog;
	bool failed_authorization;
	bool connected;
	int  HeartBeat;
	//JID myJid;
	Client *j;
	//MessageSession *m_session;
	//MessageEventFilter *m_messageEventFilter;
	//ChatStateFilter *m_chatStateFilter;
	//sessions container
	struct Session
	{
		gloox::MessageSession* m_session;
		gloox::ChatStateFilter* m_chatStateFilter;
		gloox::MessageEventFilter *m_messageEventFilter;
	};
	typedef std::map<std::string, Session> Sessions;
	Sessions mySessions;
	std::string AdminBuddy;
};
#endif
