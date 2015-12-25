#ifndef __LOG_HANDLER_H_
#define __LOG_HANDLER_H_
#include <vector>
#include <iostream>
#include "ADCommon.hpp"
#include "ADGenericChain.hpp"
using namespace std;

//#define LOG_FILE_PATH         "/tmp/messages"
#define LOG_MSG_MATCH_STRING  "BRBOX" //for filtering brbox-services specific messages
#define MAX_LOG_MESSAGE_LENGTH 512
typedef struct LOG_MSG_PACKET_T
{
	int LineNum;
	char Msg[MAX_LOG_MESSAGE_LENGTH];//max log message length is 512chars
	int TotalLines;
}LOG_MSG_PACKET;

class LogHandler  : public ADChainConsumer
{
	ADGenericChain logmsg_chain;
	//std::vector<std::string> LogList;
	std::string LogFilePath;
	RPC_SRV_RESULT  read_log_message_count(char* log_filepath,char* match_string,int *TotalMessages);
	int Is_it_valid_file(char* filepath);
	//Chain-callback functions	
	virtual int identify_chain_element(void* element,int ident,ADChainProducer* pObj);
	virtual int double_identify_chain_element(void* element,int ident1,int ident2,ADChainProducer* pObj){return 0;};
	virtual int free_chain_element_data(void* element,ADChainProducer* pObj);
	int grep_log_message_line(char* log_filepath,char* match_string,int line,char* message);
	int push_log_message(int line,char* message,ADGenericChain *pChain);
	int update_logmsg_chain(char* log_filepath,char* match_string,int total_lines);
public:
	LogHandler(std::string filePath);
	~LogHandler();
	RPC_SRV_RESULT UpdateLogList();
	int GetLogLines();
	RPC_SRV_RESULT read_log_message_line(int line_num,char* message);
};

#endif

