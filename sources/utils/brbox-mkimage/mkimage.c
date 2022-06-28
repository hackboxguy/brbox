/*
 * (C) Copyright 2000-2004
 * DENX Software Engineering
 * Wolfgang Denk, wd@denx.de
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 *
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef __WIN32__
#include <netinet/in.h>		/* for host / network byte order conversions	*/
#endif
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#if defined(__BEOS__) || defined(__NetBSD__) || defined(__APPLE__)
#include <inttypes.h>
#endif

#ifdef __WIN32__
typedef unsigned int __u32;

#define SWAP_LONG(x) \
	((__u32)( \
		(((__u32)(x) & (__u32)0x000000ffUL) << 24) | \
		(((__u32)(x) & (__u32)0x0000ff00UL) <<  8) | \
		(((__u32)(x) & (__u32)0x00ff0000UL) >>  8) | \
		(((__u32)(x) & (__u32)0xff000000UL) >> 24) ))
typedef		unsigned char	uint8_t;
typedef		unsigned short	uint16_t;
typedef		unsigned int	uint32_t;

#define     ntohl(a)	SWAP_LONG(a)
#define     htonl(a)	SWAP_LONG(a)
#endif	/* __WIN32__ */

#ifndef	O_BINARY		/* should be define'd on __WIN32__ */
#define O_BINARY	0
#endif

#include "image.h"

extern int errno;

#ifndef MAP_FAILED
#define MAP_FAILED (-1)
#endif

char *cmdname;

extern unsigned long crc32 (unsigned long crc, const char *buf, unsigned int len);

typedef struct table_entry {
	int	val;		/* as defined in image.h	*/
	char	*sname;		/* short (input) name		*/
	char	*lname;		/* long (output) name		*/
} table_entry_t;

table_entry_t arch_name[] = {
    {	IH_CPU_INVALID,		NULL,		"Invalid CPU",	},
    {	IH_CPU_ALPHA,		"alpha",	"Alpha",	},
    {	IH_CPU_ARM,		"arm",		"ARM",		},
    {	IH_CPU_I386,		"x86",		"Intel x86",	},
    {	IH_CPU_IA64,		"ia64",		"IA64",		},
    {	IH_CPU_M68K,		"m68k",		"MC68000",	},
    {	IH_CPU_MICROBLAZE,	"microblaze",	"MicroBlaze",	},
    {	IH_CPU_MIPS,		"mips",		"MIPS",		},
    {	IH_CPU_MIPS64,		"mips64",	"MIPS 64 Bit",	},
    {	IH_CPU_NIOS,		"nios",		"NIOS",		},
    {	IH_CPU_NIOS2,		"nios2",	"NIOS II",	},
    {	IH_CPU_PPC,		"ppc",		"PowerPC",	},
    {	IH_CPU_S390,		"s390",		"IBM S390",	},
    {	IH_CPU_SH,		"sh",		"SuperH",	},
    {	IH_CPU_SPARC,		"sparc",	"SPARC",	},
    {	IH_CPU_SPARC64,		"sparc64",	"SPARC 64 Bit",	},
    {	-1,			"",		"",		},
};

table_entry_t os_name[] = {
    {	IH_OS_INVALID,	NULL,		"Invalid OS",		},
    {	IH_OS_4_4BSD,	"4_4bsd",	"4_4BSD",		},
    {	IH_OS_ARTOS,	"artos",	"ARTOS",		},
    {	IH_OS_DELL,	"dell",		"Dell",			},
    {	IH_OS_ESIX,	"esix",		"Esix",			},
    {	IH_OS_FREEBSD,	"freebsd",	"FreeBSD",		},
    {	IH_OS_IRIX,	"irix",		"Irix",			},
    {	IH_OS_LINUX,	"linux",	"Linux",		},
    {	IH_OS_LYNXOS,	"lynxos",	"LynxOS",		},
    {	IH_OS_NCR,	"ncr",		"NCR",			},
    {	IH_OS_NETBSD,	"netbsd",	"NetBSD",		},
    {	IH_OS_OPENBSD,	"openbsd",	"OpenBSD",		},
    {	IH_OS_PSOS,	"psos",		"pSOS",			},
    {	IH_OS_QNX,	"qnx",		"QNX",			},
    {	IH_OS_RTEMS,	"rtems",	"RTEMS",		},
    {	IH_OS_SCO,	"sco",		"SCO",			},
    {	IH_OS_SOLARIS,	"solaris",	"Solaris",		},
    {	IH_OS_SVR4,	"svr4",		"SVR4",			},
    {	IH_OS_U_BOOT,	"u-boot",	"U-Boot",		},
    {	IH_OS_VXWORKS,	"vxworks",	"VxWorks",		},
    {	-1,		"",		"",			},
};

table_entry_t type_name[] =
{
    {	IH_TYPE_INVALID,    NULL,	  "Invalid Image",	},
    {	IH_TYPE_FILESYSTEM, "filesystem", "Filesystem Image",	},
    {	IH_TYPE_FIRMWARE,   "firmware",	  "Firmware",		},
    {	IH_TYPE_KERNEL,	    "kernel",	  "Kernel Image",	},
    {	IH_TYPE_MULTI,	    "multi",	  "Multi-File Image",	},
    {	IH_TYPE_RAMDISK,    "ramdisk",	  "RAMDisk Image",	},
    {	IH_TYPE_SCRIPT,     "script",	  "Script",		},
    {	IH_TYPE_STANDALONE, "standalone",           "Standalone Program", },
    {	IH_TYPE_UBOOT,	    "uboot", 	  "U-Boot Image", 	},
    {	IH_TYPE_BRBOX_PROJ,      STR_TYPE_BRBOX_PROJ,     "BrBox Proj Image",},//whole BrBox project file
    {	IH_TYPE_BRBOX_STTNG,     STR_TYPE_BRBOX_STTNG,    "BrBox Settng Image",},
    {	IH_TYPE_BRBOX_USRDAT,    STR_TYPE_BRBOX_USRDAT,   "BrBox UsrDat Image",},
    {	IH_TYPE_BRBOX_ROOT_BTR,  STR_TYPE_BRBOX_ROOT_BTR, "BrBox RfsBtr Image",},
    {	IH_TYPE_BRBOX_ROOT_RP1,  STR_TYPE_BRBOX_ROOT_RP1, "BrBox RfsRp1 Image",},
    {	IH_TYPE_BRBOX_ROOT_RP2,  STR_TYPE_BRBOX_ROOT_RP2, "BrBox RfsRp2 Image",},
    {	IH_TYPE_BRBOX_ROOT_BBB,  STR_TYPE_BRBOX_ROOT_BBB, "BrBox RfsBbb Image",},
    {	IH_TYPE_BRBOX_ROOT_WDB,  STR_TYPE_BRBOX_ROOT_WDB, "BrBox RfsWdb Image",},
    {	IH_TYPE_BRBOX_ROOT_RP3,  STR_TYPE_BRBOX_ROOT_RP3, "BrBox RfsRp3 Image",},
    {	IH_TYPE_BRBOX_ROOT_RP0,  STR_TYPE_BRBOX_ROOT_RP0, "BrBox RfsRp0 Image",},
    {	IH_TYPE_BRBOX_GLMT300NV2,STR_TYPE_BRBOX_GLMT300NV2,"BrBox Gl300Nv2 Image",},
    {	IH_TYPE_BRBOX_GLAR150,   STR_TYPE_BRBOX_GLAR150,   "BrBox GlAr150 Image",},
	{	IH_TYPE_BRBOX_SIGNATURE, STR_TYPE_BRBOX_SIGNATURE, "BrBox Sign Image",},
    {	-1,		    "",		  "",			},
};


table_entry_t comp_name[] = {
    {	IH_COMP_NONE,	"none",		"uncompressed",		},
    {	IH_COMP_BZIP2,	"bzip2",	"bzip2 compressed",	},
    {	IH_COMP_GZIP,	"gzip",		"gzip compressed",	},
    {	IH_COMP_LZMA2,	"lzma2",	"lzma2 compressed",	},
    {	-1,		"",		"",			},
};

static	void	copy_file (int, const char *, int);
static	void	usage	(void);
static	int	print_header (image_header_t *);
static	void	print_type (image_header_t *);
static	char	*put_table_entry (table_entry_t *, char *, int);
static	char	*put_arch (int);
static	char	*put_type (int);
static	char	*put_os   (int);
static	char	*put_comp (int);
static	int	get_table_entry (table_entry_t *, char *, char *);
static	int	get_arch(char *);
static	int	get_comp(char *);
static	int	get_os  (char *);
static	int	get_type(char *);


char	*datafile;
char	*imagefile;

int dflag    = 0;
int eflag    = 0;
int lflag    = 0;
int vflag    = 0;
int xflag    = 0;
int opt_os   = IH_OS_LINUX;
int opt_arch = IH_CPU_PPC;
int opt_type = IH_TYPE_KERNEL;
int opt_comp = IH_COMP_GZIP;

image_header_t header;
image_header_t *hdr = &header;
typedef enum
{
	compare,
	extract,
	query,
	exthdr,
	count
}TypeAction;

typedef enum
{
	cmp_binary,
	cmp_mkimage,
	cmp_invalid
}TypeCompare;


#define TEMP_MTD_FILE "/tmp/temp_mkimage_file"
char mtdfile[1024];
char temp_buffer[1024];
int silent_flag=0;
int mtd_flag = 0;

char *query_file;
char *extract_file;
char *compare_file;
int xtractflag = 0;
int xtrhdrflag = 0;
int compareflag = 0;
int queryflag = 0;
char *output_file;
int outputfileflag = 0;
int headeronly_flag=0;
int countimage_flag=0;
int max_header_counts;
int image_count=0;
int xtract_file_no_flag=0;
int file_number=0;

int skip_value =0;
int skipflag = 0;
int seek_value = 0;
int seekflag = 0;



static int printmsg(char *Msg);
static int check_mtdfile(char *imagefile);
static int check_header (image_header_t *hdr);
static int extract_file_from_multi(uint8_t image_type,char *input_file,char *output_file,image_header_t *hdr,TypeAction action,int* img_count);
static int extract_file_number(int image_no,char *input_file,char *output_file,image_header_t *hdr);
static int file_copy(char *inputfile,char *outputfile,uint32_t skip,uint32_t len);
static int print_header_only (image_header_t *hdr);
static int file_compare(char *file1,char *file2,uint32_t skip,uint32_t proj_file_len);
static int file_compare_binary(char *file1,char *file2,uint32_t skip,uint32_t seek,uint32_t file1_len);
unsigned char get_image_type(char *image_name);

int main (int argc, char **argv)
{
	int ifd;
	uint32_t checksum;
	uint32_t addr;
	uint32_t ep;
	struct stat sbuf;
	unsigned char *ptr;
	char *name = "";

	cmdname = *argv;

	addr = ep = 0;

	while (--argc > 0 && **++argv == '-')
	{
		while (*++*argv)
		{
			switch (**argv)
			{
			case 'l':
				lflag = 1;
				break;
			case 'A':
				if ((--argc <= 0) ||
				    (opt_arch = get_arch(*++argv)) < 0)
					usage ();
				goto NXTARG;
			case 'C':
				if ((--argc <= 0) ||
				    (opt_comp = get_comp(*++argv)) < 0)
					usage ();
				goto NXTARG;
			case 'O':
				if ((--argc <= 0) ||
				    (opt_os = get_os(*++argv)) < 0)
					usage ();
				goto NXTARG;
			case 'T':
				if ((--argc <= 0) ||
				    (opt_type = get_type(*++argv)) < 0)
					usage ();
				goto NXTARG;

			case 'a':
				if (--argc <= 0)
					usage ();
				addr = strtoul (*++argv, (char **)&ptr, 16);
				if (*ptr) {
					fprintf (stderr,
						"%s: invalid load address %s\n",
						cmdname, *argv);
					exit (-1);
				}
				goto NXTARG;
			case 'd':
				if (--argc <= 0)
					usage ();
				datafile = *++argv;
				dflag = 1;
				goto NXTARG;
			case 'e':
				if (--argc <= 0)
					usage ();
				ep = strtoul (*++argv, (char **)&ptr, 16);
				if (*ptr) {
					fprintf (stderr,
						"%s: invalid entry point %s\n",
						cmdname, *argv);
					exit (-1);
				}
				eflag = 1;
				goto NXTARG;
			case 'n':
				if (--argc <= 0)
					usage ();
				name = *++argv;
				goto NXTARG;
			case 'v':
				vflag++;
				break;
			case 'x':
				xflag++;
				break;
			case 's':
				silent_flag=1;
				break;
			case 'm':
				mtd_flag=1;
				break;

			case 'q':/*extract the requested image*/
				if (--argc <= 0)
					usage ();
				query_file = *++argv;
				queryflag = 1;
				goto NXTARG;

			case 'X':/*extract the requested image*/
				if (--argc <= 0)
					usage ();
				extract_file = *++argv;
				xtractflag = 1;
				goto NXTARG;
			case 'H':/*extract the requested image*/
				if (--argc <= 0)
					usage ();
				extract_file = *++argv;
				xtrhdrflag = 1;
				goto NXTARG;

			case 'c':/*compare the requested image*/
				if (--argc <= 0)
					usage ();
				compare_file = *++argv;
				compareflag = 1;
				//break;
				goto NXTARG;
			case 'k':/*skip option, to be used with -c(compare) option*/
				if (--argc <= 0)
					usage ();
				skip_value = atoi(*++argv);
				skipflag = 1;
				//break;
				goto NXTARG;
			case 'E':/*seek option, to be used with -c(compare) option*/
				if (--argc <= 0)
					usage ();
				seek_value = atoi(*++argv);
				seekflag = 1;
				//break;
				goto NXTARG;

			case 'o':/*extract the requested to this file*/
				if (--argc <= 0)
					usage ();
				output_file = *++argv;
				outputfileflag = 1;
				goto NXTARG;
			case 'h':/*print only the headers*/
				headeronly_flag=1;
				max_header_counts=atoi(*++argv);
				break;
			case 't':/*count total images*/
				countimage_flag=1;
				break;
			case 'f':
				if (--argc <= 0)
					usage ();
				file_number=atoi(*++argv);
				xtract_file_no_flag=1;
				goto NXTARG;
			default:
				usage ();
			}
		}
NXTARG:;
	}

	if(headeronly_flag)/*used for printing the versioninfo*/
	{
		uint32_t temp_len,temp_i;
		int32_t result=0;
		ifd = open(*argv, O_RDONLY|O_BINARY);

		sprintf(temp_buffer,"##############################################################\n");
		printmsg(temp_buffer);

		for(temp_i=0;temp_i<max_header_counts;temp_i++)
		{
			temp_len=read(ifd,&header,sizeof(image_header_t));
			if(temp_len != sizeof(image_header_t))break;
			result|=print_header_only(&header);
			sprintf(temp_buffer,"##############################################################\n");
			printmsg(temp_buffer);
		}
		close(ifd);
		if(result==0)
			exit (EXIT_SUCCESS);
		else
			exit (EXIT_FAILURE);
	}


	if ((argc != 1) || ((lflag ^ dflag) == 0))
		usage();

	if (!eflag) {
		ep = addr;
		/* If XIP, entry point must be after the U-Boot header */
		if (xflag)
			ep += sizeof(image_header_t);
	}

	/*
	 * If XIP, ensure the entry point is equal to the load address plus
	 * the size of the U-Boot header.
	 */
	if (xflag) {
		if (ep != addr + sizeof(image_header_t)) {
			fprintf (stderr, "%s: For XIP, the entry point must be the load addr + %lu\n",
				cmdname,
				(unsigned long)sizeof(image_header_t));
			exit (-1);
		}
	}

	imagefile = *argv;

	if(compareflag && ( skipflag == 1 || seekflag == 1 ) )
	{
		int tmpfile;
		uint32_t compare_file_size=0;
		tmpfile = open(compare_file, O_RDONLY|O_BINARY);
		if (fstat(tmpfile, &sbuf) < 0)
		{
			sprintf (temp_buffer, "%s: Can't stat %s: %s\n",cmdname, imagefile, strerror(errno));
			printmsg(temp_buffer);
			close(tmpfile);
			exit (-1);
		}
		compare_file_size=sbuf.st_size;
		close(tmpfile);
		if(file_compare_binary(compare_file,imagefile,skip_value,seek_value,compare_file_size) != 0)
			exit(-1);
		else
			exit(EXIT_SUCCESS);
	}



	if(mtd_flag)
	{
		if(check_mtdfile(imagefile))
		{
			sprintf (temp_buffer,"%s:Bad file type(it seems not a valid uimage file)!\n",cmdname);
			printmsg(temp_buffer);//imagefile);
			exit (-1);
		}
		imagefile=mtdfile;
	}

	if (lflag)
	{
		ifd = open(imagefile, O_RDONLY|O_BINARY);
	}
	else
	{
		ifd = open(imagefile, O_RDWR|O_CREAT|O_TRUNC|O_BINARY, 0666);
	}

	if (ifd < 0)
	{
		sprintf (temp_buffer, "%s: Can't open %s: %s\n",
			cmdname, imagefile, strerror(errno));
		printmsg(temp_buffer);//mtdfile);
		exit (-1);
	}

	if (lflag)
	{
		int len;
		char *data;
		/*
		 * list header information of existing image
		 */

		if (fstat(ifd, &sbuf) < 0) {
			sprintf (temp_buffer, "%s: Can't stat %s: %s\n",
				cmdname, imagefile, strerror(errno));
			printmsg(temp_buffer);
			exit (-1);
		}

		if ((unsigned)sbuf.st_size < sizeof(image_header_t)) {
			sprintf (temp_buffer,
				"%s: Bad size: \"%s\" is no valid image st_size=%d, imghdr_size=%d\n",
				cmdname, imagefile,(int)sbuf.st_size,(int)sizeof(image_header_t));
			printmsg(temp_buffer);
			exit (-1);
		}

		ptr = (unsigned char *)mmap(0,sbuf.st_size,
					    PROT_READ, MAP_SHARED, ifd, 0);

#ifdef SDSRV_USE_MUSL_LIB
		if ((void*)ptr == (void*)-1)
#else
		if ((caddr_t)ptr == (caddr_t)-1)
#endif
		{
			sprintf (temp_buffer, "%s: Can't read %s: %s\n",
				cmdname, imagefile, strerror(errno));
			printmsg(temp_buffer);
			exit (-1);
		}

		/*
		 * create copy of header so that we can blank out the
		 * checksum field for checking - this can't be done
		 * on the PROT_READ mapped data.
		 */
		memcpy (hdr, ptr, sizeof(image_header_t));

		if (ntohl(hdr->ih_magic) != IH_MAGIC)
		{
			sprintf (temp_buffer,
				"%s: Bad Magic Number: \"%s\" is no valid image\n",
				cmdname, imagefile);
			printmsg(temp_buffer);
			exit (-1);
		}

		data = (char *)hdr;
		len  = sizeof(image_header_t);

		checksum = ntohl(hdr->ih_hcrc);
		hdr->ih_hcrc = htonl(0);	/* clear for re-calculation */

		if (crc32 (0, data, len) != checksum) {
			sprintf (temp_buffer,
				"*** Warning: \"%s\" has bad header checksum!\n",
				imagefile);
			printmsg(temp_buffer);
			exit (-1);
		}

		data = (char *)(ptr + sizeof(image_header_t));
		len  = sbuf.st_size - sizeof(image_header_t) ;

		if (crc32 (0, data, len) != ntohl(hdr->ih_dcrc)) {
			sprintf (temp_buffer,
				"*** Warning: \"%s\" has corrupted data!\n",
				imagefile);
			printmsg(temp_buffer);
			exit (-1);
		}

		/* for multi-file images we need the data part, too */
		if(print_header((image_header_t *)ptr)!=0)
		{
			sprintf (temp_buffer,"one of the images has been corrupted!\n");
			printmsg(temp_buffer);
			exit (-1);
		}
		if(queryflag)
		{
			int32_t image_type=get_image_type(query_file);//atoi(query_file);
			if(extract_file_from_multi((uint8_t)image_type,imagefile,output_file,(image_header_t *)ptr,query,&image_count)!=0)
				exit(-1);
		}
		if(xtractflag || xtrhdrflag)
		{
			if(outputfileflag)
			{
				int32_t image_type=get_image_type(extract_file);//atoi(extract_file);
				if(xtractflag)
				{
					if(extract_file_from_multi((uint8_t)image_type,imagefile,output_file,(image_header_t *)ptr,extract,&image_count)!=0)
						exit(-1);
				}
				else//just extract header
				{
					if(extract_file_from_multi((uint8_t)image_type,imagefile,output_file,(image_header_t *)ptr,exthdr,&image_count)!=0)
						exit(-1);
				}

			}
			else
			{
				sprintf (temp_buffer,"output file name not specified!\n");
				printmsg(temp_buffer);
				exit (-1);
			}
		}
		if(compareflag)
		{
			if( skipflag == 1 || seekflag == 1 )
			{

				int tmpfile;
				uint32_t compare_file_size=0;
				tmpfile = open(compare_file, O_RDONLY|O_BINARY);
				if (fstat(tmpfile, &sbuf) < 0)
				{
					sprintf (temp_buffer, "%s: Can't stat %s: %s\n",cmdname, imagefile, strerror(errno));
					printmsg(temp_buffer);
					close(tmpfile);
					exit (-1);
				}
				compare_file_size=sbuf.st_size;
				close(tmpfile);
				if(file_compare_binary(compare_file,imagefile,skip_value,seek_value,compare_file_size) != 0)
					exit(-1);
			}
			else
			{
				FILE *imgfile;
				image_header_t temp_image_hdr;
				imgfile=fopen(compare_file,"r");
				if(imgfile==NULL)
				{
					//printf("%s open error!\n",compare_file);
					exit(-1);
				}
				if( fread(&temp_image_hdr,sizeof(char),sizeof(image_header_t),imgfile) < sizeof(image_header_t) )
				{
					fclose(imgfile);
					exit(-1);
				}
				if(temp_image_hdr.ih_type < IH_TYPE_STANDALONE || temp_image_hdr.ih_type > IH_TYPE_BRBOX_USRDAT)
				{
					fclose(imgfile);
					exit(-1);
				}
				fclose(imgfile);
				if(extract_file_from_multi(temp_image_hdr.ih_type,imagefile,compare_file,(image_header_t *)ptr,compare,&image_count)!=0)
					exit(-1);
			}
		}
		if(countimage_flag)
		{
			uint8_t image_type=0;
			if(extract_file_from_multi((uint8_t)image_type,imagefile,output_file,(image_header_t *)ptr,count,&image_count)!=0)
				exit(-1);
			else
				printf("total images in project file are: %d\n",image_count);
		}
		if(xtract_file_no_flag)//extract extracted file number from multi image
		{
			if(extract_file_number(file_number,imagefile,output_file,(image_header_t *)ptr)!=0)
				exit (-1);
			else
				printf("requested file extracted successfully!\n");
		}

		(void) munmap((void *)ptr, sbuf.st_size);
		(void) close (ifd);

		if(mtd_flag)/*delete the temp file*/
		{
			sprintf(mtdfile,"rm -rf %s",TEMP_MTD_FILE);
			system(mtdfile);
		}
		exit (EXIT_SUCCESS);
	}

	/*
	 * Must be -w then:
	 *
	 * write dummy header, to be fixed later
	 */
	memset (hdr, 0, sizeof(image_header_t));

	if (write(ifd, hdr, sizeof(image_header_t)) != sizeof(image_header_t)) {
		fprintf (stderr, "%s: Write error on %s: %s\n",
			cmdname, imagefile, strerror(errno));
		exit (-1);
	}

	if (opt_type == IH_TYPE_MULTI  || opt_type == IH_TYPE_SCRIPT) {
		char *file = datafile;
		unsigned long size;

		for (;;) {
			char *sep = NULL;

			if (file) {
				if ((sep = strchr(file, ':')) != NULL) {
					*sep = '\0';
				}

				if (stat (file, &sbuf) < 0) {
					fprintf (stderr, "%s: Can't stat %s: %s\n",
						cmdname, file, strerror(errno));
					exit (-1);
				}
				size = htonl(sbuf.st_size);
			} else {
				size = 0;
			}

			if (write(ifd, (char *)&size, sizeof(size)) != sizeof(size)) {
				fprintf (stderr, "%s: Write error on %s: %s\n",
					cmdname, imagefile, strerror(errno));
				exit (-1);
			}

			if (!file) {
				break;
			}

			if (sep) {
				*sep = ':';
				file = sep + 1;
			} else {
				file = NULL;
			}
		}

		file = datafile;

		for (;;) {
			char *sep = strchr(file, ':');
			if (sep) {
				*sep = '\0';
				copy_file (ifd, file, 1);
				*sep++ = ':';
				file = sep;
			} else {
				copy_file (ifd, file, 0);
				break;
			}
		}
	} else {
		copy_file (ifd, datafile, 0);
	}

	/* We're a bit of paranoid */
#if defined(_POSIX_SYNCHRONIZED_IO) && !defined(__sun__) && !defined(__FreeBSD__)
	(void) fdatasync (ifd);
#else
	(void) fsync (ifd);
#endif

	if (fstat(ifd, &sbuf) < 0) {
		fprintf (stderr, "%s: Can't stat %s: %s\n",
			cmdname, imagefile, strerror(errno));
		exit (-1);
	}

	ptr = (unsigned char *)mmap(0, sbuf.st_size,
				    PROT_READ|PROT_WRITE, MAP_SHARED, ifd, 0);
	if (ptr == (unsigned char *)MAP_FAILED) {
		fprintf (stderr, "%s: Can't map %s: %s\n",
			cmdname, imagefile, strerror(errno));
		exit (-1);
	}

	hdr = (image_header_t *)ptr;

	checksum = crc32 (0,
			  (const char *)(ptr + sizeof(image_header_t)),
			  sbuf.st_size - sizeof(image_header_t)
			 );

	/* Build new header */
	hdr->ih_magic = htonl(IH_MAGIC);
	hdr->ih_time  = htonl(sbuf.st_mtime);
	hdr->ih_size  = htonl(sbuf.st_size - sizeof(image_header_t));
	hdr->ih_load  = htonl(addr);
	hdr->ih_ep    = htonl(ep);
	hdr->ih_dcrc  = htonl(checksum);
	hdr->ih_os    = opt_os;
	hdr->ih_arch  = opt_arch;
	hdr->ih_type  = opt_type;
	hdr->ih_comp  = opt_comp;

	strncpy((char *)hdr->ih_name, name, IH_NMLEN);

	checksum = crc32(0,(const char *)hdr,sizeof(image_header_t));

	hdr->ih_hcrc = htonl(checksum);

	print_header (hdr);

	(void) munmap((void *)ptr, sbuf.st_size);

	/* We're a bit of paranoid */
#if defined(_POSIX_SYNCHRONIZED_IO) && !defined(__sun__) && !defined(__FreeBSD__)
	(void) fdatasync (ifd);
#else
	(void) fsync (ifd);
#endif

	if (close(ifd)) {
		fprintf (stderr, "%s: Write error on %s: %s\n",
			cmdname, imagefile, strerror(errno));
		exit (-1);
	}

	exit (EXIT_SUCCESS);
}

static void
copy_file (int ifd, const char *datafile, int pad)
{
	int dfd;
	struct stat sbuf;
	unsigned char *ptr;
	int tail;
	int zero = 0;
	int offset = 0;
	int size;

	if (vflag) {
		fprintf (stderr, "Adding Image %s\n", datafile);
	}

	if ((dfd = open(datafile, O_RDONLY|O_BINARY)) < 0) {
		fprintf (stderr, "%s: Can't open %s: %s\n",
			cmdname, datafile, strerror(errno));
		exit (-1);
	}

	if (fstat(dfd, &sbuf) < 0) {
		fprintf (stderr, "%s: Can't stat %s: %s\n",
			cmdname, datafile, strerror(errno));
		exit (-1);
	}

	ptr = (unsigned char *)mmap(0, sbuf.st_size,
				    PROT_READ, MAP_SHARED, dfd, 0);
	if (ptr == (unsigned char *)MAP_FAILED) {
		fprintf (stderr, "%s: Can't read %s: %s\n",
			cmdname, datafile, strerror(errno));
		exit (-1);
	}

	if (xflag) {
		unsigned char *p = NULL;
		/*
		 * XIP: do not append the image_header_t at the
		 * beginning of the file, but consume the space
		 * reserved for it.
		 */

		if ((unsigned)sbuf.st_size < sizeof(image_header_t)) {
			fprintf (stderr,
				"%s: Bad size: \"%s\" is too small for XIP\n",
				cmdname, datafile);
			exit (-1);
		}

		for (p=ptr; p < ptr+sizeof(image_header_t); p++) {
			if ( *p != 0xff ) {
				fprintf (stderr,
					"%s: Bad file: \"%s\" has invalid buffer for XIP\n",
					cmdname, datafile);
				exit (-1);
			}
		}

		offset = sizeof(image_header_t);
	}

	size = sbuf.st_size - offset;
	if (write(ifd, ptr + offset, size) != size) {
		fprintf (stderr, "%s: Write error on %s: %s\n",
			cmdname, imagefile, strerror(errno));
		exit (-1);
	}

	if (pad && ((tail = size % 4) != 0)) {

		if (write(ifd, (char *)&zero, 4-tail) != 4-tail) {
			fprintf (stderr, "%s: Write error on %s: %s\n",
				cmdname, imagefile, strerror(errno));
			exit (-1);
		}
	}

	(void) munmap((void *)ptr, sbuf.st_size);
	(void) close (dfd);
}
#include "SrcControlVersion.h"
void
usage ()
{
	fprintf (stderr, "%s build-version: %d\n",cmdname,SRC_CONTROL_VERSION);//MKIMG_BUILD_NUMBER);

	fprintf (stderr, "Usage: %s -l image\n"
			 "          -l ==> list image header information\n"
			 "       %s [-x] -A arch -O os -T type -C comp "
			 "-a addr -e ep -n name -d data_file[:data_file...] image\n",
		cmdname, cmdname);
	fprintf (stderr, "          -A ==> set architecture to 'arch'\n"
			 "          -O ==> set operating system to 'os'\n"
			 "          -T ==> set image type to 'type'\n"
			 "          -C ==> set compression type 'comp'\n"
			 "          -a ==> set load address to 'addr' (hex)\n"
			 "          -e ==> set entry point to 'ep' (hex)\n"
			 "          -n ==> set image name to 'name'\n"
			 "          -d ==> use image data from 'datafile'\n"
			 "          -x ==> set XIP (execute in place)\n"
			 "          -s ==> silent operation(do not print any messages)\n"
			 "          -m ==> input file is mtd partition(for files stored in mtd flash partition)\n"
			 "          -q ==> check if requested image is available in project file\n"
			 "          -X ==> extract the requested file from multi image\n"
			 "          -H ==> extract the requested file's Header from multi image\n"
			 "          -c ==> compare the requested file from multi image\n"
			 "          -k ==> skip number of bytes(to be used with -c option)\n"
			 "          -E ==> seek number of bytes(to be used with -c option)\n"
			 "          -o ==> extracted file to be written to this output file\n"
			 "          -t ==> count number of valid images from project file\n"
			 "          -f ==> extract the requested Nth file from multi image\n"
			 "          -h ==> print only the header information(ex:./mkimage -h 5 file) \n"
		);
	exit (-1);
}

static int print_header_only (image_header_t *hdr)
{
	time_t timestamp;
	uint32_t size;
	image_header_t temp_header;

	memcpy (&temp_header,hdr,sizeof(image_header_t));
	if(!check_header (&temp_header))
	{
		timestamp = (time_t)ntohl(hdr->ih_time);
		size = ntohl(hdr->ih_size);
		sprintf (temp_buffer,"Image Name:   %.*s\n", IH_NMLEN, hdr->ih_name);printmsg(temp_buffer);
		sprintf (temp_buffer,"Created:      %s", ctime(&timestamp));printmsg(temp_buffer);
		sprintf (temp_buffer,"Image Type:   ");printmsg(temp_buffer);print_type(hdr);
		sprintf (temp_buffer,"Data Size:    %d Bytes = %.2f kB = %.2f MB\n",
		size, (double)size / 1.024e3, (double)size / 1.048576e6 );printmsg(temp_buffer);
		sprintf (temp_buffer,"Load Address: 0x%08X\n", ntohl(hdr->ih_load));printmsg(temp_buffer);
		sprintf (temp_buffer,"Entry Point:  0x%08X\n", ntohl(hdr->ih_ep));printmsg(temp_buffer);
		return 0;
	}
	return -1;
}

static int print_header (image_header_t *hdr)
{
	uint32_t size;
	char image_name[255],image_ver[255],temp_arr1[255],image_health[255];
	image_header_t temp_header;
	int multi_image_result=0,i;

	print_header_only (hdr);

	//i=1; /*in a normal case total images will be 1*/
	if (hdr->ih_type == IH_TYPE_MULTI  || hdr->ih_type == IH_TYPE_SCRIPT)
	{
		int ptrs;
		uint32_t pos;
		unsigned long *len_ptr = (unsigned long *)((unsigned long)hdr+sizeof(image_header_t));

		char *data=(char *)len_ptr;
		/* determine number of images first (to calculate image offsets) */
		for (i=0; len_ptr[i]; ++i)	/* null pointer terminates list */
			;
		ptrs = i;		/* null pointer terminates list */

		pos = sizeof(image_header_t) + ptrs * sizeof(long);
		sprintf (temp_buffer,"Contents:\n");printmsg(temp_buffer);
		data+=(ptrs+1)*sizeof(long);
		for (i=0; len_ptr[i]; ++i)
		{
			size = ntohl(len_ptr[i]);
			memcpy (&temp_header,data,sizeof(image_header_t));
			if(!check_header ((image_header_t *)&temp_header))
			{
				char * image_data;
				uint32_t image_len;
				image_ver[0]='\0';
				sscanf((const char*)temp_header.ih_name,"%s %s %s",temp_arr1,image_name,image_ver);
				if(strlen(image_ver)==0)
				{
					strcpy(image_ver,image_name);
					strcpy(image_name,temp_arr1);
				}
				image_data = (char *)(data + sizeof(image_header_t));
				image_len  = size - sizeof(image_header_t) ;
				if (crc32 (0, image_data, image_len) != ntohl(temp_header.ih_dcrc))
				{
					sprintf(image_health,"(image cs error!)");
					multi_image_result=-1;
				}
				else
				{
					sprintf(image_health,"(image cs ok)");
				}
			}
			else
			{
				sprintf(image_name,"cs error!");
				sprintf(image_name,"xxxxxxxxx");
				sprintf(image_health,"image cs error!");
				multi_image_result=-1;
			}

			sprintf (temp_buffer,"   Image %d: %-11s %-13s %-8d Bytes = %4d kB = %d MB %s %s\n",
				i,image_name,image_ver, size, size>>10, size>>20,image_health,put_comp (temp_header.ih_comp));
			printmsg(temp_buffer);
			/* copy_file() will pad the first files to even word align */
			size += 3;
			size &= ~3;
			pos += size;
			data+=size;
		}
	}
	return multi_image_result;
}


static void
print_type (image_header_t *hdr)
{
	sprintf (temp_buffer,"%s %s %s (%s)\n",
		put_arch (hdr->ih_arch),
		put_os   (hdr->ih_os  ),
		put_type (hdr->ih_type),
		put_comp (hdr->ih_comp)
	);
	printmsg(temp_buffer);
}

static char *put_arch (int arch)
{
	return (put_table_entry(arch_name, "Unknown Architecture", arch));
}

static char *put_os (int os)
{
	return (put_table_entry(os_name, "Unknown OS", os));
}

static char *put_type (int type)
{
	return (put_table_entry(type_name, "Unknown Image", type));
}

static char *put_comp (int comp)
{
	return (put_table_entry(comp_name, "Unknown Compression", comp));
}

static char *put_table_entry (table_entry_t *table, char *msg, int type)
{
	for (; table->val>=0; ++table) {
		if (table->val == type)
			return (table->lname);
	}
	return (msg);
}

static int get_arch(char *name)
{
	return (get_table_entry(arch_name, "CPU", name));
}


static int get_comp(char *name)
{
	return (get_table_entry(comp_name, "Compression", name));
}


static int get_os (char *name)
{
	return (get_table_entry(os_name, "OS", name));
}


static int get_type(char *name)
{
	return (get_table_entry(type_name, "Image", name));
}

static int get_table_entry (table_entry_t *table, char *msg, char *name)
{
	table_entry_t *t;
	int first = 1;

	for (t=table; t->val>=0; ++t) {
		if (t->sname && strcasecmp(t->sname, name)==0)
			return (t->val);
	}
	fprintf (stderr, "\nInvalid %s Type - valid names are", msg);
	for (t=table; t->val>=0; ++t) {
		if (t->sname == NULL)
			continue;
		fprintf (stderr, "%c %s", (first) ? ':' : ',', t->sname);
		first = 0;
	}
	fprintf (stderr, "\n");
	return (-1);
}


static int printmsg(char *Msg)
{
	if(!silent_flag)
		fprintf (stderr,"%s",Msg);
	return 0;
}

static int check_header (image_header_t *hdr)
{
	int len;
	char *data;
	uint32_t checksum;
	if (ntohl(hdr->ih_magic) != IH_MAGIC)
	{
		sprintf (temp_buffer,"Bad Magic Number: its no valid image\n");
		printmsg(temp_buffer);
		//exit (-1);
		return -1;
	}

	data = (char *)hdr;

	len  = sizeof(image_header_t);

	checksum = ntohl(hdr->ih_hcrc);

	hdr->ih_hcrc = htonl(0);	/* clear for re-calculation */

	if (crc32 (0, data, len) != checksum)
	{
		sprintf (temp_buffer,"*** Warning: image has bad header checksum!\n");
		printmsg(temp_buffer);
		return -1;//exit (-1);
	}
	return 0;
}

static int check_mtdfile(char *imagefile)
{
	int file,hdr_len;
	//char temp_array[1024];
	uint32_t size;

	file = open(imagefile, O_RDONLY|O_BINARY);
	hdr_len=sizeof(image_header_t);

	if( read(file,hdr,hdr_len) < hdr_len)
	{
		printmsg("check_mtdfile:unable to read header from input file\n");
		return (-1);
	}
	close(file);

	if(check_header(hdr))
		return -1;
	size = ntohl(hdr->ih_size);

	if(file_copy(imagefile,TEMP_MTD_FILE,0,(size+hdr_len)))
		return -1;
	strcpy(mtdfile,TEMP_MTD_FILE);
	return 0;
}

static int extract_file_from_multi(uint8_t image_type,char *input_file,char *output_file,image_header_t *hdr,TypeAction action,int* img_count)
{
	uint32_t size;
	size = ntohl(hdr->ih_size);
	image_header_t temp_header;
	*img_count=0;

	if (hdr->ih_type == IH_TYPE_MULTI)
	{
		int i, ptrs;
		uint32_t pos;
		unsigned long *len_ptr = (unsigned long *) (
					(unsigned long)hdr + sizeof(image_header_t)
				);
		char *data=(char *)len_ptr;

		/* determine number of images first (to calculate image offsets) */
		for (i=0; len_ptr[i]; ++i)	/* null pointer terminates list */
			;
		ptrs = i;		/* null pointer terminates list */
		pos = sizeof(image_header_t) + ptrs * sizeof(long);
		data+=(ptrs+1)*sizeof(long);
		for (i=0; len_ptr[i]; ++i)
		{
			size = ntohl(len_ptr[i]);
			memcpy (&temp_header,data,sizeof(image_header_t));
			if(!check_header ((image_header_t *)&temp_header))
			{
				if(temp_header.ih_type == image_type)
				{
					switch(action)
					{
						case query:return 0;break;
						case compare:return file_compare(output_file,input_file,(pos+4),size);break;
						case extract:
							file_copy(input_file,output_file,(pos+4),size);
							return 0;
							break;
						case exthdr:
							file_copy(input_file,output_file,(pos+4),sizeof(image_header_t));
							return 0;
							break;
						default:return -1;


					}
					if(action==query)//requested file is found, return success.
						return 0;
					else if(action==compare)
						return file_compare(output_file,input_file,(pos+4),size);
					else
					{
						file_copy(input_file,output_file,(pos+4),size);
						return 0;
					}
				}
				(*img_count)++;
			}
			/* copy_file() will pad the first files to even word align */
			size += 3;
			size &= ~3;
			pos += size;
			data+=size;
		}
		if(action==count)
			return 0;
		sprintf (temp_buffer,"unable to find requested image in project file\n");
		printmsg(temp_buffer);
		return -1;
	}
	sprintf (temp_buffer,"input file type is not a multi type\n");
	printmsg(temp_buffer);
	return -1;
}

static int file_copy(char *inputfile,char *outputfile,uint32_t skip,uint32_t len)
{
	unsigned char out_to_std=0;
	unsigned char buffer[2050];
	uint32_t max_len=2048,read_len=0,total_len=0;
	FILE *infile, *outfile;

	if(strcmp(outputfile,"stdout") == 0 )
		out_to_std=1;
	else
		out_to_std=0;

	infile=fopen(inputfile,"r");
	if(!out_to_std)
		outfile=fopen(outputfile,"w");
	fseek(infile,skip,SEEK_CUR);

	while( (read_len=fread(buffer,sizeof(char),max_len,infile)))
	{
		if((total_len+read_len) < len)
		{
			if(out_to_std)
				fwrite(buffer,sizeof(char),read_len,stdout);
			else
				fwrite(buffer,sizeof(char),read_len,outfile);
			total_len+=read_len;
		}
		else
		{
			if(out_to_std)
				fwrite(buffer,sizeof(char),(len-total_len),stdout);
			else
				fwrite(buffer,sizeof(char),(len-total_len),outfile);
			break;
		}
	}
	fclose(infile);
	if(!out_to_std)
		fclose(outfile);
	return 0;
}
static int file_compare_binary(char *file1,char *file2,uint32_t skip,uint32_t seek,uint32_t file1_len)
{
	unsigned char f1_buff[2050],f2_buff[2050];
	uint32_t max_len=2048,i=0;
	uint32_t read_len_f1=0,read_len_f2=0,read_len_img=0;
	FILE *file1node,*file2node;

	file1node=fopen(file1,"r");
	if(file1node==NULL)
	{
		return -1;
	}
	file2node=fopen(file2,"r");
	if(file2node==NULL)
	{
		fclose(file1node);
		return -1;
	}

	if(fseek(file1node,skip,SEEK_SET) != 0 )
	{
		fclose(file1node);
		fclose(file2node);
		printf("file compare failed!(fseek on file1 failed)\n");
		return -1;
	}
	if(fseek(file2node,seek,SEEK_SET) != 0 )
	{
		fclose(file1node);
		fclose(file2node);
		printf("file compare failed!(fseek on file2 failed)\n");
		return -1;
	}

	for(i=0;i<file1_len;)
	{
		read_len_f1=fread(f1_buff,sizeof(char),max_len,file1node);
		read_len_f2=fread(f2_buff,sizeof(char),max_len,file2node);

		if((i+read_len_f1) < file1_len)
		{
			read_len_img=max_len;
		}
		else
		{
			read_len_img=file1_len-i;
		}

		if (memcmp(f1_buff,f2_buff,read_len_img)!=0 || read_len_f2 < read_len_f1 )
		{
			fclose(file1node);
			fclose(file2node);
			printf("file compare failed\n");
			return -1;
		}
		i+=read_len_img;
	}
	fclose(file1node);
	fclose(file2node);
	return 0;
}

static int file_compare(char *file2,char *file1,uint32_t skip,uint32_t proj_file_len)
{
	unsigned char img_buff[2050],prj_buff[2050];
	uint32_t max_len=2048,i=0;
	uint32_t img_size;
	uint32_t read_len_prj,read_len_img;
	FILE *imgfile,*projfile;


	image_header_t *hdr;
	imgfile=fopen(file2,"r");
	if(imgfile==NULL)
	{
		return -1;
	}

	projfile=fopen(file1,"r");
	if(projfile==NULL)
	{
		fclose(imgfile);
		return -1;
	}

	img_size=fread(img_buff,sizeof(char),sizeof(image_header_t),imgfile);
	if( img_size < sizeof(image_header_t) )
	{
		fclose(imgfile);
		fclose(projfile);
		return -1;
	}
	hdr=(image_header_t *)img_buff;
	img_size = ntohl(hdr->ih_size) + sizeof(image_header_t);

	if(img_size != proj_file_len)
	{
		fclose(imgfile);
		fclose(projfile);
		return -1;
	}

	fseek(projfile,skip,SEEK_SET);
	fseek(imgfile,0,SEEK_SET);

	for(i=0;i<proj_file_len;)
	{
		read_len_img=fread(img_buff,sizeof(char),max_len,imgfile);
		read_len_prj=fread(prj_buff,sizeof(char),max_len,projfile);

		if((i+read_len_prj) < proj_file_len)
		{
			read_len_img=max_len;
		}
		else
		{
			read_len_img=proj_file_len-i;
		}

		if(memcmp(img_buff,prj_buff,read_len_img)!=0)
		{
			fclose(imgfile);
			fclose(projfile);
			return -1;
		}
		i+=read_len_img;
	}
	fclose(imgfile);
	fclose(projfile);
	return 0;
}
static int extract_file_number(int image_no,char *input_file,char *output_file,image_header_t *hdr)
{
	uint32_t size;
	size = ntohl(hdr->ih_size);
	image_header_t temp_header;


	if (hdr->ih_type == IH_TYPE_MULTI)
	{
		int i, ptrs;
		uint32_t pos;
		unsigned long *len_ptr = (unsigned long *) (
					(unsigned long)hdr + sizeof(image_header_t)
				);
		char *data=(char *)len_ptr;

		/* determine number of images first (to calculate image offsets) */
		for (i=0; len_ptr[i]; ++i)	/* null pointer terminates list */
			;
		ptrs = i;		/* null pointer terminates list */
		pos = sizeof(image_header_t) + ptrs * sizeof(long);
		data+=(ptrs+1)*sizeof(long);
		for (i=0; len_ptr[i]; ++i)
		{
			size = ntohl(len_ptr[i]);
			memcpy (&temp_header,data,sizeof(image_header_t));
			if(!check_header ((image_header_t *)&temp_header))
			{
				if(i == image_no)
				{
					file_copy(input_file,output_file,(pos+4),size);
					return 0;
				}
			}
			/* copy_file() will pad the first files to even word align */
			size += 3;
			size &= ~3;
			pos += size;
			data+=size;
		}
		sprintf (temp_buffer,"unable to find requested image in project file\n");
		printmsg(temp_buffer);
		return -1;
	}
	sprintf (temp_buffer,"input file type is not a multi type\n");
	printmsg(temp_buffer);
	return -1;
}


unsigned char get_image_type(char *image_name)
{
	if(strcmp(image_name,"uboot") == 0 )
		return IH_TYPE_UBOOT;
	else if(strcmp(image_name,"kernel") == 0 )
		return IH_TYPE_KERNEL;
	else if(strcmp(image_name,"ramdisk") == 0 )
		return IH_TYPE_RAMDISK;
	else if(strcmp(image_name,STR_TYPE_BRBOX_PROJ) == 0 )
		return IH_TYPE_BRBOX_PROJ;
	else if(strcmp(image_name,STR_TYPE_BRBOX_STTNG) == 0 )
		return IH_TYPE_BRBOX_STTNG;
	else if(strcmp(image_name,STR_TYPE_BRBOX_USRDAT) == 0 )
		return IH_TYPE_BRBOX_USRDAT;
	else if(strcmp(image_name,STR_TYPE_BRBOX_ROOT_BTR) == 0 )
		return IH_TYPE_BRBOX_ROOT_BTR;
	else if(strcmp(image_name,STR_TYPE_BRBOX_ROOT_RP1) == 0 )
		return IH_TYPE_BRBOX_ROOT_RP1;
	else if(strcmp(image_name,STR_TYPE_BRBOX_ROOT_RP2) == 0 )
		return IH_TYPE_BRBOX_ROOT_RP2;
	else if(strcmp(image_name,STR_TYPE_BRBOX_ROOT_BBB) == 0 )
		return IH_TYPE_BRBOX_ROOT_BBB;
	else if(strcmp(image_name,STR_TYPE_BRBOX_ROOT_WDB) == 0 )
		return IH_TYPE_BRBOX_ROOT_WDB;
	else if(strcmp(image_name,STR_TYPE_BRBOX_ROOT_RP3) == 0 )
		return IH_TYPE_BRBOX_ROOT_RP3;
	else if(strcmp(image_name,STR_TYPE_BRBOX_ROOT_RP0) == 0 )
		return IH_TYPE_BRBOX_ROOT_RP0;
	else if(strcmp(image_name,STR_TYPE_BRBOX_GLMT300NV2) == 0 )
		return IH_TYPE_BRBOX_GLMT300NV2;
	else if(strcmp(image_name,STR_TYPE_BRBOX_GLAR150) == 0 )
		return IH_TYPE_BRBOX_GLAR150;
	else if(strcmp(image_name,STR_TYPE_BRBOX_SIGNATURE) == 0 )
		return IH_TYPE_BRBOX_SIGNATURE;
	else
		return IH_TYPE_INVALID;
}
