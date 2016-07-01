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
//#include <linux/i2c.h>
#include <linux/i2c-dev.h>
using namespace std;
/*****************************************************************************/
I2CBusAccess::I2CBusAccess(std::string DevNode)
{
	node=DevNode;
/*	DevOpened=RPC_SRV_RESULT_FAIL;
	fd = open(DevNode.c_str(), O_RDWR);
	switch(errno)
	{
		case ENOENT :DevOpened=RPC_SRV_RESULT_DEVNODE_OPENERR;break;//cout<<"I2CBusAccess: Device Open Error = ENOENT"<<endl;break;
		case ENOTDIR:DevOpened=RPC_SRV_RESULT_DEVNODE_OPENERR;break;//cout<<"I2CBusAccess: Device Open Error = ENOTDIR"<<endl;break;
		case EACCES :DevOpened=RPC_SRV_RESULT_DEVNODE_ACCERR;break;//cout<<"I2CBusAccess: Device Open Error = EACCES"<<endl;break;
		default:DevOpened=RPC_SRV_RESULT_SUCCESS;break;
	}
*/
}
/*****************************************************************************/
I2CBusAccess::~I2CBusAccess()
{
//	if(DevOpened)
//		close(fd);
}
/*****************************************************************************/
RPC_SRV_RESULT I2CBusAccess::SetSlaveAddr(uint8_t addr)
{
	if (ioctl(fd, I2C_SLAVE, addr) < 0) 
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
	RPC_SRV_RESULT ret=SetSlaveAddr((uint8_t)addr);
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
	return test_write_byte((char*)node.c_str(),addr,data);//TODO

	//printf("I2CBusAccess::write_byte addr=%d data=%d\n",addr,data);
	if(DevOpened!=RPC_SRV_RESULT_SUCCESS)
		return DevOpened;//i2c-device-node is not open
	RPC_SRV_RESULT ret=SetSlaveAddr((uint8_t)addr);
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
	if(DevOpened!=RPC_SRV_RESULT_SUCCESS)
		return DevOpened;//i2c-device-node is not open
	RPC_SRV_RESULT ret=SetSlaveAddr((uint8_t)addr);
	if(ret != RPC_SRV_RESULT_SUCCESS)
		return ret;
	uint8_t buff[16];
	if (read(fd,buff,2) != 2)
		return RPC_SRV_RESULT_FILE_READ_ERR;//device node read error
	*data=(uint16_t)(buff[0]<<8)|(uint16_t)buff[1];
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT I2CBusAccess::write_word(uint32_t addr, uint16_t data)
{
	//printf("I2CBusAccess::write_byte addr=%d data=%d\n",addr,data);
	if(DevOpened!=RPC_SRV_RESULT_SUCCESS)
		return DevOpened;//i2c-device-node is not open
	RPC_SRV_RESULT ret=SetSlaveAddr((uint8_t)addr);
	if(ret != RPC_SRV_RESULT_SUCCESS)
		return ret;
	uint8_t buff[16];
	buff[0]=(uint8_t)(data>>8);
	buff[1]=data&0x00FF;
	if (write(fd,buff,2) != 2)
		return RPC_SRV_RESULT_FILE_WRITE_ERR;//device node write error
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT I2CBusAccess::read_dword(uint32_t addr, uint32_t *data)
{
	if(DevOpened!=RPC_SRV_RESULT_SUCCESS)
		return DevOpened;//i2c-device-node is not open
	RPC_SRV_RESULT ret=SetSlaveAddr((uint8_t)addr);
	if(ret != RPC_SRV_RESULT_SUCCESS)
		return ret;
	uint8_t buff[16];
	if (read(fd,buff,4) != 4)
		return RPC_SRV_RESULT_FILE_READ_ERR;//device node read error
	*data=(uint32_t)(buff[0]<<24)|(uint32_t)(buff[1]<<16)|(uint32_t)(buff[2]<<8)|(uint32_t)buff[3];
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT I2CBusAccess::write_dword(uint32_t addr, uint32_t data)
{
	//printf("I2CBusAccess::write_byte addr=%d data=%d\n",addr,data);
	if(DevOpened!=RPC_SRV_RESULT_SUCCESS)
		return DevOpened;//i2c-device-node is not open
	RPC_SRV_RESULT ret=SetSlaveAddr((uint8_t)addr);
	if(ret != RPC_SRV_RESULT_SUCCESS)
		return ret;
	uint8_t buff[16];
	buff[0]=(uint8_t)(data>>24);
	buff[1]=(uint8_t)(data>>16);
	buff[2]=(uint8_t)(data>>8);
	buff[3]=data&0x000000FF;
	if (write(fd,buff,4) != 4)
		return RPC_SRV_RESULT_FILE_WRITE_ERR;//device node write error
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT I2CBusAccess::write_array(uint32_t addr, uint8_t *data,uint32_t len)
{
	if(DevOpened!=RPC_SRV_RESULT_SUCCESS)
		return DevOpened;//i2c-device-node is not open
	RPC_SRV_RESULT ret=SetSlaveAddr((uint8_t)addr);
	if(ret != RPC_SRV_RESULT_SUCCESS)
		return ret;
	//uint8_t buff[16];
	//buff[0]=data;
	if (write(fd,data,len) != len)
		return RPC_SRV_RESULT_FILE_WRITE_ERR;//device node write error
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT I2CBusAccess::test_write_byte(char* dev,uint8_t addr, uint8_t data)
{
	int myfd;
	myfd = open(dev, O_RDWR);
	if (myfd < 0) 
	{
		printf("I2CBusAccess::test_write_byte:Error opening file: %s\n", strerror(errno));
		//return RPC_SRV_RESULT_FILE_OPEN_ERR;
		switch(errno)
		{
			case ENOENT :return RPC_SRV_RESULT_FILE_NOT_FOUND;//cout<<"I2CBusAccess: Device Open Error = ENOENT"<<endl;break;
			case ENOTDIR:return RPC_SRV_RESULT_FAIL;//file is not a directory
			case EACCES :return RPC_SRV_RESULT_DEVNODE_ACCERR;//DevOpened=RPC_SRV_RESULT_DEVNODE_ACCERR;break;//cout<<"I2CBusAccess: Device Open Error = EACCES"<<endl;break;
			default:break;
		}
	}
	//fcntl(myfd, F_SETFL,fcntl(myfd, F_GETFL) | O_NONBLOCK);
	if (ioctl(myfd, I2C_SLAVE, addr) < 0) 
	{
		printf("I2CBusAccess::test_write_byte:ioctl error: %s\n", strerror(errno));
		return RPC_SRV_RESULT_FILE_WRITE_ERR;
	}
	uint8_t buff[16];buff[0]=data;
	//int val=atoi(argv[1]);
	unsigned int tst=data;
	int sz=write(myfd, &tst, 1);// != 1) 
	if(sz!=1)
	{
		printf("I2CBusAccess::test_write_byte:Error writing file:written %d bytes, errorno:%s\n",sz,strerror(errno));
		return RPC_SRV_RESULT_FILE_WRITE_ERR;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/


