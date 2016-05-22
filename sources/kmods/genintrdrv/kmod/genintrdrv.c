/*  This is the main file of generic-interrupt-driver.
 *  how to insert driver with cmdargs : sudo insmod genintrdrv.ko intrlist="1,17,4,5" 
 *  how to enable debug-log?          : sudo bash -c 'echo 1 > /sys/kernel/genintrdrv/debuglog'
 *  how to check debug-log sts?       : cat /sys/kernel/genintrdrv/debuglog 
 *  how to check active subscriptions?: cat /sys/kernel/genintrdrv/subscribe
 *  how to register for a signal?     : sudo bash -c 'echo intrnum,signum,pid > /sys/kernel/genintrdrv/subscribe'
 *  ex: sudo bash -c 'echo 0,15,13756 > /sys/kernel/genintrdrv/subscribe' (register SIGTERM(15) for pid 13756 - 0 is interrupt number)
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
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <asm/io.h>
//#include <linux/ioport.h>
//#include <linux/delay.h>
//#include <linux/version.h>
#include "chain.h"
#include <genintrdrv.h>
#include <linux/kernel.h>
/*****************************************************************************/
//variables of genintrdrv module
static int debuglogflag=0;//by default debug logging is disabled
static int interruptflag=0;
static int sigcounter=0;//number of times signal has been sent
static int intcounter=0;//number of times irq function has been called
static int bhcounter=0;//number of times bottom-half(tasklet) function has been called
atomic_t genintrdrv_message_count = ATOMIC_INIT(0);
struct proc_dir_entry *genintrdrv_proc_entry;
GENINTDRV_SIGINFO SigList;
chain SubscribersList,IRQList;
static char *intrlist = "";//commandline argument passed to module during insmod.
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
int32_t add_subscriber_to_list(GENINTDRV_SIGINFO *pNewEntry,chain *pChain)
{
	GENINTDRV_SIGINFO *pEntry=NULL;
	pEntry = (GENINTDRV_SIGINFO*)kmalloc(sizeof(GENINTDRV_SIGINFO),GFP_KERNEL);
	if(pEntry==NULL)
		return -1;
	pEntry->intr    = pNewEntry->intr;
	pEntry->sig     = pNewEntry->sig;
	pEntry->pid     = pNewEntry->pid;
	pEntry->active  = pNewEntry->active;
	if(chain_put(pChain,(void *)pEntry)!=0)//failed
	{
		kfree(pEntry);
		return -1;
	}
	return 0;
}
int32_t find_duplicate_entry(int32_t intr,int32_t sig,int32_t pid,chain *pChain)
{
	GENINTDRV_SIGINFO *pEntry=NULL;
	int size=chain_size(pChain);
	int i=0;
	chain_lock(pChain);
	while(i<size)
	{
		pEntry = (GENINTDRV_SIGINFO*)chain_get_by_index(pChain,i);//get the top element and check if it has to be removed
		if(pEntry==NULL)//no valid object
		{
			chain_unlock(pChain);
			return -1;//no duplicate found
		}
		if(pEntry->intr == intr && pEntry->sig == sig && pEntry->pid == pid && pEntry->active==1) //if entry is inactive, pid might be rolled-over, and process might get same pid
		{
			chain_unlock(pChain);
			return 0;//duplicate entry found
		}
		i++;
	}
	chain_unlock(pChain);
	return -1;//no duplicate found
}
int8_t send_signal_to_process(int32_t intr,int32_t sig,int32_t pid,int32_t* tot_sig_sent)
{
	struct siginfo info;
	struct task_struct *t=NULL;

	memset(&info, 0, sizeof(struct siginfo));
	info.si_signo = sig;//SIG_TEST;
	info.si_code = SI_QUEUE;

	//send signal
	info.si_int = intr;//1234;//real time signals may have 32 bits of data.
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
		DEBUG_MSG(debuglogflag,"sent signal=%d to pid=%d with ident=%d\n",sig,pid,intr);
	}
	return 0;
}
int8_t send_signal_to_subscribers(int32_t interrupt)
{
	GENINTDRV_SIGINFO *pEntry=NULL;
	int size=chain_size(&SubscribersList);
	int i=0;
	chain_lock(&SubscribersList);
	while(i<size)
	{
		pEntry = (GENINTDRV_SIGINFO*)chain_get_by_index(&SubscribersList,i);//get the top element and check if it has to be removed
		if(pEntry!=NULL)//no valid object
		{
			if(pEntry->intr==interrupt && pEntry->active==1)
			{
				if(send_signal_to_process(pEntry->intr,pEntry->sig,pEntry->pid,&sigcounter)!=0)
				{
					//process is not alive anymore, deactivate this entry and clean it later
					pEntry->active=0;
				}
			}
		}
		i++;
	}
	chain_unlock(&SubscribersList);
	//TODO: remove inactive entries: //void *chain_remove_by_triple_ident(chain *pChain,int32_t ident1,int32_t ident2,int32_t ident3,tpdatIdfunc_t cust_func)
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
	send_signal_to_subscribers(interruptflag);
	//send the signal to subscribers from the list
        return count;
}
static ssize_t intrsubscribe_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
	//print the whole subscription list from chain
	GENINTDRV_SIGINFO *pEntry=NULL;
	char mybuf[512];sprintf(buf,"");
	int size=chain_size(&SubscribersList);
	int i=0;
	chain_lock(&SubscribersList);
	while(i<size)
	{
		pEntry = (GENINTDRV_SIGINFO*)chain_get_by_index(&SubscribersList,i);//get the top element and check if it has to be removed
		if(pEntry!=NULL)//no valid object
		{
			if(pEntry->active==1) //there could be inactive entries
			{
				sprintf(mybuf, "%d,%d,%d\n", pEntry->intr,pEntry->sig,pEntry->pid);
				strcat(buf,mybuf);
			}
		}
		i++;
	}
	chain_unlock(&SubscribersList);
        return strlen(buf);//sprintf(buf, "%d,%d,%d\n", SigList.intr,SigList.sig,SigList.pid);
}
static ssize_t intrsubscribe_store(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf, size_t count)
{
        sscanf(buf, "%d,%d,%d", &SigList.intr,&SigList.sig,&SigList.pid);
	//add a new subscriber if there is no existing subscription
	if(find_duplicate_entry(SigList.intr,SigList.sig,SigList.pid,&SubscribersList)!=0)
	{
		//TODO: check if requested interrupt is already active
		SigList.active=1;//keep this entry active till next chance of event-sending
		if(add_subscriber_to_list(&SigList,&SubscribersList)!=0)
			DEBUG_MSG(debuglogflag,"unable to add new subscription!!!   intr=%d signal=%d pid=%d\n",SigList.intr,SigList.sig,SigList.pid);
		else
			DEBUG_MSG(debuglogflag,"subscription success intr=%d signal=%d pid=%d\n",SigList.intr,SigList.sig,SigList.pid);
	}
	else // duplicate entry, this subscription is already available
	{
		DEBUG_MSG(debuglogflag,"duplicate entry!!! intr=%d signal=%d pid=%d\n",SigList.intr,SigList.sig,SigList.pid);
	}
	return count;
}
static ssize_t sigcount_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
        return sprintf(buf, "sigcount=%d intrcount=%d bhcount=%d\n",sigcounter,intcounter,bhcounter);
}
static ssize_t sigcount_store(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf, size_t count)
{
        sscanf(buf, "%d", &sigcounter);
        return count;
}
static ssize_t activeirq_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
        return sprintf(buf, "none\n");//TODO
}
static ssize_t activeirq_store(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf, size_t count)
{
        //sscanf(buf, "%d", &sigcounter);//nothing to do
        return count;
}
static struct kobj_attribute debuglog_attribute  =__ATTR(debuglog , 0664, debuglogflag_show,debuglogflag_store);//enable/disable debug logging
static struct kobj_attribute interrupt_attribute =__ATTR(interrupt, 0664, interruptflag_show,interruptflag_store);//simulate interrupt arrival
static struct kobj_attribute subscribe_attribute =__ATTR(subscribe, 0664, intrsubscribe_show,intrsubscribe_store);//subscribe for interrupt
static struct kobj_attribute sigcount_attribute  =__ATTR(sigcount , 0664, sigcount_show,sigcount_store);//show total signals sent
static struct kobj_attribute activeirq_attribute =__ATTR(activeirq, 0664, activeirq_show,activeirq_store);//show active interrupts
static struct kobject *sysfs_kobj;
/*****************************************************************************/
//irq related stuff
int which_interrupt=-1;
void irq_tasklet_bh(unsigned long);
DECLARE_TASKLET(irq_tasklet, irq_tasklet_bh, 0);
DEFINE_SPINLOCK(irq_lock);
void irq_tasklet_bh(unsigned long hits) 
{
	bhcounter++;
	spin_lock(&irq_lock);
	//scode = scancode; //TODO: shared resource between top-half and bottom-half
	send_signal_to_subscribers(which_interrupt);//TODO:dequeue the interrupt number from chain
	spin_unlock(&irq_lock);
	return;
}
irq_handler_t irq_handler (int irq, void *dev_id, struct pt_regs *regs) 
{
	intcounter++;//number of times interrupt routine has been called
	spin_lock(&irq_lock);
	//scancode = inb (0x60);
	which_interrupt=irq;//TODO:queue the interrupt number to chain
	spin_unlock(&irq_lock);
	tasklet_schedule(&irq_tasklet);//schedule the tasklet
	return (irq_handler_t) IRQ_HANDLED;
}
/*****************************************************************************/
int32_t find_duplicate_irq_entry(int32_t irq,chain *pChain)
{
	GENINTDRV_IRQINFO *pEntry=NULL;
	int size=chain_size(pChain);
	int i=0;
	chain_lock(pChain);
	while(i<size)
	{
		pEntry = (GENINTDRV_IRQINFO*)chain_get_by_index(pChain,i);
		if(pEntry==NULL)//no valid object
		{
			chain_unlock(pChain);
			return -1;//no duplicate found
		}
		if(pEntry->irqnum == irq) //entry already exists
		{
			chain_unlock(pChain);
			return 0;//duplicate entry found
		}
		i++;
	}
	chain_unlock(pChain);
	return -1;//no duplicate found
}
int32_t add_irq_to_list(GENINTDRV_IRQINFO *pNewEntry,chain *pChain)
{
	GENINTDRV_IRQINFO *pEntry=NULL;
	pEntry = (GENINTDRV_IRQINFO*)kmalloc(sizeof(GENINTDRV_IRQINFO),GFP_KERNEL);
	if(pEntry==NULL)
		return -1;
	pEntry->irqnum    = pNewEntry->irqnum;
	pEntry->irqsts    = pNewEntry->irqsts;
	if(chain_put(pChain,(void *)pEntry)!=0)//failed
	{
		kfree(pEntry);
		return -1;
	}
	return 0;
}
int scan_cmdline_irqlist(char *str,chain *pIRQList)
{
	int maxcount=0;long intrnum;	
	char *token = str;
	char *end = str;
	GENINTDRV_IRQINFO IRQItem;

	while (token != NULL)
	{
		intrnum = simple_strtol(token,NULL,10);//convert comma separated char-string to int value
		//DEBUG_MSG(1,"val=%d strlen=%d\n",intrnum,strlen(token));
		if(find_duplicate_irq_entry((int32_t)intrnum,pIRQList)!=0)
		{
			IRQItem.irqnum=(int32_t)intrnum;
			IRQItem.irqsts=0;//irq is not registered with kernel yet
			if(add_irq_to_list(&IRQItem,pIRQList)!=0)
				DEBUG_MSG(1,"unable to add %d irq to list!!!\n",intrnum);
		}
		else // duplicate entry, this subscription is already available
		{
			DEBUG_MSG(1,"duplicate entry fount for irq=%d!!!\n",intrnum);
		}

		strsep(&end, ",");
		token = end;
		if(maxcount++>1023)break;//incase something goes wrong, then break this loop
	}
	return 0;
}
int register_irqlist(chain *pIRQList)
{
	//GENINTDRV_IRQINFO
	//ret = request_irq (1, (irq_handler_t) irq_handler, IRQF_SHARED, "genintdrv", (void *)(irq_handler));
	//if (ret)
	//	DEBUG_MSG(1,"can't get shared interrupt for keyboard\n");
	//TODO:
	return 0;
}
int free_irqlist(chain *pIRQList)
{
	//GENINTDRV_IRQINFO
	//free_irq(1, (void *)(irq_handler)); /* i can't pass NULL, this is a shared interrupt handler! */
	//TODO:
	return 0;
}
/*****************************************************************************/
//genintrdrv module related stuff
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

	chain_init(&SubscribersList);
	chain_init(&IRQList);

	//parse the cmdline args, and check which interrupts are requested to be active
	DEBUG_MSG(1,"cmdarg=%s\n",intrlist);
	scan_cmdline_irqlist(intrlist,&IRQList);
	register_irqlist(&IRQList);
	//ret = request_irq (1, (irq_handler_t) irq_handler, IRQF_SHARED, "genintdrv", (void *)(irq_handler));
	//if (ret)
	//	DEBUG_MSG(1,"can't get shared interrupt for keyboard\n");
	DEBUG_MSG(1,"\n");//always print this
	return ret;
}
static void __exit genintrdrv_exit(void)
{ 
	//free_irq(1, (void *)(irq_handler)); /* i can't pass NULL, this is a shared interrupt handler! */
	free_irqlist(&IRQList);
	kobject_put(sysfs_kobj);
 	misc_deregister(&genintrdrv_misc);
	chain_release(&IRQList);
	chain_release(&SubscribersList);
	DEBUG_MSG(1,"\n");//always print this
}
module_init(genintrdrv_init);
module_exit(genintrdrv_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Generic Interrupt Driver");
//MODULE_AUTHOR("Dillon Hicks");//original driver skeleton is taken from http://www.ittc.ku.edu/kusp/new/howto/kmods/setup.html
MODULE_AUTHOR("Albert David");
module_param(intrlist, charp, 0000);
MODULE_PARM_DESC(intrlist, "comma separated list of interrupts to be active");//read cmdline args for knowing which interrupts to be activated
/*****************************************************************************/
//todo: provide a timer based interrupt generation
//todo: when irq number -1 is requested during insmod, treat this as simulation mode
//todo: IRQ handler with top and bottom-half implementation
//how to pass parameters to module? http://www.tldp.org/LDP/lkmpg/2.4/html/x354.htm

