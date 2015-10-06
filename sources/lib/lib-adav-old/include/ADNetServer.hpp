#ifndef __ADNETSERVER_H_
#define __ADNETSERVER_H_
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <semaphore.h>
#include <netdb.h>
#include <arpa/inet.h>


#include "ADGenericChain.hpp"
#include "ADThread.hpp"
#include "ADCommon.hpp"
/*****************************************************************************/
#define AD_NET_SERVER_DEFAULT_LISTEN_PORT 65001
#define AD_NET_SERVER_MAX_BUFFER_SIZE 0xffff //65k buffer
/*****************************************************************************/
typedef enum ADLIB_TCP_SOCKET_TYPE_T
{
	ADLIB_TCP_SOCKET_TYPE_BINARY,
	ADLIB_TCP_SOCKET_TYPE_JSON,
	ADLIB_TCP_SOCKET_TYPE_UNKNOWN,
	ADLIB_TCP_SOCKET_TYPE_NONE
}ADLIB_TCP_SOCKET_TYPE;
/*****************************************************************************/

class ADNetProducer; //subject
class ADNetConsumer //observer
{
public:
	virtual int on_data_arrival(ADGenericChain* pRxChain,ADNetProducer* pObj)=0;
	virtual ~ADNetConsumer(){};
};
/*****************************************************************************/
class ADNetProducer
{
	static int IDGenerator;
	ADNetConsumer *pConsumer;//consumer object where notification goes to
	int id;//id number for consumer to distinguish between many producers

protected:
	int notify_data_arrival(ADGenericChain* pRxChain)
	{
		if(pConsumer!=NULL)		
			return pConsumer->on_data_arrival(pRxChain,this);
		return -1;
	}
	int is_helper_attached(void)
	{
		if(pConsumer==NULL)		
			return -1;
		return 0;
	}
public:
	ADNetProducer() {/*IDGenerator++;*/id=IDGenerator++;pConsumer=NULL;}
	virtual ~ADNetProducer(){};
	//int subscribe_data_arrival(ADNetConsumer* c)//,int id_num)
	int attach_on_data_arrival(ADNetConsumer* c)
	{
		//allow only one Consumer to be attached		
		if(pConsumer==NULL)
		{
			pConsumer=c;
			return id;
		}
		else
			return -1;//some other Consumer has already been attached
	}
	int getID(){return id;}

};
/*****************************************************************************/
//this is the object used to keep req/resp data in chain
struct net_data_obj
{
	int ident;//reserved for future use
	int sock_descriptor;//data belongs to this tcp connection
	int port;//port number of the client
	char ip[512];//ip address of the client
	int cltid;//corresponding identifier in the client-info chain
	int data_buffer_len;//data_len
	char* data_buffer;//data
public:
	net_data_obj(){data_buffer=NULL;};//cout<<"ADJsonRpcCommand::Constructor"<<endl;};
	~net_data_obj(){};//OBJ_MEM_DELETE(data_buffer);};//cout<<"ADJsonRpcCommand::Destructor"<<endl;};
};//net_data_obj;
/*****************************************************************************/
class ADNetServer: public ADNetProducer, public ADChainConsumer, public ADThreadConsumer
{
	ADLIB_TCP_SOCKET_TYPE sock_type;
	int socketlog;
	int connected;
	unsigned char end_server;
	int listen_port;//port to listen
	int listen_sd;//listen socket descriptor
	int max_sd;
	struct sockaddr_in addr;
	struct timeval timeout;
	fd_set master_set;
	char receive_buffer[AD_NET_SERVER_MAX_BUFFER_SIZE];//0xffff];
	int  receive_size;

	char receive_buffer_pending[AD_NET_SERVER_MAX_BUFFER_SIZE];//incase if json object is segmented
	int receive_size_pending;


	//id_response_chain
	int id_listen_thread;
	int id_response_thread;
	ADThread listen_thread;//data listening thread
	ADThread response_thread;//data sending thread

	ADGenericChain request_chain;//incoming data from listener port
	ADGenericChain response_chain;//outgoing_data to clients
	ADGenericChain clientInfo_chain;//ip and port number of client connection

	//ADGenericChain callbacks
	virtual int identify_chain_element(void* element,int ident,ADChainProducer* pObj);
	virtual int double_identify_chain_element(void* element,int ident1,int ident2,ADChainProducer* pObj);
	virtual int free_chain_element_data(void* element,ADChainProducer* pObj);

	//ADThread callbacks
	virtual int monoshot_callback_function(void* pUserData,ADThreadProducer* pObj);
	virtual int thread_callback_function(void* pUserData,ADThreadProducer* pObj);

	int initialize_helpers(void);
	int print_client_info(struct sockaddr *in_addr,socklen_t in_len,int sock_descr);
	int register_client_info(int sock_descr,int port,char* ip);
	int deregister_client_info(int sock_descr);
	int get_client_info(int sock_descr,char* cltip,int *cltport,int *cltid);

	int binary_receive_data_and_notify_consumer(int socket_descriptor,char *buf, int len);
	int json_receive_data_and_notify_consumer(int socket_descriptor,char *buf, int len);
	int segmented_json_object(int socket_descriptor,char* buf,int *len);

	int start_listening();
	int stop_listening();
	bool IsConnectionAlive(int sock_descriptor);

public:
	ADNetServer();
	ADNetServer(int port);
	~ADNetServer();	
	int schedule_response(int socket_descriptor,char *buf, int len);
	int start_listening(int port,int socket_log,ADLIB_TCP_SOCKET_TYPE socket_type=ADLIB_TCP_SOCKET_TYPE_JSON);

};

#endif
