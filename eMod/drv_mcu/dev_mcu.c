/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           ir key driver
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : DEV_KEY.c
* By      : Steven.ZGJ
* Version : V1.00
*********************************************************************************************************
*/

#include "drv_mcu_i.h"

extern    __krnl_event_t  *mcu_lock;
extern    __mcu_drv_t       mcu_drv;
ES_FILE   *fiic;
/*
**********************************************************************************************************************
*                                               FUNCTION
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
static void MCU_OpLock(void)
{
    __u8  err;

    esKRNL_SemPend(mcu_lock, 0, &err);
}
/*
**********************************************************************************************************************
*                                               FUNCTION
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
static void MCU_OpUnlock(void)
{
    esKRNL_SemPost(mcu_lock);
}
/*
****************************************************************************************************
*
*             DEV_MCU_Open
*
*  Description:
*       DEV_MCU_Open
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__hdle DEV_MCU_Open(void * open_arg, __u32 mode)
{
    if(!mcu_drv.mcu_dev.iic_file)
    {
        return (__hdle)0;
    }
    else
    {
        fiic = mcu_drv.mcu_dev.iic_file;

        //eLIBs_fioctrl(fm_drv.fm_dev.power_file, DRV_POWER_CMD_VSTS_SET, POWER_VOL_LDO5, (void *)POWER_VOL_ON);
        //esKRNL_TimeDly(5);
        //fm_mute(DRV_FM_VOICE_OFF);

        return (__hdle)&(mcu_drv.mcu_dev);
    }
}
/*
****************************************************************************************************
*
*             DEV_MCU_Close
*
*  Description:
*       DEV_MCU_Close
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DEV_MCU_Close(__hdle hmcu)
{
    //eLIBs_fioctrl(fm_drv.fm_dev.power_file, DRV_POWER_CMD_VSTS_SET, POWER_VOL_LDO5, (void *)POWER_VOL_OFF);
    //fm_mute(DRV_FM_VOICE_OFF);
    fiic = NULL;

	return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DEV_MCU_Read
*
*  Description:
*       DEV_MCU_Read
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DEV_MCU_Read(void *pdata, __u32 size, __u32 n, __hdle hPower)
{
    return 0;
}
/*
****************************************************************************************************
*
*             DEV_MCU_Write
*
*  Description:
*       DEV_MCU_Write
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DEV_MCU_Write(const void *pdata, __u32 size, __u32 n, __hdle hPower)
{
    return size * n;
}
/*
****************************************************************************************************
*
*             DEV_MCU_Ioctrl
*
*  Description:
*       DEV_MCU_Ioctrl
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DEV_MCU_Ioctrl(__hdle hPower, __u32 cmd, __s32 aux, void *pbuffer)
{
    __s32                ret;
	ret = EPDK_FAIL;

	if(!fiic)
	{
	    return ret;
	}

    MCU_OpLock();

    switch(cmd)
    {
        case DRV_MCU_CMD_CHECK:
        {
			if(mcu_drv.mcu_dev.iic_file != NULL)
			{
				__msg("ET dev_mcu cmd check ---\n");
				ret =mcu_check(mcu_drv.mcu_dev.iic_file);
			}
		 	break;	
		}
      	case DRV_MCU_CMD_CHECK_HXN:
		{
			if(mcu_drv.mcu_dev.iic_file != NULL)
			{
				__msg("HXN dev_mcu cmd check ---\n");
				ret =mcu_check_hxn(mcu_drv.mcu_dev.iic_file);
			}
			break;
		}
       default:
		{
			__wrn("Unkonwn Command...\n");

			break;
		}
	}

	MCU_OpUnlock();

	return ret;
}

__dev_devop_t mcu_dev_ops =
{
    DEV_MCU_Open,
    DEV_MCU_Close,
    DEV_MCU_Read,
    DEV_MCU_Write,
    DEV_MCU_Ioctrl
};
