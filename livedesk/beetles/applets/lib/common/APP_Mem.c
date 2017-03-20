/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2011, CHIPHD, China
*											 All Rights Reserved
*
* File    	: APP_Mem.c
* By      	: CQQ
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2011/06/28 10:35  create this file, implements the fundamental interface;
**************************************************************************************************************
*/
//#include "StdAfx.h"

#ifdef _WINDOWS
// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <crtdbg.h>

#include "..\AppDebug.h"

#pragma warning( disable : 4996 )

#endif

///////////////////////////////
#include "APP_Mem.h"
///////////////////////////////

//////////////////////////////////////////////////////////////////////////
#ifndef _ASSERT
#define _ASSERT
#endif

#ifndef ASSERT
#define ASSERT _ASSERT
#endif

#ifdef _WINDOWS
#define APP_strncpy(_dst, _src, _n)     _tcsncpy(_dst, _src, _n)
#else
#define APP_strncpy(_dst, _src, _n)     eLIBs_strncpy(_dst, _src, _n)
#endif

#ifndef __msg
#define __msg                           DEBUG_Print
#endif

//////////////////////////////////////////////////////////////////////////
#if DEBUG_APP_MEM

//�����¼����
#define DEBUG_APP_MEM_FILE_NAME_MAX_MEMLEN    64
typedef struct tag_DEBUG_APP_MEM_LINK
{
	void  *pMEM;
	int   line;
	TCHAR file_name[DEBUG_APP_MEM_FILE_NAME_MAX_MEMLEN];

	struct tag_DEBUG_APP_MEM_LINK *next;
	struct tag_DEBUG_APP_MEM_LINK *prev;
}DEBUG_APP_MEM_LINK_T;

//����ͷ
static DEBUG_APP_MEM_LINK_T g_DebugAPPMEM_Head = {NULL, 0, {0}, NULL, NULL};
//����βָ��
static DEBUG_APP_MEM_LINK_T *g_DebugAPPMEM_Tail = &g_DebugAPPMEM_Head;

//ͳ�������ڴ����
static int gs_NEW_MEM_COUNT = 0;
//ͳ������ڵ���� //TODO:to remove
static int gs_NEW_LINKNODE_COUNT = 0;

#define NewDebugAPPMEMNode(_p)  do {ap_NewMem_T_B(_p, DEBUG_APP_MEM_LINK_T); if (_p){gs_NEW_LINKNODE_COUNT++;}} while(0)
#define DelDebugAPPMEMNode(_p)  do {if (_p){gs_NEW_LINKNODE_COUNT--;} ap_DelMem_T_B(_p, DEBUG_APP_MEM_LINK_T);} while(0)

//����һ��node��¼
void *AddDebugAPPMEM(void *p, int line, TCHAR *filename)
{
	gs_NEW_MEM_COUNT++;
	ASSERT(g_DebugAPPMEM_Tail);

	APP_strncpy(g_DebugAPPMEM_Tail->file_name, filename, DEBUG_APP_MEM_FILE_NAME_MAX_MEMLEN - 1);
	g_DebugAPPMEM_Tail->line = line;
	g_DebugAPPMEM_Tail->pMEM = p;

	NewDebugAPPMEMNode(g_DebugAPPMEM_Tail->next);
	if (g_DebugAPPMEM_Tail->next)
	{
		ZeroTypeMem(g_DebugAPPMEM_Tail->next, DEBUG_APP_MEM_LINK_T);

		g_DebugAPPMEM_Tail->next->prev = g_DebugAPPMEM_Tail;
		g_DebugAPPMEM_Tail = g_DebugAPPMEM_Tail->next;
	}

	return p;
}

//����һ��node��¼
static DEBUG_APP_MEM_LINK_T* FindDebugAPPMEMNode(void *p)
{
	DEBUG_APP_MEM_LINK_T *Cur = &g_DebugAPPMEM_Head;

	while (Cur->pMEM != p)
	{
		if (Cur->next)
		{
			Cur = Cur->next;
		} 
		else
		{
			break;
		}
	}

	if (Cur->pMEM == p)
	{
		return Cur;
	} 
	else
	{
		return NULL;
	}
}

//ȥ��һ��node��¼
void *SubDebugAPPMEM(void *p)
{
	DEBUG_APP_MEM_LINK_T *Node;

	gs_NEW_MEM_COUNT--;

	Node = FindDebugAPPMEMNode(p);

	if (Node)
	{
		if (Node == &g_DebugAPPMEM_Head)
		{
			Node->file_name[0] = 0;
			Node->line         = 0;
			Node->pMEM         = NULL;
			//ZeroTypeMem(Node, DEBUG_APP_MEM_LINK_T);
		} 
		else
		{
			if (Node->prev)//ǰ�ڵ�nextָ���ڵ�
			{
				Node->prev->next = Node->next;
			}
			if (Node->next)//��ڵ�prevָ��ǰ�ڵ�
			{
				Node->next->prev = Node->prev;
			}

			ASSERT(Node != g_DebugAPPMEM_Tail);

			DelDebugAPPMEMNode(Node);
		}
	} 
	else
	{
		__msg("not find mem node, need check\n");
	}

	ASSERT(g_DebugAPPMEM_Tail);

	return Node;
}

//��ӡ��Ϣ
#ifdef _WINDOWS

#define DEBUG_APP_MEM_LINK_NOTFREE_TIP(_pNode)   \
	if ((_pNode)->pMEM) \
{ \
	DebugPrint(_T("%s(%d):memory not free\n"), (_pNode)->file_name, (_pNode)->line); \
}

#else

#define DEBUG_APP_MEM_LINK_NOTFREE_TIP(_pNode)   \
	if ((_pNode)->pMEM) \
{ \
	__msg("MEM [ %x ] isnot free, code at %s(%d):\n", (_pNode)->pMEM, (_pNode)->file_name, (_pNode)->line); \
}

#endif

//����¼
void _CheckAPPMEM(void)
{
	DEBUG_APP_MEM_LINK_T *Cur = &g_DebugAPPMEM_Head;

	__msg("!!!!!!!!!!!!!!!now gs_NEW_MEM_COUNT = %d !!!!!!!!!!!!!!!\n", gs_NEW_MEM_COUNT);

	while (Cur)
	{
		DEBUG_APP_MEM_LINK_NOTFREE_TIP(Cur);
		Cur = Cur->next;
	}
}

//�ͷ�����
void _EndOfDebugAPPMEM(void)
{
	DEBUG_APP_MEM_LINK_T *next;
	DEBUG_APP_MEM_LINK_T *Cur = &g_DebugAPPMEM_Head;

	__msg("!!!!!!!!!!!!!!!now gs_NEW_MEM_COUNT = %d !!!!!!!!!!!!!!!\n", gs_NEW_MEM_COUNT);

	DEBUG_APP_MEM_LINK_NOTFREE_TIP(Cur);

	Cur = Cur->next;
	while (Cur)
	{
		DEBUG_APP_MEM_LINK_NOTFREE_TIP(Cur);
		next = Cur->next;
		DelDebugAPPMEMNode(Cur);		
		Cur = next;
	}

	g_DebugAPPMEM_Tail = &g_DebugAPPMEM_Head;
	ZeroTypeMem(g_DebugAPPMEM_Tail, DEBUG_APP_MEM_LINK_T);

	ASSERT(gs_NEW_LINKNODE_COUNT == 0);
	if (gs_NEW_MEM_COUNT)
	{
		__err("gs_NEW_MEM_COUNT != 0");
	}
}

#endif //DEBUG_APP_MEM