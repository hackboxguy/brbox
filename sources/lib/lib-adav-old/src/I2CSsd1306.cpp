#include "I2CSsd1306.hpp"
#include <unistd.h>
#include <string.h>
#include "glcdfont.h"
#include <math.h>
/*#define LCD_LINE1 0x80
#define LCD_LINE2 0xC0
#define LCD_LINE3 0x94
#define LCD_LINE4 0xD4
#define LO_NIBBLE(b) (((b) << 4) & 0xF0)
#define HI_NIBBLE(b) ((b) & 0xF0)
#define LCD_BL 0x08
#define LCD_EN 0x04
#define LCD_RW 0x02
#define LCD_RS 0x01*/
/*****************************************************************************/
I2CSsd1306::I2CSsd1306(std::string DevNode,std::string DevType):DisplayDevice(DevNode)//,I2CBusAccess(DevNode)
{
//	io_ctrl_byte=0xff;
//	init_lcd();
//	LCD_goto(1,1);
//	print_lcd("hello world");
	devNode=DevNode;
	devType=DevType;
	screenBuf_=NULL;
	data_=NULL;
	tmp_=NULL;
	ssoledinit=false;
	init_display();
}
I2CSsd1306::~I2CSsd1306()
{
	if(ssoledinit)
		oledPower(&ssoled, 0);
   if (screenBuf_ != NULL )
   {
      delete [] screenBuf_;
      screenBuf_ = 0;
   }
}
/*****************************************************************************/
void I2CSsd1306::clearBuffer ()
{
   if (screenBuf_)
      memset (screenBuf_, 0x00, (WIDTH * HEIGHT));
}
/*****************************************************************************/
void I2CSsd1306::clearScreen ()
{
	oledFill(&ssoled, 0,1);
//	clearBuffer(); // Clear internal buffer
//	writeScreen(); // Write cleared buffer to screen
}
/*****************************************************************************/
bool I2CSsd1306::writeScreen ()
{
	bool fOK=true;
	uint16_t uiIndex = 0x00;
	uint8_t uchTemp[0x09];

	// Need to set OLED screen pointer back to 0, 0 before writing
	// This was the only way I found to do this with the chip set commands.
	//fOK = i2c_->writeCommand (3, 0x21, 0x00, 0x7F); // Set column address; start 0, end 127
	//fOK = i2c_->writeCommand (3, 0x22, 0x00, 0x07); // Set row address; start 0, end 7
	uint8_t data[16];data[0]=0x00;data[1]=0x21;data[2]=0x00;data[3]=0x7F;
	write_array((uint32_t) DISPLAY_I2C_ADDR, data,4);
	data[0]=0x00;data[1]=0x22;data[2]=0x00;data[3]=0x07;
	write_array((uint32_t) DISPLAY_I2C_ADDR, data,4);

	while (uiIndex < (WIDTH * HEIGHT))
	{
		uchTemp[0] = 0x40;           // Co = 0; D/C# = 1 for data
		for (int i = 0; i < 8; i++)  // Copy 8 bytes to temp buffer
		uchTemp[i+1] = screenBuf_[uiIndex+i];
		//fOK = i2c_->writeBytes (uchTemp, 0x09); //Write control byte with 8 data bytes
		fOK = write_array((uint32_t) DISPLAY_I2C_ADDR, uchTemp,0x09);
		uiIndex += 0x08;             // Move to next Octet
	}
	return fOK;
}
/*****************************************************************************/
bool I2CSsd1306::writeCenter (std::string text, uint8_t row)
{
	//#define FONT_NORMAL FONT_8x8
	//#define FONT_SMALL FONT_6x8
	//#define FONT_LARGE FONT_16x32
	//#define FONT_STRETCHED FONT_16x16

	//note: ssd1306: 128pix width ==> FONT_NORMAL    ==> max 16chars wide
	//note: ssd1306: 128pix width ==> FONT_STRETCHED ==> max 8chars wide
	int center = (128 - (text.length()*8))/2;
	oledWriteString(&ssoled, 0,center,row,(char*)text.c_str(),FONT_NORMAL,0,1);
	return true;

	if ((text.length()*6) <127) // If there is not more than 1 space left, don't center
		// To center, multiply # of characters * 6 (5+space), subtract the last space, then divide by 2
		return writeText (text, row, (128 - ((text.length()*6)-1))/2);
	else
		return writeText (text, row, 0);
}
/*****************************************************************************/
bool I2CSsd1306::writeText (std::string text, uint8_t row, uint8_t col) //
{
	uint8_t uchI, uchJ;
	uint16_t uiIndex;     // Indexes in the screen buffer
	uiIndex = (row * WIDTH) + col;             // Set absolute start position in buffer
	if ((row >= HEIGHT) || (col >= WIDTH))     // If asked for something out of range, abandon
		return false;
	for (uchI = 0; uchI < text.length(); uchI++)  // Loop through each character in string
	{
		for (uchJ = 0; uchJ < 5; uchJ++)             // Loop through each 7bit segment of the character
		{
			screenBuf_[uiIndex] = font[(text[uchI]*5)+uchJ]; // Trial and error!
			uiIndex ++;                              // Move to next buffer position
		}
		screenBuf_[uiIndex] = 0x00;                  // This puts a space between the characters on screen
		uiIndex++;
	}
	return true;
}
/*****************************************************************************/
RPC_SRV_RESULT I2CSsd1306::init_display()	
{
	return init_display_new();
	bool retval = false;
	screenBuf_ = new uint8_t[WIDTH * HEIGHT]; // Create a block of memory for the screen buffer
	memset(screenBuf_, 0, (WIDTH * HEIGHT));
	if (!screenBuf_)
	{
		//#ifdef DEBUG
		//cerr << "could not create screenBuf buffer" << endl;
		//#endif
		return RPC_SRV_RESULT_MEM_ERROR;//retval;
	}

	data_ = new uint8_t[TILE_SIZE];
	memset(data_, 0, TILE_SIZE);
	if (!data_)
	{
		//#ifdef DEBUG
		//cerr << "could not create data buffer" << endl;
		//#endif
		return RPC_SRV_RESULT_MEM_ERROR;
	}

	tmp_ = new uint8_t[TILE_SIZE];
	memset(tmp_, 0, TILE_SIZE);
	if (!tmp_)
	{
		//#ifdef DEBUG
		//cerr << "could not create tmp buffer" << endl;
		//#endif
		return RPC_SRV_RESULT_MEM_ERROR;
	}
	retval &= writeCommand (1, 0xAE);             // Display off
	retval &= writeCommand (2, 0xD5, 0x80);       // set display clock division
	retval &= writeCommand (2, 0xA8, 0x3F);       // set multiplex
	retval &= writeCommand (2, 0xD3, 0x00);       // set display offset
	retval &= writeCommand (1, 0x40);             // set start line #0
	retval &= writeCommand (2, 0x8D, 0x14);       // set charge pump
	retval &= writeCommand (2, 0x20, 0x00);       // Memory mode
	retval &= writeCommand (1, 0xA1);             // Segremap(0xA0 = reset, 0xA1 = 127 = SEG0)
	retval &= writeCommand (1, 0xC8);             // Com scan dec (0xC0 = reset normal, 0xC8 = scan  from Com[n-1] - Com 0
	retval &= writeCommand (2, 0xDA, 0x12);       // Set com pins
	retval &= writeCommand (2, 0x81, 0xCF);       // Set contrast
	retval &= writeCommand (2, 0xD9, 0xF1);       // Set precharge
	retval &= writeCommand (2, 0xDB, 0x40);       // Set Vcom select
	retval &= writeCommand (1, 0xA4);             // Resume RAM content display
	retval &= writeCommand (1, 0xA6);             // Normal display not inverted
	retval &= writeCommand (1, 0x00);             // low col=0
	retval &= writeCommand (1, 0x10);             // high col=0
	retval &= writeCommand (1, 0x40);             // line #0
	retval &= writeCommand (1, 0xAF);             // Display ON
	retval &= writeCommand (3, 0x21, 0x00, 0x7F); // Set column address; start 0, end 127
	retval &= writeCommand (3, 0x22, 0x00, 0x07); // Set row address; start 0, end 7
	retval &= writeCommand (1, 0xAF);             // Display ON
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
bool I2CSsd1306::writeCommand(uint8_t bytes, uint8_t byte1)
{
	uint8_t data[16];data[0]=0x00;data[1]=byte1;
	write_array((uint32_t) DISPLAY_I2C_ADDR, data,2);
	return true;
}
bool I2CSsd1306::writeCommand(uint8_t bytes, uint8_t byte1,uint8_t byte2)
{
	uint8_t data[16];data[0]=0x00;data[1]=byte1;data[2]=byte2;
	write_array((uint32_t) DISPLAY_I2C_ADDR, data,3);
	return true;
}
bool I2CSsd1306::writeCommand(uint8_t bytes, uint8_t byte1,uint8_t byte2, uint8_t byte3)
{
	uint8_t data[16];data[0]=0x00;data[1]=byte1;data[2]=byte2;data[3]=byte3;
	write_array((uint32_t) DISPLAY_I2C_ADDR, data,4);
	return true;
}
uint8_t I2CSsd1306::reverseByte (uint8_t b)
{
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}
bool I2CSsd1306::writeImage (uint8_t* image)
{
	for (int lcv=0; lcv<(WIDTH * HEIGHT); lcv++)
	{
		getTileFromBitmap(lcv);
		// rotate the 8x8 tile
		rotateTile();
		//#ifdef DEBUG
		//cout << "==============temp buffer===========" << endl;
		//#endif
		// reverse the bites in the byte
		for (int i=0; i<TILE_SIZE; i++)
		{
			tmp_[i] = reverseByte(tmp_[i]);
			//#ifdef DEBUG
			//cout << hex << uppercase << "0x" << (int)tmp_[i] << endl;
			//#endif
		}
		setTileInBuffer(lcv);
		// FIXME: we can probably skip to the next octect instead of
		//        doing this for every byte!
	}
	return false;
}
void I2CSsd1306::getTileFromBitmap (int index)
{
	int step = 0;
	for (int i=0; i<TILE_SIZE; i++, step+=STEP_SIZE)
	{
		data_[i] = screenBuf_[index+step];
		//#if DEBUG
		//cout << "getting byte at index: " << dec << index+step << " with value: "
		//<< hex << uppercase << "0x" << (int)screenBuf_[index+step] << endl;
		//#endif
	}
}
void I2CSsd1306::rotateTile()
{
	// process bits for a bytes in data
	for (int i=0; i<TILE_SIZE; i++)
	{
		//#ifdef DEBUG
		//cout << " process bit " << dec << i << endl;
		//#endif
		// process data bytes
		for (int j=0; j<TILE_SIZE; j++)
		{
			//#ifdef DEBUG
			//cout << " anding data 0x" << hex << uppercase << (int)data_[j]
			//<< " with 2 to the " << 7-i << endl;
			//#endif
			uint8_t temp = data_[j] & (uint8_t)pow(2, 7-i);
			//#ifdef DEBUG
			//cout << "\ttemp before shift: " << hex << uppercase << "0x" << (int)temp << endl;
			//#endif
			int shift = 7-i-j;
			uint8_t shifted = 0x0;
			if (shift < 0)
			{
			shift *= -1;
				//#ifdef DEBUG
				//cout << "\tshifting temp to the LEFT " << dec << shift << endl;
				//#endif
				shifted = temp << shift;
			}
			else
			{
				//#ifdef DEBUG
				//cout << "\tshifting temp to the RIGHT " << dec << shift << endl;
				//#endif
				shifted = temp >> shift;
			}
			//#ifdef DEBUG
			//cout << "\tshifted temp value: " << hex << uppercase << "0x" << (int)shifted << endl;
			//cout << "\ttmp[" << dec << i << "] beofre OR: " << hex << uppercase << (int)tmp_[i] << endl;
			//#endif
			tmp_[i] |= shifted;
			//#ifdef DEBUG
			//cout << "\ttmp[" << dec << i << "] after OR: " << hex << uppercase << "0x" << (int)tmp_[i] << endl;
			//#endif
		}
		tmp_[i] = reverseByte(tmp_[i]);
	}
}
void I2CSsd1306::setTileInBuffer(int index)
{
	int step = 0;
	for (int i=0; i<TILE_SIZE; i++, step+=STEP_SIZE)
	{
		screenBuf_[index+step] |= tmp_[i];
		//#ifdef DEBUG
		//cout << "setting byte at index: " << dec << index+step << " with value: "
		//<< hex << uppercase << "0x" << (int)tmp_[i] << endl;
		//#endif
	}
}
/*****************************************************************************/
RPC_SRV_RESULT I2CSsd1306::clear_display()
{
	//clear_display_internal(DISPLAY_LINE_FULL);
	clearScreen ();
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT I2CSsd1306::print_line(char* msg,DISPLAY_LINE line,TEXT_ALIGNMENT align)
{
/*	switch(align)
	{
		case TEXT_ALIGNMENT_LEFT  :print_lcd(msg);break;
		case TEXT_ALIGNMENT_RIGHT :print_lcd(msg);break;//TODO
		default:print_center(line,msg);break;//LCD_goto(1,1);print_lcd(msg);break//;break;
	}*/
	std::string str=msg;
	switch (line)
	{
		case DISPLAY_LINE_1:
			writeCenter(str,0);
			break;
		case DISPLAY_LINE_2:
			writeCenter(str,1);
			break;
		case DISPLAY_LINE_3:
			writeCenter(str,2);
			break;
		case DISPLAY_LINE_4:
			writeCenter(str,3);
			break;
		case DISPLAY_LINE_5:
			writeCenter(str,4);
			break;
		case DISPLAY_LINE_6:
			writeCenter(str,5);
			break;
		case DISPLAY_LINE_7:
			writeCenter(str,6);
			break;
		case DISPLAY_LINE_8:
			writeCenter(str,7);
			break;
		default://arg error
			return RPC_SRV_RESULT_ARG_ERROR;
			break;
	}
	//writeScreen();

	//writeCenter(str,0);//"hello-world1",0);
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT I2CSsd1306::set_back_light(bool sts)
{
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT I2CSsd1306::get_back_light(bool &sts)
{
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT I2CSsd1306::init_display_new()
{
	if(ssoledinit)
		return RPC_SRV_RESULT_SUCCESS;
	const char *table[]   = ADLIB_DISPLAY_TYPE_TABL;

	int i=-1, iChannel=0;
	int iOLEDAddr = -1; // typical address; it can also be 0x3d
	int iOLEDType = OLED_128x32; // Change this for your specific display
	int bFlip = 1, bInvert = 0;

	if(strcmp(devType.c_str(),table[ADLIB_DISPLAY_TYPE_SSD1306_128x32]) == 0)
		iOLEDType = OLED_128x32;
	else if(strcmp(devType.c_str(),table[ADLIB_DISPLAY_TYPE_SSD1306_128x64]) == 0)
		iOLEDType = OLED_128x64;
	else
		iOLEDType = OLED_128x32;

    std::string node(devNode);
    int position = node.find("i2c-");
    std::string value = node.substr(position+4);//(devNode.length()-position)+3);
	cout<<"node:"<<node<<":pos="<<position<<" : dev-node="<<value<<" : type="<<iOLEDType<<endl;
	iChannel = std::stoi(value);

	i=oledInit(&ssoled, iOLEDType, iOLEDAddr, bFlip, bInvert, 1, iChannel, iOLEDAddr, -1, 400000);
	if ( i != OLED_NOT_FOUND )
	{
		oledSetBackBuffer(&ssoled, ucBackBuf);
		oledFill(&ssoled, 0,1);
		ssoledinit=true;
		return RPC_SRV_RESULT_SUCCESS;
	}
	else
		return RPC_SRV_RESULT_FAIL;
}
/*****************************************************************************/


