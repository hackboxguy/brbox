#include "DispScanner.h"
#include <linux/i2c-dev.h>

#include "DispsrvJsonDef.h"
#include "DispAccess.h"
#include "DispCtrlRpc.h"
#include "I2CDualPcfLcd.hpp"
#include "I2CPcfLcd.hpp"
#include "I2CSsd1306.hpp"
#include "I2CBusAccess.h"
#include "ADSysInfo.hpp"
/*****************************************************************************/
DispScanner::DispScanner(DISPSRV_CMN_DATA_CACHE *pDataCache)//std::string DevNode,std::string DevType):DisplayDevice(DevNode)
{
    pRpcData=pDataCache;
    TimerCount=0;
    last_found_dev=0;
}
/*****************************************************************************/
DispScanner::~DispScanner()
{
}
/*****************************************************************************/
int DispScanner::AttachHeartBeat(ADTimer* pTimer)
{
	//give 100ms heartbeat to DispScanner
	pTimer->subscribe_timer_notification(this);
	myTimer=pTimer;
	return 0;
}
/*****************************************************************************/
int DispScanner::timer_notification()
{
    char node[256];node[0]='\0';
    ADLIB_DISPLAY_TYPE disp_type=pRpcData->disp_type;//user preferred disp_type
    const char *dispTbl[] = ADLIB_DISPLAY_TYPE_TABL;
    TimerCount++;
    if( (TimerCount % DISP_SCAN_DURATION) ==0 )
    {
        //if(pRpcData->pDisplay == NULL)
        //{
            if(check_for_i2c_addr(0x3c,node)==0)//ssd1306 display
            {
                //check for usr preferred disp type
                if(pRpcData->disp_type==ADLIB_DISPLAY_TYPE_SSD1306_128x64 || pRpcData->disp_type==ADLIB_DISPLAY_TYPE_SSD1306_128x32)
                    disp_type=pRpcData->disp_type;
                else
                    disp_type=ADLIB_DISPLAY_TYPE_SSD1306_128x32;
                last_found_dev=0x3c;
                //printf("found 0x3c\n");
            }
            else if(check_for_i2c_addr(0x3f,node)==0)//pcf8574 based 16x2 lcd display
            {
                disp_type=ADLIB_DISPLAY_TYPE_1602_PCF;
                last_found_dev=0x3f;
                //printf("found 0x3f\n");
            }
            else if(check_for_i2c_addr(0x27,node)==0)//pcf8574 based 16x4 lcd display
            {
                if(pRpcData->disp_type==ADLIB_DISPLAY_TYPE_2002_PCF || pRpcData->disp_type==ADLIB_DISPLAY_TYPE_2004_PCF)//user preferred
                    disp_type=pRpcData->disp_type;
                else
                    disp_type=ADLIB_DISPLAY_TYPE_1602_DUAL_PCF;
                last_found_dev=0x27;
                //printf("found 0x27\n");
            }
            else
                last_found_dev=0;
        //}
        if(pRpcData->pDisplay != NULL && last_found_dev==0)//display has been unplugged
        {
                delete pRpcData->pDisplay;
                pRpcData->pDisplay=NULL;
        }
        if(pRpcData->pDisplay == NULL && last_found_dev!=0)//display has been plugged
        {
            pRpcData->pDisplay=create_display_device(node,dispTbl[disp_type]);//create new pDisplay object
            if(pRpcData->pDisplay != NULL)
            {
                pRpcData->pDisplay->init_display();
                pRpcData->pDisplay->clear_display();
                pRpcData->pDisplay->print_line("Welcome",DISPLAY_LINE_1);
                std::string tmp = get_my_ipaddr(pRpcData->net_interface);//override specific net interface if requested via cmdline arg
                pRpcData->pDisplay->print_line((char*)tmp.c_str(),DISPLAY_LINE_2);
            }
        }
    }
    return 0;
}
/*****************************************************************************/
int DispScanner::check_for_i2c_addr(uint8_t addr,char *node)
{
    char devnode[255];
    uint8_t data[255];
    int file;int res;
    for(int i=0;i<128;i++)
    {
        sprintf(devnode,"/dev/i2c-%d",i);
        res = probe_i2c_addr(devnode,addr);
        switch(res)
        {
            case -1://node-doesnt exists (stop trying for next)
            case  0:strcpy(node,devnode);return res;//node-exists and device exists(return detected node)
            default:break;//node-exists but device doesnt exist(try next)
        }

        /*if ((file = open(devnode, O_RDWR)) < 0)
        {
            //printf("unable to open file %s\n",devnode);
            return -1;
        }
        else
        {
            if (ioctl(file, I2C_SLAVE, addr) < 0)
            {
                //printf("unable to set slave addr:%s\n",devnode);
                close(file);
                return -1;
            }
            else
            {
                if (read(file,data,1) != 1)
                {
                        //printf("unable to read from slave addr: %s\n",devnode);
                        close(file);//try next device node
                }
                else
                {
                        //printf("read success from slave addr: %s\n",devnode);
                        close(file);
                        return 0;
                }
            }
        }*/
    }
    return -1;
}
/*****************************************************************************/
int DispScanner::probe_i2c_addr(char* devnode, uint8_t addr)
{
    //return -1: node-doesnt exists (stop trying for next)
    //return  0: node-exists and device exists.
    //return  1: node-exists but device doesnt exist(try next)
    uint8_t data[255];
    int file;
    if ((file = open(devnode, O_RDWR)) < 0)
    {
        //printf("unable to open file %s\n",devnode);
        return -1;
    }
    else
    {
        if (ioctl(file, I2C_SLAVE, addr) < 0)
        {
            //printf("unable to set slave addr:%s\n",devnode);
            close(file);
            return -1;
        }
        else
        {
            if (read(file,data,1) != 1)
            {
                    //printf("unable to read from slave addr: %s\n",devnode);
                    close(file);//try next device node
                    return 1;
            }
            else
            {
                    //printf("read success from slave addr: %s\n",devnode);
                    close(file);
                    return 0;
            }
        }
    }
}
/*****************************************************************************/
DisplayDevice* DispScanner::create_display_device(std::string DevNode,std::string Type)
{
	//cout<<"devnode:"<<DevNode<<" devtype:"<<Type<<endl;
	DisplayDevice* pDevice=NULL;
	ADLIB_DISPLAY_TYPE disp_type;
	const char *dispTbl[] = ADLIB_DISPLAY_TYPE_TABL;
	ADCmnStringProcessor string_proc;
	disp_type=(ADLIB_DISPLAY_TYPE)string_proc.string_to_enum(dispTbl,(char*)Type.c_str(),ADLIB_DISPLAY_TYPE_UNKNOWN);
	if(disp_type>=ADLIB_DISPLAY_TYPE_UNKNOWN)
		return NULL;//unable to determine which display object is needed
	switch(disp_type)
	{
		case ADLIB_DISPLAY_TYPE_SSD1306_128x32   :pDevice = new I2CSsd1306(DevNode,Type);break;
		case ADLIB_DISPLAY_TYPE_SSD1306_128x64   :pDevice = new I2CSsd1306(DevNode,Type);break;
		case ADLIB_DISPLAY_TYPE_SSD1306_128x32_PI:
		case ADLIB_DISPLAY_TYPE_SSD1306_128x64_PI:pDevice = new DispAccess(DevNode,Type);break;
		case ADLIB_DISPLAY_TYPE_1602_DUAL_PCF    ://pDevice = new I2CPcfLcd(DevNode,Type);break;//I2CDualPcfLcd
		case ADLIB_DISPLAY_TYPE_1602_PCF         ://pDevice = new I2CPcfLcd(DevNode,Type);break;
		case ADLIB_DISPLAY_TYPE_2002_PCF         ://pDevice = new I2CPcfLcd(DevNode,Type);break;
		case ADLIB_DISPLAY_TYPE_2004_PCF         ://pDevice = new I2CPcfLcd(DevNode,Type);break;
		case ADLIB_DISPLAY_TYPE_1604_PCF         :pDevice = new I2CPcfLcd(DevNode,Type);break;
		default: break;
	}
	return pDevice;
}
/*****************************************************************************/
std::string DispScanner::get_my_ipaddr(std::string interface)
{
    char mac[256];
    char ip[256];
    char netmask[256];
    ADSysInfo info;
    int total_detected=0;
    eth_name_ip_str if_list[MAX_ALLOWED_INTERFACES];
    if(info.probe_eth_interface_details(&total_detected,if_list)!=0)
    {
	//printf("probe_eth_interface_details_failed\n");
	return "unknownip";
    }
    
    if( interface != "" )
    {
	if(info.read_network_info((char*)interface.c_str(),mac,ip,netmask)==0)
		return ip;
	else
		return "unknownip";
    } 
    for(int i=0;i<total_detected;i++)
    {
        if(info.read_network_info(if_list[i].dev,mac,ip,netmask)==0)
            return ip;
    }
    return "unknownip";
}
/*****************************************************************************/
