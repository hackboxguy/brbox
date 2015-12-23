#ifndef __SMS_MGR_H_
#define __SMS_MGR_H_
#include "ADThread.hpp"
#include <vector>
//#include <deque>
#include <iostream>
#include "ADCommon.hpp"
#include <gammu.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
#define MODEM_DEV_NODE "/dev/ttyUSB0" //2"
#define MODEM_AT_CONN  "at19200"

typedef enum SMS_ACCESS_TYPE_T
{
	SMS_ACCESS_UPDATE_LIST = 0,
	SMS_ACCESS_READ_LAST,
	SMS_ACCESS_READ_INDEX,
	SMS_ACCESS_COUNT_LIST,
	SMS_ACCESS_EMPTY_LIST,
	SMS_ACCESS_UNKNOWN,
	SMS_ACCESS_NONE
}SMS_ACCESS_TYPE;
struct MsgEntry
{
public:
	std::string Message;
	std::string From;
	int Location;
	int Folder;
	MsgEntry(std::string msg,std::string frm,int loc,int fold) :Message(msg),From(frm),Location(loc),Folder(fold){}
};


class SmsMgr : public ADThreadConsumer
{
	bool GsmDevDetected;
	std::string GsmDevManf ;//gsm device manufacturer
	std::string GsmDevModel;//gsm device model number
	std::string LatestUSSDReply;
	int SmsProcessThreadID;
	std::vector<MsgEntry> msgList;

	ADThread SmsProcessThread;//thread for processing the events from other services
	//thread-callback functions
	virtual int monoshot_callback_function(void* pUserData,ADThreadProducer* pObj);
	virtual int thread_callback_function(void* pUserData,ADThreadProducer* pObj){return 0;};//we are not using this one..
	int error_handler(GSM_StateMachine *s,GSM_Error error);
	RPC_SRV_RESULT GetOrUpdateTotal(SMS_ACCESS_TYPE access,int &total,int indx,
			std::string &msg,std::string &from,std::vector<MsgEntry> &newList);
	//void send_sms_callback (GSM_StateMachine *sm, int status, int MessageReference, void * user_data);

public:
	bool LogFlag;
	SmsMgr();
	~SmsMgr();
	RPC_SRV_RESULT GetSms(int index,char* msg);
	RPC_SRV_RESULT EmptySmsList();
	RPC_SRV_RESULT GetTotalSms(int *total);
	int DetectSmsDevice();
	int ReadSms(int indx);
	int SendSms(char* recipient_number,char* message_text);
	int DeleteAllSMS(int foldernum);
	int DialVoice(char* recipient_number);
	int DialUSSDCode(char* code,char* return_str);
	RPC_SRV_RESULT GetLatestUSSDReply(char* msg);
};
#endif

