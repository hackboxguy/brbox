#include "CmnRpc.h"
//#include "ADSysInfo.hpp"
//#include "LogHandler.h"
//#include <iostream>
//#include <fstream>
//#include "SysmgrJsonDef.h"
/* ------------------------------------------------------------------------- */
CmnRpc:: CmnRpc(std::string rpcName,int myIndex,bool emu,bool log,SYSMGR_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log,true) //last bool as true indicates, its a command rpc handler function
{
	pDataCache=pData;
}
/* ------------------------------------------------------------------------- */
CmnRpc::~ CmnRpc()
{
}
/* ------------------------------------------------------------------------- */
int CmnRpc::ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj)
{
	EJSON_RPCGMGR_CMD cmd = (EJSON_RPCGMGR_CMD)index;
	//cout<<"CmnRpc::ProcessWork:cmd="<<cmd<<endl;
	switch(cmd)
	{
		//case EJSON_RPCGMGR_GET_TASK_STS            :break;
		//case EJSON_RPCGMGR_GET_RPC_SRV_VERSION     :break;
		//case EJSON_RPCGMGR_TRIGGER_DATASAVE        :break;
		case EJSON_RPCGMGR_GET_SETTINGS_STS        :break;//this gets called
		//case EJSON_RPCGMGR_SHUTDOWN_SERVICE        :break;
		//case EJSON_RPCGMGR_RESET_TASK_STS          :break;
		//case EJSON_RPCGMGR_GET_READY_STS           :break;
		//case EJSON_RPCGMGR_GET_DEBUG_LOG           :break;
		//case EJSON_RPCGMGR_SET_DEBUG_LOG           :break;
		//case EJSON_RPCGMGR_TRIGGER_FACTORY_STORE   :break;
		//case EJSON_RPCGMGR_TRIGGER_FACTORY_RESTORE :break;
		//case EJSON_RPCGMGR_EVENT_SUBSCRIBE         :break;
		//case EJSON_RPCGMGR_EVENT_UNSUBSCRIBE       :break;
		//case EJSON_RPCGMGR_EVENT_NOTIFY            :break;
		//case EJSON_RPCGMGR_EVENT_PROCESS           :break;
		//case EJSON_RPCGMGR_TRIGGER_RUN             :break;
		case EJSON_RPCMGR_GET_DEVOP_STATE          :break;//this gets called
		//case EJSON_RPCMGR_SET_DEVOP_STATE          :break;
		default:break;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT CmnRpc::ProcessWorkAsync(int index,unsigned char* pWorkData)
{
	EJSON_RPCGMGR_CMD cmd = (EJSON_RPCGMGR_CMD)index;
	//cout<<"CmnRpc::ProcessWorkAsync:cmd="<<cmd<<endl;
	switch(cmd)
	{
		//case EJSON_RPCGMGR_GET_TASK_STS            :break;//internally handled
		//case EJSON_RPCGMGR_GET_RPC_SRV_VERSION     :break;//internally handled
		case EJSON_RPCGMGR_TRIGGER_DATASAVE        :break;//this gets called
		//case EJSON_RPCGMGR_GET_SETTINGS_STS        :break;
		//case EJSON_RPCGMGR_SHUTDOWN_SERVICE        :break;//internally handled
		//case EJSON_RPCGMGR_RESET_TASK_STS          :break;//internally handled
		//case EJSON_RPCGMGR_GET_READY_STS           :break;//internally handled
		//case EJSON_RPCGMGR_GET_DEBUG_LOG           :break;//internally handled
		//case EJSON_RPCGMGR_SET_DEBUG_LOG           :break;//internally handled
		case EJSON_RPCGMGR_TRIGGER_FACTORY_STORE   :break;//this gets called
		case EJSON_RPCGMGR_TRIGGER_FACTORY_RESTORE :break;//this gets called
		//case EJSON_RPCGMGR_EVENT_SUBSCRIBE         :break;//internally handled
		//case EJSON_RPCGMGR_EVENT_UNSUBSCRIBE       :break;//internally handled
		//case EJSON_RPCGMGR_EVENT_NOTIFY            :break;//internally handled
		//case EJSON_RPCGMGR_EVENT_PROCESS           :break;//internally handled
		case EJSON_RPCGMGR_TRIGGER_RUN             :break;//this gets called
		//case EJSON_RPCMGR_GET_DEVOP_STATE          :break;
		case EJSON_RPCMGR_SET_DEVOP_STATE          :break;//this gets called
		default:break;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */

