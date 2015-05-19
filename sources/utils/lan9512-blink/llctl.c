/*
 * llctl.c
 *
 *  LAN LED control
 *  
 */

#include <stdio.h>
#include <string.h>
#include <usb.h>
#include <stdlib.h>
#include <stdbool.h>

#define USB_CTRL_SET_TIMEOUT    5000
#define NET_VENDOR_ID		0x0424
#define	NET_PRODUCT_ID		0xec00

#define GPIDX			0
#define DUPIDX                  0
#define LNKIDX                  1
#define SPDIDX                  2

// these values make LEDs reflect LAN status
#define DUP_LED			0x001
#define LNK_LED                 0x010
#define SPD_LED			0x100

// these values are for controlling LEDs individually 
#define DUP_LED_OVR             0x1
#define LNK_LED_OVR             0x2
#define SPD_LED_OVR             0x4

// bitmask for configuration register
#define DUP_MASK		DUP_LED << 16 | DUP_LED_OVR << 4
#define LNK_MASK		LNK_LED << 16 | LNK_LED_OVR << 4
#define SPD_MASK		SPD_LED << 16 | SPD_LED_OVR << 4

#define MODE_OFF		0
#define MODE_ON			1
#define MODE_STATUS		2
#define MODE_TOGGLE		3
#define MODE_KEEP		15
#define MODE_WRITE		16
#define MODE_READ		17
#define MODE_ERR		-1

char* substring(const char* str, int begin, int len)
{
	if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin+len))
		return 0;

	return strndup(str + begin, len);
}

void usage()
{
	printf("Usage: llctl r | w<rawvalue> | [ f<mod> ] [ l<mod> ] [ d<mod> ]\n\n");
	printf("r: read content of configuration register other parameters are ignored after 'r'\n");
	printf("w: write <rawvalue> to configuration register (see README) other parameters are ignored after 'w'\n");
	printf("f, l, d select the fast ethernet, link, duplex status LED\n");
	printf("<mod> can be  0, 1, s, t\n");
	printf("  0: switch off LED\n");
	printf("  1: switch on LED\n");
	printf("  s: show status as designed for this LED\n");
	printf("  t: toggle LED. If LED was set to 's' before, 't' does nothing\n");
	exit(1);
}

void show_err(const char* errstr)
{
	printf("Error: %s\n\n", errstr);
	usage();
}

int ledmode(const char* str)
{
	int mode;
	mode = MODE_ERR;
	if ( *str == '0' )
		mode = MODE_OFF;
	if ( *str == '1' )
		mode = MODE_ON;
	if ( *str == 's' )
		mode = MODE_STATUS;
	if ( *str == 't' )
		mode = MODE_TOGGLE;
	return(mode);
}

int parse_cmdline(const char* str, int *cmd_arr)
{
// parse command line
// these commands are valid:
// w<value>
// write a raw value to the configuration register, value is expected to be hex
// r
// read the configuration register and print on screen
//
// commands consisting of two characters
// the first character stands for the LED
// d: duplex LED
// l: link LED
// f: fast etherne LED
// the second character is the command
// 0: switch off LED
// 1: switch on LED
// t: toggle LED (valid only if former status was 0 or 1, otherwise mode is not changed
// s: LED reflects one LAN parameter

	int index;
	index = -1;

	if ( strlen(str) == 1 && *str == 'r' )
	{
		return(MODE_READ);
	}

	if ( strlen(str) >= 2 && *str == 'w' )
	{
		index = GPIDX;
		cmd_arr[index] = strtol(str+1,NULL,16);
		return(MODE_WRITE);
	}

	if ( strlen(str) != 2 )
	{
		show_err("Argument is wrong");
	} else {
		if ( *str == 'd' )
			index = DUPIDX;
		if ( *str == 'l' )
			index = LNKIDX;
		if ( *str == 'f' )
			index = SPDIDX;
		cmd_arr[index] = ledmode(str+1);
		return(index);
	}
}


unsigned int get_config(usb_dev_handle *devh, bool output)
{
	int ret;
	unsigned int config;
	ret = usb_control_msg(devh, 0xc0, 0xA1, 0, 0x24, (char*)&config, 4, 5000);
	if ( output )
		printf ("Config: %08x\n",config);
	return (config);
}

void write_config(usb_dev_handle *devh, unsigned int value)
{
	int ret;
	ret = usb_control_msg(devh, 0x40, 0xA0, 0, 0x24, (char*)&value, 4, 5000);
}

struct usb_device *get_usb_device(struct usb_bus *bus, struct usb_device *usbdev)
{
	usb_init();
	usb_find_busses();
	usb_find_devices();
	for (bus = usb_busses; bus; bus = bus->next)
	{
		for (usbdev = bus->devices; usbdev; usbdev = usbdev->next)
		{
			if ((usbdev->descriptor.idVendor == NET_VENDOR_ID) &&
			(usbdev->descriptor.idProduct == NET_PRODUCT_ID))
			{
				return(usbdev);
			}
		}
	}

}

int main(int argc, char *argv[])
{
	// at least one argument is needed
	if ( argc < 2 )
	{
		show_err("At least one argument is required");
	}

	struct usb_bus *bus;
	struct usb_device *usbdev;
	struct usb_device *lan9512;
	usb_dev_handle *devh;

	// array to store command for every LED
	// default is to keep the LED's status
	int cmd_arr[3] = { MODE_KEEP, MODE_KEEP, MODE_KEEP };

	// parse command line and handle read and raw value writes
	int i;
	for ( i = 1; i < argc; i++ )
	{
		int res;
		res = parse_cmdline(argv[i], cmd_arr);
		if ( res < 0 )
		{
			show_err("parse_cmdline return");
		} else if (res >= MODE_WRITE ) {
			lan9512 = get_usb_device(bus,usbdev);
			devh = usb_open(lan9512);
			
			if ( res == MODE_READ )
				get_config(devh, true);
			if ( res == MODE_WRITE )
				write_config(devh,cmd_arr[0]);
			usb_close(devh);
			exit(0);
		}
	}

	// handle other tasks
	unsigned int cur_config, new_config;

	// initialize mask array
	unsigned int mask_arr[3];
	mask_arr[DUPIDX] = DUP_MASK;
	mask_arr[LNKIDX] = LNK_MASK;
	mask_arr[SPDIDX] = SPD_MASK;

	lan9512 = get_usb_device(bus,usbdev);
	devh = usb_open(lan9512);
	cur_config = get_config(devh, false);

	// array for current LED status
	unsigned int current_arr[3];

	for ( i = 0; i < 3; i++ )
	{
		current_arr[i] = cur_config & mask_arr[i];
		// toggle is only allowed if current mode MODE_ON and MODE_OFF
		if ( cmd_arr[i] == MODE_TOGGLE && current_arr[i] < 65536 )
		{
			current_arr[i] = current_arr[i] ^ (mask_arr[i] & 0xffff);
		}
		if ( cmd_arr[i] == MODE_ON )
		{
			current_arr[i] = current_arr[i] & ( 0xffff0000 ^ mask_arr[i] );
			current_arr[i] = current_arr[i] | (mask_arr[i] & 0xffff);
		}
		if ( cmd_arr[i] == MODE_OFF )
		{
			current_arr[i] = current_arr[i] & ((mask_arr[i] & 0xffff) ^ 0xffff);
		}
		if ( cmd_arr[i] == MODE_STATUS )
		{
			current_arr[i] = current_arr[i] & ((mask_arr[i] & 0xffff) ^ 0xffff);
			current_arr[i] = current_arr[i] | (mask_arr[i] & 0xffff0000);
		}
		new_config = new_config | current_arr[i];
	}
	write_config(devh,new_config);
	usb_close(devh);	
	return(0);
}
