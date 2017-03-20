/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2011, CHIPHD, China
*											 All Rights Reserved
*
* File    	: APP_Debug.h
* By      	: CQQ
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2011/07/12 20:07  create this file, implements the fundamental interface;
**************************************************************************************************************
*/
#ifndef __APP_DEBUG_H__5C18C02C_36F0_448C_AEF1_133B413C4B92__
#define __APP_DEBUG_H__5C18C02C_36F0_448C_AEF1_133B413C4B92__

#include "epdk.h"

//��ʶ��ת�ַ���
#ifndef _Symbol2Str
#define _Symbol2Str(str)                    #str
#endif

#ifndef Symbol2Str
#define Symbol2Str(str)                     _Symbol2Str(str)
#endif

#ifdef __MSG
#define SYSTEM_DEBUG_ON      1
#else
#define SYSTEM_DEBUG_ON      0
#endif

//����UI������ 
#define APP_DEBUG_UI         (SYSTEM_DEBUG_ON && 0)

#if APP_DEBUG_UI //(Ϊ�˴�ӡ��, �ɰ������ض����GUI����ʱ��������Ҫ��C�ļ���ʹ��)
//re define GUI_BMP_Draw
#ifdef GUI_BMP_Draw
#define MY_GUI_BMP_DRAW                      (ORANGEMOD_FUNC_ENTRY->ptr_GUI_BMP_Draw         	)
#undef GUI_BMP_Draw
#define GUI_BMP_Draw(_p, _x, _y)             do \
{\
	{__msg("    GUI_BMP_Draw(%d, %d)\n", _x, _y);} \
	MY_GUI_BMP_DRAW(_p, _x, _y); \
}while(0)
#endif

//re define GUI_DispStringAt
#ifdef GUI_DispStringAt
#define MY_GUI_DispStringAt                  (ORANGEMOD_FUNC_ENTRY->ptr_GUI_DispStringAt       	)
#undef GUI_DispStringAt
#define GUI_DispStringAt(_p, _x, _y)         do\
{ \
	{__msg("    GUI_DispStringAt(%d, %d)->( %s )\n", _x, _y, _p);} \
	MY_GUI_DispStringAt(_p, _x, _y); \
}while(0)
#endif

//re define GUI_DispStringInRect
#ifdef GUI_DispStringInRect
#define MY_GUI_DispStringInRect              (ORANGEMOD_FUNC_ENTRY->ptr_GUI_DispStringInRect   	)
#undef GUI_DispStringInRect
#define GUI_DispStringInRect(_p, _prt, _fmt)  do \
{ \
	{__msg("    GUI_DispStringInRect(%d, %d, %d, %d)->( %s )\n", (_prt)->x0, (_prt)->y0, (_prt)->x1, (_prt)->y1, _p);} \
	MY_GUI_DispStringInRect(_p, _prt, _fmt); \
} while (0)
#endif

#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////
#define DEBUG_RT(_tip, _rt)      __msg("%s, gui_rect(%d, %d, %d, %d)_WH(%d, %d)\n", \
	_tip, (_rt).x, (_rt).y, (_rt).x + (_rt).width, (_rt).y + (_rt).height, \
	(_rt).width, (_rt).height)

#define DEBUG_pRT(_tip, _rt)     __msg("%s, gui_rect(%d, %d, %d, %d)_WH(%d, %d)\n", \
	_tip, (_rt)->x, (_rt)->y, (_rt)->x + (_rt)->width, (_rt)->y + (_rt)->height, \
	(_rt)->width, (_rt)->height)

#define DEBUG_GUI_RT(_tip, _rt)  __msg("%s, gui_rect(%d, %d, %d, %d)_WH(%d, %d)\n", \
	_tip, (_rt).x0, (_rt).y0, (_rt).x1, (_rt).y1, \
	(_rt).x1 - (_rt).x0, (_rt).y1 - (_rt).y0)

#define DEBUG_GUI_pRT(_tip, _rt) __msg("%s, gui_rect(%d, %d, %d, %d)_WH(%d, %d)\n", \
	_tip, (_rt)->x0, (_rt)->y0, (_rt)->x1, (_rt)->y1, \
	(_rt)->x1 - (_rt)->x0, (_rt)->y1 - (_rt)->y0)


//////////////////////////for debug message
#define DEBUG_FunMsg(_FuncName, _msg)    __msg("    %s(%d, 0x%x, 0x%x)\n", \
	_FuncName, (_msg)->id, (_msg)->dwAddData1, (_msg)->dwAddData2)

#define DEBUG_FunMsgEx(_FuncName, _msg)  __msg(" [ %x ==> %x ] %s(%d, 0x%x, 0x%x)\n", \
	(_msg)->h_srcwin, (_msg)->h_deswin, _FuncName, (_msg)->id, (_msg)->dwAddData1, (_msg)->dwAddData2)

#define DEBUG_CBMSG(_func)               DEBUG_FunMsg(Symbol2Str(_func), msg)
#define DEBUG_CBMSGEx(_func)             DEBUG_FunMsgEx(Symbol2Str(_func), msg)

#define APP_DEBUG_CBMSGEx                DEBUG_FunMsgEx(__FUNCTION__, msg)

#ifdef __MSG

#define APP_DEBUG_TOUCH_MSG              do \
{ \
	char *_str[] = \
		{ \
			Symbol2Str(GUI_MSG_TOUCH_DOWN), \
			Symbol2Str(GUI_MSG_TOUCH_UP), \
			Symbol2Str(GUI_MSG_TOUCH_MOVE), \
			Symbol2Str(GUI_MSG_TOUCH_ZOOMIN), \
			Symbol2Str(GUI_MSG_TOUCH_ZOOMOUT), \
			Symbol2Str(GUI_MSG_TOUCH_NCDOWN), \
			Symbol2Str(GUI_MSG_TOUCH_NCUP), \
			Symbol2Str(GUI_MSG_TOUCH_NCMOVE), \
			Symbol2Str(GUI_MSG_TOUCH_OVERDOWN), \
			Symbol2Str(GUI_MSG_TOUCH_OVERUP), \
			Symbol2Str(GUI_MSG_TOUCH_OVERMOVE), \
			Symbol2Str(GUI_MSG_TOUCH_OVERZOOMIN), \
			Symbol2Str(GUI_MSG_TOUCH_OVERZOOMOUT), \
			Symbol2Str(GUI_MSG_TOUCH_LONGDOWN), \
			Symbol2Str(GUI_MSG_TOUCH_OVERLONGDOWN), \
		}; \
	if (msg->dwAddData1 - GUI_MSG_TOUCH_DOWN < BEETLES_TBL_SIZE(_str)) \
	{ \
		__msg(" %s [ %s ]( %d, %d )\n", \
			__FUNCTION__, _str[msg->dwAddData1 - GUI_MSG_TOUCH_DOWN], \
			LOSWORD(msg->dwAddData2), HISWORD(msg->dwAddData2)); \
	} \
	else \
	{ \
		__msg(" %s [ %d ]( %d, %d )\n", \
			__FUNCTION__, msg->dwAddData1, LOSWORD(msg->dwAddData2), HISWORD(msg->dwAddData2)); \
	} \
} while (0)

#else

#define APP_DEBUG_TOUCH_MSG               APP_DEBUG_CBMSGEx

#endif

//�˺���Ժ��������Ƿ����(���������з���ֵ�ĺ���)
#define DEBUG_CallFunc(_func, ...)        do \
{ \
	__msg("%s() call %s()\n", __FUNCTION__, Symbol2Str(_func)); \
	_func(__VA_ARGS__); \
	__msg("finish calling %s()\n", Symbol2Str(_func)); \
} while (0)

#define DEBUG_CallRetFunc(_func, ...)      \
	(__msg("%s() call %s()\n", __FUNCTION__, Symbol2Str(_func))-0, _func(__VA_ARGS__))

////////////////////////////for debug time const
#define DEBUG_TICK_TIME                (SYSTEM_DEBUG_ON && 0)//0

#if DEBUG_TICK_TIME

extern __u32 g_DebugTickCount;

#define DEBUG_Tick()                   \
{\
	__u32 _now = esKRNL_TimeGet(); \
	__msg("TickCount = %d, Now = %d\n", _now - g_DebugTickCount, _now); \
	g_DebugTickCount = _now; \
}

#else

#define DEBUG_Tick()

#endif

//////////////////////////////for debug res bmp data
#define RES_BMP_DATA_W(_p)   (*((__u16*)(_p) + 9))
#define RES_BMP_DATA_H(_p)   (0x10000 - *((__u16*)(_p) + 11))
#define DEBUG_BMP_DATA(_p)   __msg("bmp data %x(%x)-->[%c%c(%d, %d)]\n", (_p), esMEMS_VA2PA(_p), ((char*)(_p))[0], ((char*)(_p))[1], RES_BMP_DATA_W(_p), RES_BMP_DATA_H(_p))

//////////////////////////////for ASSERT
#ifdef __ast

#ifdef SIM_PC_WIN

#define ASSERT(condition)              do \
{\
	if(!(condition))\
	{\
	TCHAR szMessage[MAX_PATH*2];\
	_stprintf(szMessage, _T("\n%hs(%d):Assertion Failed!\n"), __FILE__, __LINE__);\
	OutputDebugString(szMessage);\
	DebugBreak();\
	}\
}while(0)

#else //no SIM_PC_WIN

#define ASSERT(condition)              \
	do \
{\
	if(!(condition)) {__inf("%s(%d): !!! ASSERT fail!\n", __FILE__, __LINE__);}\
} while (0)

#endif //SIM_PC_WIN

#else //no __ast

#define ASSERT(condition)              

#endif // __ast

//////////////////////////////////////////////////////////////////////////

#endif //__APP_DEBUG_H__5C18C02C_36F0_448C_AEF1_133B413C4B92__
//End of this file

