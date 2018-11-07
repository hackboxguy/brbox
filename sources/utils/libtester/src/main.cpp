/* ---------------------------------------------------------------------------- 
how to test this utility?
1)cd ~/sandbox/OE-FMW-FOO/Mercury/trunk/target (do "svn up" if needed)
2)cmake -H. -BOutput -DCMAKE_INSTALL_PREFIX=~/tmp/fmw
3)cmake --build Output -- install -j5
4)cd ~/tmp/fmw
5)export LD_LIBRARY_PATH=./lib
6)./bin/cmnlib
---------------------------------------------------------------------------- */
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define I2C_DEVICE_NODE "/dev/i2c-0"
#include "ADCommon.hpp"
/* ------------------------------------------------------------------------- */
//include test class
#include "RTCDevice.hpp"
#include "I2CDS3231.hpp"
/* ------------------------------------------------------------------------- */
using namespace std;
int main(int argc, const char* argv[])
{
	std::string DevNode=I2C_DEVICE_NODE;
	std::string Type="none";
	
	RTCDevice* pDevice=NULL;//interface-class
	pDevice = new I2CDS3231(DevNode,Type);//actual implementation behind interface-class

	if(pDevice!=NULL)
	{
		struct tm mytm;
		const char *table[]=STR_RPC_SRV_RESULT_STRING_TABLE;

		RPC_SRV_RESULT res=pDevice->get_date_time(mytm);
		cout<<"pDevice->get_date_time:returned:"<<table[res]<<endl;

		res=pDevice->set_date_time(mytm);
		cout<<"pDevice->set_date_time:returned:"<<table[res]<<endl;
	}	

	if(pDevice!=NULL)
		delete pDevice;
	return 0;
}
