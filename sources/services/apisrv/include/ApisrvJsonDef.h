#ifndef __APISRV_JSON_DEF_H_
#define __APISRV_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#include "ADCmnPortList.h"
#define  APISRV_JSON_PORT_NUMBER ADCMN_PORT_APISRV
/* ------------------------------------------------------------------------- */
typedef enum EJSON_APISRV_RPC_TYPES_T
{
	EJSON_APISRV_RPC_GET_ASYNCTASK=0,
	EJSON_APISRV_RPC_END,
	EJSON_APISRV_RPC_NONE
}EJSON_APISRV_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//EJSON_APISRV_RPC_GET_ASYNCTASK //get-async-task-in-progress
#define APISRV_RPC_ASYNCTASK_GET         "get_async_task"
#define APISRV_RPC_ASYNCTASK_ARG         "task"
#define APISRV_RPC_ASYNCTASK_ARG_TABL    {"listUpdate","devIdent","deleteAll","none","none","\0"} //show unknown as none
typedef enum APISRV_ASYNCTASK_TYPE_T
{
	APISRV_ASYNCTASK_LIST_UPDATE,
	APISRV_ASYNCTASK_IDENTIFY_DEV,
	APISRV_ASYNCTASK_DELETE_ALL,
	APISRV_ASYNCTASK_UNKNOWN,
	APISRV_ASYNCTASK_NONE
}APISRV_ASYNCTASK_TYPE;
typedef struct APISRV_ASYNCTASK_PACKET_T
{
	APISRV_ASYNCTASK_TYPE task;
}APISRV_ASYNCTASK_PACKET;
/* ------------------------------------------------------------------------- */
//keep all the data related to smart-eye-service here
typedef struct APISRV_CMN_DATA_CACHE_T
{
	void *pDevInfo;//device-info-struct(typecast in rpc handlers)
	std::string StrImgIdDebugFile;//pChar //Char //Int //pInt //Float //Enum
	void *pApiMgr;//sms processing object pointer needed by other rpc's
	EJSON_APISRV_RPC_TYPES AsyncCmdInProgress;
	APISRV_CMN_DATA_CACHE_T()
	{
		AsyncCmdInProgress=EJSON_APISRV_RPC_NONE;
	};//initialize variables here
	~ APISRV_CMN_DATA_CACHE_T(){};
}APISRV_CMN_DATA_CACHE;
/* ------------------------------------------------------------------------- */

#endif

