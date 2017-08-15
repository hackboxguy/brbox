/*
 * offset.c
 *
 * Author: Tomi Valkeinen <tomi.valkeinen@nokia.com>
 * Copyright (C) 2009-2012 Tomi Valkeinen

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <linux/fb.h>

#define ERROR(x) printf("fbtest error in line %s:%d: %s\n", __FUNCTION__, __LINE__, strerror(errno));

#define FBCTL(cmd, arg)			\
	if(ioctl(fd, cmd, arg) == -1) {	\
		ERROR("ioctl failed");	\
		exit(1); }

#define FBCTL0(cmd)			\
	if(ioctl(fd, cmd) == -1) {	\
		ERROR("ioctl failed");	\
		exit(1); }



int main(int argc, char** argv)
{
	char str[64];
	int fd;
	struct fb_var_screeninfo var;

	int opt;
	int req_fb = 0;

	printf("offset %d.%d.%d (%s)\n", VERSION, PATCHLEVEL, SUBLEVEL,
		VERSION_NAME);

	while ((opt = getopt(argc, argv, "f:")) != -1) {
		switch (opt) {
		case 'f':
			req_fb = atoi(optarg);
			break;
		default:
			printf("usage: %s [-f <fbnum>] <x> <y>\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if (argc - optind != 2) {
		printf("usage: %s [-f <fbnum>] <x> <y>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	sprintf(str, "/dev/fb%d", req_fb);
	fd = open(str, O_RDWR);

	FBCTL(FBIOGET_VSCREENINFO, &var);
	var.xoffset = atoi(argv[optind + 0]);
	var.yoffset = atoi(argv[optind + 1]);
	FBCTL(FBIOPUT_VSCREENINFO, &var);

	return 0;
}
