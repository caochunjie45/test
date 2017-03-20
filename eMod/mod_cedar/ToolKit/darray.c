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
    1.�������������ӵ�Ԫ�ظ����������ͷ���
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
    1.free�����ڴ�
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
            //û��malloc����������ԭ���ĺ���
            WRN("realloc fail in darray_shrink().\n");
        }
	}

	return RET_OK;
}

/*******************************************************************************
Function name: darray_insert
Description: 
    ����Ԫ�� indexΪ����λ��, ��0��ʼ. ��λ�õ�ԭ����Ԫ�������һλ
Parameters: 
    
Return: 
    RET_OK��
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
    1.�ڶ��п�ͷ
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
    1.����ĩβ
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
    1.�õ�ָ��λ�õ�Ԫ�أ�����һ��ָ��
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
    1.����Ԫ�ص�ָ��ֵ����ָ��λ�á�ע��ԭ����ָ��ָ������ݽṹ�����malloc�ģ�
    Ҫfree����DArray�������������, �������Լ��㶨.
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
    1.visit������Ϊ����������.��Ϊÿ�α���ʱ����Ԫ�صĲ������ܲ�һ����������ʱ��
    visit������
    2. ctx:��Ϊvisit�Ĳ�����Ҳ���Ƿ�����������ʱ����
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
    1.cmpҲ����Ϊ������ʱ����,��Ϊƥ��������ÿ�ο���Ҳ��ͬ��������Ϊ������ʱ����
    2. ctx����Ҫƥ��Ĳ���
    3. cmp�ķ���ֵΪ0����ƥ�䣬����Ĭ�Ϲ涨��
Parameters: 
    
Return: 
    index��, -1�����������, �����size����ʾû�ҵ�
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
    1.һ��Ҫ��void*������ָ���structȫ���ͷţ������ڴ�й¶
    2.void*���鱾��ҲҪ�ͷ�
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
DArray* newDArray(DataDestroyFunc data_destroy, void* data_destroy_ctx, unsigned int nMinPreAllocNr) //����Ĳ�����Ϊ������void*Ԫ��ָ������ݽṹ,��Щ������structһ�㶼��malloc������
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
    1.���� DArray. ͬʱ��ʼ��һ��
Parameters: 
    ctx:��Ϊdata_destroy_ctx.�ڵ���data_destroy���ٵ�ʱ�򣬻��õ���data_destroy_ctx
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
    ȫ������
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

