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
    1.�������������ӵ�Ԫ�ظ����������ͷ���
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
    1.free�����ڴ�
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
            //û��malloc����������ԭ���ĺ���
            WRN("realloc fail in darrayV2_shrink().\n");
        }
	}

	return RET_OK;
}


/*******************************************************************************
Function name: Impl_DArrayV2_InsertElem
Description: 
    1.����һ��Ԫ�أ�һ�㲻��void*�ˣ��������struct�����Ҫ����. ����������ʹ����
    �����ʼ��ʱ��ֵ������
Parameters: 
    data:һ��struct��ָ��,
Return: 
    
Time: 2011/5/7
*******************************************************************************/
int Impl_DArrayV2_InsertElem(DArrayV2* thiz, unsigned int index, void* data)  //����ֵRet_OK
{
    __s32 ret = RET_OOM;
	__u32 cursor = index;
    //__s32 exist_flag = 0; //��ǲ���λ���Ƿ��Ѿ���Ԫ���ˣ�����У���Ҫȡ������
	return_val_if_fail(thiz!=NULL && data!=NULL, RET_INVALID_PARAMS); 
	cursor = cursor < thiz->size ? cursor : thiz->size;
    
	if(DArrayV2_Expand(thiz, 1) == RET_OK)
	{
		__u32 i = 0;
		for(i = thiz->size; i > cursor; i--)
		{
			//thiz->data[i] = thiz->data[i-1];
            //�ƶ�Ԫ��ʱ���������ǳ�����������malloc�ı�����ͨ��ǳ������Ҳȫ���Ƶ����µ�λ��,
            //����λ�ÿ�����Ϊ����ˣ�ֱ�ӿ����Ϳ��ԣ����ǳ����������
            memcpy((__s8*)thiz->data + thiz->data_size*i, (__s8*)thiz->data + thiz->data_size*(i-1), thiz->data_size);
		}
        //����Ԫ��ʱ��ʹ���û��ĺ���
		//thiz->data[cursor] = data;
        if(thiz->data_copy)
		{
            ret = thiz->data_copy((void*)((__s8*)thiz->data + thiz->data_size*cursor), data);
		}
        else    //����û�û�����ã�Ĭ��ǳ����
        {
            memcpy((void*)((__s8*)thiz->data + thiz->data_size*cursor), data, thiz->data_size);
            ret = RET_OK;
        }
		thiz->size++;
	}

	return ret;
}
int Impl_DArrayV2_PrependElem(DArrayV2* thiz, void* data)    //����ֵRet_OK
{
    return Impl_DArrayV2_InsertElem(thiz, 0, data);
}
int Impl_DArrayV2_AppendElem(DArrayV2* thiz, void* data)     //����ֵRet_OK
{
    return Impl_DArrayV2_InsertElem(thiz, (unsigned int)-1, data);
}
int Impl_DArrayV2_DeleteElem(DArrayV2* thiz, unsigned int index)   //����ֵRet_OK
{
    __u32 i = 0;
	//__s32 ret = RET_OK;

	return_val_if_fail(thiz != NULL && thiz->size > index, RET_INVALID_PARAMS); 

    DArrayV2_DataExit(thiz, (void*)((__s8*)thiz->data + thiz->data_size*index));
	for(i = index; (i+1) < thiz->size; i++)
	{
		//thiz->data[i] = thiz->data[i+1];
		//�ƶ�Ԫ�أ�����ǳ����
		memcpy((void*)((__s8*)thiz->data + thiz->data_size*i), (void*)((__s8*)thiz->data + thiz->data_size*(i+1)), thiz->data_size);
	}
	thiz->size--;

	DArrayV2_Shrink(thiz);

    return RET_OK;
}
int Impl_DArrayV2_GetElemByIndex(DArrayV2* thiz, unsigned int index, void** data)  //����ֵRet_OK
{
    return_val_if_fail(thiz != NULL && data != NULL && index < thiz->size, RET_INVALID_PARAMS); 

	//*data = thiz->data[index];
	*data = (void*)((__s8*)thiz->data + thiz->data_size * index);

	return RET_OK;
}
/*******************************************************************************
Function name: Impl_DArrayV2_SetElemByIndex
Description: 
    1.����v2���DArray��˵����Ϊstruct��DArray�Լ�malloc�ģ�����һ�㲻��free����
    Ҫexit()����.Ȼ�󿽱��û���������struct,����û�ϣ���������ô�Լ���data_copy
    ����ʵ�֣��ڳ�ʼ��ʱ������.���ֻ����ͨ��ǳ�������û����Բ�����data_copy,
    ��data_copyĬ��ΪNULL��
Parameters: 
    
Return: 
    
Time: 2011/5/8
*******************************************************************************/
int Impl_DArrayV2_SetElemByIndex(DArrayV2* thiz, unsigned int index, void* data)   //����ֵRet_OK
{
    __s32   ret;
    return_val_if_fail(thiz != NULL && index < thiz->size, RET_INVALID_PARAMS); 
    //��exit()����ʼ��ԭ�е�Ԫ��
    DArrayV2_DataExit(thiz, (void*)((__s8*)thiz->data + thiz->data_size*index));
    //��copy
    if(thiz->data_copy)
    {
        ret = thiz->data_copy((void*)((__s8*)thiz->data + thiz->data_size*index), data);
    }
    else    //Ĭ��ǳ����
    {
        memcpy((void*)((__s8*)thiz->data + thiz->data_size*index), data, thiz->data_size);
        ret = RET_OK;
    }
    return ret;
}
unsigned int Impl_DArrayV2_GetLength(DArrayV2* thiz)  //����ֵΪԪ�ظ���
{
	return_val_if_fail(thiz != NULL, 0);
	return thiz->size;
}
int Impl_DArrayV2_ForeachElem(DArrayV2* thiz, DataVisitFunc visit, void* ctx)    //����ֵRet_OK
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
    -1:��������
    size:û�ҵ�
Time: 2011/5/10
*******************************************************************************/
unsigned int Impl_DArrayV2_FindElem(DArrayV2* thiz, DataCompareFunc cmp, void* ctx)   //����ֵΪ�±��
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
    data_exit:��ɾ��һ��Ԫ��ʱ���ȵ����û����õ�data_exit��������ʼ��,������ΪNULL,���û�����������
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

