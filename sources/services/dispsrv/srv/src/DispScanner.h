#ifndef __DISP_SCANNER_H_
#define __DISP_SCANNER_H_
#include <iostream>
//#include "ADCommon.hpp"
#include <stdint.h>
//#include "ArduiPi_OLED_lib.h"
//#include "Adafruit_GFX.h"
//#include "ArduiPi_OLED.h"
#include "I2CBusAccess.h"
#include "ADTimer.hpp"
#include "DispsrvJsonDef.h"
#define DISP_SCAN_DURATION 30 //100ms * 30 = scan every 3sec
using namespace std;
class DispScanner : public ADTimerConsumer
{
    uint8_t last_found_dev;
    DISPSRV_CMN_DATA_CACHE* pRpcData;
    unsigned int TimerCount;
    ADTimer* myTimer;
	virtual int sigio_notification(){return 0;};
	virtual int timer_notification();//{cout<<"timer-func"<<endl;return 0;};
	virtual int custom_sig_notification(int signum){return 0;};
    int check_for_i2c_addr(uint8_t addr,char* node);
    int probe_i2c_addr(char* devnode, uint8_t addr);
    DisplayDevice* create_display_device(std::string DevNode,std::string Type);
    std::string get_my_ipaddr();
public:
	DispScanner(DISPSRV_CMN_DATA_CACHE *pDataCache=NULL);
	~DispScanner();
    int AttachHeartBeat(ADTimer* pTimer);
};
#endif
