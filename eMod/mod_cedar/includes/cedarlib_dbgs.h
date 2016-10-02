/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              eDBG Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : cedarlib 的打印设置
* File    : cedarlib_dbgs.h
*
* By      : eric_wang
* Version : v1.0
* Date    : 2010-11-1
* Description:
    专门为cedarlib定义一套打印设置
    实际只有7个打印宏:
    __cedarlib_ast   :level1,行号，文件名
    __cedarlib_msg   :level4,内容，行号，文件名都打印
    __cedarlib_err   :level2,内容，行号，文件名，和reset
    __cedarlib_wrn   :level3,内容，行号，文件名都打印
    __cedarlib_here__:level4,只打印行号，文件名
    __cedarlib_inf :  level4, 没有行号、文件名
    __cedarlib_log : 无条件打印，没有行号、文件名
    [cedarlib]内部共用
**********************************************************************************************************************
*/
#ifndef _CEDARLIB_DBGS_H_
#define _CEDARLIB_DBGS_H_

//cedarlib_ref.h有可能会定义CEDARLIB_DEBUG_LEVEL_GATE的等级,如果不定义，就是release给外面的版本，就以默认等级2来定义
#ifndef CEDARLIB_DEBUG_LEVEL
#define CEDARLIB_DEBUG_LEVEL        2
#endif

#if(CEDARLIB_DEBUG_LEVEL == 0)

#elif(CEDARLIB_DEBUG_LEVEL == 1)
    #define __ENABLE_ASSERT
#elif(CEDARLIB_DEBUG_LEVEL == 2)
    #define __ENABLE_ERR
    #define __ENABLE_ASSERT
#elif(CEDARLIB_DEBUG_LEVEL == 3)
    #define __ENABLE_ERR
    #define __ENABLE_ASSERT
    #define __ENABLE_WRN
#elif(CEDARLIB_DEBUG_LEVEL == 4)
    #define __ENABLE_ERR
    #define __ENABLE_ASSERT
    #define __ENABLE_WRN
    #define __ENABLE_MSG
    #define __ENABLE_HERE
    #define __ENABLE_INF
#else
//    #error CEDARLIB_DEBUG_LEVEL_GATE is error, please config CEDARLIB_DEBUG_LEVEL_GATE to 0/1/2/3/4!
#endif

#ifndef CEDARLIB_PRINTF //如果库的使用者没有定义CEDARLIB_PRINTF，那么采用cedarlib自己的printf函数
#define CEDARLIB_PRINTF cedarlib_printf
#endif


#ifdef __ENABLE_ASSERT
    #define __cedarlib_ast(condition)    (if(!condition)                                                     \
                                        CEDARLIB_PRINTF("AST:L%d(%s): condition err!", __LINE__, __FILE__)   )
#else
    #define __cedarlib_ast(condition)
#endif

#if defined(__ENABLE_MSG)
	#define __cedarlib_msg(...)    		(CEDARLIB_PRINTF("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     CEDARLIB_PRINTF(__VA_ARGS__)									        )
#else
    #define __cedarlib_msg(...)
#endif

#if defined(__ENABLE_ERR)
    #define __cedarlib_err(...)          (CEDARLIB_PRINTF("ERR:L%d(%s):", __LINE__, __FILE__),                 \
    							    CEDARLIB_PRINTF(__VA_ARGS__),                                          \
    						        cedarlib_reset())
#else
    #define __cedarlib_err(...)    		    cedarlib_reset()
#endif

#if defined(__ENABLE_WRN)
    #define __cedarlib_wrn(...)          (CEDARLIB_PRINTF("WRN:L%d(%s):", __LINE__, __FILE__),                 \
    						     CEDARLIB_PRINTF(__VA_ARGS__)									        )
#else
    #define __cedarlib_wrn(...)
#endif

#if defined(__ENABLE_HERE)
    #define __cedarlib_here__            CEDARLIB_PRINTF("@L%d(%s)\n", __LINE__, __FILE__);

#else
    #define __cedarlib_here__
#endif


#if defined(__ENABLE_INF)
    #define __cedarlib_inf(...)           CEDARLIB_PRINTF(__VA_ARGS__)
#else
    #define __cedarlib_inf(...)
#endif

    #define __cedarlib_log(...)          CEDARLIB_PRINTF(__VA_ARGS__)

#endif  /* _CEDARLIB_DBGS_H_ */

