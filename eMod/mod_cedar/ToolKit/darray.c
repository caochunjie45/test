//#include "darray_s.h"
//#include <stdlib.h>
#include "ToolKit_s.h"
#include "darray.h"

//#define MIN_PRE_ALLOCATE_NR 10
static void darray_destroy_data(DArray* thiz, void* data)
{
	if(thiz->data_destroy != NULL)
	{
		thiz->data_destroy(thiz->data_destroy_ctx, data);
	}

	return;
}
/*******************************************************************************
Function name: darray_expand
Description: 
    1.看看够不够增加的元素个数，不够就分配
Parameters: 
    
Return: 
    
Time: 2011/5/6
*******************************************************************************/
static Ret darray_expand(DArray* thiz, __u32 need)
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS); 

	if((thiz->size + need) > thiz->alloc_size)
	{
		//__u32 alloc_size = thiz->alloc_size + (thiz->alloc_size>>1) + MIN_PRE_ALLOCATE_NR;
		__u32 alloc_size = thiz->alloc_size + (thiz->alloc_size>>1) + thiz->nMinPreAllocNr;

		void** data = (void**)realloc(thiz->data, sizeof(void*) * alloc_size);
		if(data != NULL)
		{
			thiz->data = data;
			thiz->alloc_size = alloc_size;
		}
        else
        {
            WRN("realloc fail in darray_expand().\n");
        }
	}

	return ((thiz->size + need) <= thiz->alloc_size) ? RET_OK : RET_FAIL;
}

/*******************************************************************************
Function name: darray_shrink
Description: 
    1.free部分内存
Parameters: 
    
Return: 
    
Time: 2011/5/6
*******************************************************************************/
static Ret darray_shrink(DArray* thiz)
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS); 

	//if((thiz->size < (thiz->alloc_size >> 1)) && (thiz->alloc_size > MIN_PRE_ALLOCATE_NR))
	if((thiz->size < (thiz->alloc_size >> 1)) && (thiz->alloc_size > thiz->nMinPreAllocNr))
	{
		__u32 alloc_size = thiz->size + (thiz->size >> 1);

		void** data = (void**)realloc(thiz->data, sizeof(void*) * alloc_size);
		if(data != NULL)
		{
			thiz->data = data;
			thiz->alloc_size = alloc_size;
		}
        else
        {
            //没有malloc出来，就用原来的好了
            WRN("realloc fail in darray_shrink().\n");
        }
	}

	return RET_OK;
}

/*******************************************************************************
Function name: darray_insert
Description: 
    插入元素 index为插入位置, 从0开始. 该位置的原来的元素向后移一位
Parameters: 
    
Return: 
    RET_OK等
Time: 2011/5/6
*******************************************************************************/
__s32 darray_insert(DArray* thiz, __u32 index, void* data)
{
	__s32 ret = RET_OOM;
	__u32 cursor = index;
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS); 
	cursor = cursor < thiz->size ? cursor : thiz->size;

	if(darray_expand(thiz, 1) == RET_OK)
	{
		__u32 i = 0;
		for(i = thiz->size; i > cursor; i--)
		{
			thiz->data[i] = thiz->data[i-1];
		}

		thiz->data[cursor] = data;
		thiz->size++;
		
		ret = RET_OK;
	}

	return ret;
}

/*******************************************************************************
Function name: darray_prepend
Description: 
    1.在队列开头
Parameters: 
    
Return: 
    
Time: 2011/5/6
*******************************************************************************/
__s32 darray_prepend(DArray* thiz, void* data)
{
	return darray_insert(thiz, 0, data);
}

/*******************************************************************************
Function name: darray_append
Description: 
    1.队列末尾
Parameters: 
    
Return: 
    
Time: 2011/5/6
*******************************************************************************/
__s32 darray_append(DArray* thiz, void* data)
{
	return darray_insert(thiz, (__u32)-1, data);
}

__s32 darray_delete(DArray* thiz, __u32 index)
{
	__u32 i = 0;
	//__s32 ret = (__s32)RET_OK;

	return_val_if_fail(thiz != NULL && thiz->size > index, RET_INVALID_PARAMS); 

	darray_destroy_data(thiz, thiz->data[index]);
	for(i = index; (i+1) < thiz->size; i++)
	{
		thiz->data[i] = thiz->data[i+1];
	}
	thiz->size--;

	darray_shrink(thiz);

	return RET_OK;
}

/*******************************************************************************
Function name: darray_get_by_index
Description: 
    1.得到指定位置的元素，就是一个指针
Parameters: 
    
Return: 
    
Time: 2011/5/6
*******************************************************************************/
__s32 darray_get_by_index(DArray* thiz, __u32 index, void** data)
{
	return_val_if_fail(thiz != NULL && data != NULL && index < thiz->size, 
		RET_INVALID_PARAMS); 

	*data = thiz->data[index];

	return RET_OK;
}

/*******************************************************************************
Function name: darray_set_by_index
Description: 
    1.把新元素的指针值赋到指定位置。注意原来的指针指向的数据结构如果是malloc的，
    要free。但DArray不会做这件事了, 调用者自己搞定.
Parameters: 
    
Return: 
    
Time: 2011/5/6
*******************************************************************************/
__s32 darray_set_by_index(DArray* thiz, __u32 index, void* data)
{
	return_val_if_fail(thiz != NULL && index < thiz->size, 
		RET_INVALID_PARAMS); 

	thiz->data[index] = data;

	return RET_OK;
}

__u32 darray_length(DArray* thiz)
{
	return_val_if_fail(thiz != NULL, 0);
	return thiz->size;
}

/*******************************************************************************
Function name: darray_foreach
Description: 
    1.visit函数作为参数赋进来.因为每次遍历时，对元素的操作可能不一样，所以临时赋
    visit参数。
    2. ctx:作为visit的参数，也就是访问上下文临时传入
Parameters: 
    
Return: 
    
Time: 2011/5/6
*******************************************************************************/
__s32 darray_foreach(DArray* thiz, DataVisitFunc visit, void* ctx)
{
	__u32 i = 0;	
	__s32 ret = RET_OK;
	return_val_if_fail(thiz != NULL && visit != NULL, RET_INVALID_PARAMS);

	for(i = 0; i < thiz->size; i++)
	{
		ret = visit(ctx, thiz->data[i]);
	}

	return ret;
}

/*******************************************************************************
Function name: darray_find
Description: 
    1.cmp也是作为参数临时赋入,因为匹配条件，每次可能也不同。所以作为参数临时传入
    2. ctx就是要匹配的参数
    3. cmp的返回值为0代表匹配，这是默认规定的
Parameters: 
    
Return: 
    index号, -1代表参数错误, 如果是size，表示没找到
Time: 2011/5/6
*******************************************************************************/
__u32 darray_find(DArray* thiz, DataCompareFunc cmp, void* ctx)
{
	__u32 i = 0;

	return_val_if_fail(thiz != NULL && cmp != NULL, (__u32)-1);

	for(i = 0; i < thiz->size; i++)
	{
		if(cmp(ctx, thiz->data[i]) == 0)
		{
			break;
		}
	}

	return i;
}

int DArray_Init(DArray* thiz, DataDestroyFunc data_destroy, void* data_destroy_ctx, unsigned int nMinPreAllocNr)
{
    thiz->data  = NULL;
	thiz->size  = 0;
	thiz->alloc_size = 0;
    thiz->nMinPreAllocNr = nMinPreAllocNr;
    return_val_if_fail(thiz->nMinPreAllocNr, RET_INVALID_PARAMS);
    
	thiz->data_destroy = data_destroy;
	thiz->data_destroy_ctx = data_destroy_ctx;

    thiz->InsertElem        = darray_insert;
    thiz->PrependElem       = darray_prepend;
    thiz->AppendElem        = darray_append;
    thiz->DeleteElem        = darray_delete;
    thiz->GetElemByIndex    = darray_get_by_index;
    thiz->SetElemByIndex    = darray_set_by_index;
    thiz->GetLength         = darray_length;
    thiz->ForeachElem       = darray_foreach;
    thiz->FindElem          = darray_find;
    
    return RET_OK;
}
/*******************************************************************************
Function name: DArray_Exit
Description: 
    1.一定要把void*数组所指向的struct全部释放，否则内存泄露
    2.void*数组本身也要释放
Parameters: 
    
Return: 
    
Time: 2011/5/7
*******************************************************************************/
int DArray_Exit(DArray* thiz)
{
    __u32 i = 0;
	return_val_if_fail(thiz, RET_INVALID_PARAMS);
	for(i = 0; i < thiz->size; i++)
	{
		darray_destroy_data(thiz, thiz->data[i]);
	}
	
	SAFE_FREE(thiz->data);
    thiz->data = (void**)NULL;
	//SAFE_FREE(thiz);

	return RET_OK;
}
DArray* newDArray(DataDestroyFunc data_destroy, void* data_destroy_ctx, unsigned int nMinPreAllocNr) //传入的参数是为了销毁void*元素指向的数据结构,这些真正的struct一般都是malloc出来的
{
    __s32   ret;
    DArray* thiz = (DArray*)malloc(sizeof(DArray));
    if(NULL == thiz)
    {
        WRN("malloc DArray fail\n");
        goto _err0;
    }
    memset(thiz, 0, sizeof(DArray));
    ret = DArray_Init(thiz, data_destroy, data_destroy_ctx, nMinPreAllocNr);
    if(RET_OK != ret)
    {
        WRN("init DArray fail\n");
        goto _err1;
    }
	return thiz;

_err1:
    free(thiz);
_err0:
    return (DArray*)NULL;
}
void deleteDArray(DArray* thiz)
{
    __s32   ret;
    return_if_fail(thiz);
    ret = DArray_Exit(thiz);
    if(RET_OK != ret)
    {
        WRN("DArray Exit fail\n");
    }
    SAFE_FREE(thiz);
}

#if 0
/*******************************************************************************
Function name: darray_create
Description: 
    1.创建 DArray. 同时初始化一下
Parameters: 
    ctx:作为data_destroy_ctx.在调用data_destroy销毁的时候，会用到该data_destroy_ctx
Return: 
    
Time: 2011/5/6
*******************************************************************************/
DArray* darray_create(DataDestroyFunc data_destroy, void* ctx)
{
	DArray* thiz = (DArray*)malloc(sizeof(DArray));

	if(thiz != NULL) 
	{
		thiz->data  = NULL;
		thiz->size  = 0;
		thiz->alloc_size = 0;
		thiz->data_destroy = data_destroy;
		thiz->data_destroy_ctx = ctx;
	}

	return thiz;
}

/*******************************************************************************
Function name: darray_destroy
Description: 
    全部销毁
Parameters: 
    
Return: 
    
Time: 2011/5/7
*******************************************************************************/
void darray_destroy(DArray* thiz)
{
	size_t i = 0;
	
	if(thiz != NULL)
	{
		for(i = 0; i < thiz->size; i++)
		{
			darray_destroy_data(thiz, thiz->data[i]);
		}
		
		SAFE_FREE(thiz->data);
		SAFE_FREE(thiz);
	}

	return;
}

#endif

