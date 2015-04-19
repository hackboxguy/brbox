#include "ADSerial.hpp"
#include "ADCommon.hpp"

#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;
/*****************************************************************************/
int ADSerialProducer::IDGenerator = 0;//generate Unique ID for every ADSerial object
/*****************************************************************************/
//helper callback functions from ADTimerConsumer,ADGenericChain, ADThread
int ADSerial::timer_notification()
{
	if(connected==0)
		return -1;

	//cout<<"got timer notification"<<endl;
	return 0;
}
int ADSerial::sigio_notification()//int sig_num)
{
	//cout<<"ADSerial::sigio notification"<<endl;
	if(connected==0)
		return -1;

	//check if this sigio event was due to data transmit_done
	/*if(transmission_in_progress==1 )
	{
	        if(calibModeActive ==0)
	        {
        		transmission_in_progress=0;
        		return 0;
		}
	}

	if(calibModeActive == 3) //stop calib mode,
	{
	        calibModeActive = 0;
	}

	if(calibModeActive == 2) //stop calib mode,
	{
	        calibModeActive = 3;
	}

	//Note: signal is received after RX or TX
	//TODOwakeup_thread(&pGlobalObj->rx_thread);
	//cout<<"got sigio notification!"<<endl;*/
	rx_thread.wakeup_thread();
	return 0;
}
int ADSerial::custom_sig_notification(int signum)
{
	return 0;
}
int ADSerial::identify_chain_element(void* element,int ident,ADChainProducer* pObj)
{
	//do the identification task
	//if(pObj->getID() == idreq);//this is called from reqChain
	//else if(pObj->getID() == idres);//this is called from resChain
	return -1;
}
int ADSerial::double_identify_chain_element(void* element,int ident1,int ident2,ADChainProducer* pObj)
{
	//do the identification task
	//if(pObj->getID() == idreq);//this is called from reqChain
	//else if(pObj->getID() == idres);//this is called from resChain
	return -1;
}
int ADSerial::free_chain_element_data(void* element,ADChainProducer* pObj)
{
	//this call can come from both rx or tx chain context
	//in this case, bot rx and tx data elements are same, hence no need to identify them saperately
	rx_tx_data *objData;
	objData=(rx_tx_data*)element;
	if(objData->RxTxdata!=NULL)
		MEM_DELETE(objData->RxTxdata);
	return 0;
}
int ADSerial::monoshot_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	//check if this context is called from which thread by reading ID.
	pthread_mutex_lock(&multi_callback_lock);
	int call_from=pObj->getID();
	//receive_thread
	if(call_from == id_rx_thread)
	{
		int data_len;
		unsigned char buf[MAX_RX_TX_CHUNK_SZ];//4096];
		data_len = read_serial_data(fd,buf,MAX_RX_TX_CHUNK_SZ-1);
		if(data_len<1)
		{
			//cout<<"rx_thread: no data received!!!"<<endl;
			pthread_mutex_unlock(&multi_callback_lock);
			return 0;
		}


//#ifdef RX_TX_DEBUG_PRINT
	if(uartlog)
	{
		printf("%s:rx <== ",get_timestamp());for(int i=0;i<data_len;i++)printf("%c",buf[i]);printf("\n");
		printf("             rxH<== ");for(int j=0;j<data_len;j++)printf("%02X ",buf[j]);printf("\n");
	}
//#endif
		//ADCSerial_process_received_data(serialObj,buf,data_len);
		//cout<<"rx_thread: data received!!!"<<endl;
		process_received_data(buf,data_len);
		pthread_mutex_unlock(&multi_callback_lock);
		return 0;
	}
	//transmit_thread
	else if(call_from == id_tx_thread)
	{
		rx_tx_data *rt_data;
		//serialObj=(ADCSerialObj*)data;	
		rt_data=(rx_tx_data*)tx_chain.chain_get();//(&serialObj->tx_chain);
		if(rt_data!=NULL)
		{
			//ADCSerial_send_bytes(serialObj->fd,rt_data->RxTxdata,rt_data->len);
			write_serial_data(fd,rt_data->RxTxdata,rt_data->len);
			MEM_DELETE(rt_data->RxTxdata);
			MEM_DELETE(rt_data);
			//cout<<"tx_thread: Data transmission in progress!!"<<endl;
		}
		pthread_mutex_unlock(&multi_callback_lock);
		return 0;
	}
	//process_received_data_thread
	else if(call_from == id_rx_data_proc_thread)
	{
		notify_data_arrival(&rx_chain);//callback function of user
	}
	pthread_mutex_unlock(&multi_callback_lock);
	return 0;
}
int ADSerial::thread_callback_function(void* pUserData,ADThreadProducer* pObj)
{
	//in contrast to monoshot-callback, this function shall not exit until
	//stop flag notification(sent by the ADThread Object).
	//it should stay in a while-loop
	cout<<"ADAVClass::thread_callback_function() called id="<<pObj->getID()<<endl;
	return 0;	
}
/*****************************************************************************/
ADSerial::ADSerial()//:baud(115200)
{
	pthread_mutex_init(&multi_callback_lock,NULL);
	transmission_in_progress=0;
	timer_event_attached=0;
	connected=0;
	baud=9600;//115200;
	strcpy(dev_node,"/dev/ttyS0");
	uartlog=0;
	//calibModeActive=0;
	initialize_helpers();
}
ADSerial::ADSerial(int baud_rate,char* dev_node_name)
{
	pthread_mutex_init(&multi_callback_lock,NULL);
	transmission_in_progress=0;
	timer_event_attached=0;
	connected=0;
	baud=baud_rate;
	if(strlen(dev_node_name)<DEV_NODE_NAME_MAX_SIZE)
		strcpy(dev_node,dev_node_name);
	else
		strcpy(dev_node,"/dev/ttyS0");
	initialize_helpers();
}
/*****************************************************************************/
ADSerial::~ADSerial()
{
	stop();
}
/*****************************************************************************/
int ADSerial::set_serial_port_settings(int baud_rate,char* device)
{
	baud=baud_rate;
	strcpy(dev_node,device);
	return 0;
}
/*****************************************************************************/
int ADSerial::initialize_helpers(void)
{
	//attach chain callback
	id_rx_chain=rx_chain.attach_helper(this);
	id_tx_chain=tx_chain.attach_helper(this);
	
	//attach thread callback
	id_rx_thread           = rx_thread.subscribe_thread_callback(this);
	id_tx_thread           = tx_thread.subscribe_thread_callback(this);
	id_rx_data_proc_thread = rx_data_proc_thread.subscribe_thread_callback(this);

	//initialize threads
	rx_thread.set_thread_properties(THREAD_TYPE_MONOSHOT,(void *)this);
	tx_thread.set_thread_properties(THREAD_TYPE_MONOSHOT,(void *)this);
	rx_data_proc_thread.set_thread_properties(THREAD_TYPE_MONOSHOT,(void *)this);

	return 0;
}
/*****************************************************************************/
int ADSerial::attach_timer_and_signal_notifier(ADTimer *pTimer)
{
	pTimer->subscribe_timer_notification(this);
	timer_event_attached=1;
	return 0;
}
/*****************************************************************************/
int ADSerial::start()
{
	if(connected==1)//before re-starting, ADSerial must be stopped
		return -1;

	if(timer_event_attached==0)//without timer-and-signal event, not possible to receive data
		return -1;

	converted_baud=convert_baud(baud);
	if(converted_baud<0)
		return -1;

	// Open and Initialise port
	fd = open( dev_node, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK );
	if ( fd < 0 )
		return -1;

	//start threads
	rx_thread.start_thread();
	tx_thread.start_thread();
	rx_data_proc_thread.start_thread();

	return init_serial_port();
}
/*****************************************************************************/
int ADSerial::init_serial_port()
{
	int error;
	struct termios options;
//	struct sigaction saio;  // Definition of Signal action

	// Install the signal handler before making the device asynchronous
//	saio.sa_handler = ADCSerial_handler_IO;
//	saio.sa_flags = 0;
//	saio.sa_restorer = NULL;
//	sigaction( SIGIO, &saio, NULL );

	// Allow the process to receive SIGIO
//	fcntl( fd, F_SETOWN, getpid() );
	// Make the file descriptor asynchronous (the manual page says only 
	// O_APPEND and O_NONBLOCK, will work with F_SETFL...)
	fcntl( fd, F_SETFL, FASYNC );

	// Get the current options for the port...
	tcgetattr( fd, &old_options );
	//tcgetattr( fd, &options );
	memset(&options, 0, sizeof(options));  /* clear the new struct */


	options.c_cflag = converted_baud | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	options.c_cc[VMIN] = 0;      /* block untill n bytes are received */
	options.c_cc[VTIME] = 0;     /* block untill a timer expires (n * 100 mSec.) */
	//error = tcsetattr(Cport[comport_number], TCSANOW, &new_port_settings);
	error = tcsetattr( fd, TCSANOW, &options );
	if(error==-1)
	{
		close(fd);
		//printf("unable to adjust portsettings\n");
		return -1;
	}
	connected=1;
//#ifdef RX_TX_DEBUG_PRINT
	cout<<"Serial Port "<<dev_node<<" @"<<baud<<" initialized successfully"<<endl;
//#endif
	return 0;
}
/*****************************************************************************/
int ADSerial::stop()
{
	if(connected==0)//no need to stop if not started
		return 0;

	rx_data_proc_thread.stop_thread();//(&pSerialObj->rx_data_proc_thread);
	rx_thread.stop_thread();//(&pSerialObj->rx_thread);
	tx_thread.stop_thread();//(&pSerialObj->tx_thread);
	rx_chain.remove_all();//(&pSerialObj->rx_chain);
	tx_chain.remove_all();//(&pSerialObj->tx_chain);

	tcflush(fd, TCIOFLUSH );
	tcsetattr(fd,TCSANOW,&old_options);
	close(fd);
	connected=0;
	return 0;
}
/*****************************************************************************/
int ADSerial::convert_baud(int baudrate)
{
	int baudr;
	switch(baudrate)
	{
	case      50 : baudr = B50;
		   break;
	case      75 : baudr = B75;
		   break;
	case     110 : baudr = B110;
		   break;
	case     134 : baudr = B134;
		   break;
	case     150 : baudr = B150;
		   break;
	case     200 : baudr = B200;
		   break;
	case     300 : baudr = B300;
		   break;
	case     600 : baudr = B600;
		   break;
	case    1200 : baudr = B1200;
		   break;
	case    1800 : baudr = B1800;
		   break;
	case    2400 : baudr = B2400;
		   break;
	case    4800 : baudr = B4800;
		   break;
	case    9600 : baudr = B9600;us_data_time=100000;//100ms
		   break;
	case   19200 : baudr = B19200;
		   break;
	case   38400 : baudr = B38400;
		   break;
	case   57600 : baudr = B57600;
		   break;
	case  115200 : baudr = B115200;us_data_time=1000;//10ms
		   break;
	case  230400 : baudr = B230400;
		   break;
	case  460800 : baudr = B460800;
		   break;
	case  500000 : baudr = B500000;
		   break;
	case  576000 : baudr = B576000;
		   break;
	case  921600 : baudr = B921600;
		   break;
	case 1000000 : baudr = B1000000;
		   break;
	default      : printf("invalid baudrate\n");
		   return -1;
		   break;
	}
	return baudr;
}
/*****************************************************************************/
int ADSerial::push_data_into_chain(ADGenericChain* pChain, unsigned char* data, int len)
{
	rx_tx_data *data_obj=NULL;
	if(len>MAX_RX_TX_CHUNK_SZ)return -1;//dont handle very big data chunks
	OBJECT_MEM_CREATE(data_obj,rx_tx_data);
	if(data_obj==NULL)
		return -1;//memory allocation error
	data_obj->ident=pChain->chain_generate_ident();
	data_obj->len=len;
	data_obj->RxTxdata=NULL;
	//ARRAY_MEM_CREATE(data_obj->RxTxdata,len+2);//create extra byte;//compiler error
	data_obj->RxTxdata=(unsigned char*)malloc(len+2);
	if(data_obj->RxTxdata==NULL)
	{
		MEM_DELETE(data_obj);
		return -1;
	}
	memcpy(data_obj->RxTxdata,data,len);
	if(pChain->chain_put((void *)data_obj)!=0)
	{
		printf("failed! unable to push data_obj to chain!\n");
		MEM_DELETE(data_obj->RxTxdata);
		MEM_DELETE(data_obj);
		return -1;
	}
	return 0;
}
/*****************************************************************************/
//this functions doesnt immediately send data, instead schedules a thread for sending data
int ADSerial::schedule_data_transmission(unsigned char* data,int len)
{
	if(push_data_into_chain(&tx_chain,data,len)!=0)
		return -1;
	tx_thread.wakeup_thread();//&pSerialObj->tx_thread);//tell tx_thread to start sending the data
	return 0;
}
/*****************************************************************************/
int ADSerial::process_received_data(unsigned char* data,int len)
{
	//push this data to chain, and wake-up data-received thread
	if(push_data_into_chain(&rx_chain,data,len)!=0)
		return -1;
	//printf("going to wakeup thread\n");	
	rx_data_proc_thread.wakeup_thread();//&pSerialObj->rx_data_proc_thread);//wake-up incoming data handler
	return 0;
}
/*****************************************************************************/
int ADSerial::read_serial_data(int fd, unsigned char *buf, int size)
{
	int n;
#ifndef __STRICT_ANSI__                       /* __STRICT_ANSI__ is defined when the -ansi option is used for gcc */
	if(size>SSIZE_MAX)  size = (int)SSIZE_MAX;  /* SSIZE_MAX is defined in limits.h */
#else
	//if(size>4096)  size = 4096;
	if(size>MAX_RX_TX_CHUNK_SZ)  size = MAX_RX_TX_CHUNK_SZ;
#endif
	n = read(fd, buf, size);
	return(n);
}
/*****************************************************************************/
int ADSerial::write_serial_data(int fd,unsigned char* data,int bytesToSend)
{
	int writtenBytes = 0;
	int lastBytesWritten=0;

//#ifdef RX_TX_DEBUG_PRINT
	if(uartlog)
	{
		printf("%s:tx ==> ",get_timestamp());
		for(int i=0;i<bytesToSend;i++)printf("%c",data[i]);printf("\n");
		printf("             txH==> ");for(int j=0;j<bytesToSend;j++)printf("%02X ",data[j]);printf("\n");

	}
//#endif
        tcflush(fd,TCIOFLUSH);
        transmission_in_progress=1;
	while(writtenBytes < bytesToSend)
	{
		lastBytesWritten = write(fd,data + writtenBytes,bytesToSend - writtenBytes );   
		writtenBytes += lastBytesWritten;  
		if ( lastBytesWritten < 0 ) 
		{
		    printf("write failed!\n");
		    return -1;
		}
	}
	//transmission_in_progress=1;
	//printf("written bytes = %d\n",writtenBytes);
	return 0;
}
/*****************************************************************************/
void ADSerial::set_uart_logging(int log)
{
	uartlog=log;
}
int ADSerial::get_uart_logging()
{
	return uartlog;
}
/*****************************************************************************/
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
char* ADSerial::get_timestamp()
{
	char buffer[30];
	struct timeval tv;
	time_t curtime;
	uart_timestamp[0]='\0';
	gettimeofday(&tv, NULL); 
	curtime=tv.tv_sec;
	//strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));
	strftime(buffer,30,"%T.",localtime(&curtime));
	sprintf(uart_timestamp,"%s%03ld",buffer,(tv.tv_usec/1000));
	return uart_timestamp;
}
/*****************************************************************************/






