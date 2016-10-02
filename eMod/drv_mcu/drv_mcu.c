/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           hello world sample
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : drv_fm.c
* By      : xueli
* Version : V1.00
*********************************************************************************************************
*/
#include "drv_mcu_i.h"

__mcu_drv_t       mcu_drv;
__krnl_event_t  *mcu_lock;


/*
****************************************************************************************************
*
*             DRV_MCU_MInit
*
*  Description:
*       DRV_MCU_MInit
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/

__s32 DRV_MCU_MInit(void)
{
    mcu_lock = esKRNL_SemCreate(1);
    mcu_drv.used = 0;

    return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DRV_MCU_MExit
*
*  Description:
*       DRV_MCU_MExit
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DRV_MCU_MExit(void)
{
    __u8 err;

    esKRNL_SemDel(mcu_lock, OS_DEL_ALWAYS, &err);

	return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DRV_MCU_MOpen
*
*  Description:
*       DRV_MCU_MOpen
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__mp *DRV_MCU_MOpen(__u32 mid, __u32 mod)
{
    mcu_drv.mid = mid;

	return (__mp *)&mcu_drv;
}
/*
****************************************************************************************************
*
*             DRV_MCU_MClose
*
*  Description:
*       DRV_MCU_MClose
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DRV_MCU_MClose(__mp *mp)
{
	return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DRV_MCU_MRead
*
*  Description:
*       DRV_FM_MRead
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DRV_MCU_MRead(void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return 0;
}
/*
****************************************************************************************************
*
*             DRV_MCU_MWrite
*
*  Description:
*       DRV_MCU_MWrite
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DRV_MCU_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return 0;
}




/*
****************************************************************************************************
*
*             DRV_MCU_MIoctrl
*
*  Description:
*       DRV_MCU_MIoctrl
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DRV_MCU_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer)
{
    switch(cmd)
    {
		case DRV_CMD_PLUGIN:
		{
		    if(mcu_drv.used)
		    {
                __inf("mcu already used\n");

                return EPDK_FAIL;
		    }
			
			
            mcu_drv.mcu_dev.iic_file = eLIBs_fopen("b:\\BUS\\TWI2", "r");

        	if(!mcu_drv.mcu_dev.iic_file)
        	{
        	 eDbug("----Open TWI Dev File Failed\n");

        		return EPDK_FAIL;
        	}

            mcu_drv.hReg = esDEV_DevReg("USER", "MCU", &mcu_dev_ops, 0);
    		if(!mcu_drv.hReg)
    		{
    			__wrn("user iic   registered Error!\n");
    			eLIBs_fclose(mcu_drv.mcu_dev.iic_file);

    		    return EPDK_FAIL;
    		}

            mcu_drv.used = 1;
           
            __wrn("mcu drv install OK\n");
			
    		return EPDK_OK;
		}

    	case DRV_CMD_PLUGOUT:
    	{
    	    if(mcu_drv.used == 1)
    	    {
	    	    //DRV_FM_SelectAuxAudioChannel();
        		if(mcu_drv.mcu_dev.iic_file)
        		{
        			eLIBs_fclose(mcu_drv.mcu_dev.iic_file);
        		}

        		if(!mcu_drv.hReg)
    			{
    			    __wrn("Dev not plugin!\n");
    			    return EPDK_FAIL;
    	    	}

    		    esDEV_DevUnreg(mcu_drv.hReg);
    		}
			
    		mcu_drv.used = 0;

		    return EPDK_OK;
    	}

		default:
			break;
		    return EPDK_FAIL;
    }
}

