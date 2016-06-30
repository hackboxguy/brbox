//  BeagleBone Black {Debian}
//  Title   : I2C control of SSD1306 128x64 OLED display
//  Author  : Richard St-Pierre
//  Version : 1.0 Aug 5, 2014 Initial version


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>

// bitmapped image files for oled
			
#include "box128x64.h"                  // Outer perimeter box
#include "Q_NPN.h"                      // NPN Transistor
#include "Android.h"                    // Android
#include "Inspire.h"                    // Inspire

#define I2C_DEVNODE "/dev/i2c-8"
#define I2C_DEVID  (0x78 >>1);		// Translate Address
#define CMD_COMMAND   	0x80		// Single command
#define CMD_CONT_CMD   	0x00		// Continous command
#define CMD_DATA   	0x40		// Single Data
#define CMD_CONT_DATAT 	0xC0		// Continuous Data
#define CMD_DISP_ON 	0xA4		//
#define CMD_DISP_OFF 	0xAE		//
#define CMD_REVERSE	0xA1		//
#define CMD_NORMAL	0xA0		//
#define CMD_CONTRAST	0x81		//parameter 00-FF
#define CMD_SETPAGE	0x22		//

// Global variables
unsigned char  	buffer[4];
int 		i2c_oled_fd;
int		deviceI2CAddress = I2C_DEVID;

// Function protptyping
void send_command(unsigned char cmd);
void send_dat(unsigned  char dat);


// ============================================
//         M   A   I   N
// ============================================
int main (argc, argv)
        int argc;
        char *argv[];
         {
 	int i;
	printf("\nBeagleBone I2C OLED DISPLAY - RSTP");

	i2c_oled_fd  = open(I2C_DEVNODE, O_RDWR);

	if (i2c_oled_fd  < 0) {
		printf("\nError opening file: %s\n","/dev/i2c-8");
		return 1;
	}
	if (ioctl(i2c_oled_fd, I2C_SLAVE, deviceI2CAddress) < 0) {
		printf("\nError ioctl");
		return 1;
	}
	// Power up Display

	send_command(0x8D);
	send_command(0x14);

	send_command(0xAF);		// Display ON
	usleep(100000);
	sleep(1);

	printf("\nSending Display data\n");
	send_command(0x20);		// Memory addressing mode
	send_command(0x00);		// Horizontal mode
	sleep(1);

for (;;)
    {
     for ( i=0; i < 128*8; i++) {
	send_dat(box128x64[i]);
	}
	sleep(2);
	
	for ( i=0; i < 128*8; i++) {
	send_dat(Q_NPN[i]) ;
        }
	sleep(2);

	for ( i=0; i < 128*8; i++) {
	send_dat(Android[i]) ;
        }
	sleep(2);

	for ( i=0; i < 128*8; i++) {
	send_dat(Inspire[i]) ;
        }
	sleep(2);
    }	
	


	close(i2c_oled_fd);

	return 0;
}
//========================================
void send_command(unsigned char cmd)
{
unsigned char buf[3];

	buf[0]=0x80;
	buf[1]=cmd;
	write(i2c_oled_fd,buf,2);
	printf(" %02X%02X", buf[0],buf[1]);

}
void send_dat(unsigned char dat)
{
	buffer[0]=0x40;
	buffer[1]=dat;
	write(i2c_oled_fd,buffer,2);
}

