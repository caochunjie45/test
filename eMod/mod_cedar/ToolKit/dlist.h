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
* File   : dlist.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-5-10
* Description:
    关于双向链表的说明:
    这里实现的双向链表不循环，即第一个node的prev =NULL, 最后一个node的next = NULL.
    插入元素时，如果指定的index不对，就默认加到队尾。index从0开始.
    
    
********************************************************************************
*/
/*
 * File:    dlist.h
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   double list header file.
 *
 * Copyright (c) Li XianJing
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2008-11-24 Li XianJing <xianjimli@hotmail.com> created
 * 2008-12-08 Li XianJing <xianjimli@hotmail.com> add autotest.
 *
 */

//#include <stdio.h>

#ifndef DLIST_H
#define DLIST_H

DECLS_BEGIN;
//#ifdef __cplusplus
//extern "C" {
//#endif/*__cplusplus*/

typedef enum _DListRet
{
	DLIST_RET_OK = 0,
	DLIST_RET_OOM = -2,  //内存malloc失败
	DLIST_RET_STOP = -3,
	DLIST_RET_INVALID_PARAMS = -4,
	DLIST_RET_FAIL = -1,
}DListRet;

//下面的3个回调函数,DListDataDestroyFunc()在类初始化时传入.DList销毁一个元素时使用，其他两个是临时传入的参数
typedef void     (*DListDataDestroyFunc)(void* ctx, void* data);    //DList初始化时就传入, 运行过程中有效, ctx是上下文，初始化时就传入
typedef int      (*DListDataCompareFunc)(void* ctx, void* data);    //DList find elem时才临时传入, ctx是临时传入的上下文
typedef DListRet (*DListDataVisitFunc)(void* ctx, void* data);      //DList foreach elem时临时传入, ctx是临时传入的上下文

typedef struct _DListNode DListNode;
typedef struct _DListNode
{
	DListNode* prev;
	DListNode* next;

	void* data;
}DListNode;

typedef struct _DList DList;
typedef int (*DList_InsertElem)(DList *thiz, unsigned int index, void* data);   //data的值会赋给DListNode->data,是一个指针, data就是要插入的元素,内存使用者自己分配.ret = DListRet
typedef int (*DList_PrependElem)(DList *thiz, void* data);  //ret = DListRet
typedef int (*DList_AppendElem)(DList *thiz, void* data);   //ret = DListRet
typedef int (*DList_DeleteElem)(DList *thiz, unsigned int index);   //ret = DListRet, 元素DList不负责释放,使用者自己处理
typedef int (*DList_GetElemByIndex)(DList *thiz, unsigned int index, void** data);  //*data就是指向struct的一个指针, //ret = DListRet
typedef int (*DList_SetElemByIndex)(DList *thiz, unsigned int index, void* data);   //ret = DListRet
typedef unsigned int (*DList_GetLength)(DList *thiz);   //ret = size, 这里有个潜在规定,即使是unsigned int，也不能超过int所能表示的正数的范围,因为FindElem的返回值是int
typedef int (*DList_ForeachElem)(DList *thiz, DListDataVisitFunc visit, void* ctx); //ret = DListRet
typedef int (*DList_FindElem)(DList *thiz, DListDataCompareFunc cmp, void* ctx);    //ret = index, ret = -1 意外错误, size表示没找到
typedef struct _DList
{
	DListNode* first;
	void* data_destroy_ctx;
	DListDataDestroyFunc data_destroy;

    //成员函数
    DList_InsertElem        InsertElem;
    DList_PrependElem       PrependElem;
    DList_AppendElem        AppendElem;
    DList_DeleteElem        DeleteElem;
    DList_GetElemByIndex    GetElemByIndex;
    DList_SetElemByIndex    SetElemByIndex;
    DList_GetLength         GetLength;
    DList_ForeachElem       ForeachElem;
    DList_FindElem          FindElem;
}DList;
extern int DList_Init(DList *thiz, DListDataDestroyFunc data_destroy, void* data_destroy_ctx);
extern int DList_Exit(DList *thiz);
extern DList* newDList(DListDataDestroyFunc data_destroy, void* data_destroy_ctx);
extern void deleteDList(DList *thiz);

//DList* dlist_create(DListDataDestroyFunc data_destroy, void* data_destroy_ctx);
//
//DListRet dlist_insert(DList* thiz, unsigned int index, void* data); //data的值会赋给DListNode->data
//DListRet dlist_prepend(DList* thiz, void* data);
//DListRet dlist_append(DList* thiz, void* data);
//DListRet dlist_delete(DList* thiz, unsigned int index);
//DListRet dlist_get_by_index(DList* thiz, unsigned int index, void** data);
//DListRet dlist_set_by_index(DList* thiz, unsigned int index, void* data);
//unsigned int dlist_length(DList* thiz);
//DListRet dlist_foreach(DList* thiz, DListDataVisitFunc visit, void* ctx);
//int      dlist_find(DList* thiz, DListDataCompareFunc cmp, void* ctx);
//
//void dlist_destroy(DList* thiz);

//#ifdef __cplusplus
//}
//#endif/*__cplusplus*/
DECLS_END

#endif/*DLIST*/

