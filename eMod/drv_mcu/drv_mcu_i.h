/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									          ir keyboard driver
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : drv_irkey_i.h
* By      : jerry
* Version : V1.00
*********************************************************************************************************
*/
#ifndef  _DRV_MCU_I_H_
#define  _DRV_MCU_I_H_

#include "epdk.h"

//一体加密IC
#define   DVR_MCU_ADD          0x66
#define   DRV_MCU_COMMAND1     0x11
#define   DRV_MCU_COMMAND2     0x22

#define   DRV_MCU_COMMAND3     0x33
#define   DRV_MCU_COMMAND4     0x45

#define   DRV_MCU_COMMAND5     0x57
#define   DRV_MCU_COMMAND6     0x63

#define   DRV_MCU_COMMAND7     0x65
#define   DRV_MCU_COMMAND8     0x21

#define   DRV_MCU_DATA1        0x67
#define   DRV_MCU_DATA2        0x71

#define   DRV_MCU_DATA3        0x63
#define   DRV_MCU_DATA4        0x75

#define   DRV_MCU_DATA5        0x25
#define   DRV_MCU_DATA6        0x21

#define   DRV_MCU_DATA7        0x6D
#define   DRV_MCU_DATA8        0x13


#define TWI_SCLK_10KHZ    (10000) //10kHz

//汉欣诺加密IC
#define   DVR_MCU_HXN_ADD          0x58
#define   DRV_MCU_HXN_COMMAND1     0x00
#define   DRV_MCU_HXN_COMMAND2     0x01

#define   DRV_MCU_HXN_COMMAND3     0x02
#define   DRV_MCU_HXN_COMMAND4     0x03

#define   DRV_MCU_HXN_COMMAND5     0x04
#define   DRV_MCU_HXN_COMMAND6     0x05

#define TWI_SCLK_HXN_100KHZ    (100000) //100kHz

typedef struct STRUCT_MCU_DEV
{
    __u32           status;
	ES_FILE        *iic_file;

}__mcu_dev_t;

typedef struct STRUCT_MCU_DRV
{
    __u32           mid;
    __u32           used;
    __hdle			hReg;
    __mcu_dev_t      mcu_dev;
}__mcu_drv_t;

extern  ES_FILE        *fiic;
extern  __dev_devop_t  mcu_dev_ops;

__s32 DRV_MCU_MInit(void);
__s32 DRV_MCU_MExit(void);
__mp* DRV_MCU_MOpen(__u32 mid, __u32 mode);
__s32 DRV_MCU_MClose(__mp *mp);
__u32 DRV_MCU_MRead(void *pdata, __u32 size, __u32 n, __mp *mp);
__u32 DRV_MCU_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp);
__s32 DRV_MCU_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer);

#endif /*_DRV_MCU_I_H_*/


