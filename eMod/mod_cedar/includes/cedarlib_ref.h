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
    在这里声明cedar lib引用的一些函数或定义的宏，这些函数以及参数的数据类型是某些系统定义和提供的，
    是影响移植性的操作。应该尽可能的消除。cedarlib_ref.h定义一些公共的配置。各个库
    再定义自己需要的配置.如file_depack_lib_ref.h, vdec_lib_ref.h等.
    
    目前Cedarlib_ref.h中会定义打印设置,在include文件中，本头文件一定要放在cedarlib_dbgs.h之前，否则
    这里的打印设置无效.配置以后，avi depack lib只适用于在特定系统环境下编译调试。
    
    avi depack lib在release二进制文件给客户或者在其他环境下编译时，这里的内容要全部注释掉再编译，以适用于
    更多的环境(只要客户的工程也使用arm编译器即可)，编译通过
    [cedarlib]
********************************************************************************
*/
#ifndef _CEDARLIB_REF_H_
#define _CEDARLIB_REF_H_

//==============================================================================
//                      EPDK系统的打印设置
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

