/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_cedar sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : darray_v2.c
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-5-7
* Description:
********************************************************************************
*/
#include "ToolKit_s.h"
#include "darray_v2.h"

//#define MIN_PRE_ALLOCATE_NR 10
static Ret DArrayV2_DataExit(DArrayV2* thiz, void* data)    //ret = Ret
{
    __s32   ret = RET_OK;
    if(thiz->data_exit != NULL)
	{
		ret = thiz->data_exit(thiz->data_exit_ctx, data);
	}

	return (Ret)ret;
}
/*******************************************************************************
Function name: darray_expand
Description: 
    1.看看够不够增加的元素个数，不够就分配
Parameters: 
    
Return: 
    
Time: 2011/5/6
*******************************************************************************/
static Ret DArrayV2_Expand(DArrayV2* thiz, __u32 need)  //ret = Ret
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS); 

	if((thiz->size + need) > thiz->alloc_size)
	{
		__u32 alloc_size = thiz->alloc_size + (thiz->alloc_size>>1) + thiz->nMinPreAllocNr;

		void* data = (void*)realloc(thiz->data, thiz->data_size * alloc_size);
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
static Ret DArrayV2_Shrink(DArrayV2* thiz)  //ret = Ret
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS); 

	if((thiz->size < (thiz->alloc_size >> 1)) && (thiz->alloc_size > thiz->nMinPreAllocNr))
	{
		__u32 alloc_size = thiz->size + (thiz->size >> 1);

		void* data = (void**)realloc(thiz->data, thiz->data_size * alloc_size);
		if(data != NULL)
		{
			thiz->data = data;
			thiz->alloc_size = alloc_size;
		}
        else
        {
            //没有malloc出来，就用原来的好了
            WRN("realloc fail in darrayV2_shrink().\n");
        }
	}

	return RET_OK;
}


/*******************************************************************************
Function name: Impl_DArrayV2_InsertElem
Description: 
    1.插入一个元素，一般不是void*了，而是真的struct，因此要拷贝. 拷贝函数由使用者
    在类初始化时赋值进来。
Parameters: 
    data:一个struct的指针,
Return: 
    
Time: 2011/5/7
*******************************************************************************/
int Impl_DArrayV2_InsertElem(DArrayV2* thiz, unsigned int index, void* data)  //返回值Ret_OK
{
    __s32 ret = RET_OOM;
	__u32 cursor = index;
    //__s32 exist_flag = 0; //标记插入位置是否已经有元素了，如果有，就要取代掉它
	return_val_if_fail(thiz!=NULL && data!=NULL, RET_INVALID_PARAMS); 
	cursor = cursor < thiz->size ? cursor : thiz->size;
    
	if(DArrayV2_Expand(thiz, 1) == RET_OK)
	{
		__u32 i = 0;
		for(i = thiz->size; i > cursor; i--)
		{
			//thiz->data[i] = thiz->data[i-1];
            //移动元素时，必须采用浅拷贝，如果有malloc的变量，通过浅拷贝，也全部移到了新的位置,
            //插入位置可以认为清空了，直接拷贝就可以，深拷贝浅拷贝都可以
            memcpy((__s8*)thiz->data + thiz->data_size*i, (__s8*)thiz->data + thiz->data_size*(i-1), thiz->data_size);
		}
        //插入元素时，使用用户的函数
		//thiz->data[cursor] = data;
        if(thiz->data_copy)
		{
            ret = thiz->data_copy((void*)((__s8*)thiz->data + thiz->data_size*cursor), data);
		}
        else    //如果用户没有设置，默认浅拷贝
        {
            memcpy((void*)((__s8*)thiz->data + thiz->data_size*cursor), data, thiz->data_size);
            ret = RET_OK;
        }
		thiz->size++;
	}

	return ret;
}
int Impl_DArrayV2_PrependElem(DArrayV2* thiz, void* data)    //返回值Ret_OK
{
    return Impl_DArrayV2_InsertElem(thiz, 0, data);
}
int Impl_DArrayV2_AppendElem(DArrayV2* thiz, void* data)     //返回值Ret_OK
{
    return Impl_DArrayV2_InsertElem(thiz, (unsigned int)-1, data);
}
int Impl_DArrayV2_DeleteElem(DArrayV2* thiz, unsigned int index)   //返回值Ret_OK
{
    __u32 i = 0;
	//__s32 ret = RET_OK;

	return_val_if_fail(thiz != NULL && thiz->size > index, RET_INVALID_PARAMS); 

    DArrayV2_DataExit(thiz, (void*)((__s8*)thiz->data + thiz->data_size*index));
	for(i = index; (i+1) < thiz->size; i++)
	{
		//thiz->data[i] = thiz->data[i+1];
		//移动元素，必须浅拷贝
		memcpy((void*)((__s8*)thiz->data + thiz->data_size*i), (void*)((__s8*)thiz->data + thiz->data_size*(i+1)), thiz->data_size);
	}
	thiz->size--;

	DArrayV2_Shrink(thiz);

    return RET_OK;
}
int Impl_DArrayV2_GetElemByIndex(DArrayV2* thiz, unsigned int index, void** data)  //返回值Ret_OK
{
    return_val_if_fail(thiz != NULL && data != NULL && index < thiz->size, RET_INVALID_PARAMS); 

	//*data = thiz->data[index];
	*data = (void*)((__s8*)thiz->data + thiz->data_size * index);

	return RET_OK;
}
/*******************************************************************************
Function name: Impl_DArrayV2_SetElemByIndex
Description: 
    1.对于v2版的DArray来说，因为struct是DArray自己malloc的，所以一般不会free，但
    要exit()清理.然后拷贝用户传进来的struct,如果用户希望深拷贝，那么自己把data_copy
    函数实现，在初始化时传进来.如果只是普通的浅拷贝，用户可以不设置data_copy,
    让data_copy默认为NULL。
Parameters: 
    
Return: 
    
Time: 2011/5/8
*******************************************************************************/
int Impl_DArrayV2_SetElemByIndex(DArrayV2* thiz, unsigned int index, void* data)   //返回值Ret_OK
{
    __s32   ret;
    return_val_if_fail(thiz != NULL && index < thiz->size, RET_INVALID_PARAMS); 
    //先exit()反初始化原有的元素
    DArrayV2_DataExit(thiz, (void*)((__s8*)thiz->data + thiz->data_size*index));
    //再copy
    if(thiz->data_copy)
    {
        ret = thiz->data_copy((void*)((__s8*)thiz->data + thiz->data_size*index), data);
    }
    else    //默认浅拷贝
    {
        memcpy((void*)((__s8*)thiz->data + thiz->data_size*index), data, thiz->data_size);
        ret = RET_OK;
    }
    return ret;
}
unsigned int Impl_DArrayV2_GetLength(DArrayV2* thiz)  //返回值为元素个数
{
	return_val_if_fail(thiz != NULL, 0);
	return thiz->size;
}
int Impl_DArrayV2_ForeachElem(DArrayV2* thiz, DataVisitFunc visit, void* ctx)    //返回值Ret_OK
{
    __u32 i = 0;
	int ret = RET_OK;
	return_val_if_fail(thiz != NULL && visit != NULL, RET_INVALID_PARAMS);

	for(i = 0; i < thiz->size; i++)
	{
        ret = visit(ctx, (void*)((__s8*)thiz->data + thiz->data_size*i));
        if(RET_OK != ret)
        {
            WRN("foreach visit [%d] fail\n", i);
        }
	}

	return ret;
}
/*******************************************************************************
Function name: Impl_DArrayV2_FindElem
Description: 
    
Parameters: 
    
Return: 
    -1:参数错误
    size:没找到
Time: 2011/5/10
*******************************************************************************/
unsigned int Impl_DArrayV2_FindElem(DArrayV2* thiz, DataCompareFunc cmp, void* ctx)   //返回值为下标号
{
    __u32 i = 0;

	return_val_if_fail(thiz != NULL && cmp != NULL, (unsigned int)-1);

	for(i = 0; i < thiz->size; i++)
	{
		if(cmp(ctx, (void*)((__s8*)thiz->data + thiz->data_size*i)) == 0)
		{
			break;
		}
	}

	return i;
}

int DArrayV2_Init(DArrayV2* thiz, DataExitFuncV2 data_exit, void* data_exit_ctx, DataCopyFuncV2 data_copy, unsigned int data_size, unsigned int nMinPreAllocNr)
{
    thiz->data = NULL;
    thiz->data_size = data_size;
    thiz->size = 0;
    thiz->alloc_size = 0;
    thiz->nMinPreAllocNr = nMinPreAllocNr;
    return_val_if_fail(thiz->nMinPreAllocNr, RET_INVALID_PARAMS);
    
    thiz->data_exit_ctx = data_exit_ctx;
    thiz->data_exit = data_exit;
    thiz->data_copy = data_copy;
    
    thiz->InsertElem        = Impl_DArrayV2_InsertElem;
    thiz->PrependElem       = Impl_DArrayV2_PrependElem;
    thiz->AppendElem        = Impl_DArrayV2_AppendElem;
    thiz->DeleteElem        = Impl_DArrayV2_DeleteElem;
    thiz->GetElemByIndex    = Impl_DArrayV2_GetElemByIndex;
    thiz->SetElemByIndex    = Impl_DArrayV2_SetElemByIndex;
    thiz->GetLength         = Impl_DArrayV2_GetLength;
    thiz->ForeachElem       = Impl_DArrayV2_ForeachElem;
    thiz->FindElem          = Impl_DArrayV2_FindElem;

    return RET_OK;
}
int DArrayV2_Exit(DArrayV2* thiz)
{
    __s32 ret;
    __u32 i = 0;
	return_val_if_fail(thiz, RET_INVALID_PARAMS);
	for(i = 0; i < thiz->size; i++)
	{
		ret = DArrayV2_DataExit(thiz, (void*)((__s8*)thiz->data + thiz->data_size*i));
        if(RET_OK != ret)
        {
            WRN("DArrayV2 [%d] DataExit fail\n", i);
        }
	}
	SAFE_FREE(thiz->data);
    thiz->data = NULL;
    
	return RET_OK;
}
/*******************************************************************************
Function name: newDArrayV2
Description: 
    
Parameters: 
    data_exit:当删除一个元素时，先调用用户设置的data_exit函数反初始化,可以置为NULL,如果没有特殊操作。
Return: 
    
Time: 2011/5/12
*******************************************************************************/
DArrayV2* newDArrayV2(DataExitFuncV2 data_exit, void* data_exit_ctx, DataCopyFuncV2 data_copy, unsigned int data_size, unsigned int nMinPreAllocNr)
{
    __s32   ret;
    DArrayV2 *thiz = (DArrayV2*)malloc(sizeof(DArrayV2));
    if(NULL == thiz)
    {
        WRN("malloc DArrayV2 fail\n");
        goto _err0;
    }
    memset(thiz, 0, sizeof(DArrayV2));
    ret = DArrayV2_Init(thiz, data_exit, data_exit_ctx, data_copy, data_size, nMinPreAllocNr);
    if(RET_OK != ret)
    {
        WRN("Init DArrayV2 fail\n");
        goto _err1;
    }
    return thiz;
    
_err1:
    free(thiz);
_err0:
    return (DArrayV2*)NULL;
}
void deleteDArrayV2(DArrayV2* thiz)
{
    __s32   ret;
    return_if_fail(thiz);
    ret = DArrayV2_Exit(thiz);
    if(RET_OK != ret)
    {
        WRN("DArray Exit fail\n");
    }
    SAFE_FREE(thiz);
}

