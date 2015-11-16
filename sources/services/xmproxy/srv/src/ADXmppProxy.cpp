#include "ADXmppProxy.hpp"
#include <iostream>
using namespace std;
#include <stdio.h>
/*****************************************************************************/
int ADXmppProducer::IDGenerator = 0;//generate Unique ID for every ADXmppProxy object
/*****************************************************************************/
ADXmppProxy::ADXmppProxy()
{
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
	StringList ca;
	ca.push_back( "./cacert.crt" );
	j->setCACerts( ca );

	j->logInstance().registerLogHandler( LogLevelDebug, LogAreaAll, this );

	if( j->connect( false ) )
	{
		ConnectionError ce = ConnNoError;
		while( ce == ConnNoError )
		{
			ce = j->recv();
			//printf("messaged arrived\n");
		}
		//printf( "ce: %d\n", ce );
	}
	delete( j );
	return 0;
}
/*****************************************************************************/
void ADXmppProxy::onConnect()
{
	//printf( "connected!!!\n" );
	connected=true;
}
/*****************************************************************************/
void ADXmppProxy::onDisconnect( ConnectionError e )
{
	if( e == ConnAuthenticationFailed )
	{
		failed_authorization=true;
	}
	connected=false;
	cout<<"ADXmppProxy::disconnected"<<endl;
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
	return 0;
}
/*****************************************************************************/
void ADXmppProxy::handleMessageEvent( const JID& from, MessageEventType event )
{
	//printf( "received event: %d from: %s\n", event, from.full().c_str() );
}
/*****************************************************************************/
void ADXmppProxy::handleChatState( const JID& from, ChatStateType state)
{
	//printf( "received state: %d from: %s\n", state, from.full().c_str() );
}
/*****************************************************************************/
void ADXmppProxy::handleMessageSession( MessageSession *session )
{
	//printf( "got new session\n");
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
	//printf("log: level: %d, area: %d, %s\n", level, area, message.c_str() );
}
/*****************************************************************************/


