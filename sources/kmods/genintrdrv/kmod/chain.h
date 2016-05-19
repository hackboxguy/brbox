#ifndef _CHAIN_H_
#define _CHAIN_H_
#include	<linux/semaphore.h>

// type-definition: 'pt2Function' now can be used as type
typedef int32_t (*typ_data_cp_func)(void* dest,void* src);
typedef int32_t (*typ_data_sz_func)(void);
//typedef int8_t  (*typ_custom_data_identify)(void* data,int32_t ident);
typedef int8_t  (*datIdfunc_t)(void* data,int32_t ident);
//typedef int8_t  (*typ_double_data_ident)(void* data,int32_t ident1,int32_t ident2);
typedef int8_t  (*dbdatIdfunc_t)(void* data,int32_t ident1,int32_t ident2);
typedef int8_t  (*tpdatIdfunc_t)(void* data,int32_t ident1,int32_t ident2,int32_t ident3);

struct chain_holder//typedef chain_h rename(chain_element_ref)
{
	void *pData;
	struct chain_holder * pPrev;
	struct chain_holder * pNext;
};

//chain structure to be used
typedef struct type_chain
{
	struct chain_holder base_chain;
	int32_t chain_size;
	int32_t chain_peak_size;//tell the total peak size chain has reached
	int8_t init_flag;
	struct semaphore sema;
}chain;

//int32_t chain_init( chain *pChain );
int32_t chain_init( chain *pChain);//,typ_custom_data_identify func_ptr);//,typ_data_cp_func cp_func,typ_data_sz_func sz_func);
int32_t chain_release(chain *pChain);
int32_t chain_put(chain *pChain,void *data);//creates the new element in the chain which points to data(ref)
void*   chain_get(chain *pChain);//returns the data pointer of top(first) element(and delets the element)
int32_t chain_size( chain *pChain);//returns the no of elements present in the chain
int32_t chain_peak_size( chain *pChain);

void chain_lock(chain *pChain);//for external access of sema_lock
void chain_unlock(chain *pChain);//for external access of sema_unlock
//void *chain_remove_by_ident(chain *pChain,int32_t ident);

void *chain_get_by_ident(chain *pChain,int32_t ident,datIdfunc_t cust_func);
void *chain_remove_by_ident(chain *pChain,int32_t ident,datIdfunc_t cust_func);
void *chain_get_by_double_ident(chain *pChain,int32_t ident1,int32_t ident2,dbdatIdfunc_t cust_func);
void *chain_remove_by_double_ident(chain *pChain,int32_t ident1,int32_t ident2,dbdatIdfunc_t cust_func);
void *chain_remove_by_triple_ident(chain *pChain,int32_t ident1,int32_t ident2,int32_t ident3,tpdatIdfunc_t cust_func);
void* chain_get_top(chain *pChain);
void* chain_get_by_index(chain *pChain,int index);



//chain structure to be used
typedef struct type_sl_chain
{
	struct chain_holder base_chain;
	int32_t chain_size;
	int32_t chain_peak_size;//tell the total peak size chain has reached
	int8_t init_flag;
	spinlock_t sl;
}sl_chain;//chain with spinlock implementation

int32_t sl_chain_init(sl_chain *pChain);
int32_t sl_chain_release(sl_chain *pChain);
int32_t sl_chain_put(sl_chain *pChain,void *data);
void*   sl_chain_get(sl_chain *pChain);
int32_t sl_chain_size(sl_chain *pChain);
int32_t sl_chain_peak_size(sl_chain *pChain);

void*   sl_chain_get_top(sl_chain *pChain);
void sl_chain_lock(sl_chain *pChain,unsigned long *flags);
void sl_chain_unlock(sl_chain *pChain,unsigned long *flags);
void *sl_chain_remove_by_ident(sl_chain *pChain,int32_t ident,datIdfunc_t custom_func);
void *sl_chain_get_by_double_ident(sl_chain *pChain,int32_t ident1,int32_t ident2,dbdatIdfunc_t cust_func);

#endif 






























//data object of the chain
/*typedef struct chain_data
{
	ERespDev respDev;
	int32_t offsDev;
	EClientId clientId;
	EReqAction actionId;
	unsigned int sequenceId;
	void * dRef;
	int32_t dCnt;
	EErrors * dValid;
};*/

/***********************spinlock chain header info********************************************/
//chain holder
//struct chain_holder
//{
//	void * pData;
//	struct chain_holder * pPrev;
//	struct chain_holder * pNext;
//};
	//user function used for handling custom data struct(copy_to and copy_from)
	//int32_t (*data_copy_fn) (void* dest,void* src);
	//user function that returns the custom data size
	//int32_t (*data_size_fn) (void);
	//typ_data_cp_func data_copy_fn;//function pointer which handles custom data
	//typ_data_sz_func data_size_fn;//function pointer which returns custom data size
