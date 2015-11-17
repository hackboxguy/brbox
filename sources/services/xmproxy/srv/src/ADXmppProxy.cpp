#include "ADXmppProxy.hpp"
#include <iostream>
using namespace std;
#include <stdio.h>
/*****************************************************************************/
int ADXmppProducer::IDGenerator = 0;//generate Unique ID for every ADXmppProxy object
/*****************************************************************************/
ADXmppProxy::ADXmppProxy()
{
	DebugLog=false;
	failed_authorization=false;
	connected=false;
	m_session=0;
	m_messageEventFilter=0;
	m_chatStateFilter=0;
}
/*****************************************************************************/
int ADXmppProxy::disconnect()
{
	if(connected)
		j->disconnect();
	return 0;
}
/*****************************************************************************/
int ADXmppProxy::connect(char* user,char* password)
{
	JID jid( user);
	j = new Client( jid,password);
	j->registerConnectionListener( this );
	j->registerMessageSessionHandler( this, 0 );
	j->disco()->setVersion( "messageTest", GLOOX_VERSION, "Linux" );
	j->disco()->setIdentity( "client", "jsonbot" );
	j->disco()->addFeature( XMLNS_CHAT_STATES );
	//StringList ca;
	//ca.push_back( "./cacert.crt" );
	//j->setCACerts( ca );

	j->logInstance().registerLogHandler( LogLevelDebug, LogAreaAll, this );

	if( j->connect( false ) )
	{
		ConnectionError ce = ConnNoError;
		while( ce == ConnNoError )
		{
			ce = j->recv();
			if(DebugLog)
				cout<<"ADXmppProxy::connect:Message Arrived!!!"<<endl;
		}
		if(DebugLog)
			printf( "ce: %d\n", ce );
	}
	delete( j );
	return 0;
}
/*****************************************************************************/
void ADXmppProxy::SetDebugLog(bool log)
{
	DebugLog=log;
}
/*****************************************************************************/
void ADXmppProxy::onConnect()
{
	if(DebugLog)
		cout<<"ADXmppProxy::onConnect:connected!!!"<<endl;
	connected=true;
}
/*****************************************************************************/
void ADXmppProxy::onDisconnect( ConnectionError e )
{
	if( e == ConnAuthenticationFailed )
	{
		failed_authorization=true;
		if(DebugLog)
			cout<<"ADXmppProxy::onDisconnect:disconnected due to failed authrization"<<endl;
	}
	connected=false;
	if(DebugLog)
		cout<<"ADXmppProxy::onDisconnect:disconnected"<<endl;
}
/*****************************************************************************/
bool ADXmppProxy::onTLSConnect( const CertInfo& info )
{
	return true;
}
/*****************************************************************************/
void ADXmppProxy::handleMessage( const Message& msg, MessageSession * /*session*/ )
{
	receive_request(msg.body());
//	std::string reply("i didnt understand");
//	send_reply(reply);
}
/*****************************************************************************/
int ADXmppProxy::receive_request(std::string req)
{
	//printf("%s\n",req.c_str());
	if(DebugLog)
		cout<<"ADXmppProxy::receive_request:received msg = "<<req<<endl;
	onXmppMessage(req);//callback to the attached msg-processing-object
	return 0;
}
int ADXmppProxy::send_reply(std::string reply)
{
	if(!connected)
		return -1;//if no connection, then dont send data
	std::string sub;
	//send response
	m_messageEventFilter->raiseMessageEvent( MessageEventDisplayed );
	m_messageEventFilter->raiseMessageEvent( MessageEventComposing );
	m_chatStateFilter->setChatState( ChatStateComposing );
	m_session->send( reply, sub );//after reply from json-rpc-server, call send reply
	if(DebugLog)
		cout<<"ADXmppProxy::send_reply:sending msg = "<<reply<<endl;
	return 0;
}
/*****************************************************************************/
void ADXmppProxy::handleMessageEvent( const JID& from, MessageEventType event )
{
	if(DebugLog)
		printf( "received event: %d from: %s\n", event, from.full().c_str() );
}
/*****************************************************************************/
void ADXmppProxy::handleChatState( const JID& from, ChatStateType state)
{
	if(DebugLog)
		printf( "received state: %d from: %s\n", state, from.full().c_str() );
}
/*****************************************************************************/
void ADXmppProxy::handleMessageSession( MessageSession *session )
{
	if(DebugLog)
		printf( "got new session\n");
	// this example can handle only one session. so we get rid of the old session
	j->disposeMessageSession( m_session );
	m_session = session;
	m_session->registerMessageHandler( this );
	m_messageEventFilter = new MessageEventFilter( m_session );
	m_messageEventFilter->registerMessageEventHandler( this );
	m_chatStateFilter = new ChatStateFilter( m_session );
	m_chatStateFilter->registerChatStateHandler( this );
}
/*****************************************************************************/
void ADXmppProxy::handleLog( LogLevel level, LogArea area, const std::string& message )
{
	if(DebugLog)
		printf("log: level: %d, area: %d, %s\n", level, area, message.c_str() );
}
/*****************************************************************************/


