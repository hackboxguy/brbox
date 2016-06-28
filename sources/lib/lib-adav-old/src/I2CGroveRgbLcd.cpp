#include "I2CGroveRgbLcd.hpp"
#include <unistd.h>
const unsigned char color_define[4][3] = 
{
    {255, 255, 255},            // white
    {255, 0, 0},                // red
    {0, 255, 0},                // green
    {0, 0, 255},                // blue
};
/*****************************************************************************/
I2CGroveRgbLcd::I2CGroveRgbLcd(std::string DevNode):I2CBusAccess(DevNode)
{
}
I2CGroveRgbLcd::~I2CGroveRgbLcd()
{
}
/*****************************************************************************/
void I2CGroveRgbLcd::i2c_send_byte(unsigned char dta)
{
	//Wire.beginTransmission(LCD_ADDRESS);        // transmit to device #4
	//Wire.write(dta);                            // sends five bytes
	//Wire.endTransmission();                     // stop transmitting
	write_byte((uint32_t)LCD_ADDRESS,dta);
}
/*****************************************************************************/
void I2CGroveRgbLcd::i2c_send_byteS(unsigned char *dta, unsigned char len)
{
	// Wire.beginTransmission(LCD_ADDRESS);        // transmit to device #4
	// for(int i=0; i<len; i++)
	// {
	//     Wire.write(dta[i]);
	// }
	// Wire.endTransmission();                     // stop transmitting
}
/*****************************************************************************/
inline void I2CGroveRgbLcd::command(uint8_t value)
{
	unsigned char dta[2] = {0x80, value};
	i2c_send_byteS(dta, 2);
}
/*****************************************************************************/
inline unsigned int I2CGroveRgbLcd::write(uint8_t value)
{
	unsigned char dta[2] = {0x40, value};
	i2c_send_byteS(dta, 2);
	return 1; // assume sucess
}
/*****************************************************************************/
void I2CGroveRgbLcd::setReg(unsigned char addr, unsigned char dta)
{
	// Wire.beginTransmission(RGB_ADDRESS); // transmit to device #4
	// Wire.write(addr);
	// Wire.write(dta);
	// Wire.endTransmission();    // stop transmitting
	write_byte((uint32_t)RGB_ADDRESS,dta);
}
/*****************************************************************************/
void I2CGroveRgbLcd::setRGB(unsigned char r, unsigned char g, unsigned char b)
{
	setReg(REG_RED, r);
	setReg(REG_GREEN, g);
	setReg(REG_BLUE, b);
}
/*****************************************************************************/
void I2CGroveRgbLcd::setColor(unsigned char color)
{
	if(color > 3)return ;
	setRGB(color_define[color][0], color_define[color][1], color_define[color][2]);
}
/*****************************************************************************/
// Control the backlight LED blinking
void I2CGroveRgbLcd::blinkLED(void)
{
	// blink period in seconds = (<reg 7> + 1) / 24
	// on/off ratio = <reg 6> / 256
	setReg(0x07, 0x17);  // blink every second
	setReg(0x06, 0x7f);  // half on, half off
}
/*****************************************************************************/
void I2CGroveRgbLcd::noBlinkLED(void)
{
	setReg(0x07, 0x00);
	setReg(0x06, 0xff);
}
/*****************************************************************************/
// This will 'left justify' text from the cursor
void I2CGroveRgbLcd::noAutoscroll(void)
{
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}
/*****************************************************************************/
// Allows us to fill the first 8 CGRAM locations
// with custom characters
void I2CGroveRgbLcd::createChar(uint8_t location, uint8_t charmap[])
{
	location &= 0x7; // we only have 8 locations 0-7
	command(LCD_SETCGRAMADDR | (location << 3));
	unsigned char dta[9];
	dta[0] = 0x40;
	for(int i=0; i<8; i++)
	{
		dta[i+1] = charmap[i];
	}
	i2c_send_byteS(dta, 9);
}
/*****************************************************************************/
void I2CGroveRgbLcd::scrollDisplayRight(void)
{
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}
/*****************************************************************************/
// This is for text that flows Left to Right
void I2CGroveRgbLcd::leftToRight(void)
{
	_displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}
/*****************************************************************************/
// This is for text that flows Right to Left
void I2CGroveRgbLcd::rightToLeft(void)
{
	_displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}
/*****************************************************************************/
// This will 'right justify' text from the cursor
void I2CGroveRgbLcd::autoscroll(void)
{
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}
/*****************************************************************************/
// Turns the underline cursor on/off
void I2CGroveRgbLcd::noCursor()
{
	_displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
/*****************************************************************************/
void I2CGroveRgbLcd::cursor() 
{
	_displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
/*****************************************************************************/
// Turn on and off the blinking cursor
void I2CGroveRgbLcd::noBlink()
{
	_displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
/*****************************************************************************/
void I2CGroveRgbLcd::blink()
{
	_displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
/*****************************************************************************/
// These commands scroll the display without changing the RAM
void I2CGroveRgbLcd::scrollDisplayLeft(void)
{
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
/*****************************************************************************/
void I2CGroveRgbLcd::clear()
{
	command(LCD_CLEARDISPLAY);        // clear display, set cursor position to zero
	usleep(2000);
}
/*****************************************************************************/
void I2CGroveRgbLcd::home()
{
	command(LCD_RETURNHOME);        // set cursor position to zero
	usleep(2000);
}
/*****************************************************************************/
void I2CGroveRgbLcd::setCursor(uint8_t col, uint8_t row)
{
	col = (row == 0 ? col|0x80 : col|0xc0);
	unsigned char dta[2] = {0x80, col};
	i2c_send_byteS(dta, 2);
}
/*****************************************************************************/
// Turn the display on/off (quickly)
void I2CGroveRgbLcd::noDisplay()
{
    _displaycontrol &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
/*****************************************************************************/
void I2CGroveRgbLcd::display() 
{
    _displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
/*****************************************************************************/
void I2CGroveRgbLcd::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) 
{
	//Wire.begin();
	if (lines > 1) 
	{
		_displayfunction |= LCD_2LINE;
	}
	_numlines = lines;
	_currline = 0;

	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != 0) && (lines == 1)) 
	{
		_displayfunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	usleep(50000);


	// this is according to the hitachi HD44780 datasheet
	// page 45 figure 23

	// Send function set command sequence
	command(LCD_FUNCTIONSET | _displayfunction);
	usleep(4500);

	// second try
	command(LCD_FUNCTIONSET | _displayfunction);
	usleep(150);

	// third go
	command(LCD_FUNCTIONSET | _displayfunction);

	// finally, set # lines, font size, etc.
	command(LCD_FUNCTIONSET | _displayfunction);

	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	display();

	// clear it off
	clear();

	// Initialize to default text direction (for romance languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	// set the entry mode
	command(LCD_ENTRYMODESET | _displaymode);

	// backlight init
	setReg(REG_MODE1, 0);
	// set LEDs controllable by both PWM and GRPPWM registers
	setReg(REG_OUTPUT, 0xFF);
	// set MODE2 values
	// 0010 0000 -> 0x20  (DMBLNK to 1, ie blinky mode)
	setReg(REG_MODE2, 0x20);
	setColorWhite();
}
/*****************************************************************************/

