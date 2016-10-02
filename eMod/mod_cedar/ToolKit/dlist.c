/*
 * File:    dlist.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   double list implementation.
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
//#include <stdlib.h>
//#include "dlist_s.h"
#include "ToolKit_s.h"
#include "dlist.h"

static void dlist_destroy_data(DList* thiz, void* data)
{
	if(thiz->data_destroy != NULL)
	{
		thiz->data_destroy(thiz->data_destroy_ctx, data);
	}

	return;
}

static DListNode* dlist_create_node(DList* thiz, void* data)
{
	DListNode* node = malloc(sizeof(DListNode));

	if(node != NULL)
	{
		node->prev = NULL;
		node->next = NULL;
		node->data = data;
	}

	return node;
}

static void dlist_destroy_node(DList* thiz, DListNode* node)
{
	if(node != NULL)
	{
		node->next = NULL;
		node->prev = NULL;
		dlist_destroy_data(thiz, node->data);
		free(node);
	}

	return;
}

/*******************************************************************************
Function name: dlist_get_node
Description: 
    1.得到执行位置的元素指针, index从0开始
Parameters: 
    1.fail_return_last的含义:如果找不到那个插入点，就返回最后一个元素的指针
        如果置1,表示如果index太大了, 超出了当前有效个数,那么,返回最后一个元素.
        置0,会返回NULL.
        index默认从0开始, 有效范围值是0, n-1, n为有效元素个数. 如果是添加到队尾,
    插入位置应该为n，但这样的话，如果fail_return_last =0,会返回NULL。
Return: 
    
Time: 2011/5/10
*******************************************************************************/
static DListNode* dlist_get_node(DList* thiz, unsigned int index, int fail_return_last)
{
	DListNode* iter = NULL;
	
	return_val_if_fail(thiz != NULL, NULL); 

	iter = thiz->first;

	while(iter != NULL && iter->next != NULL && index > 0)
	{
		iter = iter->next;
		index--;
	}

	if(!fail_return_last)
	{
		iter = index > 0 ? NULL : iter;
	}

	return iter;
}

int dlist_insert(DList* thiz, unsigned int index, void* data)  //ret = DListRet
{
	DListNode* node = NULL;
	DListNode* cursor = NULL;

	return_val_if_fail(thiz != NULL, DLIST_RET_INVALID_PARAMS); 

	if((node = dlist_create_node(thiz, data)) == NULL)
	{
		return DLIST_RET_OOM; 
	}
    //空队列
	if(thiz->first == NULL)
	{
		thiz->first = node;

		return DLIST_RET_OK;
	}
    //指定fail_return_last=1之后, cursor一定是指向最后一个node元素，而且肯定不为NULL
	cursor = dlist_get_node(thiz, index, 1);
	//index的值在有效范围
	if(index < thiz->GetLength(thiz))
	{
        //index指向位置0,即第一个node.
		if(thiz->first == cursor)
		{
			thiz->first = node;
		}
		else    //index指向队列中间的一个位置
		{
			cursor->prev->next = node;
			node->prev = cursor->prev;
		}
		node->next = cursor;
		cursor->prev = node;
	}
	else    //默认添加到队尾
	{
		cursor->next = node;
		node->prev = cursor;
	}

	return DLIST_RET_OK;
}

int dlist_prepend(DList* thiz, void* data) //ret = DListRet
{
	return dlist_insert(thiz, 0, data);
}

int dlist_append(DList* thiz, void* data)   //ret = DListRet
{
	return dlist_insert(thiz, (unsigned int)-1, data);
}

int dlist_delete(DList* thiz, unsigned int index)   //ret = DListRet
{
	DListNode* cursor = dlist_get_node(thiz, index, 0);
	
	return_val_if_fail(cursor != NULL, DLIST_RET_INVALID_PARAMS); 

	if(cursor != NULL)
	{
		if(cursor == thiz->first)
		{
			thiz->first = cursor->next;
		}

		if(cursor->next != NULL)
		{
			cursor->next->prev = cursor->prev;
		}

		if(cursor->prev != NULL)
		{
			cursor->prev->next = cursor->next;
		}

		dlist_destroy_node(thiz, cursor);
	}

	return DLIST_RET_OK;
}

int dlist_get_by_index(DList* thiz, unsigned int index, void** data)    //ret = DListRet
{
	DListNode* cursor = dlist_get_node(thiz, index, 0);

	return_val_if_fail(cursor != NULL, DLIST_RET_INVALID_PARAMS); 

	if(cursor != NULL)
	{
		*data = cursor->data;
	}

	return cursor != NULL ? DLIST_RET_OK : DLIST_RET_FAIL;
}

int dlist_set_by_index(DList* thiz, unsigned int index, void* data) //ret = DListRet
{
	DListNode* cursor = dlist_get_node(thiz, index, 0);

	return_val_if_fail(cursor != NULL, DLIST_RET_INVALID_PARAMS); 

	if(cursor != NULL)
	{
		cursor->data = data;
	}

	return cursor != NULL ? DLIST_RET_OK : DLIST_RET_FAIL;
}

unsigned int dlist_length(DList* thiz)
{
	unsigned int length = 0;
	DListNode* iter = NULL;
	
	return_val_if_fail(thiz != NULL, 0);

	iter = thiz->first;

	while(iter != NULL)
	{
		length++;
		iter = iter->next;
	}

	return length;
}

int dlist_foreach(DList* thiz, DListDataVisitFunc visit, void* ctx) //ret = DListRet
{
	DListRet ret = DLIST_RET_OK;
	DListNode* iter = NULL;
	
	return_val_if_fail(thiz != NULL && visit != NULL, DLIST_RET_INVALID_PARAMS);

	iter = thiz->first;

	while(iter != NULL && ret != DLIST_RET_STOP)
	{
		ret = visit(ctx, iter->data);

		iter = iter->next;
	}

	return ret;
}

/*******************************************************************************
Function name: dlist_find
Description: 
    
Parameters: 
    
Return: 
    如果返回值 i == size, 表示没找到
Time: 2011/5/10
*******************************************************************************/
int dlist_find(DList* thiz, DListDataCompareFunc cmp, void* ctx)
{
	int i = 0;
	DListNode* iter = NULL;

	return_val_if_fail(thiz != NULL && cmp != NULL, -1);

	iter = thiz->first;
	while(iter != NULL)
	{
		if(cmp(ctx, iter->data) == 0)
		{
			break;
		}
		i++;
		iter = iter->next;
	}

	return i;
}

int DList_Init(DList *thiz, DListDataDestroyFunc data_destroy, void* data_destroy_ctx)
{
    return_val_if_fail(thiz, DLIST_RET_INVALID_PARAMS);
	thiz->first = NULL;
	thiz->data_destroy = data_destroy;
	thiz->data_destroy_ctx = data_destroy_ctx;
	
    thiz->InsertElem        = dlist_insert;
    thiz->PrependElem       = dlist_prepend;
    thiz->AppendElem        = dlist_append;
    thiz->DeleteElem        = dlist_delete;
    thiz->GetElemByIndex    = dlist_get_by_index;
    thiz->SetElemByIndex    = dlist_set_by_index;
    thiz->GetLength         = dlist_length;
    thiz->ForeachElem       = dlist_foreach;
    thiz->FindElem          = dlist_find;
    return DLIST_RET_OK;
}
int DList_Exit(DList *thiz)
{
    DListNode* iter = NULL;
	DListNode* next = NULL;
	
	return_val_if_fail(thiz != NULL, DLIST_RET_INVALID_PARAMS);

	iter = thiz->first;
	while(iter != NULL)
	{
		next = iter->next;
		dlist_destroy_node(thiz, iter);
		iter = next;
	}

	thiz->first = NULL;
    return DLIST_RET_OK;
}
DList *newDList(DListDataDestroyFunc data_destroy, void* data_destroy_ctx)
{
    __s32   ret;
    DList* thiz = malloc(sizeof(DList));
    return_val_if_fail(thiz, NULL);
        
    memset(thiz, 0, sizeof(DList));
    ret = DList_Init(thiz, data_destroy, data_destroy_ctx);
    if(DLIST_RET_OK != ret)
    {
        WRN("DList init fail\n");
        goto _err0;
    }
	return thiz;

_err0:
    free(thiz);
    return NULL;
}

void deleteDList(DList *thiz)
{
    __s32   ret;
    ret = DList_Exit(thiz);
    if(DLIST_RET_OK != ret)
    {
        WRN("DList Exit fail\n");
    }
	free(thiz);
	return;
}


#if 0
DList* dlist_create(DListDataDestroyFunc data_destroy, void* data_destroy_ctx)
{
	DList* thiz = malloc(sizeof(DList));

	if(thiz != NULL)
	{
		thiz->first = NULL;
		thiz->data_destroy = data_destroy;
		thiz->data_destroy_ctx = data_destroy_ctx;
	}

	return thiz;
}
void dlist_destroy(DList* thiz)
{
	DListNode* iter = NULL;
	DListNode* next = NULL;
	
	return_if_fail(thiz != NULL);

	iter = thiz->first;
	while(iter != NULL)
	{
		next = iter->next;
		dlist_destroy_node(thiz, iter);
		iter = next;
	}

	thiz->first = NULL;
	free(thiz);

	return;
}

#endif

