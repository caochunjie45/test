/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                          SUNV BSP Master Include File
*
*                                    (c) Copyright 2011-2014, Sunny China
*                                             All Rights Reserved
*
* File    : bsp.h
* By      : Sunny
* Version : v1.0
* Date    : 2011-11-18
* Descript: BSP header file for sunv.
* Update  : date                auther      ver     notes
*           2011-11-18 10:10:49 Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __BSP_H__
#define __BSP_H__


#ifndef DATA_TYPE_X_s8
#define DATA_TYPE_X_s8
typedef __s8    s8;
#endif

#ifndef DATA_TYPE_X_u8
#define DATA_TYPE_X_u8
typedef __u8    u8;
#endif

#ifndef DATA_TYPE_X_s16
#define DATA_TYPE_X_s16
typedef __s16   s16;
#endif

#ifndef DATA_TYPE_X_u16
#define DATA_TYPE_X_u16
typedef __u16   u16;
#endif

#ifndef DATA_TYPE_X_s32
#define DATA_TYPE_X_s32
typedef __s32   s32;
#endif

#ifndef DATA_TYPE_X_u32
#define DATA_TYPE_X_u32
typedef __u32   u32;
#endif
#include "sunv.h"


#include "bsp/bsp_lradc.h"

#include "bsp/bsp_nand.h"
#include "bsp/bsp_rtc.h"
#include "bsp/bsp_sdc.h"

#include "bsp/bsp_spi.h"

#include "bsp/bsp_twi.h"
#include "bsp/bsp_usb.h"
#include "bsp/bsp_de.h"
#include "bsp/bsp_uart.h"
#include "bsp/bsp_tp.h"
#include "bsp/bsp_ir.h"
#include "bsp/bsp_adda.h"
#include "bsp/bsp_spi.h"

#include "bsp/bsp_rtc.h"
#include "bsp/bsp_sdc.h"

#include "bsp/bsp_uart.h"
#include "bsp/audio.h"


#define   DATA_TYPE_SINGLE_WORD  (1)
#define   DATA_TYPE_STRING       (2)
#define   DATA_TYPE_MULTI_WORD   (3)
#define   DATA_TYPE_GPIO_WORD    (4)

#define   SCRIPT_PARSER_OK       (0)
#define   SCRIPT_PARSER_EMPTY_BUFFER   	   (-1)
#define   SCRIPT_PARSER_KEYNAME_NULL   	   (-2)
#define   SCRIPT_PARSER_DATA_VALUE_NULL	   (-3)
#define   SCRIPT_PARSER_KEY_NOT_FIND       (-4)
#define   SCRIPT_PARSER_BUFFER_NOT_ENOUGH  (-5)

typedef unsigned long H_PARSER;


typedef struct
{
    char  gpio_name[32];
    int port;
    int port_num;
    int mul_sel;
    int pull;
    int drv_level;
    int data;
}
script_gpio_set_t;


#ifndef EBASE_TRUE
#define EBASE_TRUE      0
#endif
#ifndef EBASE_FALSE
#define EBASE_FALSE     (-1)
#endif
#ifndef EBSP_TRUE
#define EBSP_TRUE       EBASE_TRUE
#endif
#ifndef EBSP_FALSE
#define EBSP_FALSE      EBASE_FALSE
#endif
#define SRAM_TO_AC320   (0)
__s32 CSP_SRAM_set_srama_map(__s32 aux);
__s32 CSP_SRAM_get_srama_map(void);
#define TOUCHPANEL_WAKEUP_SYS_PENDING   (1<<0)
#define EXTNMI_WAKEUP_SYS_PENGDING      (1<<1)
#define USB_WAKEUP_SYS_PENDING          (1<<3)
#define ALARM_WAKEUP_SYS_PENDING        (1<<4)
#define LRADC_WAKEUP_SYS_PENDING        (1<<5)
#define PIO_WAKEUP_SYS_PENDING          (1<<6)
__u32 CSP_CCU_get_wakeup_stat(void);
#endif  //__BSP_H__
