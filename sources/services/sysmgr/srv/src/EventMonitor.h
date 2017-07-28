#ifndef __EVENT_MONITOR_H_
#define __EVENT_MONITOR_H_
#include "ADJsonRpcMgr.hpp"
#include "SysmgrJsonDef.h"
#include "ADCmnStringProcessor.hpp"

//event-monitor-list shall have following lines
//192.168.1.100 42513 13100 -1 DevOpChanged NetSttngChanged IdentStarted IdentFinished UpdateStarted UpdateFinished DiagFinished
//192.168.1.101 42513 13101 -1 DevOpChanged NetSttngChanged IdentStarted IdentFinished UpdateStarted UpdateFinished DiagFinished
//192.168.1.101 42512 12101 -1 CaptureFinished CompareFinished 
//192.168.1.102 42512 12102 -1 CaptureFinished CompareFinished 
struct EvntMonitEntry
{
	std::string ip;//event-senders ip
	int port;//event-sender's port
	int srvToken;//unique token number from my side
	int evntType; //-1 for all
	std::string EvntName[12];//name of the event: limit the event types to 5
	int subscription;//token given by the service side
	bool subscription_sts;//status of the subscription
public:
	EvntMonitEntry(string m_ip,int m_port,int m_srvToken,int m_evntType,string str1="",string str2="",string str3="",string str4="",string str5="",string str6="",string str7="") :ip(m_ip),port(m_port),srvToken(m_srvToken),evntType(m_evntType)
	{
		EvntName[0]="InProgDone";
		EvntName[1]="SrvShutdown";
		EvntName[2]="Reserved1";
		EvntName[3]="Reserved2";
		EvntName[4]="Reserved3";
		EvntName[5]=str1;
		EvntName[6]=str2;
		EvntName[7]=str3;
		EvntName[8]=str4;
		EvntName[9]=str5;
		EvntName[10]=str6;
		EvntName[11]=str7;
		subscription=-1;
		subscription_sts=false;
	}
};
/* ------------------------------------------------------------------------- */
class FindMonitEntry
{
	int myToken;
	//int myEventType;
public:
	//FindMonitEntry(int m_srvToken,int m_evntType):myToken(m_srvToken),myEventType(m_evntType){}
	FindMonitEntry(int m_srvToken):myToken(m_srvToken){}
	bool operator()(EvntMonitEntry pEntry) const
	{
		if(pEntry.srvToken == myToken)// && pEntry.evntType==myEventType)
			return true;
		else
			return false;
	}
};
/* ------------------------------------------------------------------------- */
class  EventMonitor : public ADJsonRpcMgrConsumer, public ADCmnStringProcessor
{
	SYSMGR_CMN_DATA_CACHE *pDataCache;
	std::vector<EvntMonitEntry> EventMonitList;
public:
	 EventMonitor(std::string rpcName,int myIndex,bool emu,bool log,SYSMGR_CMN_DATA_CACHE *pData);
	~ EventMonitor();
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index){};
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index){};
	virtual int ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj){};
	virtual RPC_SRV_RESULT ProcessWorkAsync(int index,unsigned char* pWorkData){};
	virtual void ReceiveEvent(int cltToken,int evntNum,int evntArg,int evntArg2);
	RPC_SRV_RESULT LoadEventMonitList(std::string listFile,std::vector<EvntMonitEntry> *pList);
	RPC_SRV_RESULT SubscribeEvents(std::vector<EvntMonitEntry> *pList);
	RPC_SRV_RESULT UnSubscribeEvents(std::vector<EvntMonitEntry> *pList);
	char* get_timestamp(char* request_timestamp);
};
/* ------------------------------------------------------------------------- */

#endif
