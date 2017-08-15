/*
 * common.h
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

#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <linux/fb.h>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define ASSERT(x) if (!(x)) \
	{ perror("assert(" __FILE__ ":" TOSTRING(__LINE__) "): "); exit(1); }
#define FBCTL0(ctl) if (ioctl(fd, ctl))\
	{ perror("fbctl0(" __FILE__ ":" TOSTRING(__LINE__) "): "); exit(1); }
#define FBCTL1(ctl, arg1) if (ioctl(fd, ctl, arg1))\
	{ perror("fbctl1(" __FILE__ ":" TOSTRING(__LINE__) "): "); exit(1); }

#define IOCTL0(fd, ctl) if (ioctl(fd, ctl))\
	{ perror("ioctl0(" __FILE__ ":" TOSTRING(__LINE__) "): "); exit(1); }
#define IOCTL1(fd, ctl, arg1) if (ioctl(fd, ctl, arg1))\
	{ perror("ioctl1(" __FILE__ ":" TOSTRING(__LINE__) "): "); exit(1); }

struct fb_info
{
	int fd;

	void *ptr;

	struct fb_var_screeninfo var;
	struct fb_fix_screeninfo fix;
	unsigned bytespp;
};

extern char fontdata_8x8[];

void fb_open(int fb_num, struct fb_info *fb_info);
void fb_update_window(int fd, short x, short y, short w, short h);
void fb_sync_gfx(int fd);
int fb_put_string(struct fb_info *fb_info, int x, int y, char *s, int maxlen,
		int color, int clear, int clearlen);

#endif
