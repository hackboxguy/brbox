#include "CXmppProxy.hpp"
#include <iostream>
using namespace std;
#include <stdio.h>
#include <time.h>
#include <string>
#include <unistd.h>
/*****************************************************************************/
int CXmppProducer::IDGenerator = 0;//generate Unique ID for every ADXmppProxy object
/*****************************************************************************/
CXmppProxy::CXmppProxy()
{
	iConnect=false;
	DebugLog=false;
	failed_authorization=false;
	connected=false;
	HeartBeat=0;
	DisconnectNow=false;
	//j=NULL;
	PingThread.subscribe_thread_callback(this);
	PingThread.set_thread_properties(THREAD_TYPE_MONOSHOT,(void *)this);
	PingThread.start_thread();
}
CXmppProxy::~CXmppProxy()
{
	//mySessions.clear();
	PingThread.stop_thread();
}
/*****************************************************************************/
int CXmppProxy::disconnect()
{
	//if(j!=NULL)
	//{
	//	j->disconnect();
	//	while(connected)
	//		usleep(100000);
	//}
	return 0;
}
/*****************************************************************************/
int CXmppProxy::connect(char* user,char* password)
{
	/*if(j!=NULL)
		return 0;
	if(DebugLog)
		cout<<"ADXmppProxy::connect: Entering===>"<<endl;
	JID jid( user);
	j = new Client( jid,password);
	connected=true;//after creation of Client object, make this flag true
	j->setSasl(true);
	j->setSASLMechanisms(gloox::SaslMechPlain);
	j->setTls(gloox::TLSPolicy::TLSOptional);

	j->registerConnectionListener( this );
	j->registerMessageSessionHandler( this, 0 );
	j->rosterManager()->registerRosterListener( this );
	j->disco()->setVersion( "messageTest", GLOOX_VERSION, "Linux" );
	j->disco()->setIdentity( "client", "jsonbot" );
	j->disco()->addFeature( XMLNS_CHAT_STATES );
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
		cout<<"ADXmppProxy::connect: exiting<==="<<endl;*/
	return 0;
}
/*****************************************************************************/
void CXmppProxy::SetDebugLog(bool log)
{
	DebugLog=log;
}
/*****************************************************************************/
/*void ADXmppProxy::onConnect()
{
	if(DebugLog)
		cout<<"ADXmppProxy::onConnect:connected!!!"<<endl;
	iConnect=true;
}*/
/*****************************************************************************/
/*int ADXmppProxy::receive_request(std::string req,std::string sender)
{
	if(DebugLog)
		cout<<"ADXmppProxy::receive_request:received from="<<sender<<" msg="<<req<<endl;
	onXmppMessage(req,sender);//callback to the attached msg-processing-object
	return 0;
}*/
int CXmppProxy::send_reply(std::string reply,std::string sender)
{
	/*if(j==NULL)
		return -1;
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
	}*/
	return -1;
}
/*****************************************************************************/
// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string CXmppProxy::currentDateTime() 
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	return buf;
}
/*****************************************************************************/
void CXmppProxy::send_client_alive_ping()
{
	//if ping-thread is already blocked, then dont push for more ping requests, just return.
	if(PingPipe.empty())
	{
		PingPipe.push_back(0);//push test-integer0 value
		PingThread.wakeup_thread();//tell the worker to start working
	}
}
/*****************************************************************************/
/*****************************************************************************/
int CXmppProxy::monoshot_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	/*while (!PingPipe.empty())
	{
		int pingsample = PingPipe.front();
		if(j!=NULL)
		{
			j->xmppPing(j->jid(),this);
			if(++HeartBeat > 3)
			{
				if(DebugLog)
					cout<<"ADXmppProxy::send_client_alive_ping:going to disconnect"<<endl;
				disconnect();
				if(DebugLog)
					cout<<"ADXmppProxy::send_client_alive_ping:disconnected"<<endl;
			}
		}
		if(DebugLog)
			cout<<"ADXmppProxy::send_client_alive_ping:"<<currentDateTime()<<" connSts="<<connected<<endl;
		PingPipe.pop_front();//after processing delete the entry
	}*/
	return 0;
}
/* ------------------------------------------------------------------------- */
//following function is used for sending async-event-notification to subscribed buddy
bool CXmppProxy::SendMessageToBuddy(std::string address, const std::string & body, const std::string & subject)
{
	if(iConnect==false)
	{
		if(DebugLog)
			cout<<"ADXmppProxy::SendMessageToBuddy:client disconnected, cannot send message to buddy!!!"<<endl;
		return false;
	}
	//check if address is in my buddy-list(doesnt make sense to send message to non-buddy - it will not be delivered)
	/*vector<std::string>::iterator it;
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
	}*/
	return false;//address is not in my buddy-list
}
/* ------------------------------------------------------------------------- */
int CXmppProxy::get_buddy_list(std::string &returnval)
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

