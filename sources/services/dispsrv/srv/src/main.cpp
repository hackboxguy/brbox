#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ADJsonRpcMgr.hpp"
#include "MyCmdline.h"
#include "SrcControlVersion.h"
#include "ADTimer.hpp"
#include "ADEvntNotifier.hpp"
/* ------------------------------------------------------------------------- */
#include "DispsrvJsonDef.h"
//#include "GpioCtrlRpc.h"
//#include <linux/i2c-dev.h>
//#include "i2c-dev.h"
#include "ArduiPi_OLED_lib.h"
#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"
/* ------------------------------------------------------------------------- */
//supported display types
//--disptype=SSD1306_128x32
struct s_opts
{
        int oled;
        int verbose;
} ;
s_opts opts = 
{
	OLED_ADAFRUIT_I2C_128x32, //OLED_ADAFRUIT_SPI_128x32, // Default oled
	false   // Not verbose
};
/* ------------------------------------------------------------------------- */
using namespace std;
int main(int argc, const char* argv[])
{
	//cmdline parsing
	char ver[255];snprintf(ver,254,"%05d",SRC_CONTROL_VERSION);
	MyCmdline CmdLine(CMDLINE_HELPER_MODE_SERVER,SERVER_JSON_PORT_NUM,ver);//40000 is the portnumber
	CmdLine.parse_cmdline_arguments(argc,(char**)argv);
	if(CmdLine.is_help_printed())
		return -1;//user just requested to print the help info

	//read the board-type info from cmdline argument
	ADCMN_DEV_INFO DevInfo;
	CmdLine.get_dev_info(&DevInfo);

	bool dbglog = CmdLine.get_debug_log();
	bool emulat = CmdLine.get_emulation_mode();

	//start 100ms timer
	ADTimer AppTimer(100,CmdLine.get_port_number());//only one instance per application(or process) must exist
	//create a common data Cache of the service
	DISPSRV_CMN_DATA_CACHE DataCache;
	ADEvntNotifier EventNotifier;//global event notification object
	DataCache.pDevInfo=(void*)&DevInfo;//rpc's needs to know board or device type
	DataCache.pEventNotifier=(void*)&EventNotifier;

	//attach rpc classes to ADJsonRpcMgr
	ADJsonRpcMgr RpcMgr(SRC_CONTROL_VERSION,dbglog,&DevInfo); //main rpc handler

	/****************************RPC list*************************************/
	//GpioCtrlRpc GpioGet(GPIOCTL_RPC_IO_GET ,EJSON_GPIOCTL_RPC_IO_GET ,emulat,dbglog,&DataCache);
	//GpioCtrlRpc GpioSet(GPIOCTL_RPC_IO_SET ,EJSON_GPIOCTL_RPC_IO_SET ,emulat,dbglog,&DataCache);
	//RpcMgr.AttachRpc(&GpioGet);
	//RpcMgr.AttachRpc(&GpioSet);

	//start listening for rpc-commands
	RpcMgr.AttachHeartBeat(&AppTimer);//attach 100ms heartbeat to ADJsonRpcMgr
	//RpcMgr.SupportShutdownRpc(false);//this is a system-manager, needs to be alive all the time, hence dont support shutdown via rpc
	RpcMgr.Start(CmdLine.get_port_number(),CmdLine.get_socket_log(),CmdLine.get_emulation_mode());
	//server is ready to serve rpc's
	RpcMgr.SetServiceReadyFlag(EJSON_RPCGMGR_READY_STATE_READY);

	//display specific stuff
        ArduiPi_OLED display;
        // SPI
        if (display.oled_is_spi_proto(opts.oled))
        {
                // SPI change parameters to fit to your LCD
                if ( !display.init(OLED_SPI_DC,OLED_SPI_RESET,OLED_SPI_CS, opts.oled) )
                        exit(EXIT_FAILURE);
        }
        else
        {
                // I2C change parameters to fit to your LCD
                if ( !display.init(OLED_I2C_RESET,opts.oled) )
                        exit(EXIT_FAILURE);
        }
        display.begin();
        // init done
        display.clearDisplay(); // clears the screen buffer
        //char buffer[128];
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        //sprintf(buffer,"%s",argv[1]);
        //display.print(buffer);//"%s",argv[1]);//Hello, world!\n");
        display.print("Hello, world!\n");
        display.display();


	//wait for sigkill or sigterm signal
	AppTimer.wait_for_exit_signal();//loop till KILL or TERM signal is received
	AppTimer.stop_timer();//stop sending heart-beats to other objects
	return 0;
}
