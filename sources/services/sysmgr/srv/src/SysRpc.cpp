#include "SysRpc.h"
#include "ADSysInfo.hpp"
//#include "SysmgrJsonDef.h"
/* ------------------------------------------------------------------------- */
SysRpc:: SysRpc(std::string rpcName,int myIndex,bool emu,bool log):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
}
/* ------------------------------------------------------------------------- */
SysRpc::~ SysRpc()
{
}
/* ------------------------------------------------------------------------- */
int SysRpc::MapJsonToBinary(JsonDataCommObj* pReq,int index)
{
	//printf("SysRpc::MapJsonToBinary called\n");
	EJSON_SYSMGR_RPC_TYPES command =(EJSON_SYSMGR_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_SYSMGR_RPC_GET_LOADINFO :return json_to_bin_get_loadinfo(pReq);
		case EJSON_SYSMGR_RPC_GET_MEMINFO  :return json_to_bin_get_meminfo(pReq);
		case EJSON_SYSMGR_RPC_GET_CPUINFO  :return json_to_bin_get_cpuinfo(pReq);
		default:break;
	}
	return -1;//0;
}
/* ------------------------------------------------------------------------- */
int SysRpc::MapBinaryToJson(JsonDataCommObj* pReq,int index)
{
	//printf("SysRpc::MapBinaryToJson called\n");
	EJSON_SYSMGR_RPC_TYPES command =(EJSON_SYSMGR_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_SYSMGR_RPC_GET_LOADINFO :return bin_to_json_get_loadinfo(pReq);
		case EJSON_SYSMGR_RPC_GET_MEMINFO  :return bin_to_json_get_meminfo(pReq);
		case EJSON_SYSMGR_RPC_GET_CPUINFO  :return bin_to_json_get_cpuinfo(pReq);
		default: break;
	}
	return -1;
}
/* ------------------------------------------------------------------------- */
int SysRpc::ProcessWork(JsonDataCommObj* pReq,int index)
{
	//printf("SysRpc::ProcessWork called\n");
	EJSON_SYSMGR_RPC_TYPES command =(EJSON_SYSMGR_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_SYSMGR_RPC_GET_LOADINFO :return process_get_loadinfo(pReq);
		case EJSON_SYSMGR_RPC_GET_MEMINFO  :return process_get_meminfo(pReq);
		case EJSON_SYSMGR_RPC_GET_CPUINFO  :return process_get_cpuinfo(pReq);
		default:break;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int SysRpc::ProcessWorkAsync(unsigned char* pWorkData)
{
	return 0;
}
/* ------------------------------------------------------------------------- */
int SysRpc::json_to_bin_get_loadinfo(JsonDataCommObj* pReq)
{
	SYSMGR_LOAD_INFO_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_LOAD_INFO_PACKET,RPC_SRV_ACT_READ,EJSON_SYSMGR_RPC_GET_LOADINFO);
	return 0;
}
int SysRpc::bin_to_json_get_loadinfo(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	SYSMGR_LOAD_INFO_PACKET* pPanelCmdObj;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	pPanelCmdObj=(SYSMGR_LOAD_INFO_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->result!=RPC_SRV_RESULT_SUCCESS)
		prepare_result_string(pPanelReq->result,pReq);
	else
	{
		prepare_result_string(pPanelReq->result,pReq,(char*)SYSMGR_RPC_LOADINFO_ARG_CURRENT,pPanelCmdObj->cur_load,
				                             (char*)SYSMGR_RPC_LOADINFO_ARG_AVERAGE,pPanelCmdObj->avg_load,
				                             (char*)SYSMGR_RPC_LOADINFO_ARG_UPTIME ,pPanelCmdObj->uptime);
	}
	OBJ_MEM_DELETE(pPanelCmdObj);
	return 0;
}
int SysRpc::process_get_loadinfo(JsonDataCommObj* pReq)
{
	ADSysInfo SysInfo;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SYSMGR_LOAD_INFO_PACKET* pPacket;
	pPacket=(SYSMGR_LOAD_INFO_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->action!=RPC_SRV_ACT_READ)
	{
		pPanelReq->result=RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
		return 0;
	}
	if(SysInfo.read_load_info(pPacket->cur_load,pPacket->avg_load,pPacket->uptime)==0)
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
int SysRpc::json_to_bin_get_meminfo(JsonDataCommObj* pReq)
{
	SYSMGR_MEM_INFO_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_MEM_INFO_PACKET,RPC_SRV_ACT_READ,EJSON_SYSMGR_RPC_GET_MEMINFO);
	return 0;
}
int SysRpc::bin_to_json_get_meminfo(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	SYSMGR_MEM_INFO_PACKET* pPanelCmdObj;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	pPanelCmdObj=(SYSMGR_MEM_INFO_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->result!=RPC_SRV_RESULT_SUCCESS)
		prepare_result_string(pPanelReq->result,pReq);
	else
	{
		prepare_result_string(pPanelReq->result,pReq,(char*)SYSMGR_RPC_MEMINFO_ARG_MEM,pPanelCmdObj->mem,
				                             (char*)SYSMGR_RPC_MEMINFO_ARG_MEMFREE,pPanelCmdObj->memfree,
				                             (char*)SYSMGR_RPC_MEMINFO_ARG_MEMUSED,pPanelCmdObj->memused);
	}
	OBJ_MEM_DELETE(pPanelCmdObj);
	return 0;
}
int SysRpc::process_get_meminfo(JsonDataCommObj* pReq)
{
	ADSysInfo SysInfo;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SYSMGR_MEM_INFO_PACKET* pPacket;
	pPacket=(SYSMGR_MEM_INFO_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->action!=RPC_SRV_ACT_READ)
	{
		pPanelReq->result=RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
		return 0;
	}
	if(SysInfo.read_mem_info(pPacket->mem,pPacket->memfree,pPacket->memused)==0)
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
int SysRpc::json_to_bin_get_cpuinfo(JsonDataCommObj* pReq)
{
	SYSMGR_CPU_INFO_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_CPU_INFO_PACKET,RPC_SRV_ACT_READ,EJSON_SYSMGR_RPC_GET_CPUINFO);
	return 0;
}
int SysRpc::bin_to_json_get_cpuinfo(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	SYSMGR_CPU_INFO_PACKET* pPanelCmdObj;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	pPanelCmdObj=(SYSMGR_CPU_INFO_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->result!=RPC_SRV_RESULT_SUCCESS)
		prepare_result_string(pPanelReq->result,pReq);
	else
	{
		prepare_result_string(pPanelReq->result,pReq,(char*)SYSMGR_RPC_CPUINFO_ARG_MODEL,pPanelCmdObj->cpu_model,
				                             (char*)SYSMGR_RPC_CPUINFO_ARG_CORES,pPanelCmdObj->cpu_cores,
				                             (char*)SYSMGR_RPC_CPUINFO_ARG_FREQ ,pPanelCmdObj->cpu_freq);
	}
	OBJ_MEM_DELETE(pPanelCmdObj);
	return 0;
}
int SysRpc::process_get_cpuinfo(JsonDataCommObj* pReq)
{
	ADSysInfo SysInfo;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SYSMGR_CPU_INFO_PACKET* pPacket;
	pPacket=(SYSMGR_CPU_INFO_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->action!=RPC_SRV_ACT_READ)
	{
		pPanelReq->result=RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
		return 0;
	}

	//pPacket->cpu_model[0]='\0';
	//pPacket->cpu_cores[0]='\0';
	//pPacket->cpu_freq[0]='\0';

	if(SysInfo.read_cpu_info(pPacket->cpu_model,pPacket->cpu_cores,pPacket->cpu_freq)==0)
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */

