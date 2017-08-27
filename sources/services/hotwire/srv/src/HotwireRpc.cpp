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
	process_show_pattern(pDataCache->StartupBkgnd);//on bootup, show solid color
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
		case EJSON_GPIOCTL_RPC_OMXACT_GET:return json_to_bin_omxact_get(pReq);
		case EJSON_GPIOCTL_RPC_OMXACT_SET:return json_to_bin_omxact_set(pReq);
		case EJSON_MPLAYSRV_RPC_SHOWFBIMG_GET:return json_to_bin_showfbimg_get(pReq);
		case EJSON_MPLAYSRV_RPC_SHOWFBIMG_SET:return json_to_bin_showfbimg_set(pReq);
		case EJSON_MPLAYSRV_RPC_QRCODEIMG_SET:return json_to_bin_qrcodeimg_set(pReq);
		case EJSON_MPLAYSRV_RPC_PATTERN_GET:return json_to_bin_pattern_get(pReq);
		case EJSON_MPLAYSRV_RPC_PATTERN_SET:return json_to_bin_pattern_set(pReq);
		case EJSON_MPLAYSRV_RPC_MEDIAFILE_TYPE_GET:return json_to_bin_mediafile_type_get(pReq);
		case EJSON_MPLAYSRV_RPC_MEDIAFILE_TYPE_SET:return json_to_bin_mediafile_type_set(pReq);
		case EJSON_MPLAYSRV_RPC_MEDIAFILE_GET:return json_to_bin_mediafile_get(pReq);
		case EJSON_MPLAYSRV_RPC_MEDIAFILE_SET:return json_to_bin_mediafile_set(pReq);
		case EJSON_MPLAYSRV_RPC_MEDIA_ACTION_SET:return json_to_bin_media_action_set(pReq);
		case EJSON_MPLAYSRV_RPC_GRAPHICS_OUT_GET:return json_to_bin_graphics_out_get(pReq);
		case EJSON_MPLAYSRV_RPC_GRAPHICS_OUT_SET:return json_to_bin_graphics_out_set(pReq);
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
		case EJSON_GPIOCTL_RPC_OMXACT_GET:return bin_to_json_omxact_get(pReq);
		case EJSON_GPIOCTL_RPC_OMXACT_SET:return bin_to_json_omxact_set(pReq);
		case EJSON_MPLAYSRV_RPC_SHOWFBIMG_GET:return bin_to_json_showfbimg_get(pReq);
		case EJSON_MPLAYSRV_RPC_SHOWFBIMG_SET:return bin_to_json_showfbimg_set(pReq);
		case EJSON_MPLAYSRV_RPC_QRCODEIMG_SET:return bin_to_json_qrcodeimg_set(pReq);
		case EJSON_MPLAYSRV_RPC_PATTERN_GET:return bin_to_json_pattern_get(pReq);
		case EJSON_MPLAYSRV_RPC_PATTERN_SET:return bin_to_json_pattern_set(pReq);
		case EJSON_MPLAYSRV_RPC_MEDIAFILE_TYPE_GET:return bin_to_json_mediafile_type_get(pReq);
		case EJSON_MPLAYSRV_RPC_MEDIAFILE_TYPE_SET:return bin_to_json_mediafile_type_set(pReq);
		case EJSON_MPLAYSRV_RPC_MEDIAFILE_GET:return bin_to_json_mediafile_get(pReq);
		case EJSON_MPLAYSRV_RPC_MEDIAFILE_SET:return bin_to_json_mediafile_set(pReq);
		case EJSON_MPLAYSRV_RPC_MEDIA_ACTION_SET:return bin_to_json_media_action_set(pReq);
		case EJSON_MPLAYSRV_RPC_GRAPHICS_OUT_GET:return bin_to_json_graphics_out_get(pReq);
		case EJSON_MPLAYSRV_RPC_GRAPHICS_OUT_SET:return bin_to_json_graphics_out_set(pReq);
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
		case EJSON_GPIOCTL_RPC_OMXACT_GET:return process_omxact_get(pReq);
		case EJSON_GPIOCTL_RPC_OMXACT_SET:return process_omxact_set(pReq,pObj);
		case EJSON_MPLAYSRV_RPC_SHOWFBIMG_GET:return process_showfbimg_get(pReq);
		case EJSON_MPLAYSRV_RPC_SHOWFBIMG_SET:return process_showfbimg_set(pReq);
		case EJSON_MPLAYSRV_RPC_QRCODEIMG_SET:return process_qrcodeimg_set(pReq);
		case EJSON_MPLAYSRV_RPC_PATTERN_GET:return process_pattern_get(pReq);
		case EJSON_MPLAYSRV_RPC_PATTERN_SET:return process_pattern_set(pReq);
		case EJSON_MPLAYSRV_RPC_MEDIAFILE_TYPE_GET:return process_mediafile_type_get(pReq);
		case EJSON_MPLAYSRV_RPC_MEDIAFILE_TYPE_SET:return process_mediafile_type_set(pReq);
		case EJSON_MPLAYSRV_RPC_MEDIAFILE_GET:return process_mediafile_get(pReq);
		case EJSON_MPLAYSRV_RPC_MEDIAFILE_SET:return process_mediafile_set(pReq);
		case EJSON_MPLAYSRV_RPC_MEDIA_ACTION_SET:return process_media_action_set(pReq);
		case EJSON_MPLAYSRV_RPC_GRAPHICS_OUT_GET:return process_graphics_out_get(pReq);
		case EJSON_MPLAYSRV_RPC_GRAPHICS_OUT_SET:return process_graphics_out_set(pReq);
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

//raspi without xwindows
//http://www.instructables.com/id/Raspberry-Pi-Wall-Display-Without-X-Windows/
//framebuffer based slideshow
//sudo fbi -a --noverbose -T 1 -t 10 image1.jpg image2.jpg image3.jpg

//omxplayer -b --layer 2 -r -o both /tmp/1.h264
//fbset -depth 8 && fbset -depth 16 (re-enables /dev/fb0 output)

//omxplayer documentation:
//https://www.raspberrypi.org/documentation/raspbian/applications/omxplayer.md

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
	sprintf(command,"fbv -f %s < %s &",imgfile.c_str(),IMG_RENDER_FIFO_FILE);
	system(command);
	//with following command, frame is rendered on the screen(need to check why?)
	sprintf(command,"echo . > %s",IMG_RENDER_FIFO_FILE);
	system(command);

	pDataCache->ScreenStatus=MPLAYSRV_SCREENSTS_IMAGE;

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
int GpioCtrlRpc::json_to_bin_pattern_get(JsonDataCommObj* pReq)
{
	MPLAYSRV_PATTERN_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,MPLAYSRV_PATTERN_PACKET,RPC_SRV_ACT_READ,EJSON_MPLAYSRV_RPC_PATTERN_GET);
	return 0;
}
int GpioCtrlRpc::bin_to_json_pattern_get(JsonDataCommObj* pReq)
{
PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,MPLAYSRV_PATTERN_PACKET,MPLAYSRV_RPC_PATTERN_ARG,PatType,MPLAYSRV_RPC_PATTERN_ARG_TABL,MPLAYSRV_PATTERN_UNKNOWN);
	return 0;
}
int GpioCtrlRpc::process_pattern_get(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	MPLAYSRV_PATTERN_PACKET* pPacket;
	pPacket=(MPLAYSRV_PATTERN_PACKET*)pPanelReq->dataRef;
	pPacket->PatType=pDataCache->pattern;
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::json_to_bin_pattern_set(JsonDataCommObj* pReq)
{
	MPLAYSRV_PATTERN_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,MPLAYSRV_PATTERN_PACKET,RPC_SRV_ACT_WRITE,EJSON_MPLAYSRV_RPC_PATTERN_SET);
	JSON_STRING_TO_ENUM(MPLAYSRV_RPC_PATTERN_ARG,MPLAYSRV_RPC_PATTERN_ARG_TABL,MPLAYSRV_PATTERN_TYPE,MPLAYSRV_PATTERN_UNKNOWN,pPanelCmdObj->PatType);
	return 0;
}
int GpioCtrlRpc::bin_to_json_pattern_set(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,MPLAYSRV_PATTERN_PACKET);
	return 0;
}
int GpioCtrlRpc::process_pattern_set(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	MPLAYSRV_PATTERN_PACKET* pPacket;
	pPacket=(MPLAYSRV_PATTERN_PACKET*)pPanelReq->dataRef;
	pDataCache->pattern=pPacket->PatType;
	pPanelReq->result=process_show_pattern(pDataCache->pattern);
	return 0;		
}
RPC_SRV_RESULT GpioCtrlRpc::process_show_pattern(MPLAYSRV_PATTERN_TYPE pat)
{
	char command[1024];char pat_cmd[255];
	switch(pat)
	{
		case MPLAYSRV_PATTERN_RED:sprintf(pat_cmd,"-r");break;
		case MPLAYSRV_PATTERN_GREEN:sprintf(pat_cmd,"-g");break;
		case MPLAYSRV_PATTERN_BLUE:sprintf(pat_cmd,"-b");break;
		case MPLAYSRV_PATTERN_CYAN:sprintf(pat_cmd,"-c");break;
		case MPLAYSRV_PATTERN_MAGENTA:sprintf(pat_cmd,"-m");break;
		case MPLAYSRV_PATTERN_YELLOW:sprintf(pat_cmd,"-y");break;
		case MPLAYSRV_PATTERN_WHITE:sprintf(pat_cmd,"-w");break;
		case MPLAYSRV_PATTERN_BLACK:sprintf(pat_cmd,"-l");break;
		default:return RPC_SRV_RESULT_ARG_ERROR;
	}
	sprintf(command,"fb-test -f 0 %s>/dev/null",pat_cmd);
	system(command);
	pDataCache->ScreenStatus=MPLAYSRV_SCREENSTS_PATTERN;
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::json_to_bin_mediafile_type_get(JsonDataCommObj* pReq)
{
	MPLAYSRV_MEDIA_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,MPLAYSRV_MEDIA_PACKET,RPC_SRV_ACT_READ,EJSON_MPLAYSRV_RPC_MEDIAFILE_TYPE_GET);
	return 0;
}
int GpioCtrlRpc::bin_to_json_mediafile_type_get(JsonDataCommObj* pReq)
{
PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,MPLAYSRV_MEDIA_PACKET,MPLAYSRV_RPC_MEDIAFILE_TYPE_ARG,MediaFileType,MPLAYSRV_RPC_MEDIAFILE_TYPE_ARG_TABL,MPLAYSRV_MEDIAFILE_TYPE_UNKNOWN);
	return 0;
}
int GpioCtrlRpc::process_mediafile_type_get(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	MPLAYSRV_MEDIA_PACKET* pPacket;
	pPacket=(MPLAYSRV_MEDIA_PACKET*)pPanelReq->dataRef;
	pPacket->MediaFileType=pDataCache->MediaFileType;//TODO
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
int GpioCtrlRpc::json_to_bin_mediafile_type_set(JsonDataCommObj* pReq)
{
	MPLAYSRV_MEDIA_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,MPLAYSRV_MEDIA_PACKET,RPC_SRV_ACT_WRITE,EJSON_MPLAYSRV_RPC_MEDIAFILE_TYPE_SET);
	JSON_STRING_TO_ENUM(MPLAYSRV_RPC_MEDIAFILE_TYPE_ARG,MPLAYSRV_RPC_MEDIAFILE_TYPE_ARG_TABL,MPLAYSRV_MEDIAFILE_TYPE,MPLAYSRV_MEDIAFILE_TYPE_UNKNOWN,pPanelCmdObj->MediaFileType);
	return 0;
}
int GpioCtrlRpc::bin_to_json_mediafile_type_set(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,MPLAYSRV_MEDIA_PACKET);
	return 0;
}
int GpioCtrlRpc::process_mediafile_type_set(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	MPLAYSRV_MEDIA_PACKET* pPacket;
	pPacket=(MPLAYSRV_MEDIA_PACKET*)pPanelReq->dataRef;
	pDataCache->MediaFileType=pPacket->MediaFileType;//TODO
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::json_to_bin_mediafile_get(JsonDataCommObj* pReq)
{
	MPLAYSRV_MEDIA_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,MPLAYSRV_MEDIA_PACKET,RPC_SRV_ACT_READ,EJSON_MPLAYSRV_RPC_MEDIAFILE_GET);
	return 0;
}
int GpioCtrlRpc::bin_to_json_mediafile_get(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP_STRING(RPC_SRV_REQ,MPLAYSRV_MEDIA_PACKET,MPLAYSRV_RPC_MEDIAFILE_ARG,MediaFilePath);
	return 0;
}
int GpioCtrlRpc::process_mediafile_get(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	MPLAYSRV_MEDIA_PACKET* pPacket;
	pPacket=(MPLAYSRV_MEDIA_PACKET*)pPanelReq->dataRef;
	if(pPanelReq->action!=RPC_SRV_ACT_READ)
	{
		pPanelReq->result=RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
		return 0;
	}
	strcpy(pPacket->MediaFilePath,pDataCache->MediaFilePath.c_str());
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
int GpioCtrlRpc::json_to_bin_mediafile_set(JsonDataCommObj* pReq)
{
	MPLAYSRV_MEDIA_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,MPLAYSRV_MEDIA_PACKET,RPC_SRV_ACT_WRITE,EJSON_MPLAYSRV_RPC_MEDIAFILE_SET);
	JSON_STRING_TO_STRING(MPLAYSRV_RPC_MEDIAFILE_ARG,pPanelCmdObj->MediaFilePath);
	return 0;
}
int GpioCtrlRpc::bin_to_json_mediafile_set(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,MPLAYSRV_SHOWFBIMG_PACKET);
	return 0;
}
int GpioCtrlRpc::process_mediafile_set(JsonDataCommObj* pReq)//,ADJsonRpcMgrProducer* pObj)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	MPLAYSRV_MEDIA_PACKET* pPacket;
	pPacket=(MPLAYSRV_MEDIA_PACKET*)pPanelReq->dataRef;
	pDataCache->MediaFilePath=pPacket->MediaFilePath;
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;//TODO//=process_show_image(pDataCache->fbimgpath);//RPC_SRV_RESULT_SUCCESS;
	return 0;		
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::json_to_bin_media_action_set(JsonDataCommObj* pReq)
{
	MPLAYSRV_MEDIA_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,MPLAYSRV_MEDIA_PACKET,RPC_SRV_ACT_WRITE,EJSON_MPLAYSRV_RPC_MEDIA_ACTION_SET);
	JSON_STRING_TO_ENUM(MPLAYSRV_RPC_MEDIA_ACTION_ARG,MPLAYSRV_RPC_MEDIA_ACTION_ARG_TABL,MPLAYSRV_MEDIA_ACTION,MPLAYSRV_MEDIA_ACTION_UNKNOWN,pPanelCmdObj->MediaAction);
	return 0;
}
int GpioCtrlRpc::bin_to_json_media_action_set(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,MPLAYSRV_MEDIA_PACKET);
	return 0;
}
int GpioCtrlRpc::process_media_action_set(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	MPLAYSRV_MEDIA_PACKET* pPacket;
	pPacket=(MPLAYSRV_MEDIA_PACKET*)pPanelReq->dataRef;
	//pDataCache->MediaAction=pPacket->MediaAction;//TODO
	pPanelReq->result=process_media_action(pPacket->MediaAction);//RPC_SRV_RESULT_SUCCESS;
	return 0;
}
RPC_SRV_RESULT GpioCtrlRpc::process_media_action(MPLAYSRV_MEDIA_ACTION act)
{
	char command[1024];
	bool omx_sts=false;
	//sprintf(command,"pgrep omxplayer");
	//if(system(command)==0)
	//	omx_sts=true;
	//else
	//	omx_sts=false;
	omx_sts=is_omx_running();

	switch(act)
	{
		case MPLAYSRV_MEDIA_ACTION_START :
				if(omx_sts==true)
					return RPC_SRV_RESULT_ACTION_NOT_ALLOWED;//video is already running, stop it first
				if(pDataCache->MediaFileType!=MPLAYSRV_MEDIAFILE_TYPE_MEDIA)
						return RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
				if(pDataCache->MediaFilePath=="")
						return RPC_SRV_RESULT_FILE_NOT_FOUND;//RPC_SRV_RESULT_ACTION_NOT_ALLOWED;
				sprintf(command,"mkfifo /tmp/omxplay.fifo;rm -rf /tmp/omxplay.finished");
				system(command);

				if(!pDataCache->LoopVideo)
				{
					//TODO: check if the media file exists, else return file-not-found
					sprintf(command,"(omxplayer -b --layer 2 -r -o both %s;fbset -depth 8 && fbset -depth 16;touch /tmp/omxplay.finished) < /tmp/omxplay.fifo &",pDataCache->MediaFilePath.c_str());
					system(command);
					sprintf(command,"echo . > /tmp/omxplay.fifo");
				}
				else
				{
					sprintf(command,"/opt/fmw/misc_binaries/omx-loop.sh %s &",pDataCache->MediaFilePath.c_str());
				}
				system(command);
				pDataCache->VideoPaused=false;
				//pDataCache->ActType=GPIOCTL_OMXACT_START;//TODO
				return RPC_SRV_RESULT_SUCCESS;		
				break;
		case MPLAYSRV_MEDIA_ACTION_PAUSE  ://user for both pause or play(sending pause on paused video, will start playing)
				if(omx_sts==true)
				{
					sprintf(command,"echo -n p > /tmp/omxplay.fifo");
					system(command);
					//pDataCache->ActType=GPIOCTL_OMXACT_INTR;//pPacket->ActType;
					pDataCache->VideoPaused=!pDataCache->VideoPaused;
					return RPC_SRV_RESULT_SUCCESS;
				}
				else
					return RPC_SRV_RESULT_ACTION_NOT_ALLOWED;//video is already stopped;//RPC_SRV_RESULT_FAIL;
				break;
		case MPLAYSRV_MEDIA_ACTION_STOP  :
				if(omx_sts==true)
				{
					if(pDataCache->LoopVideo)
					{
						sprintf(command,"touch /tmp/omxplay.stoploop");
						system(command);
					}
					sprintf(command,"echo -n q > /tmp/omxplay.fifo");
					system(command);
					//sleep(1);
					//sprintf(command,"fbset -depth 8 && fbset -depth 16");//needed, so that /dev/fb0 can be rendered again
					//system(command);
					//pDataCache->ActType=GPIOCTL_OMXACT_INTR;//pPacket->ActType;//TODO

					pDataCache->VideoPaused=false;
					pDataCache->ScreenStatus=MPLAYSRV_SCREENSTS_BLANK;
					return RPC_SRV_RESULT_SUCCESS;
				}
				else
					return RPC_SRV_RESULT_ACTION_NOT_ALLOWED;//video is already stopped
				break;
		default:
			break;
	}
	return RPC_SRV_RESULT_FAIL;
}
/* ------------------------------------------------------------------------- */
int GpioCtrlRpc::json_to_bin_graphics_out_get(JsonDataCommObj* pReq)
{
	MPLAYSRV_MEDIA_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,MPLAYSRV_MEDIA_PACKET,RPC_SRV_ACT_READ,EJSON_MPLAYSRV_RPC_GRAPHICS_OUT_GET);
	return 0;
}
int GpioCtrlRpc::bin_to_json_graphics_out_get(JsonDataCommObj* pReq)
{
PREPARE_JSON_RESP_ENUM(RPC_SRV_REQ,MPLAYSRV_MEDIA_PACKET,MPLAYSRV_RPC_GRAPHICS_OUT_ARG,GraphicsOut,MPLAYSRV_RPC_GRAPHICS_OUT_ARG_TABL,MPLAYSRV_GRAPHICS_OUT_UNKNOWN);
	return 0;
}
int GpioCtrlRpc::process_graphics_out_get(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	MPLAYSRV_MEDIA_PACKET* pPacket;
	pPacket=(MPLAYSRV_MEDIA_PACKET*)pPanelReq->dataRef;
	pPacket->GraphicsOut=pDataCache->GraphicsOut;
	pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
	return 0;
}
int GpioCtrlRpc::json_to_bin_graphics_out_set(JsonDataCommObj* pReq)
{
	MPLAYSRV_MEDIA_PACKET* pPanelCmdObj=NULL;
	PREPARE_JSON_REQUEST(RPC_SRV_REQ,MPLAYSRV_MEDIA_PACKET,RPC_SRV_ACT_WRITE,EJSON_MPLAYSRV_RPC_GRAPHICS_OUT_SET);
	JSON_STRING_TO_ENUM(MPLAYSRV_RPC_GRAPHICS_OUT_ARG,MPLAYSRV_RPC_GRAPHICS_OUT_ARG_TABL,MPLAYSRV_GRAPHICS_OUT,MPLAYSRV_GRAPHICS_OUT_UNKNOWN,pPanelCmdObj->GraphicsOut);
	return 0;
}
int GpioCtrlRpc::bin_to_json_graphics_out_set(JsonDataCommObj* pReq)
{
	PREPARE_JSON_RESP(RPC_SRV_REQ,MPLAYSRV_MEDIA_PACKET);
	return 0;
}
int GpioCtrlRpc::process_graphics_out_set(JsonDataCommObj* pReq)
{
	RPC_SRV_REQ *pPanelReq=NULL;
	pPanelReq=(RPC_SRV_REQ *)pReq->pDataObj;
	MPLAYSRV_MEDIA_PACKET* pPacket;
	pPacket=(MPLAYSRV_MEDIA_PACKET*)pPanelReq->dataRef;

	char command[1024];
	if(pPacket->GraphicsOut==MPLAYSRV_GRAPHICS_OUT_DISABLE)
		sprintf(command,"tvservice -o");
	else
		sprintf(command,"tvservice -p");
	
	if (system(command)==0)
	{
		pPanelReq->result=RPC_SRV_RESULT_SUCCESS;
		pDataCache->GraphicsOut=pPacket->GraphicsOut;
	}
	else
		pPanelReq->result=RPC_SRV_RESULT_FAIL;

	return 0;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT GpioCtrlRpc::UpdateScreenStatus()
{
	bool omx_sts=false;

	if(pDataCache->GraphicsOut==MPLAYSRV_GRAPHICS_OUT_DISABLE)
	{
		pDataCache->ScreenStatus=MPLAYSRV_SCREENSTS_OFF;
		return RPC_SRV_RESULT_SUCCESS;
	}	
	if(is_omx_running()==true)
	{
		if(pDataCache->VideoPaused==true)
		{
			pDataCache->ScreenStatus=MPLAYSRV_SCREENSTS_MPAUSE;
			return RPC_SRV_RESULT_SUCCESS;
		}
		else
		{
			pDataCache->ScreenStatus=MPLAYSRV_SCREENSTS_MPLAY;//MPLAYSRV_SCREENSTS_BLANK;
			return RPC_SRV_RESULT_SUCCESS;
		}
	}
	else
	{
			//TODO:
			//if("/tmp/omxplay.finished" file exists)
			//pDataCache->ScreenStatus=MPLAYSRV_SCREENSTS_BLANK;
			//return RPC_SRV_RESULT_SUCCESS;
	}

	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */

