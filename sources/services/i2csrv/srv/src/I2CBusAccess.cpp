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
	DevOpened=RPC_SRV_RESULT_FAIL;
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
		case ENOENT :DevOpened=RPC_SRV_RESULT_DEVNODE_OPENERR;break;//cout<<"I2CBusAccess: Device Open Error = ENOENT"<<endl;break;
		case ENOTDIR:DevOpened=RPC_SRV_RESULT_DEVNODE_OPENERR;break;//cout<<"I2CBusAccess: Device Open Error = ENOTDIR"<<endl;break;
		case EACCES :DevOpened=RPC_SRV_RESULT_DEVNODE_ACCERR;break;//cout<<"I2CBusAccess: Device Open Error = EACCES"<<endl;break;
		default:DevOpened=RPC_SRV_RESULT_SUCCESS;break;
	}

	//if (fd < 0)
	//{
		///* ERROR HANDLING: you can check errno to see what went wrong */
		//perror("Failed to open the i2c bus");
		//exit(1);
	//	cout<<"faild to open device "<<DevNode.c_str()<<endl;
	//}

}
/*****************************************************************************/
I2CBusAccess::~I2CBusAccess()
{
	if(DevOpened)
		close(fd);
}
/*****************************************************************************/
RPC_SRV_RESULT I2CBusAccess::SetSlaveAddr(int filedescr, uint8_t addr)
{
	if (ioctl(filedescr, I2C_SLAVE, addr) < 0) 
	{
		//cout<<"I2CBusAccess::SetSlaveAddr:Unable to Set Slave Addr to "<<addr<<endl;
		return RPC_SRV_RESULT_BUS_ERROR;//RPC_SRV_RESULT_FAIL;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT I2CBusAccess::read_byte(uint32_t addr, uint8_t *data)
{
	if(DevOpened!=RPC_SRV_RESULT_SUCCESS)
		return DevOpened;//i2c-device-node is not open
	RPC_SRV_RESULT ret=SetSlaveAddr(fd, (uint8_t)addr);
	if(ret != RPC_SRV_RESULT_SUCCESS)
		return ret;
	uint8_t buff[16];
	if (read(fd,buff,1) != 1)
		return RPC_SRV_RESULT_FILE_READ_ERR;//device node read error
	*data=buff[0];
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT I2CBusAccess::write_byte(uint32_t addr, uint8_t data)
{
	//printf("I2CBusAccess::write_byte addr=%d data=%d\n",addr,data);
	if(DevOpened!=RPC_SRV_RESULT_SUCCESS)
		return DevOpened;//i2c-device-node is not open
	RPC_SRV_RESULT ret=SetSlaveAddr(fd, (uint8_t)addr);
	if(ret != RPC_SRV_RESULT_SUCCESS)
		return ret;
	uint8_t buff[16];
	buff[0]=data;
	if (write(fd,buff,1) != 1)
		return RPC_SRV_RESULT_FILE_WRITE_ERR;//device node write error
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT I2CBusAccess::read_word(uint32_t addr, uint16_t *data)
{
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT I2CBusAccess::write_word(uint32_t addr, uint16_t data)
{
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT I2CBusAccess::read_dword(uint32_t addr, uint32_t *data)
{
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT I2CBusAccess::write_dword(uint32_t addr, uint32_t data)
{
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/


