#include "ADGenericChain.hpp"
#include <iostream>
using namespace std;
/*****************************************************************************/
int ADChainProducer::IDGenerator = 0;//generate Unique ID for every ADGenericChain object
/*****************************************************************************/
ADGenericChain::ADGenericChain()
{
	disable_autoremove=0;//by default, on distruction, free all elements
	chain_peak_size=0;
	chain_size=0;
	ident_generator=0;
	base_chain.pNext = NULL;
	SEMA_INIT();
	init_flag=true;
	
	//cout<<"ADGenericChain:constructor"<<endl;
	//custom_data_remover=NULL;//remover;
	//return 0;
}
//ADChain::ADChain()//DataRemoverFunc_t remover)
//{
//	chain_size=0;
//	ident_generator=0;
//	base_chain.pNext = NULL;
//	SEMA_INIT();
//	init_flag=true;
	//custom_data_remover=remover;//remover;
	//return 0;
//}
/*****************************************************************************/
ADGenericChain::~ADGenericChain()
{
	//cout<<"ADGenericChain:destructor total elements="<<chain_size<<" peak="<<chain_peak_size<<endl;
	SEMA_LOCK();
	remove_all();
	init_flag=false;//tell everyone not to use chain
	SEMA_UNLOCK();
	//cout<<"ADGenericChain:destructor"<<endl;
}
/*****************************************************************************/
int ADGenericChain::chain_empty()
{
	SEMA_LOCK();
	remove_all();
	ident_generator=0;
	SEMA_UNLOCK();
	return 0;
}
/*****************************************************************************/
int ADGenericChain::test_print(void)
{
	cout << "This is ADChain" << endl;
	return 0;
}
/*****************************************************************************/
//int ADChain::add_remover(DataRemoverFunc_t remover)
//{
	//custom_data_remover=remover;
//	return 0;	
//}
/*****************************************************************************/
//for external access to the chain
void ADGenericChain::chain_lock()
{
	SEMA_LOCK();
}
/*****************************************************************************/
//for external access to the chain
void ADGenericChain:: chain_unlock()
{
	SEMA_UNLOCK();
}
/*****************************************************************************/
int ADGenericChain::chain_add(void *data)
{
	struct chain_holder *pHolder = &base_chain;
	struct chain_holder * pNewHolder;
	pNewHolder = (struct chain_holder*)malloc(sizeof(struct chain_holder));//new chain_holder;//malloc(sizeof(struct chain_holder));//,GFP_KERNEL);
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
	chain_size++;
	if(chain_size>chain_peak_size)
		chain_peak_size=chain_size;
	return 0;
}
/*****************************************************************************/
int ADGenericChain::chain_put(void *data)
{
	int iRet = 0;
	SEMA_LOCK();
	if(init_flag==false)//check if chain has been released
	{
		SEMA_UNLOCK();
		return -1;
	}
	iRet=chain_add(data);
	SEMA_UNLOCK();
	return iRet;
}
/*****************************************************************************/
//before calling this function, user should take the lock,modify data and then 
//release lock
void *ADGenericChain::chain_get_by_ident(int ident)//, datIdfunc_t custom_func)
{
	struct chain_holder *pHolder = base_chain.pNext;
	if (pHolder == NULL)
		return NULL;

	//user did not assign function pointer which identifies the data
	//if(custom_func == NULL)
	//		return NULL;
	if(is_helper_attached()!=0)
		return NULL;


	// search last in chain
	while (pHolder != NULL)
	{ 
		if( identify_chain_element(pHolder->pData,ident) == 0 ) //new concept
		//if( (custom_func)(pHolder->pData,ident) == 0 )
			break;
		pHolder = pHolder->pNext; 
	}
	if(pHolder==NULL)
		return NULL;
	else
		return pHolder->pData;
}
/*****************************************************************************/
void *ADGenericChain::chain_remove_by_ident(int ident)//,datIdfunc_t custom_func)
{
	void *data=NULL;
	struct chain_holder *pHolder = base_chain.pNext;

	SEMA_LOCK();

	if(init_flag==false)//check if chain has been released
	{
		SEMA_UNLOCK();
		return NULL;
	}

	if (pHolder == NULL)
	{
		SEMA_UNLOCK();
		return NULL;
	}

	//user did not assign function pointer which identifies the data
	//if(custom_func == NULL)
	//{
	//	SEMA_UNLOCK();
	//	return NULL;
	//}
	if(is_helper_attached()!=0)
	{
		SEMA_UNLOCK();
		return NULL;
	}

	// search last in chain
	while (pHolder != NULL)
	{ 
		if( identify_chain_element(pHolder->pData,ident) == 0 ) //new concept
		//if( (custom_func)(pHolder->pData,ident) == 0 )
			break;
		pHolder = pHolder->pNext; 
	}

	if(pHolder==NULL)
	{
		SEMA_UNLOCK();
		return NULL;
	}
	else
	{
		data=pHolder->pData;
		if (pHolder->pNext != NULL) 
		{ 	// unlink chain element
			pHolder->pPrev->pNext = pHolder->pNext; 
			pHolder->pNext->pPrev = pHolder->pPrev; 
		} 
		else // pResp is the last in chain 
			pHolder->pPrev->pNext = NULL; 
	
		//delete pHolder;
		free(pHolder);
		chain_size--;
	}
	SEMA_UNLOCK();
	return data;
}
/*****************************************************************************/
//searches and removes a chain element based on its dual identities
void *ADGenericChain::chain_remove_by_double_ident(int ident1,int ident2)//,dbdatIdfunc_t cust_func)
{
	void *data=NULL;
	struct chain_holder *pHolder = base_chain.pNext;

	SEMA_LOCK();

	if(init_flag==false)//check if chain has been released
	{
		SEMA_UNLOCK();
		return NULL;
	}

	if (pHolder == NULL)
	{
		SEMA_UNLOCK();
		return NULL;
	}

	//user did not assign function pointer which identifies the data
	//if(cust_func==NULL)
	//{
	//	SEMA_UNLOCK();
	//	return NULL;
	//}
	if(is_helper_attached()!=0)
	{
		SEMA_UNLOCK();
		return NULL;
	}


	// search last in chain
	while (pHolder != NULL)
	{ 
		if( double_identify_chain_element(pHolder->pData,ident1,ident2) == 0 ) //new concept
		//if( (cust_func)(pHolder->pData,ident1,ident2) == 0 )	
			break;
		pHolder = pHolder->pNext; 
	}

	if(pHolder==NULL)
	{
		SEMA_UNLOCK();
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
	
		//delete pHolder;
		free(pHolder);
		chain_size--;
	}

	SEMA_UNLOCK();
	return data;
}
/*****************************************************************************/
//this function is same as get_by_ident, except it searches an element
//that matches double ident values
void* ADGenericChain::chain_get_by_double_ident(int ident1,int ident2)//,dbdatIdfunc_t cust_func)
{
	struct chain_holder *pHolder = base_chain.pNext;
	if (pHolder == NULL)
		return NULL;

	//user did not assign function pointer which identifies the data
	//if(cust_func==NULL)
	//	return NULL;
	if(is_helper_attached()!=0)
		return NULL;


	// search last in chain
	while (pHolder != NULL)
	{ 
		if( double_identify_chain_element(pHolder->pData,ident1,ident2) == 0 ) //new concept
		//if( (cust_func)(pHolder->pData,ident1,ident2) == 0 )
			break;
		pHolder = pHolder->pNext; 
	}

	if(pHolder==NULL)
		return NULL;
	else
		return pHolder->pData;
}
/*****************************************************************************/
//returns the pointer of Nth element in a chain(index is zero based)
//caller is responsible to locking the chain before calling this function
void* ADGenericChain::chain_get_by_index(int index)
{
	int i=0;
	struct chain_holder *pHolder = base_chain.pNext;
	if (pHolder == NULL)
		return NULL;

	//required index(zero based index) element is not available
	if(index>=chain_size)
		return NULL;

	// increment till required index
	while(i++<index)
		pHolder = pHolder->pNext; 
	
	if(pHolder==NULL)
		return NULL;
	else
		return pHolder->pData;
}
/*****************************************************************************/
void* ADGenericChain::chain_get()//chain *pChain)//,void *data)
{
	//int32_t iRet;
	struct chain_holder *pHolder;
	void *data;


	SEMA_LOCK();
	if(init_flag==false)//check if chain has been released
	{
		SEMA_UNLOCK();
		return NULL;//-1;
	}

	pHolder = base_chain.pNext;
	// the very first is the oldest
	if (pHolder == NULL)
	{
		SEMA_UNLOCK();
		return NULL;
	}

	//call custom function to copy custom data
	data=pHolder->pData;

	// remove from chain
	//iRet=chain_remove (pChain);
	chain_remove ();//pChain);
	
	SEMA_UNLOCK();

	return data;
}
/*****************************************************************************/
//returns the data of first element but doesnt remove the element from chain
void* ADGenericChain::get_top()//chain *pChain)
{
	void *data;
	struct chain_holder *pHolder;
	if(init_flag==false)//check if chain has been released
	{
		return NULL;
	}
	pHolder = base_chain.pNext;
	// the very first is the oldest
	if (pHolder == NULL)
	{
		return NULL;
	}
	//call custom function to copy custom data
	data=pHolder->pData;
	return data;
}
/*****************************************************************************/
int ADGenericChain::chain_remove( void )//chain *pChain)//, void *data )
{
	// there is alvays a previous link 
	struct chain_holder *pHolder = base_chain.pNext;
	if (pHolder == NULL)
		return -1;

	// check link
	if (pHolder->pNext != NULL) 
	{ 	// unlink pResp 
		pHolder->pPrev->pNext = pHolder->pNext; 
		pHolder->pNext->pPrev = pHolder->pPrev; 
	} 
	else // pResp is the last in chain 
		pHolder->pPrev->pNext = NULL; 

	// finally destroy request holder 
	//delete pHolder;
	free(pHolder);

	// decrement chain size
	chain_size--;
	return 0;
}
/*****************************************************************************/
int ADGenericChain::get_chain_size(void)
{
	int size;
	SEMA_LOCK();
	size=chain_size;
	SEMA_UNLOCK();
	return size;
}
/*****************************************************************************/
int ADGenericChain::chain_generate_ident(void)
{
	int ident;
	SEMA_LOCK();
	ident_generator++;
	ident=ident_generator;
	SEMA_UNLOCK();
	return ident;
}
/*****************************************************************************/
int ADGenericChain::remove(void)
{
	// there is always a previous link 
	struct chain_holder *pHolder = base_chain.pNext;
	if (pHolder == NULL)
		return -1;

	// check link
	if (pHolder->pNext != NULL) 
	{ 	// unlink pResp 
		pHolder->pPrev->pNext = pHolder->pNext; 
		pHolder->pNext->pPrev = pHolder->pPrev; 
	} 
	else // pResp is the last in chain 
		pHolder->pPrev->pNext = NULL; 

	// finally destroy request holder 
	//delete pHolder;
	free(pHolder);
	// decrement chain size
	chain_size--;
	return 0;
}
/*****************************************************************************/
int ADGenericChain::remove_all(void)
{
	// check the chain link 
	while (base_chain.pNext != NULL ) 
	{
		//what if pData has some data ref? it should also be released
		//if(custom_data_remover!=NULL)
		//	(custom_data_remover)(base_chain.pNext->pData);
		if(disable_autoremove==0)
		{
			free_chain_element_data(base_chain.pNext->pData);
			//remove all data references before removing chain element
			//delete base_chain.pNext->pData;
			free(base_chain.pNext->pData);//
		}
		remove();//pChain);
	}
	return 0; 
}
/*****************************************************************************/
//if this chain holds data of other chain, then do not free this, owner will take care of freeing.
int ADGenericChain::disable_auto_remove()
{
	disable_autoremove=1;
	return 0;
}
int ADGenericChain::enable_auto_remove()
{
	disable_autoremove=0;
	return 0;
}
/*****************************************************************************/
















