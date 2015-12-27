#include "XmppMgr.h"
#include <algorithm>
//#include <string>
#include <mutex>
//#include <iostream>
//#include <fstream>
//#include <stdexcept>
//#include <string.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "ADCmnStringProcessor.hpp"
#include "ADJsonRpcClient.hpp"
#include "ADCmnPortList.h"
#include "ADJsonRpcMgr.hpp"
using namespace std;
/* ------------------------------------------------------------------------- */
//supported commands over xmpp
XMPROXY_CMD_TABLE xmproxy_cmd_table[] = //EXMPP_CMD_NONE+1] = 
{
	//rpc calls common to all services
	{true ,EXMPP_CMD_GSM_MODEM_IDENT         , "gsmcheck"     ,""},
	{true ,EXMPP_CMD_SMS_LIST_UPDATE         , "smsupdate"    ,""},
	{true ,EXMPP_CMD_SMS_GET_TOTAL           , "smstotal"     ,""},
	{true ,EXMPP_CMD_SMS_GET                 , "smsget"       ,"<zero_index_msg>"},
	{true ,EXMPP_CMD_SMS_DELETE_ALL          , "smsdeleteall" ,""},
	{true ,EXMPP_CMD_SMS_SEND                , "smssend"      ,"<phone-num> <msg>"},
	{true ,EXMPP_CMD_DIAL_VOICE              , "dialvoice"    ,"<phone-num>"},
	{true ,EXMPP_CMD_DIAL_USSD               , "dialussd"     ,"<ussd-code>"},
	{true ,EXMPP_CMD_GET_USSD                , "readussd"     ,""},
	{true ,EXMPP_CMD_FMW_GET_VERSION         , "fmwver"       ,""},
	{true ,EXMPP_CMD_FMW_UPDATE              , "fmwupdt"      ,"<filename>"},
	{true ,EXMPP_CMD_FMW_REBOOT              , "reboot"       ,""},
	{true ,EXMPP_CMD_FMW_UPTIME              , "uptime"       ,""},
	{true ,EXMPP_CMD_FMW_HOSTNAME            , "hostname"     ,"[name]"},
	{true ,EXMPP_CMD_FMW_RESET_HOSTNAME      , "resethostname",""},
	{true ,EXMPP_CMD_FMW_GET_MYIP            , "publicip"     ,""},
	{false,EXMPP_CMD_FMW_GET_LOCALIP         , "localip"      ,""},
	{true ,EXMPP_CMD_DEBUG_LOG_STS           , "logsts"       ,""},
	{true ,EXMPP_CMD_LOG_UPDATE              , "logupdate"    ,""},
	{true ,EXMPP_CMD_LOG_COUNT               , "logcount"     ,""},
	{true ,EXMPP_CMD_LOG_MSG                 , "logmsg"       ,"<zero_index_lineNum>"}
};
/* ------------------------------------------------------------------------- */
XmppMgr::XmppMgr() //:AckToken(0)
{
	CyclicTime_ms=CLIENT_ALIVE_PING_DURATION_MS;//60000;//60seconds
	event_period_ms=0;
	heartbeat_ms=100;
	pMyTimer=NULL;
	LastFmwUpdateTaskID=0;

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
int XmppMgr::onXmppMessage(std::string msg,ADXmppProducer* pObj)
{
	//process the messages
	//cout<<"msg arrived: "<<msg<<endl;
	if(msg=="Echo" || msg=="echo") //for checking if client is alive
		XmppProxy.send_reply(msg);
	else if(msg=="Help" || msg=="help") //print help
		XmppProxy.send_reply(print_help());
	else
	{
		processCmd.push_back(XmppCmdEntry(msg));
		XmppCmdProcessThread.wakeup_thread();//tell the worker to start working
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int XmppMgr::thread_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	//cout<<"going to connect"<<endl;
	while(1)
	{
		//cout<<"XmppMgr::thread_callback_function: entering xmpp connect"<<endl;
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
	//std::string returnval="";
	//RPC_SRV_RESULT res=RPC_SRV_RESULT_UNKNOWN_COMMAND;//RPC_SRV_RESULT_FAIL;
	while (!processCmd.empty())
	{
		//TODO: handle semicolon separated multiple commands
		XmppCmdEntry cmd = processCmd.front();

		stringstream ss(cmd.cmdMsg);
		std::deque<string> result;
		while( ss.good() )
		{
		    string substr;
		    getline( ss, substr, ';' );
		    result.push_back( substr );
		}
		while (!result.empty()) 
		{
			//std::cout << "The first character is: " << result.front() << '\n';
			//switch(ResolveCmdStr(cmd.cmdMsg))
			std::string returnval="";
			RPC_SRV_RESULT res=RPC_SRV_RESULT_UNKNOWN_COMMAND;//RPC_SRV_RESULT_FAIL;

			switch(ResolveCmdStr(result.front()))
			{
				case EXMPP_CMD_SMS_DELETE_ALL  :res=proc_cmd_sms_deleteall(cmd.cmdMsg,returnval);break;//inProg
				case EXMPP_CMD_SMS_DELETE      :res=proc_cmd_sms_delete(cmd.cmdMsg);break;
				case EXMPP_CMD_SMS_GET         :res=proc_cmd_sms_get(cmd.cmdMsg,returnval);break;
				case EXMPP_CMD_SMS_SEND        :res=proc_cmd_sms_send(cmd.cmdMsg,returnval);break;
				case EXMPP_CMD_SMS_LIST_UPDATE :res=proc_cmd_sms_list_update(cmd.cmdMsg,returnval);break;//inProg
				case EXMPP_CMD_SMS_GET_TOTAL   :res=proc_cmd_sms_get_total(cmd.cmdMsg,returnval);break;
				case EXMPP_CMD_FMW_GET_VERSION :res=proc_cmd_fmw_get_version(cmd.cmdMsg,returnval);break;
				case EXMPP_CMD_FMW_UPDATE      :res=proc_cmd_fmw_update(cmd.cmdMsg,returnval);break;//inProg
				case EXMPP_CMD_FMW_UPDATE_STS  :res=proc_cmd_fmw_update_sts(cmd.cmdMsg,returnval);break;
				case EXMPP_CMD_FMW_UPDATE_RES  :res=proc_cmd_fmw_update_res(cmd.cmdMsg,returnval);break;
				case EXMPP_CMD_FMW_REBOOT      :res=proc_cmd_fmw_reboot(cmd.cmdMsg,returnval);break;//inProg
				case EXMPP_CMD_FMW_UPTIME      :res=proc_cmd_fmw_uptime(cmd.cmdMsg,returnval);break;
				case EXMPP_CMD_FMW_HOSTNAME    :res=proc_cmd_fmw_hostname(cmd.cmdMsg,returnval);break;
				case EXMPP_CMD_FMW_GET_MYIP    :res=proc_cmd_fmw_get_myip(cmd.cmdMsg,returnval);break;
				case EXMPP_CMD_FMW_RESET_HOSTNAME:res=proc_cmd_fmw_set_default_hostname(cmd.cmdMsg);break;
				case EXMPP_CMD_DIAL_VOICE      :res=proc_cmd_dial_voice(cmd.cmdMsg,returnval,(char*)"dial_voice");break;
				case EXMPP_CMD_DIAL_USSD       :res=proc_cmd_dial_voice(cmd.cmdMsg,returnval,(char*)"dial_ussd");break;
				case EXMPP_CMD_GET_USSD        :res=proc_cmd_get_ussd(cmd.cmdMsg,returnval);break;
				case EXMPP_CMD_DEBUG_LOG_STS   :res=proc_cmd_logsts(cmd.cmdMsg,returnval);break;
				case EXMPP_CMD_GSM_MODEM_IDENT :res=proc_cmd_gsm_modem_identify(cmd.cmdMsg,returnval);break;//inProg
				case EXMPP_CMD_LOG_UPDATE      :res=proc_cmd_log_list_update(cmd.cmdMsg,returnval);break;//inProg
				case EXMPP_CMD_LOG_COUNT       :res=proc_cmd_log_get_count(cmd.cmdMsg,returnval);break;
				case EXMPP_CMD_LOG_MSG         :res=proc_cmd_log_get_line(cmd.cmdMsg,returnval);break;
				default                        :break;
			}
			result.pop_front();
			const char *resTbl[] = STR_RPC_SRV_RESULT_STRING_TABLE;
			std::string result   = resTbl[res];
			std::string response = result +" : "+returnval;
			XmppProxy.send_reply(response);//result+":"+returnval);
		}
		processCmd.pop_front();//after processing delete the entry
	}
	//const char *resTbl[] = STR_RPC_SRV_RESULT_STRING_TABLE;
	//std::string result   = resTbl[res];
	//std::string response = result +" : "+returnval;
	//XmppProxy.send_reply(response);//result+":"+returnval);
	return 0;
}
//following function is called by EvntHandler object
//after receiving the result of inProg task, this call back is called to send reply over xmpp
RPC_SRV_RESULT XmppMgr::RpcResponseCallback(RPC_SRV_RESULT taskRes,int taskID)
{
	char tID[255];sprintf(tID,"%d",taskID);
	std::string returnval="taskID=";
	returnval+=tID;

	const char *resTbl[] = STR_RPC_SRV_RESULT_STRING_TABLE;
	std::string result   = resTbl[taskRes];
	std::string response = result +" : "+returnval;
	XmppProxy.send_reply(response);//result+":"+returnval);
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT XmppMgr::RpcResponseCallback(std::string taskRes,int taskID)
{
	char tID[255];sprintf(tID,"%d",taskID);
	std::string returnval="taskID=";
	returnval+=tID;
	std::string result   = taskRes;
	std::string response = result +" : "+returnval;
	XmppProxy.send_reply(response);
	return RPC_SRV_RESULT_SUCCESS;
}
/*RPC_SRV_RESULT XmppMgr::IsItMyAsyncTaskResp(int tid,int port)
{
	if (find_if(AsyncTaskList.begin(), AsyncTaskList.end(), FindAsyncEventEntry(tid,port)) == AsyncTaskList.end())
		return RPC_SRV_RESULT_FAIL;
	//erase the entry
	AsyncTaskList.erase(remove_if(AsyncTaskList.begin(), AsyncTaskList.end(), FindAsyncEventEntry(tid,port)) , AsyncTaskList.end());
	return RPC_SRV_RESULT_SUCCESS;
}*/
//RAII function, used for inserting entry or searching for existing entry
RPC_SRV_RESULT XmppMgr::AccessAsyncTaskList(int tid, int port, bool insertEntryFlag) 
{
	// mutex to protect cmn resource access (shared across threads)
	static std::mutex mutex;
	// lock mutex before accessing file
	std::lock_guard<std::mutex> lock(mutex);
	if(insertEntryFlag==true)
	{
		AsyncTaskList.push_back(AyncEventEntry(tid,port));
	}
	else
	{
	if (find_if(AsyncTaskList.begin(), AsyncTaskList.end(), FindAsyncEventEntry(tid,port)) == AsyncTaskList.end())
		return RPC_SRV_RESULT_FAIL;
	//erase the entry
	AsyncTaskList.erase(remove_if(AsyncTaskList.begin(), AsyncTaskList.end(), FindAsyncEventEntry(tid,port)) , AsyncTaskList.end());
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
RPC_SRV_RESULT XmppMgr::proc_cmd_sms_deleteall(std::string msg,std::string &returnval)
{
	char tID[255];tID[254]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_BBOXSMS)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result=Client.set_action_noarg_get_single_string_type((char*)"delete_all_sms",(char*)"taskId",tID);
	Client.rpc_server_disconnect();
	returnval="taskID=";returnval+=tID;
	if(result==RPC_SRV_RESULT_IN_PROG)
		//AsyncTaskList.push_back(AyncEventEntry(atoi(tID),ADCMN_PORT_BBOXSMS));
		AccessAsyncTaskList(atoi(tID),ADCMN_PORT_BBOXSMS);
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
RPC_SRV_RESULT XmppMgr::proc_cmd_sms_send(std::string msg,std::string &returnval)
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
	if(msgArg.size()<=0)
		return RPC_SRV_RESULT_ARG_ERROR;

	char tID[255];tID[254]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_BBOXSMS)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result=Client.set_double_string_get_single_string_type((char*)"send_sms",
				(char*)"destination",(char*)destArg.c_str(),
				(char*)"message",(char*)msgArg.c_str(),
				(char*)"taskId",(char*)tID);
	Client.rpc_server_disconnect();
	returnval="taskID=";returnval+=tID;
	if(result==RPC_SRV_RESULT_IN_PROG)
		//AsyncTaskList.push_back(AyncEventEntry(atoi(tID),ADCMN_PORT_BBOXSMS));
		AccessAsyncTaskList(atoi(tID),ADCMN_PORT_BBOXSMS);
	return result;
	//return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_sms_list_update(std::string msg,std::string &returnval)
{
	char tID[255];tID[254]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_BBOXSMS)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result=Client.set_action_noarg_get_single_string_type((char*)"update_sms_list",(char*)"taskId",tID);
	Client.rpc_server_disconnect();
	returnval="taskID=";returnval+=tID;
	if(result==RPC_SRV_RESULT_IN_PROG)
		//AsyncTaskList.push_back(AyncEventEntry(atoi(tID),ADCMN_PORT_BBOXSMS));
		AccessAsyncTaskList(atoi(tID),ADCMN_PORT_BBOXSMS);
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
RPC_SRV_RESULT XmppMgr::proc_cmd_fmw_update(std::string msg,std::string &returnval)
{
	//00:06:05.434-->{ "jsonrpc": "2.0", "method": "download_ftp_file", "params": { "srcaddr": "http:\/\/github.com\/hackboxguy\/downloads\/raw\/master\/bbbmmcRbox\/readme.txt", "targetpath": "\/tmp\/readme.txt" }, "id": 0 }
	//00:06:05.454<--{ "jsonrpc": "2.0", "result": { "return": "InProgress", "taskId": 1 }, "id": 0 }
	//github-file-download-cmd: wget -O /tmp/uBrBox.uimg http://github.com/hackboxguy/downloads/raw/master/README.md

	std::string cmd,cmdArg,URLPath;
	stringstream msgstream(msg);
	msgstream >> cmd;
	msgstream >> cmdArg;
	if(cmd.size()<=0)
		return RPC_SRV_RESULT_UNKNOWN_COMMAND;
	if(cmdArg.size()<=0)
		return RPC_SRV_RESULT_ARG_ERROR;

	URLPath=GITHUB_FMW_DOWNLOAD_FOLDER;//"http://github.com/hackboxguy/downloads/raw/master/" + "uBrBoxRoot.uimg"
	URLPath+=cmdArg;
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
		returnval="taskID=";returnval+=temp_str;
		if(result==RPC_SRV_RESULT_IN_PROG)
			//AsyncTaskList.push_back(AyncEventEntry(atoi(temp_str),ADCMN_PORT_SYSMGR));
			AccessAsyncTaskList(atoi(temp_str),ADCMN_PORT_SYSMGR);
	}
	Client.rpc_server_disconnect();
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_fmw_reboot(std::string msg,std::string &returnval)
{
	//01:39:52.718-->{ "jsonrpc": "2.0", "method": "set_device_operation", "params": { "operation": "reboot" }, "id": 0 }
	//01:39:52.726<--{ "jsonrpc": "2.0", "result": { "return": "InProgress", "taskId": 1 }, "id": 0 }
	char temp_str[255];temp_str[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result = Client.set_single_string_get_single_string_type((char*)"set_device_operation",(char*)"operation",(char*)"reboot",(char*)"taskId",temp_str);
	Client.rpc_server_disconnect();
	returnval="taskID=";returnval+=temp_str;
	if(result==RPC_SRV_RESULT_IN_PROG)
		//AsyncTaskList.push_back(AyncEventEntry(atoi(temp_str),ADCMN_PORT_SYSMGR));
		AccessAsyncTaskList(atoi(temp_str),ADCMN_PORT_SYSMGR);
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
	if(cmdArg.size()<=0)//get-hostname-cmd
		return proc_cmd_fmw_get_hostname(msg,returnval);
	else
		return proc_cmd_fmw_set_hostname(msg);
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
RPC_SRV_RESULT XmppMgr::proc_cmd_dial_voice(std::string msg,std::string &returnval,char* rpc_cmd)
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

	char tID[255];tID[254]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_BBOXSMS)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result=Client.set_single_string_get_single_string_type((char*)rpc_cmd,//"dial_voice",
				(char*)"destination",(char*)destArg.c_str(),
				(char*)"taskId",(char*)tID);
	Client.rpc_server_disconnect();
	returnval="taskID=";returnval+=tID;
	if(result==RPC_SRV_RESULT_IN_PROG)
		//AsyncTaskList.push_back(AyncEventEntry(atoi(tID),ADCMN_PORT_BBOXSMS));
		AccessAsyncTaskList(atoi(tID),ADCMN_PORT_BBOXSMS);
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
RPC_SRV_RESULT XmppMgr::proc_cmd_gsm_modem_identify(std::string msg,std::string &returnval)
{
	char tID[255];tID[254]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_BBOXSMS)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result=Client.set_action_noarg_get_single_string_type((char*)"identify_dev",(char*)"taskId",tID);
	Client.rpc_server_disconnect();
	returnval="taskID=";returnval+=tID;
	if(result==RPC_SRV_RESULT_IN_PROG)
		//AsyncTaskList.push_back(AyncEventEntry(atoi(tID),ADCMN_PORT_BBOXSMS));
		AccessAsyncTaskList(atoi(tID),ADCMN_PORT_BBOXSMS);
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_log_list_update(std::string msg,std::string &returnval)
{
	char tID[255];tID[254]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result=Client.set_action_noarg_get_single_string_type((char*)"update_loglist",(char*)"taskId",tID);
	Client.rpc_server_disconnect();
	returnval="taskID=";returnval+=tID;
	if(result==RPC_SRV_RESULT_IN_PROG)
		//AsyncTaskList.push_back(AyncEventEntry(atoi(tID),ADCMN_PORT_SYSMGR));//before pushing, if event comes back, then it is missed
		AccessAsyncTaskList(atoi(tID),ADCMN_PORT_SYSMGR);
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


