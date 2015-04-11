#ifndef __JSON_CMNDEF_H_
#define __JSON_CMNDEF_H_
#include <json-c/json.h>
#include "ADCommon.h"
//other common strings in rpc format
#define RPC_NAME_ARG_RESULT_OBJ    "result"
#define RPC_NAME_ARG_RESULT_PARAM  "return"

#define JSON_RPC_METHOD_NAME_MAX_LENGTH 256
#define JSON_RPC_METHOD_REQ_MAX_LENGTH  1100 //considering 1400 MTU limit
#define JSON_RPC_METHOD_RESP_MAX_LENGTH 1100 //considering 1400 MTU limit


#define MSG_CMD_TASK_RESULT_SUCCESS        "Success"    
#define MSG_CMD_TASK_RESULT_FAIL           "Fail"
#define MSG_CMD_TASK_RESULT_IN_PROGRESS    "InProgress"    //request is in progress
#define MSG_CMD_TASK_RESULT_NOT_STARTED    "NotStarted"    //request processing not started
#define MSG_CMD_TASK_RESULT_DEV_TIMEOUT    "Timeout"       //response not received from device
#define MSG_CMD_TASK_RESULT_ARG_ERROR      "ArgError"      //response not received from device
#define MSG_CMD_TASK_RESULT_MEM_ERROR      "MemError"      //memory allocation failed
#define MSG_CMD_TASK_RESULT_BUS_ERROR      "BusError"      //errors like i2c bus error
#define MSG_CMD_TASK_RESULT_ACTION_BLOCKED "ActionBlocked" //driver is not open or driver error
#define MSG_CMD_TASK_RESULT_DEV_NO_ACCESS  "DeviceError"   //driver is not open or driver error
#define MSG_CMD_TASK_RESULT_UNKNOWN        "Unknown"    //unknown error
#define MSG_CMD_TASK_RESULT_MEM_FAIL       "MemError"    //dynamic memory allocation error
#define MSG_CMD_TASK_RESULT_INVALID_PARAM  "ParamError"   //user passed invalid parameters
#define MSG_CMD_TASK_RESULT_DEV_ERR        "DevError"   //device driver error
#define MSG_CMD_TASK_RESULT_ACTUAL_VALUE   "Actual"     //request is in progress

typedef enum cmd_task_result_t
{
	CMD_TASK_RESULT_FAIL=-1,
	CMD_TASK_RESULT_SUCCESS=0,
	CMD_TASK_RESULT_INVALID_PARAM,//command argument error
	CMD_TASK_RESULT_DEV_TIMEOUT,
	CMD_TASK_RESULT_DEV_ERR,
	CMD_TASK_RESULT_MEM_FAIL,
	CMD_TASK_RESULT_IN_PROGRESS,
	CMD_TASK_RESULT_NOT_STARTED,
	CMD_TASK_RESULT_ACTUAL_VALUE,//user wants to send actual result value
	CMD_TASK_RESULT_UNKNOWN
}CMD_TASK_RESULT;
typedef struct JsonDataCommObj_t
{
	int req_id;//json rpc request ID(passed by the client)
	int cmd_index;//json rpc command index(message-id)

	CMD_TASK_RESULT mapper_result;
	RPC_SRV_RESULT  rpc_code;//as per unified error code b/w client and server
	RPC_SRV_ACT     rpc_action;//as per unified error code b/w client and server

	int worker_result;
	
	char* socket_data;//actual text data received on json-socket
	json_object *json_resp_obj;   //for returning more than one value
	char *custom_result_string;
	//actual binary req obj(communication object)
	int dataObjLen;
	unsigned char *pDataObj;//[1024]; //for storing various objects
}JsonDataCommObj;


#endif

