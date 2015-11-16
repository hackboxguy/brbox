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
	//GsmDevDetected=false;
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
			case EXMPP_CMD_SMS_GET        :res=proc_cmd_sms_get(cmd.cmdMsg);break;
			case EXMPP_CMD_SMS_SEND       :res=proc_cmd_sms_send(cmd.cmdMsg);break;
			case EXMPP_CMD_SMS_LIST_UPDATE:res=proc_cmd_sms_list_update(cmd.cmdMsg);break;
			case EXMPP_CMD_SMS_GET_TOTAL  :res=proc_cmd_sms_get_total(cmd.cmdMsg,returnval);break;
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

	//cout<<"user: "<<XmppUserName<<endl;
	//cout<<"pw  : "<<XmppUserPw<<endl;

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
	return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;//RPC_SRV_RESULT_FAIL;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_sms_delete(std::string msg)
{
	return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;//RPC_SRV_RESULT_FAIL;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_sms_get(std::string msg)
{
	return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;//RPC_SRV_RESULT_FAIL;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_sms_send(std::string msg)
{
	return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;//RPC_SRV_RESULT_FAIL;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_sms_list_update(std::string msg)
{
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect("192.168.178.47",ADCMN_PORT_BBOXSMS)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	RPC_SRV_RESULT result=Client.set_action_noarg_type((char*)"delete_all_sms");//SDCS_RPC_NAME_FORCE_ORIG_FACTORY_CALIB);
	Client.rpc_server_disconnect();
	return result;
	//return RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;//RPC_SRV_RESULT_FAIL;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT XmppMgr::proc_cmd_sms_get_total(std::string msg,std::string &returnval)
{
	char temp_str[255];
	ADJsonRpcClient Client;
	if(Client.rpc_server_connect("192.168.178.47",ADCMN_PORT_BBOXSMS)!=0)
		return RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
	//RPC_SRV_RESULT result=Client.set_action_noarg_type((char*)"get_total_sms");//SDCS_RPC_NAME_FORCE_ORIG_FACTORY_CALIB);
	RPC_SRV_RESULT result = Client.get_integer_type((char*)"get_total_sms",(char*)"msgcount",temp_str);
	Client.rpc_server_disconnect();
	//printf("");
	returnval = "5";//atoi(temp_str);
	return result;
}
/* ------------------------------------------------------------------------- */


