#include "XmppMgr.h"
//#include <algorithm>
//#include <string>
//#include <mutex>
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
/* ------------------------------------------------------------------------- */
XmppMgr::XmppMgr() //:AckToken(0)
{
	CyclicTime_ms=CLIENT_ALIVE_PING_DURATION_MS;//60000;//60seconds
	event_period_ms=0;
	heartbeat_ms=100;
	pMyTimer=NULL;

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
int XmppMgr::onXmppMessage(std::string msg,ADXmppProducer* pObj)
{
	//process the messages
	//cout<<"msg arrived: "<<msg<<endl;
	if(msg=="Echo") //for checking if client is alive
		XmppProxy.send_reply(msg);
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
	XmppProxy.connect((char*)XmppUserName.c_str(),(char*)XmppUserPw.c_str());
	//cout<<"exiting after connect"<<endl;
	return 0;
}
/* ------------------------------------------------------------------------- */
int XmppMgr::monoshot_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	std::string returnval="";
	RPC_SRV_RESULT res=RPC_SRV_RESULT_UNKNOWN_COMMAND;//RPC_SRV_RESULT_FAIL;
	while (!processCmd.empty())
	{
		XmppCmdEntry cmd = processCmd.front();
		switch(ResolveCmdStr(cmd.cmdMsg))
		{
			case EXMPP_CMD_SMS_DELETE_ALL :res=proc_cmd_sms_deleteall(cmd.cmdMsg);break;
			case EXMPP_CMD_SMS_DELETE     :res=proc_cmd_sms_delete(cmd.cmdMsg);break;
			case EXMPP_CMD_SMS_GET        :res=proc_cmd_sms_get(cmd.cmdMsg,returnval);break;
			case EXMPP_CMD_SMS_SEND       :res=proc_cmd_sms_send(cmd.cmdMsg);break;
			case EXMPP_CMD_SMS_LIST_UPDATE:res=proc_cmd_sms_list_update(cmd.cmdMsg);break;
			case EXMPP_CMD_SMS_GET_TOTAL  :res=proc_cmd_sms_get_total(cmd.cmdMsg,returnval);break;
			case EXMPP_CMD_FMW_GET_VERSION:res=proc_cmd_fmw_get_version(cmd.cmdMsg,returnval);break;
			case EXMPP_CMD_FMW_UPDATE     :res=proc_cmd_fmw_update(cmd.cmdMsg);break;
			case EXMPP_CMD_FMW_UPDATE_STS :res=proc_cmd_fmw_update_sts(cmd.cmdMsg,returnval);break;
			case EXMPP_CMD_FMW_REBOOT     :res=proc_cmd_fmw_reboot(cmd.cmdMsg);break;
			default                       :break;
		}
		processCmd.pop_front();//after processing delete the entry
	}
	const char *resTbl[] = STR_RPC_SRV_RESULT_STRING_TABLE;
	std::string result   = resTbl[res];
	std::string response = result +":"+returnval;
	XmppProxy.send_reply(response);//result+":"+returnval);
	return 0;
}
/* ------------------------------------------------------------------------- */
EXMPP_CMD_TYPES XmppMgr::ResolveCmdStr(std::string msg)
{
	EXMPP_CMD_TYPES xmpcmd;
	const char *cmdTbl[] = EXMPP_CMD_TABL;
	ADCmnStringProcessor string_proc;

	stringstream mstream(msg);
	std::string cmd;
	mstream >> cmd;
	//cout<<mstream.str();//remaining message

	xmpcmd = (EXMPP_CMD_TYPES)string_proc.string_to_enum(cmdTbl,(char*)cmd.c_str(),EXMPP_CMD_UNKNOWN);
	if(xmpcmd>=EXMPP_CMD_UNKNOWN)
		xmpcmd=EXMPP_CMD_UNKNOWN;
	return xmpcmd;
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
RPC_SRV_RESULT XmppMgr::proc_cmd_sms_deleteall(std::string msg)
{
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_BBOXSMS)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result=Client.set_action_noarg_type((char*)"delete_all_sms");
	Client.rpc_server_disconnect();
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
RPC_SRV_RESULT XmppMgr::proc_cmd_sms_send(std::string msg)
{
	return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_sms_list_update(std::string msg)
{
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_BBOXSMS)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result=Client.set_action_noarg_type((char*)"update_sms_list");
	Client.rpc_server_disconnect();
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
RPC_SRV_RESULT XmppMgr::proc_cmd_fmw_update(std::string msg)
{
	//00:06:05.434-->{ "jsonrpc": "2.0", "method": "download_ftp_file", "params": { "srcaddr": "http:\/\/github.com\/hackboxguy\/downloads\/raw\/master\/bbbmmcRbox\/readme.txt", "targetpath": "\/tmp\/readme.txt" }, "id": 0 }
	//00:06:05.454<--{ "jsonrpc": "2.0", "result": { "return": "InProgress", "taskId": 1 }, "id": 0 }
	//this works: wget -O /tmp/uBrBox.uimg http://github.com/hackboxguy/downloads/raw/master/README.md

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
	RPC_SRV_RESULT result = Client.set_double_string_get_single_string_type((char*)"download_ftp_file",
					(char*)"srcaddr",(char*)URLPath.c_str(),
					(char*)"targetpath",(char*)"/tmp/uBrBoxRoot.uimg",
					(char*)"taskID",temp_str);
	//TODO: if resuls is inprog, issue fupdate command as well
	Client.rpc_server_disconnect();
	return result;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_fmw_reboot(std::string msg)
{
	//01:39:52.718-->{ "jsonrpc": "2.0", "method": "set_device_operation", "params": { "operation": "reboot" }, "id": 0 }
	//01:39:52.726<--{ "jsonrpc": "2.0", "result": { "return": "InProgress", "taskId": 1 }, "id": 0 }
	char temp_str[255];temp_str[0]='\0';
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect(bboxSmsServerAddr.c_str(),ADCMN_PORT_SYSMGR)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result = Client.set_single_string_type((char*)"set_device_operation",(char*)"operation",(char*)"reboot");
	Client.rpc_server_disconnect();
	return result;
}
/* ------------------------------------------------------------------------- */
//get-enum-type:(get-async-task-inprog)	RPC_SRV_RESULT res = Client.get_string_type(rpc_name,enum_arg_name,temp_str);
//case EXMPP_CMD_FMW_UPDATE_STS :res=proc_cmd_fmw_update_sts(cmd.cmdMsg,returnval);break;
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
	//return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;
}
/* ------------------------------------------------------------------------- */



