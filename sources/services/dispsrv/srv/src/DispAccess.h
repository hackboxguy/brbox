#ifndef __DISP_ACCESS_H_
#define __DISP_ACCESS_H_
#include <iostream>
#include "ADCommon.hpp"
#include <stdint.h>
#include "ArduiPi_OLED_lib.h"
#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"
#include "DisplayDevice.hpp"
using namespace std;
class DispAccess : public DisplayDevice
{
	//int fd;
	RPC_SRV_RESULT DispOpened;
	std::string DispType;
        ArduiPi_OLED Display;
public:
	DispAccess(std::string DevNode,std::string DevType);
	~DispAccess();
	RPC_SRV_RESULT init_display();
	RPC_SRV_RESULT clear_display();
	//RPC_SRV_RESULT print_line(int line, char* msg);
	RPC_SRV_RESULT print_line(char* msg,DISPLAY_LINE line,TEXT_ALIGNMENT align);
	RPC_SRV_RESULT set_back_light(bool sts);
	RPC_SRV_RESULT get_back_light(bool &sts);

	//RPC_SRV_RESULT read_byte(uint32_t addr, uint8_t *data);
	//RPC_SRV_RESULT write_byte(uint32_t addr, uint8_t data);
	//RPC_SRV_RESULT read_word(uint32_t addr, uint16_t *data);
	//RPC_SRV_RESULT write_word(uint32_t addr, uint16_t data);
	//RPC_SRV_RESULT read_dword(uint32_t addr, uint32_t *data);
	//RPC_SRV_RESULT write_dword(uint32_t addr, uint32_t data);
};
#endif
