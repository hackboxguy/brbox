#include "ADXmppProxy.hpp"
#include <iostream>
using namespace std;
#include <stdio.h>
#include <time.h>
#include <string>
#include <unistd.h>

/*****************************************************************************/
int ADXmppProducer::IDGenerator = 0;//generate Unique ID for every ADXmppProxy object
/*****************************************************************************/
ADXmppProxy::ADXmppProxy()
{
	iConnect=false;
	DebugLog=false;
	failed_authorization=false;
	connected=false;
	HeartBeat=0;
	DisconnectNow=false;
	//m_session=0;
	//m_messageEventFilter=0;
	//m_chatStateFilter=0;
	j=NULL;

	PingThread.subscribe_thread_callback(this);
	PingThread.set_thread_properties(THREAD_TYPE_MONOSHOT,(void *)this);
	PingThread.start_thread();
}
ADXmppProxy::~ADXmppProxy()
{
	mySessions.clear();
	PingThread.stop_thread();
}
/*****************************************************************************/
int ADXmppProxy::disconnect()
{
	if(j!=NULL)
	{
		j->disconnect();
		while(connected)
			usleep(100000);
	}
	return 0;
}
/*****************************************************************************/
int ADXmppProxy::connect(char* user,char* password)
{
	if(j!=NULL)
		return 0;

	if(DebugLog)
		cout<<"ADXmppProxy::connect: Entering===>"<<endl;

	JID jid( user);
	//myJid=jid;
	j = new Client( jid,password);
	connected=true;//after creation of Client object, make this flag true

	//j->setSasl(true);
	//j->setSASLMechanisms(gloox::SaslMechPlain);//SaslMechDigestMd5);//SaslMechScramSha1);//);
	//j->setTls(gloox::TLSPolicy::TLSOptional);//TLSDisabled);

	j->registerConnectionListener( this );
	j->registerMessageSessionHandler( this, 0 );
	j->rosterManager()->registerRosterListener( this );
	j->disco()->setVersion( "messageTest", GLOOX_VERSION, "Linux" );
	j->disco()->setIdentity( "client", "jsonbot" );
	j->disco()->addFeature( XMLNS_CHAT_STATES );
	//StringList ca;
	//ca.push_back( "./cacert.crt" );
	//j->setCACerts( ca );

	//LogLevelDebug
	if(DebugLog)
		j->logInstance().registerLogHandler( LogLevelDebug, LogAreaAll, this );
	else
		j->logInstance().registerLogHandler( LogLevelWarning, LogAreaAll, this );
		
	HeartBeat=0;

	ConnectionError ce = ConnNoError;
	if( j->connect( false ) )
	{
		while( ce == ConnNoError )
		{
			ce = j->recv();
			if(DebugLog)
				cout<<"ADXmppProxy::connect:Message Arrived!!!"<<endl;
		}
		if(DebugLog)
			printf( "ce: %d\n", ce );
	}

	usleep(100000);

	delete( j );
	j=NULL;
	
	connected=false;

	if(DebugLog)
		cout<<"ADXmppProxy::connect: exiting<==="<<endl;

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
	iConnect=true;
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
	if(DebugLog)
		cout<<"ADXmppProxy::onDisconnect:disconnected"<<endl;
	iConnect=false;
}
/*****************************************************************************/
bool ADXmppProxy::onTLSConnect( const CertInfo& info )
{
	return true;
}
/*****************************************************************************/
void ADXmppProxy::handleMessage( const Message& msg, MessageSession * session )
{
	if(DebugLog)
	cout<<"ADXmppProxy::handleMessage:arrived:msg:"<<msg.body()<<":len:"<<msg.body().size()<<" from="<<session->target().bare()<<endl;
	if(msg.body().size()>0)
		receive_request(msg.body(),session->target().bare());
	//else 
		//ignore-the-message
}
/*****************************************************************************/
int ADXmppProxy::receive_request(std::string req,std::string sender)
{
	if(DebugLog)
		cout<<"ADXmppProxy::receive_request:received from="<<sender<<" msg="<<req<<endl;
	onXmppMessage(req,sender);//callback to the attached msg-processing-object
	return 0;
}
int ADXmppProxy::send_reply(std::string reply,std::string sender)
{
	if(j==NULL)
		return -1;
	//std::string sub;
	Sessions::iterator it = mySessions.find(sender);
	if (it != mySessions.end())
	{
		it->second.m_messageEventFilter->raiseMessageEvent( MessageEventDisplayed );
		it->second.m_messageEventFilter->raiseMessageEvent( MessageEventComposing );
		it->second.m_chatStateFilter->setChatState( ChatStateComposing );
		it->second.m_session->send( reply, gloox::EmptyString );
		if(DebugLog)
			cout<<"ADXmppProxy::send_reply:to:"<<sender<<" sending msg = "<<reply<<endl;
		return 0;
	}
	return -1;

	//send response
	//m_messageEventFilter->raiseMessageEvent( MessageEventDisplayed );
	//m_messageEventFilter->raiseMessageEvent( MessageEventComposing );
	//m_chatStateFilter->setChatState( ChatStateComposing );
	//m_session->send( reply, gloox::EmptyString );//after reply from json-rpc-server, call send reply
	//if(DebugLog)
	//	cout<<"ADXmppProxy::send_reply:to:"<<sender<<" sending msg = "<<reply<<endl;
	//return 0;
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
//this function is called first time when client starts chat(after this, handleMessage is called)
void ADXmppProxy::handleMessageSession( MessageSession *session )
{
	if(j==NULL)
		return;
	const gloox::JID& jid = session->target();
	if(DebugLog)
		cout<<"Got new message session for "<<jid.full().c_str()<<endl;
	Sessions::iterator it = mySessions.find(jid.bare());
	if (it != mySessions.end())
	{
		if(DebugLog)
		cout<<"Disposing existing message session for "<<it->second.m_session->target().full().c_str()<<endl;
		j->disposeMessageSession(it->second.m_session);
		mySessions.erase(it);
	}
	session->registerMessageHandler(this);
	gloox::ChatStateFilter* filter = new gloox::ChatStateFilter(session);
	gloox::MessageEventFilter* evntfilter = new gloox::MessageEventFilter(session);
	filter->registerChatStateHandler(this);
	evntfilter->registerMessageEventHandler( this );
	Session newSession;
	newSession.m_session            = session;
	newSession.m_chatStateFilter    = filter;
	newSession.m_messageEventFilter = evntfilter;
	mySessions[jid.bare()] = newSession;

	//printf( "got new session\n");
	// this example can handle only one session. so we get rid of the old session
	/*if(m_session)
		j->disposeMessageSession( m_session );
	m_session = session;
	m_session->registerMessageHandler( this );
	m_messageEventFilter = new MessageEventFilter( m_session );
	m_messageEventFilter->registerMessageEventHandler( this );
	m_chatStateFilter = new ChatStateFilter( m_session );
	m_chatStateFilter->registerChatStateHandler( this );*/
}
/*****************************************************************************/
void ADXmppProxy::handleLog( LogLevel level, LogArea area, const std::string& message )
{
	if(DebugLog)
		printf("log: level: %d, area: %d, %s\n", level, area, message.c_str() );
}
/*****************************************************************************/
// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string ADXmppProxy::currentDateTime() 
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	return buf;
}
void ADXmppProxy::send_client_alive_ping()
{
	//if ping-thread is already blocked, then dont push for more ping requests, just return.
	if(PingPipe.empty())
	{
		PingPipe.push_back(0);//push test-integer0 value
		PingThread.wakeup_thread();//tell the worker to start working
	}
}
/*****************************************************************************/
int ADXmppProxy::monoshot_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	while (!PingPipe.empty())
	{
		int pingsample = PingPipe.front();

		if(j!=NULL)
		{
			j->xmppPing(j->jid(),this);//,this->handleEvent);
			if(++HeartBeat > 3)//MAX_RESPONSE_TIME_OUT)
			{
				if(DebugLog)
					cout<<"ADXmppProxy::send_client_alive_ping:going to disconnect"<<endl;
			//j->disconnect();//ConnNotConnected);//ConnTlsFailed);// fake disconnect reason so that no </stream:stream> is sent
				disconnect();
				if(DebugLog)
					cout<<"ADXmppProxy::send_client_alive_ping:disconnected"<<endl;
			}
		}
		if(DebugLog)
			cout<<"ADXmppProxy::send_client_alive_ping:"<<currentDateTime()<<" connSts="<<connected<<endl;

		PingPipe.pop_front();//after processing delete the entry
	}

	return 0;
}
/*****************************************************************************/
void ADXmppProxy::handleEvent( const Event& event )
{
	switch(event.eventType())
	{
		case Event::PingPing:
			break;
		case Event::PingPong:
			--HeartBeat;
			break;
		case Event::PingError:
			break;
		default:
			break;
	}
	//cout<<"handleEvent::pong received"<<endl;
}
/*****************************************************************************/
void ADXmppProxy::onResourceBindError( ResourceBindError error )
{
	//printf( "onResourceBindError: %d\n", error );
}
void ADXmppProxy::onSessionCreateError( SessionCreateError error )
{
	//printf( "onSessionCreateError: %d\n", error );
}
void ADXmppProxy::handleItemSubscribed( const JID& jid )
{
	//printf( "subscribed %s\n", jid.bare().c_str() );
}
void ADXmppProxy::handleItemAdded( const JID& jid )
{
	//printf( "added %s\n", jid.bare().c_str() );
}
void ADXmppProxy::handleItemUnsubscribed( const JID& jid )
{
	//printf( "unsubscribed %s\n", jid.bare().c_str() );
}
void ADXmppProxy::handleItemRemoved( const JID& jid )
{
	//printf( "removed %s\n", jid.bare().c_str() );
}
void ADXmppProxy::handleItemUpdated( const JID& jid )
{
	//printf( "updated %s\n", jid.bare().c_str() );
}
void ADXmppProxy::handleRoster( const Roster& roster )
{
	//printf( "roster arriving\nitems:\n" );
	Roster::const_iterator it = roster.begin();
	for( ; it != roster.end(); ++it )
	{
		/*printf( "jid: %s, name: %s, subscription: %d\n",
		(*it).second->jidJID().full().c_str(), (*it).second->name().c_str(),
		(*it).second->subscription() );
		StringList g = (*it).second->groups();
		StringList::const_iterator it_g = g.begin();
		for( ; it_g != g.end(); ++it_g )
			printf( "\tgroup: %s\n", (*it_g).c_str() );
		RosterItem::ResourceMap::const_iterator rit = (*it).second->resources().begin();
		for( ; rit != (*it).second->resources().end(); ++rit )
			printf( "resource: %s\n", (*rit).first.c_str() );*/
		BuddyList.push_back((*it).second->jidJID().full());
		if(DebugLog)
			cout<<"ADXmppProxy::handleRoster:"<<(*it).second->jidJID().full()<<endl;
	}
}
void ADXmppProxy::handleRosterError( const IQ& /*iq*/ )
{
	//printf( "a roster-related error occured\n" );
}
void ADXmppProxy::handleRosterPresence( const RosterItem& item, const std::string& resource,
Presence::PresenceType presence, const std::string& /*msg*/ )
{
	//printf( "presence received: %s/%s -- %d\n", item.jidJID().full().c_str(), resource.c_str(), presence );
}
void ADXmppProxy::handleSelfPresence( const RosterItem& item, const std::string& resource,
	Presence::PresenceType presence, const std::string& /*msg*/ )
{
	//printf( "self presence received: %s/%s -- %d\n", item.jidJID().full().c_str(), resource.c_str(), presence );
}
bool ADXmppProxy::handleSubscriptionRequest( const JID& jid, const std::string& /*msg*/ )
{
	//printf("request for subscription");//dont allow auto subscribing
	//printf( "subscription: %s\n", jid.bare().c_str() );
	//StringList groups;
	//JID id( jid );
	//j->rosterManager()->subscribe( id, "", groups, "" );
	return false;//true;
}
bool ADXmppProxy::handleUnsubscriptionRequest( const JID& jid, const std::string& /*msg*/ )
{
	//printf( "unsubscription: %s\n", jid.bare().c_str() );
	if(DebugLog)
		cout<<"ADXmppProxy::handleUnsubscriptionRequest: user "<<jid.bare().c_str()<<" removed from buddy-list"<<endl;
	return true;
}
void ADXmppProxy::handleNonrosterPresence( const Presence& presence )
{
	//printf( "received presence from entity not in the roster: %s\n", presence.from().full().c_str() );
}
/* ------------------------------------------------------------------------- */
//following function is used for sending async-event-notification to subscribed buddy
bool ADXmppProxy::SendMessageToBuddy(std::string address, const std::string & body, const std::string & subject)
{
	//for testing async-sync event
	//XmppProxy.SendMessageToBuddy(cmdArg,"hellloooo","test-subjext");
	//return RPC_SRV_RESULT_SUCCESS;
	if(iConnect==false)
	{
		if(DebugLog)
			cout<<"ADXmppProxy::SendMessageToBuddy:client disconnected, cannot send message to buddy!!!"<<endl;
		return false;
	}
	//check if address is in my buddy-list(doesnt make sense to send message to non-buddy - it will not be delivered)
	vector<std::string>::iterator it;
	for(it = BuddyList.begin(); it != BuddyList.end(); it++)
	{
		string str = *it;
		if(address==str)
		{
			Sessions::iterator it = mySessions.find(address);
			if (it != mySessions.end()) //check if session already exists in my list
			{
				it->second.m_messageEventFilter->raiseMessageEvent( MessageEventDisplayed );
				it->second.m_messageEventFilter->raiseMessageEvent( MessageEventComposing );
				it->second.m_chatStateFilter->setChatState( ChatStateComposing );
				it->second.m_session->send( body, subject);//gloox::EmptyString );
				if(DebugLog)
			cout<<"ADXmppProxy::SendMessageToBuddy:address="<<address<<" body="<<body<<" subject="<<subject<<endl;
			}
			else //if session doesnt exist in my list, then create new session and send the message
			{
				MessageSession* session = new MessageSession(j,JID(address));
				session->send(body, subject);//todo: presence check needed before sending message?
				j->disposeMessageSession( session );
				if(DebugLog)
			cout<<"ADXmppProxy::SendMessageToBuddy:address="<<address<<" body="<<body<<" subject="<<subject<<endl;
			}
			return true;
		}
	}
	return false;//address is not in my buddy-list
}
/* ------------------------------------------------------------------------- */
int ADXmppProxy::get_buddy_list(std::string &returnval)
{
	vector<std::string>::iterator it;
	for(it = BuddyList.begin(); it != BuddyList.end(); it++)
	{
		returnval+=*it;
		returnval+='\n';
	}
	return 0;
}
/* ------------------------------------------------------------------------- */

