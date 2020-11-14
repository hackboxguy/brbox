//disclaimer: code snippet is from: https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/#full-example
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <assert.h>
#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
/*****************************************************************************/
int open_port(char *serial_node)
{
    int fd; /* File descriptor for the port */
    fd = open(serial_node, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
        printf("open_port: Unable to open /dev/ttyf1 - ");
    else
        fcntl(fd, F_SETFL, 0);
    return (fd);
}
/*****************************************************************************/
int exit_serial_port(int fd,struct termios* old_options)
{
    if (tcsetattr(fd, TCSANOW, old_options) != 0) 
    {
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
		return -1;
	}
    return 0;
}
/*****************************************************************************/
int init_serial_port(int fd, struct termios* old_options,int baudrate)
{
	int error;
	struct termios tty;
	fcntl( fd, F_SETFL, FASYNC );

	// Get the current options for the port...
	tcgetattr( fd, old_options );
	memset(&tty, 0, sizeof(tty));  /* clear the new struct */

	tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
	tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
	tty.c_cflag |= CS8; // 8 bits per byte (most common)
	tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
	tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO; // Disable echo
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL; // Disable new-line echo
	tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

	tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	tty.c_cc[VMIN] = 0;

	// Set in/out baud rate to be 9600
	cfsetispeed(&tty, baudrate);//B9600);
	cfsetospeed(&tty, baudrate);//B9600);

	// Save tty settings, also checking for error
	if (tcsetattr(fd, TCSANOW, &tty) != 0) {
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
		return -1;
	}
}
void usage(const char *program)
{
    printf("Usage: %s ", program);
    printf("[-d <dev_node>] [-n <relay_pos>] [-s <on/off>] ");
    printf("    -d - usb-relay-device-node\n");
    printf("    -n - relay position\n");
    printf("    -s - relay state [on/off]\n");
    printf("    e.g. %s -d /dev/ttyUSB0 -n 1 -s on\n",program);
    //exit(EXIT_FAILURE);
}
/*****************************************************************************/
int main(int argc, char *argv[])
{
	struct termios old_options;	
	unsigned char buffer[16];
    const char* program = "usb-relay-ctl";//basename(argv[0]);
    char dev_node[256];
    char relay_state[256];
    int relay_pos;
    int opt = 0;
    while ((opt = getopt(argc, argv, "d:n:s")) != -1)
    {
        switch(opt)
        {
            case 'd':
                strcpy(dev_node,optarg);
                break;
            case 'n':
                relay_pos = atoi(optarg);
                break;
            case 's':
                strcpy(relay_state,optarg);
                break;
            default:
                usage(program);
                break;
        }
    }

	buffer[0]=0XA0;
	buffer[1]=0x01;
    if(strcmp(relay_state,"on"))
    {	
        buffer[2]=0x01;	
        buffer[3]=0xA2;	
    }
    else
    {
        buffer[2]=0x00;	
        buffer[3]=0xA1;	
    }

	int serial_fd=open_port("/dev/ttyUSB0");	
	if(serial_fd == -1 )
        return -1;
	init_serial_port(serial_fd,&old_options,B9600);
	write(serial_fd,buffer,4);	
    exit_serial_port(serial_fd,&old_options);
  	
    close(serial_fd);	
	return 0;
}
/*****************************************************************************/
