/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                   DRV KEY
*
*                                    (c) Copyright 2011-2014, Jackie China
*                                             All Rights Reserved
*
* File    : drv_key_i.h
* By      : Jackie
* Version : v1.0
* Date    : 2011-11-28
* Descript: key driver internal head file.
* Update  : date                auther      ver     notes
*           2011-11-28 15:07    Jackie      1.0     Create this file.
*********************************************************************************************************
*/

#ifndef	__DRV_KEY_I_H__
#define	__DRV_KEY_I_H__

#include "epdk.h"

#define LRADC_KEY_DEV_NAME  "KEY"
#define KEY_CODE_TABLE_SIZE   8


typedef struct KEY_DRV
{
    __u32            mid;
    __u32            used;
    __u32            status;
    __input_dev_t    *pKeyc_dev; /* lradc key device handle */    
}__key_drv_t;

typedef struct KEY_DEV
{
    __u32 used;
}__key_dev_t;

typedef struct LRADCK_SCANCODE_INFO
{
    __u32	keyMin; 
    __u32	keyMax;
    __u32	keyCode; //keyMin-keyMax Scancode mapping to keyCode
}__scancode2key_t;

/* KEY Message */
typedef struct KEY_MSG
{
    __u32 key_value;  //key value.
    __u32 flag;       //1:keydown; 0:keyup.		
}__key_msg_t;

extern __s32 DEV_KEY_Ioctrl(__hdle hKey, __u32 cmd, __s32 aux, void *pbuffer);

__s32 DRV_KEY_MInit(void);
__s32 DRV_KEY_MExit(void);
__mp *DRV_KEY_MOpen(__u32 mid, __u32 mod);
__s32 DRV_KEY_MClose(__mp *mp);
__u32 DRV_KEY_MRead(void *pdata, __u32 size, __u32 n, __mp *mp);
__u32 DRV_KEY_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp);
__s32 DRV_KEY_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer);

#endif
