#include "DispCtrlRpc.h"
#include "ADSysInfo.hpp"
#include "DisplayDevice.hpp"
/* ------------------------------------------------------------------------- */
DispCtrlRpc:: DispCtrlRpc(std::string rpcName,int myIndex,bool emu,bool log,SYSMGR_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
	pDataCache=pData;	
}
/* ------------------------------------------------------------------------- */
DispCtrlRpc::~ DispCtrlRpc()
{
}
/* ------------------------------------------------------------------------- */
int DispCtrlRpc::MapJsonToBinary(JsonDataCommObj* pReq,int index)
{
	EJSON_SYSMGR_RPC_TYPES command =(EJSON_SYSMGR_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_SYSMGR_RPC_DISP_CLEAR:return json_to_bin_disp_clear(pReq);
		case EJSON_SYSMGR_RPC_DISP_PRINT:return json_to_bin_disp_print(pReq);
	}
	return -1;
}
/* ------------------------------------------------------------------------- */
int DispCtrlRpc::MapBinaryToJson(JsonDataCommObj* pReq,int index)
{
	EJSON_SYSMGR_RPC_TYPES command =(EJSON_SYSMGR_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_SYSMGR_RPC_DISP_CLEAR:return bin_to_json_disp_clear(pReq);
		case EJSON_SYSMGR_RPC_DISP_PRINT:return bin_to_json_disp_print(pReq);
	}
	return -1;
}
/* ------------------------------------------------------------------------- */
int DispCtrlRpc::ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj)
{
	EJSON_SYSMGR_RPC_TYPES command =(EJSON_SYSMGR_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_SYSMGR_RPC_DISP_CLEAR:return process_disp_clear(pReq,pDataCache);
		case EJSON_SYSMGR_RPC_DISP_PRINT:return process_disp_print(pReq,pDataCache);
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT DispCtrlRpc::ProcessWorkAsync(int index,unsigned char* pWorkData)
{
	RPC_SRV_RESULT ret_val=RPC_SRV_RESULT_FAIL;

	return ret_val;
}
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_DISP_CLEAR
int DispCtrlRpc::json_to_bin_disp_clear(JsonDataCommObj* pReq)
{
	SYSMGR_PRINT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_PRINT_PACKET,RPC_SRV_ACT_WRITE,EJSON_SYSMGR_RPC_DISP_CLEAR);
	return 0;
}
int DispCtrlRpc::bin_to_json_disp_clear(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,SYSMGR_PRINT_PACKET);
	return 0;
}
int DispCtrlRpc::process_disp_clear(JsonDataCommObj* pReq,SYSMGR_CMN_DATA_CACHE *pData)
{
	DisplayDevice *pDisp=(DisplayDevice *)pData->pDisplay;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SYSMGR_PRINT_PACKET* pPacket;
	pPacket=(SYSMGR_PRINT_PACKET*)pPanelReq->dataRef;
	if(pData->pDisplay!=NULL)
		pPanelReq->result=pDisp->clear_display();
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_DISP_PRINT
int DispCtrlRpc::json_to_bin_disp_print(JsonDataCommObj* pReq)
{
	SYSMGR_PRINT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,SYSMGR_PRINT_PACKET,RPC_SRV_ACT_WRITE,EJSON_SYSMGR_RPC_DISP_PRINT);
	//after assigning pointer to pPanelReq->dataRef, modify cmd-req-obj with correct client arguments
JSON_STRING_TO_ENUM(SYSMGR_RPC_DISP_PRINT_LINE_ARG,SYSMGR_RPC_DISP_LINE_ARG_TABL,EJSON_SYSMGR_LINE,EJSON_SYSMGR_LINE_UNKNOWN,pPanelCmdObj->line);
	//extract targetFilePath
	JSON_STRING_TO_STRING(SYSMGR_RPC_DISP_PRINT_MESG_ARG,pPanelCmdObj->msg);
	return 0;
}
int DispCtrlRpc::bin_to_json_disp_print(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,SYSMGR_PRINT_PACKET);
	return 0;
}
int DispCtrlRpc::process_disp_print(JsonDataCommObj* pReq,SYSMGR_CMN_DATA_CACHE *pData)
{
	DisplayDevice *pDisp=(DisplayDevice *)pData->pDisplay;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	SYSMGR_PRINT_PACKET* pPacket;
	pPacket=(SYSMGR_PRINT_PACKET*)pPanelReq->dataRef;
	DISPLAY_LINE myline=(DISPLAY_LINE)pPacket->line;//important: keep enum DISPLAY_LINE and EJSON_DISPSRV_LINE same
	if(pData->pDisplay!=NULL)
		pPanelReq->result=pDisp->print_line(pPacket->msg,myline);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */

