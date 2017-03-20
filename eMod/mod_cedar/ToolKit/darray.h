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
* File   : darray.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-5-7
* Description:
    ͨ�ö�̬�����ʵ��,����Ԫ��ͳһΪvoid* ���ͣ�ָ��һ��malloc���������ݽṹ(opaque struct)
    ���ļ���Ϊ����API,������������ʹ�ñ�׼C��
********************************************************************************
*/
//#include <stdio.h>
//#include "typedef.h"

#ifndef DARRAY_H
#define DARRAY_H

DECLS_BEGIN;

typedef enum _Ret
{
	RET_OK = EPDK_OK,
	RET_FAIL = EPDK_FAIL, 
    RET_OOM = -2,   //�ڴ�û���ˣ�malloc������
    RET_STOP = -3,
    RET_INVALID_PARAMS = -4,
}Ret;

//#define return_if_fail(p) if(!(p)) \
//	{printf("%s:%d Warning: "#p" failed.\n", \
//		__func__, __LINE__); return;}
//#define return_val_if_fail(p, ret) if(!(p)) \
//	{printf("%s:%d Warning: "#p" failed.\n",\
//	__func__, __LINE__); return (ret);}

//#define return_if_fail(p) if(!(p)) \
//	{WARNING("Warning: "#p" failed.\n" \
//		); return;}
//#define return_val_if_fail(p, ret) if(!(p)) \
//	{WARNING("Warning: "#p" failed.\n" \
//	); return (ret);}

//#define return_if_fail(p)
//#define return_val_if_fail(p, ret)

#define SAFE_FREE(p) if(p != NULL) {free(p); p = NULL;}

/*******************************************************************************
������Ҫ������ʵ�ֵĺ�����˵��:(DArray��һ��Ҳ�����õ�)
1. DataExitFuncV2: 
    ��ΪDArrayV2��ֱ��malloc ������struct����ģ�����ʹ���߲���Ҫʵ��
struct�����������������ǵ�struct�п����Լ��ڲ���ָ�������Ҫmalloc����˻���Ҫʵ��
exit()�������ڷ�����ȡ��ǰ,�ͷ��ڴ档һ��DArrayV2_DeleteElem()��DArrayV2_SetElemByIndex()
��������������, DArrayV2_SetElemByIndex()����Ϊ��ԭ����Ԫ�ر�ȡ��ǰ�������ͷ�malloc
���ڴ棬�����ڴ�й¶��ԭ����Ԫ�ض���DArrayV2�Լ�malloc�����ģ�����DArrayV2����
��֤�����ڴ�й¶��DArray��һ���Լ�malloc��������void*���飬void*ָ���struct��
ʹ�����Լ�����Ļ������Լ�ָ���Ĺ̶������һ��Ԫ�أ���Ҳ�ṩ��DataDestroyFunc()
��ʹ�����Լ����ٻ�exit��
    �ڳ�ʼ��ʱ�ͱ��븳ֵ��Ҳ����˵��һ��DArrayV2��������ֻ����һ��exit��Ϊ��
    �û����Բ�ʵ�֣�Ĭ��ΪNULL��Ҳ���ǲ���free�Ķ�����

    data:����ָ��Ԫ�ص�ָ��ֵ��
    ����ֵ: RET_OK��

2. DataCopyFuncV2:
    ��insert��set by indexʱ���Ǳ���������ģ��������ǳ�����û��Լ��������û�
���Բ�ʵ�֣������ʵ�֣�Ĭ��ǳ����.
    �����ʼ��ʱ�ͱ��븳ֵ��Ҳ����˵:ֻ����һ��Copy��Ϊ��
    
3. DataCompareFuncV2:
    �Ƚ�2��Ԫ���Ƿ���ͬ���ú����������û��ṩ��
    ��������ʱ����һ��ú���ֻ����DArrayV2_FindElem(),��Ϊ�������룬����DArrayV2ƥ��������
    ��һ��:ret = 0��ʾƥ��. data����void*Ԫ��, ctx��ƥ��ֵ���û���ʱ����������ģ�һ���void*ָ���struct��������ͬ��.����DArray_FindElem(), ctx >= data, ����ֵΪ>=0.
    �ڶ���:ret = 0��ʾƥ��. data����Ԫ��, ctx��ƥ��ֵ���û���ʱ����������ģ�һ���DArrayV2��struct������ͬ, ����DArray_FindElem()
    
4. DataVisitFuncV2:
    �������ڷ���һ��Ԫ��ʱ����ʲô���Ĳ�����
    ��ʱ��Ϊ��������DArrayV2_ForeachElem()������DArrayV2����������ʲô���顣
    ��һ��:Ret, data������һ��void*Ԫ��, ctx���û���ʱ�����������,����DArray_ForeachElem()
    �ڶ���:Ret, data������һ��struct��ָ��, ctx���û���ʱ�����������,����DArray_ForeachElem()
*******************************************************************************/
typedef void    (*DataDestroyFunc)(void* ctx, void* data); //����void*Ԫ��data��ָ���struct, ctx�������õ������ģ����ʼ��ʱ�ʹ�����ָ�룬�����޸���ָ���struct
typedef int     (*DataCompareFunc)(void* ctx, void* data); //ret = 0��ʾƥ��. data����void*Ԫ��, ctx��ƥ��ֵ���û���ʱ����������ģ�һ���void*ָ���struct��������ͬ��.����DArray_FindElem(), ctx >= data, ����ֵΪ>=0.
typedef int     (*DataVisitFunc)(void* ctx, void* data);   //Ret, data������һ��void*Ԫ��, ctx���û���ʱ�����������,����DArray_ForeachElem()

typedef struct tag_DArray DArray;
typedef int (*DArray_InsertElem)(DArray* thiz, unsigned int index, void* data);  //����ֵRet_OK
typedef int (*DArray_PrependElem)(DArray* thiz, void* data);    //����ֵRet_OK
typedef int (*DArray_AppendElem)(DArray* thiz, void* data);     //����ֵRet_OK
typedef int (*DArray_DeleteElem)(DArray* thiz, unsigned int index);   //����ֵRet_OK
typedef int (*DArray_GetElemByIndex)(DArray* thiz, unsigned int index, void** data);  //����ֵRet_OK
typedef int (*DArray_SetElemByIndex)(DArray* thiz, unsigned int index, void* data);   //����ֵRet_OK
typedef unsigned int (*DArray_GetLength)(DArray* thiz);  //����ֵΪԪ�ظ���
typedef int (*DArray_ForeachElem)(DArray* thiz, DataVisitFunc visit, void* ctx);    //����ֵRet_OK
typedef unsigned int (*DArray_FindElem)(DArray* thiz, DataCompareFunc cmp, void* ctx);   //����ֵΪ�±��
typedef struct tag_DArray
{
	void** data;    //void* �������ʼ��ַ,ÿ��void*Ԫ��ָ��һ�����������ݽṹ, �ǲ���������Ҫ�����Ķ���
	unsigned int size;    //��ǰ��ռ��Ԫ�ظ���
	unsigned int alloc_size;  //��ǰ�Ѿ�malloc������Ԫ���ܸ���
    unsigned int nMinPreAllocNr;    //���Ԥ�����Ԫ�ظ���, #define MIN_PRE_ALLOCATE_NR 10
    
	void* data_destroy_ctx; //����data_destroy()ʱ����Ϊ���������ģ���Ϊ����ʱ�Ĳο�������֪�������ã�������һ��Ԫ��ǰ�����Ը���ctx�����ݣ�����������
	DataDestroyFunc data_destroy;   //ɾ��Ԫ��ʱ�����Ԫ����ָ�룬ָ��һ��malloc�����ݽṹ������Ҫ���øú���free.��Ȼ�����struct�����ǹ̶������飬����Ҫ��DArray����free���͸�NULL��

    //DArray�Ĳ��������⹫����API
    DArray_InsertElem       InsertElem;
    DArray_PrependElem      PrependElem;
    DArray_AppendElem       AppendElem;
    DArray_DeleteElem       DeleteElem;
    DArray_GetElemByIndex   GetElemByIndex;
    DArray_SetElemByIndex   SetElemByIndex;
    DArray_GetLength        GetLength;
    DArray_ForeachElem      ForeachElem;
    DArray_FindElem         FindElem;
}DArray;
//��ʱ�򣬶�̬�����Լ�ֱ����Ϊ�������ڣ���ʱ��ʼ��������
extern int DArray_Init(DArray* thiz, DataDestroyFunc data_destroy, void* data_destroy_ctx, unsigned int nMinPreAllocNr);
extern int DArray_Exit(DArray* thiz);
//��ʱ�򣬶�̬����ֻ����Ϊһ��ָ����ڣ���ʱ��Ҫnewһ������
extern DArray* newDArray(DataDestroyFunc data_destroy, void* data_destroy_ctx, unsigned int nMinPreAllocNr); //����Ĳ�����Ϊ������void*Ԫ��ָ������ݽṹ,��Щ������structһ�㶼��malloc������
extern void deleteDArray(DArray* thiz);  //ɾ��DArray������Ԫ�أ����Ԫ��ָ������ݽṹ��Ҫfree,Ҳ�����data_destroy() ����free

DECLS_END

#endif/*DARRAY_H*/

