#include "ADSerialProto.h"
#include <iostream>
using namespace std;
/*****************************************************************************/
//callback function from ADTimer
int ADSerialProto::timer_notification()
{
	return 0;
}
int ADSerialProto::sigio_notification()//int sig_num)
{
	//Note: signal is received after RX or TX
	//i dont need sigio, it is needed by ADSerial Object	
	return 0;
}
int ADSerialProto::custom_sig_notification(int signum)
{
	return 0;
}
/*****************************************************************************/
int ADSerialProto::notify_data_arrival(ADGenericChain* pRxChain,ADSerialProducer* pObj)
{
	return 0;
}
/*****************************************************************************/
ADSerialProto::ADSerialProto()
{
	//attach myself to ADSerial so that my callback function(notify_data_arrival)
	//is called upon reception of serial data
	SerialHandler.subscribe_data_arrival(this);
	pMyTimer=NULL;
}
/*****************************************************************************/
ADSerialProto::~ADSerialProto()
{
	SerialHandler.stop();
}
/*****************************************************************************/
int ADSerialProto::attach_timer_and_signal_notifier(ADTimer *pTimer)
{
	pTimer->subscribe_timer_notification(this);//attach myself
	SerialHandler.attach_timer_and_signal_notifier(pTimer);//attach ADSerial
	pMyTimer=pTimer;
	return 0;
}
int ADSerialProto::connect_uart(int baud,char* dev_node)
{
	if(pMyTimer==NULL)return -1;//timer-and-signal notificatin is must
	SerialHandler.set_serial_port_settings(baud,dev_node);
	return SerialHandler.start();//on successfull start, this will return 0
}
/*****************************************************************************/

