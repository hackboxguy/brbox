#ifndef __DISPLAY_DEVICE_H_
#define __DISPLAY_DEVICE_H_
#include <iostream>
#include "ADCommon.hpp"
#include <stdint.h>
#include "I2CBusAccess.h"
using namespace std;
typedef enum DISPLAY_LINE_T
{
	DISPLAY_LINE_1,
	DISPLAY_LINE_2,
	DISPLAY_LINE_3,
	DISPLAY_LINE_4,
	DISPLAY_LINE_FULL,
	DISPLAY_LINE_UNKNOWN,
	DISPLAY_LINE_NONE
}DISPLAY_LINE;
typedef enum TEXT_ALIGNMENT_T
{
	TEXT_ALIGNMENT_LEFT,
	TEXT_ALIGNMENT_RIGHT,
	TEXT_ALIGNMENT_CENTER,
	TEXT_ALIGNMENT_UNKNOWN,
	TEXT_ALIGNMENT_NONE
}TEXT_ALIGNMENT;
class DisplayDevice : public I2CBusAccess
{
public:
	DisplayDevice(std::string devnode);
	~DisplayDevice();
	virtual RPC_SRV_RESULT init_display()=0;
	virtual RPC_SRV_RESULT clear_display()=0;
	virtual RPC_SRV_RESULT print_line(char* msg,DISPLAY_LINE line=DISPLAY_LINE_1,TEXT_ALIGNMENT align=TEXT_ALIGNMENT_CENTER)=0;
	//RPC_SRV_RESULT init_display();
	//RPC_SRV_RESULT clear_display();
	//RPC_SRV_RESULT print_line(int line, char* msg);
};
#endif
