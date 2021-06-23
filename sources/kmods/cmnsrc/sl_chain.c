
/* - module description ---------------------------------------------------- */
/* - doxygen conformant description about the functionality of this module - */
/* ######################################################################### */
/** @file          fpga_driver_sl_chain.c
 *  @author        Albert David
 *  @brief         generic chain handler with spinlock
 *
 *  This file includes basic functionalities of a chain handler
 *
 *  Owner:         Albert David
 *  Create Date:   2009/10/22
 *  Last change:   $Date: 2016-01-14 11:40:50 +0100 (Do, 14 Jan 2016) $
 *  Last modifier: $Author: kaps $
 *  Revision:      $Revision: 21540 $
 *
 */
/* ######################################################################### */
#include <linux/slab.h>
#include <linux/gfp.h>
#include "fpga_driver_sl_chain.h"
/******************************************************************************
 * Variable and Datatype declarations
 *****************************************************************************/
/******************************************************************************
 * Function Prototypes
 *****************************************************************************/
static int32_t sl_chain_add(sl_chain *pChain,void *data);
static int32_t sl_chain_remove(sl_chain *pChain);//, void *data );
static int32_t sl_chain_remove_all(sl_chain *pChain);
/*****************************************************************************/
int32_t sl_chain_init( sl_chain *pChain)
{
	pChain->chain_size=0;
	pChain->chain_peak_size=0;
	pChain->base_chain.pNext = NULL;
	spin_lock_init(&pChain->sl);
	pChain->init_flag=1;
	return 0;
}
/*****************************************************************************/
int32_t sl_chain_release(sl_chain *pChain)
{
	unsigned long flags;
	spin_lock_irqsave(&pChain->sl,flags);
	sl_chain_remove_all(pChain);
	pChain->init_flag=0;//tell everyone not to use chain
	spin_unlock_irqrestore(&pChain->sl,flags);
	//TODO: implement DECLARE_COMPLETION
	return 0;
}
/*****************************************************************************/
//for external access to the chain
void sl_chain_lock(sl_chain *pChain,unsigned long *flags)
{
	spin_lock_irqsave(&pChain->sl,*flags);
}
/*****************************************************************************/
//for external access to the chain
void sl_chain_unlock(sl_chain *pChain,unsigned long *flags)
{
	spin_unlock_irqrestore(&pChain->sl,*flags);
}

/*****************************************************************************/
//returns the data of first element but doesnt remove the element from chain
void* sl_chain_get_top(sl_chain *pChain)
{
	void *data;
	unsigned long flags;
	struct chain_holder *pHolder;

	spin_lock_irqsave(&pChain->sl,flags);
	if(!pChain->init_flag)//check if chain has been released
	{
		spin_unlock_irqrestore(&pChain->sl,flags);
		return NULL;
	}
	pHolder = pChain->base_chain.pNext;
	// the very first is the oldest
	if (pHolder == NULL)
	{
		spin_unlock_irqrestore(&pChain->sl,flags);
		return NULL;//TODO E_ERROR;
	}
	//call custom function to copy custom data
	data=pHolder->pData;
	spin_unlock_irqrestore(&pChain->sl,flags);
	return data;
}
/*****************************************************************************/
int32_t sl_chain_put(sl_chain *pChain,void *data)
{
	int32_t iRet = 0;
	unsigned long flags;
	spin_lock_irqsave(&pChain->sl,flags);
	if(!pChain->init_flag)//check if chain has been released
	{
		spin_unlock_irqrestore(&pChain->sl,flags);
		return -1;
	}
	iRet=sl_chain_add(pChain,data);
	spin_unlock_irqrestore(&pChain->sl,flags);
	return iRet;
}
/*****************************************************************************/
void * sl_chain_get(sl_chain *pChain)
{
	int32_t iRet;
	struct chain_holder *pHolder;
	unsigned long flags;
	void *data;

	spin_lock_irqsave(&pChain->sl,flags);

	if(!pChain->init_flag)//check if chain has been released
	{
		spin_unlock_irqrestore(&pChain->sl,flags);
		return NULL;
	}

	pHolder = pChain->base_chain.pNext;
	// the very first is the oldest
	if (pHolder == NULL)
	{
		spin_unlock_irqrestore(&pChain->sl,flags);
		return NULL;//TODO E_ERROR;
	}

	//call custom function to copy custom data
	//(*pChain->data_copy_fn)(data,pHolder->pData);
	data=pHolder->pData;

	// remove from chain
	iRet=sl_chain_remove(pChain);//,pHolder->pData);//TODO pData parameter is not needed
	spin_unlock_irqrestore(&pChain->sl,flags);

	return data;//TODO E_OKAY;
}
/*****************************************************************************/
void *sl_chain_remove_by_ident(sl_chain *pChain,int32_t ident,datIdfunc_t custom_func)
{
	void *data=NULL;
	struct chain_holder *pHolder = NULL;

	unsigned long flags;
	spin_lock_irqsave(&pChain->sl,flags);

	pHolder =  pChain->base_chain.pNext;

	if(!pChain->init_flag)//check if chain has been released
	{
		spin_unlock_irqrestore(&pChain->sl,flags);
		return NULL;
	}

	if (pHolder == NULL)
	{
		spin_unlock_irqrestore(&pChain->sl,flags);
		return NULL;
	}

	//user did not assign function pointer which identifies the data
	if(custom_func == NULL)
	{
		spin_unlock_irqrestore(&pChain->sl,flags);
		return NULL;
	}

	// search last in chain
	while (pHolder != NULL)
	{ 
		//if( (*pChain->data_identify_func)(pHolder->pData,ident) == 0 )
		if( (custom_func)(pHolder->pData,ident) == 0 )
			break;
		pHolder = pHolder->pNext; 
	}

	if(pHolder==NULL)
	{
		spin_unlock_irqrestore(&pChain->sl,flags);
		return NULL;
	}
	else
	{
		data=pHolder->pData;
		//TODO remove chain and return data
		if (pHolder->pNext != NULL) 
		{ 	// unlink chain element
			pHolder->pPrev->pNext = pHolder->pNext; 
			pHolder->pNext->pPrev = pHolder->pPrev; 
		} 
		else // pResp is the last in chain 
			pHolder->pPrev->pNext = NULL; 
	
		kfree(pHolder);
		pChain->chain_size--;
	}

	spin_unlock_irqrestore(&pChain->sl,flags);
	return data;
}
/*****************************************************************************/
//this function is same as get_by_ident, except it searches an element
//that matches double ident values
void *sl_chain_get_by_double_ident(sl_chain *pChain,int32_t ident1,int32_t ident2,dbdatIdfunc_t cust_func)
{
	struct chain_holder *pHolder = pChain->base_chain.pNext;
	if (pHolder == NULL)
		return NULL;

	//user did not assign function pointer which identifies the data
	if(cust_func==NULL)
		return NULL;

	// search last in chain
	while (pHolder != NULL)
	{ 
		if( (cust_func)(pHolder->pData,ident1,ident2) == 0 )
			break;
		pHolder = pHolder->pNext; 
	}

	if(pHolder==NULL)
		return NULL;
	else
		return pHolder->pData;
}
/*****************************************************************************/
static int32_t sl_chain_remove( sl_chain *pChain)//, void *data )
{
	// there is alvays a previous link 
	struct chain_holder *pHolder = pChain->base_chain.pNext;
	if (pHolder == NULL)
		return -1;//TODO E_ERROR;

	// check link
	if (pHolder->pNext != NULL) 
	{ 	// unlink pResp 
		pHolder->pPrev->pNext = pHolder->pNext; 
		pHolder->pNext->pPrev = pHolder->pPrev; 
	} 
	else // pResp is the last in chain 
		pHolder->pPrev->pNext = NULL; 

	// finally destroy request holder 
	kfree(pHolder);
	// decrement chain size
	pChain->chain_size--;
	return 0;//TODO E_OKAY;
}
/*****************************************************************************/
int32_t sl_chain_size(sl_chain *pChain)
{
	int32_t size;
	unsigned long flags;
	spin_lock_irqsave(&pChain->sl,flags);
	size=pChain->chain_size;
	spin_unlock_irqrestore(&pChain->sl,flags);
	return size;
}
int32_t sl_chain_peak_size(sl_chain *pChain)
{
	int32_t size;
	unsigned long flags;
	spin_lock_irqsave(&pChain->sl,flags);
	size=pChain->chain_peak_size;
	spin_unlock_irqrestore(&pChain->sl,flags);
	return size;
}
/*****************************************************************************/
static int32_t sl_chain_add(sl_chain *pChain,void *data)
{
	//int32_t data_size=0;
	// assign pointer to base holder
	struct chain_holder *pHolder = &pChain->base_chain;
	struct chain_holder * pNewHolder;
	//void *	pNewRequest;

	// crete new response holder
	pNewHolder = (struct chain_holder*)kmalloc(sizeof(struct chain_holder),GFP_ATOMIC);
	if (pNewHolder == NULL)
		return -1;//TODO E_ERROR;

	//call user function pointer to get the custom data size
	//data_size = (*pChain->data_size_fn)();

	// crete new driver request
	//pNewRequest = kmalloc(data_size,GFP_KERNEL);
	//if (pNewRequest == NULL)
		//return -1;//TODO E_ERROR;

	//(*pChain->data_copy_fn)(pNewRequest,data);
	// assign new response to new holder
	//pNewHolder->pData = pNewRequest;
	pNewHolder->pData = data;

	// search last in chain
	while (pHolder->pNext != NULL) 
		pHolder = pHolder->pNext; 

	// add from tail
	pHolder->pNext    = pNewHolder; 
	pNewHolder->pPrev = pHolder; 
	pNewHolder->pNext = NULL;  // already done but !
	// increment chain size
	pChain->chain_size++;
	if(pChain->chain_size > pChain->chain_peak_size)
		pChain->chain_peak_size=pChain->chain_size;//record the max peak reached
		
	return 0;//TODO E_OKAY;
}
/*****************************************************************************/
static int32_t sl_chain_remove_all(sl_chain *pChain)
{
	// check the chain link 
	while (pChain->base_chain.pNext != NULL ) 
	{
		kfree(pChain->base_chain.pNext->pData);//remove all data references before removing chain
		sl_chain_remove (pChain);//,pChain->base_chain.pNext->pData);
	}
	return 0;//TODO E_OKAY; 
}
/*****************************************************************************/
