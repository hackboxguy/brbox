//note this implementation is taken from gloox library example:message_example.cpp
//it depends on gloox library
#ifndef __ADXMPP_PROXY_H_
#define __ADXMPP_PROXY_H_

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
                    MessageEventHandler, MessageHandler, ChatStateHandler, public ADXmppProducer
{
public:
	ADXmppProxy();
	~ADXmppProxy(){}
	int disconnect();
	int connect(char* user,char* password);
	int send_reply(std::string reply);
	int receive_request(std::string request);
	bool get_connect_sts(){return connected;};

	virtual void onConnect();
	virtual void onDisconnect( ConnectionError e );
	virtual bool onTLSConnect( const CertInfo& info );
	virtual void handleMessage( const Message& msg, MessageSession * /*session*/ );
	virtual void handleMessageEvent( const JID& from, MessageEventType event );
	virtual void handleChatState( const JID& from, ChatStateType state );
	virtual void handleMessageSession( MessageSession *session );
	virtual void handleLog( LogLevel level, LogArea area, const std::string& message );
private:
	bool failed_authorization;
	bool connected;
	Client *j;
	MessageSession *m_session;
	MessageEventFilter *m_messageEventFilter;
	ChatStateFilter *m_chatStateFilter;
};
#endif
