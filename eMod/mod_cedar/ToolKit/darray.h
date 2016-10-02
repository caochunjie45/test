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
    通用动态数组的实现,数组元素统一为void* 类型，指向一个malloc出来的数据结构(opaque struct)
    本文件作为对外API,所有数据类型使用标准C。
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
    RET_OOM = -2,   //内存没有了，malloc不出来
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
关于需要调用者实现的函数的说明:(DArray第一版也是适用的)
1. DataExitFuncV2: 
    因为DArrayV2是直接malloc 连续的struct数组的，所以使用者不需要实现
struct的析构函数，但考虑到struct有可能自己内部的指针变量需要malloc，因此还是要实现
exit()函数，在废弃或被取代前,释放内存。一般DArrayV2_DeleteElem()和DArrayV2_SetElemByIndex()
都会调用这个函数, DArrayV2_SetElemByIndex()是因为在原来的元素被取代前，必须释放malloc
的内存，否则内存泄露。原来的元素都是DArrayV2自己malloc出来的，所以DArrayV2必须
保证不能内存泄露。DArray第一版自己malloc出来的是void*数组，void*指向的struct是
使用者自己分配的或者是自己指定的固定数组的一个元素，但也提供了DataDestroyFunc()
让使用者自己销毁或exit。
    在初始化时就必须赋值。也就是说，一旦DArrayV2起来，就只能有一种exit行为。
    用户可以不实现，默认为NULL，也就是不做free的动作了

    data:就是指向元素的指针值了
    返回值: RET_OK等

2. DataCopyFuncV2:
    在insert或set by index时，是必须调用它的，深拷贝还是浅拷贝用户自己决定。用户
可以不实现，如果不实现，默认浅拷贝.
    在类初始化时就必须赋值，也就是说:只能有一种Copy行为。
    
3. DataCompareFuncV2:
    比较2个元素是否相同，该函数必须由用户提供。
    而且是临时给，一般该函数只用于DArrayV2_FindElem(),作为参数传入，告诉DArrayV2匹配条件。
    第一版:ret = 0表示匹配. data就是void*元素, ctx是匹配值，用户临时传入的上下文，一般和void*指向的struct类型是相同的.用于DArray_FindElem(), ctx >= data, 返回值为>=0.
    第二版:ret = 0表示匹配. data就是元素, ctx是匹配值，用户临时传入的上下文，一般和DArrayV2的struct类型相同, 用于DArray_FindElem()
    
4. DataVisitFuncV2:
    作用是在访问一个元素时，做什么样的操作。
    临时作为参数传入DArrayV2_ForeachElem()，告诉DArrayV2遍历数组做什么事情。
    第一版:Ret, data是其中一个void*元素, ctx是用户临时传入的上下文,用于DArray_ForeachElem()
    第二版:Ret, data是其中一个struct的指针, ctx是用户临时传入的上下文,用于DArray_ForeachElem()
*******************************************************************************/
typedef void    (*DataDestroyFunc)(void* ctx, void* data); //销毁void*元素data所指向的struct, ctx是销毁用的上下文，类初始化时就传入了指针，可以修改其指向的struct
typedef int     (*DataCompareFunc)(void* ctx, void* data); //ret = 0表示匹配. data就是void*元素, ctx是匹配值，用户临时传入的上下文，一般和void*指向的struct类型是相同的.用于DArray_FindElem(), ctx >= data, 返回值为>=0.
typedef int     (*DataVisitFunc)(void* ctx, void* data);   //Ret, data是其中一个void*元素, ctx是用户临时传入的上下文,用于DArray_ForeachElem()

typedef struct tag_DArray DArray;
typedef int (*DArray_InsertElem)(DArray* thiz, unsigned int index, void* data);  //返回值Ret_OK
typedef int (*DArray_PrependElem)(DArray* thiz, void* data);    //返回值Ret_OK
typedef int (*DArray_AppendElem)(DArray* thiz, void* data);     //返回值Ret_OK
typedef int (*DArray_DeleteElem)(DArray* thiz, unsigned int index);   //返回值Ret_OK
typedef int (*DArray_GetElemByIndex)(DArray* thiz, unsigned int index, void** data);  //返回值Ret_OK
typedef int (*DArray_SetElemByIndex)(DArray* thiz, unsigned int index, void* data);   //返回值Ret_OK
typedef unsigned int (*DArray_GetLength)(DArray* thiz);  //返回值为元素个数
typedef int (*DArray_ForeachElem)(DArray* thiz, DataVisitFunc visit, void* ctx);    //返回值Ret_OK
typedef unsigned int (*DArray_FindElem)(DArray* thiz, DataCompareFunc cmp, void* ctx);   //返回值为下标号
typedef struct tag_DArray
{
	void** data;    //void* 数组的起始地址,每个void*元素指向一个真正的数据结构, 那才是我们所要操作的对象
	unsigned int size;    //当前已占的元素个数
	unsigned int alloc_size;  //当前已经malloc出来的元素总个数
    unsigned int nMinPreAllocNr;    //最初预分配的元素个数, #define MIN_PRE_ALLOCATE_NR 10
    
	void* data_destroy_ctx; //调用data_destroy()时，作为销毁上下文，作为销毁时的参考，还不知道怎样用，但销毁一个元素前，可以更改ctx的内容，因此有灵活性
	DataDestroyFunc data_destroy;   //删除元素时，如果元素是指针，指向一个malloc的数据结构，就需要调用该函数free.当然，如果struct数组是固定的数组，不需要在DArray里面free，就赋NULL。

    //DArray的操作，对外公开的API
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
//有时候，动态数组自己直接作为变量存在，这时初始化就行了
extern int DArray_Init(DArray* thiz, DataDestroyFunc data_destroy, void* data_destroy_ctx, unsigned int nMinPreAllocNr);
extern int DArray_Exit(DArray* thiz);
//有时候，动态数组只是作为一个指针存在，这时需要new一个出来
extern DArray* newDArray(DataDestroyFunc data_destroy, void* data_destroy_ctx, unsigned int nMinPreAllocNr); //传入的参数是为了销毁void*元素指向的数据结构,这些真正的struct一般都是malloc出来的
extern void deleteDArray(DArray* thiz);  //删除DArray的所有元素，如果元素指向的数据结构需要free,也会调用data_destroy() 进行free

DECLS_END

#endif/*DARRAY_H*/

