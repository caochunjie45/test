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
* File   : darray_v2.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-5-7
* Description:
    第二版和第一版的不同在于，第二版DArray自己直接按struct的大小分配内存，
    第一版用void*数组, 指向的struct仍然要用户自己分配，实际上对于struct而言，并不是连续的，
    只是void*连续而已。
    
    如果数据量小，用第二版更直接.即第二版darrayV2适用于struct不是很大，数量不是很多，但用户希望像真正的数组那样操作的情况

    如果数据量大，插入、删除操作少, 查找多，用第一版好一些。struct是malloc出来的，但是查找，排序方便。
一般void*指向的struct是malloc出来的。

    如果数据量大, 插入、删除操作多，那么应该用双向链表。void*一般指向的struct是malloc出来的。

    使用注意事项:
    thiz->data_size是一个struct在内存中占据的字节数，一般大于等于它自身的字节数，
    因为有内存对齐的原因，一定要用sizeof()来得到值

    关于需要调用者实现的4个回调函数的说明: DArray两个版本都适用
    DataExitFuncV2:如果用户在struct里有指针，其指向的地址是malloc出来的，那么用户一定要实现这个回调函数，在反初始化时，DArrayV2会调用。
如果用户不需要free，那么可以置为NULL。这样DArray就不做什么特殊操作了
    DataCopyFuncV2: 如果用户希望深拷贝，那么就自己实现这个回调函数. DArray会在赋值时调用。如果用户
没有特殊操作，可以置为NULL。DArray默认浅拷贝。
    上面两个函数是在DArray初始化时传入的，在DArray运行期间不能改变的。

    DataCompareFuncV2:
    DataVisitFuncV2:
    这两个回调函数是在findelem和foreach时作为参数传入的,每次可以不同。以实现不同的
查找要求或遍历时改变struct的需求。
********************************************************************************
*/
#ifndef _DARRAY_V2_H_
#define _DARRAY_V2_H_

DECLS_BEGIN;


//typedef Ret (*SortFunc)(void** array, size_t nr, DataCompareFunc cmp);    //动态数组是可以快速排序的

//元素data struct如果自己的成员有malloc出来的内存, 那么该struct在废弃前或被取代前必须exit一下，
//把内存和其他资源释放掉. ctx是exit用的上下文，类初始化时就传入了指针，可以在调用exit()前修改其指向的数据结构，exit()执行
//过程中用于参考
typedef int      (*DataExitFuncV2)(void* data_exit_ctx, void* data); //RET_OK. 反初始化时, struct内的指针，要不要free由使用者自己决定, ctx在类初始化时就传入
typedef int      (*DataCopyFuncV2)(void* pDesdata, void* pSrcdata); //RET_OK. 深拷贝、浅拷贝由使用者自己决定
//typedef int      (*DataCompareFuncV2)(void* ctx, void* data); //ret = 0表示匹配. data就是元素, ctx是匹配值，用户临时传入的上下文，一般和DArrayV2的struct类型相同, 用于DArray_FindElem()
//typedef int      (*DataVisitFuncV2)(void* ctx, void* data);   //Ret, data是其中一个struct的指针, ctx是用户临时传入的上下文,用于DArray_ForeachElem()

typedef struct tag_DArrayV2 DArrayV2;
typedef int (*DArrayV2_InsertElem)(DArrayV2* thiz, unsigned int index, void* data);  //返回值Ret_OK
typedef int (*DArrayV2_PrependElem)(DArrayV2* thiz, void* data);    //返回值Ret_OK
typedef int (*DArrayV2_AppendElem)(DArrayV2* thiz, void* data);     //返回值Ret_OK, data是指向struct的指针
typedef int (*DArrayV2_DeleteElem)(DArrayV2* thiz, unsigned int index);   //返回值Ret_OK
typedef int (*DArrayV2_GetElemByIndex)(DArrayV2* thiz, unsigned int index, void** data);  //返回值Ret_OK, data是指向struct的指针，只得到指针值, void**data = 具体struct的指针
typedef int (*DArrayV2_SetElemByIndex)(DArrayV2* thiz, unsigned int index, void* data);   //返回值Ret_OK, 要取代原有的元素,原来的元素会调用exit()反初始化，然后调用copy()函数
typedef unsigned int (*DArrayV2_GetLength)(DArrayV2* thiz);  //返回值为元素个数
typedef int (*DArrayV2_ForeachElem)(DArrayV2* thiz, DataVisitFunc visit, void* ctx);    //返回值Ret_OK
typedef unsigned int (*DArrayV2_FindElem)(DArrayV2* thiz, DataCompareFunc cmp, void* ctx);   //返回值为下标号, size表示没找到
typedef struct tag_DArrayV2
{
	void* data;                     // 用户的struct数组的起始地址,就是我们所要操作的对象数组,因为不知道类型，所以必须知道一个对象的大小，才能分配内存
    unsigned int data_size;         // 一个struct元素所占的字节数
	unsigned int size;              // 当前已占的元素个数
	unsigned int alloc_size;        // 当前已经malloc出来的元素总个数
    unsigned int nMinPreAllocNr;    // 最初预分配的元素个数, #define MIN_PRE_ALLOCATE_NR 10

	void* data_exit_ctx; //调用data_exit()时，作为反初始化上下文，作为销毁时的参考，还不知道怎样用. 在类初始化时就传入了. 每反初始化一个元素前，可以更改ctx的内容，在exit()时做参考. 因此有灵活性
	DataExitFuncV2 data_exit;   //删除元素时，如果元素是指针，指向一个malloc的数据结构，就需要调用该函数free. 如果置为NULL，就不调用了
	DataCopyFuncV2 data_copy;   //InsertElem时，一般用这个函数拷贝，如果置为NULL，就默认浅拷贝

    //DArray的操作，对外公开的API
    DArrayV2_InsertElem       InsertElem;
    DArrayV2_PrependElem      PrependElem;
    DArrayV2_AppendElem       AppendElem;   //插入队尾
    DArrayV2_DeleteElem       DeleteElem;
    DArrayV2_GetElemByIndex   GetElemByIndex;
    DArrayV2_SetElemByIndex   SetElemByIndex;
    DArrayV2_GetLength        GetLength;
    DArrayV2_ForeachElem      ForeachElem;
    DArrayV2_FindElem         FindElem;
}DArrayV2;
//有时候，动态数组自己直接作为变量存在，这时初始化就行了
extern int DArrayV2_Init(DArrayV2* thiz, DataExitFuncV2 data_exit, void* data_exit_ctx, DataCopyFuncV2 data_copy, unsigned int data_size, unsigned int nMinPreAllocNr);
extern int DArrayV2_Exit(DArrayV2* thiz);
//有时候，动态数组只是作为一个指针存在，这时需要new一个出来
extern DArrayV2* newDArrayV2(DataExitFuncV2 data_exit, void* data_exit_ctx, DataCopyFuncV2 data_copy, unsigned int data_size, unsigned int nMinPreAllocNr); //传入的参数是为了反初始化void*指向的struct数组,这些struct数组是DArrayV2自己malloc出来的逻辑连续的内存
extern void deleteDArrayV2(DArrayV2* thiz);  //删除DArray的所有元素，如果元素指向的数据结构需要free,也会调用data_destroy() 进行free

DECLS_END

#endif  /* _DARRAY_V2_H_ */

