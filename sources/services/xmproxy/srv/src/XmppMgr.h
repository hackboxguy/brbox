#ifndef __XMPP_MGR_H_
#define __XMPP_MGR_H_
#include "ADXmppProxy.hpp"
#include <iostream>
#include <deque>
#include "ADCommon.hpp"
#include "ADThread.hpp"
#include "ADTimer.hpp"

using namespace std;
#define CLIENT_ALIVE_PING_DURATION_MS 60000;//60seconds
#define GITHUB_FMW_DOWNLOAD_FOLDER "http://github.com/hackboxguy/downloads/raw/master/"

#define BBOXSMS_SERVER_ADDR "127.0.0.1"
#define EXMPP_CMD_TABL    {"Smsdeleteall","Smsdelete","Smsget","Smssend","Smsupdate","Smstotal","Fmwver","Fmwupdate","Fmwupsts","Reboot","Unknown","none","\0"}
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
	EXMPP_CMD_FMW_REBOOT,
	EXMPP_CMD_UNKNOWN,
	EXMPP_CMD_NONE
}EXMPP_CMD_TYPES;

struct XmppCmdEntry
{
	std::string cmdMsg;
public:
	XmppCmdEntry(std::string msg) :cmdMsg(msg){}
};


class XmppMgr : public ADXmppConsumer, public ADThreadConsumer, public ADTimerConsumer
{
	int heartbeat_ms;
	int event_period_ms;//
	int CyclicTime_ms;//60sec cycle counter

	ADTimer* pMyTimer;
	bool DebugLog;
	std::string bboxSmsServerAddr;
	std::deque<XmppCmdEntry> processCmd;//fifo for processing xmpp messages

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
	RPC_SRV_RESULT proc_cmd_sms_deleteall(std::string msg);
	RPC_SRV_RESULT proc_cmd_sms_delete(std::string msg);
	RPC_SRV_RESULT proc_cmd_sms_get(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_sms_send(std::string msg);
	RPC_SRV_RESULT proc_cmd_sms_list_update(std::string msg);
	RPC_SRV_RESULT proc_cmd_sms_get_total(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_fmw_get_version(std::string msg,std::string &returnval);
	RPC_SRV_RESULT proc_cmd_fmw_update(std::string msg);
	RPC_SRV_RESULT proc_cmd_fmw_reboot(std::string msg);
	RPC_SRV_RESULT proc_cmd_fmw_update_sts(std::string msg,std::string &returnval);

public:
	XmppMgr();
	~XmppMgr();
	RPC_SRV_RESULT Start(std::string accountFilePath);
	RPC_SRV_RESULT Stop();
	RPC_SRV_RESULT SendMessage(std::string msg);
	void SetDebugLog(bool log);
	int AttachHeartBeat(ADTimer* pTimer);

};
#endif

