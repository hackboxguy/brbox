#include "DispCtrlRpc.h"
//#include "DispAccess.h"
#include "ADEvntNotifier.hpp"//for eventing
/* ------------------------------------------------------------------------- */
DispCtrlRpc:: DispCtrlRpc(std::string rpcName,int myIndex,bool emu, bool log,DISPSRV_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
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
	EJSON_DISPSRV_RPC_TYPES command =(EJSON_DISPSRV_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_DISPSRV_RPC_DISP_INIT :return json_to_bin_disp_init(pReq);
		case EJSON_DISPSRV_RPC_DISP_CLEAR:return json_to_bin_clear_display(pReq);
		case EJSON_DISPSRV_RPC_DISP_PRINT:return json_to_bin_print_display(pReq);
		default:break;
	}
	return -1;//0;
}
/* ------------------------------------------------------------------------- */
int DispCtrlRpc::MapBinaryToJson(JsonDataCommObj* pReq,int index)
{
	EJSON_DISPSRV_RPC_TYPES command =(EJSON_DISPSRV_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_DISPSRV_RPC_DISP_INIT :return bin_to_json_disp_init(pReq);
		case EJSON_DISPSRV_RPC_DISP_CLEAR:return bin_to_json_clear_display(pReq);
		case EJSON_DISPSRV_RPC_DISP_PRINT:return bin_to_json_print_display(pReq);
		default:break;
	}
	return -1;
}
/* ------------------------------------------------------------------------- */
int DispCtrlRpc::ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj)
{
	EJSON_DISPSRV_RPC_TYPES command =(EJSON_DISPSRV_RPC_TYPES)index;
	switch(command)
	{
		case EJSON_DISPSRV_RPC_DISP_INIT :return process_disp_init(pReq,pDataCache);
		case EJSON_DISPSRV_RPC_DISP_CLEAR:return process_clear_display(pReq,pDataCache);
		case EJSON_DISPSRV_RPC_DISP_PRINT:return process_print_display(pReq,pDataCache);
		default:break;
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
//EJSON_DISPSRV_RPC_DISP_INIT
int DispCtrlRpc::json_to_bin_disp_init(JsonDataCommObj* pReq)
{
	DISPSRV_PRINT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,DISPSRV_PRINT_PACKET,RPC_SRV_ACT_WRITE,EJSON_DISPSRV_RPC_DISP_INIT);
	return 0;
}
int DispCtrlRpc::bin_to_json_disp_init(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,DISPSRV_PRINT_PACKET);
	return 0;
}
int DispCtrlRpc::process_disp_init(JsonDataCommObj* pReq,DISPSRV_CMN_DATA_CACHE *pData)
{
	DisplayDevice *pDisp=pData->pDisplay;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	DISPSRV_PRINT_PACKET* pPacket;
	pPacket=(DISPSRV_PRINT_PACKET*)pPanelReq->dataRef;
	if(pData->pDisplay!=NULL)
		pPanelReq->result=pDisp->init_display();
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	//pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_DISPSRV_RPC_DISP_CLEAR
int DispCtrlRpc::json_to_bin_clear_display(JsonDataCommObj* pReq)
{
	DISPSRV_PRINT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,DISPSRV_PRINT_PACKET,RPC_SRV_ACT_WRITE,EJSON_DISPSRV_RPC_DISP_CLEAR);
	return 0;
}
int DispCtrlRpc::bin_to_json_clear_display(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,DISPSRV_PRINT_PACKET);
	return 0;
}
int DispCtrlRpc::process_clear_display(JsonDataCommObj* pReq,DISPSRV_CMN_DATA_CACHE *pData)
{
	DisplayDevice *pDisp=pData->pDisplay;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	DISPSRV_PRINT_PACKET* pPacket;
	pPacket=(DISPSRV_PRINT_PACKET*)pPanelReq->dataRef;
	if(pData->pDisplay!=NULL)
		pPanelReq->result=pDisp->clear_display();
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	return 0;
}
/* ------------------------------------------------------------------------- */
//EJSON_DISPSRV_RPC_DISP_PRINT
int DispCtrlRpc::json_to_bin_print_display(JsonDataCommObj* pReq)
{
	DISPSRV_PRINT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,DISPSRV_PRINT_PACKET,RPC_SRV_ACT_WRITE,EJSON_DISPSRV_RPC_DISP_PRINT);
	//after assigning pointer to pPanelReq->dataRef, modify cmd-req-obj with correct client arguments
JSON_STRING_TO_ENUM(DISPSRV_RPC_DISP_PRINT_LINE_ARG,DISPSRV_RPC_DISP_LINE_ARG_TABL,EJSON_DISPSRV_LINE,EJSON_DISPSRV_LINE_UNKNOWN,pPanelCmdObj->line);
	//extract targetFilePath
	JSON_STRING_TO_STRING(DISPSRV_RPC_DISP_PRINT_MESG_ARG,pPanelCmdObj->msg);
	return 0;
}
int DispCtrlRpc::bin_to_json_print_display(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,DISPSRV_PRINT_PACKET);
	return 0;
}
int DispCtrlRpc::process_print_display(JsonDataCommObj* pReq,DISPSRV_CMN_DATA_CACHE *pData)
{
	DisplayDevice *pDisp=pData->pDisplay;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	DISPSRV_PRINT_PACKET* pPacket;
	pPacket=(DISPSRV_PRINT_PACKET*)pPanelReq->dataRef;
	DISPLAY_LINE myline=(DISPLAY_LINE)pPacket->line;//important: keep enum DISPLAY_LINE and EJSON_DISPSRV_LINE same
	if(pData->pDisplay!=NULL)
		pPanelReq->result=pDisp->print_line(pPacket->msg,myline);
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;
	//pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
/* ------------------------------------------------------------------------- */


