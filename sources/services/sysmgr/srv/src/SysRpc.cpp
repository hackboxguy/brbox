#include "SysRpc.h"
#include "ADSysInfo.hpp"
//#include "SysmgrJsonDef.h"
/* ------------------------------------------------------------------------- */
SysRpc:: SysRpc(std::string rpcName,int myIndex,bool emu,bool log,SYSMGR_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
	pDataCache=pData;
	crnt_fmwver_updated=false;
	bkup_fmwver_updated=false;
	krnl_fmwver_updated=false;
	boot_system_updated=false;
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
		case EJSON_SYSMGR_RPC_GET_LOADINFO    :return json_to_bin_get_loadinfo(pReq);
		case EJSON_SYSMGR_RPC_GET_MEMINFO     :return json_to_bin_get_meminfo(pReq);
		case EJSON_SYSMGR_RPC_GET_CPUINFO     :return json_to_bin_get_cpuinfo(pReq);
		case EJSON_SYSMGR_RPC_GET_DEV_OP      :return json_to_bin_get_devop(pReq);
		case EJSON_SYSMGR_RPC_SET_DEV_OP      :return json_to_bin_set_devop(pReq);
		case EJSON_SYSMGR_RPC_GET_FMWVER      :return json_to_bin_get_fmwver(pReq);
		case EJSON_SYSMGR_RPC_GET_BOOT_SYSTEM :return json_to_bin_get_bootsys(pReq);
		case EJSON_SYSMGR_RPC_SET_FMWUPDATE   :return json_to_bin_set_fmwupdate(pReq);
		case EJSON_SYSMGR_RPC_SET_DOWNLOADFTP :return json_to_bin_download_file(pReq,EJSON_SYSMGR_RPC_SET_DOWNLOADFTP);
		case EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP:return json_to_bin_download_file(pReq,EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP);

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
		case EJSON_SYSMGR_RPC_GET_LOADINFO    :return bin_to_json_get_loadinfo(pReq);
		case EJSON_SYSMGR_RPC_GET_MEMINFO     :return bin_to_json_get_meminfo(pReq);
		case EJSON_SYSMGR_RPC_GET_CPUINFO     :return bin_to_json_get_cpuinfo(pReq);
		case EJSON_SYSMGR_RPC_GET_DEV_OP      :return bin_to_json_get_devop(pReq);
		case EJSON_SYSMGR_RPC_SET_DEV_OP      :return bin_to_json_set_devop(pReq);
		case EJSON_SYSMGR_RPC_GET_FMWVER      :return bin_to_json_get_fmwver(pReq);
		case EJSON_SYSMGR_RPC_GET_BOOT_SYSTEM :return bin_to_json_get_bootsys(pReq);
		case EJSON_SYSMGR_RPC_SET_FMWUPDATE   :return bin_to_json_set_fmwupdate(pReq);
		case EJSON_SYSMGR_RPC_SET_DOWNLOADFTP :return bin_to_json_download_file(pReq,EJSON_SYSMGR_RPC_SET_DOWNLOADFTP);
		case EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP:return bin_to_json_download_file(pReq,EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP);
		default: break;
	}
	return -1;
}
/* ------------------------------------------------------------------------- */
int SysRpc::ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj)
{
	//printf("SysRpc::ProcessWork called\n");
	EJSON_SYSMGR_RPC_TYPES command =(EJSON_SYSMGR_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_SYSMGR_RPC_GET_LOADINFO    :return process_get_loadinfo(pReq);
		case EJSON_SYSMGR_RPC_GET_MEMINFO     :return process_get_meminfo(pReq);
		case EJSON_SYSMGR_RPC_GET_CPUINFO     :return process_get_cpuinfo(pReq);
		case EJSON_SYSMGR_RPC_GET_DEV_OP      :return process_get_devop(pReq);
		case EJSON_SYSMGR_RPC_SET_DEV_OP      :return process_set_devop(pReq,pObj);
		case EJSON_SYSMGR_RPC_GET_FMWVER      :return process_get_fmwver(pReq);
		case EJSON_SYSMGR_RPC_GET_BOOT_SYSTEM :return process_get_bootsys(pReq);
		case EJSON_SYSMGR_RPC_SET_FMWUPDATE   :return process_set_fmwupdate(pReq,pObj);
		case EJSON_SYSMGR_RPC_SET_DOWNLOADFTP :return process_download_file(pReq,EJSON_SYSMGR_RPC_SET_DOWNLOADFTP,pObj);
		case EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP:return process_download_file(pReq,EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP,pObj);

		default:break;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT SysRpc::ProcessWorkAsync(int cmd,unsigned char* pWorkData)
{
	RPC_SRV_RESULT ret_val=RPC_SRV_RESULT_FAIL;
	switch(cmd)
	{
		case EJSON_SYSMGR_RPC_SET_DEV_OP:
			{
				SYSMGR_DEV_OP_PACKET *pPacket;
				pPacket=(SYSMGR_DEV_OP_PACKET*)pWorkData;
				ret_val=process_async_set_devop(pPacket);
				OBJ_MEM_DELETE(pWorkData);
			}
			break;
		case EJSON_SYSMGR_RPC_SET_FMWUPDATE:
			{
				SYSMGR_FMWUPDATE_PACKET *pPacket;
				pPacket=(SYSMGR_FMWUPDATE_PACKET*)pWorkData;
				ret_val=process_async_set_fmwupdate(pPacket);
				OBJ_MEM_DELETE(pWorkData);
			}
			break;
		case EJSON_SYSMGR_RPC_SET_DOWNLOADFTP:
		case EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP:
			{
				SYSMGR_DOWNLOAD_FILE_PACKET *pPacket;
				pPacket=(SYSMGR_DOWNLOAD_FILE_PACKET*)pWorkData;
				ret_val=process_async_download_file(pPacket,(EJSON_SYSMGR_RPC_TYPES)cmd);
				OBJ_MEM_DELETE(pWorkData);
			}
			break;
		default:
			break;
	}
	return ret_val;
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

	if(SysInfo.read_cpu_info(pPacket->cpu_model,pPacket->cpu_cores,pPacket->cpu_freq)==0)
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
int SysRpc::json_to_bin_get_devop(JsonDataCommObj* pReq)
{
	SYSMGR_DEV_OP_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_DEV_OP_PACKET,RPC_SRV_ACT_READ,EJSON_SYSMGR_RPC_GET_DEV_OP);
	return 0;
}
int SysRpc::bin_to_json_get_devop(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,SYSMGR_DEV_OP_PACKET,SYSMGR_RPC_DEV_OP_ARG,operation,SYSMGR_RPC_DEV_OP_ARG_TABL,EJSON_SYSMGR_DEV_OP_UNKNOWN);
	return 0;
}
int SysRpc::process_get_devop(JsonDataCommObj* pReq)
{
	ADSysInfo SysInfo;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SYSMGR_DEV_OP_PACKET* pPacket;
	pPacket=(SYSMGR_DEV_OP_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->action!=RPC_SRV_ACT_READ)
	{
		pPanelReq->result=RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
		return 0;
	}
	//pDataCache->DevOp.operation=EJSON_SYSMGR_DEV_OP_ON;//TODO: init this state correctly on startup

	pPacket->operation=pDataCache->DevOp.operation;
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
int SysRpc::json_to_bin_set_devop(JsonDataCommObj* pReq)
{
	SYSMGR_DEV_OP_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_DEV_OP_PACKET,RPC_SRV_ACT_WRITE,EJSON_SYSMGR_RPC_SET_DEV_OP);
	JSON_STRING_TO_ENUM(SYSMGR_RPC_DEV_OP_ARG,SYSMGR_RPC_DEV_OP_ARG_TABL,EJSON_SYSMGR_DEV_OP,EJSON_SYSMGR_DEV_OP_UNKNOWN,pPanelCmdObj->operation);
	return 0;
}
int SysRpc::bin_to_json_set_devop(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_IN_PROG(RPC_SRV_REQ,SYSMGR_DEV_OP_PACKET,RPCMGR_RPC_TASK_STS_ARGID);
	return 0;
}
int SysRpc::process_set_devop(JsonDataCommObj* pReq,ADJsonRpcMgrProducer* pObj)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SYSMGR_DEV_OP_PACKET* pPacket;
	pPacket=(SYSMGR_DEV_OP_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->action!=RPC_SRV_ACT_WRITE)
	{
		pPanelReq->result=RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
		return 0;
	}

	SYSMGR_DEV_OP_PACKET* pWorkData=NULL;
	OBJECT_MEM_NEW(pWorkData,SYSMGR_DEV_OP_PACKET);//delete this object in run_work() callback function
	if(pWorkData == NULL)
	{
		pPanelReq->result=RPC_SRV_RESULT_MEM_ERROR;
		return -1;
	}
	pWorkData->operation=pPacket->operation;
	pPanelReq->result=pObj->PushAsyncTask(EJSON_SYSMGR_RPC_SET_DEV_OP,(unsigned char*)pWorkData,&pPacket->taskID,WORK_CMD_AFTER_DONE_PRESERVE);
	if(pPanelReq->result!=RPC_SRV_RESULT_IN_PROG)
		OBJ_MEM_DELETE(pWorkData);
	return 0;
}
RPC_SRV_RESULT SysRpc::process_async_set_devop(SYSMGR_DEV_OP_PACKET* pPacket)
{
	char cmdline[512];
	ADSysInfo SysInfo;
	RPC_SRV_RESULT ret_val=RPC_SRV_RESULT_ARG_ERROR;
	switch(pPacket->operation)
	{
		case EJSON_SYSMGR_DEV_OP_IDLE   :
		case EJSON_SYSMGR_DEV_OP_ON     :
		case EJSON_SYSMGR_DEV_OP_STANDBY:
			pDataCache->DevOp.operation=pPacket->operation;
			ret_val=RPC_SRV_RESULT_SUCCESS;
			break;
		//case EJSON_SYSMGR_DEV_OP_BOOT :
		case EJSON_SYSMGR_DEV_OP_REBOOT :
			sprintf(cmdline,"%s","sleep 3;reboot");//CMDLINE_TRIGGER_REBOOT_CMD);
			//#define CMDLINE_TRIGGER_REBOOT_CMD       "sleep 3;reboot"
			ret_val=SysInfo.run_shell_script(cmdline,get_emulation_flag());
			break;
		default:
			break;
	}
	return ret_val;
}
/* ------------------------------------------------------------------------- */
int SysRpc::json_to_bin_get_fmwver(JsonDataCommObj* pReq)
{
	SYSMGR_FMW_MODULE_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_FMW_MODULE_PACKET,RPC_SRV_ACT_READ,EJSON_SYSMGR_RPC_GET_FMWVER);
	//extract "module" parameter from json string
	JSON_STRING_TO_ENUM(SYSMGR_RPC_FMW_VER_ARG_MODULE,SYSMGR_RPC_FMW_VER_ARG_TABL,SYSMGR_FMW_MODULE_TYPE,SYSMGR_FMW_MODULE_UNKNOWN,pPanelCmdObj->module);
	return 0;
}
int SysRpc::bin_to_json_get_fmwver(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,SYSMGR_FMW_MODULE_PACKET,SYSMGR_RPC_FMW_VER_ARG,cmn_fname_ver_str);
	return 0;
}
int SysRpc::process_get_fmwver(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SYSMGR_FMW_MODULE_PACKET* pPacket;
	pPacket=(SYSMGR_FMW_MODULE_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->action!=RPC_SRV_ACT_READ)
	{
		pPanelReq->result=RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
		return 0;
	}
	if(get_emulation_flag()==true)
	{
		strcpy(pPacket->cmn_fname_ver_str,"00.01.12345");
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
		return 0;
	}
	strcpy(pPacket->cmn_fname_ver_str,"unknown");
	char command[255];
	switch(pPacket->module)
	{
		case SYSMGR_FMW_MODULE_BRBOX_CURRENT:
			if(crnt_fmwver_updated==true)
			{
				strcpy(pPacket->cmn_fname_ver_str,pDataCache->crnt_fmwver);
				pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
				return 0;
			}
			sprintf(command,"%s -v > %s",SYSMGR_BRDSK_TOOL,SYSMGR_TEMP_FMW_READ_FILE);
			break;
		case SYSMGR_FMW_MODULE_BRBOX_BACKUP :
			if(bkup_fmwver_updated==true)
			{
				strcpy(pPacket->cmn_fname_ver_str,pDataCache->bkup_fmwver);
				pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
				return 0;
			}
			sprintf(command,"%s -b > %s",SYSMGR_BRDSK_TOOL,SYSMGR_TEMP_FMW_READ_FILE);
			break;
		//case SYSMGR_FMW_MODULE_BRBOX_KERNEL :
			//if(krnl_fmwver_updated==true)
			//{
			//	strcpy(pPacket->cmn_fname_ver_str,pDataCache->krnl_fmwver);
			//	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
			//	return 0;
			//}
			//break;
		default:
			pPanelReq->result=RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;
			return 0;
			break;
	}
	if(system(command)!=0)
	{
		pPanelReq->result=RPC_SRV_RESULT_FILE_OPEN_ERR;
		return 0;
	}
	char temp_str[255];
	FILE *shell;
	shell= fopen(SYSMGR_TEMP_FMW_READ_FILE,"r");
	if(shell == NULL )
	{
		pPanelReq->result=RPC_SRV_RESULT_FILE_OPEN_ERR;
		return 0;
	}
	size_t read_bytes = fread(temp_str,1,100,shell);
	fclose(shell);
	if(read_bytes>0)
	{
		temp_str[read_bytes]='\0';
		if(temp_str[strlen(temp_str)-1]=='\n')//remove the carriage return line
			temp_str[strlen(temp_str)-1]='\0';
		strcpy(pPacket->cmn_fname_ver_str,temp_str);
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
		
		switch(pPacket->module)
		{
		case SYSMGR_FMW_MODULE_BRBOX_CURRENT:
			crnt_fmwver_updated=true;
			strcpy(pDataCache->crnt_fmwver,temp_str);
			break;
		case SYSMGR_FMW_MODULE_BRBOX_BACKUP :
			bkup_fmwver_updated=true;
			strcpy(pDataCache->bkup_fmwver,temp_str);
			break;
		//case SYSMGR_FMW_MODULE_BRBOX_KERNEL :
			//krnl_fmwver_updated=true;
			//strcpy(pDataCache->krnl_fmwver,temp_str);
			//break;
		default:
			pPanelReq->result=RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;
			break;
		}
	}
	else
		pPanelReq->result=RPC_SRV_RESULT_FILE_READ_ERR;
	return 0;
}
/* ------------------------------------------------------------------------- */
int SysRpc::json_to_bin_get_bootsys(JsonDataCommObj* pReq)
{
	SYSMGR_BOOT_SYSTEM_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_BOOT_SYSTEM_PACKET,RPC_SRV_ACT_READ,EJSON_SYSMGR_RPC_GET_BOOT_SYSTEM);
	return 0;
}
int SysRpc::bin_to_json_get_bootsys(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,SYSMGR_BOOT_SYSTEM_PACKET,SYSMGR_RPC_BOOT_SYSTEM_ARG,system,SYSMGR_RPC_BOOT_SYSTEM_ARG_TABL,SYSMGR_BOOT_SYSTEM_UNKNOWN);
	return 0;
}
int SysRpc::process_get_bootsys(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SYSMGR_BOOT_SYSTEM_PACKET* pPacket;
	pPacket=(SYSMGR_BOOT_SYSTEM_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->action!=RPC_SRV_ACT_READ)
	{
		pPanelReq->result=RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
		return 0;
	}
	if(boot_system_updated==true) //is this data already cached?
	{
		pPacket->system   = pDataCache->bootsys;
		pPanelReq->result = RPC_SRV_RESULT_SUCCESS;
		return 0;
	}

	//if not in cache, read it for once
	char command[255];
	sprintf(command,"%s -c > %s",SYSMGR_BRDSK_TOOL,SYSMGR_TEMP_FMW_READ_FILE);
	if(system(command)!=0)
	{
		pPanelReq->result=RPC_SRV_RESULT_FILE_OPEN_ERR;
		return 0;
	}

	char temp_str[255];
	FILE *shell;
	shell= fopen(SYSMGR_TEMP_FMW_READ_FILE,"r");
	if(shell == NULL )
	{
		pPanelReq->result=RPC_SRV_RESULT_FILE_OPEN_ERR;
		return 0;
	}
	size_t read_bytes = fread(temp_str,1,100,shell);
	fclose(shell);
	if(read_bytes>0)
	{
		temp_str[read_bytes]='\0';
		if(temp_str[strlen(temp_str)-1]=='\n')//remove the carriage return line
			temp_str[strlen(temp_str)-1]='\0';
		const char *table[]   = SYSMGR_RPC_BOOT_SYSTEM_ARG_TABL;
		SYSMGR_BOOT_SYSTEM_TYPE bootsystem=(SYSMGR_BOOT_SYSTEM_TYPE)string_to_enum(table,temp_str,SYSMGR_BOOT_SYSTEM_UNKNOWN);
		if(bootsystem>=SYSMGR_BOOT_SYSTEM_UNKNOWN)
		{
			pPacket->system=SYSMGR_BOOT_SYSTEM_UNKNOWN;
			pPanelReq->result=RPC_SRV_RESULT_FAIL;
			return 0;
		}
		pPacket->system=bootsystem;
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
		//cache this variable, no need to read it again from filesystem
		pDataCache->bootsys=bootsystem;
		boot_system_updated=true;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
int SysRpc::json_to_bin_set_fmwupdate(JsonDataCommObj* pReq)
{
	SYSMGR_FMWUPDATE_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_FMWUPDATE_PACKET,RPC_SRV_ACT_WRITE,EJSON_SYSMGR_RPC_SET_FMWUPDATE);
	//extract "module" parameter from json string
	JSON_STRING_TO_ENUM(SYSMGR_RPC_FMWUPDATE_ARG,SYSMGR_RPC_FMWUPDATE_ARG_TABL,SYSMGR_FMW_MODULE_TYPE,SYSMGR_FMW_MODULE_UNKNOWN,pPanelCmdObj->module);
	//extract "filepath" parameter from json string
	JSON_STRING_TO_STRING(SYSMGR_RPC_FMWUPDATE_ARG_FILEPATH,pPanelCmdObj->cmn_fname_ver_str);
	return 0;
}
int SysRpc::bin_to_json_set_fmwupdate(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_IN_PROG(RPC_SRV_REQ,SYSMGR_FMWUPDATE_PACKET,RPCMGR_RPC_TASK_STS_ARGID);
	return 0;
}
int SysRpc::process_set_fmwupdate(JsonDataCommObj* pReq,ADJsonRpcMgrProducer* pObj)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SYSMGR_FMWUPDATE_PACKET* pPacket;
	pPacket=(SYSMGR_FMWUPDATE_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->action!=RPC_SRV_ACT_WRITE)
	{
		pPanelReq->result=RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
		return 0;
	}
	//only 'project' module is supported currently
	if(pPacket->module!=SYSMGR_FMW_MODULE_BRBOX_PROJECT)
	{
		pPanelReq->result=RPC_SRV_RESULT_ARG_ERROR;
		return 0;
	}

	//create a copy packet and initialize with supplied values
	SYSMGR_FMWUPDATE_PACKET* pWorkData=NULL;
	OBJECT_MEM_NEW(pWorkData,SYSMGR_FMWUPDATE_PACKET);//delete this object in run_work() callback function
	if(pWorkData == NULL)
	{
		pPanelReq->result=RPC_SRV_RESULT_MEM_ERROR;
		return -1;
	}
	pWorkData->module=pPacket->module;
	strcpy(pWorkData->cmn_fname_ver_str,pPacket->cmn_fname_ver_str);

	pPanelReq->result=pObj->PushAsyncTask(EJSON_SYSMGR_RPC_SET_FMWUPDATE,(unsigned char*)pWorkData,&pPacket->taskID,WORK_CMD_AFTER_DONE_PRESERVE);
	if(pPanelReq->result!=RPC_SRV_RESULT_IN_PROG)
		OBJ_MEM_DELETE(pWorkData);
	return 0;
}
RPC_SRV_RESULT SysRpc::process_async_set_fmwupdate(SYSMGR_FMWUPDATE_PACKET* pPacket)
{
	char cmdline[512];
	ADSysInfo SysInfo;
	RPC_SRV_RESULT ret_val=RPC_SRV_RESULT_ARG_ERROR;
	switch(pPacket->module)
	{
		case SYSMGR_FMW_MODULE_BRBOX_PROJECT :
			sprintf(cmdline,"%s -u %s",SYSMGR_UPDATE_TOOL,pPacket->cmn_fname_ver_str);
			ret_val=SysInfo.run_shell_script(cmdline,get_emulation_flag());//backup-fmw will be updated
			bkup_fmwver_updated=false;//re-read bkup-fmw-version when fmw-version-rpc is called
			break;
		default:
			break;
	}
	return ret_val;
}
/* ------------------------------------------------------------------------- */
int SysRpc::json_to_bin_download_file(JsonDataCommObj* pReq,EJSON_SYSMGR_RPC_TYPES command)
{
	SYSMGR_DOWNLOAD_FILE_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_DOWNLOAD_FILE_PACKET,RPC_SRV_ACT_WRITE,command);
	//extract sourceURL(for ftp) or serverip (for tftp)
	JSON_STRING_TO_STRING(SYSMGR_RPC_DOWNLOADFILE_ARG_SRCADDR,pPanelCmdObj->srcurl);

	//for tftp source file name is also needed
	if(command==EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP)
		JSON_STRING_TO_STRING(SYSMGR_RPC_DOWNLOADFILE_ARG_SRCFILE,pPanelCmdObj->sourcefilepath);
	else
		pPanelCmdObj->sourcefilepath[0]='\0';//keep it empty
	//extract targetFilePath
	JSON_STRING_TO_STRING(SYSMGR_RPC_DOWNLOADFILE_ARG_TARFILE,pPanelCmdObj->targetfilepath);
	return 0;
}
int SysRpc::bin_to_json_download_file(JsonDataCommObj* pReq,EJSON_SYSMGR_RPC_TYPES command)
{
	PREPARE_JSON_RESP_IN_PROG(RPC_SRV_REQ,SYSMGR_DOWNLOAD_FILE_PACKET,RPCMGR_RPC_TASK_STS_ARGID);
	return 0;
}
int SysRpc::process_download_file(JsonDataCommObj* pReq,EJSON_SYSMGR_RPC_TYPES command,ADJsonRpcMgrProducer* pObj)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SYSMGR_DOWNLOAD_FILE_PACKET* pPacket;
	pPacket=(SYSMGR_DOWNLOAD_FILE_PACKET*)pPanelReq->dataRef;

	//create a copy packet and initialize with supplied values
	SYSMGR_DOWNLOAD_FILE_PACKET* pWorkData=NULL;
	OBJECT_MEM_NEW(pWorkData,SYSMGR_DOWNLOAD_FILE_PACKET);//delete this object in run_work() callback function
	if(pWorkData == NULL)
	{
		pPanelReq->result=RPC_SRV_RESULT_MEM_ERROR;
		return -1;
	}
	strcpy(pWorkData->srcurl,pPacket->srcurl);
	strcpy(pWorkData->sourcefilepath,pPacket->sourcefilepath);
	strcpy(pWorkData->targetfilepath,pPacket->targetfilepath);

	pPanelReq->result=pObj->PushAsyncTask(command,(unsigned char*)pWorkData,&pPacket->taskID,WORK_CMD_AFTER_DONE_PRESERVE);
	if(pPanelReq->result!=RPC_SRV_RESULT_IN_PROG)
		OBJ_MEM_DELETE(pWorkData);
	return 0;
}
RPC_SRV_RESULT SysRpc::process_async_download_file(SYSMGR_DOWNLOAD_FILE_PACKET* pPacket,EJSON_SYSMGR_RPC_TYPES command)
{
	char cmdline[512];
	ADSysInfo SysInfo;
	RPC_SRV_RESULT ret_val=RPC_SRV_RESULT_ARG_ERROR;
	switch(command)
	{
		case EJSON_SYSMGR_RPC_SET_DOWNLOADFTP :
			sprintf(cmdline,"wget %s -O %s",pPacket->srcurl,pPacket->targetfilepath);//targetfilepath must be fullpath+filename
			ret_val=SysInfo.run_shell_script(cmdline,get_emulation_flag());//backup-fmw will be updated
			break;
		case EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP :
			sprintf(cmdline,"tftp -g -r %s -l %s %s",pPacket->sourcefilepath,pPacket->targetfilepath,pPacket->srcurl);
			ret_val=SysInfo.run_shell_script(cmdline,get_emulation_flag());//backup-fmw will be updated
			break;
		default:
			break;
	}
	return ret_val;
}
/* ------------------------------------------------------------------------- */


