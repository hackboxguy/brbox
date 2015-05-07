//http://elinux.org/RPi_Low-level_peripherals
//http://codeandlife.com/2015/03/25/raspberry-pi-2-vs-1-gpio-benchmark/
//http://www.robot-electronics.co.uk/htm/raspberry_pi_examples.htm
//http://elinux.org/RPi_HardwareHistory
//http://raspi.tv/2013/rpi-gpio-basics-2-how-to-check-what-pi-board-revision-you-have

#ifndef __RASPI_IO_H_
#define __RASPI_IO_H_
#include <stdio.h>
#include <iostream>
#include "ADCommon.hpp"
#include "GpioctlJsonDef.h"
using namespace std;
/*---------------------------------------------------------------------------*/
class  RaspiIo
{
	//RPC_SRV_RESULT capture_camera_image(int video_device,int frmPixels,int frmLines,Mat& image);
	//RPC_SRV_RESULT identify_image_box(/*bool logImage=false,*/std::string imgPath="");//check identify-pattern-box exists
	bool NodeOpen;
	volatile unsigned *pGpio;
	RPC_SRV_RESULT OpenGPIONode();
	RPC_SRV_RESULT CloseGPIONode();

public:
	bool logflag;
	RaspiIo();
	~ RaspiIo();
	RPC_SRV_RESULT ReadGPIO (unsigned int addr,unsigned int &data);
	RPC_SRV_RESULT WriteGPIO(unsigned int addr,unsigned int data);
};
/*---------------------------------------------------------------------------*/
#endif
