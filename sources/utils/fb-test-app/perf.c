/*
 * perf.c
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <linux/types.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>

static int fd;
static void *fb;
static struct fb_var_screeninfo var;
static struct fb_fix_screeninfo fix;
static unsigned bytespp;
FILE *logfile;

int timeval_subtract(struct timeval *result, struct timeval *x,
		struct timeval *y)
{
	/* Perform the carry for the later subtraction by updating y. */
	if (x->tv_usec < y->tv_usec) {
		int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
		y->tv_usec -= 1000000 * nsec;
		y->tv_sec += nsec;
	}
	if (x->tv_usec - y->tv_usec > 1000000) {
		int nsec = (x->tv_usec - y->tv_usec) / 1000000;
		y->tv_usec += 1000000 * nsec;
		y->tv_sec -= nsec;
	}

	/* Compute the time remaining to wait.
	   tv_usec is certainly positive. */
	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_usec = x->tv_usec - y->tv_usec;

	/* Return 1 if result is negative. */
	return x->tv_sec < y->tv_sec;
}

static struct timeval timeval;

static void start_timing()
{
	gettimeofday(&timeval, NULL);
}

static unsigned long long stop_timing()
{
	struct timeval tv2, res;
	unsigned long long usecs;

	gettimeofday(&tv2, NULL);
	timeval_subtract(&res, &tv2, &timeval);
	usecs = res.tv_usec + res.tv_sec * 1000 * 1000;

	return usecs;
}

typedef void (*test_func)(unsigned, unsigned long long *, unsigned long long *);

static void run(const char *name, test_func func)
{
	unsigned long long usecs;
	unsigned long long pixels;
	unsigned long long pix_per_sec;
	const unsigned calib_loops = 5;
	const unsigned runtime_secs = 5;
	int loops;

	printf("%s: ", name);

	/* dunno if these work as I suppose, but I try to prevent any
	 * disk activity during test */
	fflush(stdout);
	fflush(logfile);
	sync();

	/* calibrate */
	func(calib_loops, &usecs, &pixels);
	loops = runtime_secs * 1000 * 1000 * calib_loops / usecs;

	func(loops, &usecs, &pixels);

	pix_per_sec = pixels * 1000 * 1000 / usecs;

	printf("%llu pix, %llu us, %llu pix/s\n", pixels,
			usecs, pix_per_sec);
	fprintf(logfile, "%s,%llu,%llu,%llu\n", name, pixels,
			usecs, pix_per_sec);
}


#define RUN(t) run(#t, t)

static void sequential_horiz_singlepixel_read(unsigned loops,
		unsigned long long *usecs, unsigned long long *pixels)
{
	const unsigned xres = var.xres_virtual;
	const unsigned yres = var.yres_virtual;

	unsigned l = loops;
	unsigned x, y;
	unsigned sum = 0;

	start_timing();

	while (l--) {
		__u32 *p32 = fb;
		for (y = 0; y < var.yres_virtual; ++y) {
			for (x = 0; x < var.xres_virtual; ++x)
				sum += p32[x];
			p32 += (fix.line_length / sizeof(*p32));
		}
	}

	*usecs = stop_timing();
	*pixels = xres * yres * loops;

	if (sum == 0xffffffff) {
		printf("bad luck\n");
		int *p = 0;
		*p = 0;
	}
}

static void sequential_horiz_singlepixel_write(unsigned loops,
		unsigned long long *usecs, unsigned long long *pixels)
{
	const unsigned xres = var.xres_virtual;
	const unsigned yres = var.yres_virtual;

	unsigned l = loops;
	unsigned x, y;

	start_timing();

	while (l--) {
		__u32 *p32 = fb;
		for (y = 0; y < var.yres_virtual; ++y) {
			for (x = 0; x < var.xres_virtual; ++x)
				p32[x] = x * y * (loops - l);
			p32 += (fix.line_length / sizeof(*p32));
		}
	}

	*usecs = stop_timing();
	*pixels = xres * yres * loops;
}

static void sequential_vert_singlepixel_read(unsigned loops,
		unsigned long long *usecs, unsigned long long *pixels)
{
	const unsigned xres = var.xres_virtual;
	const unsigned yres = var.yres_virtual;

	unsigned l = loops;
	unsigned x, y;
	unsigned sum = 0;

	start_timing();

	while (l--) {
		for (x = 0; x < var.xres_virtual; ++x) {
			__u32 *p32 = ((__u32 *)fb) + x;
			for (y = 0; y < var.yres_virtual; ++y) {
				sum += *p32;
				p32 += (fix.line_length / sizeof(*p32));
			}
		}
	}

	*usecs = stop_timing();
	*pixels = xres * yres * loops;

	if (sum == 0xffffffff) {
		printf("bad luck\n");
		int *p = 0;
		*p = 0;
	}
}

static void sequential_vert_singlepixel_write(unsigned loops,
		unsigned long long *usecs, unsigned long long *pixels)
{
	const unsigned xres = var.xres_virtual;
	const unsigned yres = var.yres_virtual;

	unsigned l = loops;
	unsigned x, y;

	start_timing();

	while (l--) {
		for (x = 0; x < var.xres_virtual; ++x) {
			__u32 *p32 = ((__u32 *)fb) + x;
			for (y = 0; y < var.yres_virtual; ++y) {
				*p32 = x * y * (loops - l);
				p32 += (fix.line_length / sizeof(*p32));
			}
		}
	}

	*usecs = stop_timing();
	*pixels = xres * yres * loops;
}

static void sequential_line_read(unsigned loops,
		unsigned long long *usecs, unsigned long long *pixels)
{
	const unsigned xres = var.xres_virtual;
	const unsigned yres = var.yres_virtual;

	unsigned l = loops;
	unsigned y;
	void *linebuf;

	linebuf = malloc(var.xres_virtual * bytespp);

	start_timing();

	while (l--) {
		void *p = fb;
		for (y = 0; y < var.yres_virtual; ++y) {
			memcpy(linebuf, p, var.xres_virtual * bytespp);
			p += fix.line_length;
		}
	}

	*usecs = stop_timing();
	*pixels = xres * yres * loops;

	free(linebuf);
}

static void sequential_line_write(unsigned loops,
		unsigned long long *usecs, unsigned long long *pixels)
{
	const unsigned xres = var.xres_virtual;
	const unsigned yres = var.yres_virtual;

	unsigned l = loops;
	unsigned y;
	void *linebuf;

	linebuf = malloc(var.xres_virtual * bytespp);
	for (y = 0; y < var.xres_virtual * bytespp; ++y)
		((unsigned char*)linebuf)[y] = y;

	start_timing();

	while (l--) {
		void *p = fb;
		for (y = 0; y < var.yres_virtual; ++y) {
			memcpy(p, linebuf, var.xres_virtual * bytespp);
			p += fix.line_length;
		}
	}

	*usecs = stop_timing();
	*pixels = xres * yres * loops;

	free(linebuf);
}

static void nonsequential_singlepixel_write(unsigned loops,
		unsigned long long *usecs, unsigned long long *pixels)
{
	const unsigned xres = var.xres_virtual;
	const unsigned yres = var.yres_virtual;

	unsigned l = loops;
	unsigned i;

	start_timing();

	while (l--) {
		for (i = 0; i < yres * xres; ++i) {
			const unsigned yparts = 4;
			const unsigned xparts = 4;
			unsigned x, y;

			y = (i % yparts) * (yres / yparts);
			y += (i / yparts) % (yres / yparts);

			x = ((i / yres) % xparts) * (xres / xparts);
			x += ((i / yres) / xparts) % (xres / xparts);

			__u32 *p32 = fb + y * fix.line_length;

			p32[x] = x * y * (loops - l);
		}
	}

	*usecs = stop_timing();
	*pixels = xres * yres * loops;
}

static void nonsequential_singlepixel_read(unsigned loops,
		unsigned long long *usecs, unsigned long long *pixels)
{
	const unsigned xres = var.xres_virtual;
	const unsigned yres = var.yres_virtual;

	unsigned l = loops;
	unsigned i;
	unsigned sum = 0;

	start_timing();

	while (l--) {
		for (i = 0; i < yres * xres; ++i) {
			const unsigned yparts = 16;
			const unsigned xparts = 8;
			unsigned x, y;

			y = (i % yparts) * (yres / yparts);
			y += (i / yparts) % (yres / yparts);

			x = ((i / yres) % xparts) * (xres / xparts);
			x += ((i / yres) / xparts) % (xres / xparts);

			__u32 *p32 = fb + y * fix.line_length;

			sum += p32[x];
		}
	}

	*usecs = stop_timing();
	*pixels = xres * yres * loops;

	if (sum == 0xffffffff) {
		printf("bad luck\n");
		int *p = 0;
		*p = 0;
	}
}

int main(int argc, char **argv)
{
	int fb_num = 0;
	char str[64];

	printf("perf %d.%d.%d (%s)\n", VERSION, PATCHLEVEL, SUBLEVEL,
		VERSION_NAME);

	if (argc != 3) {
		printf("usage: %s <fbnum> <logfile>\n", argv[0]);
		return 0;
	}

	fb_num = atoi(argv[1]);
	sprintf(str, "/dev/fb%d", fb_num);
	fd = open(str, O_RDWR);

	if (ioctl(fd, FBIOGET_VSCREENINFO, &var))
		return -1;

	if (ioctl(fd, FBIOGET_FSCREENINFO, &fix))
		return -1;

	bytespp = var.bits_per_pixel / 8;

	fb = mmap(NULL, fix.line_length * var.yres_virtual,
			PROT_READ | PROT_WRITE, MAP_SHARED,
			fd, 0);
	if (fb == MAP_FAILED)
		return -1;

	logfile = fopen(argv[2], "w");
	if (logfile == NULL) {
		printf("Failed to open logfile\n");
		return -1;
	}

	RUN(sequential_horiz_singlepixel_read);
	RUN(sequential_horiz_singlepixel_write);

	RUN(sequential_vert_singlepixel_read);
	RUN(sequential_vert_singlepixel_write);

	RUN(sequential_line_read);
	RUN(sequential_line_write);

	RUN(nonsequential_singlepixel_write);
	RUN(nonsequential_singlepixel_read);

	close(fd);
	fclose(logfile);

	return 0;
}

