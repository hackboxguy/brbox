#include "NetRpc.h"
/* ------------------------------------------------------------------------- */
NetRpc:: NetRpc(std::string rpcName,int myIndex):ADJsonRpcMgrConsumer(rpcName,myIndex)
{
}
/* ------------------------------------------------------------------------- */
NetRpc::~ NetRpc()
{
}
/* ------------------------------------------------------------------------- */
int NetRpc::MapJsonToBinary(JsonDataCommObj* pReq,int index)
{
	return 0;
}
/* ------------------------------------------------------------------------- */
int NetRpc::MapBinaryToJson(JsonDataCommObj* pReq,int index)
{
	return 0;
}
/* ------------------------------------------------------------------------- */
int NetRpc::ProcessWork(JsonDataCommObj* pReq,int index)
{
	return 0;
}
/* ------------------------------------------------------------------------- */
int NetRpc::ProcessWorkAsync(unsigned char* pWorkData)
{
	return 0;
}
/* ------------------------------------------------------------------------- */


