/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_duckweed sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : file_depack_ref.h (reference)
* Version: V1.0
* By     : Eric_wang
* Date   : 2010-10-26
* Description:
    ����������cedar lib���õ�һЩ��������ĺ꣬��Щ�����Լ�����������������ĳЩϵͳ������ṩ�ģ�
    ��Ӱ����ֲ�ԵĲ�����Ӧ�þ����ܵ�������cedarlib_ref.h����һЩ���������á�������
    �ٶ����Լ���Ҫ������.��file_depack_lib_ref.h, vdec_lib_ref.h��.
    
    ĿǰCedarlib_ref.h�лᶨ���ӡ����,��include�ļ��У���ͷ�ļ�һ��Ҫ����cedarlib_dbgs.h֮ǰ������
    ����Ĵ�ӡ������Ч.�����Ժ�avi depack libֻ���������ض�ϵͳ�����±�����ԡ�
    
    avi depack lib��release�������ļ����ͻ����������������±���ʱ�����������Ҫȫ��ע�͵��ٱ��룬��������
    ����Ļ���(ֻҪ�ͻ��Ĺ���Ҳʹ��arm����������)������ͨ��
    [cedarlib]
********************************************************************************
*/
#ifndef _CEDARLIB_REF_H_
#define _CEDARLIB_REF_H_

//==============================================================================
//                      EPDKϵͳ�Ĵ�ӡ����
//==============================================================================
#include "dbgs.h"
#if  (EPDK_DEBUG_LEVEL      == EPDK_DEBUG_LEVEL_NOCHECK)
    #define CEDARLIB_DEBUG_LEVEL (0)
#elif(EPDK_DEBUG_LEVEL    == EPDK_DEBUG_LEVEL_CHECK_ALL)
    #if(EPDK_DEBUG_LEVEL_GATE == 0)
        #define CEDARLIB_DEBUG_LEVEL (0)
    #elif(EPDK_DEBUG_LEVEL_GATE >= 1)
        #define CEDARLIB_DEBUG_LEVEL (1)
    #endif
#elif(EPDK_DEBUG_LEVEL    == EPDK_DEBUG_LEVEL_LOG_ERRORS)
    #if(EPDK_DEBUG_LEVEL_GATE == 0)
        #define CEDARLIB_DEBUG_LEVEL (0)
    #elif(EPDK_DEBUG_LEVEL_GATE == 1)
        #define CEDARLIB_DEBUG_LEVEL (1)
    #elif(EPDK_DEBUG_LEVEL_GATE >= 2)
        #define CEDARLIB_DEBUG_LEVEL (2)
    #endif

#elif(EPDK_DEBUG_LEVEL    == EPDK_DEBUG_LEVEL_LOG_WARNINGS)
    #if(EPDK_DEBUG_LEVEL_GATE == 0)
        #define CEDARLIB_DEBUG_LEVEL (0)
    #elif(EPDK_DEBUG_LEVEL_GATE == 1)
        #define CEDARLIB_DEBUG_LEVEL (1)
    #elif(EPDK_DEBUG_LEVEL_GATE == 2)
        #define CEDARLIB_DEBUG_LEVEL (2)
    #elif(EPDK_DEBUG_LEVEL_GATE >= 3)
        #define CEDARLIB_DEBUG_LEVEL (3)
    #endif

#elif(EPDK_DEBUG_LEVEL    == EPDK_DEBUG_LEVEL_LOG_ALL)
    #if(EPDK_DEBUG_LEVEL_GATE == 0)
        #define CEDARLIB_DEBUG_LEVEL (0)
    #elif(EPDK_DEBUG_LEVEL_GATE == 1)
        #define CEDARLIB_DEBUG_LEVEL (1)
    #elif(EPDK_DEBUG_LEVEL_GATE == 2)
        #define CEDARLIB_DEBUG_LEVEL (2)
    #elif(EPDK_DEBUG_LEVEL_GATE == 3)
        #define CEDARLIB_DEBUG_LEVEL (3)
    #elif(EPDK_DEBUG_LEVEL_GATE == 4)
        #define CEDARLIB_DEBUG_LEVEL (4)
    #endif
#else
    #error EPDK_DEBUG_LEVEL is error, please config DEBUGLEVEL to 0/1/2/3/4!
#endif

extern int eLIBs_printf( const char * str, ...);
#ifndef CEDARLIB_PRINTF
#define CEDARLIB_PRINTF eLIBs_printf
//#define CEDARLIB_PRINTF cedarlib_printf
#endif

#endif  /* _CEDARLIB_REF_H_ */

