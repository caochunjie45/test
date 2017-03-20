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
    ����˫�������˵��:
    ����ʵ�ֵ�˫������ѭ��������һ��node��prev =NULL, ���һ��node��next = NULL.
    ����Ԫ��ʱ�����ָ����index���ԣ���Ĭ�ϼӵ���β��index��0��ʼ.
    
    
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
	DLIST_RET_OOM = -2,  //�ڴ�mallocʧ��
	DLIST_RET_STOP = -3,
	DLIST_RET_INVALID_PARAMS = -4,
	DLIST_RET_FAIL = -1,
}DListRet;

//�����3���ص�����,DListDataDestroyFunc()�����ʼ��ʱ����.DList����һ��Ԫ��ʱʹ�ã�������������ʱ����Ĳ���
typedef void     (*DListDataDestroyFunc)(void* ctx, void* data);    //DList��ʼ��ʱ�ʹ���, ���й�������Ч, ctx�������ģ���ʼ��ʱ�ʹ���
typedef int      (*DListDataCompareFunc)(void* ctx, void* data);    //DList find elemʱ����ʱ����, ctx����ʱ�����������
typedef DListRet (*DListDataVisitFunc)(void* ctx, void* data);      //DList foreach elemʱ��ʱ����, ctx����ʱ�����������

typedef struct _DListNode DListNode;
typedef struct _DListNode
{
	DListNode* prev;
	DListNode* next;

	void* data;
}DListNode;

typedef struct _DList DList;
typedef int (*DList_InsertElem)(DList *thiz, unsigned int index, void* data);   //data��ֵ�ḳ��DListNode->data,��һ��ָ��, data����Ҫ�����Ԫ��,�ڴ�ʹ�����Լ�����.ret = DListRet
typedef int (*DList_PrependElem)(DList *thiz, void* data);  //ret = DListRet
typedef int (*DList_AppendElem)(DList *thiz, void* data);   //ret = DListRet
typedef int (*DList_DeleteElem)(DList *thiz, unsigned int index);   //ret = DListRet, Ԫ��DList�������ͷ�,ʹ�����Լ�����
typedef int (*DList_GetElemByIndex)(DList *thiz, unsigned int index, void** data);  //*data����ָ��struct��һ��ָ��, //ret = DListRet
typedef int (*DList_SetElemByIndex)(DList *thiz, unsigned int index, void* data);   //ret = DListRet
typedef unsigned int (*DList_GetLength)(DList *thiz);   //ret = size, �����и�Ǳ�ڹ涨,��ʹ��unsigned int��Ҳ���ܳ���int���ܱ�ʾ�������ķ�Χ,��ΪFindElem�ķ���ֵ��int
typedef int (*DList_ForeachElem)(DList *thiz, DListDataVisitFunc visit, void* ctx); //ret = DListRet
typedef int (*DList_FindElem)(DList *thiz, DListDataCompareFunc cmp, void* ctx);    //ret = index, ret = -1 �������, size��ʾû�ҵ�
typedef struct _DList
{
	DListNode* first;
	void* data_destroy_ctx;
	DListDataDestroyFunc data_destroy;

    //��Ա����
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
//DListRet dlist_insert(DList* thiz, unsigned int index, void* data); //data��ֵ�ḳ��DListNode->data
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

