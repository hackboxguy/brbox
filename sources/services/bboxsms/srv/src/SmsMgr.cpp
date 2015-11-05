#include "SmsMgr.h"
#include <algorithm>
#include <string>
#include <mutex>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string.h>

/* ------------------------------------------------------------------------- */
SmsMgr::SmsMgr() //:AckToken(0)
{
	GsmDevDetected=false;
	SmsProcessThreadID=SmsProcessThread.subscribe_thread_callback(this);
	SmsProcessThread.set_thread_properties(THREAD_TYPE_MONOSHOT,(void *)this);
	SmsProcessThread.start_thread();
	//processThreadID=EventProcessThread.subscribe_thread_callback(this);
	//EventProcessThread.set_thread_properties(THREAD_TYPE_MONOSHOT,(void *)this);
	//EventProcessThread.start_thread();
}
SmsMgr::~SmsMgr()
{
	SmsProcessThread.stop_thread();
}
/* ------------------------------------------------------------------------- */
int SmsMgr::monoshot_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	//todo:
	return 0;
}
RPC_SRV_RESULT SmsMgr::GetSms(int index,char* msg)
{
	//strcpy(msg,"testmsg1");
	int tot;std::string message,frm;std::vector<MsgEntry> dummyList;
	RPC_SRV_RESULT res=GetOrUpdateTotal(SMS_ACCESS_READ_INDEX,tot,index,message,frm,dummyList);
	strcpy(msg,message.c_str());
	return res;//RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT SmsMgr::GetTotalSms(int *total)
{
	int tot;std::string msg,frm;std::vector<MsgEntry> dummyList;
	RPC_SRV_RESULT res=GetOrUpdateTotal(SMS_ACCESS_COUNT_LIST,tot,0,msg,frm,dummyList);
	*total=tot;
	return res;//RPC_SRV_RESULT_SUCCESS;
}
//RAII function, used for reading total messages or updating the msgList from different threads
RPC_SRV_RESULT SmsMgr::GetOrUpdateTotal(SMS_ACCESS_TYPE access,int &total,int indx,std::string &msg,std::string &from,std::vector<MsgEntry> &newList) 
{
	// mutex to protect cmn resource access (shared across threads)
	static std::mutex mutex;
	// lock mutex before accessing file
	std::lock_guard<std::mutex> lock(mutex);

	switch(access)
	{
		case SMS_ACCESS_UPDATE_LIST:
			if (!msgList.empty())
				msgList.clear();
			for(int i=0;i<newList.size();i++)
			{
				msgList.push_back(MsgEntry(newList[i].Message,newList[i].From,newList[i].Location,newList[i].Folder));
			}
			break;
		case SMS_ACCESS_READ_LAST  :
			if (msgList.empty())
				return RPC_SRV_RESULT_FILE_EMPTY;
			msg  = msgList.back().Message;
			from = msgList.back().From;
			break;//read the latest sms
		case SMS_ACCESS_READ_INDEX :
			if (msgList.empty())
				return RPC_SRV_RESULT_FILE_EMPTY;
			if(indx>=msgList.size())//zero based index
				return RPC_SRV_RESULT_VALUE_OUT_OF_RANGE;
			msg  = msgList[indx].Message;
			from = msgList[indx].From;
			break;//read the sms from index(Nth sms from 0)
		case SMS_ACCESS_COUNT_LIST :total=msgList.size();break;
		default                    :return RPC_SRV_RESULT_ARG_ERROR;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
int SmsMgr::error_handler(GSM_StateMachine *s,GSM_Error error)
{
	if (error != ERR_NONE)
	{
		//printf("ERROR: %s\n", GSM_ErrorString(error));//TODO: log this message
		if (GSM_IsConnected(s))
			GSM_TerminateConnection(s);
		return -1;//exit(error);
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int SmsMgr::DetectSmsDevice()
{
	GsmDevDetected=false;
	GSM_StateMachine *s;
	//INI_Section *cfg;
	GSM_Error error;
	char buffer[100];

	GSM_Debug_Info *debug_info;
	/*
	 * We don't need gettext, but need to set locales so that
	 * charset conversion works.
	 */
	GSM_InitLocales(NULL);

	/* disable global debugging to stderr */
	debug_info = GSM_GetGlobalDebug();
	GSM_SetDebugFileDescriptor(stderr, FALSE, debug_info);
	//GSM_SetDebugLevel("textall", debug_info);

	/* Allocates state machine */
	s = GSM_AllocStateMachine();
	if (s == NULL)
		return -1;//3;

	/*
	 * Enable state machine debugging to stderr
	 * Same could be achieved by just using global debug config.
	 */
	debug_info = GSM_GetDebug(s);
	GSM_SetDebugGlobal(FALSE, debug_info);
	GSM_SetDebugFileDescriptor(stderr, FALSE, debug_info);
	GSM_SetDebugLevel("textall", debug_info);

	/*
	 * Find configuration file (first command line parameter or
	 * defaults)
	 */
//	error = GSM_FindGammuRC(&cfg,NULL);// argc == 2 ? argv[1] : NULL);
//	if(error_handler(s,error)!=0)
//		return -1;

	/* Read it */
//	error = GSM_ReadConfig(cfg, GSM_GetConfig(s, 0), 0);
//	if(error_handler(s,error)!=0)
//		return -1;
	/* Free config file structures */
//	INI_Free(cfg);
	
	//following lines are used when /home/user/.gammurc is not available
	GSM_Config *cfg;
	cfg = GSM_GetConfig(s, 0);
	free(cfg->Device);
	cfg->Device     = strdup(MODEM_DEV_NODE);//argv[1]);
	free(cfg->Connection);
	cfg->Connection = strdup(MODEM_AT_CONN);//argv[2]);
	cfg->SyncTime   = TRUE;
	//strcpy(cfg->Model, argv[3]);
	/* We have one valid configuration */
	GSM_SetConfigNum(s, 1);

	/* Connect to phone */
	/* 1 means number of replies you want to wait for */
	error = GSM_InitConnection(s, 1);
	if(error_handler(s,error)!=0)
		return -1;


	/* Here you can do some stuff with phone... */

	/* As an example we read some information about phone: */

	/* Manufacturer name */
	error = GSM_GetManufacturer(s, buffer);
	if(error_handler(s,error)!=0)
		return -1;
	//printf("Manufacturer  : %s\n", buffer);
	GsmDevManf=buffer;//gsm device manufacturer

	/* Model name */
	error = GSM_GetModel(s, buffer);
	if(error_handler(s,error)!=0)
		return -1;
	//printf("Model         : %s (%s)\n",
	//	GSM_GetModelInfo(s)->model,
	//	buffer);
	GsmDevModel=buffer;//;//gsm device model number

	/* Terminate connection */
	error = GSM_TerminateConnection(s);
	if(error_handler(s,error)!=0)
		return -1;

	/* Free up used memory */
	GSM_FreeStateMachine(s);

	GsmDevDetected=true;//device detection is success
	//cout<<"DevManf : "<<GsmDevManf<<" Model : "<<GsmDevModel<<endl;
	return 0;
}
/* ------------------------------------------------------------------------- */
//http://wammu.eu/docs/manual/c/examples.html
int SmsMgr::ReadSms(int indx)
{
	std::vector<MsgEntry> myList;

	GSM_StateMachine *s;
	//INI_Section *cfg;
	GSM_Error error;

	GSM_Debug_Info *debug_info;
	gboolean start;
	GSM_MultiSMSMessage 	sms;
	int i;

	/* Register signal handler */
	//signal(SIGINT, interrupt);
	//signal(SIGTERM, interrupt);

	/*
	 * We don't need gettext, but need to set locales so that
	 * charset conversion works.
	 */
	GSM_InitLocales(NULL);

	/* disable global debugging to stderr */
	debug_info = GSM_GetGlobalDebug();
	GSM_SetDebugFileDescriptor(stderr, FALSE, debug_info);
	//GSM_SetDebugLevel("textall", debug_info);


	/* Allocates state machine */
	s = GSM_AllocStateMachine();
	if (s == NULL)
		return -1;//3;

	/*
	 * Enable state machine debugging to stderr
	 * Same could be achieved by just using global debug config.
	 */
	//debug_info = GSM_GetDebug(s);
	//GSM_SetDebugGlobal(FALSE, debug_info);
	//GSM_SetDebugFileDescriptor(stderr, TRUE, debug_info);
	//GSM_SetDebugLevel("textall", debug_info);

	/*
	 * Find configuration file (first command line parameter or
	 * defaults)
	 */
//	error = GSM_FindGammuRC(&cfg, NULL);//argc == 2 ? argv[1] : NULL);
//	if(error_handler(s,error)!=0)
//		return -1;
	/* Read it */
//	error = GSM_ReadConfig(cfg, GSM_GetConfig(s, 0), 0);
//	if(error_handler(s,error)!=0)
//		return -1;
	/* Free config file structures */
//	INI_Free(cfg);

	GSM_Config *cfg;
	cfg = GSM_GetConfig(s, 0);
	free(cfg->Device);
	cfg->Device     = strdup(MODEM_DEV_NODE);//argv[1]);
	free(cfg->Connection);
	cfg->Connection = strdup(MODEM_AT_CONN);//argv[2]);
	cfg->SyncTime   = TRUE;
	/* We have one valid configuration */
	GSM_SetConfigNum(s, 1);

	/* Connect to phone */
	/* 1 means number of replies you want to wait for */
	error = GSM_InitConnection(s, 1);
	if(error_handler(s,error)!=0)
		return -1;

	/* Read all messages */
	error = ERR_NONE;
	start = TRUE;
	sms.Number = 0;
	sms.SMS[0].Location = 0;
	sms.SMS[0].Folder = 0;
	int total_messages=0;

	std::string msg;//=sms.SMS[i].Text;
	std::string frm;//=sms.SMS[i].Number;

	while (error == ERR_NONE)// && !gshutdown) 
	{
		error = GSM_GetNextSMS(s, &sms, start);
		if (error == ERR_EMPTY) break;
		if(error_handler(s,error)!=0)
			return -1;
		start = FALSE;

		/* Now we can do something with the message */
		for (i = 0; i < sms.Number; i++) 
		{
			total_messages++;
			//printf("Location: %d, Folder: %d\n", sms.SMS[i].Location, sms.SMS[i].Folder);
			//printf("Number: \"%s\"\n", DecodeUnicodeConsole(sms.SMS[i].Number));
			frm=DecodeUnicodeConsole(sms.SMS[i].Number);
			/*
			 * Decoding with GSM_DecodeMultiPartSMS is also an option here,
			 * but for simplicity we use this approach which will handle only
			 * text messages.
			 */
			if (sms.SMS[i].Coding == SMS_Coding_8bit) 
			{
				//printf("8-bit message, can not display\n");
				msg="Error!! 8bit msg";
			}
			else
			{
				//printf("Text: \"%s\"\n", DecodeUnicodeConsole(sms.SMS[i].Text));
				msg=DecodeUnicodeConsole(sms.SMS[i].Text);
			}
			//printf("\n");
			myList.push_back(MsgEntry(msg,frm,sms.SMS[i].Location,sms.SMS[i].Folder));
		}
	}

	/* Terminate connection */
	error = GSM_TerminateConnection(s);
	if(error_handler(s,error)!=0)
		return -1;

	/* Free up used memory */
	GSM_FreeStateMachine(s);

	int dummyTotal;
	GetOrUpdateTotal(SMS_ACCESS_UPDATE_LIST,dummyTotal,0,msg,frm,myList);
	return total_messages;//0;
}
int SmsMgr::DeleteAllSMS(int foldernum)
//void DeleteAllSMS(int foldernum) //int argc, char *argv[])
{
	GSM_StateMachine *gsm;

	GSM_MultiSMSMessage 	sms;
	GSM_SMSFolders		folders;
	//int			foldernum;
	gboolean		start = TRUE;
	GSM_Error error;

	//GSM_Init(TRUE);
	GSM_InitLocales(NULL);
	/* disable global debugging to stderr */
	//debug_info = GSM_GetGlobalDebug();
	//GSM_SetDebugFileDescriptor(stderr, FALSE, debug_info);
	//GSM_SetDebugLevel("textall", debug_info);
	/* Allocates state machine */
	gsm = GSM_AllocStateMachine();
	if (gsm == NULL)
		return -1;//3;

	GSM_Config *cfg;
	cfg = GSM_GetConfig(gsm, 0);
	free(cfg->Device);
	cfg->Device     = strdup(MODEM_DEV_NODE);//argv[1]);
	free(cfg->Connection);
	cfg->Connection = strdup(MODEM_AT_CONN);//argv[2]);
	cfg->SyncTime   = TRUE;
	/* We have one valid configuration */
	GSM_SetConfigNum(gsm, 1);
	/* Connect to phone */
	/* 1 means number of replies you want to wait for */
	error = GSM_InitConnection(gsm, 1);
	if(error_handler(gsm,error)!=0)
		return -1;



	error=GSM_GetSMSFolders(gsm, &folders);
	//Print_Error(error);

	//GetStartStop(&foldernum, NULL, 2, argc, argv);

	if (foldernum > folders.Number) 
	{
		//printf(_("Too high folder number (max. %i)\n"),folders.Number);
		//fflush(stdout);
		//GSM_Terminate();
		//Terminate(2);
		error = GSM_TerminateConnection(gsm);
		if(error_handler(gsm,error)!=0)
			return -1;
		GSM_FreeStateMachine(gsm);
		return -1;
	}

	//printf(_("Deleting SMS from \"%s\" folder: "),DecodeUnicodeConsole(folders.Folder[foldernum-1].Name));
	//fflush(stdout);

	while (error == ERR_NONE) 
	{
		sms.SMS[0].Folder=0x00;
		error=GSM_GetNextSMS(gsm, &sms, start);
		switch (error)
		{
			case ERR_EMPTY:
				break;
			case ERR_CORRUPTED:
				//printf(stderr, "\n%s\n", _("Corrupted message, skipping"));
				error = ERR_NONE;
				continue;
			default:
				//Print_Error(error);
				if (sms.SMS[0].Folder == foldernum) 
				{
					sms.SMS[0].Folder=0x00;
					error=GSM_DeleteSMS(gsm, &sms.SMS[0]);
					//Print_Error(error);
					//printf("*");
				}
		}
		start=FALSE;
	}
	//printf("\n");
	//fflush(stdout);
	//GSM_Terminate();

	/* Terminate connection */
	error = GSM_TerminateConnection(gsm);
	if(error_handler(gsm,error)!=0)
		return -1;

	/* Free up used memory */
	GSM_FreeStateMachine(gsm);
	return 0;

}
//todo:
//1)dont use /home/user/gammu.rc file, instead user your own file 
//2)add sms-update-list rpc
//3)add sms-delete rpc
//4)get sms shall return msg + from (two strings)
//5)add get-latest-sms rpc

/*
http://wammu.eu/docs/manual/c/examples.html

adav@adav-private-linux:~$ sudo gammu --identify
Device               : /dev/ttyUSB0
Manufacturer         : Huawei
Model                : E173 (E173)
Firmware             : 11.126.29.00.76
IMEI                 : 867749018934194
SIM IMSI             : 262075201404740


Location: 100002, Folder: 3
Number: "+491726110283"
Text: "Another test msg"

Location: 100003, Folder: 3
Number: "+491726110283"
Text: "Here comes another for testing"

*/

