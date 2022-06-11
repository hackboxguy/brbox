#include "DevIdentGlMt300nv2.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;
/* ------------------------------------------------------------------------- */
DevIdentGlMt300nv2::DevIdentGlMt300nv2()
{
	//on startup Keep the power and wan LED off
	//char command[255];
	//sprintf(command,"echo 0 > /sys/class/leds/gl-mt300n-v2\\:green\\:power/brightness");
	//system(command);
	//sprintf(command,"echo 0 > /sys/class/leds/gl-mt300n-v2\\:green\\:wan/brightness");
	//system(command);
}
/* ------------------------------------------------------------------------- */
DevIdentGlMt300nv2::~DevIdentGlMt300nv2()
{
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT DevIdentGlMt300nv2::device_identify()
{
	char command[255];
	//blink left-green led for 10 seconds on gl-mt300n-v2

	//preserve last state
	sprintf(command,"cat /sys/class/leds/green:power/brightness > /tmp/last-power-led-state.txt");
	system(command);

	for(int i=0;i<10;i++)
	{
		//sprintf(command,"echo 1 > /sys/class/leds/gl-mt300n-v2\\:green\\:power/brightness");
		sprintf(command,"echo 1 > /sys/class/leds/green:power/brightness");
		system(command);
		usleep(250000);
		//sprintf(command,"echo 0 > /sys/class/leds/gl-mt300n-v2\\:green\\:power/brightness");
		sprintf(command,"echo 0 > /sys/class/leds/green:power/brightness");
		system(command);
		usleep(250000);
	}

	//revert back last state
	sprintf(command,"echo $(cat /tmp/last-power-led-state.txt) > /sys/class/leds/green:power/brightness");
	system(command);

	if(system(command)!=0)
	{
		;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
