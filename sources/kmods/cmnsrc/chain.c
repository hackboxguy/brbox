/* ######################################################################### */
#include <linux/slab.h>
#include <linux/fs.h>
#include "chain.h"
/******************************************************************************
 * Variable and Datatype declarations
 *****************************************************************************/
/******************************************************************************
 * Function Prototypes
 *****************************************************************************/
static int32_t chain_add(chain *pChain,void *data);
static int32_t chain_remove( chain *pChain);//, void *data );
static int32_t chain_remove_all(chain *pChain);
/*****************************************************************************/
int32_t chain_init( chain *pChain)//,typ_custom_data_identify data_id_func)
{
	pChain->chain_size=0;
	pChain->chain_peak_size=0;
	pChain->base_chain.pNext = NULL;
	sema_init(&pChain->sema, 1);
	pChain->init_flag=1;
	return 0;
}
/*****************************************************************************/
int32_t chain_release(chain *pChain)
{
	down(&pChain->sema);
	chain_remove_all(pChain);
	pChain->init_flag=0;//tell everyone not to use chain
	up(&pChain->sema);
	//TODO: implement DECLARE_COMPLETION
	return 0;
}
/*****************************************************************************/
//for external access to the chain
void chain_lock(chain *pChain)
{
	down(&pChain->sema);
}
/*****************************************************************************/
//for external access to the chain
void chain_unlock(chain *pChain)
{
	up(&pChain->sema);
}
/*****************************************************************************/
int32_t chain_put(chain *pChain,void *data)
{
	int32_t iRet = 0;
	down(&pChain->sema);
	if(!pChain->init_flag)//check if chain has been released
	{
		up(&pChain->sema);
		return -1;
	}
	iRet=chain_add(pChain,data);
	up(&pChain->sema);
	return iRet;
}
/*****************************************************************************/
//before calling this function, user should take the lock,modify data and then 
//release lock
void *chain_get_by_ident(chain *pChain,int32_t ident, datIdfunc_t custom_func)
{
	struct chain_holder *pHolder = pChain->base_chain.pNext;
	if (pHolder == NULL)
		return NULL;

	//pChain->data_identify_func=custom_func;

	//user did not assign function pointer which identifies the data
	//if(pChain->data_identify_func==NULL)
	if(custom_func == NULL)
			return NULL;

	// search last in chain
	while (pHolder != NULL)
	{ 
		//if( (*pChain->data_identify_func)(pHolder->pData,ident) == 0 )
		if( (custom_func)(pHolder->pData,ident) == 0 )
			break;
		pHolder = pHolder->pNext; 
	}

	if(pHolder==NULL)
		return NULL;
	else
		return pHolder->pData;

}
/*****************************************************************************/
void *chain_remove_by_ident(chain *pChain,int32_t ident,datIdfunc_t custom_func)
{
	void *data=NULL;
	struct chain_holder *pHolder = NULL;

	down(&pChain->sema);
	
	pHolder = 	pChain->base_chain.pNext;

	if(!pChain->init_flag)//check if chain has been released
	{
		up(&pChain->sema);
		return NULL;
	}

	if (pHolder == NULL)
	{
		up(&pChain->sema);
		return NULL;
	}

	//pChain->data_identify_func=custom_func;

	//user did not assign function pointer which identifies the data
	//if(pChain->data_identify_func==NULL)
	if(custom_func == NULL)
	{
		up(&pChain->sema);
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
		up(&pChain->sema);
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

	up(&pChain->sema);
	return data;
}
/*****************************************************************************/
//searches and removes a chain element based on its dual identities
void *chain_remove_by_double_ident(chain *pChain,int32_t ident1,int32_t ident2,dbdatIdfunc_t cust_func)
{
	void *data=NULL;
	struct chain_holder *pHolder = NULL;

	down(&pChain->sema);

	pHolder = pChain->base_chain.pNext;

	if(!pChain->init_flag)//check if chain has been released
	{
		up(&pChain->sema);
		return NULL;
	}

	if (pHolder == NULL)
	{
		up(&pChain->sema);
		return NULL;
	}

	//pChain->double_data_ident_func=cust_func;

	//user did not assign function pointer which identifies the data
	//if(pChain->double_data_ident_func==NULL)
	if(cust_func==NULL)
	{
		up(&pChain->sema);
		return NULL;
	}

	// search last in chain
	while (pHolder != NULL)
	{ 
		//if( (*pChain->double_data_ident_func)(pHolder->pData,ident1,ident2) == 0 )
		if( (cust_func)(pHolder->pData,ident1,ident2) == 0 )	
			break;
		//if( (*pChain->data_identify_func)(pHolder->pData,ident) == 0 )
		//	break;
		pHolder = pHolder->pNext; 
	}

	if(pHolder==NULL)
	{
		up(&pChain->sema);
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

	up(&pChain->sema);
	return data;
}
/*****************************************************************************/
//this function is same as get_by_ident, except it searches an element
//that matches double ident values
void *chain_get_by_double_ident(chain *pChain,int32_t ident1,int32_t ident2,dbdatIdfunc_t cust_func)
{
	struct chain_holder *pHolder = pChain->base_chain.pNext;
	if (pHolder == NULL)
		return NULL;

	//user did not assign function pointer which identifies the data
	//if(pChain->double_data_ident_func==NULL)
	if(cust_func==NULL)
		return NULL;

	// search last in chain
	while (pHolder != NULL)
	{ 
		//if( (*pChain->double_data_ident_func)(pHolder->pData,ident1,ident2) == 0 )
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
void* chain_get(chain *pChain)//,void *data)
{
	int32_t iRet;
	struct chain_holder *pHolder;
	void *data;


	down(&pChain->sema);
	if(!pChain->init_flag)//check if chain has been released
	{
		up(&pChain->sema);
		return NULL;//-1;
	}

	pHolder = pChain->base_chain.pNext;
	// the very first is the oldest
	if (pHolder == NULL)
	{
		up(&pChain->sema);
		return NULL;//-1;//TODO E_ERROR;
	}

	//call custom function to copy custom data
	//(*pChain->data_copy_fn)(data,pHolder->pData);
	data=pHolder->pData;

	// remove from chain
	iRet=chain_remove (pChain);//,pHolder->pData);
	up(&pChain->sema);

	//return iRet;//TODO E_OKAY;
	return data;
}
//returns the data of first element but doesnt remove the element from chain
void* chain_get_top(chain *pChain)
{
	void *data;
	struct chain_holder *pHolder;

	//down(&pChain->sema);

	if(!pChain->init_flag)//check if chain has been released
	{
		//up(&pChain->sema);
		return NULL;
	}
	pHolder = pChain->base_chain.pNext;
	// the very first is the oldest
	if (pHolder == NULL)
	{
		//up(&pChain->sema);
		return NULL;//TODO E_ERROR;
	}
	//call custom function to copy custom data
	data=pHolder->pData;

	//up(&pChain->sema);
	return data;
}

/*****************************************************************************/
static int32_t chain_remove( chain *pChain)//, void *data )
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
int32_t chain_size( chain *pChain)
{
	int32_t size;
	down(&pChain->sema);
	size=pChain->chain_size;
	up(&pChain->sema);
	return size;
}
int32_t chain_peak_size( chain *pChain)
{
	int32_t size;
	down(&pChain->sema);
	size=pChain->chain_peak_size;
	up(&pChain->sema);
	return size;
}
/*****************************************************************************/
static int32_t chain_add(chain *pChain,void *data)
{
	struct chain_holder *pHolder = &pChain->base_chain;
	struct chain_holder * pNewHolder;
	pNewHolder = (struct chain_holder*)kmalloc(sizeof(struct chain_holder),GFP_KERNEL);
	if (pNewHolder == NULL)
		return -1;

	pNewHolder->pData = data;

	// search last in chain
	while (pHolder->pNext != NULL) 
		pHolder = pHolder->pNext; 

	// add from tail
	pHolder->pNext    = pNewHolder; 
	pNewHolder->pPrev = pHolder; 
	pNewHolder->pNext = NULL; 

	// increment chain size
	pChain->chain_size++;
	if(pChain->chain_size > pChain->chain_peak_size)
		pChain->chain_peak_size=pChain->chain_size;//record the max peak reached

	return 0;
}

/*****************************************************************************/
static int32_t chain_remove_all(chain *pChain)
{
	// check the chain link 
	while (pChain->base_chain.pNext != NULL ) 
	{
		//TODO what if pData has some data ref? it should also be released
		kfree(pChain->base_chain.pNext->pData);//remove all data references before removing chain
		chain_remove (pChain);//,pChain->base_chain.pNext->pData);
	}
	return 0; 
}
/*****************************************************************************/
