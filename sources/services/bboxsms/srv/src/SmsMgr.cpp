#include "SmsMgr.h"
#include <algorithm>
#include <string>
#include <mutex>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string.h>
volatile GSM_Error sms_send_status;

/* ------------------------------------------------------------------------- */
SmsMgr::SmsMgr() //:AckToken(0)
{
	GsmDevDetected=false;
	SmsProcessThreadID=SmsProcessThread.subscribe_thread_callback(this);
	SmsProcessThread.set_thread_properties(THREAD_TYPE_MONOSHOT,(void *)this);
	SmsProcessThread.start_thread();
	LatestUSSDReply="none";
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
RPC_SRV_RESULT SmsMgr::EmptySmsList()
{
	int index;
	int tot;std::string message,frm;std::vector<MsgEntry> dummyList;
	RPC_SRV_RESULT res=GetOrUpdateTotal(SMS_ACCESS_EMPTY_LIST,tot,index,message,frm,dummyList);
	return res;
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
		case SMS_ACCESS_EMPTY_LIST:
			if (!msgList.empty())
				msgList.clear();
			break;
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
/* Handler for SMS send reply */
void send_sms_callback (GSM_StateMachine *sm, int status, int MessageReference, void * user_data)
{
//	printf("Sent SMS on device: \"%s\"\n", GSM_GetConfig(sm, -1)->Device);
	if (status==0) 
	{
//		printf("..OK");
		sms_send_status = ERR_NONE;
	}
	else
	{
//		printf("..error %i", status);
		sms_send_status = ERR_UNKNOWN;
	}
//	printf(", message reference=%d\n", MessageReference);
}
//http://wammu.eu/docs/manual/c/examples.html
int SmsMgr::SendSms(char* recipient_number,char* message_text)
{
	int return_value=-1;

	GSM_SMSMessage sms;
	GSM_SMSC PhoneSMSC;

	GSM_StateMachine *s;
	//INI_Section *cfg;
	GSM_Error error;


	GSM_Debug_Info *debug_info;
	//gboolean start;
	//GSM_MultiSMSMessage 	sms;
	//int i;

	GSM_InitLocales(NULL);

	/* disable global debugging to stderr */
	debug_info = GSM_GetGlobalDebug();
	GSM_SetDebugFileDescriptor(stderr, FALSE, debug_info);
	//GSM_SetDebugLevel("textall", debug_info);

	/* Prepare message */
	/* Cleanup the structure */
	memset(&sms, 0, sizeof(sms));
	/* Encode message text */
	EncodeUnicode(sms.Text, message_text, strlen(message_text));
	/* Encode recipient number */
	EncodeUnicode(sms.Number, recipient_number, strlen(recipient_number));
	/* We want to submit message */
	sms.PDU = SMS_Submit;
	/* No UDH, just a plain message */
	sms.UDH.Type = UDH_NoUDH;
	/* We used default coding for text */
	sms.Coding = SMS_Coding_Default_No_Compression;
	/* Class 1 message (normal) */
	sms.Class = 1;


	/* Allocates state machine */
	s = GSM_AllocStateMachine();
	if (s == NULL)
		return -1;//3;

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

	GSM_SetSendSMSStatusCallback(s, send_sms_callback, NULL);

	/* We need to know SMSC number */
	PhoneSMSC.Location = 1;
	error = GSM_GetSMSC(s, &PhoneSMSC);
	if(error_handler(s,error)!=0)
		return -1;

	/* Set SMSC number in message */
	CopyUnicodeString(sms.SMSC.Number, PhoneSMSC.Number);

	/*
	 * Set flag before callind SendSMS, some phones might give
	 * instant response
	 */
	sms_send_status = ERR_TIMEOUT;

	/* Send message */
	error = GSM_SendSMS(s, &sms);
	if(error_handler(s,error)!=0)
		return -1;

	int timeout=0;
	/* Wait for network reply */
	while (timeout++ <50) //wait for 5seconds before failing
	{
		GSM_ReadDevice(s, TRUE);
		if (sms_send_status == ERR_NONE) 
		{
			/* Message sent OK */
			return_value = 0;
			break;
		}
		if (sms_send_status != ERR_TIMEOUT) 
		{
			/* Message sending failed */
			return_value = -1;//100;
			break;
		}
		usleep(100000);
		//cout<<"waiting for send "<<timeout<<endl;
	}

	/* Terminate connection */
	error = GSM_TerminateConnection(s);
	if(error_handler(s,error)!=0)
		return -1;

	/* Free up used memory */
	GSM_FreeStateMachine(s);


	return return_value;
}


volatile int TerminateID = -1;
void IncomingCall0(GSM_StateMachine *sm UNUSED, GSM_Call *call, void * user_data)
{
        if (call->CallIDAvailable) 
	{
                TerminateID = call->CallID;
        }
}

int SmsMgr::DialVoice(char* recipient_number)
{
	GSM_StateMachine *gsm;
	GSM_Error error;
	GSM_InitLocales(NULL);

	/* disable global debugging to stderr */
	GSM_Debug_Info *debug_info;
	debug_info = GSM_GetGlobalDebug();
	GSM_SetDebugFileDescriptor(stderr, FALSE, debug_info);
	//GSM_SetDebugLevel("textall", debug_info);

	gsm = GSM_AllocStateMachine();

	if (gsm == NULL)
		return -1;

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

	GSM_CallShowNumber ShowNumber = GSM_CALL_DefaultNumberPresence;
	//GSM_CallShowNumber ShowNumber = GSM_CALL_ShowNumber;
	//GSM_CallShowNumber ShowNumber = GSM_CALL_HideNumber;


	TerminateID = -1;
	GSM_SetIncomingCallCallback(gsm, IncomingCall0, NULL);
	error = GSM_SetIncomingCall(gsm, TRUE);
	if(error_handler(gsm,error)!=0)
		return -1;
	//Print_Error(error);
	error = GSM_DialVoice(gsm, recipient_number, ShowNumber);
	if(error_handler(gsm,error)!=0)
		return -1;
	//Print_Error(error);
	//sleep(GetInt(argv[3]));
	int timeout=0;
	while (timeout++ <300) //wait for 30seconds and then hangon
	{
		//cout<<"DialVoice waiting "<<timeout<<endl;
		usleep(100000);
	}

	GSM_ReadDevice(gsm, TRUE);
	if (TerminateID != -1)
	{
		error = GSM_CancelCall(gsm, TerminateID, FALSE);
		if(error_handler(gsm,error)!=0)
			return -1;
	}
	else
	{
		error = GSM_CancelCall(gsm, 0, TRUE);
		if(error_handler(gsm,error)!=0)
			return -1;
	}

	/* Terminate connection */
	error = GSM_TerminateConnection(gsm);
	if(error_handler(gsm,error)!=0)
		return -1;

	/* Free up used memory */
	GSM_FreeStateMachine(gsm);
	return 0;
}

bool ussdCodeArrived=false;
char ussdCode[1024];
volatile int num_replies = 0;
void IncomingUSSD(GSM_StateMachine *sm UNUSED, GSM_USSDMessage *ussd, void *user_data)
{
        //printf("%s\n", ("USSD received"));
        //printf(LISTFORMAT, ("Status"));

        switch(ussd->Status) {
                case USSD_NoActionNeeded:
                        printf("%s\n", ("No action needed"));
                        break;
                case USSD_ActionNeeded:
                        printf("%s\n", ("Action needed"));
                        break;
                case USSD_Terminated:
                        printf("%s\n", ("Terminated"));
                        break;
                case USSD_AnotherClient:
                        printf("%s\n", ("Another client replied"));
                        break;
                case USSD_NotSupported:
                        printf("%s\n", ("Not supported"));
                        break;
                case USSD_Timeout:
                        printf("%s\n", ("Timeout"));
                        break;
//#ifndef CHECK_CASES
                default:
//#endif
//                case USSD_Unknown:
                        printf("%s\n", ("Unknown"));
                        break;
        }

	strcpy(ussdCode,DecodeUnicodeConsole(ussd->Text));
	ussdCodeArrived=true;
//        printf(LISTFORMAT "\"%s\"\n", ("Service reply"), DecodeUnicodeConsole(ussd->Text));
        //printf("IncomingUSSD: %s",DecodeUnicodeConsole(ussd->Text));
        //fflush(stdout);
}
void IncomingUSSD2(GSM_StateMachine *sm, GSM_USSDMessage *ussd, void * user_data)
{
        IncomingUSSD(sm, ussd, user_data);
        num_replies++;
}
int SmsMgr::DialUSSDCode(char* code,char* return_str)
{
//	cout<<"ussd code to be sent is = "<<code<<endl;
	//ussdCode[0]='\0';
	sprintf(ussdCode,"no-ussd-code");
	ussdCodeArrived=false;

        int last_replies;
        time_t last_reply;

	GSM_StateMachine *gsm;
	GSM_Error error;
	GSM_InitLocales(NULL);

	/* disable global debugging to stderr */
	GSM_Debug_Info *debug_info;
	debug_info = GSM_GetGlobalDebug();
	GSM_SetDebugFileDescriptor(stderr, FALSE, debug_info);
	//GSM_SetDebugLevel("textall", debug_info);

	gsm = GSM_AllocStateMachine();
//cout<<"DialUSSDCode = 0"<<endl;
	if (gsm == NULL)
		return -1;
//cout<<"DialUSSDCode = 1"<<endl;

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
//cout<<"DialUSSDCode = 2"<<endl;
	if(error_handler(gsm,error)!=0)
		return -1;
//cout<<"DialUSSDCode = 3"<<endl;


	GSM_SetIncomingUSSDCallback(gsm, IncomingUSSD2, NULL);
	error=GSM_SetIncomingUSSD(gsm,TRUE);
//cout<<"DialUSSDCode = 4"<<endl;
	if(error_handler(gsm,error)!=0)
		return -1;
//cout<<"DialUSSDCode = 5"<<endl;

	error=GSM_DialService(gsm, code);
	// Fallback to voice call, it can work with some phones 
	if (error == ERR_NOTIMPLEMENTED || error == ERR_NOTSUPPORTED)
	{
//cout<<"DialUSSDCode = falling back to voice call"<<endl;
		error=GSM_DialVoice(gsm, code, GSM_CALL_DefaultNumberPresence);
	}
//cout<<"DialUSSDCode = 6"<<endl;
	if(error_handler(gsm,error)!=0)
		return -1;
//cout<<"DialUSSDCode = 7"<<endl;


	num_replies = 0;
	last_replies = 0;
	last_reply = time(NULL);
	int timeout=0;
	//while (1)//!gshutdown)
	while (timeout++ <300) //wait for 30seconds and then hangon
	{
		/*if (num_replies != last_replies) 
		{
			last_replies = num_replies;
			last_reply = time(NULL);
		}
		else if (num_replies == 0 && difftime(time(NULL), last_reply) > 60)
		{
			// Wait one minute for reply
			break;//gshutdown = TRUE;
		}
		else if (num_replies > 0 && difftime(time(NULL), last_reply) > 30)
		{
			// Wait for consequent replies for 30 seconds
			break;//gshutdown = TRUE;
		}*/
		if(ussdCodeArrived==true)break;
		GSM_ReadDevice(gsm, FALSE);
		//cout<<"read-ussd code: in while loop"<<endl;
		usleep(100000);
	}
        error=GSM_SetIncomingUSSD(gsm, FALSE);
	if(error_handler(gsm,error)!=0)
		return -1;


	strcpy(return_str,ussdCode);
	cout<<"ussd-code = "<<return_str<<endl;
	LatestUSSDReply=ussdCode;
	/* Terminate connection */
	error = GSM_TerminateConnection(gsm);
	if(error_handler(gsm,error)!=0)
		return -1;

	/* Free up used memory */
	GSM_FreeStateMachine(gsm);

	if(timeout>=300)
		return -1;//wait timed out: ussd code didnt arrive
	return 0;
}
RPC_SRV_RESULT SmsMgr::GetLatestUSSDReply(char* msg)
{
	strcpy(msg,LatestUSSDReply.c_str());
	return RPC_SRV_RESULT_SUCCESS;
}
/*void GetUSSD(int argc UNUSED, char *argv[])
{
        GSM_Error error;
        int last_replies;
        time_t last_reply;

        GSM_Init(TRUE);

        signal(SIGINT, interrupt);
        fprintf(stderr, "%s\n", _("Press Ctrl+C to break..."));
        fflush(stderr);

        GSM_SetIncomingUSSDCallback(gsm, IncomingUSSD2, NULL);

        error=GSM_SetIncomingUSSD(gsm,TRUE);
        Print_Error(error);

        error=GSM_DialService(gsm, argv[2]);
        // Fallback to voice call, it can work with some phones 
        if (error == ERR_NOTIMPLEMENTED || error == ERR_NOTSUPPORTED) {
                error=GSM_DialVoice(gsm, argv[2], GSM_CALL_DefaultNumberPresence);
        }
        Print_Error(error);

        num_replies = 0;
        last_replies = 0;
        last_reply = time(NULL);
        while (!gshutdown) {
                if (num_replies != last_replies) {
                        last_replies = num_replies;
                        last_reply = time(NULL);
                } else if (num_replies == 0 && difftime(time(NULL), last_reply) > 60) {
                        // Wait one minute for reply
                        gshutdown = TRUE;
                } else if (num_replies > 0 && difftime(time(NULL), last_reply) > 30) {
                        // Wait for consequent replies for 30 seconds
                        gshutdown = TRUE;
                }
                GSM_ReadDevice(gsm, FALSE);
        }

        error=GSM_SetIncomingUSSD(gsm, FALSE);
        Print_Error(error);
        GSM_Terminate();
}*/


/*
http://wammu.eu/docs/manual/c/examples.html
*/

