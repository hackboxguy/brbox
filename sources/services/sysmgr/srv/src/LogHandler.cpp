#include "LogHandler.h"
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
/* ------------------------------------------------------------------------- */
LogHandler::LogHandler(std::string filePath):LogFilePath(filePath)
{
	logmsg_chain.attach_helper(this);
}
/* ------------------------------------------------------------------------- */
LogHandler::~LogHandler()
{
	logmsg_chain.remove_all();
}
/* ------------------------------------------------------------------------- */
//chain-callback functions
int LogHandler::identify_chain_element(void* element,int ident,ADChainProducer* pObj)
{
	LOG_MSG_PACKET* pPtr;
	pPtr=(LOG_MSG_PACKET*)element;
	if(pPtr->LineNum==ident)
		return 0;
	else
		return -1;
}
/* ------------------------------------------------------------------------- */
int LogHandler::free_chain_element_data(void* element,ADChainProducer* pObj)
{
	//nothing to free here, coz LOG_MSG_HOLDER doesnt have any dynamically allocated memory pointer
	return 0;
}
/* ------------------------------------------------------------------------- */
int LogHandler::grep_log_message_line(char* log_filepath,char* match_string,int line,char* message)
{
	char command[1024];
	char temp_str[MAX_LOG_MESSAGE_LENGTH];
	//grep SDSRV file | sed '3,3!d' (prints the 3rd line)
	sprintf(command,"grep %s %s | sed '%d,%d!d' ",match_string,log_filepath,line,line);//grep the line-range
	FILE *shell;
	shell= popen(command,"r");
	if(shell == NULL)
		return -1;
	if(fgets(temp_str,MAX_LOG_MESSAGE_LENGTH-2,shell)==NULL)
	{
		pclose(shell);
		return -1;
	}
	temp_str[MAX_LOG_MESSAGE_LENGTH-1]='\0';
	strcpy(message,temp_str);
	pclose(shell);
	return 0;
}
/* ------------------------------------------------------------------------- */
int LogHandler::push_log_message(int line,char* message,ADGenericChain *pChain)
{
	LOG_MSG_PACKET *pHolder=NULL;
	OBJECT_MEM_NEW(pHolder,LOG_MSG_PACKET);
	if(pHolder==NULL)
		return -1;
	pHolder->LineNum=line;
	strcpy(pHolder->Msg,message);
	if(pChain->chain_put((void *)pHolder)!=0)
	{
		OBJ_MEM_DELETE(pHolder);
		//LOG_ERR_MSG("SDSRV:sdsms","SdsmsLog::push_log_message:unable to push item in chain");
		return -1;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int LogHandler::update_logmsg_chain(char* log_filepath,char* match_string,int total_lines)
{
	char msg[MAX_LOG_MESSAGE_LENGTH];
	logmsg_chain.chain_empty();
	for(int i=0;i<total_lines;i++)
	{
		if(grep_log_message_line(log_filepath,match_string,i+1,msg)==0)
			push_log_message(i+1,msg,&logmsg_chain);
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT  LogHandler::read_log_message_count(char* log_filepath,char* match_string,int *TotalMessages)
{
	int count=0;
	char command[1024];
	char temp_str[256];
	sprintf(command,"grep %s %s | sed -n '$='",match_string,log_filepath);//count number of lines
	FILE *shell;
	shell= popen(command,"r");
	if(shell == NULL)
		return RPC_SRV_RESULT_FILE_OPEN_ERR;
	if(fgets(temp_str,250,shell)!=NULL)
	{
		temp_str[255]='\0';
		count=atoi(temp_str);
		pclose(shell);
		update_logmsg_chain(log_filepath,match_string,count);
		*TotalMessages=count;
		return RPC_SRV_RESULT_SUCCESS;
	}
	else
	{
		pclose(shell);
		return RPC_SRV_RESULT_ITEM_NOT_FOUND;
	}
}
/* ------------------------------------------------------------------------- */
int LogHandler::Is_it_valid_file(char* filepath)
{
	struct stat buffer;
	if(stat(filepath,&buffer)!=0)
		return -1;
	if(buffer.st_mode & S_IFREG)
		return 0;
	return -1;//it could be a directory
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT LogHandler::UpdateLogList()
{
	//check if LogFilePath exists
	if(Is_it_valid_file((char*)LogFilePath.c_str())!=0)
		return RPC_SRV_RESULT_FILE_READ_ERR;
	//clean my vector list
	//if (!LogList.empty())
	//	LogList.clear();
	int TotalMessages;//for dummy read
	logmsg_chain.remove_all();
	read_log_message_count((char*)LogFilePath.c_str(),(char*)LOG_MSG_MATCH_STRING,&TotalMessages);
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
int LogHandler::GetLogLines()
{
	return logmsg_chain.get_chain_size();
}
/* ------------------------------------------------------------------------- */
//before calling this function, make sure that chain is updated by calling UpdateLogList
RPC_SRV_RESULT LogHandler::read_log_message_line(int line_num,char* message)
{
	//LOG_MSG_PACKET pPacket;
	LOG_MSG_PACKET *pHolder=NULL;
	logmsg_chain.chain_lock();
	int temp=line_num-1;//caller is calling by zero based index
	pHolder=(LOG_MSG_PACKET*)logmsg_chain.chain_get_by_ident(temp);//line_num);
	if(pHolder==NULL)//no task by this ID is in my list
	{
		logmsg_chain.chain_unlock();
		return RPC_SRV_RESULT_ITEM_NOT_FOUND;
	}
	strcpy(message,pHolder->Msg);
	logmsg_chain.chain_unlock();
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */

