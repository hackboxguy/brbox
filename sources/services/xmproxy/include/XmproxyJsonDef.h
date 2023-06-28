#ifndef __XMPROXY_JSON_DEF_H_
#define __XMPROXY_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#include "ADCmnPortList.h"
#define  XMPROXY_JSON_PORT_NUMBER ADCMN_PORT_XMPROXY
/* ------------------------------------------------------------------------- */
typedef enum EJSON_XMPROXY_RPC_TYPES_T
{
	EJSON_XMPROXY_RPC_GET_ASYNCTASK=0,
	EJSON_XMPROXY_RPC_SMS_DELETE_ALL,
	//EJSON_XMPROXY_RPC_SMS_DELETE,
	//EJSON_XMPROXY_RPC_SMS_TOTAL_GET,
	//EJSON_XMPROXY_RPC_SMS_GET,
	//EJSON_XMPROXY_RPC_SMS_SEND,
	//EJSON_XMPROXY_RPC_SMS_LIST_UPDATE,
	//EJSON_XMPROXY_RPC_SMS_IDENTIFY_DEV, //check if modem device is detected correctly
	EJSON_XMPROXY_RPC_GET_ONLINE_STATUS,
	EJSON_XMPROXY_RPC_SET_ONLINE_STATUS, //via rpc, force to go offline
	EJSON_XMPROXY_RPC_SET_SEND_MESSAGE, //send xmpp message to clients from roster
	EJSON_XMPROXY_RPC_SET_SUBSCRIBE, //send xmpp message for friend-request
	EJSON_XMPROXY_RPC_SET_ACCEPT_BUDDY, //accept friend request when arrives
	EJSON_XMPROXY_RPC_END,
	EJSON_XMPROXY_RPC_NONE
}EJSON_XMPROXY_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//EJSON_XMPROXY_RPC_GET_ASYNCTASK //get-async-task-in-progress
#define XMPROXY_RPC_ASYNCTASK_GET         "get_async_task"
#define XMPROXY_RPC_ASYNCTASK_ARG         "task"
#define XMPROXY_RPC_ASYNCTASK_ARG_TABL    {"listUpdate","devIdent","deleteAll","none","none","\0"} //show unknown as none
typedef enum XMPROXY_ASYNCTASK_TYPE_T
{
	XMPROXY_ASYNCTASK_LIST_UPDATE,
	XMPROXY_ASYNCTASK_IDENTIFY_DEV,
	XMPROXY_ASYNCTASK_DELETE_ALL,
	XMPROXY_ASYNCTASK_UNKNOWN,
	XMPROXY_ASYNCTASK_NONE
}XMPROXY_ASYNCTASK_TYPE;
typedef struct XMPROXY_ASYNCTASK_PACKET_T
{
	XMPROXY_ASYNCTASK_TYPE task;
}XMPROXY_ASYNCTASK_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SMARTEYE_RPC_SMS_DELETE_ALL=0,
//EJSON_SMARTEYE_RPC_SMS_GET,
//EJSON_SMARTEYE_RPC_SMS_SEND,
/*#define BBOXSMS_RPC_SMS_DELETE_ALL    "delete_all_sms"
#define BBOXSMS_RPC_SMS_DELETE        "delete_sms"
#define BBOXSMS_RPC_SMS_TOTAL_GET     "get_total_sms"
#define BBOXSMS_RPC_SMS_GET           "get_sms"
#define BBOXSMS_RPC_SMS_ARG_INDX      "index"
#define BBOXSMS_RPC_SMS_ARG_MSG       "message"
#define BBOXSMS_RPC_SMS_ARG_TOTAL     "msgcount"
#define BBOXSMS_RPC_SMS_SEND          "send_sms"
#define BBOXSMS_RPC_SMS_LIST_UPDATE   "update_sms_list"
#define BBOXSMS_RPC_SMS_IDENTIFY_DEV  "identify_dev"
typedef struct BBOXSMS_SMS_PACKET_T
{
	int total_sms;
	int index;
	char sms[1024];
	int taskID;
}BBOXSMS_SMS_PACKET;*/
/* ------------------------------------------------------------------------- */
//EJSON_XMPROXY_RPC_GET_ONLINE_STATUS (get_online_status)
//EJSON_XMPROXY_RPC_SET_ONLINE_STATUS (set_online_status)
#define XMPROXY_RPC_ONLINE_STATUS_GET        "get_online_status"
#define XMPROXY_RPC_ONLINE_STATUS_SET        "set_online_status"
#define XMPROXY_RPC_ONLINE_STATUS_ARG        "status"
#define XMPROXY_RPC_ONLINE_STATUS_ARG_TABL   {"online","offline","unknown","none","\0"}
typedef enum XMPROXY_ONLINESTS_TYPE_T
{
	XMPROXY_ONLINESTS_ONLINE,
	XMPROXY_ONLINESTS_OFFLINE,
	XMPROXY_ONLINESTS_UNKNOWN,
	XMPROXY_ONLINESTS_NONE
}XMPROXY_ONLINESTS_TYPE;
typedef struct XMPROXY_ONLINESTS_PACKET_T
{
	XMPROXY_ONLINESTS_TYPE status;
}XMPROXY_ONLINESTS_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_XMPROXY_RPC_SET_SEND_MESSAGE
#define XMPROXY_RPC_SEND_MESSAGE_SET        "send_message"
#define XMPROXY_RPC_SEND_MESSAGE_TO_ARG     "to"
#define XMPROXY_RPC_SEND_MESSAGE_MSG_ARG    "msg"
typedef struct XMPROXY_SENDMSG_PACKET_T
{
	char to[512];
	char msg[1024];
}XMPROXY_SENDMSG_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_XMPROXY_RPC_SET_SUBSCRIBE
#define XMPROXY_RPC_SUBSCRIBE            "send_subscribe"
#define XMPROXY_RPC_SUBSCRIBE_TO_ARG     "to"
#define XMPROXY_RPC_SUBSCRIBE_MSG_ARG    "msg"
typedef struct XMPROXY_SUBSCRIBE_PACKET_T
{
	char to[512];
	char msg[1024];
}XMPROXY_SUBSCRIBE_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_XMPROXY_RPC_SET_ACCEPT_BUDDY
#define XMPROXY_RPC_ACCEPT_BUDDY            "accept_buddy"
#define XMPROXY_RPC_ACCEPT_BUDDY_TO_ARG     "to"
#define XMPROXY_RPC_ACCEPT_BUDDY_MSG_ARG    "msg"
typedef struct XMPROXY_ACCEPT_BUDDY_PACKET_T
{
	char to[512];
	char msg[1024];
}XMPROXY_ACCEPT_BUDDY_PACKET;
/* ------------------------------------------------------------------------- */
//keep all the data related to smart-eye-service here
typedef struct XMPROXY_CMN_DATA_CACHE_T
{
	void *pDevInfo;//device-info-struct(typecast in rpc handlers)
	std::string StrImgIdDebugFile;//pChar //Char //Int //pInt //Float //Enum
	void *pXmpMgr;//sms processing object pointer needed by other rpc's
	EJSON_XMPROXY_RPC_TYPES AsyncCmdInProgress;
	XMPROXY_CMN_DATA_CACHE_T()
	{
		AsyncCmdInProgress=EJSON_XMPROXY_RPC_NONE;
	};//initialize variables here
	~ XMPROXY_CMN_DATA_CACHE_T(){};
}XMPROXY_CMN_DATA_CACHE;
/* ------------------------------------------------------------------------- */

#endif
