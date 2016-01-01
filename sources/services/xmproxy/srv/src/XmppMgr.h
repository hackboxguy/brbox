#ifndef __XMPP_MGR_H_
#define __XMPP_MGR_H_
#include "ADXmppProxy.hpp"
#include <iostream>
#include <vector>
#include <deque>
#include "ADCommon.hpp"
#include "ADThread.hpp"
#include "ADTimer.hpp"

using namespace std;
#define CLIENT_ALIVE_PING_DURATION_MS 60000;//60seconds
#define GITHUB_FMW_DOWNLOAD_FOLDER "http://github.com/hackboxguy/downloads/raw/master/"

#define BBOXSMS_SERVER_ADDR "127.0.0.1"
#define EXMPP_CMD_TABL    {"smsdeleteall","smsdelete","smsget","smssend","smsupdate","smstotal","fmwver","fmwupdt","fmwupsts","fmwupres","reboot","uptime","hostname","myip","resethostname","dialvoice","dialussd","readussd","logsts","gsmcheck","logupdate","logcount","logmsg","unknown","none","\0"}
#define EXMPP_CMD_TABL_HELP    {"","<zero_based_index>","<zero_based_index>","<phone-num> <msg>","","","","<filename>","","","","","","","","<phone-num>","<ussd-code>","","<port> [sts]","","","","<index>","unknown","none","\0"}

typedef enum EXMPP_CMD_TYPES_T
{
	EXMPP_CMD_SMS_DELETE_ALL=0,
	EXMPP_CMD_SMS_DELETE,
	EXMPP_CMD_SMS_GET,
	EXMPP_CMD_SMS_SEND,
	EXMPP_CMD_SMS_LIST_UPDATE,
	EXMPP_CMD_SMS_GET_TOTAL,
	EXMPP_CMD_FMW_GET_VERSION,
	EXMPP_CMD_FMW_UPDATE,
	EXMPP_CMD_FMW_UPDATE_STS,
	EXMPP_CMD_FMW_UPDATE_RES, //result of the last fmw update command
	EXMPP_CMD_FMW_REBOOT,
	EXMPP_CMD_FMW_UPTIME,
	//EXMPP_CMD_FMW_GET_HOSTNAME,
	//EXMPP_CMD_FMW_SET_HOSTNAME,
	EXMPP_CMD_FMW_HOSTNAME,
	EXMPP_CMD_FMW_GET_MYIP,
	EXMPP_CMD_FMW_RESET_HOSTNAME,
	EXMPP_CMD_DIAL_VOICE,
	EXMPP_CMD_DIAL_USSD,
	EXMPP_CMD_GET_USSD,
	EXMPP_CMD_DEBUG_LOG_STS,
	EXMPP_CMD_GSM_MODEM_IDENT,//identify gsm-usb modem
	EXMPP_CMD_LOG_UPDATE,//trigger reloading of log message to vector list
	EXMPP_CMD_LOG_COUNT, //read total items in vector list
	EXMPP_CMD_LOG_MSG,   //read item-msg from vector list
	EXMPP_CMD_FMW_GET_LOCALIP,
	EXMPP_CMD_UNKNOWN,
	EXMPP_CMD_NONE
}EXMPP_CMD_TYPES;

struct XmppCmdEntry
{
	std::string cmdMsg;
public:
	XmppCmdEntry(std::string msg) :cmdMsg(msg){}
};
/* ------------------------------------------------------------------------- */
struct AyncEventEntry
{
	int taskID; //async event taskID returned by server
	int srvPort;//port where async command was sent
public:
	AyncEventEntry(int tid,int port) :taskID(tid),srvPort(port){}
};
//following functor object is used as predicator for finding a specific vector element entry based on srvToken
class FindAsyncEventEntry
{
	const int mytaskID;
	const int mysrvPort;
public:
	FindAsyncEventEntry(const int tid,const int port) :mytaskID(tid),mysrvPort(port){}
	bool operator()(AyncEventEntry pEntry) const
	{
		if(pEntry.taskID == mytaskID && pEntry.srvPort == mysrvPort)
			return true;
		else
			return false;
	}
};
/* ------------------------------------------------------------------------- */
typedef struct XMPROXY_CMD_TABLE_T
{
	bool cmdsts;
	EXMPP_CMD_TYPES cmd;
	char cmd_name[128];
	char cmd_arg[128];
}XMPROXY_CMD_TABLE;
/* ------------------------------------------------------------------------- */
class XmppMgr : public ADXmppConsumer, public ADThreadConsumer, public ADTimerConsumer
{
	int heartbeat_ms;
	int event_period_ms;//
	int CyclicTime_ms;//60sec cycle counter
	int LastFmwUpdateTaskID;

	ADTimer* pMyTimer;
	bool DebugLog;
	std::string bboxSmsServerAddr;
	std::deque<XmppCmdEntry> processCmd;//fifo for processing xmpp messages
	std::vector<AyncEventEntry> AsyncTaskList;

	std::string XmppUserName;
	std::string XmppUserPw;
	ADXmppProxy XmppProxy;//xmpp client
	//xmpp-client-callback functions
	virtual int onXmppMessage(std::string msg,ADXmppProducer* pObj);

	ADThread XmppClientThread,XmppCmdProcessThread;//thread for xmpp client connection
	virtual int monoshot_callback_function(void* pUserData,ADThreadProducer* pObj);//{return 0;};
	virtual int thread_callback_function(void* pUserData,ADThreadProducer* pObj);//{return 0;};//we are not using this one..

	//ADTimerConsumer overrides: 100ms timer and sigio 
	virtual int sigio_notification(){return 0;};
	virtual int timer_notification();//{return 0;};
	virtual int custom_sig_notification(int signum){return 0;};

	EXMPP_CMD_TYPES ResolveCmdStr(std::string cmd);
	RPC_SRV_RESULT proc_cmd_sms_deleteall(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_sms_delete(std::string msg);
	RPC_SRV_RESULT proc_cmd_sms_get(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_sms_send(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_sms_list_update(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_sms_get_total(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_fmw_get_version(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_fmw_update(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_fmw_reboot(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_fmw_update_sts(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_fmw_update_res(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_fmw_uptime(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_fmw_get_hostname(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_fmw_set_hostname(std::string msg);
	RPC_SRV_RESULT proc_cmd_fmw_hostname(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_fmw_get_myip(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_fmw_set_default_hostname(std::string msg);
	RPC_SRV_RESULT proc_cmd_dial_voice(std::string msg,std::string &returnval,char* rpc_cmd);
	RPC_SRV_RESULT proc_cmd_get_ussd(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_logsts(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_gsm_modem_identify(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_log_list_update(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_log_get_count(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_log_get_line(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_fmw_get_localip(std::string msg,std::string &returnval);
	std::string print_help();
public:
	XmppMgr();
	~XmppMgr();
	RPC_SRV_RESULT Start(std::string accountFilePath);
	RPC_SRV_RESULT Stop();
	RPC_SRV_RESULT SendMessage(std::string msg);
	void SetDebugLog(bool log);
	int AttachHeartBeat(ADTimer* pTimer);
	RPC_SRV_RESULT RpcResponseCallback(RPC_SRV_RESULT taskRes,int taskID);//called by eventHandler
	RPC_SRV_RESULT RpcResponseCallback(std::string taskRes,int taskID);
	//RPC_SRV_RESULT IsItMyAsyncTaskResp(int tid,int port);
	RPC_SRV_RESULT AccessAsyncTaskList(int tid, int port, bool insertEntryFlag=true);
	void SetUSBGsmSts(bool sts);
};
#endif

