#include "HotwireRpc.h"
#define IMG_RENDER_FIFO_FILE "/tmp/img-renderer-process.fifo"
/* ------------------------------------------------------------------------- */
GpioCtrlRpc:: GpioCtrlRpc(std::string rpcName,int myIndex,bool emu, bool log,GPIOCTL_CMN_DATA_CACHE *pData):ADJsonRpcMgrConsumer(rpcName,myIndex,emu,log)
{
	pDataCache=pData;
	pDataCache->ActType=GPIOCTL_OMXACT_IDLE;
	char command[512];
	sprintf(command,"dd if=/dev/zero of=/dev/fb0;setterm -cursor off >/dev/tty1");
	system(command);
}
/* ------------------------------------------------------------------------- */
GpioCtrlRpc::~ GpioCtrlRpc()
{
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::MapJsonToBinary(JsonDataCommObj* pReq,int index)
{
	EJSON_GPIOCTL_RPC_TYPES command =(EJSON_GPIOCTL_RPC_TYPES)index;
	switch(command)
	{
		//case EJSON_GPIOCTL_RPC_IO_GET :return json_to_bin_gpio_get(pReq);
		//case EJSON_GPIOCTL_RPC_IO_SET :return json_to_bin_gpio_set(pReq);
		case EJSON_GPIOCTL_RPC_OMXACT_GET:return json_to_bin_omxact_get(pReq);
		case EJSON_GPIOCTL_RPC_OMXACT_SET:return json_to_bin_omxact_set(pReq);
		case EJSON_MPLAYSRV_RPC_SHOWFBIMG_GET:return json_to_bin_showfbimg_get(pReq);
		case EJSON_MPLAYSRV_RPC_SHOWFBIMG_SET:return json_to_bin_showfbimg_set(pReq);
		case EJSON_MPLAYSRV_RPC_QRCODEIMG_SET:return json_to_bin_qrcodeimg_set(pReq);
		default:break;
	}
	return -1;//0;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::MapBinaryToJson(JsonDataCommObj* pReq,int index)
{
	EJSON_GPIOCTL_RPC_TYPES command =(EJSON_GPIOCTL_RPC_TYPES)index;
	switch(command)
	{
		//case EJSON_GPIOCTL_RPC_IO_GET :return bin_to_json_gpio_get(pReq);
		//case EJSON_GPIOCTL_RPC_IO_SET :return bin_to_json_gpio_set(pReq);
		case EJSON_GPIOCTL_RPC_OMXACT_GET:return bin_to_json_omxact_get(pReq);
		case EJSON_GPIOCTL_RPC_OMXACT_SET:return bin_to_json_omxact_set(pReq);
		case EJSON_MPLAYSRV_RPC_SHOWFBIMG_GET:return bin_to_json_showfbimg_get(pReq);
		case EJSON_MPLAYSRV_RPC_SHOWFBIMG_SET:return bin_to_json_showfbimg_set(pReq);
		case EJSON_MPLAYSRV_RPC_QRCODEIMG_SET:return bin_to_json_qrcodeimg_set(pReq);
		default:break;
	}
	return -1;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::ProcessWork(JsonDataCommObj* pReq,int index,ADJsonRpcMgrProducer* pObj)
{
	EJSON_GPIOCTL_RPC_TYPES command =(EJSON_GPIOCTL_RPC_TYPES)index;
	switch(command)
	{
		//case EJSON_GPIOCTL_RPC_IO_GET :return process_gpio_get(pReq);
		//case EJSON_GPIOCTL_RPC_IO_SET :return process_gpio_set(pReq);
		case EJSON_GPIOCTL_RPC_OMXACT_GET:return process_omxact_get(pReq);
		case EJSON_GPIOCTL_RPC_OMXACT_SET:return process_omxact_set(pReq,pObj);
		case EJSON_MPLAYSRV_RPC_SHOWFBIMG_GET:return process_showfbimg_get(pReq);
		case EJSON_MPLAYSRV_RPC_SHOWFBIMG_SET:return process_showfbimg_set(pReq);
		case EJSON_MPLAYSRV_RPC_QRCODEIMG_SET:return process_qrcodeimg_set(pReq);
		default:break;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT GpioCtrlRpc::ProcessWorkAsync(int cmd,unsigned char* pWorkData)
{
	RPC_SRV_RESULT ret_val=RPC_SRV_RESULT_FAIL;
	//pDataCache->AsyncCmdInProgress=(EJSON_SYSMGR_RPC_TYPES)cmd;//To know which async command is in progress
	switch(cmd)
	{
		case EJSON_GPIOCTL_RPC_OMXACT_SET:
			{
				GPIOCTL_OMXACT_PACKET *pPacket;
				pPacket=(GPIOCTL_OMXACT_PACKET*)pWorkData;
				ret_val=process_omx_action(pPacket->ActType);
				OBJ_MEM_DELETE(pWorkData);
			}
			break;
		default:
			break;
	}
	//pDataCache->AsyncCmdInProgress=EJSON_SYSMGR_RPC_NONE;
	return ret_val;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::json_to_bin_gpio_get(JsonDataCommObj* pReq)
{
	//GPIOCTL_IO_ACCESS_PACKET* pPanelCmdObj=NULL;
	//PREPARE_JSON_REQUEST(RPC_SRV_REQ,GPIOCTL_IO_ACCESS_PACKET,RPC_SRV_ACT_READ,EJSON_GPIOCTL_RPC_IO_GET);
	//JSON_STRING_TO_INT(GPIOCTL_RPC_IO_ADDR_ARG,pPanelCmdObj->addr);
	return 0;
}
int GpioCtrlRpc::bin_to_json_gpio_get(JsonDataCommObj* pReq)
{
	//PREPARE_JSON_RESP_INT(RPC_SRV_REQ,GPIOCTL_IO_ACCESS_PACKET,GPIOCTL_RPC_IO_DATA_ARG,data);
	return 0;
}
int GpioCtrlRpc::process_gpio_get(JsonDataCommObj* pReq)
{
	/*RaspiIo Raspi;
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	GPIOCTL_IO_ACCESS_PACKET* pPacket;
	pPacket=(GPIOCTL_IO_ACCESS_PACKET*)pPanelReq->dataRef;

	//pPacket->data=pDataCache->tmpData;
	//pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	pPanelReq->result=Raspi.ReadGPIO (pPacket->addr,pPacket->data);
	//RPC_SRV_RESULT RaspiIo::WriteGPIO(unsigned int addr,unsigned int data)



	//set debug logging flag
	//ImgId.logflag=get_debug_log_flag();//get_debug_log_flag() is a function f parent class ADJsonRpcMgrConsumer
	//pPanelReq->result=ImgId.IdentifyPattern(pPacket->pattern_type,pDataCache->StrImgIdDebugFile);
	*/
	return 0;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::json_to_bin_gpio_set(JsonDataCommObj* pReq)
{
	//GPIOCTL_IO_ACCESS_PACKET* pPanelCmdObj=NULL;
	//PREPARE_JSON_REQUEST(RPC_SRV_REQ,GPIOCTL_IO_ACCESS_PACKET,RPC_SRV_ACT_WRITE,EJSON_GPIOCTL_RPC_IO_SET);
	//JSON_STRING_TO_INT(GPIOCTL_RPC_IO_ADDR_ARG,pPanelCmdObj->addr);
	//JSON_STRING_TO_INT(GPIOCTL_RPC_IO_DATA_ARG,pPanelCmdObj->data);
	return 0;
}
int GpioCtrlRpc::bin_to_json_gpio_set(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,GPIOCTL_IO_ACCESS_PACKET);
	return 0;
}
int GpioCtrlRpc::process_gpio_set(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	GPIOCTL_IO_ACCESS_PACKET* pPacket;
	pPacket=(GPIOCTL_IO_ACCESS_PACKET*)pPanelReq->dataRef;

	/*RaspiIo Raspi;
	pPanelReq->result=Raspi.WriteGPIO(pPacket->addr,pPacket->data);
	//pDataCache->tmpData=pPacket->data;
	//pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	*/
	return 0;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::json_to_bin_omxact_get(JsonDataCommObj* pReq)
{
	GPIOCTL_OMXACT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,GPIOCTL_OMXACT_PACKET,RPC_SRV_ACT_READ,EJSON_GPIOCTL_RPC_OMXACT_GET);
	return 0;
}
int GpioCtrlRpc::bin_to_json_omxact_get(JsonDataCommObj* pReq)
{
PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,GPIOCTL_OMXACT_PACKET,GPIOCTL_RPC_OMXACT_ARG,ActType,GPIOCTL_RPC_OMXACT_ARG_TABL,GPIOCTL_OMXACT_UNKNOWN);
	return 0;
}
int GpioCtrlRpc::process_omxact_get(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	GPIOCTL_OMXACT_PACKET* pPacket;
	pPacket=(GPIOCTL_OMXACT_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->action!=RPC_SRV_ACT_READ)
	{
		pPanelReq->result=RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
		return 0;
	}
	pPacket->ActType=pDataCache->ActType;
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::json_to_bin_omxact_set(JsonDataCommObj* pReq)
{
	GPIOCTL_OMXACT_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,GPIOCTL_OMXACT_PACKET,RPC_SRV_ACT_WRITE,EJSON_GPIOCTL_RPC_OMXACT_SET);
	JSON_STRING_TO_ENUM(GPIOCTL_RPC_OMXACT_ARG,GPIOCTL_RPC_OMXACT_ARG_TABL,GPIOCTL_OMXACT_TYPE,GPIOCTL_OMXACT_UNKNOWN,pPanelCmdObj->ActType);
	return 0;
}
int GpioCtrlRpc::bin_to_json_omxact_set(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,GPIOCTL_OMXACT_PACKET);
	return 0;
}
int GpioCtrlRpc::process_omxact_set(JsonDataCommObj* pReq,ADJsonRpcMgrProducer* pObj)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	GPIOCTL_OMXACT_PACKET* pPacket;
	pPacket=(GPIOCTL_OMXACT_PACKET*)pPanelReq->dataRef;

	//create a copy packet and initialize with supplied values
	GPIOCTL_OMXACT_PACKET* pWorkData=NULL;
	OBJECT_MEM_NEW(pWorkData,GPIOCTL_OMXACT_PACKET);//delete this object in run_work() callback function
	if(pWorkData == NULL)
	{
		pPanelReq->result=RPC_SRV_RESULT_MEM_ERROR;
		return -1;
	}
	pWorkData->ActType=pPacket->ActType;
	pPanelReq->result=pObj->PushAsyncTask(EJSON_GPIOCTL_RPC_OMXACT_SET,(unsigned char*)pWorkData,&pPacket->taskID,WORK_CMD_AFTER_DONE_DELETE);//PRESERVE);
	if(pPanelReq->result!=RPC_SRV_RESULT_IN_PROG)
	{
		OBJ_MEM_DELETE(pWorkData);
		return 0;
	}
	else
	{
		//pDataCache->ActType=pPacket->ActType;
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	}
	return 0;		
}
//player window
//mkfifo /tmp/cmd
//omxplayer -o both start.mp4 < /tmp/cmd

//other window
//echo . > /tmp/cmd
//echo -n p > /tmp/cmd (pause)
//echo -n q > /tmp/cmd (quit)
RPC_SRV_RESULT GpioCtrlRpc::process_omx_action(GPIOCTL_OMXACT_TYPE act)
{
	char command[1024];
	//if(pDataCache->ActType!=GPIOCTL_OMXACT_IDLE) //if not idle make it idle
	{
		if(is_omx_running()==true)//stop only if omxplayer is already running
		{
			sprintf(command,"echo -n q > /tmp/cmd");
			system(command);
		}
	}
	switch(act)
	{
		case GPIOCTL_OMXACT_START : 
				sprintf(command,"mkfifo /tmp/cmd");
				system(command);
				sprintf(command,"omxplayer -o both %s < /tmp/cmd &",GPIOCTL_OMXACT_START_FILE);
				system(command);
				sprintf(command,"echo . > /tmp/cmd");
				system(command);
				pDataCache->ActType=GPIOCTL_OMXACT_START;//pPacket->ActType;
				break;
		case GPIOCTL_OMXACT_INTR  :
				sprintf(command,"mkfifo /tmp/cmd");
				system(command);
				sprintf(command,"omxplayer -o both %s < /tmp/cmd &",GPIOCTL_OMXACT_INTR_FILE);
				system(command);
				sprintf(command,"echo . > /tmp/cmd");
				system(command);
				pDataCache->ActType=GPIOCTL_OMXACT_INTR;//pPacket->ActType;
				break;
		case GPIOCTL_OMXACT_STOP  :
				sprintf(command,"mkfifo /tmp/cmd");
				system(command);
				sprintf(command,"omxplayer -o both %s < /tmp/cmd &",GPIOCTL_OMXACT_STOP_FILE);
				system(command);
				sprintf(command,"echo . > /tmp/cmd");
				system(command);
				pDataCache->ActType=GPIOCTL_OMXACT_STOP;//pPacket->ActType;
				break;
		case GPIOCTL_OMXACT_WARN  : break;
		case GPIOCTL_OMXACT_IDLE  :
				//if(is_omx_running()==true)
				//{
				//	sprintf(command,"echo -n q > /tmp/cmd");
			//		system(command);
				//}
				pDataCache->ActType=GPIOCTL_OMXACT_IDLE;
				break;
		default:
			break;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
bool GpioCtrlRpc::is_omx_running()
{
	char command[1024];
	sprintf(command,"ps cax | grep [o]mxplayer >/dev/null");
	if (system(command)==0)
		return true;
	else
		return false;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::json_to_bin_showfbimg_get(JsonDataCommObj* pReq)
{
	MPLAYSRV_SHOWFBIMG_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,MPLAYSRV_SHOWFBIMG_PACKET,RPC_SRV_ACT_READ,EJSON_MPLAYSRV_RPC_SHOWFBIMG_GET);
	return 0;
}
int GpioCtrlRpc::bin_to_json_showfbimg_get(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,MPLAYSRV_SHOWFBIMG_PACKET,MPLAYSRV_RPC_SHOWFBIMG_ARG,fbimgpath);
	return 0;
}
int GpioCtrlRpc::process_showfbimg_get(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	MPLAYSRV_SHOWFBIMG_PACKET* pPacket;
	pPacket=(MPLAYSRV_SHOWFBIMG_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->action!=RPC_SRV_ACT_READ)
	{
		pPanelReq->result=RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
		return 0;
	}
	strcpy(pPacket->fbimgpath,pDataCache->fbimgpath.c_str());
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::json_to_bin_showfbimg_set(JsonDataCommObj* pReq)
{
	MPLAYSRV_SHOWFBIMG_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,MPLAYSRV_SHOWFBIMG_PACKET,RPC_SRV_ACT_WRITE,EJSON_MPLAYSRV_RPC_SHOWFBIMG_SET);
	JSON_STRING_TO_STRING(MPLAYSRV_RPC_SHOWFBIMG_ARG,pPanelCmdObj->fbimgpath);
	return 0;
}
int GpioCtrlRpc::bin_to_json_showfbimg_set(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,MPLAYSRV_SHOWFBIMG_PACKET);
	return 0;
}
int GpioCtrlRpc::process_showfbimg_set(JsonDataCommObj* pReq)//,ADJsonRpcMgrProducer* pObj)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	MPLAYSRV_SHOWFBIMG_PACKET* pPacket;
	pPacket=(MPLAYSRV_SHOWFBIMG_PACKET*)pPanelReq->dataRef;
	pDataCache->fbimgpath=pPacket->fbimgpath;
	pPanelReq->result=process_show_image(pDataCache->fbimgpath);//RPC_SRV_RESULT_SUCCESS;
	return 0;		
}
//show qr code:
//echo -ne "\033[9;2]" > /dev/tty1;dd if=/dev/zero of=/dev/fb0;fbv /tmp/dp.png
//remove qr code:
//dd if=/dev/zero of=/dev/fb0;setterm -cursor off > /dev/tty1
RPC_SRV_RESULT GpioCtrlRpc::process_show_image(std::string imgfile)
{
	//TODO: if video is already playing, then return fail, dont render any image
	char command[1024];
	if(imgfile=="none")
	{
		//if(is_screen_image_active()==true)//stop only if fbv image rendering is already running
		{
			//stop fbv command which is running in background
			//sprintf(command,"echo -n q > %s",IMG_RENDER_FIFO_FILE);
			//system(command);
			//clean framebuffer and turn off blinking cursor
			sprintf(command,"dd if=/dev/zero of=/dev/fb0;setterm -cursor off >/dev/tty1");
			system(command);
			//pDataCache->fbimgpath="none";

		}
		return RPC_SRV_RESULT_SUCCESS;
	}
	//check if image file exists //TODO:
	sprintf(command,"mkfifo %s",IMG_RENDER_FIFO_FILE);
	system(command);

	//wake-up sleeping display-output and clear screen
	sprintf(command,"echo -ne \"\\033[9;2]\">/dev/tty1;dd if=/dev/zero of=/dev/fb0");
	system(command);

	//using fbv command render the image file
	sprintf(command,"fbv %s < %s &",imgfile.c_str(),IMG_RENDER_FIFO_FILE);
	system(command);
	//with following command, frame is rendered on the screen(need to check why?)
	sprintf(command,"echo . > %s",IMG_RENDER_FIFO_FILE);
	system(command);


	//pDataCache->fbimgpath=imgfile;
	return RPC_SRV_RESULT_SUCCESS;
}
bool GpioCtrlRpc::is_screen_image_active()
{
	char command[1024];
	sprintf(command,"ps cax | grep [f]bv >/dev/null");
	if (system(command)==0)
		return true;
	else
		return false;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::json_to_bin_qrcodeimg_set(JsonDataCommObj* pReq)
{
	MPLAYSRV_QRCODEIMG_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,MPLAYSRV_QRCODEIMG_PACKET,RPC_SRV_ACT_WRITE,EJSON_MPLAYSRV_RPC_QRCODEIMG_SET);
	JSON_STRING_TO_STRING(MPLAYSRV_RPC_QRCODEIMG_ARG_FILEPATH,pPanelCmdObj->qrfilepath);
	JSON_STRING_TO_STRING(MPLAYSRV_RPC_QRCODEIMG_ARG_QRSTRING,pPanelCmdObj->qrstring);
	return 0;
}
int GpioCtrlRpc::bin_to_json_qrcodeimg_set(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,MPLAYSRV_SHOWFBIMG_PACKET);
	return 0;
}
int GpioCtrlRpc::process_qrcodeimg_set(JsonDataCommObj* pReq)//,ADJsonRpcMgrProducer* pObj)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	MPLAYSRV_QRCODEIMG_PACKET* pPacket;
	pPacket=(MPLAYSRV_QRCODEIMG_PACKET*)pPanelReq->dataRef;
	pPanelReq->result=create_qrcode_image(pPacket->qrfilepath,pPacket->qrstring);
	return 0;		
}
RPC_SRV_RESULT GpioCtrlRpc::create_qrcode_image(char* qrfilepath,char* qrstring)
{
	//qrencode -d 500 -s 37  "DP-3" -o /tmp/image.png :TODO: for the moment, qrcode size is fixed, to be configurable via rpc
	char command[1024];
	auto_scale_qr_size(strlen(qrstring),&pDataCache->qr_size);
	sprintf(command,"qrencode -d %d -s %d \"%s\" -o %s",pDataCache->qr_density,pDataCache->qr_size,qrstring,qrfilepath);
	if (system(command)==0)
		return RPC_SRV_RESULT_SUCCESS;
	else
		return RPC_SRV_RESULT_FAIL;
}
RPC_SRV_RESULT GpioCtrlRpc::auto_scale_qr_size(int qrstrlen,int* qr_size)
{
	//following sizes are tested on fullHD screen(based on the string length, this function defines the qrcode size)
	//char=size
	//15=37
	//20=35
	//25=34
	//26=35
	//30=33
	//34=31
	//42=30
	//52=31
	*qr_size=37;
	if(qrstrlen<=15)*qr_size=37;
	else if(qrstrlen>15 && qrstrlen<=20)*qr_size=37;
	else if(qrstrlen>20 && qrstrlen<=25)*qr_size=34;
	else if(qrstrlen>25 && qrstrlen<=30)*qr_size=33;
	else if(qrstrlen>30 && qrstrlen<=35)*qr_size=31;
	else if(qrstrlen>35)*qr_size=30;
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */

