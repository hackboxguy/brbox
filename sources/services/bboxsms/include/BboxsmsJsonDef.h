#ifndef __BBOXSMS_JSON_DEF_H_
#define __BBOXSMS_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#include "ADCmnPortList.h"
#define  BBOXSMS_JSON_PORT_NUMBER ADCMN_PORT_BBOXSMS
/* ------------------------------------------------------------------------- */
typedef enum EJSON_BBOXSMS_RPC_TYPES_T
{
	EJSON_BBOXSMS_RPC_SMS_DELETE_ALL=0,
	EJSON_BBOXSMS_RPC_SMS_DELETE,
	EJSON_BBOXSMS_RPC_SMS_TOTAL_GET,
	EJSON_BBOXSMS_RPC_SMS_GET,
	EJSON_BBOXSMS_RPC_SMS_SEND,
	EJSON_BBOXSMS_RPC_END,
	EJSON_BBOXSMS_RPC_NONE
}EJSON_BBOXSMS_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//EJSON_SMARTEYE_RPC_SMS_DELETE_ALL=0,
//EJSON_SMARTEYE_RPC_SMS_GET,
//EJSON_SMARTEYE_RPC_SMS_SEND,
#define BBOXSMS_RPC_SMS_DELETE_ALL    "delete_all_sms"
#define BBOXSMS_RPC_SMS_DELETE        "delete_sms"
#define BBOXSMS_RPC_SMS_TOTAL_GET     "get_total_sms"
#define BBOXSMS_RPC_SMS_GET           "get_sms"
#define BBOXSMS_RPC_SMS_ARG_INDX      "index"
#define BBOXSMS_RPC_SMS_ARG_MSG       "message"

#define BBOXSMS_RPC_SMS_SEND          "send_sms"
typedef struct BBOXSMS_SMS_PACKET_T
{
	int total_sms;
	int index;
	char sms[1024];
}BBOXSMS_SMS_PACKET;
/* ------------------------------------------------------------------------- */
//keep all the data related to smart-eye-service here
typedef struct BBOXSMS_CMN_DATA_CACHE_T
{
	void *pDevInfo;//device-info-struct(typecast in rpc handlers)
	std::string StrImgIdDebugFile;//pChar //Char //Int //pInt //Float //Enum
}BBOXSMS_CMN_DATA_CACHE;
/* ------------------------------------------------------------------------- */

#endif

