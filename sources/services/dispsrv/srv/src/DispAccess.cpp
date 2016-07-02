#include "DispAccess.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
using namespace std;
struct s_opts
{
        int oled;
        int verbose;
} ;
//s_opts opts = 
//{
//	OLED_ADAFRUIT_I2C_128x32, //OLED_ADAFRUIT_SPI_128x32, // Default oled
//	false   // Not verbose
//};
/*****************************************************************************/
DispAccess::DispAccess(std::string DevNode,std::string DevType):DisplayDevice(DevNode)
//DispAccess::DispAccess(std::string disptype)
{
	DispType=DevType;
	DispOpened=init_display();
}
/*****************************************************************************/
DispAccess::~DispAccess()
{
	//if(DevOpened)
	//	close(fd);
}
/*****************************************************************************/
RPC_SRV_RESULT DispAccess::init_display()
{
	//DispOpened=RPC_SRV_RESULT_FAIL;
	s_opts opts;opts.verbose=false;	
	if(DispType=="SSD1306_128x32_PI")
		opts.oled=OLED_ADAFRUIT_I2C_128x32;
	else if(DispType=="SSD1306_128x64_PI")
		opts.oled=OLED_ADAFRUIT_I2C_128x64;
	else
		opts.oled=OLED_ADAFRUIT_I2C_128x32;
        // SPI
        if (Display.oled_is_spi_proto(opts.oled))
        {
                // SPI change parameters to fit to your LCD
                if ( !Display.init(OLED_SPI_DC,OLED_SPI_RESET,OLED_SPI_CS, opts.oled) )
                        return RPC_SRV_RESULT_FAIL;//DispOpened=RPC_SRV_RESULT_FAIL;
        }
        else
        {
                // I2C change parameters to fit to your LCD
                if ( !Display.init(OLED_I2C_RESET,opts.oled) )
                        return RPC_SRV_RESULT_FAIL;//DispOpened=RPC_SRV_RESULT_FAIL;
        }
        Display.begin();
        Display.clearDisplay(); // clears the screen buffer

	//usleep(100000);usleep(100000);usleep(100000);usleep(100000);
        //Display.begin();
        //Display.clearDisplay(); // clears the screen buffer
	//DispOpened=RPC_SRV_RESULT_SUCCESS;
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
RPC_SRV_RESULT DispAccess::clear_display()
{
	if(DispOpened!=RPC_SRV_RESULT_SUCCESS)
		return DispOpened;
        Display.clearDisplay(); // clears the screen buffer
        Display.setTextSize(1);
        Display.setTextColor(WHITE);
        Display.setCursor(0,0);
	Display.display();
	Display.display();
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
//RPC_SRV_RESULT DispAccess::print_line(int line, char* msg)
RPC_SRV_RESULT DispAccess::print_line(char* msg,DISPLAY_LINE line,TEXT_ALIGNMENT align)
{
	int tmpline,tmppos=0;
	switch(line)
	{
		case DISPLAY_LINE_1:tmpline=0;break;
		case DISPLAY_LINE_2:tmpline=1;break;
		case DISPLAY_LINE_3:tmpline=2;break;
		case DISPLAY_LINE_4:tmpline=3;break;
		default:return RPC_SRV_RESULT_ARG_ERROR;
	}
        Display.setTextSize(1);
        Display.setTextColor(WHITE);
	//if(align==TEXT_ALIGNMENT_CENTER)
		tmppos=((21-strlen(msg))/2);
        Display.setCursor(tmppos*6,tmpline*8);//128x32=21char wide(8pix high)
        Display.print(msg);//"Hello, world!\n");
        Display.display();
	Display.display();
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/

