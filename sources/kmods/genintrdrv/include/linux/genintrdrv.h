#ifndef GENINTRDRV_H
#define GENINTRDRV_H

/* 
 * ===============================================
 *             GENINTRDRV Data Structures
 * ===============================================
 */

/* Convienient constant name */
#define GENINTRDRV_MODULE_NAME "genintrdrv"

/* 
 * This header file is used by both kernel code and user code. The
 * portion of the header used by kernel code is concealed from user
 * code by the __KERNEL__ ifdef.
 */
#ifdef __KERNEL__


/* 
 * We want to put datastream instrumentation points in the code in
 * general, but we also want to illustrate how to create a
 * "production" version of the module without instrumentation so we
 * control inclusion of the DSKI header with this kernel config
 * symbol. 
 * 
 * Note that we also define that we also define the HELLOWORLD_DEBUG
 * statement is used in place of printk to include narrative debug
 * statements in the datastream output.
 */
#ifdef CONFIG_KUSP_GENINTRDRV_DSKI
#include <linux/kusp/dski.h>
#define GENINTRDRV_DEBUG(fmt, args...) DSTRM_DEBUG(GENINTRDRV, DEBUG, fmt, ## args)
#else
#define GENINTRDRV_DEBUG(fmt, args...) 
#endif /* CONFIG_KUSP_HELLOWORLD_DSKI */



extern atomic_t genintrdrv_print_count;


/*
 * Required Proc File-system Struct
 *
 * Used to map entry into proc file table upon module insertion
 */
extern struct proc_dir_entry *genintrdrv_proc_entry;

#endif /* __KERNEL__*/

/* 
 * ===============================================
 *             Public API Functions
 * ===============================================
 */

/*
 * There typically needs to be a struct definition for each flavor of
 * IOCTL call.
 */
typedef struct genintrdrv_ioctl_inc_s {
	int placeholder;
} genintrdrv_ioctl_inc_t;

/* 
 * This generic union allows us to make a more generic IOCTRL call
 * interface. Each per-IOCTL-flavor struct should be a member of this
 * union.
 */
typedef union genintrdrv_ioctl_param_u {
	genintrdrv_ioctl_inc_t      set;
} genintrdrv_ioctl_param_union;


/* 
 * Used by _IOW to create the unique IOCTL call numbers. It appears
 * that this is supposed to be a single character from the examples I
 * have looked at so far. 
 */
#define GENINTRDRV_MAGIC 't'

/*
 * For each flavor of IOCTL call you will need to make a macro that
 * calls the _IOW() macro. This macro is just a macro that creates a
 * unique ID for each type of IOCTL call. It uses a combination of bit
 * shifting and OR-ing of each of these arguments to create the
 * (hopefully) unique constants used for IOCTL command values.
 */
#define GENINTRDRV_IOCTL_INCREMENT	   _IOW(GENINTRDRV_MAGIC, 1, genintrdrv_ioctl_inc_t)

#undef PDEBUG	/* undef it, just in case */
#ifdef GENINTRDRV_DRV_DEBUG
//#ifdef DEBUG
# define PDEBUG(fmt,args...) printk( "%s:%04d:%s:" fmt,GENINTRDRV_MODULE_NAME,__LINE__,__FUNCTION__, ## args)
#else
# define PDEBUG(fmt,args...) /* not debugging: nothing */
#endif
#undef PDEBUGG
#define PDEBUGG(fmt,args...) /* nothing: it's a placeholder */

#define DEBUG_MSG(logflag,fmt,args...)\
do			\
{			\
	if(logflag==1)\
	{\
	printk(KERN_DEBUG "%s:%04d:%s:"fmt,GENINTRDRV_MODULE_NAME,__LINE__,__FUNCTION__, ## args);\
	}\
} while (0)

#endif /* GENINTRDRV_H */
