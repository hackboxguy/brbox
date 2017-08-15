/*
 * gb-string.c
 *
 * Author: aguirre.nicolas@gmail.com
 * Copyright (C) 2013 Nicolas Aguirre

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

#include "common.h"

static struct fb_info fb_info;

static void draw_pixel(struct fb_info *fb_info, int x, int y, unsigned color)
{
	void *fbmem;

	fbmem = fb_info->ptr;
	if (fb_info->var.bits_per_pixel == 8) {
		unsigned char *p;

		fbmem += fb_info->fix.line_length * y;

		p = fbmem;

		p += x;

		*p = color;
	} else if (fb_info->var.bits_per_pixel == 16) {
		unsigned short c;
		unsigned r = (color >> 16) & 0xff;
		unsigned g = (color >> 8) & 0xff;
		unsigned b = (color >> 0) & 0xff;
		unsigned short *p;

		r = r * 32 / 256;
		g = g * 64 / 256;
		b = b * 32 / 256;

		c = (r << 11) | (g << 5) | (b << 0);

		fbmem += fb_info->fix.line_length * y;

		p = fbmem;

		p += x;

		*p = c;
	} else if (fb_info->var.bits_per_pixel == 24) {
		unsigned int *p;
		unsigned c;

		fbmem += fb_info->fix.line_length * y;
		fbmem += 3 * x;

		p = fbmem;

        c = *p;
        c = (c & 0xFF000000) | (color & 0x00FFFFFF);

		*p = c;
	} else {
		unsigned int *p;

		fbmem += fb_info->fix.line_length * y;

		p = fbmem;

		p += x;

		*p = color;
	}
}

void fill_screen_solid(struct fb_info *fb_info, unsigned int color)
{

	unsigned x, y;
	unsigned h = fb_info->var.yres;
	unsigned w = fb_info->var.xres;

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++)
			draw_pixel(fb_info, x, y, color);
	}
}

void show_help(void)
{
	printf("Usage: fb-string x y string color bg_color\n");
	printf("Where x          = x position of the top left corner\n");
	printf("      y          = y position of the top left corner\n");
	printf("      string     = String to display\n");
	printf("      color      = Text Color\n");
	printf("      bg_color   = background Color\n");
}

int main(int argc, char **argv)
{
	int req_fb = 0;
	int color, bg_color, x, y;
	
	if (argc != 6)
	  {
	    show_help();
	    return EXIT_FAILURE;
	  }

	fb_open(req_fb, &fb_info);
	if (!fb_info.ptr)
	  return EXIT_FAILURE;

	x = strtoul(argv[1], NULL, 0);
	y = strtoul(argv[2], NULL, 0);
	color =  strtoul(argv[4], NULL, 0);
	bg_color = strtoul(argv[5], NULL, 0);

	fill_screen_solid(&fb_info, bg_color);
	fb_put_string(&fb_info, x, y, argv[3], strlen(argv[3]), color , 0, 0);

	return EXIT_SUCCESS;
}
