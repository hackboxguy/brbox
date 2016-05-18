#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/types.h>
#include <linux/seq_file.h>
#include <genintrdrv.h>
static int debuglogflag=0;
static int interruptflag=0;
atomic_t genintrdrv_message_count = ATOMIC_INIT(0);
struct proc_dir_entry *genintrdrv_proc_entry;
static void genintrdrv_inc_message_count(void)
{
	atomic_inc(&genintrdrv_message_count);
}
static int genintrdrv_open(struct inode *inode, struct file *file)
{
	printk("genintrdrv_open()\n");
	return 0;
}
static int genintrdrv_close(struct inode *inode, struct file *file)
{
	printk("genintrdrv_close()\n");
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
				printk("genintrdrv: GENINTRDRV_IOCTL_INCREMENT called\n");
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
static int genintrdrv_proc_read(char *buffer,
				char **buffer_location,
				off_t offset, int buffer_length, int *eof, void *data)
{
	int ret;
	int i;
	int message_count = atomic_read(&genintrdrv_message_count);
	DEBUG_MSG(debuglogflag,"reading genintrdrv proc entry.\n");
	//DEBUG_MSG(logflag,fmt,args...)

	if (offset > 0) 
	{
		/* we have finished to read, return 0 */
		ret  = 0;
	}
	else 
	{
		strcpy(buffer, "");
		for (i = 0; i < message_count; i ++) 
		{
			buffer = strncat(buffer, "Hello World!\n", 
				(buffer_length - strlen(buffer)) );		
		}
		ret = strlen(buffer);
	}
	return ret;
}
struct file_operations
genintrdrv_dev_fops = {
	.owner          = THIS_MODULE,
	.unlocked_ioctl = genintrdrv_ioctl,
	.open           = genintrdrv_open,
	.release        = genintrdrv_close,
};
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

static ssize_t debuglogflag_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
        return sprintf(buf, "%d\n", debuglogflag);
}
static ssize_t debuglogflag_store(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf, size_t count)
{
        sscanf(buf, "%d", &debuglogflag);
        return count;
}
static ssize_t interruptflag_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
        return sprintf(buf, "%d\n", interruptflag);
}
static ssize_t interruptflag_store(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf, size_t count)
{
        sscanf(buf, "%d", &interruptflag);
        return count;
}
static struct kobj_attribute debuglog_attribute  =__ATTR(debuglog, 0666, debuglogflag_show,debuglogflag_store);
static struct kobj_attribute interrupt_attribute =__ATTR(interrupt, 0666, interruptflag_show,interruptflag_store);
static struct kobject *sysfs_kobj;
static int __init genintrdrv_init(void)
{
	int ret = 0;
	ret = misc_register(&genintrdrv_misc);/*Attempt to register the module as a misc. device with the kernel*/
	if (ret < 0) /* Registration failed so give up. */
	{
		printk("%s: misc registration failed!!!!\n",GENINTRDRV_MODULE_NAME);
		return ret;
	}
	ret=genintrdrv_create_proc();
	sysfs_kobj = kobject_create_and_add("genintrdrv", kernel_kobj);
	if(!sysfs_kobj)
		return -ENOMEM;
        ret = sysfs_create_file(sysfs_kobj, &debuglog_attribute.attr);
        if (ret) 
                printk("failed to create the debuglog file in /sys/kernel/kobject_example \n");
        ret = sysfs_create_file(sysfs_kobj, &interrupt_attribute.attr);
        if (ret) 
                printk("failed to create the interrupt file in /sys/kernel/kobject_example \n");

	printk("%s: init\n",GENINTRDRV_MODULE_NAME);
	return ret;
}
static void __exit genintrdrv_exit(void)
{ 
	kobject_put(sysfs_kobj);
        remove_proc_entry(GENINTRDRV_MODULE_NAME, genintrdrv_proc_entry);
	misc_deregister(&genintrdrv_misc);
	printk("%s: exit\n",GENINTRDRV_MODULE_NAME);
}
module_init(genintrdrv_init);
module_exit(genintrdrv_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Generic Interrupt Driver");
//MODULE_AUTHOR("Dillon Hicks");//original driver skeleton is taken from http://www.ittc.ku.edu/kusp/new/howto/kmods/setup.html
MODULE_AUTHOR("Albert David");
