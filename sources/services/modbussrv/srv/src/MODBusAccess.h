#ifndef __MOD_BUS_ACCESS_H_
#define __MOD_BUS_ACCESS_H_
#include <iostream>
#include "ADCommon.hpp"
//#include "ADJsonRpcClient.hpp"
#include <stdint.h>
using namespace std;

class MODBusAccess
{
	int fd;
	std::string node;
	RPC_SRV_RESULT DevOpened;
	/*ADLIB_DEV_CONN_TYPE ConnType;
	ADJsonRpcClient Client;
	std::string IpAddr;
	int Port;
	RPC_SRV_RESULT test_write_byte(char* dev,uint8_t addr, uint8_t data);
	bool is_number(const std::string& s);
	bool validateIpAddress(const string &ipAddress);
	RPC_SRV_RESULT is_it_network_node(std::string devnode,int &port,std::string &ip);
	RPC_SRV_RESULT is_it_device_node(std::string devnode);
	int32_t i2c_smbus_access(int file, char read_write, uint8_t command,int size, union i2c_smbus_data *data);
	int32_t i2c_smbus_read_byte(int file);
	int32_t i2c_smbus_write_byte(int file,uint8_t value);*/

public:
	MODBusAccess(std::string DevNode);
	~MODBusAccess();
	/*RPC_SRV_RESULT SetSlaveAddr(uint8_t addr);
	RPC_SRV_RESULT read_byte(uint32_t addr, uint8_t *data);
	RPC_SRV_RESULT write_byte(uint32_t addr, uint8_t data);
	RPC_SRV_RESULT read_word(uint32_t addr, uint16_t *data);
	RPC_SRV_RESULT write_word(uint32_t addr, uint16_t data);
	RPC_SRV_RESULT read_dword(uint32_t addr, uint32_t *data);
	RPC_SRV_RESULT write_dword(uint32_t addr, uint32_t data);
	RPC_SRV_RESULT write_array(uint32_t addr, uint8_t *data,uint32_t len);
	RPC_SRV_RESULT read_array(uint32_t addr, uint8_t *data,uint32_t len);*/
};
#endif
