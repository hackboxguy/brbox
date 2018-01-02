#include "DevIdentA5V11.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;
/* ------------------------------------------------------------------------- */
DevIdentA5V11::DevIdentA5V11()
{
}
/* ------------------------------------------------------------------------- */
DevIdentA5V11::~DevIdentA5V11()
{
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT DevIdentA5V11::device_identify()
{
	char command[255];
	//blink red led on a5-v11 pocket router
	for(int i=0;i<10;i++)
	{
		sprintf(command,"echo 1 > /sys/class/leds/a5-v11\\:red\\:power/brightness");
		system(command);
		usleep(250000);
		sprintf(command,"echo 0 > /sys/class/leds/a5-v11\\:red\\:power/brightness");
		system(command);
		usleep(250000);
	}

	//switch the red led to ON
	sprintf(command,"echo 1 > /sys/class/leds/a5-v11\\:red\\:power/brightness");
	if(system(command)!=0)
	{
		//pPanelReq->result=RPC_SRV_RESULT_FILE_OPEN_ERR;
		//return 0;
		;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */

