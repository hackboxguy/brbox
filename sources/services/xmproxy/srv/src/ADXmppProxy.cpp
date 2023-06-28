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
	OnDemandDisconnect=false;//user requested disconnection via rpc
	//m_session=0;
	//m_messageEventFilter=0;
	//m_chatStateFilter=0;
	j=NULL;
	AdminBuddy="";
	BkupAdminBuddy="";
	//AcceptThisBuddy="";
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
int ADXmppProxy::connect(char* user,char* password, std::string adminbuddy, std::string bkupadminbuddy)
{
	if(j!=NULL)
		return 0;

	if(DebugLog)
		cout<<"ADXmppProxy::connect: Entering===>"<<endl;
	AdminBuddy=adminbuddy;
	BkupAdminBuddy=bkupadminbuddy;

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
	//ca.push_back( "/home/usr/prosody.crt" );
	//j->setCACerts( ca );

	//LogLevelDebug
	if(DebugLog)
		j->logInstance().registerLogHandler( LogLevelDebug, LogAreaAll, this );
	else
		j->logInstance().registerLogHandler( LogLevelWarning, LogAreaAll, this );

	//if(BOSH_Connection==true)
	//{
	//	j->setCompression(false);
	//	ConnectionTCPClient* conn0 = new ConnectionTCPClient( j->logInstance(), "bind.jabber.de", 443 );
	//	ConnectionBOSH* conn1 = new ConnectionBOSH( j, conn0, j->logInstance(), "bind.jabber.de", "jabber.de" );
		//conn1->setMode( ConnectionBOSH::ModeLegacyHTTP );
		//conn1->setMode( ConnectionBOSH::ModePersistentHTTP );
	//	j->setConnectionImpl( conn1 );
	//	j->setForceNonSasl( true );
	//}

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
	if(DebugLog)
	{
		cout<<"ADXmppProxy::onTLSConnect:called!!!"<<endl;
		time_t from( info.date_from );
		time_t to( info.date_to );
		printf( "status: %d\nissuer: %s\npeer: %s\nprotocol: %s\nmac: %s\ncipher: %s\ncompression: %s\n",
				info.status, info.issuer.c_str(), info.server.c_str(),
				info.protocol.c_str(), info.mac.c_str(), info.cipher.c_str(),
				info.compression.c_str() );
		printf( "from: %s", ctime( &from ) );
		printf( "to:   %s", ctime( &to ) );
	}
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

	//if sender is admin, then just process the request else check if sender is in our BuddyList.
	if(sender == AdminBuddy || sender == BkupAdminBuddy)
	{
		onXmppMessage(req,sender);
		return 0;
	}

	//process sender message only if sender is part of our roster(respond to only authorized users)
	Roster *pRoster=j->rosterManager()->roster();
	Roster::const_iterator it = pRoster->begin();
	for( ; it != pRoster->end(); ++it )
	{
		//cout<<"ADXmppProxy::handleItemAdded:"<<(*it).second->jidJID().full()<<endl;
		if(sender==(*it).second->jidJID().full())
		{
			onXmppMessage(req,sender);//callback to the attached msg-processing-object
			return 0;
		}
	}

	//process sender message only if sender is part of our roster(respond to only authorized users)
	//vector<std::string>::iterator it;
	//for(it = BuddyList.begin(); it != BuddyList.end(); it++)
	//{
	//	string str = *it;
	//	if(sender==str)
	//	{
	//		onXmppMessage(req,sender);//callback to the attached msg-processing-object
	//		return 0;
	//	}
	//}
	if(DebugLog)
		cout<<"ADXmppProxy::receive_request:sender is not authorized!!! ingnoring the request"<<endl;
	return -1;
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
	if(DebugLog)
		cout<<"ADXmppProxy::onResourceBindError: error:"<<error<<endl;
}
void ADXmppProxy::onSessionCreateError( SessionCreateError error )
{
	//printf( "onSessionCreateError: %d\n", error );
	if(DebugLog)
		cout<<"ADXmppProxy::onSessionCreateError: error:"<<error<<endl;
}
void ADXmppProxy::handleItemSubscribed( const JID& jid )
{
	//printf( "subscribed %s\n", jid.bare().c_str() );
	if(DebugLog)
		cout<<"ADXmppProxy::handleItemSubscribed: subscribed:"<<jid.bare().c_str()<<endl;
}
void ADXmppProxy::handleItemAdded( const JID& jid )
{
	//printf( "added %s\n", jid.bare().c_str() );
	if(DebugLog)
		cout<<"ADXmppProxy::handleItemAdded: added:"<<jid.bare().c_str()<<endl;

	//Roster *pRoster=j->rosterManager()->roster();
	//Roster::const_iterator it = pRoster->begin();
	//for( ; it != pRoster->end(); ++it )
	//{
	//	cout<<"ADXmppProxy::handleItemAdded:"<<(*it).second->jidJID().full()<<endl;
	//}
}
void ADXmppProxy::handleItemUnsubscribed( const JID& jid )
{
	//printf( "unsubscribed %s\n", jid.bare().c_str() );
	if(DebugLog)
		cout<<"ADXmppProxy::handleItemUnsubscribed: unsubscribed:"<<jid.bare().c_str()<<endl;
}
void ADXmppProxy::handleItemRemoved( const JID& jid )
{
	//printf( "removed %s\n", jid.bare().c_str() );
	if(DebugLog)
		cout<<"ADXmppProxy::handleItemRemoved: removed:"<<jid.bare().c_str()<<endl;
}
void ADXmppProxy::handleItemUpdated( const JID& jid )
{
	//printf( "updated %s\n", jid.bare().c_str() );
	if(DebugLog)
		cout<<"ADXmppProxy::handleItemUpdated: updated:"<<jid.bare().c_str()<<endl;
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


		//vector<std::string>::iterator bit;
		//for(bit = BuddyList.begin(); bit != BuddyList.end(); bit++)
		//{
		//	string str = *bit;
		//	if((*it).second->jidJID().full()==str) //if this buddy is already in our list, then dont add duplicate entry
		//		continue;
		//}
		//BuddyList.push_back((*it).second->jidJID().full());//this user is not found in our BuddyList, hence add to our list
		if(DebugLog)
			cout<<"ADXmppProxy::handleRoster:"<<endl;
	}
}
void ADXmppProxy::handleRosterError( const IQ& /*iq*/ )
{
	//printf( "a roster-related error occured\n" );
	if(DebugLog)
		cout<<"ADXmppProxy::handleRosterError: roster-related error occured"<<endl;
}
void ADXmppProxy::handleRosterPresence( const RosterItem& item, const std::string& resource,
Presence::PresenceType presence, const std::string& /*msg*/ )
{
	//printf( "presence received: %s/%s -- %d\n", item.jidJID().full().c_str(), resource.c_str(), presence );
	if(DebugLog)
		cout<<"ADXmppProxy::handleRosterPresence: received:"<<item.jidJID().full().c_str()<<":"<<resource.c_str()<<":"<<convert_presence_enum_to_str(presence)<<endl;
}
void ADXmppProxy::handleSelfPresence( const RosterItem& item, const std::string& resource,
	Presence::PresenceType presence, const std::string& /*msg*/ )
{
	//printf( "self presence received: %s/%s -- %d\n", item.jidJID().full().c_str(), resource.c_str(), presence );
	if(DebugLog)
		cout<<"ADXmppProxy::handleSelfPresence: received:"<<item.jidJID().full().c_str()<<":"<<resource.c_str()<<":"<<presence<<endl;
}
bool ADXmppProxy::handleSubscriptionRequest( const JID& jid, const std::string& /*msg*/ )
{
	if(DebugLog)
		cout<<"ADXmppProxy::handleSubscriptionRequest:from:"<<jid.bare().c_str()<<endl;

	//check if subscriber-buddy needs to be accepted based on available cmdline-arg or through some other means(AdminBuddy),
	//if subscriber-buddy is to be accepted, then accept the subscription and also send subscription-request to buddy in return
	if(jid.bare()==AdminBuddy || jid.bare()==BkupAdminBuddy)
	{
		//StringList groups;
		//JID id( jid );
		//j->rosterManager()->subscribe( id, "", groups, "" );
		if(DebugLog)
			cout<<"ADXmppProxy::handleSubscriptionRequest:AdminBuddy has been accepted:"<<AdminBuddy<<endl;
		return true;//true;
	}
	else
	{
		//check if this buddy is to be accepted if available in approved-AcceptBuddyList
		std::string tmp = jid.bare().c_str();
		cout<<"ADXmppProxy::handleSubscriptionRequest:searching:"<<tmp<<endl;
		std::vector<string>::iterator itr = std::find(AcceptBuddyList.begin(), AcceptBuddyList.end(), tmp);//jid.bare());
		if (itr != AcceptBuddyList.end())
		{
			//StringList groups;
			//JID id( jid );
			//j->rosterManager()->subscribe( id, "", groups, "" );
			if(DebugLog)
				cout<<"ADXmppProxy::handleSubscriptionRequest:Accepting pre-approved buddy:"<<*itr<<endl;
			return true;
		}


		/*vector<std::string>::iterator bit;
		for(bit = AcceptBuddyList.begin(); bit != AcceptBuddyList.end(); bit++)
		{
			string str = *bit;
			if(jid.bare() == str) //accept this buddy as this buddy is already part of our approved list
			{
				StringList groups;
				JID id( jid );
				j->rosterManager()->subscribe( id, "", groups, "" );
				if(DebugLog)
					cout<<"ADXmppProxy::handleSubscriptionRequest:Accepting pre-approved buddy:"<<str<<endl;
				return true;
			}
		}*/
		if(DebugLog)
			cout<<"ADXmppProxy::handleSubscriptionRequest: this buddy has not been authorized: "<<jid<<endl;
		return false;
	}
}
bool ADXmppProxy::handleUnsubscriptionRequest( const JID& jid, const std::string& /*msg*/ )
{
	//printf( "unsubscription: %s\n", jid.bare().c_str() );
	//TODO: remove this user from our BuddyList so that we dont send events to this user.
	if(DebugLog)
		cout<<"ADXmppProxy::handleUnsubscriptionRequest: user "<<jid.bare().c_str()<<" removed from buddy-list"<<endl;
	remove_buddy(jid.bare().c_str());//remove from our buddy list : TODO: check if this entry is also removed from roster
	//unsubscribe_buddy(std::string buddy); //TODO: check if mutual unsubscription is needed
	return true;
}
void ADXmppProxy::handleNonrosterPresence( const Presence& presence )
{
	//printf( "received presence from entity not in the roster: %s\n", presence.from().full().c_str() );
	if(DebugLog)
		cout<<"ADXmppProxy::handleNonrosterPresence: entity not in the roster: "<<presence.from().full().c_str()<<endl;
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
	//vector<std::string>::iterator it;
	//for(it = BuddyList.begin(); it != BuddyList.end(); it++)
	Roster *pRoster=j->rosterManager()->roster();
	Roster::const_iterator it = pRoster->begin();
	for( ; it != pRoster->end(); ++it )
	{
		string str = (*it).second->jidJID().full();//*it;
		if(address==str)//send message only if sender is in our Roster()
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
bool ADXmppProxy::IsMyBuddy(std::string buddyaddress)
{
	if(iConnect==false)
	{
		if(DebugLog)
			cout<<"ADXmppProxy::IsMyBuddy:client disconnected, cannot check if its my buddy!!!"<<endl;
		return false;
	}
	Roster *pRoster=j->rosterManager()->roster();
	Roster::const_iterator it = pRoster->begin();
	for( ; it != pRoster->end(); ++it )
	{
		string str = (*it).second->jidJID().full();//*it;
		if(buddyaddress==str)//return true only if buddyaddr is in our Roster()
			return true;
	}
	return false;
}
/* ------------------------------------------------------------------------- */
int ADXmppProxy::get_buddy_list(std::string &returnval)
{
	//vector<std::string>::iterator it;
	//for(it = BuddyList.begin(); it != BuddyList.end(); it++)
	//{
	//	returnval+=*it;
	//	returnval+='\n';
	//}
	Roster *pRoster=j->rosterManager()->roster();
	Roster::const_iterator it = pRoster->begin();
	for( ; it != pRoster->end(); ++it )
	{
		//cout<<"ADXmppProxy::handleItemAdded:"<<(*it).second->jidJID().full()<<endl;
		returnval+=(*it).second->jidJID().full();
		returnval+='\n';
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
bool ADXmppProxy::get_connected_status()
{
	return iConnect;
}
/* ------------------------------------------------------------------------- */
std::string ADXmppProxy::convert_presence_enum_to_str(Presence::PresenceType presence)
{
	switch(presence)
	{
		case Presence::Available:
			return "Available";
		case Presence::Chat:
			return "Chat";
		case Presence::Away:
			return "Away";
		case Presence::DND: //do-not-disturb
			return "DND";
		case Presence::XA: //extended-away
			return "XA";
		case Presence::Unavailable:
			return "Unavailable";
		case Presence::Probe:
			return "Probe";
		case Presence::Error:
			return "Error";
		case Presence::Invalid:
			return "Invalid";
		default:
			return "Unknown";
	}
}
/* ------------------------------------------------------------------------- */
//note: only admin-buddy shall call this function
int ADXmppProxy::accept_buddy(std::string buddy)
{
	vector<std::string>::iterator bit;
	for(bit = AcceptBuddyList.begin(); bit != AcceptBuddyList.end(); bit++)
	{
		string str = *bit;
		if(buddy==str) //if this buddy is already in our list, then just return success
		{
			if(DebugLog)
				cout<<"ADXmppProxy::accept_buddy:buddy is already in our list:"<<buddy<<endl;
			return 0;
		}
	}
	AcceptBuddyList.push_back(buddy);//next-time, when this buddy subscribes, then accept it
	if(DebugLog)
		cout<<"ADXmppProxy::accept_buddy:buddy has been added to our list:"<<buddy<<endl;
	return 0;
}
int ADXmppProxy::remove_buddy(std::string buddy)
{
	std::vector<string>::iterator itr = std::find(AcceptBuddyList.begin(), AcceptBuddyList.end(), buddy);
	if (itr != AcceptBuddyList.end())
	{
		AcceptBuddyList.erase(itr);
		if(DebugLog)
			cout<<"ADXmppProxy::remove_buddy:buddy has been removed from our list:"<<buddy<<endl;
		return 0;
	}
	else
	{
		if(DebugLog)
			cout<<"ADXmppProxy::remove_buddy:requested buddy is not in the list!!!!:"<<buddy<<endl;
		return -1;
	}
}
/* ------------------------------------------------------------------------- */
bool ADXmppProxy::is_admin_user(std::string user)
{
	if(user == AdminBuddy || user == BkupAdminBuddy)
		return true;
	else
		return false;
}
/* ------------------------------------------------------------------------- */
int ADXmppProxy::subscribe_buddy(std::string buddy)
{
	//StringList groups;
	JID id( buddy );
	j->rosterManager()->subscribe(id);//, "", groups, "" );
	return 0;
}
/* ------------------------------------------------------------------------- */
int ADXmppProxy::unsubscribe_buddy(std::string buddy)
{
	//StringList groups;
	JID id( buddy );
	j->rosterManager()->unsubscribe(id);//, "", groups, "" );
	return 0;
}
/* ------------------------------------------------------------------------- */
int ADXmppProxy::get_buddy_online_state(std::string buddy)
{
	JID id( buddy );
	return j->rosterManager()->getRosterItem(id)->online();
	//j->rosterManager()->getRosterItem(id)->subscription();
}
/* ------------------------------------------------------------------------- */
int ADXmppProxy::get_accept_buddy_list(std::string &returnval)
{
	vector<std::string>::iterator it;
	for(it = AcceptBuddyList.begin(); it != AcceptBuddyList.end(); it++)
	{
		returnval+=*it;
		returnval+='\n';
	}
	//Roster *pRoster=j->rosterManager()->roster();
	//Roster::const_iterator it = pRoster->begin();
	//for( ; it != pRoster->end(); ++it )
	//{
	//	returnval+=(*it).second->jidJID().full();
	//	returnval+='\n';
	//}
	return 0;
}
/* ------------------------------------------------------------------------- */
