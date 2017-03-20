/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2011, CHIPHD, China
*											 All Rights Reserved
*
* File    	: touch_adapter.c
* By      	: CQQ
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2011/06/17 13:55  create this file, implements the fundamental interface;
**************************************************************************************************************
*/
#include "touch_adapter.h"
//////////////////////////////////////////////////////////////////////////

/************************************************************************/
// function --- �ж�pos���ĸ�����
// rect_sz  --- ��������
// count    --- �������
// x        --- x ����
// y        --- y ����
// return   --- �����ĳ�������ڷ��ض�Ӧ�����idֵ(��0��)�����򷵻�-1
/************************************************************************/
__s32 touch_check_rect_sz(TOUCH_UI_RECT rect_sz[], __s32 count, TOUCH_UI_INT x, TOUCH_UI_INT y)
{
	__s32 i;

	for (i = count; i--; )
	{
		if (XInRect(x, rect_sz[i]) && YInRect(y, rect_sz[i]))
		{
			break;
		}
	}

	if (0 <= i)
	{
		return i;
	}
	else
	{
		return TOUCH_UI_INVALID_ID;
	}
}


/************************************************************************/
// function --- �ж�pos���ĸ�����
// rect_sz  --- ��������
// id_sz    --- ��Ӧ�����id����
// count    --- �������
// pos      --- ����
// return   --- �����ĳ�������ڷ��ض�Ӧ�����idֵ�����򷵻�-1
/************************************************************************/
__s32 touch_check_rect_sz_ex(TOUCH_UI_RECT rect_sz[], __s32 id_sz[], __s32 count, TOUCH_UI_POS* pos)
{
	__s32 i;

	for (i = count; i--; )
	{
		if (pPtInRect(pos, rect_sz[i]))
		{
			break;
		}
	}

	if (0 <= i)
	{
		return id_sz[i];
	}
	else
	{
		return TOUCH_UI_INVALID_ID;
	}
}


