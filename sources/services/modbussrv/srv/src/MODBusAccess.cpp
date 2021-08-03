#include "MODBusAccess.h"
#include <sys/time.h>
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
/* ------------------------------------------------------------------------- */
MODBusAccess::MODBusAccess(std::string DevNode,bool debuglog,int baud,char parity,int stopbits)
{
	LogFlag=debuglog;

	node=DevNode;
	DevOpened=InitModbus(node.c_str(),baud,parity,stopbits,LogFlag);//false);//9600,'N',1,false);
	if(DevOpened==RPC_SRV_RESULT_SUCCESS)
	{
		if(LogFlag)
			printf("MODBusAccess::MODBusAccess: InitModbus success\n");
	}
	else
	{
		if(LogFlag)
			printf("MODBusAccess::MODBusAccess: InitModbus failed\n");
	}
	//LOG_DEBUG_MSG(LogFlag,"BRBOX:modbussrv","MODBusAccess::MODBusAccess:modbus dev open failed");
	//{
	//	float voltage=0;
	//	getMeasureFloat(ctx, VOLTAGE, 1, 2,voltage);
	//	printf("Voltage: %3.2f V \n",voltage);
	//}
}
/* ------------------------------------------------------------------------- */
MODBusAccess::~MODBusAccess()
{
	if(DevOpened==RPC_SRV_RESULT_SUCCESS)
	{
		modbus_close(ctx);
		modbus_free(ctx);
		DevOpened=RPC_SRV_RESULT_FAIL;
	}	
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT MODBusAccess::InitModbus(const char* devnode,int baud,char parity,int stop_bits,bool debug)
{
	ctx = modbus_new_rtu(devnode, baud, parity, 8, stop_bits);

	if (ctx == NULL)
	{
		if(LogFlag)
			printf("MODBusAccess::InitModbus: modbux ctx is null\n");
		return RPC_SRV_RESULT_DEV_NOT_ACCESSIBLE;//modbus is not accessible
	}
#if LIBMODBUS_VERSION_MAJOR >= 3 && LIBMODBUS_VERSION_MINOR >= 1 && LIBMODBUS_VERSION_MICRO >= 2
	if(LogFlag)
		printf("MODBusAccess::InitModbus: detected libmodbus ver is >=3.1.2\n");
	uint32_t resp_timeout = 2;
	modbus_set_byte_timeout(ctx, -1, 0);
	resp_timeout *= 100000;
	modbus_set_response_timeout(ctx, 0, resp_timeout);
#else
	if(LogFlag)
		printf("MODBusAccess::InitModbus: detected libmodbus ver is <3.1.2\n");
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
		if(LogFlag)
			printf("MODBusAccess::InitModbus: modbus connect failed\n");
		return RPC_SRV_RESULT_FAIL;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT MODBusAccess::getEnergyParam(EJSON_ENRGYPARM type,std::string &result)
{
	uint16_t addr;
	float param_value;
	switch(type)
	{
		case EJSON_ENRGYPARM_VOLTAGE      :addr=VOLTAGE  ;break;
		case EJSON_ENRGYPARM_CURRENT      :addr=CURRENT  ;break;
		case EJSON_ENRGYPARM_POWER        :addr=POWER    ;break;
		case EJSON_ENRGYPARM_POWER_ACT    :addr=APOWER   ;break;
		case EJSON_ENRGYPARM_POWER_REACT  :addr=RAPOWER  ;break;
		case EJSON_ENRGYPARM_PFACTOR      :addr=PFACTOR  ;break;
		case EJSON_ENRGYPARM_PHASE_ANGLE  :addr=PANGLE   ;break;
		case EJSON_ENRGYPARM_FREQ         :addr=FREQUENCY;break;
		case EJSON_ENRGYPARM_IMPORT_ACT   :addr=IAENERGY ;break;
		case EJSON_ENRGYPARM_EXPORT_ACT   :addr=EAENERGY ;break;
		case EJSON_ENRGYPARM_IMPORT_REACT :addr=IRAENERGY;break;
		case EJSON_ENRGYPARM_EXPORT_REACT :addr=ERAENERGY;break;
		case EJSON_ENRGYPARM_TOTAL_ACT    :addr=TAENERGY ;break;
		case EJSON_ENRGYPARM_TOTAL_REACT  :addr=TRENERGY ;break;
		default: return RPC_SRV_RESULT_ARG_ERROR;
	}

	//read the parameter over modbus
	RPC_SRV_RESULT res=getMeasureFloat(ctx, addr, 1, 2,param_value);//addr
	char tmpstr[255];
	if(res==RPC_SRV_RESULT_SUCCESS)
		sprintf(tmpstr,"%f",param_value);
	else
		sprintf(tmpstr,"read-error");
	result=tmpstr;
	return res;
}
/* ------------------------------------------------------------------------- */
RPC_SRV_RESULT MODBusAccess::getMeasureFloat(modbus_t *ctx, int address, int retries, int nb,float &result) 
{
	if(DevOpened!=RPC_SRV_RESULT_SUCCESS)
		return RPC_SRV_RESULT_DEV_NOT_ACCESSIBLE;
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
		gettimeofday(&tvStart, NULL); 
		rc = modbus_read_input_registers(ctx, address, nb, tab_reg);
		errno_save = errno;
		gettimeofday(&tvStop, NULL); 
	}

	if (rc == -1) 
	{
		if(LogFlag)
			printf("MODBusAccess::getMeasureFloat: modbus_read_input_registers failed\n");
		return RPC_SRV_RESULT_FAIL;
	}
	// swap LSB and MSB
	uint16_t tmp1 = tab_reg[0];
	uint16_t tmp2 = tab_reg[1];
	tab_reg[0] = tmp2;
	tab_reg[1] = tmp1;

	result = modbus_get_float(&tab_reg[0]);
	return RPC_SRV_RESULT_SUCCESS;
}
/* ------------------------------------------------------------------------- */
//return write_register(ctx, 1, 1, 0x0300);//toggle single relay having slave addr=1
//return write_register(ctx, 1, 1, 0x0100);//switch-ON single relay having slave addr=1
//return write_register(ctx, 1, 1, 0x0200);//switch-OFF single relay having slave addr=1
//uint16_t tmp_value;return read_register(ctx,  1, 1, &tmp_value);//read switch state of single relay having slave addr=1
//bin/modbusclt --relay=1,1,on
//bin/modbusclt --relay=1,1,off
//bin/modbusclt --relay=1,1,toggle
//bin/modbusclt --relay=1,1
RPC_SRV_RESULT MODBusAccess::write_register(modbus_t *ctx, uint16_t slave, uint16_t address, uint16_t value)
{
	modbus_set_slave(ctx, slave);
	if(modbus_write_register(ctx, address, value) == 1)
		return RPC_SRV_RESULT_SUCCESS;
	else
		return RPC_SRV_RESULT_FAIL;
}
RPC_SRV_RESULT MODBusAccess::read_register(modbus_t *ctx, uint16_t slave, uint16_t address, uint16_t *value)
{
	int nb=1;
	uint16_t tab_reg[nb * sizeof(uint16_t)];
	modbus_set_slave(ctx, slave);
	int res=modbus_read_registers(ctx, address,nb, tab_reg);
	if(res==-1)
		return RPC_SRV_RESULT_FAIL;
	else
	{
		//TODO: check exact number of bytes read
		*value=tab_reg[0];
		printf("res=%d value=%04X\n",res,tab_reg[0]);
		return RPC_SRV_RESULT_SUCCESS;
	}
}
/* ------------------------------------------------------------------------- */
//how to set new baud rate
//new_baud_rate=2 (2 is for 9600)
//changeConfigFloat(ctx, BAUD_RATE, new_baud_rate, RESTART_FALSE, 2);
//modbus_close(ctx);
//modbus_free(ctx);



