#include "I2CDualPcfLcd.hpp"
#include <unistd.h>
#include <string.h>
/*****************************************************************************/
I2CDualPcfLcd::I2CDualPcfLcd(std::string DevNode,std::string DevType):DisplayDevice(DevNode)//,I2CBusAccess(DevNode)
{
	io_ctrl_byte=0xff;
	init_lcd();
	clear_display_internal(DISPLAY_LINE_FULL);
}
I2CDualPcfLcd::~I2CDualPcfLcd()
{
}
/*****************************************************************************/
void I2CDualPcfLcd::update_io_ctrl_device()
{
	write_byte((uint32_t)LCD_PCF_CTRL_ADDRESS,io_ctrl_byte);
}
void I2CDualPcfLcd::update_io_ctrl_cache(uint8_t location,uint8_t value)
{
	if(value)//make this bit high in cache
		io_ctrl_byte|=(0x01<<location);
	else//make this bit low in cache
		io_ctrl_byte&=~(0x01<<location);
}
/*****************************************************************************/
void I2CDualPcfLcd::write_inst(uint8_t byte)
{
	update_io_ctrl_cache(0,0);//make RS=0
	update_io_ctrl_cache(1,0);//make LCDEN=0
	update_io_ctrl_device();

	//data
	write_byte((uint32_t)LCD_PCF_DATA_ADDRESS,byte);

	update_io_ctrl_cache(0,0);//make RS=0
	update_io_ctrl_cache(1,1);//make LCDEN=1
	update_io_ctrl_device();

	update_io_ctrl_cache(0,0);//make RS=0
	update_io_ctrl_cache(1,0);//make LCDEN=0
	update_io_ctrl_device();
}
/*****************************************************************************/
void I2CDualPcfLcd::write_data(uint8_t byte)
{
	update_io_ctrl_cache(0,1);//make RS=1
	update_io_ctrl_cache(1,0);//make LCDEN=0
	update_io_ctrl_device();

	write_byte((uint32_t)LCD_PCF_DATA_ADDRESS,byte);

	update_io_ctrl_cache(0,1);//make RS=1
	update_io_ctrl_cache(1,1);//make LCDEN=1
	update_io_ctrl_device();

	update_io_ctrl_cache(0,1);//make RS=1
	update_io_ctrl_cache(1,0);//make LCDEN=1
	update_io_ctrl_device();
}
/*****************************************************************************/
/********function for moving cursor to the required position**********/
#define DISPLAY_TYPE 16 //TODO: temprory
void I2CDualPcfLcd::go_to(uint8_t x)
{
	if(x>DISPLAY_TYPE){x-=DISPLAY_TYPE;x+=0x40;}//x+24;//0x40
	x=x-1;
	x|=0x80;
	write_inst(x);
//	usleep(50000);
}
/*****************************************************************************/
void I2CDualPcfLcd::go_to_centre(char *ptr,DISPLAY_LINE line)
{
	if(line==DISPLAY_LINE_2)go_to(((DISPLAY_TYPE-strlen(ptr))/2) +1+DISPLAY_TYPE);
	else go_to(((DISPLAY_TYPE-strlen(ptr))/2)+1);
//	usleep(10000);//wait(10);
}
/*****************************************************************************/
void I2CDualPcfLcd::init_lcd(void)
{
	io_ctrl_byte=0xFF;//this byte acts as the cache for pcf_io expander
	write_byte((uint32_t)LCD_PCF_CTRL_ADDRESS,io_ctrl_byte);
//	usleep(40000);//wait(40);
	write_inst(0x38);
//	usleep(10000);//wait(10);
	write_inst(0x38);
	write_inst(0x30);
//	usleep(10000);//wait(2);
	write_inst(0x30);
//	usleep(10000);//wait(2);
	write_inst(0x38);
	write_inst(0x14);
	write_inst(0x0C);
	write_inst(0x06);
	write_inst(0x01);
//	usleep(60000);//wait(60);
}
/*****************************************************************************/
void I2CDualPcfLcd::print_lcd(char *ptr)
{
	int i=1;
	while(*ptr)
	{
		write_data(*ptr++);
		i++;
		if(i>16)break;
	}
}
/*****************************************************************************/
void I2CDualPcfLcd::clear_display_internal(DISPLAY_LINE line)
{
	switch (line)
	{
		case DISPLAY_LINE_1:
			go_to(1);
			print_lcd((char*)"                ");//Blank);
			break;
		case DISPLAY_LINE_2:
			go_to(17);
			print_lcd((char*)"                ");//Blank);
			break;
		case DISPLAY_LINE_FULL:
			write_inst(0x01);
//			usleep(10000);
			break;
		default://arg error
			break;
	}
}
/*****************************************************************************/
void I2CDualPcfLcd::print_center(DISPLAY_LINE line,char *string)
{
	clear_display_internal(line);
	go_to_centre(string,line);
	print_lcd(string);
}
/*****************************************************************************/
//interface functions
RPC_SRV_RESULT I2CDualPcfLcd::print_line(char* msg,DISPLAY_LINE line,TEXT_ALIGNMENT align)
{
	switch(align)
	{
		case TEXT_ALIGNMENT_LEFT  :print_lcd(msg);break;
		case TEXT_ALIGNMENT_RIGHT :print_lcd(msg);break;//TODO
		//case TEXT_ALIGNMENT_CENTER:
		default:print_center(line,msg);break;
	}
	//print_lcd(msg);
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/

