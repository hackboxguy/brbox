#ifndef __NET_RPC_H_
#define __NET_RPC_H_
#include "ADJsonRpcMgr.hpp"
#include "SysmgrJsonDef.h"
#include "ADCmnStringProcessor.hpp"
/* ------------------------------------------------------------------------- */
class  NetRpc : public ADJsonRpcMgrConsumer, public ADCmnStringProcessor
{
public:
	 NetRpc(std::string rpcName,int myIndex,bool emu,bool log);//getRpc,std::string setRpc);
	~ NetRpc();
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index);
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index);
	virtual int ProcessWork(JsonDataCommObj* pReq,int index);
	virtual int ProcessWorkAsync(unsigned char* pWorkData);

	int json_to_bin_get_mac_addr(JsonDataCommObj* pReq);
	int bin_to_json_get_mac_addr(JsonDataCommObj* pReq);
	int process_get_mac_addr(JsonDataCommObj* pReq);

	int json_to_bin_set_mac_addr(JsonDataCommObj* pReq);
	int bin_to_json_set_mac_addr(JsonDataCommObj* pReq);
	int process_set_mac_addr(JsonDataCommObj* pReq);

	//returns total number of available eth interfaces(except local loopback)
	int json_to_bin_get_eth_count(JsonDataCommObj* pReq);
	int bin_to_json_get_eth_count(JsonDataCommObj* pReq);
	int process_get_eth_count(JsonDataCommObj* pReq);

	//returns name of the eth interface for a given index
	int json_to_bin_get_eth_name(JsonDataCommObj* pReq);
	int bin_to_json_get_eth_name(JsonDataCommObj* pReq);
	int process_get_eth_name(JsonDataCommObj* pReq);
};
/* ------------------------------------------------------------------------- */

#endif
