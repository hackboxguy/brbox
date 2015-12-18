#include "ADNetClient.hpp"
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h> 
#include <errno.h>
#include <unistd.h>
/*****************************************************************************/
ADNetClient::ADNetClient():ip(""),port(-1),connected(false)
{
}
ADNetClient::ADNetClient(string ip_addr,int prt):ip(""),port(-1),connected(false)
{
	ip=ip_addr;
	port=prt;
	sock_connect();//user has passed the ip and port, hence make a connection to server
	//connected=true;
	//initialized=1;
	//connected=0;
	//port=-1;
	//ip="";//strcpy(ip,"");	
}
/*****************************************************************************/
ADNetClient::~ADNetClient()
{
	sock_disconnect();
}
/*****************************************************************************/
int ADNetClient::test_print(void)
{
	 cout << "This is ADNetClient ip="<<ip<<" port="<<port<<" connected="<<connected << endl;
	return 0;
}
string ADNetClient::get_ip_addr()
{
	return ip;
}
/*****************************************************************************/
int ADNetClient::sock_connect(string ip_addr,int port_num)
{
	if(connected==true)
		return -1;//already connected
	if(port_num<1)
		return -1;//invalid port requested
	//if(strcmp(ip_addr,"")==0)//null ip
	if(ip_addr == "")//null ip
		return -1;
	port=port_num;
	ip=ip_addr;
	return sock_connect();
	//return 0;
}
/*****************************************************************************/
int ADNetClient::sock_connect(void)
{
	int rc;
	struct sockaddr_in   addr;

	if(connected==true)
		return -1;//already connected

	if(port<1)return -1;
	if(ip=="")return -1;

	/*************************************************/
	/* Create an AF_INET stream socket               */
	/*************************************************/
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		cout<<ip<<" : unable to open socket "<<endl;      	
		return -1;
	}

	/*************************************************/
	/* Initialize the socket address structure       */
	/*************************************************/
	memset(&addr, 0, sizeof(addr));
	addr.sin_family      = AF_INET;//TODO: check if user has passed ipv6 address an set this value as AF_INET6.
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	addr.sin_port        = htons(port);

	/*************************************************/
	/* Connect to the server                         */
	/*************************************************/
	rc = connect(sockfd,(struct sockaddr *)&addr,sizeof(struct sockaddr_in));
	if (rc < 0)
	{
		cout<<ip<<" : unable to connect to socket port = "<<port<<endl;
		close(sockfd);
		return -1;
	}
	//printf("Connection completed on port=%d ip=%s\n",port,ip);
	//client->port=port;
	//strcpy(client->ip,ip);
	connected=true;
	return 0;
}
/*****************************************************************************/
int ADNetClient::sock_disconnect(void)
{
	if(connected)
		close(sockfd);
	connected=false;
	//port=-1;
	//strcpy(client->ip,"");	
	//return 0;
	return 0;
}
/*****************************************************************************/
int ADNetClient::send_data(char* send_buf)
{
	unsigned int len;
	//record time A here
	CmdTimer.reset();
	len = send(sockfd, send_buf, strlen(send_buf) + 1, 0);
	if (len != strlen(send_buf) + 1)
	{
		cout<<"send error!"<<endl;
		return -1;
	}
	//printf("%d bytes sent\n", len);
	return 0;
}
/*****************************************************************************/
int ADNetClient::receive_data(char* recv_buf,int buf_total_size)
{
	if(connected)
	{
		int data= recv(sockfd, recv_buf, buf_total_size/*sizeof(recv_buf)*/, 0);
		//return 0;
		//cout<<"ADNetClient::receive_data: received = "<<data<<endl;
		return data;
	}
	else
		return -1;
}
/*****************************************************************************/
int ADNetClient::receive_data_blocking(char* recv_buf,int buf_total_size,int timeout_ms)
{
	int data=-1;int my_timeout=0;
	if(connected)
	{
		while(data==-1)
		{
			data = recv(sockfd, recv_buf, buf_total_size, 0);
			if(data!=-1)
				break;
			usleep(1000);
			if(my_timeout++>=timeout_ms)
			{
				//record time B here
				time_elapsed=CmdTimer.elapsed();
				return -1;
			}
		}
		//record time B here
		time_elapsed=CmdTimer.elapsed();			
		if(data>=0)recv_buf[data]='\0';
		return data;
	}
	else
		return -1;
}
/*****************************************************************************/
double ADNetClient::get_communication_time_in_ms()
{
	return time_elapsed;
}
/*****************************************************************************/
