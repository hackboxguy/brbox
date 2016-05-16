/*
 * genintrdrv.c - Example kmod utilizing ioctl and procfs
 *
 */
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/types.h>
#include <linux/seq_file.h>

/* 
 * This is a relative include which assumes that it is being compiled
 * from $KUSPROOT/examples/kmods/genintrdrv/kmod. Thus, the relative
 * pathname referes the corresponding directory in the example source
 * tree.
 * 
 * The Makefile in the kernel code that is called by the Makefile for
 * this module does not have an obvious way to set the
 * C_INCLUDE_PATH. The kernel module Makefile seems to hardcode the
 * include directory to be relative to the kernel area. Therefore, to
 * be able to include the header file we created for this module I
 * have been forced to use a relative pathname.
 *
 * If this module were installed to the system (via an RPM for
 * example) this assertion would be false because the header file
 * would be placed in the kernel include area.
 */
#include "../include/linux/genintrdrv.h"


/*
 * The number of times to append the line "Hello World!" to the
 * /proc/genintrdrv buffer.
 */
atomic_t genintrdrv_message_count = ATOMIC_INIT(0);

/*
 * Required Proc File-system Struct
 *
 * Used to map entry into proc file table upon module insertion
 */
struct proc_dir_entry *genintrdrv_proc_entry;


/* 
 * ===============================================
 *                Public Interface
 * ===============================================
 */

/*
 * Increament the message count. This could just as easily be done
 * within the genintrdrv_ioctl() function.
 */
static void genintrdrv_inc_message_count(void)
{
	atomic_inc(&genintrdrv_message_count);
}
/* 
 * ===============================================
 *                IOCTL Interface
 * ===============================================
 */

/*
 * Generic open call that will always be successful since there is no
 * extra setup we need to do for this module as far
 */
static int genintrdrv_open(struct inode *inode, struct file *file)
{
	return 0;
}
static int genintrdrv_close(struct inode *inode, struct file *file)
{
	return 0;
}
static long genintrdrv_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	int                               ret = 0;
	genintrdrv_ioctl_param_union      local_param;

	printk("genintrdrv_ioctl()\n");

	if (copy_from_user
	    ((void *)&local_param, (void *)ioctl_param, _IOC_SIZE(ioctl_num)))
		return  -ENOMEM;

	switch (ioctl_num)
	{
		case GENINTRDRV_IOCTL_INCREMENT:
			{
				genintrdrv_inc_message_count();
				ret = 0;
				break;
			}
		default:
			{
				printk("ioctl: no such command\n");
				ret = -EINVAL;
			}
	} /* end of switch(ioctl_num) */
	return ret;
}
/* 
 * ===============================================
 *            Proc File Table Interface
 * ===============================================
 */

/* 
 * The type and order of the parameters is standard for a /proc/ read
 * routine, at least, and possibly for all read routines but we didn't
 * go and check this assuption yet.
 *
 */
static int genintrdrv_proc_read(char *buffer,
				char **buffer_location,
				off_t offset, int buffer_length, int *eof, void *data)
{
	int ret;
	int i;

	/* 
	 * Obtain a local copy of the atomic variable that is
	 * gaurenteed not to change while in the buffer formatting
	 * logic..
	 */
	int message_count = atomic_read(&genintrdrv_message_count);

	printk("reading genintrdrv proc entry.\n");
	
	/* 
	 * We give all of our information in one go, so if the user
	 * asks us if we have more information the answer should
	 * always be no.
	 *
	 * This is important because the standard read function from
	 * the library would continue to issue the read system call
	 * until the kernel replies that it has no more information,
	 * or until its buffer is filled.
	 * 
	 */
	if (offset > 0) {
		/* we have finished to read, return 0 */
		ret  = 0;
	} else {
		/* fill the buffer, return the buffer size This
		 * assumes that the buffer passed in is big enough to
		 * hold what we put in it. More defensive code would
		 * check the input buffer_length parameter to check
		 * the validity of that assumption.
		 *
		 * Note that the return value from this call (number
		 * of characters written to the buffer) from this will
		 * be added to the current offset at the file
		 * descriptor level managed by the system code that is
		 * called by this routine.
		 */
		
		/* 
		 * Make sure we are starting off with a clean buffer;
		 */
		strcpy(buffer, "");
		for (i = 0; i < message_count; i ++) {
			/* 
			 * Now that we are sure what the buffer
			 * contains we can just append the message the
			 * desired number of times. The third argument
			 * to strncat() makes sure we do not go over
			 * the length of the buffer.
			 */
			buffer = strncat(buffer, "Hello World!\n", 
				(buffer_length - strlen(buffer)) );		
		}
		ret = strlen(buffer);
	}

	return ret;
}


/* 
 * The file_operations struct is an instance of the standard character
 * device table entry. We choose to initialize only the open, release,
 * and unlock_ioctl elements since these are the only functions we use
 * in this module.
 */
struct file_operations
genintrdrv_dev_fops = {
	.owner          = THIS_MODULE,
	.unlocked_ioctl = genintrdrv_ioctl,
	.open           = genintrdrv_open,
	.release        = genintrdrv_close,
};

/* 
 * This is an instance of the miscdevice structure which is used in
 * the genintrdrv_init routine as a part of registering the module
 * when it is loaded.
 * 
 * The device type is "misc" which means that it will be assigned a
 * static major number of 10. We deduced this by doing ls -la /dev and
 * noticed several different entries we knew to be modules with major
 * number 10 but with different minor numbers.
 * 
 */
static struct miscdevice
genintrdrv_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = GENINTRDRV_MODULE_NAME,
	.fops  = &genintrdrv_dev_fops,
};


static int hello_proc_show(struct seq_file *m, void *v)
{
  printk("Hello proc!\n");
  return 0;
}

static int hello_proc_open(struct inode *inode, struct  file *file)
{
  return single_open(file, hello_proc_show, NULL);
}

static const struct file_operations genintrdrv_proc_ops = {
  .owner = THIS_MODULE,
  .open = hello_proc_open,
  .read = genintrdrv_proc_read,//seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};

static int genintrdrv_create_proc(void)
{
//#if LINUX_VERSION_CODE >= KERNEL_VERSCONFIG_MODULE_SIG=nION(3,10) //(2,6,36)
//	genintrdrv_proc_entry = create_proc_entry(GENINTRDRV_MODULE_NAME, 
//						  S_IRUGO | S_IWUGO, NULL,&genintrdrv_dev_fops);
//	if (genintrdrv_proc_entry == NULL) return -ENOMEM;
//	genintrdrv_proc_entry->read_proc = genintrdrv_proc_read;
//#else
	proc_create_data(GENINTRDRV_MODULE_NAME, 0 /* default mode */,
			NULL /* parent dir */, &genintrdrv_proc_ops,
			NULL /* client data */);
	return 0;
//#endif
}

static void genintrdrv_remove_proc(void)
{
	/* No problem if it was not registered. */
	remove_proc_entry(GENINTRDRV_MODULE_NAME, NULL /* parent dir */);
}


/*
 * This routine is executed when the module is loaded into the
 * kernel. I.E. during the insmod command.
 *
 */
static int __init genintrdrv_init(void)
{
	int ret = 0;
	ret = misc_register(&genintrdrv_misc);/*Attempt to register the module as a misc. device with the kernel*/
	if (ret < 0) /* Registration failed so give up. */
		goto out;
	ret=genintrdrv_create_proc();
	printk("genintrdrv module installed\n");
out:
	return ret;
}

static void __exit genintrdrv_exit(void)
{ 
        remove_proc_entry(GENINTRDRV_MODULE_NAME, genintrdrv_proc_entry);
	misc_deregister(&genintrdrv_misc);
	printk("genintrdrv module uninstalled\n");
}

module_init(genintrdrv_init);
module_exit(genintrdrv_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Generic Interrupt Driver");
//MODULE_AUTHOR("Dillon Hicks");//original driver skeleton is taken from http://www.ittc.ku.edu/kusp/new/howto/kmods/setup.html
MODULE_AUTHOR("Albert David");
