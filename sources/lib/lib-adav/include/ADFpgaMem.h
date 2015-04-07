#ifndef __AD_FPGA_MEM_H_
#define __AD_FPGA_MEM_H_
#include "ADCommon.h"
typedef enum ADFPGA_MEM_ACC_T
{
	ADFPGA_MEM_ACC_OFF=0, //disconnected (munmapped or driver is closed)
	ADFPGA_MEM_ACC_ON,    //connected (mmap or driver-open is active)
	ADFPGA_MEM_ACC_UNKNOWN,
	ADFPGA_MEM_ACC_NONE
}ADFPGA_MEM_ACC;

class ADFpgaMem
{
	ADFPGA_MEM_ACC access_sts;
	int emulation_mode;
	int fpga_mem_node;//gpmc file node
	unsigned short *pFpga;//memory mmapped fpga pointer
	unsigned long fpga_memsize;
	unsigned long fpga_memaddr;
public:
	ADFpgaMem();
	~ADFpgaMem();
	//open/close fpga device access via driver or mmap
	//RPC_SRV_RESULT get_fpga_dev_access(int* state);
	//RPC_SRV_RESULT set_fpga_dev_access(SDFS_FPGA_DEVACC state);

	RPC_SRV_RESULT connect(char* fpga_mem_device,unsigned long fpga_mem_addr,unsigned long fpga_mem_size);
	RPC_SRV_RESULT disconnect();
	ADFPGA_MEM_ACC get_connection_status();

	RPC_SRV_RESULT set_emulation_mode(int mode);
	RPC_SRV_RESULT get_emulation_mode(int* mode);


	//fpga register direct access
	RPC_SRV_RESULT read_fpga_register(unsigned int addr,unsigned short* data);
	RPC_SRV_RESULT write_fpga_register(unsigned int addr,unsigned short  data);
	RPC_SRV_RESULT read_fpga_register_array(unsigned int addr,int size,unsigned short** data);
	RPC_SRV_RESULT write_fpga_register_array(unsigned int addr,int size,unsigned short** data);
	RPC_SRV_RESULT write_fpga_register_bit(unsigned int addr,int bitpos,int bitval); //for LSBBit, bitpos=0,  for MSBBit, bitpos=15
	RPC_SRV_RESULT read_fpga_register_bit(unsigned int addr,int bitpos,int* bitval); //for LSBBit, bitpos=0,  for MSBBit, bitpos=15

};
#endif
