/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                           power key board driver
*
*                                    (c) Copyright 2010-2012, Sunny China
*                                             All Rights Reserved
*
* File    : powerkey_dev.c
* By      : Sunny
* Version : v1.0
* Date    : 2011-1-6 20:30:20
* Descript: 将电源键抽象为一个输入设备注册到MELIS输入子系统.
* Update  : date                auther      ver     notes
*           2011-1-6 20:30:56   Sunny       1.0     电源键输入设备驱动.
*********************************************************************************************************
*/

#include    "powerkey_dev_i.h"

/* 电源键设备注册的设备句柄     */
__input_dev_t *pPowerKeyDev;

/* 电源键支持的设备类型         */
static __u32 PowerKeyDevCodes[] = 
{
	KPAD_POWER,
	KPAD_POWEROFF,
};

/*------------------------------------------------
 *
 *      电源键设备节点操作函数集
 *
 *------------------------------------------------
 */
static __hdle PowerKey_DevOpen(void *openArg, __u32 Mode)
{
    __wrn("you can't open powerkey device\n");
   	return NULL;
}

static int PowerKey_DevClose(__hdle hDev) 
{
    __wrn("you can't close powerkey device\n");
   	return EPDK_FAIL;	
}

static __u32 PowerKey_DevRead(void *pBuffer, __u32 blk, __u32 n, __hdle hDev) 
{
    __wrn("you can't read powerkey device\n");
    return n;
}

static __u32 PowerKey_DevWrite(const void * pBuffer, __u32 blk, __u32 n, __hdle hDev) 
{
    __wrn("you can't write powerkey device\n");
    return 0;
}

static __s32 PowerKey_DevIoctrl(__hdle hDev, __u32 Cmd, __s32 Aux, void *pBuffer) 
{
    __wrn("you can't write powerkey device\n");
 	return EPDK_OK;
}

__dev_devop_t PowerKeyOps = 
{
    PowerKey_DevOpen,
    PowerKey_DevClose,
    PowerKey_DevRead,
    PowerKey_DevWrite,
    PowerKey_DevIoctrl
};

__s32 PowerKeyDevInit(void)
{
    /* 分配电源键输入设备句柄，并向输入子系统注册.*/
    __s32               i;
    __u32               KeyCodeNum;
    pPowerKeyDev = (__input_dev_t *)eLIBs_malloc(sizeof(__input_dev_t));
    if (pPowerKeyDev == NULL)
    {
    	__wrn("not enough memory for INPUT physical device handle\n");
    	return EPDK_FAIL;
    }
    eLIBs_memset(pPowerKeyDev, 0, sizeof(__input_dev_t));
    
    /* 设备电源键设备信息，用于向设备管理系统注册一个管理设备节点 */
    pPowerKeyDev->classname = INPUT_CLASS_NAME;
    pPowerKeyDev->name      = "PowerKey";
    pPowerKeyDev->ops       = &PowerKeyOps;
    pPowerKeyDev->parg      = NULL;
    
    pPowerKeyDev->id.product = 0x0001;
    pPowerKeyDev->id.version = 0x0001;
    
    /* 设置电源键设备类型，属于按键类设备 */
    pPowerKeyDev->device_class = INPUT_KEYBOARD_CLASS;
    
    /* 设置电源键设备支持的事件类型，只支持按键事件,
     * 注意电源键设备并不支持重复键。
     */
    pPowerKeyDev->evbit[0] = BIT_MASK(EV_KEY);
    
    /* 设置电源键设备支持的按键事件 */
    KeyCodeNum = sizeof(PowerKeyDevCodes) / sizeof(PowerKeyDevCodes[0]);
    for (i = 0; i < KeyCodeNum; i++)
    {
    	__set_bit(PowerKeyDevCodes[i], pPowerKeyDev->keybit);
    }
    
    /* 设备电源键设备的事件反馈回掉函数为NULL,不支持事件的回馈 */
    pPowerKeyDev->event = NULL;
    
    /* 向输入子系统注册电源键输入设备 */
    if (esINPUT_RegDev(pPowerKeyDev) != EPDK_OK)
    {
        __wrn("register powerkey device to INPUT system failed\n");
        eLIBs_free(pPowerKeyDev);
    	return EPDK_FAIL;
    }
    
    __inf("PowerKey device initialize succeeded\n");
    return EPDK_OK;
}

void PowerKeyDevExit(void)
{
    if (pPowerKeyDev)
    {
        /* 注销设备并释放内存 */
        esINPUT_UnregDev(pPowerKeyDev);
        eLIBs_free(pPowerKeyDev);
    }
}


__s32 PowerKeyDevSendEvent(__u32 KeyCode, __bool IsDown)
{
    if (IsDown)
    {
        /* 发送按键按下事件 */
        esINPUT_SendEvent(pPowerKeyDev,
	                      EV_KEY,
	                      KeyCode,
	                      KEY_DOWN_ACTION
	                      );
        /* 发送同步事件     */
        esINPUT_SendEvent(pPowerKeyDev,
	                      EV_SYN,
	                      SYN_REPORT,
	                      0
	                      );
    }
    else
    {
        /* 发送按键抬起事件 */
        esINPUT_SendEvent(pPowerKeyDev,
	                      EV_KEY,
	                      KeyCode,
	                      KEY_UP_ACTION
	                      );
        /* 发送同步事件     */
        esINPUT_SendEvent(pPowerKeyDev,
	                      EV_SYN,
	                      SYN_REPORT,
	                      0
	                      );
    }
    return EPDK_OK;
}
