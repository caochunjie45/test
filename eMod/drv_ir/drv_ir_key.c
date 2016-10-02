/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                  DRV IR KEY
*
*                                    (c) Copyright 2011-2014, Jackie China
*                                             All Rights Reserved
*
* File    : ir_key_drv.c
* By      : Jackie
* Version : v1.0
* Date    : 2012-01-09
* Descript: ir key driver source file.
* Update  : date                auther      ver     notes
*           2012-01-09 15:07    Jackie      1.0     Create this file.
*********************************************************************************************************
*/

#include "drv_ir_key_i.h"
#include "ir_key_proc.h"

#define IR_KEY_DEV_NAME     "IR_KEY"

#define BITS_PER_BYTE		8
#define DIV_ROUND_UP(n,d)   (((n) + (d) - 1) / (d))
#define BITS_PER_LONG       (sizeof(long) * BITS_PER_BYTE)
#define BIT(nr)			    (1UL << (nr))
#define BIT_MASK(nr)		(1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)		((nr) / BITS_PER_LONG)
#define BITS_TO_LONGS(nr)	DIV_ROUND_UP(nr, BITS_PER_LONG)

/**
 * __set_bit - Set a bit in memory
 * @nr: the bit to set
 * @addr: the address to start counting from
 *
 * Unlike set_bit(), this function is non-atomic and may be reordered.
 * If it's called on the same region of memory simultaneously, the effect
 * may be that only one operation succeeds.
 */
static __inline void __set_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

	*p  |= mask;
}

__ir_key_drv_t ir_key_drv;
extern __dev_devop_t ir_key_dev_ops;

__s32 ir_key_drv_event(__input_dev_t *dev, __u32 type, __u32 code, __s32 value)
{
    __msg("ir keyborad feedback event [type:%d, code:%d, value:%d]\n",
                type, code, value);
 	return EPDK_OK;
}

/*
*********************************************************************************************************
*                           DRV IR KEY MODULE INIT
*
* Description: DRV IR KEY module init.
*
* Parameters : NONE.               
*
* Returns    : EPDK_OK if success,  EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/

__s32 DRV_IR_KEY_MInit(void)
{
    __msg("IR KEY MInit\n");

	eLIBs_memset(&ir_key_drv, 0, sizeof(__ir_key_drv_t));
	__here__
    return IR_KEY_Init();
}

/*
*********************************************************************************************************
*                           DRV IR KEY MODULE EXIT
*
* Description: DRV IR KEY module exit.
*
* Parameters : NONE.               
*
* Returns    : EPDK_OK if success,  EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/
__s32 DRV_IR_KEY_MExit(void)
{
    __msg("IR KEY MExit\n");

    eLIBs_memset(&ir_key_drv, 0, sizeof(__ir_key_drv_t));

    return IR_KEY_Exit();
}

/*
*********************************************************************************************************
*                           DRV IR KEY MODULE OPEN
*
* Description: DRV IR KEY module open.
*
* Parameters : NONE.               
*
* Returns    : EPDK_OK if success,  EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/
__mp *DRV_IR_KEY_MOpen(__u32 mid, __u32 mod)
{
    __s32 cpu_sr;

    __msg("IR KEY MOpen = %d\n", mid);

    ENTER_CRITICAL(cpu_sr);
    if(ir_key_drv.used)
    {
        __wrn("ir key driver already openned by someone else!\n");
        EXIT_CRITICAL(cpu_sr);
        return (__mp *)0;
    }
    ir_key_drv.used = 1;
    EXIT_CRITICAL(cpu_sr);

    ir_key_drv.mid = mid;
    return (__mp *)&ir_key_drv;
}

/*
*********************************************************************************************************
*                           DRV IR KEY MODULE CLOSE
*
* Description: DRV IR KEY module close.
*
* Parameters : NONE.               
*
* Returns    : EPDK_OK if success,  EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/
__s32 DRV_IR_KEY_MClose(__mp *mp)
{
    __ir_key_drv_t * drv = (__ir_key_drv_t *)mp;

    __msg("IR KEY MClose\n");

    if(drv->used == 0)
    {
        return EPDK_FAIL;
    }
    drv->used = 0;

	return EPDK_OK;
}

/*
*********************************************************************************************************
*                           DRV IR KEY MODULE READ
*
* Description: DRV IR KEY module read.
*
* Parameters : NONE.               
*
* Returns    : EPDK_OK if success,  EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/
__u32 DRV_IR_KEY_MRead(void *pdata, __u32 size, __u32 n, __mp *mp)
{
    __msg("do nothing!\n");
    return 0;
}

/*
*********************************************************************************************************
*                           DRV IR KEY MODULE WRITE
*
* Description: DRV IR KEY module write.
*
* Parameters : NONE.               
*
* Returns    : EPDK_OK if success,  EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/
__u32 DRV_IR_KEY_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp)
{
    __msg("do nothing!\n");
    return 0;
}

/*
*********************************************************************************************************
*                           DRV IR KEY MODULE IOCTRL
*
* Description: DRV IR KEY module ioctrl.
*
* Parameters : NONE.               
*
* Returns    : EPDK_OK if success,  EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/
__s32 DRV_IR_KEY_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer)
{
     __s32 cpu_sr = 0;

    switch(cmd)
    {
		case DRV_CMD_PLUGIN:
		{
		    __u32 i = 0;
		    __input_dev_t   *pIrkey_Dev = NULL;

		    ENTER_CRITICAL(cpu_sr);
		    if(ir_key_drv.status)
		    {
		        __wrn("Driver cannot support more than one ir key board!\n");
		        EXIT_CRITICAL(cpu_sr);
		        return EPDK_FAIL;
		    }
		    ir_key_drv.status = 1;
		    EXIT_CRITICAL(cpu_sr);

		    pIrkey_Dev = (__input_dev_t *)eLIBs_malloc(sizeof(__input_dev_t));
		    if(NULL == pIrkey_Dev)
		    {
                __wrn("allocate memory for input device hanle fail!\n");
                return EPDK_FAIL;
		    }
		    eLIBs_memset(pIrkey_Dev, 0, sizeof(__input_dev_t));
		    ir_key_drv.ir_key_dev.used = 0;/* for device manager */

            /* general managment system information */
		    pIrkey_Dev->classname = INPUT_CLASS_NAME;
		    pIrkey_Dev->name      = IR_KEY_DEV_NAME;
		    pIrkey_Dev->ops       = &ir_key_dev_ops;
		    pIrkey_Dev->parg      = &(ir_key_drv.ir_key_dev);

		    pIrkey_Dev->id.product = 0x0001;
		    pIrkey_Dev->id.version = 0x0001;

            /* KEYBOARD DEVICE */
		    pIrkey_Dev->device_class = INPUT_KEYBOARD_CLASS;
            /* set keyboard support event types */
		    pIrkey_Dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REP);

            /* set keyboard support event codes */
            for(i = 0; i < IR_KEY_CODE_TABLE_SIZE; i++)
            {
                __set_bit(ir_key_map[i], pIrkey_Dev->keybit);
            }
		    /* set key event handler, handle the feedback events */
		    pIrkey_Dev->event = ir_key_drv_event;

		    /* register keyboard device to INPUT managemant system */
		    if(esINPUT_RegDev(pIrkey_Dev) != EPDK_OK)
		    {
                __wrn("register ir keyboard device to input system failed\n");
                eLIBs_free(pIrkey_Dev);
	        	return EPDK_FAIL;
		    }
		    /* attach pIR_KEYDev to keydrv */
		    ir_key_drv.input_dev = pIrkey_Dev;

		    __inf("IR_KEY device initialize succeeded!\n");
		    return EPDK_OK;

		}
		case DRV_CMD_PLUGOUT:
		{
		    /* unregister keyboard deivce from INPUT manage system,
             * free keyboard device handle.
             */
		    if (ir_key_drv.used)
            {
                __wrn("IR_KEY is using now\n");
                return EPDK_FAIL;
            }
	        esINPUT_UnregDev(ir_key_drv.input_dev);
	        eLIBs_free(ir_key_drv.input_dev);
            return EPDK_OK;

		}
		case DRV_CMD_GET_STATUS:
		{
		    if(ir_key_drv.status)
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
