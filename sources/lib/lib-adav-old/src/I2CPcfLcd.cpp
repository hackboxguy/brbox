#include "I2CPcfLcd.hpp"
#include <unistd.h>
#include <string.h>
#define LCD_LINE1 0x80
#define LCD_LINE2 0xC0
#define LCD_LINE3 0x94
#define LCD_LINE4 0xD4
#define LO_NIBBLE(b) (((b) << 4) & 0xF0)
#define HI_NIBBLE(b) ((b) & 0xF0)
#define LCD_BL 0x08
#define LCD_EN 0x04
#define LCD_RW 0x02
#define LCD_RS 0x01
/*****************************************************************************/
I2CPcfLcd::I2CPcfLcd(std::string DevNode,std::string DevType):DisplayDevice(DevNode)//,I2CBusAccess(DevNode)
{
	io_ctrl_byte=0xff;
	init_lcd();
	//clear_display_internal(DISPLAY_LINE_FULL);
	LCD_goto(1,1);
	//print_lcd("hello world");
}
I2CPcfLcd::~I2CPcfLcd()
{
}
/*****************************************************************************/
void I2CPcfLcd::update_io_ctrl_device()
{
	write_byte((uint32_t)LCD_PCF_ADDRESS,io_ctrl_byte);
	//ENABLE_HIGH;
	//delay(2);
	//ENABLE_LOW;
}
void I2CPcfLcd::update_io_ctrl_cache(uint8_t location,uint8_t value)
{
	if(value)//make this bit high in cache
		io_ctrl_byte|=(0x01<<location);
	else//make this bit low in cache
		io_ctrl_byte&=~(0x01<<location);
}
/*****************************************************************************/
void I2CPcfLcd::write_inst(uint8_t byte)
{
	update_io_ctrl_cache(0,0);//make RS=0
	update_io_ctrl_cache(1,0);//make R/W=0
	update_io_ctrl_cache(2,1);//make LCDEN=1
	update_io_ctrl_cache(3,1);//keep backlight high
	io_ctrl_byte = (io_ctrl_byte & 0x0F)|((byte<<4) & 0xF0);//send high nibble
	//io_ctrl_byte = (io_ctrl_byte & 0x0F)|(byte & 0xF0); // send lower nibble

	update_io_ctrl_device();
	update_io_ctrl_cache(2,0);//make LCDEN=0
	update_io_ctrl_device();
	update_io_ctrl_cache(2,1);//make LCDEN=1

	io_ctrl_byte = (io_ctrl_byte & 0x0F)|(byte & 0xF0); // send lower nibble
	//io_ctrl_byte = (io_ctrl_byte & 0x0F)|((byte<<4) & 0xF0);//send high nibble

	update_io_ctrl_device();
	update_io_ctrl_cache(2,0);//make LCDEN=0
	update_io_ctrl_device();
	update_io_ctrl_cache(2,1);//make LCDEN=1
}
/*****************************************************************************/
/********function for moving cursor to the required position**********/
#define DISPLAY_TYPE 16 //TODO: temprory
void I2CPcfLcd::go_to(uint8_t x)
{
	if(x>DISPLAY_TYPE){x-=DISPLAY_TYPE;x+=0x40;}//x+24;//0x40
	x=x-1;
	x|=0x80;
	write_inst(x);
}
/*****************************************************************************/
void I2CPcfLcd::LCD_goto(unsigned char row, unsigned char column)
{
	switch(row)
	{
	case 1: write_inst(LCD_LINE1 + (column - 1), 1); break;
	case 2: write_inst(LCD_LINE2 + (column - 1), 1); break;
	case 3: write_inst(LCD_LINE3 + (column - 1), 1); break;
	case 4: write_inst(LCD_LINE4 + (column - 1), 1); break;
	default: write_inst(LCD_LINE1 + (column - 1), 1); break;
	}
}
/*****************************************************************************/
void I2CPcfLcd::go_to_centre(char *ptr,DISPLAY_LINE line)
{
	if(line==DISPLAY_LINE_2)go_to(((DISPLAY_TYPE-strlen(ptr))/2) +1+DISPLAY_TYPE);
	else go_to(((DISPLAY_TYPE-strlen(ptr))/2)+1);
}
/*****************************************************************************/
void I2CPcfLcd::init_lcd(void)
{
	io_ctrl_byte=0xFF;//this byte acts as the cache for pcf_io expander
	write_byte((uint32_t)LCD_PCF_ADDRESS,io_ctrl_byte);
	write_inst(0x30,0);//usleep(50000);
	write_inst(0x30,0);//usleep(50000);
	write_inst(0x30,0);//usleep(50000);
	write_inst(0x20,0);//usleep(50000);
	write_inst(0x28,1);//usleep(50000);
	write_inst(0x08,1);//usleep(50000);
	write_inst(0x01,1);//usleep(50000); // display on cursor on
	write_inst(0x06,1);//usleep(50000); // increment cursor
	write_inst(0x0C,1);//usleep(50000); // increment cursor
}
/*****************************************************************************/
void I2CPcfLcd::write_inst(unsigned char data,unsigned char cmdtype)
{
	unsigned char lcddata; 
	// Write high nibble
	lcddata = HI_NIBBLE(data) |LCD_BL;
	write_byte((uint32_t)LCD_PCF_ADDRESS,lcddata | LCD_EN);usleep(100);
	write_byte((uint32_t)LCD_PCF_ADDRESS,lcddata & ~LCD_EN);usleep(100);

	// cmdtype = 0; One cycle write, cmdtype = 1; Two cycle writes
	if (cmdtype)
	{
		// Write low nibble
		lcddata = LO_NIBBLE(data) |LCD_BL;
		write_byte((uint32_t)LCD_PCF_ADDRESS,lcddata | LCD_EN);usleep(500);
		write_byte((uint32_t)LCD_PCF_ADDRESS,lcddata & ~LCD_EN);usleep(500);
	}
	//__delay_ms(2); // For most command, Wait > 100 us is ok.
}
/*****************************************************************************/
void I2CPcfLcd::write_data(unsigned char data)
{
	unsigned char lcddata;
	lcddata = HI_NIBBLE(data)|LCD_BL|LCD_RS; // Get high nibble
	write_byte((uint32_t)LCD_PCF_ADDRESS,lcddata | LCD_EN);usleep(500);
	write_byte((uint32_t)LCD_PCF_ADDRESS,lcddata & ~LCD_EN);usleep(500);
	lcddata = LO_NIBBLE(data)|LCD_BL|LCD_RS; // Get low nibble
	write_byte((uint32_t)LCD_PCF_ADDRESS,lcddata | LCD_EN);usleep(500);
	write_byte((uint32_t)LCD_PCF_ADDRESS,lcddata & ~LCD_EN);usleep(500);
} // LCD_putch()
/*****************************************************************************/
void I2CPcfLcd::print_lcd(char *ptr)
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
void I2CPcfLcd::clear_display_internal(DISPLAY_LINE line)
{
	switch (line)
	{
		case DISPLAY_LINE_1:
			LCD_goto(1,1);//go_to(1);
			print_lcd((char*)"                ");//Blank);
			break;
		case DISPLAY_LINE_2:
			LCD_goto(2,1);//go_to(17);
			print_lcd((char*)"                ");//Blank);
			break;
		case DISPLAY_LINE_FULL:
			write_inst(0x01,1);
			break;
		default://arg error
			break;
	}
}
/*****************************************************************************/
void I2CPcfLcd::print_center(DISPLAY_LINE line,char *string)
{
	clear_display_internal(line);
	go_to_centre(string,line);
	if(line==DISPLAY_LINE_1)LCD_goto(1,1);
	else if(line==DISPLAY_LINE_2)LCD_goto(2,1);
	print_lcd(string);
}
/*****************************************************************************/
//interface functions
RPC_SRV_RESULT I2CPcfLcd::print_line(char* msg,DISPLAY_LINE line,TEXT_ALIGNMENT align)
{
	switch(align)
	{
		case TEXT_ALIGNMENT_LEFT  :print_lcd(msg);break;
		case TEXT_ALIGNMENT_RIGHT :print_lcd(msg);break;//TODO
		//case TEXT_ALIGNMENT_CENTER:
		default:print_center(line,msg);break;//LCD_goto(1,1);print_lcd(msg);break//;break;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/

