#ifndef __I2C_GROVE_RGB_LCD_H_
#define __I2C_GROVE_RGB_LCD_H_

#define LCD_ADDRESS     (0x7c>>1)
#define RGB_ADDRESS     (0xc4>>1)

#include <stdint.h>
#include "I2CBusAccess.h"
// color define 
#define WHITE           0
#define RED             1
#define GREEN           2
#define BLUE            3

#define REG_RED         0x04        // pwm2
#define REG_GREEN       0x03        // pwm1
#define REG_BLUE        0x02        // pwm0

#define REG_MODE1       0x00
#define REG_MODE2       0x01
#define REG_OUTPUT      0x08

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00


class I2CGroveRgbLcd : public I2CBusAccess
{
	uint8_t _displaymode;
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
	void begin(uint8_t cols, uint8_t lines, uint8_t dotsize);
public:
	I2CGroveRgbLcd(std::string DevNode);
	~I2CGroveRgbLcd();
};
#endif

