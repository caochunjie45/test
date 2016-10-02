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
* File   : cedarlib_basic_typedef.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2010-11-2
* Description:
    这个头文件只能用于cedarlib的内部共用头文件.禁止用于对外API文件，因为它重定义了
__s32等类型，如果引用cedarlib的API文件的系统定义了__s32等类型的话，那么就会出现意想
之外的__s32被重定义的异常情况。
    [cedarlib]内部共用
********************************************************************************
*/
#ifndef _CEDARLIB_BASIC_TYPEDEF_H_
#define _CEDARLIB_BASIC_TYPEDEF_H_
//==============================================================================
//                              基本数据类型的重定义
//==============================================================================
#ifdef __u64
#undef __u64
#endif
#ifdef __u32
#undef __u32
#endif
#ifdef __u16
#undef __u16
#endif
#ifdef __u8
#undef __u8
#endif
#ifdef __s64
#undef __s64
#endif
#ifdef __s32
#undef __s32
#endif
#ifdef __s16
#undef __s16
#endif
#ifdef __s8
#undef __s8
#endif
#ifdef __bool
#undef __bool
#endif
#ifdef FALSE
#undef FALSE
#endif
#ifdef TRUE
#undef TRUE
#endif
#ifdef NULL
#undef NULL
#endif

#define __u64    unsigned long long
#define __u32    unsigned int
#define __u16    unsigned short
#define __u8     unsigned char
#define __s64    long long
#define __s32    int
#define __s16    short
#define __s8     char
#define __bool   char
#define TRUE    (1)
#define FALSE   (0)
#define NULL    (void*)0

typedef struct
{
    __u32   tmp;
}FILE;

#endif  /* _CEDARLIB_BASIC_TYPEDEF_H_ */

