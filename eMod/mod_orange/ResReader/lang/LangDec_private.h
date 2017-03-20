/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              eMOD Sub-System
*
*                                   (c) Copyright 2007-2009, SW.China
*                                             All Rights Reserved
*
* Moudle  : Lemon
* File    : LangDec_private.h
*
* By      : Andy
* Version : v1.0
* Date    : 2008-11-9 9:13:15
**********************************************************************************************************************
*/

#ifndef __LANGDEC_PRIVATE_H__
#define __LANGDEC_PRIVATE_H__

#include "const.h"
#if (EPDK_OS == EPDK_OS_WIN2K) || (EPDK_OS == EPDK_OS_WINXP) || (EPDK_OS == EPDK_OS_VISTA)

    #define LANGDEC_CHECK_ARG_EN 0

    #include <stdio.h>
    #include <assert.h>
    #include <stdlib.h>
    #include <string.h>

    #define __PACKED

    typedef unsigned int        __u32;
    typedef unsigned short      __u16;
    typedef unsigned char 	    __u8;
    typedef signed int          __s32;
    typedef signed short        __s16;
    typedef signed char         __s8;
    typedef signed char 	    __bool;
    
#elif EPDK_OS == EPDK_OS_EPOS
    
	#include "mod_orange_i.h"

    #if EPDK_DEBUG_LEVEL >= EPDK_DEBUG_LEVEL_CHECK_ALL
        #define LANGDEC_CHECK_ARG_EN 1
    #else
        #define LANGDEC_CHECK_ARG_EN 0
    #endif

    #define __PACKED    __packed
    #define fopen1       esRESM_ROpen
    #define fclose1      esRESM_RClose
    #define fread1       esRESM_RRead
    #define fseek1       esRESM_RSeek
    
    #define malloc(x)   esMEMS_Malloc(0, x)
    #define free(x)     esMEMS_Mfree(0, x)
    #define FILE        __resm_rsb_t
    
    #define strcmp      eLIBs_strcmp
    #define memcpy      eLIBs_memcpy
    

#else
    #error please select EPDK_OS
#endif

#include "lang_fmt.h"

typedef struct tag_HLANGLIST
{	
	__u16		LangID;			//LangID�� �磺Ox400
	__u32		StringTabOff;	//�ַ������Ӧ��ʼ��ַ��
}HLANGLIST;

typedef struct tag_HLANHEAD
{
	FILE 	    *fp;	
	HLANGLIST	*pHLangList;	//ָ��������Ϣ����Ϣ��
	
	__u32		LangTabOff;		//LANG����ʼ��ַƫ����
	__u16		LangSize;		//LANG����ÿ����Ŀ��С	
	__u32		LangNum;		//LANG����Ŀ����,�������������
	
	__u16		StringSize;		//�ַ���size
	__u32		StringNum;		//�ַ���������
	__u32		align;			//���ݱ߽����ģʽ��	
	__u16		*pIdTab;
	
}HHEAD;


#endif /* __LANGDEC_PRIVATE_H__  */




