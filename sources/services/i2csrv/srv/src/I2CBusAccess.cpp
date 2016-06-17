#include "I2CBusAccess.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>	/* for NAME_MAX */
#include <sys/ioctl.h>
#include <string.h>
#include <strings.h>	/* for strcasecmp() */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
//#include "i2cbusses.h"
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
using namespace std;
/*****************************************************************************/
I2CBusAccess::I2CBusAccess(std::string DevNode)
{
	DevOpened=false;
	fd = open(DevNode.c_str(), O_RDWR);
	/*if (fd < 0 && (errno == ENOENT || errno == ENOTDIR)) 
	{
		sprintf(filename, "/dev/i2c-%d", i2cbus);
		file = open(filename, O_RDWR);
	}
	if (file < 0 && !quiet)
	{
		if (errno == ENOENT) 
		{
			fprintf(stderr, "Error: Could not open file "
			"`/dev/i2c-%d' or `/dev/i2c/%d': %s\n",
			i2cbus, i2cbus, strerror(ENOENT));
		}
		else
		{
			fprintf(stderr, "Error: Could not open file ""`%s': %s\n", filename, strerror(errno));
			if (errno == EACCES)
				fprintf(stderr, "Run as root?\n");
		}
	}*/
	switch(errno)
	{
		case ENOENT :cout<<"I2CBusAccess: Device Open Error = ENOENT"<<endl;break;
		case ENOTDIR:cout<<"I2CBusAccess: Device Open Error = ENOTDIR"<<endl;break;
		case EACCES :cout<<"I2CBusAccess: Device Open Error = EACCES"<<endl;break;
		default:DevOpened=true;break;
	}
}
/*****************************************************************************/
I2CBusAccess::~I2CBusAccess()
{
	if(DevOpened)
		close(fd);
}
/*****************************************************************************/
RPC_SRV_RESULT I2CBusAccess::SetSlaveAddr(int fd, uint8_t addr)
{
	//if (ioctl(fd, force ? I2C_SLAVE_FORCE : I2C_SLAVE, address) < 0) 
	if (ioctl(fd, I2C_SLAVE, addr) < 0) 
	{
		//fprintf(stderr,"Error: Could not set address to 0x%02x: %s\n",address, strerror(errno));
		cout<<"I2CBusAccess::SetSlaveAddr:Unable to Set Slave Addr to "<<addr<<endl;
		return RPC_SRV_RESULT_FAIL;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/

