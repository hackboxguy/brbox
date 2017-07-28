#include <algorithm>
#include <mutex>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cerrno>
#include <iomanip>
//#include "ADCmnStringProcessor.hpp"
#include "EventMonitor.h"
#include "ADJsonRpcClient.hpp"
#include "ADCmnPortList.h"
/* ------------------------------------------------------------------------- */
EventMonitor:: EventMonitor(std::string rpcName,int myIndex,bool emu,bool log,SYSMGR_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
	pDataCache=pData;
	if(pDataCache->EvntMonitorConfigFile=="")//nothing to do here if config file is empty
		return;
	LoadEventMonitList(pDataCache->EvntMonitorConfigFile,&EventMonitList);
	SubscribeEvents(&EventMonitList);
}
/* ------------------------------------------------------------------------- */
EventMonitor::~ EventMonitor()
{
	if(pDataCache->EvntMonitorConfigFile=="")//nothing to do here if config file is empty
		return;
	UnSubscribeEvents(&EventMonitList);
}
/* ------------------------------------------------------------------------- */
void EventMonitor::ReceiveEvent(int myToken,int evntNum,int evntArg,int evntArg2)
{
	if(pDataCache->EvntMonitorConfigFile=="")//nothing to do here if config file is empty
		return;
	char mytimestamp[255],myarg1[255],myarg2[255];

	std::vector<EvntMonitEntry>::iterator it = find_if(EventMonitList.begin(), EventMonitList.end(), FindMonitEntry(myToken));
	if(it != EventMonitList.end())
	{
		if(evntNum==1) //1 is for shutdown, indicates that server is going bye-bye, hence disable this entry from my list.
			(*it).subscription_sts=false;
		sprintf(myarg1,"%04d",evntArg);
		sprintf(myarg2,"%04d",evntArg2);
		if(evntNum<=11)//max 11 event types are allowed
			cout<<get_timestamp(mytimestamp)<<" | ip="<<(*it).ip<<" | token="<<(*it).srvToken<<" | evntType="<<setw(25)<<left<<(*it).EvntName[evntNum]<<" | Arg1="<<myarg1<<" | Arg2="<<myarg2<<endl;
	}
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT EventMonitor::LoadEventMonitList(std::string listFile,std::vector<EvntMonitEntry> *pList)
{
	if(listFile.size()<=0)
		return RPC_SRV_RESULT_FAIL;
	std::ifstream infile(listFile);
	std::string line;
	std::string str1,str2,str3,str4,str5,str6,str7;
	while (std::getline(infile, line))
	{
		str1="";str2="";str3="";str4="";str5="";str6="";str7="";
		stringstream iss(line);
		std::string strip,strport,strsrvtoken,strevnttyp;
		iss >> strip;
		iss >> strport;
		iss >> strsrvtoken;
		iss >> strevnttyp;
		iss >> str1;iss >> str2;iss >> str3;iss >> str4;iss >> str5;iss >> str6;iss >> str7;
		int port=atoi(strport.c_str());
		int srvToken=atoi(strsrvtoken.c_str());
		int evntType=atoi(strevnttyp.c_str());
		pList->push_back(EvntMonitEntry(strip,port,srvToken,evntType,str1,str2,str3,str4,str5,str6,str7));
//cout<<"ip="<<strip<<" port="<<port<<" srvToken="<<srvToken<<" evntType="<<evntType<<" str1="<<str1<<" str2="<<str2<<" str3="<<str3<<" str4="<<str4<<" str5="<<str5<<" str6="<<str7<<" str7="<<str6<<endl;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT EventMonitor::SubscribeEvents(std::vector<EvntMonitEntry> *pList)
{
	std::vector<EvntMonitEntry>::iterator it;
	for(it = pList->begin(); it != pList->end(); it++)
	{
		EvntMonitEntry pEntry = (*it);
		pEntry.subscription=-1;
		SUBSCRIBE_EVENT(pEntry.ip,pEntry.port,&pEntry.subscription,pEntry.srvToken,pEntry.evntType,40001);
		if(pEntry.subscription!=-1)
			pEntry.subscription_sts=true;
		else
		{
			char mytimestamp[255];
			cout<<get_timestamp(mytimestamp)<<" | subscription failed for ip="<<pEntry.ip<<" and token="<<pEntry.srvToken<<endl;
			pEntry.subscription_sts=false;
		}
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT EventMonitor::UnSubscribeEvents(std::vector<EvntMonitEntry> *pList)
{
	std::vector<EvntMonitEntry>::iterator it;
	for(it = pList->begin(); it != pList->end(); it++)
	{
		EvntMonitEntry pEntry = (*it);
		if(pEntry.subscription_sts==true)
			UNSUBSCRIBE_EVENT(pEntry.ip,pEntry.port,pEntry.subscription);
		pEntry.subscription=false;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
char* EventMonitor::get_timestamp(char* request_timestamp)
{
	char buffer[30];
	struct timeval tv;
	time_t curtime;
	request_timestamp[0]='\0';
	gettimeofday(&tv, NULL); 
	curtime=tv.tv_sec;
	//strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));
	strftime(buffer,30,"%T.",localtime(&curtime));
	sprintf(request_timestamp,"%s%03ld",buffer,(tv.tv_usec/1000));
	return request_timestamp;
}
/* ------------------------------------------------------------------------- */

