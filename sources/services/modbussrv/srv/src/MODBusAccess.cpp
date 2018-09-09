#include "MODBusAccess.h"
using namespace std;
/*****************************************************************************/
MODBusAccess::MODBusAccess(std::string DevNode)
{
	node=DevNode;
	DevOpened=RPC_SRV_RESULT_FAIL;
	/*if(is_it_device_node(DevNode)==RPC_SRV_RESULT_SUCCESS)//check if it is a char-device-node
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
	}*/
	//cout<<"modbus-dev-node="<<node<<endl;
}
/*****************************************************************************/
MODBusAccess::~MODBusAccess()
{
	//if(DevOpened==RPC_SRV_RESULT_SUCCESS && ConnType==ADLIB_DEV_CONN_TYPE_DEVNODE)
	//	close(fd);
	//else if(DevOpened==RPC_SRV_RESULT_SUCCESS && ConnType==ADLIB_DEV_CONN_TYPE_NETWORK)
	//	Client.rpc_server_disconnect();
}
/*****************************************************************************/

