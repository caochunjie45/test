/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                   DRV KEY
*
*                                    (c) Copyright 2011-2014, Jackie China
*                                             All Rights Reserved
*
* File    : drv_key.c
* By      : Jackie
* Version : v1.0
* Date    : 2011-11-28
* Descript: key driver source file.
* Update  : date                auther      ver     notes
*           2011-11-28 15:07    Jackie      1.0     Create this file.
*********************************************************************************************************
*/

#include "drv_key_i.h"

#define BITS_PER_BYTE		8
#define DIV_ROUND_UP(n,d)   (((n) + (d) - 1) / (d))
#define BITS_PER_LONG       (sizeof(long) * BITS_PER_BYTE)
#define BIT(nr)			    (1UL << (nr))
#define BIT_MASK(nr)		(1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)		((nr) / BITS_PER_LONG)
#define BITS_TO_LONGS(nr)	DIV_ROUND_UP(nr, BITS_PER_LONG)


__key_drv_t    keydrv;
__key_dev_t    keydev;

extern __dev_devop_t keyc_dev_ops;
extern __scancode2key_t scancode2key[];
extern __s32 KEY_Init(void);
extern __s32 KEY_Exit(void);


/**
 * __SetBit - Set a bit in memory
 * @nr: the bit to set
 * @addr: the address to start counting from
 *
 * Unlike set_bit(), this function is non-atomic and may be reordered.
 * If it's called on the same region of memory simultaneously, the effect
 * may be that only one operation succeeds.
 */
static __inline void __SetBit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

	*p  |= mask;
}


/*
*********************************************************************************************************
*                             DRIVER MODULE INIT
*
*Description: key driver module init.
*
*Arguments  : None.
*
*Return     : Always return successful.
*********************************************************************************************************
*/

__s32 DRV_KEY_MInit(void)
{
    keydrv.used = 0;
	__msg("drv_key: Init!\n");
	KEY_Init();
    return EPDK_OK;
}

/*
*********************************************************************************************************
*                             DRIVER MODULE EXIT
*
*Description: key driver module exit.
*
*Arguments  : None.
*
*Return     : Module exit result;
*               == 0    module exit successful;
*               != 0    module exit failed, maybe the module is used by other module.
*********************************************************************************************************
*/

__s32 DRV_KEY_MExit(void)
{
    __msg("drv_key: Exit!\n");
    keydrv.used = 0;
    KEY_Exit();
    return EPDK_OK;
}

/*
*********************************************************************************************************
*                            DRIVER MODULE OPEN
*
*Description: key driver module open.
*
*Arguments  : None.
*
*Return     : Module exit result;
*               == 0    module exit successful;
*               != 0    module exit failed, maybe the module is used by other module.
*********************************************************************************************************
*/

__mp *DRV_KEY_MOpen(__u32 mid, __u32 mod)
{
	__s32 cpu_sr;

    ENTER_CRITICAL(cpu_sr);
    if(keydrv.used)
    {
        __wrn("driver already openned by someone else!\n");
        EXIT_CRITICAL(cpu_sr);
        return (__mp *)0;
    }
    keydrv.used = 1;
    EXIT_CRITICAL(cpu_sr);

    keydrv.mid = mid;
	return (__mp *)&keydrv;
}

/*
*********************************************************************************************************
*                          DRIVER MODULE EXIT
*
*Description: key driver module close.
*
*Arguments  : None.
*
*Return     : Module exit result;
*               == 0    module exit successful;
*               != 0    module exit failed, maybe the module is used by other module.
*********************************************************************************************************
*/

__s32 DRV_KEY_MClose(__mp *mp)
{
	__key_drv_t * drv = (__key_drv_t *)mp;
    if(drv->used == 0)
    {
        return EPDK_FAIL;
    }
    drv->used = 0;

	return EPDK_OK;
}

/*
*********************************************************************************************************
*                           DRIVER MODULE READ
*
*Description: key driver module read, do nothing.
*
*Arguments  :
*
*Return     :
*
*Note       : 
*********************************************************************************************************
*/

__u32 DRV_KEY_MRead(void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return 0;
}

/*
*********************************************************************************************************
*                           DRIVER MODULE WRITE
*
*Description: key driver module write, do nothing.
*
*Arguments  :
*
*Return     :
*
*Note       : 
*********************************************************************************************************
*/

__u32 DRV_KEY_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return 0;
}

/*
*********************************************************************************************************
*                         DRIVER MODULE IO CONTROL
*
*Description: key driver module IO control.
*
*Arguments  : mp        the handle of the key driver module;
*             cmd       the command send to key driver;
*             aux       user define;
*             pbuffer   user define;
*
*Return     :
*********************************************************************************************************
*/

__s32 DRV_KEY_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer)
{
    __s32 cpu_sr = 0;

    switch(cmd)
    {
		case DRV_CMD_PLUGIN:
		{
		    __u32 keyCodeNum = 0;
		    __u32 i = 0;
		    __input_dev_t   *pKeycDev = NULL;

		    ENTER_CRITICAL(cpu_sr);
		    if(keydrv.status)
		    {
		        __wrn("driver cannot support more than one key board!\n");
		        EXIT_CRITICAL(cpu_sr);
		        return EPDK_FAIL;
		    }
		    keydrv.status = 1;
		    EXIT_CRITICAL(cpu_sr);

		    pKeycDev = (__input_dev_t *)eLIBs_malloc(sizeof(__input_dev_t));
		    if(NULL == pKeycDev)
		    {
                __wrn("allocate memory for input device hanle fail!\n");
                return EPDK_FAIL;
		    }
		    eLIBs_memset(pKeycDev, 0, sizeof(__input_dev_t));
		    keydev.used = 0;/* for device manager */

            /* general managment system information */
		    pKeycDev->classname = INPUT_CLASS_NAME;
		    pKeycDev->name      = LRADC_KEY_DEV_NAME;
		    pKeycDev->ops       = &keyc_dev_ops;
		    pKeycDev->parg      = &keydev;

		    pKeycDev->id.product = 0x0001;
		    pKeycDev->id.version = 0x0001;

            /* KEYBOARD DEVICE */
		    pKeycDev->device_class = INPUT_KEYBOARD_CLASS;
            /* set keyboard support event types */
		    pKeycDev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REP);

            /* set keyboard support event codes */
            keyCodeNum = KEY_CODE_TABLE_SIZE;
            for(i = 0; i < keyCodeNum; i++)
            {
                __SetBit(scancode2key[i].keyCode, pKeycDev->keybit);
            }
		    /* set key event handler, handle the feedback events */
		    pKeycDev->event = NULL;

		    /* register keyboard device to INPUT managemant system */
		    if(esINPUT_RegDev(pKeycDev) != EPDK_OK)
		    {
                __wrn("register keyboard device to input system failed\n");
                eLIBs_free(pKeycDev);
	        	return EPDK_FAIL;
		    }
		    /* attach pKeycDev to keydrv */
		    keydrv.pKeyc_dev = pKeycDev;

		    __inf("keyc device initialize succeeded!\n");
		    return EPDK_OK;

		}
		case DRV_CMD_PLUGOUT:
		{
		    /* unregister keyboard deivce from INPUT manage system,
             * free keyboard device handle.
             */
		    if (keydev.used)
            {
                __wrn("keyc is using now\n");
                return EPDK_FAIL;
            }
	        esINPUT_UnregDev(keydrv.pKeyc_dev);
	        eLIBs_free(keydrv.pKeyc_dev);
            return EPDK_OK;

		}
		case DRV_CMD_GET_STATUS:
		{
		    if(keydrv.status)
		    {
		        return DRV_STA_BUSY;
		    }
		    else
		    {
		        return DRV_STA_FREE;
		    }
		}
		default:
			break;
	}
	return EPDK_FAIL;
}
