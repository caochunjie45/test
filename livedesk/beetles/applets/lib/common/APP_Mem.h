/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2011, CHIPHD, China
*											 All Rights Reserved
*
* File    	: APP_Mem.h
* By      	: CQQ
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2011/06/28 10:34  create this file, implements the fundamental interface;
**************************************************************************************************************
*/
#ifndef __APP_MEM_H__C7FF2028_BBAF_4206_B67C_2DEE5D2EAF60__
#define __APP_MEM_H__C7FF2028_BBAF_4206_B67C_2DEE5D2EAF60__
//////////////////////////////////////////////////////////////////////////
#ifndef _WINDOWS//for ePDK
#include "epdk.h"

typedef char        TCHAR;
#define _T(x)       x
#endif
//////////////////////platform interface//////////////////////////////////
/////////////////////////////////�ڴ���
#ifdef _WINDOWS
//for win32
//�����ڴ�
#define ap_NewMem_B(_p, _size)                  ((_p) = (new BYTE[_size]))//((_p) = (void*)(new BYTE[_size]))
#define ap_NewMem_M(_p, _heap, _size)           ap_NewMem_B(_p, _size)
#define ap_NewMem_P(_p, _page, _mode)           ap_NewMem_B(_p, (_page) * 1024)

#define ap_NewMem_T_B(_p, _type)                ((_p) = new _type)
#define ap_NewMem_T_M(_p, _heap, _type)         ap_NewMem_T_B(_p, _type)

//�ͷ��ڴ�
#define ap_DelMem_B(_p, _size)                  (delete [] _p)
#define ap_DelMem_M(_p, _heap)                  (delete [] _p)
#define ap_DelMem_P(_p, _page)                  ap_DelMem_B(_p, (_page) * 1024)

#define ap_DelMem_T_B(_p, _type)                (delete _p)
#define ap_DelMem_T_M(_p, _heap)                (delete _p)

//�����ڴ�
#ifndef ZeroMemory
#define ZeroMemory(_p, _size)                    memset((_p), 0, (_size))
#endif

#else
//for other platform
//eDPK
//�����ڴ�
#define ap_NewMem_B(_p, _size)                    ((_p) = esMEMS_Balloc(_size))
#define ap_NewMem_M(_p, _heap, _size)             ((_p) = esMEMS_Malloc(_heap, _size))
#define ap_NewMem_P(_p, _page, _mode)             ((_p) = esMEMS_Palloc(_page, _mode))

#define ap_NewMem_T_B(_p, _type)                  ((_p) = (_type *)esMEMS_Balloc(sizeof(_type)))
#define ap_NewMem_T_M(_p, _heap, _type)           ((_p) = (_type *)esMEMS_Malloc(_heap, sizeof(_type)))

//�ͷ��ڴ�
#define ap_DelMem_B(_p, _size)                    (esMEMS_Bfree(_p, _size))
#define ap_DelMem_M(_p, _heap)                    (esMEMS_Mfree(_heap, _p))
#define ap_DelMem_P(_p, _page)                    (esMEMS_Pfree(_p, _page))

#define ap_DelMem_T_B(_p, _type)                  ap_DelMem_B(_p, sizeof(_type))
#define ap_DelMem_T_M(_p, _heap)                  ap_DelMem_M(_p, _heap)

//�����ڴ�
#ifndef ZeroMemory
#define ZeroMemory(_p, _size)                    eLIBs_memset((_p), 0, (_size))
#endif

#endif //_WINDOWS

//���ĳ�����ڴ�
#define ZeroTypeMem(_p, _type)                   ZeroMemory(_p, sizeof(_type))

/////////////////////////////////�����
#ifdef _WINDOWS
//ͼƬ��Դ
#define ap_dsk_theme_open(_h, _id)               NULL
#define ap_dsk_theme_close(_h)                   NULL

//�ļ�
#define ap_fopen(_f, _name, _mode)               NULL
#define ap_fclose(_f)                            NULL

//ͼ��
#define ap_create_layer(_layer, _pcreateinfo)    NULL
#define ap_destroy_layer(_layer)                 NULL

#else//EDPK_OS
//ͼƬ��Դ
#define ap_dsk_theme_open(_h, _id)               (_h = dsk_theme_open(_id))
#define ap_dsk_theme_close(_h)                   dsk_theme_close(_h)

//�ļ�
#define ap_fopen(_f, _name, _mode)               (_f = eLIBs_fopen(_name, _mode))
#define ap_fclose(_f)                            eLIBs_fclose(_f)

//ͼ��
#define ap_create_layer(_layer, _pcreateinfo)    (_layer = GUI_LyrWinCreate(_pcreateinfo))
#define ap_destroy_layer(_layer)                 GUI_LyrWinDelete(_layer)

#endif //_WINDOWS

////////////////////////platform interface end////////////////////////////


///////////////////////////app interface//////////////////////////////////
//��Ҫ debug memory,�޸� DEBUG_APP_MEM ���弴��
#ifdef _WINDOWS

#ifdef _DEBUG
#define DEBUG_APP_MEM         1
#else
#define DEBUG_APP_MEM         0
#endif

#else
//EDPK_OS

#define DEBUG_APP_MEM ((EPDK_DEBUG_LEVEL_GATE == 4) && 0)

#endif

#if DEBUG_APP_MEM
//debug mem
extern void *AddDebugAPPMEM(void *p, int line, TCHAR *filename);
extern void *SubDebugAPPMEM(void *p);
extern void _CheckAPPMEM(void);//���û�ͷŵĽڵ�
extern void _EndOfDebugAPPMEM(void);//�ͷ�����
#define CheckAPPMEM()       do \
{esKSRV_SysInfo(); __msg("  !!!!  CheckAPPMEM, %s\n", __FUNCTION__); _CheckAPPMEM();} while (0)

#define StartDebugAPPMEM()  do \
{esKSRV_SysInfo(); __msg("  !!!!  StartDebugAPPMEM, %s\n", __FUNCTION__); _CheckAPPMEM();} while (0)

#define EndOfDebugAPPMEM()  do \
{_EndOfDebugAPPMEM(); esKSRV_SysInfo(); __msg("  !!!!  EndOfDebugAPPMEM, %s\n", __FUNCTION__);} while (0)

//�����ڴ�
#define APP_NewMem_B(_p, _size)                 do \
{ \
	ap_NewMem_B(_p, _size); \
	AddDebugAPPMEM(_p, __LINE__, _T(__FILE__)); \
} while (0)

#define APP_NewMem_M(_p, _heap, _size)          do \
{ \
	ap_NewMem_M(_p, _heap, _size); \
	AddDebugAPPMEM(_p, __LINE__, _T(__FILE__)); \
} while (0)

#define APP_NewMem_P(_p, _page, _mode)          do \
{ \
	ap_NewMem_P(_p, _page, _mode); \
	AddDebugAPPMEM(_p, __LINE__, _T(__FILE__)); \
} while (0)

#define APP_NewMem_T_B(_p, _type)               do \
{ \
	ap_NewMem_T_B(_p, _type); \
	AddDebugAPPMEM(_p, __LINE__, _T(__FILE__)); \
} while (0)

#define APP_NewMem_T_M(_p, _heap, _type)        do \
{ \
	ap_NewMem_T_M(_p, _heap, _type); \
	AddDebugAPPMEM(_p, __LINE__, _T(__FILE__)); \
} while (0)

//�ͷ��ڴ�
#define APP_DelMem_B(_p, _size)                 if (_p){SubDebugAPPMEM(_p); ap_DelMem_B(_p, _size); _p = NULL;}
#define APP_DelMem_M(_p, _heap)                 if (_p){SubDebugAPPMEM(_p); ap_DelMem_M(_p, _heap); _p = NULL;}
#define APP_DelMem_P(_p, _page)                 if (_p){SubDebugAPPMEM(_p); ap_DelMem_P(_p, _page); _p = NULL;}

#define APP_DelMem_T_B(_p, _type)               APP_DelMem_B(_p, sizeof(_type))
#define APP_DelMem_T_M(_p, _heap)               APP_DelMem_M(_p, _heap)

/////////////////////////////////�����
//ͼƬ��Դ
#define APP_dsk_theme_open(_h, _id)             do \
{ \
	ap_dsk_theme_open(_h, _id); \
	AddDebugAPPMEM((void*)_h, __LINE__, _T(__FILE__)); \
} while (0)

#define APP_dsk_theme_close(_h)                 do { SubDebugAPPMEM((void*)_h); ap_dsk_theme_close(_h); } while (0)

//�ļ�
#define APP_fopen(_f, _name, _mode)             do \
{ \
	ap_fopen(_f, _name, _mode); \
	AddDebugAPPMEM((void*)_f, __LINE__, _T(__FILE__)); \
} while (0)

#define APP_fclose(_f)                          if (_f) {SubDebugAPPMEM((void*)_f); ap_fclose(_f); _f = NULL;}

//ͼ��
#define APP_create_layer(_layer, _pcreateinfo)  do \
{ \
	ap_create_layer(_layer, _pcreateinfo); \
	AddDebugAPPMEM((void*)_layer, __LINE__, _T(__FILE__)); \
} while (0)

#define APP_destroy_layer(_layer)               if (_layer) {SubDebugAPPMEM((void*)_layer); ap_destroy_layer(_layer); _layer = NULL;}

#else

#define CheckAPPMEM()          NULL
#define EndOfDebugAPPMEM()     NULL
#define StartDebugAPPMEM()     NULL

//eDPK
//�����ڴ�
#define APP_NewMem_B(_p, _size)                 ap_NewMem_B(_p, _size)
#define APP_NewMem_M(_p, _heap, _size)          ap_NewMem_M(_p, _heap, _size)
#define APP_NewMem_P(_p, _page, _mode)          ap_NewMem_P(_p, _page, _mode)

#define APP_NewMem_T_B(_p, _type)               ap_NewMem_T_B(_p, _type)
#define APP_NewMem_T_M(_p, _heap, _type)        ap_NewMem_T_M(_p, _heap, _type)

//�ͷ��ڴ�
#define APP_DelMem_B(_p, _size)                 if (_p){ap_DelMem_B(_p, _size); _p = NULL;}
#define APP_DelMem_M(_p, _heap)                 if (_p){ap_DelMem_M(_p, _heap); _p = NULL;}
#define APP_DelMem_P(_p, _page)                 if (_p){ap_DelMem_P(_p, _page); _p = NULL;}

#define APP_DelMem_T_B(_p, _type)               APP_DelMem_B(_p, sizeof(_type))
#define APP_DelMem_T_M(_p, _heap)               APP_DelMem_M(_p, _heap)

/////////////////////////////////�����
//ͼƬ��Դ
#define APP_dsk_theme_open(_h, _id)             ap_dsk_theme_open(_h, _id)
#define APP_dsk_theme_close(_h)                 ap_dsk_theme_close(_h) //dsk_theme_close�ڲ������ж�

//�ļ�
#define APP_fopen(_f, _name, _mode)             ap_fopen(_f, _name, _mode)
#define APP_fclose(_f)                          if (_f) {ap_fclose(_f); _f = NULL;}

//ͼ��
#define APP_create_layer(_layer, _pcreateinfo)  ap_create_layer(_layer, _pcreateinfo)
#define APP_destroy_layer(_layer)               if (_layer) {ap_destroy_layer(_layer); _layer = NULL;}

#endif //DEBUG_APP_MEM
///////////////////////////app interface end//////////////////////////////

#endif //__APP_MEM_H__C7FF2028_BBAF_4206_B67C_2DEE5D2EAF60__
//End of this file

