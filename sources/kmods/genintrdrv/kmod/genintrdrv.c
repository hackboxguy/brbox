/*  This is the main file of generic-interrupt-driver. 
 *  how to enable debug-log?  : sudo bash -c 'echo 1 > /sys/kernel/genintrdrv/debuglog'
 *  how to check debug-log sts: cat /sys/kernel/genintrdrv/debuglog 
 *  how to register for a signal: sudo bash -c 'echo intrnum,signum,pid > /sys/kernel/genintrdrv/subscribe'
 *  ex: sudo bash -c 'echo 0,15,13756 > /sys/kernel/genintrdrv/subscribe' (register SIGTERM for pid 13756)
 *      sudo bash -c 'echo 0 > /sys/kernel/genintrdrv/interrupt' (this will terminate process id 13756)
*/  
/*****************************************************************************/
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/types.h>
#include <linux/seq_file.h>
#include <linux/pid.h>
#include <linux/sched.h>
//#include <linux/ioport.h>
//#include <linux/slab.h>
//#include <linux/delay.h>
//#include <linux/version.h>
#include <genintrdrv.h>
static int debuglogflag=0;
static int interruptflag=0;
static int sigcounter=0;
atomic_t genintrdrv_message_count = ATOMIC_INIT(0);
struct proc_dir_entry *genintrdrv_proc_entry;
GENINTDRV_SIGINFO SigList;
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
int8_t send_signal_to_process(int32_t pid,int32_t sig,int32_t ident,int32_t* tot_sig_sent)
{
	struct siginfo info;
	struct task_struct *t=NULL;

	memset(&info, 0, sizeof(struct siginfo));
	info.si_signo = sig;//SIG_TEST;
	info.si_code = SI_QUEUE;

	//send signal
	info.si_int = ident;//1234;//real time signals may have 32 bits of data.
	rcu_read_lock();
	t=pid_task(find_vpid(pid),PIDTYPE_PID);
	rcu_read_unlock();
	if(t == NULL)//user process is no more alive
	{
		//TODO remove the response object from resp_chain
		DEBUG_MSG(debuglogflag,"pid %d not alive, unable to send signal!!\n",pid);
		return -1;
	}
	else
	{
		send_sig_info(sig/*SIG_TEST*/, &info,t);
		*tot_sig_sent+=1;//keep the statistics for debug
		DEBUG_MSG(debuglogflag,"sent signal=%d to pid=%d with ident=%d\n",sig,pid,ident);
	}
	return 0;
}
/*****************************************************************************/
//sysfs related stuff
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
	//TODO:search items by interruptflag ident and send signals to matching entries
	send_signal_to_process(SigList.pid,SigList.sig,SigList.intr,&sigcounter);//use scanned interrupt num
        return count;
}
static ssize_t intrsubscribe_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
	//TODO: print the whole subscription list from chain
        return sprintf(buf, "%d,%d,%d\n", SigList.intr,SigList.sig,SigList.pid);
}
static ssize_t intrsubscribe_store(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf, size_t count)
{
        sscanf(buf, "%d,%d,%d", &SigList.intr,&SigList.sig,&SigList.pid);
        return count;
}
static ssize_t sigcount_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
        return sprintf(buf, "%d\n", sigcounter);
}
static ssize_t sigcount_store(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf, size_t count)
{
        sscanf(buf, "%d", &sigcounter);
        return count;
}
static struct kobj_attribute debuglog_attribute  =__ATTR(debuglog , 0664, debuglogflag_show,debuglogflag_store);//enable/disable debug logging
static struct kobj_attribute interrupt_attribute =__ATTR(interrupt, 0664, interruptflag_show,interruptflag_store);//simulate interrupt arrival
static struct kobj_attribute subscribe_attribute =__ATTR(subscribe, 0664, intrsubscribe_show,intrsubscribe_store);//subscribe for interrupt
static struct kobj_attribute sigcount_attribute  =__ATTR(sigcount , 0664, sigcount_show,sigcount_store);//show total signals sent
static struct kobject *sysfs_kobj;
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

	//create sysfs node for debuglog
        ret = sysfs_create_file(sysfs_kobj, &debuglog_attribute.attr);
        if (ret) 
                printk("failed to create /sys/kernel/genintrdrv/debuglog \n");

	//create sysfs node for interrupt generation
         ret = sysfs_create_file(sysfs_kobj, &interrupt_attribute.attr);
        if (ret) 
                printk("failed to create /sys/kernel/genintrdrv/interrupt\n");

	//create sysfs node for interrupt generation
         ret = sysfs_create_file(sysfs_kobj, &subscribe_attribute.attr);
        if (ret) 
                printk("failed to create /sys/kernel/genintrdrv/subscribe\n");

	//create sysfs node for reading total signal sent
         ret = sysfs_create_file(sysfs_kobj, &sigcount_attribute.attr);
        if (ret) 
                printk("failed to create /sys/kernel/genintrdrv/sigcount\n");

	//TODO: initialize registration list
	//chain_init(&SubscribersList);

	DEBUG_MSG(1,"\n");//always print this
	return ret;
}
static void __exit genintrdrv_exit(void)
{ 
	kobject_put(sysfs_kobj);
 	misc_deregister(&genintrdrv_misc);
	//TODO: cleanup list
	//chain_release(&SubscribersList);
	DEBUG_MSG(1,"\n");//always print this
}
module_init(genintrdrv_init);
module_exit(genintrdrv_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Generic Interrupt Driver");
//MODULE_AUTHOR("Dillon Hicks");//original driver skeleton is taken from http://www.ittc.ku.edu/kusp/new/howto/kmods/setup.html
MODULE_AUTHOR("Albert David");
/*****************************************************************************/

	

