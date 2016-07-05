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
#include <sstream>
#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include <stdlib.h> 
//#include "i2cbusses.h"
//#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "ADJsonRpcMgr.hpp"
using namespace std;
/*****************************************************************************/
I2CBusAccess::I2CBusAccess(std::string DevNode)
{
	node=DevNode;
	DevOpened=RPC_SRV_RESULT_FAIL;
	if(is_it_device_node(DevNode)==RPC_SRV_RESULT_SUCCESS)//check if it is a char-device-node
	{
		fd = open(DevNode.c_str(), O_RDWR);
		switch(errno)
		{
			case ENOENT :DevOpened=RPC_SRV_RESULT_FILE_NOT_FOUND;break;
			case ENOTDIR:DevOpened=RPC_SRV_RESULT_FILE_NOT_FOUND;break;
			case EACCES :DevOpened=RPC_SRV_RESULT_DEVNODE_ACCERR;break;
			default:DevOpened=RPC_SRV_RESULT_SUCCESS;ConnType=ADLIB_DEV_CONN_TYPE_DEVNODE;break;
		}
	}
	else if(is_it_network_node(DevNode,Port,IpAddr)==RPC_SRV_RESULT_SUCCESS)
	{
		if(Client.rpc_server_connect(IpAddr.c_str(),Port)!=0)
			DevOpened= RPC_SRV_RESULT_HOST_NOT_REACHABLE_ERR;
		else
		{
			DevOpened=RPC_SRV_RESULT_SUCCESS;
			ConnType=ADLIB_DEV_CONN_TYPE_NETWORK;
		}
		//read string type
		//RPC_SRV_RESULT res = Client.get_string_type(rpc_name,enum_arg_name,temp_str);
		//Client.rpc_server_disconnect();
	}
}
/*****************************************************************************/
I2CBusAccess::~I2CBusAccess()
{
	if(DevOpened==RPC_SRV_RESULT_SUCCESS && ConnType==ADLIB_DEV_CONN_TYPE_DEVNODE)
		close(fd);
	else if(DevOpened==RPC_SRV_RESULT_SUCCESS && ConnType==ADLIB_DEV_CONN_TYPE_NETWORK)
		Client.rpc_server_disconnect();
}
/*****************************************************************************/
bool I2CBusAccess::is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}
/*****************************************************************************/
bool I2CBusAccess::validateIpAddress(const string &ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
    return result != 0;
}
/*****************************************************************************/
//checks if a given device node is a pair of port/ipaddress
//accepted format is "40001:192.168.1.10" or "192.168.1.13:40003" or "40007"(if given string is just a port number, then ip is localhost)
RPC_SRV_RESULT I2CBusAccess::is_it_network_node(std::string devnode,int &port,std::string &ip)
{
	vector<string> strings;
	istringstream f(devnode);//"192.168.1.1,40001");
	string s,str1,str2;
	ip="none";
	port=-1;    

	while (getline(f, s, ':'))
	{
		//cout << s << endl;
		strings.push_back(s);
	}
	if(strings.size()<=0 || strings.size()>2)
	{
		//cout<<"invalid string"<<endl;
		return RPC_SRV_RESULT_FAIL;
	}
	if(strings.size()==2)
	{
		str1=strings[0];
		str2=strings[1];
	}
	else
	{
		str1=strings[0];//str1 is assumed to be port
		str2="127.0.0.1";//take default local ip
	}
	if(is_number(str1))
	{
		//cout<<"port="<<str1<<endl;
		port=atoi(str1.c_str());
	}
	else if(is_number(str2))
	{
		//cout<<"str2="<<str2<<endl;
		port=atoi(str2.c_str());
	}
	else
		return RPC_SRV_RESULT_FAIL;//port=-1;
	//ip:
	if(validateIpAddress(str1)==true)
		ip=str1;
	else if(validateIpAddress(str2)==true)
		ip=str2;
	else
		return RPC_SRV_RESULT_FAIL;//ip="none";
	//cout<<"port="<<port<<" ip="<<ip<<endl;
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
//checks if a given string is a character device node
RPC_SRV_RESULT I2CBusAccess::is_it_device_node(std::string devnode)
{
	struct stat buf;
	stat(devnode.c_str(), &buf);
	if(S_ISCHR(buf.st_mode))
		return RPC_SRV_RESULT_SUCCESS;
	return RPC_SRV_RESULT_FAIL;
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
	if(ConnType==ADLIB_DEV_CONN_TYPE_DEVNODE)
	{
		RPC_SRV_RESULT ret=SetSlaveAddr((uint8_t)addr);
		if(ret != RPC_SRV_RESULT_SUCCESS)
			return ret;

		//uint8_t buff[16];
		//if (I2C_READ(fd,buff,1) != 1)
		//	return RPC_SRV_RESULT_FILE_READ_ERR;//device node read error

		int32_t smret=i2c_smbus_read_byte(fd);
		if(smret<1)
			return RPC_SRV_RESULT_FILE_READ_ERR;//device node read error
		else
			*data=(uint8_t)smret;

		//*data=buff[0];
		return RPC_SRV_RESULT_SUCCESS;
	}
	else if(ConnType==ADLIB_DEV_CONN_TYPE_NETWORK)
	{
		char tmpbuf[64];
//pCmdObj->result=pSrvSockConn->get_integer_type_with_addr_para(pCmdObj->get_rpc_name,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_int_value,pCmdObj->second_arg_param_name,pCmdObj->first_arg_param_value);//&tmp_int);
		RPC_SRV_RESULT retval=Client.get_integer_type_with_addr_para((char*)RPCMGR_RPC_MW_BYTE_GET,(char*)RPCMGR_RPC_MW_ARGADDR,addr,(char*)RPCMGR_RPC_MW_ARGDATA,tmpbuf);
		if(retval==RPC_SRV_RESULT_SUCCESS)
			*data=(uint8_t)atoi(tmpbuf);
		return retval;	
	}
	else
		return RPC_SRV_RESULT_FAIL;
}
RPC_SRV_RESULT I2CBusAccess::write_byte(uint32_t addr, uint8_t data)
{
	//return test_write_byte((char*)node.c_str(),addr,data);//TODO
	//printf("I2CBusAccess::write_byte addr=%d data=%d\n",addr,data);
	if(DevOpened!=RPC_SRV_RESULT_SUCCESS)
		return DevOpened;//i2c-device-node is not open
	if(ConnType==ADLIB_DEV_CONN_TYPE_DEVNODE)
	{
		RPC_SRV_RESULT ret=SetSlaveAddr((uint8_t)addr);
		if(ret != RPC_SRV_RESULT_SUCCESS)
			return ret;
		//uint8_t buff[16];
		//buff[0]=data;
		//if (I2C_WRITE(fd,buff,1) != 1)
		//	return RPC_SRV_RESULT_FILE_WRITE_ERR;//devantech usb-i2c driver returns 0bytes, raspi returns 1byte.
		int32_t smret=i2c_smbus_write_byte(fd,data);
		if(smret<0) //TODO: write bytes returns 0, although 1byte is written correctly, this is observed with devantech i2c driver.
			return RPC_SRV_RESULT_FILE_WRITE_ERR;

		return RPC_SRV_RESULT_SUCCESS;
	}
	else if(ConnType==ADLIB_DEV_CONN_TYPE_NETWORK)
	{
return Client.set_integer_type_with_addr_para((char*)RPCMGR_RPC_MW_BYTE_SET,(char*)RPCMGR_RPC_MW_ARGADDR,addr,(char*)RPCMGR_RPC_MW_ARGDATA,data);
//push_int_get_set_with_dev_addr_arg_command(EJSON_RPCMGR_GET_MW_BYTE,EJSON_RPCMGR_SET_MW_BYTE,
//			        RPCMGR_RPC_MW_BYTE_GET,RPCMGR_RPC_MW_BYTE_SET,
//				(char*)RPCMGR_RPC_MW_ARGDATA,
//				(char*)RPCMGR_RPC_MW_ARGADDR,-1,subarg);
	}
	else
		return RPC_SRV_RESULT_FAIL;
}
/*****************************************************************************/
RPC_SRV_RESULT I2CBusAccess::read_word(uint32_t addr, uint16_t *data)
{
	if(DevOpened!=RPC_SRV_RESULT_SUCCESS)
		return DevOpened;//i2c-device-node is not open
	if(ConnType==ADLIB_DEV_CONN_TYPE_DEVNODE)
	{
		RPC_SRV_RESULT ret=SetSlaveAddr((uint8_t)addr);
		if(ret != RPC_SRV_RESULT_SUCCESS)
			return ret;
		uint8_t buff[16];
		if (I2C_READ(fd,buff,2) != 2)
			return RPC_SRV_RESULT_FILE_READ_ERR;//device node read error
		*data=(uint16_t)(buff[0]<<8)|(uint16_t)buff[1];
		return RPC_SRV_RESULT_SUCCESS;
	}
	else if(ConnType==ADLIB_DEV_CONN_TYPE_NETWORK)
	{
		char tmpbuf[64];
		RPC_SRV_RESULT retval=Client.get_integer_type_with_addr_para((char*)RPCMGR_RPC_MW_WORD_GET,(char*)RPCMGR_RPC_MW_ARGADDR,addr,(char*)RPCMGR_RPC_MW_ARGDATA,tmpbuf);
		if(retval==RPC_SRV_RESULT_SUCCESS)
			*data=(uint16_t)atoi(tmpbuf);
		return retval;	
	}
	else
		return RPC_SRV_RESULT_FAIL;

}
RPC_SRV_RESULT I2CBusAccess::write_word(uint32_t addr, uint16_t data)
{
	//printf("I2CBusAccess::write_byte addr=%d data=%d\n",addr,data);
	if(DevOpened!=RPC_SRV_RESULT_SUCCESS)
		return DevOpened;//i2c-device-node is not open
	if(ConnType==ADLIB_DEV_CONN_TYPE_DEVNODE)
	{
		RPC_SRV_RESULT ret=SetSlaveAddr((uint8_t)addr);
		if(ret != RPC_SRV_RESULT_SUCCESS)
			return ret;
		uint8_t buff[16];
		buff[0]=(uint8_t)(data>>8);
		buff[1]=data&0x00FF;
		if (I2C_WRITE(fd,buff,2) != 2)
			return RPC_SRV_RESULT_FILE_WRITE_ERR;//device node write error
		return RPC_SRV_RESULT_SUCCESS;
	}
	else if(ConnType==ADLIB_DEV_CONN_TYPE_NETWORK)
	{
		return Client.set_integer_type_with_addr_para((char*)RPCMGR_RPC_MW_WORD_SET,(char*)RPCMGR_RPC_MW_ARGADDR,addr,(char*)RPCMGR_RPC_MW_ARGDATA,data);
	}
	else
		return RPC_SRV_RESULT_FAIL;
}
/*****************************************************************************/
RPC_SRV_RESULT I2CBusAccess::read_dword(uint32_t addr, uint32_t *data)
{
	if(DevOpened!=RPC_SRV_RESULT_SUCCESS)
		return DevOpened;//i2c-device-node is not open
	if(ConnType==ADLIB_DEV_CONN_TYPE_DEVNODE)
	{
		RPC_SRV_RESULT ret=SetSlaveAddr((uint8_t)addr);
		if(ret != RPC_SRV_RESULT_SUCCESS)
			return ret;
		uint8_t buff[16];
		if (I2C_READ(fd,buff,4) != 4)
			return RPC_SRV_RESULT_FILE_READ_ERR;//device node read error
		*data=(uint32_t)(buff[0]<<24)|(uint32_t)(buff[1]<<16)|(uint32_t)(buff[2]<<8)|(uint32_t)buff[3];
		return RPC_SRV_RESULT_SUCCESS;
	}
	else if(ConnType==ADLIB_DEV_CONN_TYPE_NETWORK)
	{
		char tmpbuf[64];
		RPC_SRV_RESULT retval=Client.get_integer_type_with_addr_para((char*)RPCMGR_RPC_MW_DWORD_GET,(char*)RPCMGR_RPC_MW_ARGADDR,addr,(char*)RPCMGR_RPC_MW_ARGDATA,tmpbuf);
		if(retval==RPC_SRV_RESULT_SUCCESS)
			*data=(uint32_t)atoi(tmpbuf);//TODO: check if atol is needed
		return retval;	
	}
	else
		return RPC_SRV_RESULT_FAIL;
}
RPC_SRV_RESULT I2CBusAccess::write_dword(uint32_t addr, uint32_t data)
{
	//printf("I2CBusAccess::write_byte addr=%d data=%d\n",addr,data);
	if(DevOpened!=RPC_SRV_RESULT_SUCCESS)
		return DevOpened;//i2c-device-node is not open
	if(ConnType==ADLIB_DEV_CONN_TYPE_DEVNODE)
	{
		RPC_SRV_RESULT ret=SetSlaveAddr((uint8_t)addr);
		if(ret != RPC_SRV_RESULT_SUCCESS)
			return ret;
		uint8_t buff[16];
		buff[0]=(uint8_t)(data>>24);
		buff[1]=(uint8_t)(data>>16);
		buff[2]=(uint8_t)(data>>8);
		buff[3]=data&0x000000FF;
		if (I2C_WRITE(fd,buff,4) != 4)
			return RPC_SRV_RESULT_FILE_WRITE_ERR;//device node write error
		return RPC_SRV_RESULT_SUCCESS;
	}
	else if(ConnType==ADLIB_DEV_CONN_TYPE_NETWORK)
	{
		return Client.set_integer_type_with_addr_para((char*)RPCMGR_RPC_MW_DWORD_SET,(char*)RPCMGR_RPC_MW_ARGADDR,addr,(char*)RPCMGR_RPC_MW_ARGDATA,data);
	}
	else
		return RPC_SRV_RESULT_FAIL;
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
	if (I2C_WRITE(fd,data,len) != len)
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
		//printf("I2CBusAccess::test_write_byte:Error opening file: %s\n", strerror(errno));
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
		//printf("I2CBusAccess::test_write_byte:ioctl error: %s\n", strerror(errno));
		return RPC_SRV_RESULT_FILE_WRITE_ERR;
	}
	uint8_t buff[16];buff[0]=data;
	//int val=atoi(argv[1]);
	unsigned int tst=data;
	int sz=I2C_WRITE(myfd, &tst, 1);// != 1) 
	if(sz!=1)
	{
		//printf("I2CBusAccess::test_write_byte:Error writing file:written %d bytes, errorno:%s\n",sz,strerror(errno));
		return RPC_SRV_RESULT_FILE_WRITE_ERR;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
int32_t I2CBusAccess::i2c_smbus_access(int file, char read_write, uint8_t command,int size, union i2c_smbus_data *data)
{
	struct i2c_smbus_ioctl_data args;
	args.read_write = read_write;
	args.command = command;
	args.size = size;
	args.data = data;
	return ioctl(file,I2C_SMBUS,&args);
}
int32_t I2CBusAccess::i2c_smbus_read_byte(int file)
{
#ifdef I2C_ACCESS_TYPE_SMBUS
	union i2c_smbus_data data;
	if (i2c_smbus_access(file,I2C_SMBUS_READ,0,I2C_SMBUS_BYTE,&data))
		return -1;
	else
		return 0x0FF & data.byte;
	return 0;
#else
	uint8_t buff[16];
	return read(file,buff,1);
#endif
}
int32_t I2CBusAccess::i2c_smbus_write_byte(int file,uint8_t value)
{
#ifdef I2C_ACCESS_TYPE_SMBUS
	size_t sz=i2c_smbus_access(file,I2C_SMBUS_WRITE,value,
	                        I2C_SMBUS_BYTE,NULL);
	//printf("I2CBusAccess::i2c_smbus_write_byte:error: %s\n", strerror(errno));
	return sz;
#else
	uint8_t buff[16];buff[0]=value;
	size_t sz=write(file,buff,1);
	//printf("I2CBusAccess::i2c_smbus_write_byte:error: %s\n", strerror(errno));
	return sz;
#endif
}
/*****************************************************************************/



