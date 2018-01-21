#ifndef __I2C_PCF_LCD_H_
#define __I2C_PCF_LCD_H_

#define LCD_PCF_DATA_ADDRESS     (0x70>>1)
#define LCD_PCF_ADDRESS     (0x4E>>1) //(0x72>>1) 0100 1110
#include <stdint.h>
//#include "I2CBusAccess.h"
#include "DisplayDevice.hpp"
class I2CPcfLcd : public DisplayDevice //, public I2CBusAccess
{
	int DISPLAY_TYPE;
	ADLIB_DISPLAY_TYPE disp_type;
	uint8_t io_ctrl_byte;
	void update_io_ctrl_device();
	void update_io_ctrl_cache(uint8_t location,uint8_t value);
	void write_inst(uint8_t byte);
	//void write_data(uint8_t byte);
	void go_to(uint8_t x);
	void go_to_centre(char *ptr,DISPLAY_LINE line);
	void clear_display_internal(DISPLAY_LINE line);
	void init_lcd(void);
	void print_lcd(char *ptr);
	void print_center(DISPLAY_LINE line,char *string);

	void write_inst(unsigned char data,unsigned char cmdtype);
	void write_data(unsigned char data);
	void LCD_goto(unsigned char row, unsigned char column);

public:
	I2CPcfLcd(std::string DevNode,std::string DevType);
	~I2CPcfLcd();
	//interface functions
	RPC_SRV_RESULT init_display(){init_lcd();return RPC_SRV_RESULT_SUCCESS;};
	RPC_SRV_RESULT clear_display(){clear_display_internal(DISPLAY_LINE_FULL);return RPC_SRV_RESULT_SUCCESS;};
	RPC_SRV_RESULT print_line(char* msg,DISPLAY_LINE line,TEXT_ALIGNMENT align=TEXT_ALIGNMENT_CENTER);
	RPC_SRV_RESULT set_back_light(bool sts);
	RPC_SRV_RESULT get_back_light(bool &sts);
};
#endif

