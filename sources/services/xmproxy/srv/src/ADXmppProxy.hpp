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
#include "ADThread.hpp"
#include <deque>

using namespace gloox;
/* ------------------------------------------------------------------------- */
class ADXmppProducer; //subject
class ADXmppConsumer //observer
{
public:
	virtual int onXmppMessage(std::string msg,ADXmppProducer* pObj)=0;
	virtual ~ADXmppConsumer(){};
};
class ADXmppProducer
{
	static int IDGenerator;
	ADXmppConsumer *pConsumer;//consumer object where notification goes to
	int id;//id number for consumer to distinguish between many producers

protected:
	int onXmppMessage(std::string msg)
	{
		if(pConsumer!=NULL)		
			return pConsumer->onXmppMessage(msg,this);
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
                    MessageEventHandler, MessageHandler, ChatStateHandler,EventHandler, public ADXmppProducer,public ADThreadConsumer
{
public:
	ADXmppProxy();
	~ADXmppProxy();//{}
	int disconnect();
	int connect(char* user,char* password);
	int send_reply(std::string reply);
	int receive_request(std::string request);
	bool get_connect_sts(){return connected;};
	void SetDebugLog(bool log);
	void send_client_alive_ping();
	const std::string currentDateTime();
	bool getForcedDisconnect(){return DisconnectNow;}
	void setForcedDisconnect(){DisconnectNow=true;}

	virtual void handleEvent( const Event& event );// = 0;
	virtual void onConnect();
	virtual void onDisconnect( ConnectionError e );
	virtual bool onTLSConnect( const CertInfo& info );
	virtual void handleMessage( const Message& msg, MessageSession * /*session*/ );
	virtual void handleMessageEvent( const JID& from, MessageEventType event );
	virtual void handleChatState( const JID& from, ChatStateType state );
	virtual void handleMessageSession( MessageSession *session );
	virtual void handleLog( LogLevel level, LogArea area, const std::string& message );

	std::deque<int> PingPipe;//fifo for processing xmpp ping requests
	ADThread PingThread;
	virtual int monoshot_callback_function(void* pUserData,ADThreadProducer* pObj);//{return 0;};
	virtual int thread_callback_function(void* pUserData,ADThreadProducer* pObj){return 0;};

private:
	bool DisconnectNow;
	bool DebugLog;
	bool failed_authorization;
	bool connected;
	int  HeartBeat;
	//JID myJid;
	Client *j;
	MessageSession *m_session;
	MessageEventFilter *m_messageEventFilter;
	ChatStateFilter *m_chatStateFilter;
};
#endif
