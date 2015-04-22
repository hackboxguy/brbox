#ifndef __NET_RPC_H_
#define __NET_RPC_H_
#include "ADJsonRpcMgr.hpp"
/* ------------------------------------------------------------------------- */
class  NetRpc : public ADJsonRpcMgrConsumer
{
public:
	 NetRpc(std::string rpcName,int myIndex);//getRpc,std::string setRpc);
	~ NetRpc();
	virtual int MapJsonToBinary(JsonDataCommObj* pReq,int index);
	virtual int MapBinaryToJson(JsonDataCommObj* pReq,int index);
	virtual int ProcessWork(JsonDataCommObj* pReq,int index);
	virtual int ProcessWorkAsync(unsigned char* pWorkData);
};
/* ------------------------------------------------------------------------- */

#endif
