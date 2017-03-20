/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              eDBG Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : cedarlib �Ĵ�ӡ����
* File    : cedarlib_dbgs.h
*
* By      : eric_wang
* Version : v1.0
* Date    : 2010-11-1
* Description:
    ר��Ϊcedarlib����һ�״�ӡ����
    ʵ��ֻ��7����ӡ��:
    __cedarlib_ast   :level1,�кţ��ļ���
    __cedarlib_msg   :level4,���ݣ��кţ��ļ�������ӡ
    __cedarlib_err   :level2,���ݣ��кţ��ļ�������reset
    __cedarlib_wrn   :level3,���ݣ��кţ��ļ�������ӡ
    __cedarlib_here__:level4,ֻ��ӡ�кţ��ļ���
    __cedarlib_inf :  level4, û���кš��ļ���
    __cedarlib_log : ��������ӡ��û���кš��ļ���
    [cedarlib]�ڲ�����
**********************************************************************************************************************
*/
#ifndef _CEDARLIB_DBGS_H_
#define _CEDARLIB_DBGS_H_

//cedarlib_ref.h�п��ܻᶨ��CEDARLIB_DEBUG_LEVEL_GATE�ĵȼ�,��������壬����release������İ汾������Ĭ�ϵȼ�2������
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

#ifndef CEDARLIB_PRINTF //������ʹ����û�ж���CEDARLIB_PRINTF����ô����cedarlib�Լ���printf����
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

