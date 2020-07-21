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
//#define DISPLAY_TYPE 20 //16 //This class is for display length 16
/*****************************************************************************/
I2CPcfLcd::I2CPcfLcd(std::string DevNode,std::string DevType):DisplayDevice(DevNode)//,I2CBusAccess(DevNode)
{
	const char *dispTbl[] = ADLIB_DISPLAY_TYPE_TABL;
	ADCmnStringProcessor string_proc;
	disp_type=(ADLIB_DISPLAY_TYPE)string_proc.string_to_enum(dispTbl,(char*)DevType.c_str(),ADLIB_DISPLAY_TYPE_UNKNOWN);
	if(disp_type>=ADLIB_DISPLAY_TYPE_UNKNOWN)
		disp_type=ADLIB_DISPLAY_TYPE_1602_PCF;
	switch(disp_type)
	{
		//case ADLIB_DISPLAY_TYPE_1602_DUAL_PCF: DISPLAY_TYPE=16;break;
		//case ADLIB_DISPLAY_TYPE_1604_DUAL_PCF: DISPLAY_TYPE=16;break;
		case ADLIB_DISPLAY_TYPE_2002_PCF     :
		case ADLIB_DISPLAY_TYPE_2004_PCF     : DISPLAY_TYPE=20;break;
		default                              : DISPLAY_TYPE=16;break;
	}

	//there are 2 types of i2c-lcd-dot-matrix devices with 2 different addresses(0x7E/0x4E) or 7bit addr(0x3F/0x27)
	io_ctrl_byte=0xFF;
	LCD_PCF_ADDRESS=(0x7E>>1);//first try with address 0x7E
	if(write_byte((uint32_t)LCD_PCF_ADDRESS,io_ctrl_byte)!=RPC_SRV_RESULT_SUCCESS)
		LCD_PCF_ADDRESS=(0x4E>>1);//else try with address 0x4E
	BkLight=true;
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
	update_io_ctrl_cache(3,0);//keep backlight high
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
	//set_back_light(BkLight);
}
/*****************************************************************************/
void I2CPcfLcd::write_inst(unsigned char data,unsigned char cmdtype)
{
	unsigned char lcddata; 
	// Write high nibble
	if(BkLight==false)
		lcddata = HI_NIBBLE(data) ;//|LCD_BL;
	else	
		lcddata = HI_NIBBLE(data)|LCD_BL;
		
	write_byte((uint32_t)LCD_PCF_ADDRESS,lcddata | LCD_EN);usleep(100);
	write_byte((uint32_t)LCD_PCF_ADDRESS,lcddata & ~LCD_EN);usleep(100);

	// cmdtype = 0; One cycle write, cmdtype = 1; Two cycle writes
	if (cmdtype)
	{
		// Write low nibble
		if(BkLight==false)
			lcddata = LO_NIBBLE(data) ;//|LCD_BL;
		else
			lcddata = LO_NIBBLE(data)|LCD_BL;
		write_byte((uint32_t)LCD_PCF_ADDRESS,lcddata | LCD_EN);usleep(500);
		write_byte((uint32_t)LCD_PCF_ADDRESS,lcddata & ~LCD_EN);usleep(500);
	}
	//__delay_ms(2); // For most command, Wait > 100 us is ok.
}
/*****************************************************************************/
void I2CPcfLcd::write_data(unsigned char data)
{
	unsigned char lcddata;
	if(BkLight==false)
		lcddata = HI_NIBBLE(data)|LCD_RS;//|LCD_BL; // Get high nibble
	else
		lcddata = HI_NIBBLE(data)|LCD_RS|LCD_BL; // Get high nibble
	write_byte((uint32_t)LCD_PCF_ADDRESS,lcddata | LCD_EN);usleep(500);
	write_byte((uint32_t)LCD_PCF_ADDRESS,lcddata & ~LCD_EN);usleep(500);
	if(BkLight==false)
		lcddata = LO_NIBBLE(data)|LCD_RS;//|LCD_BL; // Get low nibble
	else
		lcddata = LO_NIBBLE(data)|LCD_RS|LCD_BL; // Get low nibble
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
		if(i>DISPLAY_TYPE)break;
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
		case DISPLAY_LINE_3:
			LCD_goto(3,1);//go_to(17);
			print_lcd((char*)"                ");//Blank);
			break;
		case DISPLAY_LINE_4:
			LCD_goto(4,1);//go_to(17);
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
	int indx=0;int len=0;
	if(strlen(string)>DISPLAY_TYPE)
		len=DISPLAY_TYPE;
	else
		len=strlen(string);
	indx=((DISPLAY_TYPE-len)/2)+1;
	clear_display_internal(line);
	if(line==DISPLAY_LINE_1)
		LCD_goto(1,indx);
	else if(line==DISPLAY_LINE_2)
		LCD_goto(2,indx);
	else if(line==DISPLAY_LINE_3)
		LCD_goto(3,indx);
	else if(line==DISPLAY_LINE_4)
		LCD_goto(4,indx);
	print_lcd(string);
}
/*****************************************************************************/
//interface functions
RPC_SRV_RESULT I2CPcfLcd::print_line(char* msg,DISPLAY_LINE line,TEXT_ALIGNMENT align)
{
	int len=strlen(msg);int indx=0;int myline=0;
	if(len>DISPLAY_TYPE)
		len=DISPLAY_TYPE;
	indx=DISPLAY_TYPE-len;

	if(line==DISPLAY_LINE_1)
		myline=1;
	else if(line==DISPLAY_LINE_2)
		myline=2;
	else if(line==DISPLAY_LINE_3)
		myline=3;
	else if(line==DISPLAY_LINE_4)
		myline=4;
	else
		myline=1;

	switch(align)
	{
		case TEXT_ALIGNMENT_LEFT  :LCD_goto(myline,1);print_lcd(msg);break;
		case TEXT_ALIGNMENT_RIGHT :LCD_goto(myline,indx+1);print_lcd(msg);break;//TODO
		default:print_center(line,msg);break;
	}
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT I2CPcfLcd::set_back_light(bool sts)
{
	//uint8_t lcddata;
	//if(read_byte((uint32_t)LCD_PCF_ADDRESS, &lcddata)!=RPC_SRV_RESULT_SUCCESS)
	//	{printf("failed to read lcd pcf\n");return RPC_SRV_RESULT_FAIL;}
	//if(sts==true)
	//	{write_byte((uint32_t)LCD_PCF_ADDRESS,lcddata | 0x80);BkLight=true;}
	//else
	//	{write_byte((uint32_t)LCD_PCF_ADDRESS,lcddata & 0x7F);BkLight=false;}
	//return RPC_SRV_RESULT_SUCCESS;

	unsigned char lcddata;
	if(sts==false)
		lcddata = HI_NIBBLE(0);
	else
		lcddata = HI_NIBBLE(0)|LCD_BL;
	BkLight=sts;
	write_byte((uint32_t)LCD_PCF_ADDRESS,lcddata);
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT I2CPcfLcd::get_back_light(bool &sts)
{
	sts=BkLight;
	//uint8_t lcddata;
	//if(read_byte((uint32_t)LCD_PCF_ADDRESS, &lcddata)!=RPC_SRV_RESULT_SUCCESS)
	//	return RPC_SRV_RESULT_FAIL;
	//if((lcddata | 0x7F) == 0x7F)
	//	sts=false;
	//else
	//	sts=true;
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/

