/*****************************************************************************/
#ifndef __ADCOMMON_H_
#define __ADCOMMON_H_

//message debugging prints

//#define MSTAR_EMULATION_MODE 1 //echo back received data
#define EMULATION_MODE       1
#define SOCKET_DEBUG_MESSAGE 1
#define RX_TX_DEBUG_PRINT    1

#define SDMS_JSON_PORT_NUMBER 42510 //smart-display-video-service
#ifndef MSTAR_EMULATION_MODE
	#define SDPS_JSON_PORT_NUMBER 42511 //smart-display-panel-service
#else
	#define SDPS_JSON_PORT_NUMBER 42521 //act as mstar uart slave
#endif
#define SDFS_JSON_PORT_NUMBER  42512 //smart-display-fpga-service
#define SDSMS_JSON_PORT_NUMBER 42513 //smart-display-system-mgr-service
#define SDINS_JSON_PORT_NUMBER 42514
#define SDES_JSON_PORT_NUMBER 42515
#define SDCS_JSON_PORT_NUMBER 42516
#define SDGS_JSON_PORT_NUMBER 42517
/**************************************************/
//different types of temprory files used in /tmp
//Reason for listing this tempfile list is to avoid any collision by different services.
//#define SDSMS_TEMP_FILE_VALIDATE_FMW
/**************************************************/
//following are the common RPC index,name and args which are same for all services
//task status related index,name and arg strings
#define ADLIB_RPC_NAME_GET_TASK_STATUS      "get_rpc_req_status" //shall remain same in all services
#define ADLIB_RPC_INDX_GET_TASK_STATUS      0
#define ADLIB_RPC_PARM_TASK_STS_ID          "taskId"
#define ADLIB_RPC_PARM_TASK_STS             "taskStatus"
#define ADLIB_RPC_PARM_ASYNC_RESP_TYPE      "resptype"
#define ADLIB_RPC_TABL_ASYNC_RESP_TYPE      {"poll","trigger","event","unknown","none","\0"}
typedef enum ADLIB_ASYNC_RESP_TYPE_T
{
	ADLIB_ASYNC_RESP_TYPE_POLL,    //after async call is finished, no event is sent, caller should poll for status of the taskID(existing implementation)
	ADLIB_ASYNC_RESP_TYPE_TRIGGER, //after async call is finished, no result is available(fire-and-forget type of async call)
	ADLIB_ASYNC_RESP_TYPE_EVENT,   //after async call is finished, event is sent to subscriber along with taskID and result-value
	ADLIB_ASYNC_RESP_TYPE_UNKNOWN,
	ADLIB_ASYNC_RESP_TYPE_NONE
}ADLIB_ASYNC_RESP_TYPE;


//rpc version related index,name and arg strings
#define ADLIB_RPC_NAME_GET_SRV_VER          "get_rpc_srv_version" //shall remain same in all services
#define ADLIB_RPC_INDX_GET_SRV_VER          1
#define ADLIB_RPC_PARM_GET_SRV_VER          "version"

//rpc savesettings related index,name and arg strings
#define ADLIB_RPC_NAME_TRIGGER_DATASAVE     "trigger_settings_save" //shall remain same in all services
#define ADLIB_RPC_INDX_TRIGGER_DATASAVE     2

//rpc settings related index,name and arg strings
#define ADLIB_RPC_NAME_GET_SETTINGS_STATUS  "get_settings_status" //shall remain same in all services
#define ADLIB_RPC_INDX_GET_SETTINGS_STATUS  3
#define ADLIB_RPC_PARM_GET_SETTINGS_STATUS  "status"

//shutdown service related index,name and arg strings
#define ADLIB_RPC_NAME_SHUTDOWN_SERVICE     "trigger_shutdown" //shall remain same in all services
#define ADLIB_RPC_INDX_SHUTDOWN_SERVICE     4

//cleanup task status-result chain and reset ID to 0
//calling this rpc may cause result of work-in-progress to be lost(so use this carefully) 
#define ADLIB_RPC_NAME_RESET_TASK_STS     "reset_task_status" //shall remain same in all services
#define ADLIB_RPC_INDX_RESET_TASK_STS     5

#define ADLIB_RPC_NAME_GET_READY_STS      "get_ready_status" //service is ready to accept rpc-messages when "ready"
#define ADLIB_RPC_INDX_GET_READY_STS      6
#define ADLIB_RPC_PARM_GET_READY_STS      "status"
#define ADLIB_RPC_TABL_GET_READY_STS      {"notready","ready","busy","unknown","none","\0"}
typedef enum ADLIB_SERVICE_READY_STATE_T
{
	ADLIB_SERVICE_READY_STATE_NOT_READY,
	ADLIB_SERVICE_READY_STATE_READY,
	ADLIB_SERVICE_READY_STATE_BUSY,
	ADLIB_SERVICE_READY_STATE_UNKNOWN,
	ADLIB_SERVICE_READY_STATE_NONE
}ADLIB_SERVICE_READY_STATE;

#define ADLIB_RPC_NAME_TRIGGER_STORE_FACTRY     "trigger_factory_store"
#define ADLIB_RPC_INDX_TRIGGER_STORE_FACTRY     9
#define ADLIB_RPC_NAME_TRIGGER_RESTORE_FACTRY   "trigger_factory_restore"
#define ADLIB_RPC_INDX_TRIGGER_RESTORE_FACTRY   10

//subscribe event
#define RPCMGR_RPC_EVENT_SUBSCRIBE    "subscribe_event"
#define RPCMGR_RPC_EVENT_UNSUBSCRIBE  "unsubscribe_event"
#define RPCMGR_RPC_EVENT_NOTIFY       "notify_event"  //for self calling within server
#define RPCMGR_RPC_EVENT_PROCESS      "process_event" //notification from other services
#define RPCMGR_RPC_EVENT_ARG_CLTTOK   "cltToken" //sent from clt to srv, but srv will return this with events
#define RPCMGR_RPC_EVENT_ARG_PORT     "port"
#define RPCMGR_RPC_EVENT_ARG_EVENTNUM "evntNum"  //which of the events
#define RPCMGR_RPC_EVENT_ARG_SRVTOK   "srvToken" //sent from server to client as a subscription token
#define RPCMGR_RPC_EVENT_ARG_EXTRA    "evntArg1"  //optional extra argument sent with eventNum
#define RPCMGR_RPC_EVENT_ARG2_EXTRA   "evntArg2"  //optional second argument sent with eventNum
#define ADLIB_EVENT_NUM_INPROG_DONE 0
#define ADLIB_EVENT_NUM_SHUT_DOWN   1
#define ADLIB_EVENT_NUM_RESERVED1   2
#define ADLIB_EVENT_NUM_RESERVED2   3
#define ADLIB_EVENT_NUM_RESERVED3   4
#define ADLIB_EVENT_NUM_END         5
#define ADLIB_RPC_EVENT_TYPE_ENUM_TABL {"InProgDone","Shutdown","Reservd1","Reservd2","Reservd3","unknown","none","\0"}
//eventing related macros
//if(Client.set_integer_type_with_addr_para((char*)RPCMGR_RPC_EVENT_NOTIFY,\
	     (char*)RPCMGR_RPC_EVENT_ARG_EVENTNUM,evntNum,\
	     (char*)RPCMGR_RPC_EVENT_ARG_EXTRA,evntArg)!=RPC_SRV_RESULT_SUCCESS)\

#define NOTIFY_EVENT(evntNum,evntArg,selfPortNum,evntArg2)\
do			\
{			\
	ADJsonRpcClient Client;\
	if(Client.rpc_server_connect("127.0.0.1",selfPortNum)!=0)\
	{\
		LOG_DEBUG_MSG_1_ARG(true,"BRBOX:NotifySelf","Unable connect on port = %d",selfPortNum);\
	}\
	else\
	{\
		if(Client.set_three_int_type((char*)RPCMGR_RPC_EVENT_NOTIFY,\
				     (char*)RPCMGR_RPC_EVENT_ARG_EVENTNUM,evntNum,\
				     (char*)RPCMGR_RPC_EVENT_ARG_EXTRA,evntArg,\
				     (char*)RPCMGR_RPC_EVENT_ARG2_EXTRA,evntArg2)!=RPC_SRV_RESULT_SUCCESS)\
		{\
			LOG_DEBUG_MSG_2_ARG(true,"BRBOX:NotifySelf","Unable send event = %d on port = %d",evntNum,selfPortNum);\
		}\
		Client.rpc_server_disconnect();\
	}\
} while (0)

#define SUBSCRIBE_EVENT(ip,destPort,srvToken,cltToken,evntNum,srcPort)\
do			\
{			\
	ADJsonRpcClient Client;\
	if(Client.rpc_server_connect(ip,destPort)!=0)\
	{\
		LOG_DEBUG_MSG_1_ARG(true,"BRBOX:SubscribeEvent","Unable connect on port = %d",destPort);\
	}\
	else\
	{\
		if(Client.set_three_int_get_one_int((char*)RPCMGR_RPC_EVENT_SUBSCRIBE,\
					     (char*)RPCMGR_RPC_EVENT_ARG_CLTTOK,cltToken,\
					     (char*)RPCMGR_RPC_EVENT_ARG_PORT,srcPort,\
					     (char*)RPCMGR_RPC_EVENT_ARG_EVENTNUM,evntNum,\
					     (char*)RPCMGR_RPC_EVENT_ARG_SRVTOK,srvToken)!=RPC_SRV_RESULT_SUCCESS)\
		{\
			LOG_DEBUG_MSG_2_ARG(true,"BRBOX:SubscribeEvent","Unable subscribe event = %d on port = %d",evntNum,cltToken);\
		}\
		Client.rpc_server_disconnect();\
	}\
} while (0)
#define UNSUBSCRIBE_EVENT(ip,destPort,srvToken)\
do			\
{			\
	ADJsonRpcClient Client;\
	if(Client.rpc_server_connect(ip,destPort)!=0)\
	{\
		LOG_DEBUG_MSG_1_ARG(true,"BRBOX:SubscribeEvent","Unable connect on port = %d",destPort);\
	}\
	else\
	{\
		if(Client.set_integer_type(  (char*)RPCMGR_RPC_EVENT_UNSUBSCRIBE,\
					     (char*)RPCMGR_RPC_EVENT_ARG_SRVTOK,srvToken)!=RPC_SRV_RESULT_SUCCESS)\
		{\
			LOG_DEBUG_MSG_2_ARG(true,"BRBOX:SubscribeEvent","Unable unsubscribe srvToken = %d on port = %d",srvToken,destPort);\
		}\
		Client.rpc_server_disconnect();\
	}\
} while (0)


//internal rpc-chain report


//
/**************************************************/
#define ADLIB_SDSRV_LOG_PREFIX "SDSRV"
#define APPLICATION_NAME "libadav"

#undef APPDEBUG	/* undef it, just in case */
#ifdef APPLICATION_DEBUG
# define APPDEBUG(fmt, args...) printf( "%s:%04d:%s:" fmt,APPLICATION_NAME,__LINE__,__FUNCTION__, ## args)
#else
# define APPDEBUG(fmt, args...) /* not debugging: nothing */
#endif
#undef APPDEBUGG
#define APPDEBUGG(fmt, args...) /* nothing: it's a placeholder */
#define MSG(fmt, args...) printf( "%s:%04d:%s:" fmt,APPLICATION_NAME,__LINE__,__FUNCTION__, ## args)


#include <syslog.h>
#define LOG_ERR_MSG(service,msg)\
do			\
{			\
	openlog (service, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);\
	syslog(LOG_ERR, msg);\
	closelog ();\
} while (0)
#define LOG_ERR_MSG_WITH_ARG(service,msg,arg)\
do			\
{			\
	openlog (service, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);\
	syslog(LOG_ERR, msg,arg);\
	closelog ();\
} while (0)
#define LOG_DEBUG_MSG(logflag,service,msg)\
do			\
{			\
	if(logflag==true)\
	{\
	openlog (service, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);\
	syslog(LOG_DEBUG, msg);\
	closelog ();\
	}\
} while (0)
#define LOG_DEBUG_MSG_1_ARG(logflag,service,msg,arg1)\
do			\
{			\
	if(logflag==true)\
	{\
		openlog (service, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);\
		syslog(LOG_DEBUG, msg,arg1);\
		closelog ();\
	}\
} while (0)
#define LOG_DEBUG_MSG_2_ARG(logflag,service,msg,arg1,arg2)\
do			\
{			\
	if(logflag==true)\
	{\
		openlog (service, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);\
		syslog(LOG_DEBUG, msg,arg1,arg2);\
		closelog ();\
	}\
} while (0)
/*****************************************************************************/
//used for creating memory for structures
// like: obj * pMemory = new (obj)
#define OBJECT_MEM_CREATE(pMemory,obj)\
do			\
{			\
	if (pMemory != NULL)\
	{\
		MSG("OBJ_MEM_CREATE():mem creation for non NULL pointer detected\n");\
		;\
	}\
	else		\
	{\
		pMemory=(obj*)malloc(sizeof(obj));\
		if(pMemory==NULL)\
			MSG("OBJ_MEM_CREATE():mem could not be allocated for size=%d\n",(int)sizeof(obj));\
	}\
} while (0)
/*****************************************************************************/
//used for creating memory for arrays(e.g char/int/float etc)
#define ARRAY_MEM_CREATE(pMemory,size)\
do			\
{			\
	if (pMemory != NULL)\
	{\
		MSG("ARR_MEM_CREATE():mem creation for non NULL pointer detected\n");\
		;\
	}\
	else		\
	{\
		pMemory=malloc(size);\
		if(pMemory==NULL)\
			MSG("ARR_MEM_CREATE():mem could not be allocated for size=%d\n",size);\
	}\
} while (0)
/*****************************************************************************/
#define MEM_DELETE(pMemory)\
do			\
{			\
	if (pMemory == NULL)\
	{\
		MSG("mem deletion of NULL pointer detected\n");\
		;\
	}\
	else		\
	{\
		free(pMemory);\
		pMemory = NULL;\
	}\
} while (0)
/*****************************************************************************/
#define OBJECT_MEM_NEW(pMemory,obj)\
do			\
{			\
	if (pMemory != NULL)\
	{\
		MSG("OBJ_MEM_CREATE():mem creation for non NULL pointer detected\n");\
		;\
	}\
	else		\
	{\
		pMemory=new obj;\
		if(pMemory==NULL)\
			MSG("OBJ_MEM_CREATE():mem could not be allocated for size=%d\n",(int)sizeof(obj));\
	}\
} while (0)
/*****************************************************************************/
#define OBJ_MEM_DELETE(pMemory)\
do			\
{			\
	if (pMemory == NULL)\
	{\
		MSG("mem deletion of NULL pointer detected\n");\
		;\
	}\
	else		\
	{\
		delete pMemory;\
		pMemory = NULL;\
	}\
} while (0)
/*****************************************************************************/
#define ARRAY_MEM_NEW(pMemory,size)\
do			\
{			\
	if (pMemory != NULL)\
	{\
		MSG("ARR_MEM_CREATE():mem creation for non NULL pointer detected\n");\
		;\
	}\
	else		\
	{\
		pMemory=new char [size];\
		if(pMemory==NULL)\
			MSG("ARR_MEM_CREATE():mem could not be allocated for size=%d\n",(int)size);\
	}\
} while (0)
#define ARRAY_MEM_DELETE(pMemory)	\
	do							\
	{							\
		if (pMemory == NULL)\
			MSG("ARRAY_MEM_DELETE():NULL pointer detected.\n");\
		else					\
			delete [] pMemory;	\
		pMemory = NULL;			\
	} while (0)
/*****************************************************************************/
//rpc communication objects between mapper and worker
typedef enum rpc_srv_action_t
{
	RPC_SRV_ACT_READ = 0,
	RPC_SRV_ACT_WRITE,
	RPC_SRV_ACT_VERIFY,
	RPC_SRV_ACT_READONLY,
	RPC_SRV_ACT_WRITEONLY,
	RPC_SRV_ACT_READ_AND_WRITE,
    RPC_SRV_ACT_RESET,
	RPC_SRV_ACT_UNKNOWN,
	RPC_SRV_ACT_NONE
}RPC_SRV_ACT;
//unified error coding between client and server function calls
typedef enum rpc_srv_result_t
{
	RPC_SRV_RESULT_SUCCESS = 0, //command success
	RPC_SRV_RESULT_FAIL,        //for "some" reason command failed
	RPC_SRV_RESULT_IN_PROG,     //action is in progress
	RPC_SRV_RESULT_BUSY,        //request will be rejected because same command has already been triggered and it is busy.
	RPC_SRV_RESULT_NOT_STARTED, //action has not started
	RPC_SRV_RESULT_TIMEOUT,     //ex: did not receive rx bytes on uart or from other device
	RPC_SRV_RESULT_ARG_ERROR,   //user supplied wrong argument
	RPC_SRV_RESULT_MEM_ERROR,   //internal dynamic memory allocation error
	RPC_SRV_RESULT_BUS_ERROR,   //incase of i2c like interface(for fmtr)
	RPC_SRV_RESULT_ACTION_NOT_ALLOWED,//read or write action not allowed
	RPC_SRV_RESULT_DEV_NOT_ACCESSIBLE,//kernel-space device is not open
	RPC_SRV_RESULT_FILE_NOT_FOUND,//file on the target not found
	RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE,//feature not implemented yet
	RPC_SRV_RESULT_FEATURE_UNSUPPORTED,  //feature will not be supported
	RPC_SRV_RESULT_TASK_ID_NOT_FOUND,
	RPC_SRV_RESULT_VALUE_OUT_OF_RANGE,
	RPC_SRV_RESULT_FILE_OPEN_ERR,
	RPC_SRV_RESULT_FILE_READ_ERR,
	RPC_SRV_RESULT_FILE_WRITE_ERR,
	RPC_SRV_RESULT_FILE_SAME,
	RPC_SRV_RESULT_FILE_DIFFERENT,
	RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR,
	RPC_SRV_RESULT_RESEND,
	RPC_SRV_RESULT_CS_EXPOSURE_ERR,
	RPC_SRV_RESULT_CS_I2C_ERR,
	RPC_SRV_RESULT_ITEM_NOT_FOUND,
	RPC_SRV_RESULT_FILE_ERROR, //file provided is not suitable for this device(eg:wrong fmw file)
	RPC_SRV_RESULT_DEVINFO_ERROR, //invalid Device-Info, eg: some functionality is based on device-type, if device-type not available, then return this error
	RPC_SRV_RESULT_FILE_EMPTY, //file exists but data is empty
	RPC_SRV_RESULT_ITEM_DUPLICATE_FOUND,//event subscription: duplicate subscription found
	RPC_SRV_RESULT_UNKNOWN_COMMAND,
	RPC_SRV_RESULT_DEVNODE_ACCERR, //device node access error(run service as sudo to fix this issue)
	RPC_SRV_RESULT_DEVNODE_OPENERR, //device node open error
	RPC_SRV_RESULT_UNKNOWN,
	RPC_SRV_RESULT_NONE
}RPC_SRV_RESULT;
#define STR_RPC_SRV_RESULT_STRING_TABLE  {"Success","Fail","InProgress","Busy","NotStarted","Timeout","ArgError","MemError","BusError","ActionBlocked","DeviceError","FileNotFound","FeatureNotAvailable","UnsupportedFeature","taskIDNotFound","valueOutOfRange","FileOpenErr","FileReadErr","FileWriteErr","FileSame","FileNotSame","HostUnreachable","resend","csExposureError","csI2CErr","ItemNotFound","FileError","DevInfoErr","FileEmpty","DuplicateItem","UnknownCmd","DevNodeAccErr","DevNodeOpenErr","Unknown","none","\0"}
/*#define STR_RPC_SRV_RESULT_SUCCESS        "Success"    
#define STR_RPC_SRV_RESULT_FAIL           "Fail"
#define STR_RPC_SRV_RESULT_IN_PROGRESS    "InProgress"    //request is in progress
#define STR_RPC_SRV_RESULT_NOT_STARTED    "NotStarted"    //request processing not started
#define STR_RPC_SRV_RESULT_DEV_TIMEOUT    "Timeout"       //response not received from device
#define STR_RPC_SRV_RESULT_ARG_ERROR      "ArgError"      //response not received from device
#define STR_RPC_SRV_RESULT_MEM_ERROR      "MemError"      //memory allocation failed
#define STR_RPC_SRV_RESULT_BUS_ERROR      "BusError"      //errors like i2c bus error
#define STR_RPC_SRV_RESULT_ACTION_BLOCKED "ActionBlocked" //driver is not open or driver error
#define STR_RPC_SRV_RESULT_DEV_NO_ACCESS  "DeviceError"   //driver is not open or driver error
#define STR_RPC_SRV_RESULT_UNKNOWN        "Unknown"    //unknown error
*/
typedef struct rpc_srv_request_t
{
	RPC_SRV_ACT        action;   //read or write
	int	           cmd;      //worker_command
	int                devID;    //in case of uart-daisy-chain, panel_id is needed
	int	 	   clientId; //original requestor
	int		   reqIdent; //request identifier
	RPC_SRV_RESULT     result;   //result of the command execution
	unsigned int	   dataSize; //size of dataRef buffer
	unsigned char	   *dataRef; //actual data object of a command
}RPC_SRV_REQ;
/*****************************************************************************/
//macros for mappers 
#define PREPARE_JSON_REQUEST(REQ_OBJ,REQ_PACKET,REQ_ACTION,REQ_CMD)\
do			\
{			\
	if(prepare_req_object(pReq,REQ_ACTION,REQ_CMD)!=0)\
		return -1;\
	REQ_OBJ *pPanelReq=NULL;\
	pPanelReq=(REQ_OBJ *)pReq->pDataObj;\
	pPanelReq->dataSize = sizeof(REQ_PACKET);\
	OBJECT_MEM_NEW(pPanelCmdObj,REQ_PACKET);\
	pPanelReq->dataRef=(unsigned char*)pPanelCmdObj;\
	if(pPanelReq->dataRef==NULL)\
	{\
		pReq->mapper_result=CMD_TASK_RESULT_MEM_FAIL;\
		return -1;\
	}\
} while (0)
/*****************************************************************************/
#define PREPARE_JSON_RESP(REQ_OBJ,REQ_PACKET)\
do			\
{			\
	REQ_OBJ *pPanelReq=NULL;\
	REQ_PACKET* pPanelCmdObj;\
	pPanelReq=(REQ_OBJ *)pReq->pDataObj;\
	pPanelCmdObj=(REQ_PACKET*)pPanelReq->dataRef;\
	prepare_result_string(pPanelReq->result,pReq);\
	OBJ_MEM_DELETE(pPanelCmdObj);\
} while (0)
/*****************************************************************************/
#define PREPARE_JSON_RESP_STRING(REQ_OBJ,REQ_PACKET,STRING_NAME,STRING_VALUE)\
do			\
{			\
	REQ_OBJ *pPanelReq=NULL;\
	REQ_PACKET* pPanelCmdObj;\
	pPanelReq=(REQ_OBJ *)pReq->pDataObj;\
	pPanelCmdObj=(REQ_PACKET*)pPanelReq->dataRef;\
	if(pPanelReq->result!=RPC_SRV_RESULT_SUCCESS)\
		prepare_result_string(pPanelReq->result,pReq);\
	else\
	{\
		sprintf(pReq->custom_result_string,"%s",pPanelCmdObj->STRING_VALUE);\
		prepare_result_string(pPanelReq->result,pReq,(char*)STRING_NAME,pReq->custom_result_string);\
	}		\
	OBJ_MEM_DELETE(pPanelCmdObj);\
} while (0)
/*****************************************************************************/
#define PREPARE_PANEL_JSON_RESP_STRING(REQ_OBJ,REQ_PACKET,STRING_NAME,STRING_VALUE)\
do			\
{			\
	REQ_OBJ *pPanelReq=NULL;\
	REQ_PACKET* pPanelCmdObj;\
	pPanelReq=(REQ_OBJ *)pReq->pDataObj;\
	pPanelCmdObj=(REQ_PACKET*)pPanelReq->dataRef;\
	if(pPanelReq->result!=EDISP_PANEL_RESULT_SUCCESS)\
		prepare_result_string(pPanelReq->result,pReq);\
	else\
	{\
		sprintf(pReq->custom_result_string,"%s",pPanelCmdObj->STRING_VALUE);\
		prepare_result_string(pPanelReq->result,pReq,(char*)STRING_NAME,pReq->custom_result_string);\
	}		\
	OBJ_MEM_DELETE(pPanelCmdObj);\
} while (0)
/*****************************************************************************/
#define PREPARE_JSON_RESP_ENUM(REQ_OBJ,REQ_PACKET,ENUM_NAME,ENUM_VALUE,ENUM_TABLE,ENUM_MAX)\
do			\
{			\
	REQ_OBJ *pPanelReq=NULL;\
	REQ_PACKET* pPanelCmdObj;\
	pPanelReq=(REQ_OBJ *)pReq->pDataObj;\
	pPanelCmdObj=(REQ_PACKET*)pPanelReq->dataRef;\
	if(pPanelReq->result!=RPC_SRV_RESULT_SUCCESS)\
		prepare_result_string(pPanelReq->result,pReq);\
	else\
	{\
		const char *table[]   = ENUM_TABLE;\
		if(pPanelCmdObj->ENUM_VALUE>ENUM_MAX || pPanelCmdObj->ENUM_VALUE<0)\
				pPanelCmdObj->ENUM_VALUE=ENUM_MAX;\
		sprintf(pReq->custom_result_string,"%s",table[pPanelCmdObj->ENUM_VALUE]);\
		prepare_result_string(pPanelReq->result,pReq,(char*)ENUM_NAME,pReq->custom_result_string);\
	}		\
	OBJ_MEM_DELETE(pPanelCmdObj);\
} while (0)
/*****************************************************************************/
#define PREPARE_JSON_RESP_IN_PROG(REQ_OBJ,REQ_PACKET,TID_NAME)\
do			\
{			\
	REQ_OBJ *pPanelReq=NULL;\
	REQ_PACKET* pPanelCmdObj;\
	pPanelReq=(REQ_OBJ *)pReq->pDataObj;\
	pPanelCmdObj=(REQ_PACKET*)pPanelReq->dataRef;\
	if(pPanelReq->result==RPC_SRV_RESULT_SUCCESS || pPanelReq->result==RPC_SRV_RESULT_IN_PROG)\
	{\
		sprintf(pReq->custom_result_string,"%d",pPanelCmdObj->taskID);	\
		prepare_result_string(pPanelReq->result,pReq,(char*)TID_NAME,pPanelCmdObj->taskID);\
	}		\
	else\
		prepare_result_string(pPanelReq->result,pReq);\
	OBJ_MEM_DELETE(pPanelCmdObj);\
} while (0)

#define PREPARE_JSON_RESP_INT(REQ_OBJ,REQ_PACKET,INT_NAME,INT_VALUE)\
do			\
{			\
	REQ_OBJ *pPanelReq=NULL;\
	REQ_PACKET* pPanelCmdObj;\
	pPanelReq=(REQ_OBJ *)pReq->pDataObj;\
	pPanelCmdObj=(REQ_PACKET*)pPanelReq->dataRef;\
	if(pPanelReq->result==RPC_SRV_RESULT_SUCCESS || pPanelReq->result==RPC_SRV_RESULT_IN_PROG)\
	{\
		sprintf(pReq->custom_result_string,"%d",pPanelCmdObj->INT_VALUE);	\
		prepare_result_string(pPanelReq->result,pReq,(char*)INT_NAME,pPanelCmdObj->INT_VALUE);\
	}		\
	else\
		prepare_result_string(pPanelReq->result,pReq);\
	OBJ_MEM_DELETE(pPanelCmdObj);\
} while (0)

/*****************************************************************************/
//picks up defined string(ENUM_NAME) from the socket data and puts it to ENUM
#define JSON_STRING_TO_ENUM(ENUM_NAME,ENUM_TABLE,ENUM_TYP,ENUM_MAX,ENUM)\
do			\
{			\
	char param_value[255];\
	if(find_single_param((char*)pReq->socket_data,(char*)ENUM_NAME,param_value)!=0)\
	{\
		pReq->mapper_result=CMD_TASK_RESULT_INVALID_PARAM;\
		OBJ_MEM_DELETE(pPanelCmdObj);\
		return -1;\
	}\
	const char *table[]   = ENUM_TABLE;\
	ENUM=(ENUM_TYP)string_to_enum(table,param_value,(int)ENUM_MAX);\
	if(ENUM>=ENUM_MAX)\
	{\
		pReq->mapper_result=CMD_TASK_RESULT_INVALID_PARAM;\
		OBJ_MEM_DELETE(pPanelCmdObj);\
		return -1;\
	}\
} while (0)
/*****************************************************************************/
//picks up defined string(STRING_NAME) from the socket data and puts it to STRING_VALUE
#define JSON_STRING_TO_STRING(STRING_NAME,STRING_VALUE)\
do			\
{			\
	char param_value[255];\
	if(find_single_param((char*)pReq->socket_data,(char*)STRING_NAME,param_value)!=0)\
	{\
		pReq->mapper_result=CMD_TASK_RESULT_INVALID_PARAM;\
		OBJ_MEM_DELETE(pPanelCmdObj);\
		return -1;\
	}\
	strcpy(STRING_VALUE,param_value);\
} while (0)
/*****************************************************************************/
//picks up defined string(STRING_NAME) from the socket data and puts it to STRING_VALUE
#define JSON_STRING_TO_INT(INT_NAME,INT_VALUE)\
do			\
{			\
	char param_value[255];\
	if(find_single_param((char*)pReq->socket_data,(char*)INT_NAME,param_value)!=0)\
	{\
		pReq->mapper_result=CMD_TASK_RESULT_INVALID_PARAM;\
		OBJ_MEM_DELETE(pPanelCmdObj);\
		return -1;\
	}\
	INT_VALUE=atoi(param_value);\
} while (0)
/*****************************************************************************/
//picks up defined string(STRING_NAME) from the socket data and puts it to STRING_VALUE
#define JSON_STRING_TO_DOUBLE(DOUBLE_NAME,DOUBLE_VALUE)\
do			\
{			\
    char param_value[255];\
    if(find_single_param((char*)pReq->socket_data,(char*)DOUBLE_NAME,param_value)!=0)\
    {\
        pReq->mapper_result=CMD_TASK_RESULT_INVALID_PARAM;\
        OBJ_MEM_DELETE(pPanelCmdObj);\
        return -1;\
    }\
    DOUBLE_VALUE=atof(param_value);\
} while (0)
/*****************************************************************************/
//picks up defined string(STRING_NAME) from the socket data and puts it to UNSIGNED_LONG_VALUE
#define JSON_STRING_TO_ULONG(ULONG_NAME,ULONG_VALUE)\
do			\
{			\
	char param_value[255];\
	if(find_single_param((char*)pReq->socket_data,(char*)ULONG_NAME,param_value)!=0)\
	{\
		pReq->mapper_result=CMD_TASK_RESULT_INVALID_PARAM;\
		OBJ_MEM_DELETE(pPanelCmdObj);\
		return -1;\
	}\
	sscanf(param_value,"%lu", &ULONG_VALUE);\
} while (0)
/*****************************************************************************/
//used for data settings handling
//checks if the enum string is correct and converts it to enum else sets the enum to default value
#define SETTING_STRING_TO_ENUM(STRING_IN,ENUM_OUT,ENUM_TYP,ENUM_TABLE,ENUM_MAX,DEFAULT_VAL)\
do			\
{			\
	const char *table[]   = ENUM_TABLE;\
	ENUM_OUT=(ENUM_TYP)string_to_enum(table,STRING_IN,(int)ENUM_MAX);\
	if(ENUM_OUT>=ENUM_MAX)\
	{\
		ENUM_OUT=DEFAULT_VAL;\
	}\
} while (0)
/*****************************************************************************/
//used for data settings handling
//checks if the setting is correctly loaded, else set the variable to default value
#define VALIDATE_SETTING(setting_indx,setting_var,setting_default)\
do			\
{			\
	if(StngTbl[setting_indx].key_state != ESETTINGS_KEY_STATE_LOADED)\
		setting_var=setting_default;\
} while (0)
/*****************************************************************************/
//used for data settings handling
//checks if the enum-setting is correctly loaded, else set the variable to default value
#define VALIDATE_SETTING_ENUM(setting_indx,enum_var,enum_def,enum_str,enum_tbl,enum_type,enum_max)\
do			\
{			\
	if(StngTbl[setting_indx].key_state != ESETTINGS_KEY_STATE_LOADED)\
	{\
		enum_var=enum_def;\
		const char *tbl[]   = enum_tbl;\
		strcpy(enum_str,tbl[enum_var]);\
	}\
	else\
	{\
		const char *tbl[]   = enum_tbl;\
		enum_var=(enum_type)string_to_enum(tbl,enum_str,(int)enum_max);\
		if(enum_var>=enum_max)\
		{\
			enum_var=enum_def;\
		}\
	}\
} while (0)
/*****************************************************************************/
//used for data settings handling
//converts enum into string
#define SETTING_ENUM_TO_STRING(enum_string,enum,enum_tbl)\
do			\
{			\
	const char *tbl[]   = enum_tbl;\
	strcpy(enum_string,tbl[enum]);\
} while (0)
/*****************************************************************************/
//common data objects for deviceTypes and variants
/*****************************************************************************/
typedef enum ADLIB_DEV_CONN_TYPE_T
{
	ADLIB_DEV_CONN_TYPE_DEVNODE,//direct device-node access
	ADLIB_DEV_CONN_TYPE_NETWORK,//connected via rpc
	ADLIB_DEV_CONN_TYPE_UNKNOWN,
	ADLIB_DEV_CONN_TYPE_NONE
}ADLIB_DEV_CONN_TYPE;
/*****************************************************************************/
//common enumeration types
#define ADLIB_STATUS_FLAG_TYPE_TABL      {"lo","hi","unknown","none","\0"}
typedef enum ADLIB_STATUS_FLAG_TYPE_T
{
	ADLIB_STATUS_FLAG_TYPE_LO,
	ADLIB_STATUS_FLAG_TYPE_HI,
	ADLIB_STATUS_FLAG_TYPE_UNKNOWN,
	ADLIB_STATUS_FLAG_TYPE_NONE
}ADLIB_STATUS_FLAG_TYPE;
#endif
