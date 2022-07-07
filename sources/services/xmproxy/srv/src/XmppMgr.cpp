#include "XmppMgr.h"
#include <algorithm>
#include <mutex>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cerrno>
#include <iterator>
#include "ADCmnStringProcessor.hpp"
#include "ADJsonRpcClient.hpp"
#include "ADCmnPortList.h"
#include "ADJsonRpcMgr.hpp"
using namespace std;
/* ------------------------------------------------------------------------- */
//supported commands over xmpp-channel
XMPROXY_CMD_TABLE xmproxy_cmd_table[] = //EXMPP_CMD_NONE+1] =
{
	{true ,EXMPP_CMD_GSM_MODEM_IDENT         , "gsmcheck"     ,""},
	{true ,EXMPP_CMD_SMS_LIST_UPDATE         , "smsupdate"    ,""},
	{true ,EXMPP_CMD_SMS_GET_TOTAL           , "smstotal"     ,""},
	{true ,EXMPP_CMD_SMS_GET                 , "smsget"       ,"<zero_index_msg>"},
	{true ,EXMPP_CMD_SMS_DELETE_ALL          , "smsdeleteall" ,""},
	{true ,EXMPP_CMD_SMS_SEND                , "smssend"      ,"<phone-num> <msg>"},
	{true ,EXMPP_CMD_DIAL_VOICE              , "dialvoice"    ,"<phone-num>"},
	{true ,EXMPP_CMD_DIAL_USSD               , "dialussd"     ,"<ussd-code>"},
	{true ,EXMPP_CMD_GET_USSD                , "readussd"     ,""},
	{true ,EXMPP_CMD_FMW_GET_VERSION         , "version"      ,""},
	{true ,EXMPP_CMD_FMW_UPDATE              , "sysupdate"    ,""},
	{true ,EXMPP_CMD_FMW_REBOOT              , "reboot"       ,""},
	{true ,EXMPP_CMD_FMW_POWEROFF            , "poweroff"     ,""},
	{true ,EXMPP_CMD_FMW_UPTIME              , "uptime"       ,""},
	{true ,EXMPP_CMD_FMW_HOSTNAME            , "hostname"     ,"[name]"},
	{true ,EXMPP_CMD_FMW_RESET_HOSTNAME      , "resethostname",""},
	{true ,EXMPP_CMD_FMW_GET_MYIP            , "publicip"     ,""},
	{true ,EXMPP_CMD_FMW_GET_LOCALIP         , "localip"      ,""},
	{false,EXMPP_CMD_DEBUG_LOG_STS           , "logsts"       ,""},//due to bug, disabled(to be fixed later)
	{false,EXMPP_CMD_LOG_UPDATE              , "logupdate"    ,""},//due to bug, disabled(to be fixed later)
	{false,EXMPP_CMD_LOG_COUNT               , "logcount"     ,""},//due to bug, disabled(to be fixed later)
	{false,EXMPP_CMD_LOG_MSG                 , "logmsg"       ,"<zero_index_lineNum>"},//due to bug, disabled(to be fixed later)
	{true ,EXMPP_CMD_GPIO                    , "gpio"         ,"<gpio_num> [sts(0/1)]"},
	{true ,EXMPP_CMD_GSM_EVENT_NOTIFY        , "eventgsm"     ,"<sts[0/1]>"},
	{true ,EXMPP_CMD_GPIO_EVENT_NOTIFY       , "eventgpio"    ,"<gpio_num> [sts(0/1)]"},
	{true ,EXMPP_CMD_ALIAS                   , "alias"        ,"name=cmd"},
	{true ,EXMPP_CMD_SLEEP                   , "sleep"        ,"<seconds>"}, //adding delay between multiple commands
	{true ,EXMPP_CMD_ACCOUNT                 , "account"      ,""}, //xmpp logid ID used for logging into server
	{true ,EXMPP_CMD_BOTNAME                 , "botname"      ,"[name]"}, //xmpp chat-bot name for identification
	{true ,EXMPP_CMD_BUDDY_LIST              , "buddylist"    ,""}, //prints buddy list
	{true ,EXMPP_CMD_SHELLCMD                , "shellcmd"     ,"<command>"}, //executes remote shell command
	{true ,EXMPP_CMD_SHELLCMD_RESP           , "shellcmdresp" ,""}, //reads response of last executed shell command
	{true ,EXMPP_CMD_SHELLCMD_TRIG           , "shellcmdtrig" ,"<command>"}, //executes remote shell command but without redirecting the output
	{true ,EXMPP_CMD_DEVIDENT                , "identify" ,""},  //identify board by blinking onboard LED
	{true ,EXMPP_CMD_SHUTDOWN                , "xmpshutdown" ,""}, //shutdown xmpp server(for xmpp logout)
	{true ,EXMPP_CMD_SONOFF                  , "sonoff" ,"<ip/hostname> [sts(on/off/toggle)]"},//http based control of sonoff relay(tasmota fmw)
	{true ,EXMPP_CMD_DISPCLEAR               , "dispclear" ,""},
	{true ,EXMPP_CMD_DISPPRINT               , "display" ,"<line1/line2/line3..]> <message>"},
	{true ,EXMPP_CMD_DISPBKLT                , "dispbklt" ,"<sts[on/off]>"}
};
/* ------------------------------------------------------------------------- */
XmppMgr::XmppMgr() //:AckToken(0)
{
	CyclicTime_ms=CLIENT_ALIVE_PING_DURATION_MS;//60000;//60seconds
	event_period_ms=0;
	heartbeat_ms=100;
	pMyTimer=NULL;
	LastFmwUpdateTaskID=0;
	XmppTaskIDCounter=0;
	AliasListFile="";
	EventSubscrListFile="";
	XmppUserName="";
	XmppBotName="";
	XmppBotNameFilePath="";
	XmppNetInterface="";
	DebugLog=false;
	//GsmDevDetected=false;
	bboxSmsServerAddr=BBOXSMS_SERVER_ADDR;

	XmppProxy.attach_callback(this);
	XmppClientThread.subscribe_thread_callback(this);
	XmppClientThread.set_thread_properties(THREAD_TYPE_NOBLOCK,(void *)this);

	XmppCmdProcessThread.subscribe_thread_callback(this);
	XmppCmdProcessThread.set_thread_properties(THREAD_TYPE_MONOSHOT,(void *)this);
	XmppCmdProcessThread.start_thread();
}
XmppMgr::~XmppMgr()
{
	/*int MaxTime=0;
	XmppProxy.disconnect();
	while(XmppProxy.get_connect_sts()==true && MaxTime++<50) //max 5sec wait
	{
		usleep(100000);
		cout<<"waiting for disconnect"<<endl;
	}*/
	//XmppClientThread.stop_thread();
	XmppCmdProcessThread.stop_thread();
	AliasList.clear();
	//myEventList.clear();
}
/* ------------------------------------------------------------------------- */
int XmppMgr::AttachHeartBeat(ADTimer* pTimer)
{
	//give 100ms heartbeat to ADDisplayMgr
	//this is needed for detecting 3sec timeout for uart communication
	pTimer->subscribe_timer_notification(this);
	pMyTimer=pTimer;
	return 0;
}
int XmppMgr::timer_notification()
{
	event_period_ms+=heartbeat_ms;
	if(event_period_ms<CyclicTime_ms) //typically 60seconds of WhiteSpacePingTime
		return 0;
	event_period_ms=0;
	XmppProxy.send_client_alive_ping();//every 60sec send whitespacePing to xmpp-server to be online always
	return 0;
}
/* ------------------------------------------------------------------------- */
void XmppMgr::SetDebugLog(bool log)
{
	DebugLog=log;
	XmppProxy.SetDebugLog(log);
}
void XmppMgr::SetUSBGsmSts(bool sts)
{
	int total_cmds=sizeof(xmproxy_cmd_table)/sizeof(XMPROXY_CMD_TABLE);
	for(int i=0;i<total_cmds;i++)
	{
		switch(xmproxy_cmd_table[i].cmd)
		{
			case EXMPP_CMD_GSM_MODEM_IDENT:
			case EXMPP_CMD_SMS_LIST_UPDATE:
			case EXMPP_CMD_SMS_GET_TOTAL  :
			case EXMPP_CMD_SMS_GET        :
			case EXMPP_CMD_SMS_DELETE_ALL :
			case EXMPP_CMD_SMS_SEND       :
			case EXMPP_CMD_DIAL_VOICE     :
			case EXMPP_CMD_DIAL_USSD      :
			case EXMPP_CMD_GET_USSD       :
			case EXMPP_CMD_GSM_EVENT_NOTIFY:
				if(xmproxy_cmd_table[i].cmdsts==true) //if default is disabled, then dont enable it
					xmproxy_cmd_table[i].cmdsts=sts;
				break;
			default:
				break;
		}
	}
}
/* ------------------------------------------------------------------------- */
void XmppMgr::SetOpenWrtCmdGroupSts(bool sts)
{
	int total_cmds=sizeof(xmproxy_cmd_table)/sizeof(XMPROXY_CMD_TABLE);
	for(int i=0;i<total_cmds;i++)
	{
		switch(xmproxy_cmd_table[i].cmd)
		{
			case EXMPP_CMD_GPIO              :
			case EXMPP_CMD_GPIO_EVENT_NOTIFY :
			case EXMPP_CMD_FMW_HOSTNAME      :
			case EXMPP_CMD_FMW_RESET_HOSTNAME:
			case EXMPP_CMD_FMW_POWEROFF      :
			case EXMPP_CMD_SHUTDOWN          :
				if(xmproxy_cmd_table[i].cmdsts==true) //if default is disabled, then dont enable it
					xmproxy_cmd_table[i].cmdsts=sts;
				break;
			default:
				break;
		}
	}
}
/* ------------------------------------------------------------------------- */
std::string XmppMgr::print_help()
{
	std::string help="",cmd,cmdhlp;
	/*const char *cmdTbl[]     = EXMPP_CMD_TABL;
	const char *cmdTblHelp[] = EXMPP_CMD_TABL_HELP;
	for(int i=0;i<EXMPP_CMD_UNKNOWN;i++)
	{
		cmd=cmdTbl[i];cmdhlp=cmdTblHelp[i];
		help+=cmd+" "+cmdhlp+"\n";
	}*/
	int total_cmds=sizeof(xmproxy_cmd_table)/sizeof(XMPROXY_CMD_TABLE);
	for(int i=0;i<total_cmds;i++)
	{
		if(xmproxy_cmd_table[i].cmdsts!=true)continue;//dont show disabled commands
		cmd=xmproxy_cmd_table[i].cmd_name;cmdhlp=xmproxy_cmd_table[i].cmd_arg;
		help+=cmd+" "+cmdhlp+"\n";
	}
	return help;
}
/* ------------------------------------------------------------------------- */
#ifdef USE_CXMPP_LIB
int XmppMgr::onXmppMessage(std::string msg,std::string sender,CXmppProducer* pObj)
#else
int XmppMgr::onXmppMessage(std::string msg,std::string sender,ADXmppProducer* pObj)
#endif
{
	//process the messages
	//cout<<"msg arrived: "<<msg<<endl;
	if(msg=="Echo" || msg=="echo") //for checking if client is alive
		XmppProxy.send_reply(msg,sender);
	else if(msg=="Help" || msg=="help") //print help
		XmppProxy.send_reply(print_help(),sender);
	else
	{
		processCmd.push_back(XmppCmdEntry(msg,sender));
		XmppCmdProcessThread.wakeup_thread();//tell the worker to start working
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int XmppMgr::thread_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	//cout<<"going to connect"<<endl;
	int loginAttempt=0;
	while(1)
	{
		//cout<<"XmppMgr::thread_callback_function: entering xmpp connect"<<endl;
		LOG_DEBUG_MSG_1_ARG(DebugLog,"BRBOX:xmproxy","XmppMgr::thread_callback_function::xmpp login attempt=%d",++loginAttempt);
		if(XmppProxy.getOnDemandDisconnect()==false)//user has requested for disconnect via rpc
			XmppProxy.connect((char*)XmppUserName.c_str(),(char*)XmppUserPw.c_str());
		//cout<<"XmppMgr::thread_callback_function: exited xmpp connect"<<endl;
		//XmppProxy.disconnect();
		if(XmppProxy.getForcedDisconnect())break;

		//before retrying wait 5sec
		usleep(800000);if(XmppProxy.getForcedDisconnect())break;
		usleep(800000);if(XmppProxy.getForcedDisconnect())break;
		usleep(800000);if(XmppProxy.getForcedDisconnect())break;
		usleep(800000);if(XmppProxy.getForcedDisconnect())break;
		usleep(800000);if(XmppProxy.getForcedDisconnect())break;
		//check why disconneted, if forced disconnect, then break the loop, else try to connect again
	}
	//cout<<"exiting after connect"<<endl;
	return 0;
}
/* ------------------------------------------------------------------------- */
int XmppMgr::monoshot_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	while (!processCmd.empty())
	{
		//TODO: handle semicolon separated multiple commands
		XmppCmdEntry cmd = processCmd.front();

		//std::string temp=cmd.cmdMsg;
		stringstream mystream(cmd.cmdMsg);
		std::string mycmd;
		mystream >> mycmd;
		transform(mycmd.begin(), mycmd.end(), mycmd.begin(), ::tolower);
		if(mycmd=="alias")
		{
			std::string myreturnval="";
			RPC_SRV_RESULT myres=RPC_SRV_RESULT_UNKNOWN_COMMAND;//RPC_SRV_RESULT_FAIL;
			myres=proc_cmd_alias(cmd.cmdMsg,myreturnval);
			const char *myresTbl[] = STR_RPC_SRV_RESULT_STRING_TABLE;
			std::string myresult   = myresTbl[myres];
			std::string myresponse = "return="+myresult +" : "+"result="+myreturnval;
			XmppProxy.send_reply(myresponse,cmd.sender);
			processCmd.pop_front();
			return 0;
		}
		//check if it is an alias
		transform(cmd.cmdMsg.begin(), cmd.cmdMsg.end(), cmd.cmdMsg.begin(), ::tolower);//convert all lower case
		Alias::iterator it = AliasList.find(cmd.cmdMsg);
		if (it != AliasList.end())
			cmd.cmdMsg=it->second;
		//cout<<"cmd="<<cmd.cmdMsg<<endl;

		stringstream ss(cmd.cmdMsg);
		std::deque<string> result;
		while( ss.good() )
		{
			string substr;
			getline( ss, substr, ';' );
			//nested alias(allow only two level of alias expansion)
			Alias::iterator it = AliasList.find(substr);
			if (it != AliasList.end())
			{
				stringstream sss(it->second);
				while( sss.good() )
				{
					string subsubstr;
					getline( sss, subsubstr, ';' );
					result.push_back( subsubstr );
				}
			}
			else
				result.push_back( substr );
		}
		while (!result.empty())
		{
			std::string returnval="";
			RPC_SRV_RESULT res=RPC_SRV_RESULT_UNKNOWN_COMMAND;//RPC_SRV_RESULT_FAIL;
			std::string cmdcmdMsg=result.front();

			//check if it is an alias
			//transform(cmdcmdMsg.begin(), cmdcmdMsg.end(), cmdcmdMsg.begin(), ::tolower);//convert all lower case
			//Alias::iterator it = AliasList.find(cmdcmdMsg);
			//if (it != AliasList.end())
			//	cmdcmdMsg=it->second;

			switch(ResolveCmdStr(cmdcmdMsg))
			{
				case EXMPP_CMD_SMS_DELETE_ALL  :res=proc_cmd_sms_deleteall(cmdcmdMsg,returnval,cmd.sender);break;//inProg
				case EXMPP_CMD_SMS_DELETE      :res=proc_cmd_sms_delete(cmdcmdMsg);break;
				case EXMPP_CMD_SMS_GET         :res=proc_cmd_sms_get(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_SMS_SEND        :res=proc_cmd_sms_send(cmdcmdMsg,returnval,cmd.sender);break;//inProg
				case EXMPP_CMD_SMS_LIST_UPDATE :res=proc_cmd_sms_list_update(cmdcmdMsg,returnval,cmd.sender);break;//inProg
				case EXMPP_CMD_SMS_GET_TOTAL   :res=proc_cmd_sms_get_total(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_FMW_GET_VERSION :res=proc_cmd_fmw_get_version(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_FMW_UPDATE      :res=proc_cmd_fmw_update(cmdcmdMsg,returnval,cmd.sender);break;//inProg
				case EXMPP_CMD_FMW_UPDATE_STS  :res=proc_cmd_fmw_update_sts(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_FMW_UPDATE_RES  :res=proc_cmd_fmw_update_res(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_FMW_REBOOT      :res=proc_cmd_fmw_reboot(cmdcmdMsg,returnval,cmd.sender);break;//inProg
				case EXMPP_CMD_FMW_POWEROFF    :res=proc_cmd_fmw_poweroff(cmdcmdMsg,returnval,cmd.sender);break;//inProg
				case EXMPP_CMD_FMW_UPTIME      :res=proc_cmd_fmw_uptime(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_FMW_HOSTNAME    :res=proc_cmd_fmw_hostname(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_FMW_GET_MYIP    :res=proc_cmd_fmw_get_myip(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_FMW_RESET_HOSTNAME:res=proc_cmd_fmw_set_default_hostname(cmdcmdMsg);break;
				case EXMPP_CMD_DIAL_VOICE      :res=proc_cmd_dial_voice(cmdcmdMsg,returnval,(char*)"dial_voice",cmd.sender);break;//inPrg
				case EXMPP_CMD_DIAL_USSD       :res=proc_cmd_dial_voice(cmdcmdMsg,returnval,(char*)"dial_ussd",cmd.sender);break;//inPrg
				case EXMPP_CMD_GET_USSD        :res=proc_cmd_get_ussd(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_DEBUG_LOG_STS   :res=proc_cmd_logsts(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_GSM_MODEM_IDENT :res=proc_cmd_gsm_modem_identify(cmdcmdMsg,returnval,cmd.sender);break;//inProg
				case EXMPP_CMD_LOG_UPDATE      :res=proc_cmd_log_list_update(cmdcmdMsg,returnval,cmd.sender);break;//inProg
				case EXMPP_CMD_LOG_COUNT       :res=proc_cmd_log_get_count(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_LOG_MSG         :res=proc_cmd_log_get_line(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_FMW_GET_LOCALIP :res=proc_cmd_fmw_get_localip(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_GPIO            :res=proc_cmd_gpio(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_GSM_EVENT_NOTIFY:res=proc_cmd_event_gsm(cmdcmdMsg,cmd.sender,returnval);break;
				case EXMPP_CMD_GPIO_EVENT_NOTIFY:res=proc_cmd_event_gpio(cmdcmdMsg,cmd.sender,returnval);break;
				//case EXMPP_CMD_ALIAS           :res=proc_cmd_alias(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_SLEEP           :res=proc_cmd_sleep(cmdcmdMsg);break;
				case EXMPP_CMD_ACCOUNT         :res=proc_cmd_account_name(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_BOTNAME         :res=proc_cmd_bot_name(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_BUDDY_LIST      :res=proc_cmd_buddy_list(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_SHELLCMD        :res=proc_cmd_shellcmd(cmdcmdMsg,returnval,cmd.sender,EXMPP_CMD_SHELLCMD);break;//inProgbreak;
				case EXMPP_CMD_SHELLCMD_RESP   :res=proc_cmd_shellcmdresp(cmdcmdMsg,returnval,cmd.sender);break;//inProgbreak;
				case EXMPP_CMD_SHELLCMD_TRIG   :res=proc_cmd_shellcmd(cmdcmdMsg,returnval,cmd.sender,EXMPP_CMD_SHELLCMD_TRIG);break;//inProgbreak;
				case EXMPP_CMD_DEVIDENT        :res=proc_cmd_devident(cmdcmdMsg,returnval,cmd.sender);break;//inProg
				case EXMPP_CMD_SHUTDOWN        :res=proc_cmd_xmpshutdown(cmdcmdMsg,returnval,cmd.sender);break;//inProg
				case EXMPP_CMD_SONOFF          :res=proc_cmd_sonoff(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_DISPCLEAR       :res=proc_cmd_disp_clear(cmdcmdMsg);break;//,returnval,cmd.sender);break;
				case EXMPP_CMD_DISPPRINT       :res=proc_cmd_disp_print(cmdcmdMsg,returnval);break;
				case EXMPP_CMD_DISPBKLT        :res=proc_cmd_disp_backlight(cmdcmdMsg,returnval);break;
				default                        :break;
			}
			result.pop_front();
			const char *resTbl[] = STR_RPC_SRV_RESULT_STRING_TABLE;
			std::string result   = resTbl[res];
			std::string response = "return="+result +" : "+"result="+returnval;
			XmppProxy.send_reply(response,cmd.sender);//result+":"+returnval);
		}
		processCmd.pop_front();//after processing delete the entry
	}
	return 0;
}
//following function is called by EvntHandler object
//after receiving the result of inProg task, this call back is called to send reply over xmpp
RPC_SRV_RESULT XmppMgr::RpcResponseCallback(RPC_SRV_RESULT taskRes,int taskID,std::string to)
{
	char tID[255];sprintf(tID,"%d",taskID);
	std::string returnval="taskID=";
	returnval+=tID;

	const char *resTbl[] = STR_RPC_SRV_RESULT_STRING_TABLE;
	std::string result   = resTbl[taskRes];
	std::string response = "return="+result +" : "+"result="+returnval;
	XmppProxy.send_reply(response,to);//result+":"+returnval);
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT XmppMgr::RpcResponseCallback(std::string taskRes,int taskID,std::string to)
{
	char tID[255];sprintf(tID,"%d",taskID);
	std::string returnval="taskID=";
	returnval+=tID;
	std::string result   = taskRes;
	std::string response = "return="+result +" : "+"result="+returnval;
	XmppProxy.send_reply(response,to);
	return RPC_SRV_RESULT_SUCCESS;
}
//RAII function, used for inserting entry or searching for existing entry
RPC_SRV_RESULT XmppMgr::AccessAsyncTaskList(int tid, int port, bool insertEntryFlag,int *xmpptID,std::string &sender)
{
	// mutex to protect cmn resource access (shared across threads)
	static std::mutex mutex;
	// lock mutex before accessing file
	std::lock_guard<std::mutex> lock(mutex);
	if(insertEntryFlag==true)
	{
		XmppTaskIDCounter++;//global task id counter for external xmpp clients
		AsyncTaskList.push_back(AyncEventEntry(tid,port,XmppTaskIDCounter,sender));
		if(xmpptID!=NULL)
			*xmpptID=XmppTaskIDCounter;
	}
	else
	{
	//cout<<"size-before="<<AsyncTaskList.size()<<endl;
	std::vector<AyncEventEntry>::iterator it = find_if(AsyncTaskList.begin(), AsyncTaskList.end(), FindAsyncEventEntry(tid,port));
	if(it == AsyncTaskList.end())
		return RPC_SRV_RESULT_FAIL;
	if(xmpptID!=NULL)
		*xmpptID=(*it).xmppTID;
	sender=(*it).to;
	//if (find_if(AsyncTaskList.begin(), AsyncTaskList.end(), FindAsyncEventEntry(tid,port)) == AsyncTaskList.end())
	//	return RPC_SRV_RESULT_FAIL;
	//erase the entry
	AsyncTaskList.erase(remove_if(AsyncTaskList.begin(), AsyncTaskList.end(), FindAsyncEventEntry(tid,port)) , AsyncTaskList.end());
	//cout<<"size-after="<<AsyncTaskList.size()<<endl;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
EXMPP_CMD_TYPES XmppMgr::ResolveCmdStr(std::string msg)
{
	stringstream mstream(msg);
	std::string cmd;
	mstream >> cmd;
	transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

	/*EXMPP_CMD_TYPES xmpcmd;
	const char *cmdTbl[] = EXMPP_CMD_TABL;
	ADCmnStringProcessor string_proc;
	//cout<<mstream.str();//remaining message
	//------------function to convert string into lowercase---------------
	transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
	//--------------------------------------------------------------------
	xmpcmd = (EXMPP_CMD_TYPES)string_proc.string_to_enum(cmdTbl,(char*)cmd.c_str(),EXMPP_CMD_UNKNOWN);
	if(xmpcmd>=EXMPP_CMD_UNKNOWN)
		xmpcmd=EXMPP_CMD_UNKNOWN;
	return xmpcmd;*/

	std::string name;
	int total_cmds=sizeof(xmproxy_cmd_table)/sizeof(XMPROXY_CMD_TABLE);
	for(int i=0;i<total_cmds;i++)
	{
		if(xmproxy_cmd_table[i].cmdsts!=true)continue;//dont support disabled commands
		name=xmproxy_cmd_table[i].cmd_name;
		if(cmd == name)
			return xmproxy_cmd_table[i].cmd;
	}
	return EXMPP_CMD_UNKNOWN;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::Start(std::string accountFilePath)
{
	if(accountFilePath.size()<=0)
		return RPC_SRV_RESULT_FAIL;

	//fill AliasList with contents of alias-file
	LoadAliasList(AliasListFile);
	LoadEventSubscrList(EventSubscrListFile,&myEventList);

	//cout<<"loginfilepath: "<<accountFilePath<<endl;
	std::ifstream file(accountFilePath.c_str());
	std::string line,key;

	//read username
	std::getline(file, line);
	if(line.size()<=0)
		return RPC_SRV_RESULT_FAIL;
	stringstream userstream(line);
	userstream >> key;            //first-word  ("user:")
	userstream >> XmppUserName;//=userstream.str();//second-word ("xyz.user@ubuntujabber.de")
	if(XmppUserName.size()<=0)
		return RPC_SRV_RESULT_FAIL;

	//read passowrd
	std::getline(file, line);
	if(line.size()<=0)
		return RPC_SRV_RESULT_FAIL;
	stringstream pwstream(line);
	pwstream >> key;           //first-word  ("pw:")
	pwstream >> XmppUserPw;//=pwstream.str();//second-word ("xyzpw")
	if(XmppUserPw.size()<=0)
		return RPC_SRV_RESULT_FAIL;

	if(DebugLog)
	{
		cout<<"XmppMgr::Start:user: "<<XmppUserName<<endl;
		cout<<"XmppMgr::Start:pw  : "<<XmppUserPw<<endl;
	}

	XmppClientThread.start_thread();
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT XmppMgr::Stop()
{
	//XmppProxy.disconnect();
	int MaxTime=0;
	XmppProxy.setForcedDisconnect();
	XmppProxy.disconnect();
	//while(XmppProxy.get_connect_sts()==true && MaxTime++<50) //max 5sec wait
	//{
	//	usleep(100000);
	//	cout<<"waiting for disconnect"<<endl;
	//}
	//XmppClientThread.stop_thread();
	XmppClientThread.stop_thread();
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT XmppMgr::set_online_status(bool status)
{
	if(status == true) //go online
		XmppProxy.setOnDemandDisconnect(false);//thread will notice this in next 5sec sampling, and then will go online
	else //go offline
	{
		XmppProxy.setOnDemandDisconnect(true);
		XmppProxy.disconnect();//force the disconnection if already online
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::SendMessage(std::string msg)
{
	//int tot;std::string message,frm;std::vector<MsgEntry> dummyList;
	//RPC_SRV_RESULT res=GetOrUpdateTotal(SMS_ACCESS_READ_INDEX,tot,index,message,frm,dummyList);
	//strcpy(msg,message.c_str());
	XmppProxy.send_reply(msg);
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_sms_deleteall(std::string msg,std::string &returnval,std::string sender)
{
	char tID[255];tID[254]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_BBOXSMS)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result=Client.set_action_noarg_get_single_string_type((char*)"delete_all_sms",(char*)"taskId",tID);
	Client.rpc_server_disconnect();
	returnval="taskID=";//returnval+=tID;
	int xmptid=-1;
	if(result==RPC_SRV_RESULT_IN_PROG)
		AccessAsyncTaskList(atoi(tID),ADCMN_PORT_BBOXSMS,true,&xmptid,sender);
	sprintf(tID,"%d",xmptid);returnval+=tID;
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_sms_delete(std::string msg)
{
	return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_sms_get(std::string msg,std::string &returnval)
{
	std::string cmd,cmdArg;
	stringstream msgstream(msg);
	msgstream >> cmd;
	msgstream >> cmdArg;
	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	if(cmdArg.size()<=0)
		return RPC_SRV_RESULT_ARG_ERROR;
	xpandarg(cmdArg); //e.g: smsget $index
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_BBOXSMS)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	int msgIndex=atoi(cmdArg.c_str());
	char returnmsg[1024];returnmsg[0]='\0';
	RPC_SRV_RESULT result=Client.get_int_type_with_string_para((char*)"get_sms",(char*)"index",msgIndex,returnmsg,(char*)"message");
	returnval=returnmsg;
	Client.rpc_server_disconnect();
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_sms_send(std::string msg,std::string &returnval,std::string sender)
{
	std::string cmd,destArg,msgArg;
	stringstream msgstream(msg);
	msgstream >> cmd;
	msgstream >> destArg;
	getline(msgstream, msgArg); //get rest of the string!
	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	if(destArg.size()<=0)
		return RPC_SRV_RESULT_ARG_ERROR;
	xpandarg(destArg); //e.g: smssend $phonenum $msg
	if(msgArg.size()<=0)
		return RPC_SRV_RESULT_ARG_ERROR;
	xpandargs(msgArg); //e.g: smssend $phonenum $msg
	char tID[255];tID[254]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_BBOXSMS)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result=Client.set_double_string_get_single_string_type((char*)"send_sms",
				(char*)"destination",(char*)destArg.c_str(),
				(char*)"message",(char*)msgArg.c_str(),
				(char*)"taskId",(char*)tID);
	Client.rpc_server_disconnect();
	returnval="taskID=";//returnval+=tID;
	int xmptid=-1;
	if(result==RPC_SRV_RESULT_IN_PROG)
		AccessAsyncTaskList(atoi(tID),ADCMN_PORT_BBOXSMS,true,&xmptid,sender);
	sprintf(tID,"%d",xmptid);returnval+=tID;
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_sms_list_update(std::string msg,std::string &returnval,std::string sender)
{
	char tID[255];tID[254]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_BBOXSMS)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result=Client.set_action_noarg_get_single_string_type((char*)"update_sms_list",(char*)"taskId",tID);
	Client.rpc_server_disconnect();
	returnval="taskID=";//returnval+=tID;
	int xmptid=-1;
	if(result==RPC_SRV_RESULT_IN_PROG)
		AccessAsyncTaskList(atoi(tID),ADCMN_PORT_BBOXSMS,true,&xmptid,sender);
	sprintf(tID,"%d",xmptid);returnval+=tID;
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_sms_get_total(std::string msg,std::string &returnval)
{
	char temp_str[255];temp_str[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_BBOXSMS)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result = Client.get_integer_type((char*)"get_total_sms",(char*)"msgcount",temp_str);
	Client.rpc_server_disconnect();
	returnval=temp_str;
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_fmw_get_version(std::string msg,std::string &returnval)
{
//01:39:17.322-->{ "jsonrpc": "2.0", "method": "get_fmw_version", "params": { "module": "current" }, "id": 0 }
//01:39:17.408<--{ "jsonrpc": "2.0", "result": { "return": "Success", "version": "00.01.00341" }, "id": 0 }
	char temp_str[255];temp_str[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result = Client.get_string_type_with_string_para((char*)"get_fmw_version",(char*)"module",(char*)"current",temp_str,(char*)"version");
	Client.rpc_server_disconnect();
	returnval=temp_str;
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_fmw_update(std::string msg,std::string &returnval,std::string sender)
{
	//00:06:05.434-->{ "jsonrpc": "2.0", "method": "download_ftp_file", "params": { "srcaddr": "http:\/\/github.com\/hackboxguy\/downloads\/raw\/master\/bbbmmcRbox\/readme.txt", "targetpath": "\/tmp\/readme.txt" }, "id": 0 }
	//00:06:05.454<--{ "jsonrpc": "2.0", "result": { "return": "InProgress", "taskId": 1 }, "id": 0 }
	//github-file-download-cmd: wget -O /tmp/uBrBox.uimg http://github.com/hackboxguy/downloads/raw/master/README.md

	std::string cmd,URLPath;//cmdArg
	stringstream msgstream(msg);
	msgstream >> cmd;
	//msgstream >> cmdArg;
	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	//if(cmdArg.size()<=0)
	//	return RPC_SRV_RESULT_ARG_ERROR;

	std::ifstream file(BRBOX_SYS_CONFIG_FILE_PATH);
	std::string line;
	std::getline(file, line);
	if(line.size()<=0)
		return RPC_SRV_RESULT_FAIL;//unable to detect system type
	line+=".uimg";//ex: raspi1-rbox.uimg

	URLPath=GITHUB_FMW_DOWNLOAD_FOLDER;//"http://github.com/hackboxguy/downloads/raw/master/" + "uBrBoxRoot.uimg"
	URLPath+=line;//cmdArg;
	//LOG_DEBUG_MSG_1_ARG(true,"BRBOX:xmproxy","XmppMgr::proc_cmd_fmw_update::cmdARG=%s",cmdArg.c_str());
	//LOG_DEBUG_MSG_1_ARG(true,"BRBOX:xmproxy","XmppMgr::proc_cmd_fmw_update::filepath=%s",URLPath.c_str());

	char temp_str[255];temp_str[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	//file download rpc call
	RPC_SRV_RESULT result = Client.set_double_string_get_single_string_type((char*)"download_ftp_file",
					(char*)"srcaddr",(char*)URLPath.c_str(),
					(char*)"targetpath",(char*)"/tmp/uBrBoxRoot.uimg",
					(char*)"taskId",temp_str);
	//Note: for caller, its one cmd, but internally there are two(download & update)
	//pushAsyncEvent ID only for update-cmd(ignore download-complete taskEvent).

	//fmw update rpc call
//01:19:27.440-->{ "jsonrpc": "2.0", "method": "firmware_update", "params": { "module": "project", "filepath": "\/tmp\/messages" }, "id": 0 }
//01:19:27.449<--{ "jsonrpc": "2.0", "result": { "return": "InProgress", "taskId": 4 }, "id": 0 }
	if(result==RPC_SRV_RESULT_IN_PROG)
	{
		result=Client.set_double_string_get_single_string_type((char*)"firmware_update",(char*)"module",(char*)"project",
									(char*)"filepath",(char*)"/tmp/uBrBoxRoot.uimg",
									(char*)"taskId",temp_str);
		LastFmwUpdateTaskID=atoi(temp_str);
		returnval="taskID=";//returnval+=temp_str;
		int xmptid=-1;
		if(result==RPC_SRV_RESULT_IN_PROG)
			AccessAsyncTaskList(atoi(temp_str),ADCMN_PORT_SYSMGR,true,&xmptid,sender);
		sprintf(temp_str,"%d",xmptid);returnval+=temp_str;
	}
	Client.rpc_server_disconnect();
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_fmw_reboot(std::string msg,std::string &returnval,std::string sender)
{
	//01:39:52.718-->{ "jsonrpc": "2.0", "method": "set_device_operation", "params": { "operation": "reboot" }, "id": 0 }
	//01:39:52.726<--{ "jsonrpc": "2.0", "result": { "return": "InProgress", "taskId": 1 }, "id": 0 }
	char temp_str[255];temp_str[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result = Client.set_single_string_get_single_string_type((char*)"set_device_operation",(char*)"operation",(char*)"reboot",(char*)"taskId",temp_str);
	Client.rpc_server_disconnect();
	returnval="taskID=";//returnval+=temp_str;
	int xmptid=-1;
	if(result==RPC_SRV_RESULT_IN_PROG)
		AccessAsyncTaskList(atoi(temp_str),ADCMN_PORT_SYSMGR,true,&xmptid,sender);
	sprintf(temp_str,"%d",xmptid);returnval+=temp_str;
	return result;
}
RPC_SRV_RESULT XmppMgr::proc_cmd_fmw_poweroff(std::string msg,std::string &returnval,std::string sender)
{
	char temp_str[255];temp_str[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result = Client.set_single_string_get_single_string_type((char*)"set_device_operation",(char*)"operation",(char*)"standby",(char*)"taskId",temp_str);
	Client.rpc_server_disconnect();
	returnval="taskID=";//returnval+=temp_str;
	int xmptid=-1;
	if(result==RPC_SRV_RESULT_IN_PROG)
		AccessAsyncTaskList(atoi(temp_str),ADCMN_PORT_SYSMGR,true,&xmptid,sender);
	sprintf(temp_str,"%d",xmptid);returnval+=temp_str;
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_fmw_update_sts(std::string msg,std::string &returnval)
{
	//00:40:37.476-->{ "jsonrpc": "2.0", "method": "get_async_task", "id": 0 }
	//00:40:37.480<--{ "jsonrpc": "2.0", "result": { "return": "Success", "task": "none" }, "id": 0 }
	char temp_str[255];temp_str[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result = Client.get_string_type((char*)"get_async_task",(char*)"task",temp_str);
	Client.rpc_server_disconnect();
	returnval=temp_str;
	if(result!=RPC_SRV_RESULT_SUCCESS)
		return result;
	if(returnval=="none")
		returnval="Idle";
	else
		returnval="Busy";
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_fmw_update_res(std::string msg,std::string &returnval)
{
	//01:02:29.609-->{ "jsonrpc": "2.0", "method": "get_rpc_req_status", "params": { "taskId": "1" }, "id": 0 }
	//01:02:29.615<--{ "jsonrpc": "2.0", "result": { "return": "Success", "taskStatus": "taskIDNotFound" }, "id": 0 }
	char tRes[255];tRes[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	char tID[255];sprintf(tID,"%d",LastFmwUpdateTaskID);
	RPC_SRV_RESULT result = Client.get_string_type_with_string_para((char*)"get_rpc_req_status",(char*)"taskId",tID,
						tRes,(char*)"taskStatus");
	Client.rpc_server_disconnect();
	returnval=tRes;
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_fmw_uptime(std::string msg,std::string &returnval)
{
//01:42:50.068-->{ "jsonrpc": "2.0", "method": "get_load_info", "id": 0 }
//01:42:50.078<--{ "jsonrpc": "2.0", "result": { "return": "Success", "current": "0", "average": "0", "uptime": "2632" }, "id": 0 }
	char str_cur[255];str_cur[0]='\0';
	char str_avg[255];str_avg[0]='\0';
	char str_uptm[255];str_uptm[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result = Client.get_three_string_type(   (char*)"get_load_info",(char*)"current",str_cur,
								(char*)"average",str_avg,(char*)"uptime",str_uptm);
	Client.rpc_server_disconnect();
	int hrs=atoi(str_uptm);hrs/=3600;
	char hrs_str[255];sprintf(hrs_str,"%d",hrs);
	//returnval=str_uptm;
	//returnval+=" Sec";
	returnval=hrs_str;
	returnval+=" Hours";
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_fmw_get_hostname(std::string msg,std::string &returnval)
{
//14:52:09.440-->{ "jsonrpc": "2.0", "method": "get_debug_outfile", "id": 0 }
//14:52:09.440<--{ "jsonrpc": "2.0", "result": { "return": "Success", "filepath": "" }, "id": 0 }
	char temp_str[255];temp_str[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result = Client.get_string_type((char*)"get_hostname",(char*)"hostname",temp_str);
	Client.rpc_server_disconnect();
	returnval=temp_str;
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_fmw_set_hostname(std::string msg)
{
//14:52:21.336-->{ "jsonrpc": "2.0", "method": "set_debug_outfile", "params": { "filepath": "\/home\/adav\/tmp" }, "id": 0 }
//14:52:21.336<--{ "jsonrpc": "2.0", "result": { "return": "Success" }, "id": 0 }
	std::string cmd,cmdArg;
	stringstream msgstream(msg);
	msgstream >> cmd;
	msgstream >> cmdArg;
	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	if(cmdArg.size()<=0)
		return RPC_SRV_RESULT_ARG_ERROR;
	//char temp_str[255];temp_str[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result = Client.set_single_string_type((char*)"set_hostname",(char*)"hostname",(char*)cmdArg.c_str());
	Client.rpc_server_disconnect();
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_fmw_hostname(std::string msg,std::string &returnval)
{
	std::string cmd,cmdArg;
	stringstream msgstream(msg);
	msgstream >> cmd;
	msgstream >> cmdArg;
	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	xpandarg(cmdArg); //e.g: hostname $myhostname
	if(cmdArg.size()<=0)//get-hostname-cmd
		return proc_cmd_fmw_get_hostname(msg,returnval);
	else
		return proc_cmd_fmw_set_hostname(msg);
}
RPC_SRV_RESULT XmppMgr::proc_cmd_gpio(std::string msg,std::string &returnval)
{
	std::string cmd,cmdArg,cmdArg2;
	stringstream msgstream(msg);
	msgstream >> cmd;
	msgstream >> cmdArg;
	msgstream >> cmdArg2;
	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	if(cmdArg.size()<=0)//get-gpio pin number
		return RPC_SRV_RESULT_ARG_ERROR;
	xpandarg(cmdArg); //e.g: gpio $num $laststate
	if(cmdArg2.size()<=0)//read gpio pin
	{
		int gpioAddr=atoi(cmdArg.c_str());
		char temp_str[255];temp_str[0]='\0';
		ADJsonRpcClient Client;
		if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_GPIOCTL)!=0)
			return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
		RPC_SRV_RESULT result = Client.get_integer_type_with_addr_para((char*)"gpio_get",(char*)"addr",gpioAddr,
										(char*)"data",temp_str);
		Client.rpc_server_disconnect();
		returnval=temp_str;
		return result;
	}
	else //write gpio pin
	{
		xpandarg(cmdArg2); //e.g: gpio $num $laststate
		int gpioAddr=atoi(cmdArg.c_str());
		int gpioVal=atoi(cmdArg2.c_str());
		ADJsonRpcClient Client;
		if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_GPIOCTL)!=0)
			return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
		RPC_SRV_RESULT result = Client.set_integer_type_with_addr_para((char*)"gpio_set",(char*)"addr",gpioAddr,
										(char*)"data",gpioVal);
		Client.rpc_server_disconnect();
		return result;
	}
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_fmw_get_myip(std::string msg,std::string &returnval)
{
//14:52:09.440-->{ "jsonrpc": "2.0", "method": "get_debug_outfile", "id": 0 }
//14:52:09.440<--{ "jsonrpc": "2.0", "result": { "return": "Success", "filepath": "" }, "id": 0 }
	char temp_str[255];temp_str[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result = Client.get_string_type((char*)"get_my_public_ip",(char*)"ip",temp_str);
	Client.rpc_server_disconnect();
	returnval=temp_str;
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_fmw_get_localip(std::string msg,std::string &returnval)
{
	char temp_str[255];temp_str[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result;
	if(XmppNetInterface=="") //if not requested via cmdlinearg, then scan for available ones
	{
		result = Client.get_string_type_with_string_para((char*)"get_ip_addr",(char*)"iface",(char*)"eth0",
										temp_str,(char*)"addr");//first try for eth0
		if(result!=RPC_SRV_RESULT_SUCCESS)
		{
			result = Client.get_string_type_with_string_para((char*)"get_ip_addr",(char*)"iface",(char*)"eth1",
										temp_str,(char*)"addr");//second try for eth1
			if(result!=RPC_SRV_RESULT_SUCCESS)
			{
			result = Client.get_string_type_with_string_para((char*)"get_ip_addr",(char*)"iface",(char*)"ppp0",
										temp_str,(char*)"addr");//third try for ppp0
			}
		}
	}
	else
	{
		result = Client.get_string_type_with_string_para((char*)"get_ip_addr",(char*)"iface",(char*)XmppNetInterface.c_str(),
										temp_str,(char*)"addr");//check the requested iface name
	}
	Client.rpc_server_disconnect();
	returnval=temp_str;
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_fmw_set_default_hostname(std::string msg)
{
//14:52:09.440-->{ "jsonrpc": "2.0", "method": "set_default_hostname", "id": 0 }
//14:52:09.440<--{ "jsonrpc": "2.0", "result": { "return": "Success"}, "id": 0 }
	char temp_str[255];temp_str[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result = Client.set_action_noarg_type((char*)"set_default_hostname");
	Client.rpc_server_disconnect();
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_dial_voice(std::string msg,std::string &returnval,char* rpc_cmd,std::string sender)
{
	std::string cmd,destArg;//,msgArg;
	stringstream msgstream(msg);
	msgstream >> cmd;
	msgstream >> destArg;
	//getline(msgstream, msgArg); //get rest of the string!
	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	if(destArg.size()<=0)
		return RPC_SRV_RESULT_ARG_ERROR;
	//if(msgArg.size()<=0)
	//	return RPC_SRV_RESULT_ARG_ERROR;
	xpandarg(destArg); //e.g: dialvoice $phonenum
	char tID[255];tID[254]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_BBOXSMS)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result=Client.set_single_string_get_single_string_type((char*)rpc_cmd,//"dial_voice",
				(char*)"destination",(char*)destArg.c_str(),
				(char*)"taskId",(char*)tID);
	Client.rpc_server_disconnect();
	returnval="taskID=";//returnval+=tID;
	int xmptid=-1;
	if(result==RPC_SRV_RESULT_IN_PROG)
		AccessAsyncTaskList(atoi(tID),ADCMN_PORT_BBOXSMS,true,&xmptid,sender);
	sprintf(tID,"%d",xmptid);returnval+=tID;
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_get_ussd(std::string msg,std::string &returnval)
{
	char temp_str[255];temp_str[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_BBOXSMS)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result = Client.get_string_type((char*)"get_ussd",(char*)"message",temp_str);
	Client.rpc_server_disconnect();
	returnval=temp_str;
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_logsts(std::string msg,std::string &returnval)
{
	std::string cmd,portArg,logArg;
	stringstream msgstream(msg);
	msgstream >> cmd;
	msgstream >> portArg;//service port-num
	msgstream >> logArg; //log flag [enable/disable]

	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	if(portArg.size()<=0)
		return RPC_SRV_RESULT_ARG_ERROR;
	if(logArg.size()<=0)
	{
		//read log flag
		char temp_str[255];temp_str[254]='\0';
		ADJsonRpcClient Client;
		if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),atoi(portArg.c_str()))!=0)
			return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
		RPC_SRV_RESULT result=Client.get_string_type((char*)RPCMGR_RPC_DEBUG_LOG_GET,
					(char*)RPCMGR_RPC_DEBUG_LOG_ARGSTS,temp_str);
		Client.rpc_server_disconnect();
		returnval=temp_str;
		return result;
	}
	else
	{
		//write log flag
		ADJsonRpcClient Client;
		if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),atoi(portArg.c_str()))!=0)
			return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
		RPC_SRV_RESULT result=Client.set_single_string_type((char*)RPCMGR_RPC_DEBUG_LOG_SET,
					(char*)RPCMGR_RPC_DEBUG_LOG_ARGSTS,(char*)logArg.c_str());
		Client.rpc_server_disconnect();
		return result;
	}
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_gsm_modem_identify(std::string msg,std::string &returnval,std::string sender)
{
	char tID[255];tID[254]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_BBOXSMS)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result=Client.set_action_noarg_get_single_string_type((char*)"identify_dev",(char*)"taskId",tID);
	Client.rpc_server_disconnect();
	returnval="taskID=";//returnval+=tID;
	int xmptid=-1;
	if(result==RPC_SRV_RESULT_IN_PROG)
		AccessAsyncTaskList(atoi(tID),ADCMN_PORT_BBOXSMS,true,&xmptid,sender);
	sprintf(tID,"%d",xmptid);returnval+=tID;
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_log_list_update(std::string msg,std::string &returnval,std::string sender)
{
	char tID[255];tID[254]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result=Client.set_action_noarg_get_single_string_type((char*)"update_loglist",(char*)"taskId",tID);
	Client.rpc_server_disconnect();
	returnval="taskID=";//returnval+=tID;
	int xmptid=-1;
	if(result==RPC_SRV_RESULT_IN_PROG)
		AccessAsyncTaskList(atoi(tID),ADCMN_PORT_SYSMGR,true,&xmptid,sender);
	sprintf(tID,"%d",xmptid);returnval+=tID;
	return result;
}
RPC_SRV_RESULT XmppMgr::proc_cmd_log_get_count(std::string msg,std::string &returnval)
{
	char temp_str[255];temp_str[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result = Client.get_integer_type((char*)"get_log_count",(char*)"logcount",temp_str);
	Client.rpc_server_disconnect();
	returnval=temp_str;
	return result;
}
RPC_SRV_RESULT XmppMgr::proc_cmd_log_get_line(std::string msg,std::string &returnval)
{
	std::string cmd,cmdArg;
	stringstream msgstream(msg);
	msgstream >> cmd;
	msgstream >> cmdArg;
	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	if(cmdArg.size()<=0)
		return RPC_SRV_RESULT_ARG_ERROR;

	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	int msgIndex=atoi(cmdArg.c_str());
	char returnmsg[1024];returnmsg[0]='\0';
	RPC_SRV_RESULT result=Client.get_int_type_with_string_para((char*)"get_log_line",(char*)"index",msgIndex,returnmsg,(char*)"logmsg");
	returnval=returnmsg;
	Client.rpc_server_disconnect();
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_event_gsm(std::string msg,std::string sender,std::string &returnval)
{
	std::string cmd,cmdArg;
	stringstream msgstream(msg);
	msgstream >> cmd;
	msgstream >> cmdArg;
	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	std::vector<EventSubscrEntry>::iterator it = find_if(myEventList.begin(), myEventList.end(), FindEventSubscrEntry(sender,0,EXMPP_EVNT_GSM));
	if(cmdArg.size()<=0) //read status
	{
		if(it != myEventList.end())
		{
			if((*it).m_Status == true)
				returnval="1";
			else
				returnval="0";
		}
		else
			returnval="0";
	}
	else //write status
	{
		if(it != myEventList.end())//found entry, modify existing entry
		{
			if(cmdArg=="1")
				(*it).m_Status=true;
			else
				(*it).m_Status=false;
			RewriteEventSubscrList(EventSubscrListFile,&myEventList);
		}
		else
		{
			bool sts=false;
			if(cmdArg=="1")
			{
				sts=true;
				myEventList.push_back(EventSubscrEntry(sender,EXMPP_EVNT_GSM,0,sts));
				ExtendEventSubscrList(EventSubscrListFile,sender,EXMPP_EVNT_GSM,0);
			}
		}
	}
	return RPC_SRV_RESULT_SUCCESS;
}

RPC_SRV_RESULT XmppMgr::proc_cmd_event_gpio(std::string msg,std::string sender,std::string &returnval)
{
	std::string cmd,cmdArg,cmdArgVal;
	stringstream msgstream(msg);
	msgstream >> cmd;
	msgstream >> cmdArg;
	msgstream >> cmdArgVal;
	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	if(cmdArg.size()<=0) //gpio number
		return RPC_SRV_RESULT_ARG_ERROR;
	xpandarg(cmdArg); //e.g: evntgpio $ionum $laststate
	int io=atoi(cmdArg.c_str());//gpio number
	std::vector<EventSubscrEntry>::iterator it = find_if(myEventList.begin(), myEventList.end(), FindEventSubscrEntry(sender,io,EXMPP_EVNT_GPIO));
	if(cmdArgVal.size()<=0) //read status
	{
		if(it != myEventList.end())
		{
			if((*it).m_Status == true)
				returnval="1";
			else
				returnval="0";
		}
		else
			returnval="0";
	}
	else //write status
	{
		xpandarg(cmdArgVal); //e.g: evntgpio $ionum $laststate
		if(it != myEventList.end())//found entry, modify existing entry
		{
			if(cmdArgVal=="1")
				(*it).m_Status=true;
			else
				(*it).m_Status=false;
			RewriteEventSubscrList(EventSubscrListFile,&myEventList);
		}
		else
		{
			bool sts=false;
			if(cmdArgVal=="1")
			{
				sts=true;
				myEventList.push_back(EventSubscrEntry(sender,EXMPP_EVNT_GPIO,io,sts));
				ExtendEventSubscrList(EventSubscrListFile,sender,EXMPP_EVNT_GPIO,io);
			}
		}
	}
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT XmppMgr::LoadEventSubscrList(std::string listFile,std::vector<EventSubscrEntry> *pList)
{
	if(listFile.size()<=0)
		return RPC_SRV_RESULT_FAIL;
	std::ifstream infile(listFile);
	std::string line;
	while (std::getline(infile, line))
	{
		stringstream iss(line);
		std::string addr,type,arg,sts;
		iss >> addr;
		iss >> type;
		iss >> arg;
		int io=atoi(arg.c_str());//gpio number
		const char *table[]   = EXMPP_EVNT_TYPES_TABL;
		if(type==table[EXMPP_EVNT_GSM])
			pList->push_back(EventSubscrEntry(addr,EXMPP_EVNT_GSM,io,true));
		else if(type==table[EXMPP_EVNT_GPIO])
			pList->push_back(EventSubscrEntry(addr,EXMPP_EVNT_GPIO,io,true));
		//else, dont know the type, so dont fill this record to list
	}
	//infile.close();
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT XmppMgr::ExtendEventSubscrList(std::string listFile,std::string addr,EXMPP_EVNT_TYPES type,int arg)
{
	if(listFile.size()<=0)
		return RPC_SRV_RESULT_FAIL;
	std::ofstream outfile;
	outfile.open(listFile, std::ios::app);
	std::string Typ,Arg;
	const char *table[]   = EXMPP_EVNT_TYPES_TABL;
	Typ=table[type];
	Arg = std::to_string(arg);
	outfile <<addr<<" "<<Typ<<" "<<Arg<<endl;
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT XmppMgr::RewriteEventSubscrList(std::string listFile,std::vector<EventSubscrEntry> *pList)
{
	if(listFile.size()<=0)
		return RPC_SRV_RESULT_FAIL;
	std::ofstream outfile;
	outfile.open(listFile, std::ofstream::out | std::ofstream::trunc);
	for( std::vector<EventSubscrEntry>::iterator it = pList->begin(); it != pList->end(); ++it)
	{
		if((*it).m_Status==true)
		{
			std::string addr,typ,arg;
			addr=(*it).subscriber;
			const char *table[]   = EXMPP_EVNT_TYPES_TABL;
			typ=table[(*it).m_EvntType];
			arg = std::to_string((*it).m_EvntArg);
			outfile <<addr<<" "<<typ<<" "<<arg<<endl;
		}
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_alias(std::string msg,std::string &returnval)
{
	//cout<<"msg="<<msg<<endl;
	//msg "Alias Light ON=Gpio 2 0"
	transform(msg.begin(), msg.end(), msg.begin(), ::tolower);//convert all lower case
	if(msg=="alias")//if it is just the "alias" print the content of AliasList
	{
		returnval='\n';//"";
		for( Alias::iterator it = AliasList.begin(); it != AliasList.end(); ++it)
		{
			std::string line=it->first+"="+it->second;
			returnval+=line;
			returnval+='\n';
		}
		return RPC_SRV_RESULT_SUCCESS;
	}
	std::string cmd,value="";
	stringstream msgstream(msg);
	getline(msgstream,cmd,'=');//"alias light on"
	getline(msgstream,value,'=');//"gpio 2 0"

	std::string name,key;
	stringstream alias(cmd);
	alias>>name;//alias
	getline(alias,key);
	key.erase(0, key.find_first_not_of(" \t\n\r\f\v"));//remove leading whitespace " light on"
	if(value=="")//request for deleting an alias from list
	{
		Alias::iterator it = AliasList.find(key);
		if (it != AliasList.end())
			AliasList.erase(it);
		//re-write alias-file with contents of AliasList
		RewriteAliasList(AliasListFile);
		return RPC_SRV_RESULT_SUCCESS;
	}
	//cout<<"key  ="<<key<<endl;//"light on"
	//cout<<"value="<<value<<endl;//"gpio 2 0"
	Alias::iterator it = AliasList.find(key);
	if (it != AliasList.end())
	{
		AliasList.erase(it);//erase existing
		AliasList[key] = value;//add new
		RewriteAliasList(AliasListFile);//update list file
	}
	else
	{
		//add new alias to list
		AliasList[key] = value;
		//extend alias-file with contents of AliasList
		ExtendAliasList(AliasListFile,key,value);
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
#include <unistd.h>
RPC_SRV_RESULT XmppMgr::proc_cmd_sleep(std::string msg)
{
	std::string cmd,cmdArg;
	stringstream msgstream(msg);
	msgstream >> cmd;
	msgstream >> cmdArg;
	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	if(cmdArg.size()<=0)
		return RPC_SRV_RESULT_ARG_ERROR;
	xpandarg(cmdArg); //e.g: sleep $timesec
	int sec=atoi(cmdArg.c_str());
	usleep(sec *1000 * 1000);
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::LoadAliasList(std::string listFile)
{
	if(listFile.size()<=0)
		return RPC_SRV_RESULT_FAIL;
	std::ifstream infile(listFile);
	std::string line;
	while (std::getline(infile, line))
	{
		stringstream iss(line);std::string cmd,value;
		getline(iss,cmd,'=');//"alias light on"
		getline(iss,value,'=');//"gpio 2 0"
		AliasList[cmd] = value;
	}
	//infile.close();
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT XmppMgr::ExtendAliasList(std::string listFile,std::string key,std::string val)
{
	if(listFile.size()<=0)
		return RPC_SRV_RESULT_FAIL;
	std::ofstream outfile;
	outfile.open(listFile, std::ios::app);
	outfile <<key<<"="<<val<<endl;
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT XmppMgr::RewriteAliasList(std::string listFile)
{
	if(listFile.size()<=0)
		return RPC_SRV_RESULT_FAIL;
	std::ofstream outfile;
	outfile.open(listFile, std::ofstream::out | std::ofstream::trunc);
	for( Alias::iterator it = AliasList.begin(); it != AliasList.end(); ++it)
		outfile <<it->first<<"="<<it->second<<endl;
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_account_name(std::string msg,std::string &returnval)
{
	if(XmppUserName.size()<=0)
		returnval="none";
	else
		returnval=XmppUserName;
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_bot_name(std::string msg,std::string &returnval)
{
	std::string cmd,cmdArg;
	stringstream msgstream(msg);
	msgstream >> cmd;
	getline(msgstream, cmdArg); //get rest of the string!
	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	if(cmdArg.size()<=0)//get-hostname-cmd
	{
		if(XmppBotName.size()<=0)
			returnval="none";
		else
			returnval=XmppBotName;
	}
	else
	{
		xpandarg(cmdArg); //e.g: botname $name
		XmppBotName=cmdArg;
		if(XmppBotNameFilePath.size()>0)
		{
			std::ofstream outfile;
			outfile.open(XmppBotNameFilePath, std::ofstream::out | std::ofstream::trunc);
				outfile<<XmppBotName<<endl;
		}
	}
	return RPC_SRV_RESULT_SUCCESS;
}
void XmppMgr::SetBotNameFilePath(std::string filepath)
{
	XmppBotNameFilePath=filepath;
	if(XmppBotNameFilePath.size()>0)
	{
		std::ifstream infile(XmppBotNameFilePath);
		std::getline(infile, XmppBotName);
	}
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_buddy_list(std::string msg,std::string &returnval)
{
	returnval='\n';
	XmppProxy.get_buddy_list(returnval);
	//for( Alias::iterator it = AliasList.begin(); it != AliasList.end(); ++it)
	//{
	//	std::string line=it->first+"="+it->second;
	//	returnval+=line;
	//	returnval+='\n';
	//}
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::GpioEventCallback(int evntNum,int evntArg)
{
	char msg[255];
	sprintf(msg,"eventgpio %d",evntArg);//prepare event message
	std::string evntStr(msg);
	std::vector<EventSubscrEntry>::iterator it;
	for(it = myEventList.begin(); it != myEventList.end(); it++)
	{
		EventSubscrEntry pEntry = (*it);
		if(pEntry.m_EvntType == EXMPP_EVNT_GPIO && pEntry.m_EvntArg == evntArg && pEntry.m_Status==true)
			XmppProxy.SendMessageToBuddy(pEntry.subscriber,evntStr,"event");
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_shellcmd(std::string msg,std::string &returnval,std::string sender,EXMPP_CMD_TYPES cmdtype)
{
	std::string cmd,destArg;//,msgArg;
	stringstream msgstream(msg);
	msgstream >> cmd;
	//msgstream >> destArg;
	getline(msgstream, destArg); //get rest of the string!

	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	if(destArg.size()<=0)
		return RPC_SRV_RESULT_ARG_ERROR;

	xpandargs(destArg); //expand argarray if any of them have $ prefix

	char tID[255];tID[254]='\0';
	char rpccmd[512];
	//send shell command to system-manager service
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	if(cmdtype==EXMPP_CMD_SHELLCMD_TRIG)
		sprintf(rpccmd,"run_shellcmdtrig");
	else
		sprintf(rpccmd,"run_shellcmd");

	RPC_SRV_RESULT result=Client.set_single_string_get_single_string_type(rpccmd,
				(char*)"command",(char*)destArg.c_str(),
				(char*)"taskId",(char*)tID);
	Client.rpc_server_disconnect();

	returnval="taskID=";
	int xmptid=-1;
	if(result==RPC_SRV_RESULT_IN_PROG)
		AccessAsyncTaskList(atoi(tID),ADCMN_PORT_SYSMGR,true,&xmptid,sender);
	sprintf(tID,"%d",xmptid);returnval+=tID;
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_shellcmdresp(std::string msg,std::string &returnval,std::string sender)
{
	//returnval='\n';
	//XmppProxy.get_buddy_list(returnval);
	std::string filename(SHELLCMD_RESP_FILE_PATH);//SHELLCMD_RESP_FILE_PATH declared in ADCommon.hpp
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		unsigned int length = in.tellg();
		if(length>SHELLCMD_RESP_MAX_FILE_SIZE)//SHELLCMD_RESP_MAX_FILE_SIZE declared in ADCommon.hpp
			return RPC_SRV_RESULT_VALUE_OUT_OF_RANGE;
		returnval.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&returnval[0], returnval.size());
		in.close();
		return RPC_SRV_RESULT_SUCCESS;
	}
	else
		return RPC_SRV_RESULT_FILE_OPEN_ERR;
	//throw(errno);
	//return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_devident(std::string msg,std::string &returnval,std::string sender)
{
	char tID[255];tID[254]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result=Client.set_action_noarg_get_single_string_type((char*)"device_identify",(char*)"taskId",tID);
	Client.rpc_server_disconnect();
	returnval="taskID=";//returnval+=tID;
	int xmptid=-1;
	if(result==RPC_SRV_RESULT_IN_PROG)
		AccessAsyncTaskList(atoi(tID),ADCMN_PORT_SYSMGR,true,&xmptid,sender);
	sprintf(tID,"%d",xmptid);returnval+=tID;
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_xmpshutdown(std::string msg,std::string &returnval,std::string sender)
{
	//send shutdown rpc call to self.
	char tID[255];tID[254]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_XMPROXY)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result=Client.set_action_noarg_get_single_string_type((char*)"trigger_shutdown",(char*)"taskId",tID);
	Client.rpc_server_disconnect();
	returnval="taskID=";
	int xmptid=-1;
	if(result==RPC_SRV_RESULT_IN_PROG)
		AccessAsyncTaskList(atoi(tID),ADCMN_PORT_XMPROXY,true,&xmptid,sender);
	sprintf(tID,"%d",xmptid);returnval+=tID;
	return result;
}
/* ------------------------------------------------------------------------- */
#include "ADSonOffHttpClient.hpp"
//sonoff 192.168.1.2 on     - ip/write/on
//sonoff 192.168.1.2 off    - ip/write/off
//sonoff 192.168.1.2 toggle - ip/write/toggle
//sonoff sonoff-001 toggle  - hostname/write/toggle
//sonoff sonoff-001         - hostname/read
//sonoff 192.168.1.2        - ip/read
RPC_SRV_RESULT XmppMgr::proc_cmd_sonoff(std::string msg,std::string &returnval)
{
	RPC_SRV_RESULT result;
	std::string cmd,cmdArg,cmdArg2;
	stringstream msgstream(msg);
	msgstream >> cmd;
	msgstream >> cmdArg;
	msgstream >> cmdArg2;
	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	if(cmdArg.size()<=0)//get sonoff host/ip string
		return RPC_SRV_RESULT_ARG_ERROR;
	xpandarg(cmdArg); //e.g: sonoff $ip on
	//if(cmdArg.at(0) == '$')
	//{
	//	cmdArg.erase(0,1);//remote first $ char
	//	transform(cmdArg.begin(), cmdArg.end(), cmdArg.begin(), ::tolower);
	//	Alias::iterator it = AliasList.find(cmdArg);
	//	if (it != AliasList.end())
	//		cmdArg=it->second;
	//}
	if(cmdArg2.size()<=0)//read sonoff
	{
		char ip[128];SONOFF_STATE state;
		//resolve hostname/ip addr to ip
		if(hostname_to_ip((char*)cmdArg.c_str() ,ip)!=RPC_SRV_RESULT_SUCCESS)
			return RPC_SRV_RESULT_FAIL;
		char temp_str[255];temp_str[0]='\0';
		ADSonOffHttpClient Client;
		if(Client.rpc_server_connect(ip,80)!=0)
			return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
		result=Client.get_sonoff_state(state);
		Client.rpc_server_disconnect();
		if(state==SONOFF_STATE_ON)
			returnval="on";//temp_str;
		else if (state==SONOFF_STATE_OFF)
			returnval="off";
		else
			returnval="unknown";
		return result;
	}
	else //write sonoff
	{
		xpandarg(cmdArg2); //e.g: sonoff $ip $laststate
		char ip[128];
		ADSonOffHttpClient Client;
		//resolve hostname/ip addr to ip
		if(hostname_to_ip((char*)cmdArg.c_str() ,ip)!=RPC_SRV_RESULT_SUCCESS)
			return RPC_SRV_RESULT_FAIL;
		if(cmdArg2=="on")
		{
			if(Client.rpc_server_connect(ip,80)!=0)
				return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
			result=Client.set_sonoff_state(SONOFF_STATE_ON);
		}
		else if (cmdArg2=="off")
		{
			if(Client.rpc_server_connect(ip,80)!=0)
				return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
			result=Client.set_sonoff_state(SONOFF_STATE_OFF);
		}
		else if (cmdArg2=="toggle")
		{
			if(Client.rpc_server_connect(ip,80)!=0)
				return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
			result=Client.set_sonoff_toggle();
		}
		else
			return RPC_SRV_RESULT_ARG_ERROR;

		Client.rpc_server_disconnect();
		return result;
	}
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::hostname_to_ip(char * hostname , char* ip)
{
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
	if ( (he = gethostbyname( hostname ) ) == NULL)
		return RPC_SRV_RESULT_FAIL;
	addr_list = (struct in_addr **) he->h_addr_list;
	for(i = 0; addr_list[i] != NULL; i++)
	{
		//Return the first one;
		strcpy(ip , inet_ntoa(*addr_list[i]) );
		return RPC_SRV_RESULT_SUCCESS;
	}
	return RPC_SRV_RESULT_FAIL;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_disp_clear(std::string msg)
{
	char temp_str[255];temp_str[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result = Client.set_action_noarg_type((char*)"display_clear");
	Client.rpc_server_disconnect();
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_disp_print(std::string msg,std::string &returnval)
{
	std::string cmd,cmdArg,cmdArg2;
	stringstream msgstream(msg);
	msgstream >> cmd;
	msgstream >> cmdArg;
	getline(msgstream, cmdArg2); //get rest of the string!

	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	if(cmdArg.size()<=0)//get-line number
		return RPC_SRV_RESULT_ARG_ERROR;
	xpandarg(cmdArg); //e.g: display $line $msg
	if(cmdArg2.size()<=0)//read printed message
	{
		int gpioAddr=atoi(cmdArg.c_str());
		char temp_str[255];temp_str[0]='\0';
		ADJsonRpcClient Client;
		if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)//ADCMN_PORT_DISPSRV
			return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
		RPC_SRV_RESULT result = Client.get_integer_type_with_addr_para((char*)"display_print",(char*)"line",gpioAddr,
										(char*)"msg",temp_str);
		Client.rpc_server_disconnect();
		returnval=temp_str;
		return result;
	}
	else //write gpio pin
	{
		xpandarg(cmdArg2); //e.g: display $line $msg
		ADJsonRpcClient Client;
		if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)//ADCMN_PORT_DISPSRV
			return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
		RPC_SRV_RESULT result = Client.set_double_string_type((char*)"display_print",(char*)"line",
						(char*)cmdArg.c_str(),(char*)"msg",(char*)cmdArg2.c_str());
		Client.rpc_server_disconnect();
		return result;
	}
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_disp_backlight(std::string msg,std::string &returnval)
{
	std::string cmd,cmdArg;
	stringstream msgstream(msg);
	msgstream >> cmd;
	msgstream >> cmdArg;
	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	if(cmdArg.size()<=0)//get-hostname-cmd
		return proc_cmd_get_display_backlight(msg,returnval);
	else
		return proc_cmd_set_display_backlight(msg);
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_get_display_backlight(std::string msg,std::string &returnval)
{
//14:52:09.440-->{ "jsonrpc": "2.0", "method": "display_backlight_get", "id": 0 }
//14:52:09.440<--{ "jsonrpc": "2.0", "result": { "return": "Success", "status": "on" }, "id": 0 }
	char temp_str[255];temp_str[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result = Client.get_string_type((char*)"display_backlight_get",(char*)"status",temp_str);
	Client.rpc_server_disconnect();
	returnval=temp_str;
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_set_display_backlight(std::string msg)
{
//14:52:21.336-->{ "jsonrpc": "2.0", "method": "display_backlight_set", "params": { "status": "off" }, "id": 0 }
//14:52:21.336<--{ "jsonrpc": "2.0", "result": { "return": "Success" }, "id": 0 }
	std::string cmd,cmdArg;
	stringstream msgstream(msg);
	msgstream >> cmd;
	msgstream >> cmdArg;
	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	if(cmdArg.size()<=0)
		return RPC_SRV_RESULT_ARG_ERROR;
	xpandarg(cmdArg); //e.g: dispbklt $sts
	//char temp_str[255];temp_str[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result = Client.set_single_string_type((char*)"display_backlight_set",(char*)"status",(char*)cmdArg.c_str());
	Client.rpc_server_disconnect();
	return result;
}
/* ------------------------------------------------------------------------- */
//removes first $ sign and expands the argument with alias if found
RPC_SRV_RESULT XmppMgr::xpandarg(std::string &cmdArg)
{
	if(cmdArg.at(0) == '$')
	{
		cmdArg.erase(0,1);//remote first $ char
		transform(cmdArg.begin(), cmdArg.end(), cmdArg.begin(), ::tolower);
		Alias::iterator it = AliasList.find(cmdArg);
		if (it != AliasList.end())
			cmdArg=it->second;
	}
	RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
//split strings by space, expand string if prefixed with  $ sign and merge back the string
RPC_SRV_RESULT XmppMgr::xpandargs(std::string &cmdArg)
{
	//cout <<"before: "<< cmdArg <<endl;
    std::string buf;                 // Have a buffer string
    std::stringstream ss(cmdArg);    // Insert the string into a stream
    std::vector<std::string> tokens; // Create vector to hold our words
    while (ss >> buf)
        tokens.push_back(buf);
	for( std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		if((*it).at(0) == '$')
		{
			(*it).erase(0,1);//remote first $ char
			transform((*it).begin(), (*it).end(), (*it).begin(), ::tolower);
			Alias::iterator myit = AliasList.find((*it));
			if (myit != AliasList.end())
				(*it)=myit->second;
		}
	}
	std::ostringstream result_stream;
	std::ostream_iterator < std::string > oit( result_stream," " );
	std::copy( tokens.begin(), tokens.end(), oit );
	cmdArg = result_stream.str();//merge strings back with space as saparator
	//cout <<"after: "<< cmdArg <<endl;
	RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */