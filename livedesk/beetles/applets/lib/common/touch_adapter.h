/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2011, CHIPHD, China
*											 All Rights Reserved
*
* File    	: touch_adapter.h
* By      	: CQQ
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2011/06/17 13:55  create this file, implements the fundamental interface;
**************************************************************************************************************
*/
#ifndef __TOUCH_ADAPTER_H__7E1D0283_7AB6_4807_A453_40CAF20877E5__
#define __TOUCH_ADAPTER_H__7E1D0283_7AB6_4807_A453_40CAF20877E5__
//////////////////////////////////////////////////////////////////////////
#include "epdk.h"

typedef  __s16        TOUCH_UI_INT;

typedef struct tag_TOUCH_UI_POS 
{
	TOUCH_UI_INT x;
	TOUCH_UI_INT y;
}TOUCH_UI_POS;

//distance pos (���귶Χ)
typedef struct tag_TOUCH_UI_DPOS
{
	TOUCH_UI_INT d0;
	TOUCH_UI_INT d1;
}TOUCH_UI_DPOS;

typedef struct tag_TOUCH_UI_RTSZ
{
	TOUCH_UI_INT width;
	TOUCH_UI_INT height;
}TOUCH_UI_RTSZ;

typedef struct tag_TOUCH_UI_RECT
{
	TOUCH_UI_INT x;
	TOUCH_UI_INT y;
	TOUCH_UI_INT width;
	TOUCH_UI_INT height;
}TOUCH_UI_RECT;

#define TOUCH_UI_INVALID_ID            -1
#define TouchValidID(_id)              (0 <= (_id))
//////////////////////////////////////////////////////////////////////////
#define ValueInRange(_min, _v, _max)        ((_min) <= (_v) && (_v) < (_max))
#define ValueBetweenRange(_min, _v, _max)   ((_min) <= (_v) && (_v) <= (_max))

#define ValueBetweenRangeEx(_min, _v1, _v2, _max) (((_min) <= (_v1)) && ((_v1) < (_v2)) && ((_v2) <= (_max)))

#define ValueOutOfRange(_min, _v, _max)     ((_v) < (_min) || (_max) < (_v))
///////////////

#define XInRect(_x, _rt)          ValueInRange(_rt.x, _x, _rt.x + _rt.width)
#define YInRect(_y, _rt)          ValueInRange(_rt.y, _y, _rt.y + _rt.height)
#define XYInRect(_x, _y, _rt)     (XInRect(_x, _rt) && YInRect(_y, _rt))

#define PtInRect(_pt, _rt)        (XInRect(_pt.x, _rt) && YInRect(_pt.y, _rt))
#define pPtInRect(_pPt, _rt)      (XInRect(_pPt->x, _rt) && YInRect(_pPt->y, _rt))

// #define PtInGuiRect(_pt, _Rect)   \
// 	( (_Rect.left <= _pt.x) && (_pt.x < _Rect.right) \
// 	&& (_Rect.top <= _pt.y) && (_pt.y < _Rect.bottom) )

//////////////////////////////////////////////////////////////////////////

/************************************************************************/
// function --- �ж�(x, y)���ĸ�����
// rect_sz  --- ��������
// count    --- �������
// x        --- x ����
// y        --- y ����
// return   --- �����ĳ�������ڷ�������Ķ�Ӧidֵ(��0��)�����򷵻�-1
/************************************************************************/
extern __s32 touch_check_rect_sz(TOUCH_UI_RECT rect_sz[], __s32 count, 
								 TOUCH_UI_INT x, TOUCH_UI_INT y);

/************************************************************************/
// function --- �ж�pos���ĸ�����
// rect_sz  --- ��������
// id_sz    --- ��Ӧ�����id����
// count    --- �������
// pos      --- ����
// return   --- �����ĳ�������ڷ��ض�Ӧ�����idֵ�����򷵻�-1
/************************************************************************/
extern __s32 touch_check_rect_sz_ex(TOUCH_UI_RECT rect_sz[], __s32 id_sz[], 
							 __s32 count, TOUCH_UI_POS* pos);

//////////////////////////////////////////////////////////////////////////

#endif //__TOUCH_ADAPTER_H__7E1D0283_7AB6_4807_A453_40CAF20877E5__
//End of this file

