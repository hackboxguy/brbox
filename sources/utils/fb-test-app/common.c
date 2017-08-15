/*
 * common.c
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
#include <linux/kd.h>

#include "common.h"

void fb_open(int fb_num, struct fb_info *fb_info)
{
	char str[64];
	int fd,tty;

	tty = open("/dev/tty1", O_RDWR);

	if(ioctl(tty, KDSETMODE, KD_GRAPHICS) == -1)
		printf("Failed to set graphics mode on tty1\n");

	sprintf(str, "/dev/fb%d", fb_num);
	fd = open(str, O_RDWR);

	ASSERT(fd >= 0);

	fb_info->fd = fd;
	IOCTL1(fd, FBIOGET_VSCREENINFO, &fb_info->var);
	IOCTL1(fd, FBIOGET_FSCREENINFO, &fb_info->fix);

	printf("fb res %dx%d virtual %dx%d, line_len %d, bpp %d\n",
			fb_info->var.xres, fb_info->var.yres,
			fb_info->var.xres_virtual, fb_info->var.yres_virtual,
			fb_info->fix.line_length, fb_info->var.bits_per_pixel);

	void *ptr = mmap(0,
			fb_info->var.yres_virtual * fb_info->fix.line_length,
			PROT_WRITE | PROT_READ,
			MAP_SHARED, fd, 0);

	ASSERT(ptr != MAP_FAILED);

	fb_info->ptr = ptr;
}

static void fb_clear_area(struct fb_info *fb_info, int x, int y, int w, int h)
{
	int i = 0;
	int loc;
	char *fbuffer = (char *)fb_info->ptr;
	struct fb_var_screeninfo *var = &fb_info->var;
	struct fb_fix_screeninfo *fix = &fb_info->fix;

	for (i = 0; i < h; i++) {
		loc = (x + var->xoffset) * (var->bits_per_pixel / 8)
			+ (y + i + var->yoffset) * fix->line_length;
		memset(fbuffer + loc, 0, w * var->bits_per_pixel / 8);
	}
}

static void fb_put_char(struct fb_info *fb_info, int x, int y, char c,
		unsigned color)
{
	int i, j, bits, loc;
	unsigned char *p8;
	unsigned short *p16;
	unsigned int *p32;
	struct fb_var_screeninfo *var = &fb_info->var;
	struct fb_fix_screeninfo *fix = &fb_info->fix;

	for (i = 0; i < 8; i++) {
		bits = fontdata_8x8[8 * c + i];
		for (j = 0; j < 8; j++) {
			loc = (x + j + var->xoffset) * (var->bits_per_pixel / 8)
				+ (y + i + var->yoffset) * fix->line_length;
			if (loc >= 0 && loc < fix->smem_len &&
					((bits >> (7 - j)) & 1)) {
				switch (var->bits_per_pixel) {
				case 8:
					p8 =  fb_info->ptr + loc;
					*p8 = color;
				case 16:
					p16 = fb_info->ptr + loc;
					*p16 = color;
					break;
				case 24:
				case 32:
					p32 = fb_info->ptr + loc;
					*p32 = color;
					break;
				}
			}
		}
	}
}

int fb_put_string(struct fb_info *fb_info, int x, int y, char *s, int maxlen,
		int color, int clear, int clearlen)
{
	int i;
	int w = 0;

	if (clear)
		fb_clear_area(fb_info, x, y, clearlen * 8, 8);

	for (i = 0; i < strlen(s) && i < maxlen; i++) {
		fb_put_char(fb_info, (x + 8 * i), y, s[i], color);
		w += 8;
	}

	return w;
}

