#ifndef __I2C_DUAL_PCF_LCD_H_
#define __I2C_DUAL_PCF_LCD_H_

#define LCD_PCF_DATA_ADDRESS     (0x70>>1)
#define LCD_PCF_CTRL_ADDRESS     (0x72>>1)
#include <stdint.h>
#include "I2CBusAccess.h"
typedef enum LCD_DISP_LINE_T
{
	LCD_DISP_LINE_1,
	LCD_DISP_LINE_2,
	LCD_DISP_LINE_FULL,
	LCD_DISP_LINE_UNKNOWN,
	LCD_DISP_LINE_NONE
}LCD_DISP_LINE;
class I2CDualPcfLcd : public I2CBusAccess
{
	uint8_t io_ctrl_byte;
	void update_io_ctrl_device();
	void update_io_ctrl_cache(uint8_t location,uint8_t value);
	void write_inst(uint8_t byte);
	void write_data(uint8_t byte);
	void go_to(uint8_t x);
	void go_to_centre(char *ptr,LCD_DISP_LINE line);


	/*uint8_t _displaymode;
	uint8_t _displaycontrol;
	uint8_t _displayfunction;
	uint8_t _currline,_numlines;
	void i2c_send_byte(unsigned char dta);
	void i2c_send_byteS(unsigned char *dta, unsigned char len);
	inline void command(uint8_t value);
	inline unsigned int write(uint8_t value);
	void setReg(unsigned char addr, unsigned char dta);
	void setRGB(unsigned char r, unsigned char g, unsigned char b);
	void setColor(unsigned char color);
	void blinkLED(void);
	void noBlinkLED(void);
	void noAutoscroll(void);
	void createChar(uint8_t location, uint8_t charmap[]);
	void scrollDisplayRight(void);
	void leftToRight(void);
	void rightToLeft(void);
	void autoscroll(void);
	void noCursor();
	void cursor();
	void noBlink();
	void blink();
	void scrollDisplayLeft(void);
	void clear();
	void home();
	void setCursor(uint8_t col, uint8_t row);
	void noDisplay();
	void display();
	void setColorAll(){setRGB(0, 0, 0);}
	void setColorWhite(){setRGB(255, 255, 255);}
	void begin(uint8_t cols, uint8_t lines, uint8_t dotsize);*/
public:
	I2CDualPcfLcd(std::string DevNode);
	~I2CDualPcfLcd();
	void init_lcd(void);
	void print_lcd(char *ptr);
	void clear_display(LCD_DISP_LINE line);
	void print_center(LCD_DISP_LINE line,char *string);
};
#endif

