#include "DevIdentRaspiAZero.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;
/*****************************************************************************/
DevIdentRaspiAZero::DevIdentRaspiAZero()
{
}
/*****************************************************************************/
DevIdentRaspiAZero::~DevIdentRaspiAZero()
{
}
/*****************************************************************************/
RPC_SRV_RESULT DevIdentRaspiAZero::device_identify()
{
	//# Set the Pi Zero ACT LED trigger to 'none'.
	//echo none | tee /sys/class/leds/led0/trigger
	//# Turn off the Pi Zero ACT LED.
	//echo 1 | tee /sys/class/leds/led0/brightness
	//# Turn ON the Pi Zero ACT LED.
	//echo 0 | tee /sys/class/leds/led0/brightness
	//# Set the Pi Zero ACT LED trigger to 'mmc0'.
	//echo mmc0 | tee /sys/class/leds/led0/trigger

	//set the ACT led mode to none
	char command[255];
	sprintf(command,"echo none | tee /sys/class/leds/led0/trigger");
	if(system(command)!=0)
	{
		//pPanelReq->result=RPC_SRV_RESULT_FILE_OPEN_ERR;
		//return 0;
		;
	}

	//blink the ACT LED
	for(int i=0;i<10;i++)
	{
		sprintf(command,"echo 1 | tee /sys/class/leds/led0/brightness");
		system(command);
		usleep(250000);
		sprintf(command,"echo 0 | tee /sys/class/leds/led0/brightness");
		system(command);
		usleep(250000);
	}

	//set the ACT led mode to emmc
	sprintf(command,"echo mmc0 | tee /sys/class/leds/led0/trigger");
	if(system(command)!=0)
	{
		//pPanelReq->result=RPC_SRV_RESULT_FILE_OPEN_ERR;
		//return 0;
		;
	}
	return RPC_SRV_RESULT_SUCCESS;//FEATURE_NOT_AVAILABLE;
}
/*****************************************************************************/

