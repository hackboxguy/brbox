#ifndef __I2C_SSD_1306_H_
#define __I2C_SSD_1306_H_
#define DISPLAY_I2C_ADDR   (0x3C)
#define WIDTH     128          // 128 columns of 8bits deep
#define HEIGHT    8            // 4 rows, 8 bits high
#define TILE_SIZE 8
#define STEP_SIZE 16
#include <stdint.h>
#include "DisplayDevice.hpp"
class I2CSsd1306 : public DisplayDevice //, public I2CBusAccess
{
	/*uint8_t io_ctrl_byte;
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
	void LCD_goto(unsigned char row, unsigned char column);*/

	uint8_t *screenBuf_;
	uint8_t *data_;
	uint8_t *tmp_;
	//I2C *i2c_;

	void clearBuffer ();
	void clearScreen ();
	bool writeScreen ();
	bool writeCenter (std::string text, uint8_t row);
	bool writeText (std::string text, uint8_t row, uint8_t col);
	bool writeCommand(uint8_t bytes, uint8_t byte1);
	bool writeCommand(uint8_t bytes, uint8_t byte1,uint8_t byte2);
	bool writeCommand(uint8_t bytes, uint8_t byte1,uint8_t byte2, uint8_t byte3);
public:
	I2CSsd1306(std::string DevNode,std::string DevType);
	~I2CSsd1306();
	//interface functions
	RPC_SRV_RESULT init_display();//{init_lcd();return RPC_SRV_RESULT_SUCCESS;};
	RPC_SRV_RESULT clear_display();//{clear_display_internal(DISPLAY_LINE_FULL);return RPC_SRV_RESULT_SUCCESS;};
	RPC_SRV_RESULT print_line(char* msg,DISPLAY_LINE line,TEXT_ALIGNMENT align);
};
#endif

