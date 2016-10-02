/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                               PIN Controller
*
*                                    (c) Copyright 2011-2014, Sunny China
*                                             All Rights Reserved
*
* File    : csp_pin.h
* By      : Sunny
* Version : v1.0
* Date    : 2011-11-21
* Descript: pin controller interface header file.
* Update  : date                auther      ver     notes
*           2011-11-21 13:10:54 Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef	__CSP_PIN_H__
#define	__CSP_PIN_H__

#define PMU_PIN_PORT    0xffff


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
user_gpio_set_t;

//------------------------------------------------------------------------------
//  pin pull status
//------------------------------------------------------------------------------
typedef enum __PIN_PULL
{
	PIN_PULL_DISABLE 	=	0x00,
	PIN_PULL_UP			=	0x01,
	PIN_PULL_DOWN		=	0x02,
	PIN_PULL_RESERVED	=	0x03,
	PIN_PULL_DEFAULT 	= 	0xFF
} __pin_pull_e;


//------------------------------------------------------------------------------
//  pin multi driver leveles
//------------------------------------------------------------------------------
typedef	enum __PIN_MULTI_DRIVING
{
	PIN_MULTI_DRIVING_0			=	0x00,
	PIN_MULTI_DRIVING_1			=	0x01,
	PIN_MULTI_DRIVING_2			=	0x02,
	PIN_MULTI_DRIVING_3			=	0x03,
	PIN_MULTI_DRIVING_DEFAULT	=	0xFF
} __pin_multi_driving_e;


//------------------------------------------------------------------------------
//  pin data bit status
//------------------------------------------------------------------------------
typedef enum __PIN_DATA_STATUS
{
    PIN_DATA_LOW     = 0x00,
    PIN_DATA_HIGH    = 0x01,
    PIN_DATA_DEFAULT = 0XFF
} __pin_data_status_e;


//------------------------------------------------------------------------------
//  pin int trigger mode
//------------------------------------------------------------------------------
typedef enum __PIN_INT_MODE
{
    PIN_INT_POSITIVE_EDGE   =   0x0,
    PIN_INT_NEGATIVE_EDGE   =   0x1,
    PIN_INT_HIGN_LEVEL      =   0x2,
    PIN_INT_LOW_LEVEL       =   0x3,
    PIN_INT_DOUBLE_EDGE     =   0x4
} __pin_int_mode_e;


//------------------------------------------------------------------------------
//  the source clock of pin int
//------------------------------------------------------------------------------
typedef enum __PIN_INT_SOURCE_CLK
{
    PIN_INT_SRC_CLK_32K = 0x0,
    PIN_INT_SRC_CLK_24M = 0x1
} __pin_int_source_clk_e;


//------------------------------------------------------------------------------
//  pin multi select functions
//------------------------------------------------------------------------------
typedef enum __PIN_MULTI_SELECT
{
    PIN_MULTI_SEL_INPUT  = 0x0,
    PIN_MULTI_SEL_OUTPUT = 0x1,
} __pin_multi_select_t;

//------------------------------------------------------------------------------
//  user pin setting information
//------------------------------------------------------------------------------
#define PIN_NAME_LEN    32

typedef struct __USER_PIN_SET
{
    char    PinName[PIN_NAME_LEN];
    __s32   Port;
    __s32   PortNum;
    __s32   MultiSel;
    __s32   Pull;
    __s32   DrvLevel;
    __s32   Data;
} __user_pin_set_t;

//------------------------------------------------------------------------------
//  PIN module interfaces
//------------------------------------------------------------------------------
__s32   CSP_PIN_Init(void);
__s32   CSP_PIN_Exit(void);
__hdle  CSP_PIN_Request(__user_pin_set_t *pGrpStat, __u32 GrpSize);
__s32   CSP_PIN_Release(__hdle hPin, __bool bRestore);
__s32   CSP_PIN_GetPinGrpStatus(__hdle hPin, __user_pin_set_t *pGrpStat, __u32 GrpSize, __bool bFromHW);
__s32   CSP_PIN_GetPinStatus(__hdle hPin, __user_pin_set_t *pPinStat, const char *pPinName, __bool bFromHW);
__s32   CSP_PIN_SetPinStatus(__hdle hPin, __user_pin_set_t *pPinStat, const char *pPinName, __bool bSetUserStat);
__s32   CSP_PIN_SetPinIO(__hdle hPin, __bool bOut, const char *pPinName);
__s32   CSP_PIN_SetPinPull(__hdle hPin, __u32 PullStat, const char *pPinName);
__s32   CSP_PIN_SetPinDriver(__hdle hPin, __u32 DrvLevel, const char *pPinName);
__s32   CSP_PIN_ReadPinData(__hdle hPin, const char *pPinName);
__s32   CSP_PIN_WritePinData(__hdle hPin, __u32 Data, const char *pPinName);

__s32   CSP_PIN_GetIrqNum(void);
__s32   CSP_PIN_SetIntMode(__hdle hPin, __u32 IntMode);
__bool  CSP_PIN_QueryInt(__hdle hPin);
__s32   CSP_PIN_ClearPending(__hdle hPin);
__s32   CSP_PIN_EnableInt(__hdle hPin);
__s32   CSP_PIN_DisableInt(__hdle hPin);
__s32   CSP_PIN_SetIntClock(__u32 Clock);
__s32   CSP_PIN_SetIntDebounce(__u32 n);

#endif	//__CSP_PIN_H__

