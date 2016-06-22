#ifndef __DISP_ACCESS_H_
#define __DISP_ACCESS_H_
#include <iostream>
#include "ADCommon.hpp"
#include <stdint.h>
#include "ArduiPi_OLED_lib.h"
#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"
using namespace std;
class DispAccess
{
	//int fd;
	RPC_SRV_RESULT DispOpened;
	std::string DispType;
        ArduiPi_OLED Display;
public:
	DispAccess(std::string disptype);
	~DispAccess();
	RPC_SRV_RESULT init_display();
	RPC_SRV_RESULT clear_display();
	RPC_SRV_RESULT print_line(int line, char* msg);
	//RPC_SRV_RESULT read_byte(uint32_t addr, uint8_t *data);
	//RPC_SRV_RESULT write_byte(uint32_t addr, uint8_t data);
	//RPC_SRV_RESULT read_word(uint32_t addr, uint16_t *data);
	//RPC_SRV_RESULT write_word(uint32_t addr, uint16_t data);
	//RPC_SRV_RESULT read_dword(uint32_t addr, uint32_t *data);
	//RPC_SRV_RESULT write_dword(uint32_t addr, uint32_t data);
};
#endif
