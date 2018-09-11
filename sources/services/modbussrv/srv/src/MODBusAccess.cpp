#include "MODBusAccess.h"
using namespace std;
#define VOLTAGE   0x0000
#define CURRENT   0x0006
#define POWER     0x000C
#define APOWER    0x0012
#define RAPOWER   0x0018
#define PFACTOR   0x001E
#define PANGLE    0x0024
#define FREQUENCY 0x0046
#define IAENERGY  0x0048
#define EAENERGY  0x004A
#define IRAENERGY 0x004C
#define ERAENERGY 0x004E
#define TAENERGY  0x0156
#define TRENERGY  0x0158
#include <unistd.h>
/*****************************************************************************/
MODBusAccess::MODBusAccess(std::string DevNode,int baud,char parity,int stopbits)
{
	node=DevNode;
	DevOpened=InitModbus(node.c_str(),baud,parity,stopbits,false);//9600,'N',1,false);
	//if(DevOpened==RPC_SRV_RESULT_SUCCESS)
	//{
	//	float voltage=0;
	//	getMeasureFloat(ctx, VOLTAGE, 1, 2,voltage);
	//	printf("Voltage: %3.2f V \n",voltage);
	//}
}
/*****************************************************************************/
MODBusAccess::~MODBusAccess()
{
	if(DevOpened==RPC_SRV_RESULT_SUCCESS)
	{
		modbus_close(ctx);
		modbus_free(ctx);
		DevOpened=RPC_SRV_RESULT_FAIL;
	}	
}
/*****************************************************************************/
RPC_SRV_RESULT MODBusAccess::InitModbus(const char* devnode,int baud,char parity,int stop_bits,bool debug)
{
	ctx = modbus_new_rtu(devnode, baud, parity, 8, stop_bits);

	if (ctx == NULL)
		return RPC_SRV_RESULT_DEV_NOT_ACCESSIBLE;//modbus is not accessible

#if LIBMODBUS_VERSION_MAJOR >= 3 && LIBMODBUS_VERSION_MINOR >= 1 && LIBMODBUS_VERSION_MICRO >= 2
	uint32_t resp_timeout = 2;
	modbus_set_byte_timeout(ctx, -1, 0);
	resp_timeout *= 100000;
	modbus_set_response_timeout(ctx, 0, resp_timeout);
#else
	//byte timeout
	struct timeval timeout;
	timeout.tv_sec = -1;
	timeout.tv_usec = 0;
	modbus_set_byte_timeout(ctx, &timeout);
	// Response timeout
	time_t resp_timeout =2;
	resp_timeout*= 100000;    
	timeout.tv_sec = 0;
	timeout.tv_usec = resp_timeout;
	modbus_set_response_timeout(ctx, &timeout);
#endif

	modbus_set_error_recovery(ctx, MODBUS_ERROR_RECOVERY_NONE);
	//usleep(settle_time);


	modbus_set_debug(ctx, debug?1:0);//1);
	int device_address = 1;
	modbus_set_slave(ctx, device_address);

	if (modbus_connect(ctx) == -1) 
	{
		modbus_free(ctx);
		return RPC_SRV_RESULT_FAIL;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT MODBusAccess::getMeasureFloat(modbus_t *ctx, int address, int retries, int nb,float &result) 
{
	uint16_t tab_reg[nb * sizeof(uint16_t)];
	int rc = -1;
	int i;
	int j = 0;
	int exit_loop = 0;
	int errno_save=0;
	struct timeval tvStart, tvStop;

	while (j < retries && exit_loop == 0) 
	{
		j++;

		//if (command_delay) 
		//{
		//	log_message(debug_flag, "Sleeping command delay: %ldus", command_delay);
		//	usleep(command_delay);
		//}

		//log_message(debug_flag, "%d/%d. Register Address %d [%04X]", j, retries, 30000+address+1, address);
		gettimeofday(&tvStart, NULL); 
		rc = modbus_read_input_registers(ctx, address, nb, tab_reg);
		errno_save = errno;
		gettimeofday(&tvStop, NULL); 
	}

	if (rc == -1) 
		return RPC_SRV_RESULT_FAIL;;//exit_error(ctx);

	// swap LSB and MSB
	uint16_t tmp1 = tab_reg[0];
	uint16_t tmp2 = tab_reg[1];
	tab_reg[0] = tmp2;
	tab_reg[1] = tmp1;

	result = modbus_get_float(&tab_reg[0]);
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
//how to set new baud rate
//new_baud_rate=2 (2 is for 9600)
//changeConfigFloat(ctx, BAUD_RATE, new_baud_rate, RESTART_FALSE, 2);
//modbus_close(ctx);
//modbus_free(ctx);



