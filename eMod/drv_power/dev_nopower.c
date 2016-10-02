/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                               ir key driver
*
*                                (c) Copyright 2006-2007, Steven.ZGJ China
*                                            All    Rights Reserved
*
* File    : DEV_Power.c
* By      : Steven.ZGJ
* Version : V1.00
*********************************************************************************************************
*/

#include "drv_power_i.h"


extern __power_drv_t    powerdrv;

static __u8     dc2set = 1, ldo1set = 1;
static __u16    Vldo1_set = 1250;
static __u16    Vldo2_set = 3000;
static __u16    Vldo3_set = 1800;
static __u16    Vldo4_set = 3300;
static __u16    Vldo5_set = 3300;
static __u16    Vdcdc1_set = 3300;
static __u16    Vdcdc2_set = 1240;
static __u16    Vdcdc3_set = 2500;

/*
****************************************************************************************************
*
*             DEV_KEY_Open
*
*  Description:
*       DRV_MOpen
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__hdle DEV_NoPower_Open(void * open_arg, __u32 mode)
{
    return (__hdle)&(powerdrv.power_dev);
}


/*
****************************************************************************************************
*
*             DEV_KEY_Close
*
*  Description:
*       DRV_irkey_MClose
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DEV_NoPower_Close(__hdle hPower)
{
    return EPDK_OK;
}


/*
****************************************************************************************************
*
*             DEV_KEY_Read
*
*  Description:
*       DRV_irkey_MRead
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DEV_NoPower_Read(void *pdata, __u32 size, __u32 n, __hdle hPower)
{
    return 0;
}


/*
****************************************************************************************************
*
*             DEV_KEY_Write
*
*  Description:
*       DRV_irkey_MWrite
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DEV_NoPower_Write(const void *pdata, __u32 size, __u32 n, __hdle hPower)
{
    return size * n;
}


/*
****************************************************************************************************
*
*             DEV_KEY_Ioctrl
*
*  Description:
*       DRV_irkey_MIoctrl
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DEV_NoPower_Ioctrl(__hdle hPower, __u32 cmd, __s32 aux, void *pbuffer)
{
    __twi_dev_para_ex_t     hstr;
    __s32                ret;

    ret = EPDK_OK;
    eLIBs_memset(&hstr, 0, sizeof(__twi_dev_para_ex_t));

    P1_OpLock();

    switch(cmd)
    {
        case DRV_POWER_CMD_READ_REG:
        {
            __inf("drvpower DRV_POWER_CMD_READ_REG start\n");
            ret = 0xff;

            break;
        }

        case DRV_POWER_CMD_WRITE_REG:
        {
            __inf("drvpower DRV_POWER_CMD_WRITE_REG start\n");

            break;
        }

        case DRV_POWER_CMD_POWER_ON:
        {
            break;
        }

        case DRV_POWER_CMD_POWER_OFF:
        {
            __inf("drvpower DRV_POWER_CMD_POWER_OFF start\n");

            break;
        }

        case DRV_POWER_CMD_INT_ENABLE:
        {
            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_INT_DISABLE:
        {
            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_INT_QUERY:
        {
            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_INT_CLEAR:
        {
            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_VOUT_GET:
        {
            __inf("drvpower DRV_POWER_CMD_VOUT_GET start\n");

            if(aux == POWER_VOL_LDO1)
            {
                if(ldo1set)
                {
                    ret = Vldo1_set;
                }
                else
                {
                    ret = Vldo1_set;
                }
            }
            else if((aux == POWER_VOL_DCDC1) || (aux == POWER_VOL_DCDC2))
            {
                if(aux == POWER_VOL_DCDC1)
                {
                    ret = Vdcdc1_set;
                }
                else
                {
                    if(dc2set)
                    {
                        __inf("drvpower DRV_POWER_CMD_VOUT_GET dc2set\n");
                        ret =  Vdcdc2_set;
                    }
                    else
                    {
                        ret = Vdcdc2_set;
                    }
                }
            }
            else if((aux == POWER_VOL_LDO2) || (aux == POWER_VOL_LDO3) || (aux == POWER_VOL_DCDC3))
            {
                if(aux == POWER_VOL_LDO2)
                {
                    ret = Vldo2_set;
                }
                else if(aux == POWER_VOL_DCDC3)
                {
                    ret = Vdcdc3_set;
                }
                else
                {
                    ret = Vldo3_set ;
                }
            }
            else if(aux == POWER_VOL_LDO4)
            {
                if(aux == POWER_VOL_LDO4)
                {
                    ret = Vldo4_set ;
                }
                else
                {
                    ret = Vldo5_set ;
                }
            }
            __inf("drvpower DRV_POWER_CMD_VOUT_GET dc2 return %d\n",ret);

            break;
        }

        case DRV_POWER_CMD_VOUT_SET:
        {
            __u32       tmp;

            tmp = (__u32)pbuffer;
            if((aux == POWER_VOL_DCDC1) || (aux == POWER_VOL_DCDC2))
            {
                if(aux == POWER_VOL_DCDC1)
                {
                    Vdcdc1_set = tmp;
                }
                else
                {
                    Vdcdc2_set = tmp;
                }
            }
            else if(aux == POWER_VOL_LDO1)
            {
                __wrn("LDO1 Voltage  cant be set!\n");
                Vldo1_set = tmp;
            }
            else if((aux == POWER_VOL_LDO2) || (aux == POWER_VOL_LDO3) || (aux ==POWER_VOL_DCDC3))
            {
                if(aux == POWER_VOL_DCDC3)
                {
                    Vdcdc3_set = tmp;
                }
                else if(aux == POWER_VOL_LDO2)
                {
                    Vldo2_set = tmp;
                }
                else
                {
                    Vldo3_set = tmp;
                }
            }
            else if(aux == POWER_VOL_LDO4)
            {
                if(aux == POWER_VOL_LDO4)
                {
                    Vldo4_set = tmp;
                }
                else
                {
                    Vldo5_set = tmp;
                }
            }
            ret = EPDK_OK;

            break;
        }
        case DRV_POWER_CMD_VSTS_GET:
        {
            break;
        }

        case DRV_POWER_CMD_VSTS_SET:
        {
            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_PWREPT_GET:
        {
            break;
        }
        case DRV_POWER_CMD_PWREPT_SET:
        {
            break;
        }

        case DRV_POWER_CMD_BATADC_GET:
        {
            __drv_power_battery_status_t1   *bat_sts = (__drv_power_battery_status_t1 *)pbuffer;

            bat_sts->bat_vol = 2500;
            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_BATADC_SET:
        {
            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_GET_BAT_RDC:
        {
            ret = 500;
            break;
        }

        case DRV_POWER_CMD_BATSTATUS_TABLE_GET:
        {
            __drv_power_battery_status_t2  *bat_sts = (__drv_power_battery_status_t2 *)pbuffer;

            bat_sts->battery_exist = 0x0;
            bat_sts->dcin_valid = 0x01;
            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_DCINADC_GET:
        {
            __drv_power_dcin_status_t   *dcin_sts = (__drv_power_dcin_status_t *)pbuffer;

            dcin_sts->dcin_exist = 0x01;
            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_DCINADC_SET:
        {
            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_CHARGE_GET:
        {
            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_CHARGE_SET:
        {
            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_BASIC_STATUS:
        {
            __drv_power_basic_status_t  *pbasic = (__drv_power_basic_status_t *)pbuffer;

            pbasic->bat_exist   = 0;
            pbasic->bat_charge_status =0;
            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_IPS_GET:
        {
            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_IPS_SET:
        {
            ret = EPDK_OK;
            break;
        }
        case DRV_POWER_CMD_GET_DCIN:
        {
            __drv_power_battery_status_t3   *pstatus = (__drv_power_battery_status_t3 *)pbuffer;

            pstatus->charge_status = 0x0;
            ret = EPDK_OK;
            break;
        }
        case DRV_POWER_CMD_GET_FUELGUAGE:
        {
            __drv_power_fuelguage_t   *pbat = (__drv_power_fuelguage_t *)pbuffer;

            pbat->charge_status = 0x0;
            pbat->rest_vol      = 0x0;
            pbat->rest_time     = 0x0;
            ret = EPDK_OK;
            break;
        }
        case DRV_POWER_CMD_SET_USB_CHARGE:
        {
            ret = EPDK_OK;
            break;
        }
        case DRV_POWER_CMD_SET_STANDBY_FLAG:
        {
            __inf("set sdandby to  1 \n");
            break;
        }
        default:
        {
            __wrn("Unkonwn Command...\n");
            ret = EPDK_FAIL;
            break;
        }
    }

    P1_OpUnlock();

    return ret;
}

__dev_devop_t nopower_dev_ops =
{
    DEV_NoPower_Open,
    DEV_NoPower_Close,
    DEV_NoPower_Read,
    DEV_NoPower_Write,
    DEV_NoPower_Ioctrl
};

