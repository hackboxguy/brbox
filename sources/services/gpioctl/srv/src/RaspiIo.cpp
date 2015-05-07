#include "RaspiIo.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
/*---------------------------------------------------------------------------*/
//raspberry specific defines
#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
#define RASPI_GPIO_PAGE_SIZE     (4*1024)
#define RASPI_GPIO_BLOCK_SIZE    (4*1024)

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(pGpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(pGpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(pGpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))
#define GPIO_SET *(pGpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(pGpio+10) // clears bits which are 1 ignores bits which are 0
#define GET_GPIO(g) (*(pGpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH
#define GPIO_PULL *(pGpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(pGpio+38) // Pull up/pull down clock
/*---------------------------------------------------------------------------*/
RaspiIo::RaspiIo()
{
	NodeOpen=false;
}
/*---------------------------------------------------------------------------*/
RaspiIo::~RaspiIo()
{
}
/*---------------------------------------------------------------------------*/
RPC_SRV_RESULT RaspiIo::ReadGPIO (unsigned int addr,unsigned int &data)
{
	if(OpenGPIONode()!=RPC_SRV_RESULT_SUCCESS)
		return RPC_SRV_RESULT_FAIL;
	INP_GPIO(addr);
	data=GET_GPIO(addr);
	CloseGPIONode();
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT RaspiIo::WriteGPIO(unsigned int addr,unsigned int data)
{
	if(OpenGPIONode()!=RPC_SRV_RESULT_SUCCESS)
		return RPC_SRV_RESULT_FAIL;
	//for making pin as output, first make and IN and then OUT
	INP_GPIO(addr);
	OUT_GPIO(addr);
	if(data==0)
		GPIO_SET = 1<<addr;
	else
		GPIO_CLR = 1<<addr;
	CloseGPIONode();
	return RPC_SRV_RESULT_SUCCESS;
}
/*---------------------------------------------------------------------------*/
RPC_SRV_RESULT RaspiIo::OpenGPIONode()
{
	int  mem_fd;
	void *gpio_map;
	// open /dev/mem 
	if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) 
	{
		//printf("can't open /dev/mem \n");
		return RPC_SRV_RESULT_FAIL;
	}
	//mmap GPIO
	gpio_map = mmap(NULL,                 //Any adddress in our space will do
			RASPI_GPIO_BLOCK_SIZE,//Map length
			PROT_READ|PROT_WRITE, // Enable reading & writting to mapped memory
			MAP_SHARED,           //Shared with other processes
			mem_fd,               //File to map
			GPIO_BASE);           //Offset to GPIO peripheral
	close(mem_fd);//No need to keep mem_fd open after mmap
	if (gpio_map == MAP_FAILED) 
	{
		//printf("mmap error %d\n", (int)gpio_map);//errno also set!
		return RPC_SRV_RESULT_FAIL;
	}
	// Always use volatile pointer!
	pGpio = (volatile unsigned *)gpio_map;
	NodeOpen=true;
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT RaspiIo::CloseGPIONode()
{
	if(NodeOpen==true)
	{
		munmap((void*)pGpio,RASPI_GPIO_BLOCK_SIZE);
		NodeOpen=false;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/*---------------------------------------------------------------------------*/


