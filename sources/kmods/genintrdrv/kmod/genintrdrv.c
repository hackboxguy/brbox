/*  This is the main file of generic-interrupt-driver. 
 *  how to enable debug-log?  : sudo bash -c 'echo 1 > /sys/kernel/genintrdrv/debuglog'
 *  how to check debug-log sts: cat /sys/kernel/genintrdrv/debuglog 
*/
/*****************************************************************************/
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
/*****************************************************************************/
/*static void genintrdrv_inc_message_count(void)
{
	atomic_inc(&genintrdrv_message_count);
}*/
static int genintrdrv_open(struct inode *inode, struct file *file)
{
	DEBUG_MSG(debuglogflag,"\n");
	return 0;
}
static int genintrdrv_close(struct inode *inode, struct file *file)
{
	DEBUG_MSG(debuglogflag,"\n");
	return 0;
}
static long genintrdrv_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	int                               ret = 0;
	genintrdrv_ioctl_param_union      local_param;

	DEBUG_MSG(debuglogflag,"\n");

	if (copy_from_user
	    ((void *)&local_param, (void *)ioctl_param, _IOC_SIZE(ioctl_num)))
		return  -ENOMEM;

	switch (ioctl_num)
	{
		case GENINTRDRV_IOCTL_INCREMENT:
			{
				DEBUG_MSG(debuglogflag,"GENINTRDRV_IOCTL_INCREMENT called\n");
				//genintrdrv_inc_message_count();
				ret = 0;
				break;
			}
		default:
			{
				DEBUG_MSG(debuglogflag,"no such ioctl command\n");
				ret = -EINVAL;
			}
	}
	return ret;
}
/*****************************************************************************/
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
static struct kobj_attribute debuglog_attribute  =__ATTR(debuglog, 0664, debuglogflag_show,debuglogflag_store);//enable/disable debug logging
static struct kobj_attribute interrupt_attribute =__ATTR(interrupt, 0664, interruptflag_show,interruptflag_store);//simulate interrupt arrival
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
	sysfs_kobj = kobject_create_and_add("genintrdrv", kernel_kobj);
	if(!sysfs_kobj)
		return -ENOMEM;
        ret = sysfs_create_file(sysfs_kobj, &debuglog_attribute.attr);
        if (ret) 
                printk("failed to create the debuglog file in /sys/kernel/genintrdrv \n");
        ret = sysfs_create_file(sysfs_kobj, &interrupt_attribute.attr);
        if (ret) 
                printk("failed to create the interrupt file in /sys/kernel/genintrdrv \n");
	DEBUG_MSG(1,"\n");//always print this
	return ret;
}
static void __exit genintrdrv_exit(void)
{ 
	kobject_put(sysfs_kobj);
 	misc_deregister(&genintrdrv_misc);
	DEBUG_MSG(1,"\n");//always print this
}
module_init(genintrdrv_init);
module_exit(genintrdrv_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Generic Interrupt Driver");
//MODULE_AUTHOR("Dillon Hicks");//original driver skeleton is taken from http://www.ittc.ku.edu/kusp/new/howto/kmods/setup.html
MODULE_AUTHOR("Albert David");
/*****************************************************************************/
