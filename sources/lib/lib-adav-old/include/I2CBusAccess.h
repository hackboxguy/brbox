#ifndef __I2C_BUS_ACCESS_H_
#define __I2C_BUS_ACCESS_H_
#include <iostream>
#include "ADCommon.hpp"
#include "ADJsonRpcClient.hpp"
#include <stdint.h>
using namespace std;
//#include "ADCmdlineHelper.hpp"
//#include "ADCmnStringProcessor.hpp"

#define I2C_SMBUS_READ	1
#define I2C_SMBUS_WRITE	0
#define I2C_SMBUS_QUICK		    0
#define I2C_SMBUS_BYTE		    1
#define I2C_SMBUS_BYTE_DATA	    2
#define I2C_SMBUS_WORD_DATA	    3
#define I2C_SMBUS_PROC_CALL	    4
#define I2C_SMBUS_BLOCK_DATA	    5
#define I2C_SMBUS_I2C_BLOCK_DATA    6
#define I2C_SMBUS_BLOCK_PROC_CALL   7		/* SMBus 2.0 */
#define I2C_SMBUS_BLOCK_DATA_PEC    8		/* SMBus 2.0 */
#define I2C_SMBUS_PROC_CALL_PEC     9		/* SMBus 2.0 */
#define I2C_SMBUS_BLOCK_PROC_CALL_PEC  10	/* SMBus 2.0 */
#define I2C_SMBUS_WORD_DATA_PEC	   11		/* SMBus 2.0 */

#define I2C_ACCESS_TYPE_SMBUS 1 //use this for smbus based access

#define I2C_READ(FD,BUFF,SIZE) 	read(FD,BUFF,SIZE)
#define I2C_WRITE(FD,BUFF,SIZE) write(FD,BUFF,SIZE)

/*#define I2C_SMBUS_READ(FD,BUFF,SIZE)\
do			\
{			\
	struct i2c_smbus_ioctl_data args;\
	union i2c_smbus_data smdata;\
	args.read_write = I2C_SMBUS_WRITE;\
	args.command = buf[0];\
	args.size = 2;\
	args.data = &smdata;\
	if (ioctl(devhandle, I2C_SMBUS, &args) < 0)\
	{\
		return(-1);\
	}\
} while (0)*/
#define I2C_SMBUS_BLOCK_MAX	32	/* As specified in SMBus standard */
#define I2C_SMBUS_I2C_BLOCK_MAX	32	/* Not specified but we use same structure */
union i2c_smbus_data
{
	uint8_t byte;
	uint16_t word;
	uint8_t block[I2C_SMBUS_BLOCK_MAX + 3]; /* block[0] is used for length */
                          /* one more for read length in block process call */
	                                            /* and one more for PEC */
};
class I2CBusAccess
{
	int fd;
	RPC_SRV_RESULT DevOpened;
	ADLIB_DEV_CONN_TYPE ConnType;
	std::string node;
	ADJsonRpcClient Client;
	std::string IpAddr;
	int Port;
	RPC_SRV_RESULT test_write_byte(char* dev,uint8_t addr, uint8_t data);
	bool is_number(const std::string& s);
	bool validateIpAddress(const string &ipAddress);
	RPC_SRV_RESULT is_it_network_node(std::string devnode,int &port,std::string &ip);
	int32_t i2c_smbus_access(int file, char read_write, uint8_t command,int size, union i2c_smbus_data *data);
	int32_t i2c_smbus_read_byte(int file);
	int32_t i2c_smbus_write_byte(int file,uint8_t value);

public:
	I2CBusAccess(std::string DevNode);
	~I2CBusAccess();
	RPC_SRV_RESULT SetSlaveAddr(uint8_t addr);
	RPC_SRV_RESULT read_byte(uint32_t addr, uint8_t *data);
	RPC_SRV_RESULT write_byte(uint32_t addr, uint8_t data);
	RPC_SRV_RESULT read_word(uint32_t addr, uint16_t *data);
	RPC_SRV_RESULT write_word(uint32_t addr, uint16_t data);
	RPC_SRV_RESULT read_dword(uint32_t addr, uint32_t *data);
	RPC_SRV_RESULT write_dword(uint32_t addr, uint32_t data);
	RPC_SRV_RESULT write_array(uint32_t addr, uint8_t *data,uint32_t len);
	RPC_SRV_RESULT read_array(uint32_t addr, uint8_t *data,uint32_t len);
	RPC_SRV_RESULT is_it_device_node(std::string devnode);
};
#endif
