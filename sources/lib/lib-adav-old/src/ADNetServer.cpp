#include "ADNetServer.hpp"
#include "ADCommon.hpp"

#include <iostream>
using namespace std;
#define AD_NETWORK_TRUE             1
#define AD_NETWORK_FALSE            0

int ADNetProducer::IDGenerator = 0;//generate Unique ID for every ADNetServer object
/*****************************************************************************/
//ADGenericChain callbacks
int ADNetServer::identify_chain_element(void* element,int ident,ADChainProducer* pObj)
{
	return -1;
}
int ADNetServer::double_identify_chain_element(void* element,int ident1,int ident2,ADChainProducer* pObj)
{
	return -1;
}
int ADNetServer::free_chain_element_data(void* element,ADChainProducer* pObj)
{
	net_data_obj *obj;
	obj=(net_data_obj*)element;
	//free(obj->data_buffer);
	OBJ_MEM_DELETE(obj->data_buffer);
	return 0;
}
/*****************************************************************************/
bool ADNetServer::IsConnectionAlive(int sock_descriptor)
{
	//char rec_buff[5];
	//printf("connection alive returned = %d\n",recv(sock_descriptor, rec_buff, 0, MSG_DONTWAIT));
	//return true;
	socklen_t len;
	struct sockaddr_storage addr;
	//char ipstr[INET6_ADDRSTRLEN];
	//int port;
	len = sizeof addr;
	if(getpeername(sock_descriptor, (struct sockaddr*)&addr, &len)==0)
	{
		//printf("connection is alive\n");
		return true;
	}
	else
	{
		//printf("connection is broken\n");
		return false;
	}

/*	if (addr.ss_family == AF_INET)// deal with both IPv4 and IPv6:
	{
		struct sockaddr_in *s = (struct sockaddr_in *)&addr;
		port = ntohs(s->sin_port);
		inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
	}
	else // AF_INET6
	{ 
		struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
		port = ntohs(s->sin6_port);
		inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
	}
	printf("Peer IP address: %s\n", ipstr);
	printf("Peer port      : %d\n", port);
*/
}
/*****************************************************************************/
//ADThread callbacks(this thread function sends the response data back to client)
int ADNetServer::monoshot_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	int call_from=pObj->getID();
	if(call_from == id_response_thread)//if my call
	{
		net_data_obj *resp_obj;
		resp_obj=(net_data_obj*)response_chain.chain_get();
		if(resp_obj!=NULL)
		{
			if(IsConnectionAlive(resp_obj->sock_descriptor))
				send(resp_obj->sock_descriptor,resp_obj->data_buffer,resp_obj->data_buffer_len,MSG_DONTWAIT|MSG_NOSIGNAL);//0);
			OBJ_MEM_DELETE(resp_obj->data_buffer);
			OBJ_MEM_DELETE(resp_obj);

		}
	}
	return 0;
}
int ADNetServer::thread_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	int call_from=pObj->getID();
	if(call_from != id_listen_thread)
		return 0;//not my call

	struct sockaddr in_addr;//used for reading client's ip & port
	socklen_t in_len;//used for reading client's ip & port

	int i,rc,desc_ready,close_conn,new_sd;
	fd_set working_set;
	//ADCNetServerObj *server;
	//server=(ADCNetServerObj*)data;	
	in_len=sizeof(in_addr);//used for reading client's ip & port

	do
	{
		timeout.tv_sec  = 1;// * 60;
		timeout.tv_usec = 0;
		/**********************************************************/
		/* Copy the master fd_set over to the working fd_set.     */
		/**********************************************************/
		memcpy(&working_set, &master_set, sizeof(master_set));

		/**********************************************************/
		/* Call select() and wait 5 minutes for it to complete.   */
		/**********************************************************/
		//printf("Waiting on select()...\n");
		rc = select(max_sd + 1, &working_set, NULL, NULL, &timeout);

		if(end_server == AD_NETWORK_TRUE)
			break;

		/**********************************************************/
		/* Check to see if the select call failed.                */
		/**********************************************************/
		if (rc < 0)
		{
			printf("  select() failed");
			break;
		}
		/**********************************************************/
		/* Check to see if 1sec time out expired.         */
		/**********************************************************/
		if (rc == 0)
		{
			//printf("  select() timed out.  End program.\n");
			//break;
			continue;
		}

		/**********************************************************/
		/* One or more descriptors are readable.  Need to         */
		/* determine which ones they are.                         */
		/**********************************************************/
		desc_ready = rc;
		for (i=0; i <= max_sd  &&  desc_ready > 0; ++i)
		{
			/*******************************************************/
			/* Check to see if this descriptor is ready            */
			/*******************************************************/
			if (FD_ISSET(i, &working_set))
			{
				/****************************************************/
				/* A descriptor was found that was readable - one   */
				/* less has to be looked for.  This is being done   */
				/* so that we can stop looking at the working set   */
				/* once we have found all of the descriptors that   */
				/* were ready.                                      */
				/****************************************************/
				desc_ready -= 1;

				/****************************************************/
				/* Check to see if this is the listening socket     */
				/****************************************************/
				if (i == listen_sd)
				{
					//printf("  Listening socket is readable\n");
					/*************************************************/
					/* Accept all incoming connections that are      */
					/* queued up on the listening socket before we   */
					/* loop back and call select again.              */
					/*************************************************/
					do
					{
						/**********************************************/
						/* Accept each incoming connection.  If       */
						/* accept fails with EWOULDBLOCK, then we     */
						/* have accepted all of them.  Any other      */
						/* failure on accept will cause us to end the */
						/* server.                                    */
						/**********************************************/
						new_sd = accept(listen_sd, &in_addr,&in_len);//NULL, NULL);
						if (new_sd < 0)
						{
							if (errno != EWOULDBLOCK)
							{
								printf("  accept() failed");
								end_server = AD_NETWORK_TRUE;
							}
							break;
						}
						/**********************************************/
						/* check the ip addr and port num of  incoming*/
						/* client connection,this is useful for       */
						/* sending async notification                 */
						/**********************************************/
						print_client_info(&in_addr,in_len);

						/**********************************************/
						/* Add the new incoming connection to the     */
						/* master read set                            */
						/**********************************************/
						//printf("  New incoming connection - %d\n", new_sd);
						FD_SET(new_sd, &master_set);
						if (new_sd > max_sd)
							max_sd = new_sd;

						/**********************************************/
						/* Loop back up and accept another incoming   */
						/* connection                                 */
						/**********************************************/
					} while (new_sd != -1);
				}
				/****************************************************/
				/* This is not the listening socket, therefore an   */
				/* existing connection must be readable             */
				/****************************************************/
				else
				{
					//printf("  Descriptor %d is readable\n", i);
					close_conn = AD_NETWORK_FALSE;
					/*************************************************/
					/* Receive all incoming data on this socket      */
					/* before we loop back and call select again.    */
					/*************************************************/
					do
					{
						/**********************************************/
						/* Receive data on this connection until the  */
						/* recv fails with EWOULDBLOCK.  If any other */
						/* failure occurs, we will close the          */
						/* connection.                                */
						/**********************************************/
						rc = recv(i, receive_buffer, sizeof(receive_buffer), MSG_DONTWAIT);//0);
						//printf("received %d bytes\n",rc);
						if (rc < 0)
						{
							if (errno != EWOULDBLOCK)
							{
								//printf("  recv() failed\n");
								close_conn = AD_NETWORK_TRUE;
							}
							break;
						}
						/**********************************************/
						/* Check to see if the connection has been    */
						/* closed by the client                       */
						/**********************************************/
						if (rc == 0)
						{
							//printf("  Connection closed\n");
							close_conn = AD_NETWORK_TRUE;
							break;
						}
						/**********************************************/
						/* Data was recevied                          */
						/**********************************************/
						receive_size = rc;
						receive_buffer[receive_size]='\0';
						//users request handler						
						//TODO:(server->req_handler)(server,i);
						switch(sock_type)
						{
							case ADLIB_TCP_SOCKET_TYPE_JSON:json_receive_data_and_notify_consumer(i,receive_buffer,receive_size);break;
							default:binary_receive_data_and_notify_consumer(i,receive_buffer,receive_size);break;
						}
					} while (AD_NETWORK_TRUE);
					/*************************************************/
					/* If the close_conn flag was turned on, we need */
					/* to clean up this active connection.  This     */
					/* clean up process includes removing the        */
					/* descriptor from the master set and            */
					/* determining the new maximum descriptor value  */
					/* based on the bits that are still turned on in */
					/* the master set.                               */
					/*************************************************/
					if (close_conn)
					{
						close(i);
						FD_CLR(i, &master_set);
						if (i == max_sd)
						{
							while (FD_ISSET(max_sd, &master_set) == AD_NETWORK_FALSE)
							max_sd -= 1;
						}
					}
				} /* End of existing connection is readable */
			} /* End of if (FD_ISSET(i, &working_set)) */
		} /* End of loop through selectable descriptors */
	} while (end_server == AD_NETWORK_FALSE);
	return 0;
}
/*****************************************************************************/
ADNetServer::ADNetServer()
{
	receive_size_pending=0;
	socketlog=0;//disable logging
	connected=0;
	listen_port=AD_NET_SERVER_DEFAULT_LISTEN_PORT;
	end_server = AD_NETWORK_FALSE;
	sock_type=ADLIB_TCP_SOCKET_TYPE_JSON;
	initialize_helpers();//attach callbacks
}
/*****************************************************************************/
ADNetServer::ADNetServer(int port)
{
	receive_size_pending=0;
	socketlog=0;
	connected=0;
	listen_port=port;//AD_NET_SERVER_DEFAULT_LISTEN_PORT;
	end_server = AD_NETWORK_FALSE;
	sock_type=ADLIB_TCP_SOCKET_TYPE_JSON;
	initialize_helpers();//attach callbacks
}
/*****************************************************************************/
ADNetServer::~ADNetServer()
{
	stop_listening();
}
/*****************************************************************************/
int ADNetServer::start_listening(int port,int socket_log,ADLIB_TCP_SOCKET_TYPE socket_type)
{
	socketlog=socket_log;
	listen_port=port;
	sock_type=socket_type;
	return start_listening();
}
/*****************************************************************************/
int ADNetServer::start_listening()//connect()
{
	int rc;
	int on=1;//setsockopt:optval and optsize

	/*************************************************************/
	/* Create an AF_INET stream socket to receive incoming       */
	/* connections on                                            */
	/*************************************************************/
	listen_sd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sd < 0)
	{
		printf("socket() failed");
		return -1;
	}

	/*************************************************************/
	/* Allow socket descriptor to be reuseable                   */
	/*************************************************************/
	rc = setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR,
		   (char *)&on, sizeof(on));
	if (rc < 0)
	{
		printf("setsockopt() failed");
		close(listen_sd);
		return -1;
	}

	/*************************************************************/
	/* Set socket to be non-blocking.  All of the sockets for    */
	/* the incoming connections will also be non-blocking since  */
	/* they will inherit that state from the listening socket.   */
	/*************************************************************/
	rc = ioctl(listen_sd, FIONBIO, (char *)&on);
	if (rc < 0)
	{
		printf("ioctl() failed");
		close(listen_sd);
		return -1;
	}

	/*************************************************************/
	/* Bind the socket                                           */
	/*************************************************************/
	memset(&addr, 0, sizeof(addr));
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port        = htons(listen_port);//SERVER_PORT);
	//listen_port=listen_port;
	rc = bind(listen_sd,(struct sockaddr *)&addr, sizeof(addr));
	if (rc < 0)
	{
		printf("bind() failed");
		close(listen_sd);
		return -1;
	}

	/*************************************************************/
	/* Set the listen back log                                   */
	/*************************************************************/
	rc = listen(listen_sd, 32);
	if (rc < 0)
	{
		printf("listen() failed");
		close(listen_sd);
		return -1;
	}
	/*************************************************************/
	/* Initialize the master fd_set                              */
	/*************************************************************/
	FD_ZERO(&master_set);
	max_sd = listen_sd;
	FD_SET(listen_sd, &master_set);

	end_server = AD_NETWORK_FALSE;//thread checks for this flag
	//initialize_helpers();
	response_thread.start_thread();
	listen_thread.start_thread();
	connected=1;
	return 0;
}
int ADNetServer::stop_listening()
{
	if(connected==0)//no need to stop if not started
		return 0;
	end_server = AD_NETWORK_TRUE;

	response_thread.stop_thread();

	//end_server = AD_NETWORK_TRUE;
	sleep(1);
	listen_thread.stop_thread();

	request_chain.remove_all();
	response_chain.remove_all();

	for (int i=0; i <= max_sd; ++i)
	{
		if (FD_ISSET(i, &master_set))
		 close(i);
	}

	connected=0;
	return 0;
}
/*****************************************************************************/
int ADNetServer::initialize_helpers(void)
{
	//attach chain callback
	//id_response_chain=response_chain.attach_helper(this);
	request_chain.attach_helper(this);
	response_chain.attach_helper(this);
	
	//attach thread callback
	id_listen_thread   = listen_thread.subscribe_thread_callback(this);
	id_response_thread = response_thread.subscribe_thread_callback(this);

	//initialize threads
	listen_thread.set_thread_properties(THREAD_TYPE_NOBLOCK,(void *)this);
	response_thread.set_thread_properties(THREAD_TYPE_MONOSHOT,(void *)this);

	return 0;
}
/*****************************************************************************/
int ADNetServer::print_client_info(struct sockaddr *in_addr,socklen_t in_len)
{
	int in_clt_info;
	char hbuf[NI_MAXHOST],sbuf[NI_MAXSERV];
	in_clt_info = getnameinfo (in_addr,in_len,hbuf,sizeof(hbuf),sbuf,sizeof(sbuf),NI_NUMERICHOST | NI_NUMERICSERV);
	if(in_clt_info==0)
	{
//#ifdef SOCKET_DEBUG_MESSAGE
		if(socketlog)
			printf("accepted connection from clientip=%s, port=%s\n",hbuf,sbuf);
//#endif
		//;
	}
	else
	{
		if(socketlog)
			printf("getnameinfo returned = %d\n",in_clt_info);
	}
	return 0;
}
/*****************************************************************************/
int ADNetServer::schedule_response(int socket_descriptor,char *buf, int len)
{
	net_data_obj *resp_obj=NULL;
	OBJECT_MEM_NEW(resp_obj,net_data_obj);
	if(resp_obj==NULL)
		return -1;//memory allocation error
	resp_obj->ident=0;//TODO create internal ident
	resp_obj->sock_descriptor=socket_descriptor;
	resp_obj->data_buffer_len=len;
	resp_obj->data_buffer=NULL;
	ARRAY_MEM_NEW(resp_obj->data_buffer,(len+2));
	if(resp_obj->data_buffer==NULL)
	{
		OBJ_MEM_DELETE(resp_obj);
		return -1;
	}
	strcpy(resp_obj->data_buffer,buf);
	if(response_chain.chain_put((void *)resp_obj)!=0)
	{
		printf("failed! unable to push response object to chain!\n");
		//free(resp_obj->data_buffer);
		//free(resp_obj);
		OBJ_MEM_DELETE(resp_obj->data_buffer);
		OBJ_MEM_DELETE(resp_obj);
		return -1;
	}
	response_thread.wakeup_thread();//tell response_thread to start sending the data
	return 0;
/*

	net_data_obj *resp_obj=NULL;
	resp_obj=(net_data_obj *)malloc(sizeof(net_data_obj));
	if(resp_obj==NULL)
		return -1;//memory allocation error
	resp_obj->ident=0;//TODO create internal ident
	resp_obj->sock_descriptor=socket_descriptor;
	resp_obj->data_buffer_len=len;
	resp_obj->data_buffer = (char*)malloc(len+2);//include space for null character
	if(resp_obj->data_buffer==NULL)
	{
		free(resp_obj);
		return -1;
	}
	strcpy(resp_obj->data_buffer,buf);
	if(response_chain.chain_put((void *)resp_obj)!=0)
	{
		printf("failed! unable to push response object to chain!\n");
		free(resp_obj->data_buffer);
		free(resp_obj);
		return -1;
	}
	response_thread.wakeup_thread();//tell response_thread to start sending the data
	return 0;
*/
}
/*****************************************************************************/
//upon arrival of data on tcp socket, copy this data to internal chain
//and notify the consumer(the one who processes the data)
int ADNetServer::binary_receive_data_and_notify_consumer(int socket_descriptor,char *buf, int len)
{
	printf("receive_data_and_notify_consumer called\n");
	net_data_obj *resp_obj=NULL;
	OBJECT_MEM_NEW(resp_obj,net_data_obj);
	if(resp_obj==NULL)
		return -1;//memory allocation error
	resp_obj->ident=request_chain.chain_generate_ident();
	resp_obj->sock_descriptor=socket_descriptor;
	resp_obj->data_buffer_len=len;
	ARRAY_MEM_NEW(resp_obj->data_buffer,(len+2));
	if(resp_obj->data_buffer==NULL)
	{
		OBJ_MEM_DELETE(resp_obj);
		return -1;
	}
	strcpy(resp_obj->data_buffer,buf);
	if(request_chain.chain_put((void *)resp_obj)!=0)
	{
		printf("failed! unable to push response object to chain!\n");
		OBJ_MEM_DELETE(resp_obj->data_buffer);
		OBJ_MEM_DELETE(resp_obj);
		return -1;
	}
	notify_data_arrival(&request_chain);//notify consumer
	printf("receive_data_and_notify_consumer finished\n");
	return 0;
}
/*****************************************************************************/
//new packet receiver with tcp framing logic(to separate multiple json requests of a tcp stream)
int ADNetServer::json_receive_data_and_notify_consumer(int socket_descriptor,char *buf, int len)
{
	int last_index=0;
	char* pTemp=buf;
	//check if full json object has arrived, else wait for the remaining bytes to arrive
	//if(segmented_json_object(socket_descriptor,buf,&len))
	//	return 0;
	for(int i=0;i<len;i++) //logic to find multiple json objects in a tcp stream
	{
		if(buf[i] == '}' && buf[i+1] == '{') //its easy becase json has curly-braces as delimiter
		{
			//printf("found two objects");
			net_data_obj *resp_obj=NULL;
			OBJECT_MEM_NEW(resp_obj,net_data_obj);
			if(resp_obj==NULL)
			{
				return -1;//memory allocation error
			}
			resp_obj->ident=request_chain.chain_generate_ident();
			resp_obj->sock_descriptor=socket_descriptor;
			resp_obj->data_buffer_len=i-last_index+1;//len;
			ARRAY_MEM_NEW(resp_obj->data_buffer,(i-last_index+3));
			if(resp_obj->data_buffer==NULL)
			{
				OBJ_MEM_DELETE(resp_obj);
				return -1;
			}
			strncpy(resp_obj->data_buffer,pTemp,i-last_index+1);
			resp_obj->data_buffer[i-last_index+1]='\0';
			pTemp=&buf[i+1];//printf(" next = %c\n",pTemp[0]);
			if(request_chain.chain_put((void *)resp_obj)!=0)
			{
				printf("failed! unable to push response object to chain!\n");
				OBJ_MEM_DELETE(resp_obj->data_buffer);
				OBJ_MEM_DELETE(resp_obj);
				return -1;
			}
			last_index=i+1;

			//printf("successful notification for first object\n");
			//printf("%s    len=%d\n",resp_obj->data_buffer,resp_obj->data_buffer_len);
			//notify_data_arrival(&request_chain);//notify consumer
		}
	}

	net_data_obj *resp_obj=NULL;
	OBJECT_MEM_NEW(resp_obj,net_data_obj);
	if(resp_obj==NULL)
	{
		return -1;//memory allocation error
	}
	resp_obj->ident=request_chain.chain_generate_ident();
	resp_obj->sock_descriptor=socket_descriptor;
	resp_obj->data_buffer_len=len-last_index;
	ARRAY_MEM_NEW(resp_obj->data_buffer,(len-last_index)+2);////(len+2));
	if(resp_obj->data_buffer==NULL)
	{
		OBJ_MEM_DELETE(resp_obj);
		return -1;
	}
	//strcpy(resp_obj->data_buffer,buf);
	strncpy(resp_obj->data_buffer,pTemp,(len-last_index)+1);

	if(request_chain.chain_put((void *)resp_obj)!=0)
	{
		printf("failed! unable to push response object to chain!\n");
		OBJ_MEM_DELETE(resp_obj->data_buffer);
		OBJ_MEM_DELETE(resp_obj);
		return -1;
	}
	notify_data_arrival(&request_chain);//notify consumer
	return 0;
}
/*****************************************************************************/
int ADNetServer::segmented_json_object(int socket_descriptor,char* buf,int *len)
{
	char* pendingPtr = &receive_buffer_pending[receive_size_pending];
	strcpy(pendingPtr,buf);
	//delimiter checking
	int curly_brace_open=0;
	int curly_brace_close=0;

	//check if json string is segmented
	for(int i=0;i<(*len+receive_size_pending);i++)
	{
		if(receive_buffer_pending[i] == '{')
			curly_brace_open++;
		else if(receive_buffer_pending[i] == '}')
			curly_brace_close++;
	}
	if( curly_brace_open != curly_brace_close)
	{
		receive_size_pending=*len;
		//printf("segmented data\n");
		return 1;
	}
	else
	{
		strcpy(buf,receive_buffer_pending);
		*len+=receive_size_pending;
		receive_size_pending=0;
	}
	return 0;
}

