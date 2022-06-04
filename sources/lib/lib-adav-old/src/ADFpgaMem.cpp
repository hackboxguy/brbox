#include "ADFpgaMem.hpp"
#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;
/*****************************************************************************/
ADFpgaMem::ADFpgaMem()
{
	emulation_mode=0;
	access_sts=ADFPGA_MEM_ACC_OFF;
	fpga_memsize=0;
	fpga_memaddr=0;
}
/*****************************************************************************/
ADFpgaMem::~ADFpgaMem()
{
	disconnect();//freeup the memory
}
/*****************************************************************************/
//fpga register direct access
RPC_SRV_RESULT ADFpgaMem::read_fpga_register(unsigned int addr,unsigned short* data)
{
	if(access_sts==ADFPGA_MEM_ACC_OFF)
		return RPC_SRV_RESULT_DEV_NOT_ACCESSIBLE;
	//if(addr>=FPGA_REGISTER_MEMORY_BANK_SIZE)
	//	return RPC_SRV_RESULT_ARG_ERROR;//dont allow reading beyond gpmc area
	*data=pFpga[addr];
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT ADFpgaMem::write_fpga_register(unsigned int addr,unsigned short  data)
{
	if(access_sts==ADFPGA_MEM_ACC_OFF)
		return RPC_SRV_RESULT_DEV_NOT_ACCESSIBLE;
	//if(addr>=FPGA_REGISTER_MEMORY_BANK_SIZE)
	//	return RPC_SRV_RESULT_ARG_ERROR;//dont allow reading beyond gpmc area
	pFpga[addr]=data;
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
//fpga register direct access - array
RPC_SRV_RESULT ADFpgaMem::read_fpga_register_array(unsigned int addr,int size,unsigned short** data)
{
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT ADFpgaMem::write_fpga_register_array(unsigned int addr,int size,unsigned short** data)
{
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT ADFpgaMem::write_fpga_register_bit(unsigned int addr,int bitpos,int bitval) //for LSBBit, bitpos=0,  for MSBBit, bitpos=15
{
	unsigned short data,mask=1;
	if(read_fpga_register(addr,&data)!=RPC_SRV_RESULT_SUCCESS)
		return RPC_SRV_RESULT_FAIL;
	mask=mask<<bitpos;//shift 1 bitposTimes.
	if(bitval)//enable
		data|=mask;
	else //disable
		data&=~mask;
	return write_fpga_register(addr,data);
}
RPC_SRV_RESULT ADFpgaMem::read_fpga_register_bit(unsigned int addr,int bitpos,int* bitval) //for LSBBit, bitpos=0,  for MSBBit, bitpos=15
{
	unsigned short data,mask=1;
	if(read_fpga_register(addr,&data)!=RPC_SRV_RESULT_SUCCESS)
		return RPC_SRV_RESULT_FAIL;
	data&=mask<<bitpos;//shift 1 bitposTimes.
	if(data==0)
		*bitval=0;
	else
		*bitval=1;
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT ADFpgaMem::connect(char* fpga_mem_device,unsigned long fpga_mem_addr,unsigned long fpga_mem_size)
{
	if(access_sts==ADFPGA_MEM_ACC_ON)
		return RPC_SRV_RESULT_SUCCESS;//fpga is already mapped
	if(emulation_mode==0)
	{
		/********************************Open Device Node file************************************/
		if ((fpga_mem_node = open(fpga_mem_device,O_RDWR)) < 0)
			return RPC_SRV_RESULT_DEV_NOT_ACCESSIBLE;
		/********************************Write Data to device***********************************/
		void *pTemp=mmap(NULL,fpga_mem_size,PROT_READ|PROT_WRITE, MAP_SHARED,fpga_mem_node,fpga_mem_addr);
		if ( pTemp < MAP_FAILED)
		{
			close(fpga_mem_node);
			//printf("FpgaAccessMmap::connect(): dev not accessible\n");	
			return RPC_SRV_RESULT_DEV_NOT_ACCESSIBLE;
		}
		pFpga=(unsigned short *)pTemp;
		fpga_memsize=fpga_mem_size;
		fpga_memaddr=fpga_mem_addr;
	}
	else
	{
		pFpga = new unsigned short[fpga_mem_size];//FPGA_REGISTER_MEMORY_BANK_SIZE];
	}
	access_sts=ADFPGA_MEM_ACC_ON;
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT ADFpgaMem::disconnect()
{
	if(access_sts==ADFPGA_MEM_ACC_OFF)
		return RPC_SRV_RESULT_SUCCESS;//fpga mem is already disconnected
	if(emulation_mode==0)
	{
		munmap(pFpga,fpga_memsize);
		close(fpga_mem_node);
		fpga_memsize=0;
		fpga_memaddr=0;
		//printf("FpgaAccessMmap::dosconnect(): fpga unmapped successfully\n");	
	}
	else
	{
		delete[] pFpga;
	}
	access_sts=ADFPGA_MEM_ACC_OFF;
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT ADFpgaMem::set_emulation_mode(int mode)
{
	emulation_mode=mode;
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT ADFpgaMem::get_emulation_mode(int* mode)
{
	*mode=emulation_mode;
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
ADFPGA_MEM_ACC ADFpgaMem::get_connection_status()
{
	return access_sts;
}
/*****************************************************************************/


