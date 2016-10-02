/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              eBIOS Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : power driver module
* File    : power_script.h
* By      : kevin
* Version : v1.00
* Date    : 2010-6-8 14:07
**********************************************************************************************************************
*/
#ifndef __POWER_SCRIPT_H__
#define __POWER_SCRIPT_H__

#include "ePDK.h"

#define KEY_WORD_MAX_SIZE           (32)
#define SCRIPT_BUF_SIZE             (1024*8)
#define P1CURRENT_SCRIPT_PATH       ("C:\\drv\\power_cfg.ini")

#define TABLE           '\t'
#define SPACE           ' '
#define NEWLINE         '\r'
#define ENTER           '\n'
#define REMARK          ';'

typedef enum __PARS_SCRIPT_RESULT
{
    SCRIPT_PARSE_ERR_NONE   =0,
    SCRIPT_PARSE_ERR_EOF    =1,

} __pars_script_result_t;


typedef struct __P1CURRENT_SCRIPT_FUNC_MAP
{
    char        keystr[KEY_WORD_MAX_SIZE];
    __s32       (*func)(void);

} __p1current_script_func_mapt_t;


typedef struct __P1CURRENT_SCRIPT_PSR
{
    __u8        *buf_start;
    __u32       buf_size;
    __u8        *script_end;
    __u8        *cur_ptr;

    __u8        string[KEY_WORD_MAX_SIZE + 8];

} __p1current_script_psr_t;


#endif /* __POWER_SCRIPT_H__ */

