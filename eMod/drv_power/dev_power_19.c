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

#define ABS(x)                  ( (x) >0 ? (x) : -(x) )
#define Max_Bat_Cur             15
#define Discharge_Rdc_Cal       53

extern __power_drv_t            powerdrv;
extern __drv_power_fuelguage_t  fuelguage;
extern __sys_p1current_cfg_t    *pP1CurrentCfg;
extern __s32        this_rdc;
extern __u8         coulomb_flag;   //在GET_COULOMBCOUNTER命令里表示正负
extern __u8         standby;
extern __u8         PMU_type;
__u8        AXP199_Version = 0;
__u16       Vout_dc2_target = 0;

static __u8     usb_charge_exist = 0;
static __u32    _reg2test(__u32 value,__u8 version)
{
    __u32 ret;

    switch(value)
    {
        case 0:
        {
            if(version)
            {
                ret = 100;
            }
            else
            {
                ret = 100;
            }
            break;
        }
        case 1:
        {
            if(version)
            {
                ret = 190;
            }
            else
            {
                ret = 190;
            }
            break;
        }
        case 2:
        {
            if(version)
                ret = 280;
            else
                ret = 280;
            break;
        }
        case 3:
        {
            if(version)
                ret = 360;
            else
                ret = 360;
            break;
        }
        case 4:
        {
            if(version)
                ret = 450;
            else
                ret = 450;
            break;
        }
        case 5:
        {
            if(version)
                ret = 550;
            else
                ret = 550;
            break;
        }
        case 6:
        {
            if(version)
                ret = 630;
            else
                ret = 630;
            break;
        }
        case 7:
        {
            if(version)
                ret = 700;
            else
                ret = 700;
            break;
        }
        case 8:
        {
            if(version)
                ret = 780;
            else
                ret = 780;
            break;
        }
        case 9:
        {
            if(version)
                ret = 880;
            else
                ret = 880;
            break;
        }
        case 10:
        {
            if(version)
                ret = 960;
            else
                ret = 960;
            break;
        }
        case 11:
        {
            if(version)
                ret = 1000;
            else
                ret = 1000;
            break;
        }
        case 12:
        {
            if(version)
                ret = 1080;
            else
                ret = 1080;
            break;
        }
        case 13:
        {
            if(version)
                ret = 1160;
            else
                ret = 1160;
            break;
        }
        case 14:
        {
            if(version)
                ret = 1240;
            else
                ret = 1240;
            break;
        }
        case 15:
        {
            if(version)
                ret = 1320;
            else
                ret = 1320;
            break;
        }
        default:
        {
            ret = 1;
            break;
        }
    }
    return ret;
}

//******************************************************************************
//通过脚本解析出来的电流值估算电池供电时间
//******************************************************************************

void rest_time_cal_19 (__u8 lcd_level)
{
//    InitP1currentCfgPar();
    fuelguage.rest_time_music = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->musiccurrent;
    fuelguage.rest_time_standby = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 /  pP1CurrentCfg->standbycurrent;

    switch(lcd_level)
    {
        case 1:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level1;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level1;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level1;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 2:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level2;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level2;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level2;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 3:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level3;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level3;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level3;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 4:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level4;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level4;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level4;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 5:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level5;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level5;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level5;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 6:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level6;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level6;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level6;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 7:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level7;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level7;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level7;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 8:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level8;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level8;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level8;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 9:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level9;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level9;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level9;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 10:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level10;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level10;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level10;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 11:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level11;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level11;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level1;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 12:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level12;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level12;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level12;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 13:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level13;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level13;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level13;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 14:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level14;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level14;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level14;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 15:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level15;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level15;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level15;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 16:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level16;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level16;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level16;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 17:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level17;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level17;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level17;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 18:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level18;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level18;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level18;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 19:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level19;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level19;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level19;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 20:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level20;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level20;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level20;
            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        default:
        {
            break;
        }
    }

}


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
__hdle DEV_Power_19_Open(void * open_arg, __u32 mode)
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
__s32 DEV_Power_19_Close(__hdle hPower)
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
__u32 DEV_Power_19_Read(void *pdata, __u32 size, __u32 n, __hdle hPower)
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
__u32 DEV_Power_19_Write(const void *pdata, __u32 size, __u32 n, __hdle hPower)
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
__s32 DEV_Power_19_Ioctrl(__hdle hPower, __u32 cmd, __s32 aux, void *pbuffer)
{
    __powerdev_t        *hdev     = (__powerdev_t *)hPower;
    __twi_dev_para_ex_t     hstr;
    __u8                 tmp_value;
    __s32                ret;
    __u8        byte_addr = 0;

    ret = EPDK_FAIL;
    eLIBs_memset(&hstr, 0, sizeof(__twi_dev_para_ex_t));

    P1_OpLock();

    switch(cmd)
    {
        case DRV_POWER_CMD_READ_REG:
        {
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = aux;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte

            if(EPDK_OK == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                ret = tmp_value;
            }

            break;
        }

        case DRV_POWER_CMD_WRITE_REG:
        {
            tmp_value = (__u32)pbuffer;
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = aux;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte

            ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

            break;
        }

        case DRV_POWER_CMD_POWER_ON:
        {
            break;
        }

        case DRV_POWER_CMD_POWER_OFF:
        {
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER19_OFF_CTL;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            tmp_value |= 1<<7;
            ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

            break;
        }

        case DRV_POWER_CMD_INT_ENABLE:
        {
            __u8  temp;

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte

            temp = (aux >> 0) & 0xff;
            if(temp)
            {
                hstr.byte_count = 1;
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_INTEN1;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                tmp_value |= temp;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                {
                    break;
                }
            }

            temp = (aux >> 7) & 0xff;
            if(temp)
            {
                hstr.byte_count = 1;
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_INTEN2;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                tmp_value |= temp;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                {
                    break;
                }
            }

            temp = (aux >> 15) & 0xff;
            if(temp)
            {
                hstr.byte_count = 1;
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_INTEN3;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                tmp_value |= temp;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                {
                    break;
                }
            }

            temp = (aux >> 23) & 0xff;
            if(temp)
            {
                hstr.byte_count = 1;
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_INTEN4;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                tmp_value |= temp;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                {
                    break;
                }
            }

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_INT_DISABLE:
        {
            __u8  temp;

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte

            temp = (aux >> 0) & 0xff;
            if(temp)
            {
                hstr.byte_count = 1;
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_INTEN1;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                tmp_value  &= ~temp;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                {
                    break;
                }
            }

            temp = (aux >> 7) & 0xff;
            if(temp)
            {
                hstr.byte_count = 1;
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_INTEN2;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                tmp_value  &= ~temp;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                {
                    break;
                }
            }

            temp = (aux >> 15) & 0xff;
            if(temp)
            {
                hstr.byte_count = 1;
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_INTEN3;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                tmp_value  &= ~temp;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                {
                    break;
                }
            }
            temp = (aux >> 23) & 0xff;
            if(temp)
            {
                hstr.byte_count = 1;
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_INTEN4;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                tmp_value  &= ~temp;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                {
                    break;
                }
            }
            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_INT_QUERY:
        {
            __u8 *data = (__u8 *)pbuffer;

            __inf("p1 int query!\n");

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER19_INTEN1;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                __inf("p1 POWER19_INTEN1 error!\n");
                break;
            }
            __inf("p1 POWER19_INTEN1 %d!\n",tmp_value);

            byte_addr  = POWER19_INTEN2;
            hstr.byte_addr  = &byte_addr;
            hstr.data       =  &tmp_value;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                __inf("p1 POWER19_INTEN2 error!\n");
                break;
            }
            __inf("p1 POWER19_INTEN2 %d!\n",tmp_value);

            byte_addr  = POWER19_INTEN3;
            hstr.byte_addr  = &byte_addr;
            hstr.data       =  &tmp_value;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                __inf("p1 POWER19_INTEN31 error!\n");
                break;
            }
            __inf("p1 POWER19_INTEN3 %d!\n",tmp_value);

            byte_addr  = POWER19_INTEN4;
            hstr.byte_addr  = &byte_addr;
            hstr.data       =  &tmp_value;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                __inf("p1 POWER19_INTEN4 error!\n");
                break;
            }
            __inf("p1 POWER19_INTEN4 %d!\n",tmp_value);

            byte_addr  = POWER19_INTSTS1;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = data + 0;
            __inf("p1 int query10!\n");
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                __inf("p1 int query12!\n");
                break;
            }
            __inf("p1 int query1!\n");

            byte_addr  = POWER19_INTSTS2;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = data + 1;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            __inf("p1 int query2!\n");
            byte_addr  = POWER19_INTSTS3;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = data + 2;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            __inf("p1 int query3!\n");
            byte_addr  = POWER19_INTSTS4;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = data + 3;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            __inf("p1 int query4!\n");
            ret = EPDK_OK;
            break;
       }

        case DRV_POWER_CMD_INT_CLEAR:
        {
            tmp_value = *((__u8 *)pbuffer + 0);

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER19_INTSTS1;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte

            __inf("p1 int clear intsts1 = %d!\n",tmp_value);
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }

            tmp_value = *((__u8 *)pbuffer + 1);
            byte_addr  = POWER19_INTSTS2;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = &tmp_value;
            __inf("p1 int clear intsts2 = %d!\n",tmp_value);
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }

            tmp_value = *((__u8 *)pbuffer + 2);
            byte_addr  = POWER19_INTSTS3;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = &tmp_value;
            __inf("p1 int clear intsts3 = %d!\n",tmp_value);
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }

            tmp_value = *((__u8 *)pbuffer + 3);
            byte_addr  = POWER19_INTSTS4;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = &tmp_value;
            __inf("p1 int clear intsts4 = %d!\n",tmp_value);
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_VOUT_GET:
        {
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.byte_count         = 1;  // data is 1 byte

            if(aux == POWER_VOL_LDO1)
            {
                ret = 1250;
            }
            else if(aux == POWER_VOL_DCDC1)
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_DC1OUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret = 700 + tmp_value  * 25;
				__inf("Voltage Scale! Vout_Getdc1_%d\n",ret);
            }
            else if(aux == POWER_VOL_DCDC2)
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_DC2OUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret = 700 + tmp_value  * 25;
                if((ret <= (Vout_dc2_target + 25))  &&  (ret >= (Vout_dc2_target)))
                    ret = Vout_dc2_target;
                __inf("Voltage Scale! Vout_Getdc2_%d\n",ret);
            }
            else if(aux == POWER_VOL_DCDC3)
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_DC3OUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret = 700 + tmp_value  * 25;
				__inf("Voltage Scale! Vout_Getdc3_%d\n",ret);
            }
            else if(aux == POWER_VOL_EXTEN)
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_LDO3OUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret = 700 + tmp_value  * 25;
            }
            else if((aux == POWER_VOL_LDO2) || (aux == POWER_VOL_LDO3))
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_LDO24OUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                if(aux == POWER_VOL_LDO2)
                {
                    ret = 1800 + ((tmp_value >> 4) & 0x0f) * 100;
                }
                else
                {
                    ret = 1800 + (tmp_value  & 0x0f) * 100;
                }
            }

            else if(aux == POWER_VOL_GPIO0)
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_GPIO0_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret = 1800 + ((tmp_value >> 4) & 0x0f) * 100;
            }

            else if((aux == POWER_VOL_LDO5))                        //接受POWER_VOL_LDO5 当GPIO0处理，做VMIC
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_GPIO0_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret = 1800 + ((tmp_value >> 4) & 0x0f) * 100;
            }

            break;
        }

        case DRV_POWER_CMD_VOUT_SET:
        {
            __u32               tmp;

            tmp = (__u32)pbuffer;

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.byte_count = 1;

            __wrn("Voltage Scale! Vout_Set\n");
            if(aux == POWER_VOL_DCDC1 )
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_DC1OUT_VOL;
                hstr.byte_addr  = &byte_addr;
				__wrn("Voltage Scale! DCDC1_%d\n",tmp);

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                if((tmp >= 700) && (tmp <= 3500))
                {
                    tmp_value &= 0x80;
                    tmp_value = ((tmp - 700)/25) ;
                }
                else
                {
                    break;
                }
                ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }

            if(aux == POWER_VOL_DCDC2 )
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_DC2OUT_VOL;
                hstr.byte_addr  = &byte_addr;
                __wrn("Voltage Scale! DCDC2_%d\n",tmp);
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                if( (tmp >= 700) && (tmp <= 2275))
                {
                    //fix dcdc2 to a valid value, 25mv per step
                    tmp = tmp/25;
                    tmp = tmp*25;

                    __wrn("Voltage Scale! tmp_value %d\n",tmp_value);
                    if(tmp > (700 + tmp_value*25))
                    {
                        while(tmp > (700 + tmp_value*25))
                        {
                            tmp_value++;
                            ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                            if(ret != EPDK_OK)
                            {
                                break;
                            }
                            ret = (EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr));
                            if(ret != EPDK_OK)
                            {
                                break;
                            }
                        }
                    }

                    if(tmp <= (675 + tmp_value*25))
                    {
                        while(tmp <= (675 + tmp_value*25))
                        {
                            tmp_value--;
                            ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                            if(ret != EPDK_OK)
                            {
                                break;
                            }
                            ret = (EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr));
                            if(ret != EPDK_OK)
                            {
                                break;
                            }
                        }
                    }

                    Vout_dc2_target = tmp;
                }
                else
                {
                    break;
                }
            }

            if(aux == POWER_VOL_DCDC3 )
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_DC3OUT_VOL;
                hstr.byte_addr  = &byte_addr;
				__wrn("Voltage Scale! DCDC3_%d\n",tmp);

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                if((!(tmp%25)) && (tmp >= 700) && (tmp <= 3500))
                {
                    tmp_value &= 0x80;
                    tmp_value = ((tmp - 700)/25) ;
                }
                else
                {
                    break;
                }
                ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
            else if(aux == POWER_VOL_LDO1)
            {
                __wrn("Voltage LDO1 can't be set!\n");
            }
            else if((aux == POWER_VOL_EXTEN))
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_LDO3OUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                if((!(tmp%25)) && (tmp >= 700) && (tmp <= 3500))
                {
                    tmp_value &= 0x80;
                    tmp_value = ((tmp - 700)/25) ;
                }
                else
                {
                    break;
                }
                ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
            else if((aux == POWER_VOL_LDO3) || (aux == POWER_VOL_LDO2))
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_LDO24OUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                if(aux == POWER_VOL_LDO4)
                {
                    if((!(tmp%100)) && (tmp >= 1800) && (tmp <= 3300))
                    {
                        tmp_value &= 0xf0;
                        tmp_value |= ((tmp - 1800)/100) ;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    if((!(tmp%100)) && (tmp >= 1800) && (tmp <= 3300))
                    {
                        tmp_value &= 0x0f;
                        tmp_value |= ((tmp - 1800)/100) ;
                    }
                    else
                    {
                        break;
                    }
                }
                ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }

            else if(aux == POWER_VOL_LDO1)
            {
                __wrn("Voltage LDO1 can't be set!\n");
            }
            else if((aux == POWER_VOL_LDO5))                        //接受POWER_VOL_LDO5 当GPIO0处理，做VMIC
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER19_GPIO0_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                if((!(tmp%100)) && (tmp >= 1800) && (tmp <= 3300))
                {
                    tmp_value = (((tmp - 1800)/100)  << 4);
                }
                else
                {
                    break;
                }
                __wrn("Voltage LDOGPIO tmp_value = %d!\n",tmp_value);
                ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
						
            break;
        }

        case DRV_POWER_CMD_VSTS_GET:                    //Power Rail On/Off control
        {
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte

            hstr.byte_count = 1;
            hstr.data       = &tmp_value;

            if(aux == POWER_VOL_LDO1)
            {
                ret = 1;
            }
            else if((aux == POWER_VOL_DCDC1) || (aux == POWER_VOL_LDO3) || (aux == POWER_VOL_DCDC3) || (aux == POWER_VOL_LDO2))
            {
                byte_addr  = POWER19_LDO24_DC13_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                if(aux == POWER_VOL_DCDC1)
                {
                    ret = tmp_value  & 0x01 ;
                }
                else if(aux == POWER_VOL_DCDC3)
                {
                    ret  =(tmp_value >> 1) & 0x01;
                }
                else if(aux == POWER_VOL_LDO3)
                {
                    ret = (tmp_value >> 3) & 0x01;
                }
                else
                {
                    ret = (tmp_value >> 2) & 0x01;
                }
            }
            else if((aux == POWER_VOL_DCDC2) || (aux == POWER_VOL_EXTEN))
            {
                byte_addr  = POWER19_LDO3_DC2_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                if(aux == POWER_VOL_DCDC2)
                {
                    ret = tmp_value & 0x01;
                }
                else
                {
                    ret = (tmp_value >> 2) & 0x01;
                }
            }
            else if(aux == POWER_VOL_LDO5)                    //接受POWER_VOL_LDO5 当GPIO0处理，做VMIC
            {
                byte_addr  = POWER19_GPIO0_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                if((tmp_value & 0x07) == 0x02)
                {
                    ret = 1;
                }
                else
                {
                    ret = 0;
                }
            }
            else if(aux == POWER_VOL_SW1)                    //接受POWER_VOL_SW1 当GPIO1处理，做lcd_vcc 控制
            {
                byte_addr  = POWER19_GPIO1_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                if((tmp_value & 0x07)  == 0x05)
                {
                	ret = 0x01;
                }
                else
                {
                	ret = 0x0;
                }
            }

            break;
        }

        case DRV_POWER_CMD_VSTS_SET:            //1:enable,0:disable
        {
            __u32  temp;

            temp            = (__u32)pbuffer;

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.byte_count = 1;
            hstr.data       = &tmp_value;

            if((aux == POWER_VOL_DCDC1) || (aux == POWER_VOL_LDO3) || (aux == POWER_VOL_DCDC3) || (aux == POWER_VOL_LDO2))
            {
                byte_addr  = POWER19_LDO24_DC13_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                if(aux == POWER_VOL_DCDC1)
                {
                    tmp_value &= 0xfe;
                    tmp_value |= (temp >> 0);
                }
                else if(aux == POWER_VOL_DCDC3)
                {
                    tmp_value &= 0xfd;
                    tmp_value |= (temp << 1);
                }
                else if(aux == POWER_VOL_LDO3)
                {
                    tmp_value &= 0xf7;
                    tmp_value |= (temp << 3);
                }
                else
                {
                    tmp_value &= 0xfb;
                    tmp_value |= (temp << 2);
                }
                ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
            else if((aux == POWER_VOL_DCDC2) || (aux == POWER_VOL_EXTEN))
            {
                byte_addr  = POWER19_LDO3_DC2_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                if(aux == POWER_VOL_DCDC2)
                {
                    tmp_value &= 0xfe;
                    tmp_value |= (temp << 0);
                }
                else
                {
                    tmp_value &= 0xfb;
                    tmp_value |= (temp << 2);
                }
                ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
            else if(aux == POWER_VOL_LDO5)                    //接受POWER_VOL_LDO5 当GPIO0处理，做VMIC
            {
                __wrn("LDOGPIO to be set!\n");
                byte_addr  = POWER19_GPIO0_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                if(temp)
                {
                    tmp_value &= 0xf8;
                    tmp_value |= 0x02;
                }
                else
                {
                    tmp_value |= 0x07;
                }
                ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
            else if(aux == POWER_VOL_SW1)       //接受POWER_VOL_SW1 当GPIO1处理，做lcd_vcc 控制,拉低打开电源
            {
                __wrn("Crontrol GPIO1\n");
                byte_addr  = POWER19_GPIO1_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                if(temp)
                {
                    tmp_value &= 0xf8;
                    tmp_value |= 0x05;
                }
                else
                {
                    tmp_value |= 0x07;
                }
                ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }

            break;
        }

        case DRV_POWER_CMD_PWREPT_GET:
        {
            __u8 IC_type = 0;
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.byte_count = 1;
            hstr.data       = &tmp_value;

//            __inf("CMD_PWREPT_GET!\n");
            if(aux == POWER_EPT_1)
            {
//                __inf("Control GPIO0_get\n");
                byte_addr  = POWER19_GPIO0_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value  &= 0xf8;
                tmp_value |= 0x01;                         //Set GPIO0 as input
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr  = POWER19_GPIO012_SIGNAL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret  = 0x01;
                ret  |= ((tmp_value >> 4) & 0x01) << 2;
            }
            else if(aux == POWER_EPT_2)
            {
//                __inf("Control GPIO1_get\n");
                byte_addr  = POWER19_GPIO1_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value  &= 0xf8;
                tmp_value |= 0x01;                         //Set GPIO1 as input
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr  = POWER19_GPIO012_SIGNAL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret  = 0x01;
                ret  |= ((tmp_value >> 5) & 0x01) << 2;
            }
            else if(aux == POWER_EPT_3)
            {
//                __inf("Control GPIO2_get_get\n");
                byte_addr  = POWER19_GPIO2_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value &= 0xf8;
                tmp_value |= 0x01;                         //Set GPIO2 as input
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr  = POWER19_GPIO012_SIGNAL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret  = 0x01;
                ret  |= ((tmp_value >> 6) & 0x01) << 2;
            }
            else if(aux == POWER_EPT_4)
            {
//                __inf("Control BATSENSE_get\n");
                byte_addr  = POWER19_SENSE_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value  &= 0xfC;
                tmp_value |= 0x82;                         //Set GPIO3 as input
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr  = POWER19_SENSE_SIGNAL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret  = 0x01;
                ret  |= ((tmp_value >> 4) & 0x01) << 2;
            }
            else if(aux == POWER_EPT_5)
            {
//                __inf("Control CHSENSE_get\n");
                byte_addr  = POWER19_SENSE_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value  &= 0xf3;
                tmp_value |= 0x88;                         //Set GPIO4 as input
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr  = POWER19_SENSE_SIGNAL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret  = 0x01;
                ret  |= ((tmp_value >> 5) & 0x01) << 2;
            }
            else if(aux == POWER_EPT_6)
            {
//                __inf("Control GPIO_RSTO_get\n");
                byte_addr  = POWER19_RSTO_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value  &= 0x3f;
                tmp_value |= 0xc0;                         //Set GPIO5 as input
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr =POWER19_IC_TYPE; //Get IC Type
		            hstr.byte_addr  = &byte_addr;
		            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
		            {
		                break;
		            }
		            if((tmp_value & 0x0f) == 0x03)
		          	{
		          		IC_type = 1; //AW1628
		          	}
		          	else
		        		{
		        			IC_type = 0; //AW1612
		        		}
		        		if(IC_type)
	        			{
	        				byte_addr  = POWER19_GPIO67_CTL;
	                hstr.byte_addr  = &byte_addr;
	                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
	                {
	                    break;
	                }
	                ret  = 0x01;
	                ret  |= ((tmp_value >> 0) & 0x01) << 2;
	        			}
		        		else
		        		{
	                byte_addr  = POWER19_RSTO_CTL;
	                hstr.byte_addr  = &byte_addr;
	                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
	                {
	                    break;
	                }
	                ret  = 0x01;
	                ret  |= ((tmp_value >> 4) & 0x01) << 2;
              }
            }
            else if(aux == POWER_EPT_7)
            {
//				__inf("Control PWREN_GPIO Get\n");
								byte_addr  = POWER19_GPIO67_CTL;
								hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value  &= 0xbf;
                tmp_value |= 0x40;                         //Set GPIO6 as input
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr  = POWER19_GPIO67_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret  = 0x01;
                ret  |= ((tmp_value >> 4) & 0x01) << 2;
            }
            else if(aux == POWER_EPT_8)
            {
//		__inf("Control STSEN_GPIO Get\n");
								byte_addr  = POWER19_GPIO67_CTL;
								hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value  &= 0xfb;
                tmp_value |= 0x04;                         //Set GPIO7 as input
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr =POWER19_IC_TYPE; //Get IC Type
		            hstr.byte_addr  = &byte_addr;
		            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
		            {
		                break;
		            }
		            if((tmp_value & 0x0f) == 0x03)
		          	{
		          		IC_type = 1; //AW1628
		          	}
		          	else
		        		{
		        			IC_type = 0; //AW1612
		        		}
		        		if(IC_type)
	        			{
	        				byte_addr  = POWER19_RSTO_CTL;
	                hstr.byte_addr  = &byte_addr;
	                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
	                {
	                    break;
	                }
	                ret  = 0x01;
	                ret  |= ((tmp_value >> 4) & 0x01) << 2;
	        			}
		        		else
		        		{
	                byte_addr  = POWER19_GPIO67_CTL;
	                hstr.byte_addr  = &byte_addr;
	                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
	                {
	                    break;
	                }
	                ret  = 0x01;
	                ret  |= ((tmp_value >> 0) & 0x01) << 2;
              }
            }
            break;
        }

        case DRV_POWER_CMD_PWREPT_SET:
        {
            __u32   temp;
            temp            = (__u32)pbuffer;

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte

			__msg("cmd = DRV_POWER_CMD_PWREPT_SET  aux = %d\n",aux);
            if(aux == POWER_EPT_1)
            {
                __wrn("Control GPIO0_set\n");
                byte_addr  = POWER19_GPIO0_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value &= 0xf8;
                tmp_value |= 0x00;                         //Set GPIO0 as output
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr  = POWER19_GPIO012_SIGNAL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value &= 0xfe;
                tmp_value |= (((temp >> 2) & 0x01) << 0);
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
            else if(aux == POWER_EPT_2)
            {
                __wrn("Control GPIO1_set\n");
                byte_addr  = POWER19_GPIO1_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value  &= 0xf8;
                tmp_value |= 0x00;                         //Set GPIO1 as output
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr  = POWER19_GPIO012_SIGNAL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value &= 0xfd;
                tmp_value |= (((temp >> 2) & 0x01) << 1);
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
            else if(aux == POWER_EPT_3)
            {
                __wrn("Control GPIO2_set\n");
                byte_addr  = POWER19_GPIO2_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value  &= 0xf8;
                tmp_value |= 0x00;                         //Set GPIO2 as output
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr  = POWER19_GPIO012_SIGNAL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value &= 0xfb;
                tmp_value |= (((temp >> 2) & 0x01) << 2);
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
            else if(aux == POWER_EPT_4)
            {
                __wrn("Control BATSENSE_set\n");
                byte_addr  = POWER19_SENSE_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value  &= 0xfc;
                tmp_value |= 0x81;                         //Set GPIO3 as output
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr  = POWER19_SENSE_SIGNAL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value &= 0xfe;
                tmp_value |= (((temp >> 2) & 0x01) << 0);
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
            else if(aux == POWER_EPT_5)
            {
                __wrn("Control CHSENSE_set\n");
                byte_addr  = POWER19_SENSE_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value  &= 0xf3;
                tmp_value |= 0x84;                         //Set GPIO4 as output
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr  = POWER19_SENSE_SIGNAL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value &= 0xfd;
                tmp_value |= (((temp >> 2) & 0x01) << 1);
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
            else if(aux == POWER_EPT_6)
            {
                __wrn("Control RSTO_set\n");
                byte_addr  = POWER19_RSTO_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value  &= 0x3f;
                tmp_value |= 0x80;                         //Set GPIO5 as output
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr  = POWER19_RSTO_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value &= 0xdf;
                tmp_value |= (((temp >> 2) & 0x01) << 5);
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
            else if(aux == POWER_EPT_7)
            {
//				__wrn("Control PWREN_GPIO Set\n");
				byte_addr  = POWER19_GPIO67_CTL;
				hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value  &= 0xbf;						 //Set GPIO6 as output
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr  = POWER19_GPIO67_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value &= 0xdf;
                tmp_value |= (((temp >> 2) & 0x01) << 5);
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
            else if(aux == POWER_EPT_8)
            {
//				__wrn("Control SYSEN_GPIO Set\n");
				byte_addr  = POWER19_GPIO67_CTL;
				hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value  &= 0xfb;			 //Set GPIO7 as output
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr  = POWER19_GPIO67_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value &= 0xfd;
                tmp_value |= (((temp >> 2) & 0x01) << 1);
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_CHGSTAT_GET:
        {
            __drv_power_charge_stat     *Charge_Stat = (__drv_power_charge_stat *)pbuffer;
            __u8    temp_value[4];

            eLIBs_memset(Charge_Stat, 0, sizeof(__drv_power_charge_stat));

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER19_STATUS;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = temp_value;
            hstr.byte_count         = 2;  // data is 1 byte

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
//            __inf("POWER_STATUS = %d %d\n",temp_value[0],temp_value[1]);

            Charge_Stat->acin_valid = (temp_value[0] & 0x40) >> 6;
            Charge_Stat->vbus_valid = (temp_value[0] & 0x10) >> 4;
            Charge_Stat->ext_power_valid = Charge_Stat->acin_valid | Charge_Stat->vbus_valid;
            Charge_Stat->bat_current_direction = (temp_value[0] & 0x04) >> 2;
            Charge_Stat->in_Short = (temp_value[0] & 0x02) >> 1;
            Charge_Stat->int_over_temp = (temp_value[1] & 0x80) >> 7;
            Charge_Stat->charge_indication = (temp_value[1] & 0x40) >> 6;
            Charge_Stat->battery_exist = (temp_value[1] & 0x20) >> 5;
            Charge_Stat->battery_active = (temp_value[1] & 0x08) >> 3;
            Charge_Stat->low_charge_current = (temp_value[1] & 0x04) >> 2;

            hstr.byte_count = 4;
            hstr.data       = temp_value;
            byte_addr  = POWER19_BAT_AVERCHGCUR_H8;
            hstr.byte_addr  = &byte_addr;

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            Charge_Stat->battery_current = ((temp_value[0]<<5) + (temp_value[1]&0x1f) + (temp_value[2]<<5) + (temp_value[3]&0x1f))*0.5;
//            __inf("!!!!!!!!!dev_power19 Charge_Stat->battery_current temp_value[0]= %x\n",temp_value[0]);
//            __inf("!!!!!!!!!dev_power19 Charge_Stat->battery_current temp_value[1]= %x\n",temp_value[1]);
//            __inf("!!!!!!!!!dev_power19 Charge_Stat->battery_discharge_current temp_value[2]= %x\n",temp_value[2]);
//            __inf("!!!!!!!!!dev_power19 Charge_Stat->battery_discharge_current temp_value[3]= %x\n",temp_value[3]);
//            __inf("!!!!!!!!!dev_power19 Charge_Stat->battery_current = %d\n",Charge_Stat->battery_current);

            hstr.byte_count = 2;
            hstr.data       = temp_value;
            byte_addr  = POWER19_BAT_AVERVOL_H8;//获取外部电源是否存在、电池是否接入、电池电流方向、充电否等信息
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            Charge_Stat->battery_vol = ((temp_value[0]<<4) + (temp_value[1]&0x0f))*1.1;
//            __inf("Charge_Stat->battery_vol = %d\n",Charge_Stat->battery_vol);

            hstr.byte_count = 1;
            byte_addr  = POWER19_CHARGE1;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            Charge_Stat->Ichgset = _reg2test(temp_value[0] & 0x0f,AXP199_Version & 0x10);
//            __inf("Charge_Stat->Ichgset = %d\n",Charge_Stat->Ichgset);
            Charge_Stat->Iendchg = Charge_Stat->Ichgset * (10 + 5 * ((temp_value[0] >> 4) & 0x01))/100;
//            __inf("Charge_Stat->Iendchg = %d\n",Charge_Stat->Iendchg);

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_BATADC_GET:
        {
            __drv_power_battery_status_t1   *bat_sts = (__drv_power_battery_status_t1 *)pbuffer;

            eLIBs_memset(bat_sts, 0, sizeof(__drv_power_battery_status_t1));
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER19_MODE_CHGSTATUS;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            bat_sts->battery_exist = ((tmp_value >> 5) & 0x01);
            if(bat_sts->battery_exist)
            {
                __u8 adcsw;

                hstr.data       = &adcsw;
                byte_addr  = POWER19_ADC_EN1;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                bat_sts->bat_voladc_enable  = ((adcsw >> 7) & 0x01);
                bat_sts->bat_crntadc_enable = ((adcsw >> 6) & 0x01);
                byte_addr  = POWER19_ADC_SPEED;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                adcsw = ((adcsw >> 6) & 0x03);
                bat_sts->adc_freq    = (25 << adcsw);

                hstr.data       = &tmp_value;
                if(bat_sts->bat_voladc_enable)
                {
                    byte_addr  = POWER19_BAT_AVERVOL_H8;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    bat_sts->bat_vol =  (tmp_value << 4);
                    byte_addr  = POWER19_BAT_AVERVOL_L4;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    bat_sts->bat_vol +=  tmp_value ;
                    bat_sts->bat_vol *= 1.1;
                }

                byte_addr  = POWER19_STATUS;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                bat_sts->bat_status = ((tmp_value >> 2) & 0x01);


                if(bat_sts->bat_crntadc_enable)
                {
                    if(bat_sts->bat_status)                                                //charging
                    {
                        byte_addr  = POWER19_BAT_AVERCHGCUR_H8;
                        hstr.byte_addr  = &byte_addr;
                        if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                        {
                            break;
                        }
                        bat_sts->bat_crnt = (tmp_value << 5);
                        byte_addr  = POWER19_BAT_AVERCHGCUR_L5;
                        hstr.byte_addr  = &byte_addr;
                        if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                        {
                            break;
                        }
                        bat_sts->bat_crnt += tmp_value;
                        bat_sts->bat_crnt /= 2;
                    }
                    else
                    {
                        byte_addr  = POWER19_BAT_AVERDISCHGCUR_H8;
                        hstr.byte_addr  = &byte_addr;
                        if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                        {
                            break;
                        }
                        bat_sts->bat_crnt = (tmp_value << 5);
                        byte_addr  = POWER19_BAT_AVERDISCHGCUR_L5;
                        hstr.byte_addr  = &byte_addr;
                        if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                        {
                            break;
                        }
                        bat_sts->bat_crnt += tmp_value;
                        bat_sts->bat_crnt /= 2;
                    }
                }
            }

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_BATADC_SET:
        {
            __drv_power_battery_status_t1  *bat_sts = (__drv_power_battery_status_t1 *)pbuffer;
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER19_ADC_SPEED;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            if(bat_sts->adc_freq)
            {
                tmp_value &= ~(0x03 << 2);
                if(bat_sts->adc_freq == 25)
                {
                    tmp_value |=   0x00 << 6;
                }
                else if(bat_sts->adc_freq == 50)
                {
                    tmp_value |=   0x01 << 6;
                }
                else if(bat_sts->adc_freq == 100)
                {
                    tmp_value |=   0x02 << 6;
                }
                else if(bat_sts->adc_freq == 200)
                {
                    tmp_value |=   0x03 << 6;
                }
            }
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }

            byte_addr  = POWER19_ADC_EN1;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            if(!(bat_sts->bat_voladc_enable & 0x02))
            {
                tmp_value &= ~(1 << 7);
                tmp_value |= bat_sts->bat_voladc_enable << 7;
            }

            if(!(bat_sts->bat_crntadc_enable & 0x02))
            {
                tmp_value &= ~(1 << 6);
                tmp_value |= bat_sts->bat_crntadc_enable << 6;
            }

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_GET_BAT_RDC:
        {
            __u8        bat_exist,dcin_valid;
            ES_FILE     *pfile;
            __s8        key_value[3] = {'0', '0', '0'};

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER19_STATUS;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            dcin_valid =(((tmp_value >> 6) & 0x01) | ((tmp_value >> 4) & 0x01));
            byte_addr  = POWER19_MODE_CHGSTATUS;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            bat_exist = ((tmp_value >> 5) & 0x01);
            if(!bat_exist)
            {
//                __wrn("battery dosent exist\n");
                goto  _POWER_GET_RDC_;
            }
            if(dcin_valid)//充电
            {
                __u32   i,temp;
                __s32   averPreVol = 0, averPreCur = 0,averNextVol = 0,averNextCur = 0;
                __u32   batvol,batcrnt;
								__wrn("=====AXP19x caculat RDC!Get charge Data!===== \n");
                for(i = 0; i< POWER_RDC_COUNT; i++)
                {
                    
                    byte_addr  = POWER19_BAT_AVERVOL_H8;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    batvol =  (tmp_value << 4);
                    byte_addr  = POWER19_BAT_AVERVOL_L4;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    batvol += (tmp_value & 0x0f) ;
                    batvol *= 1.1;
                    __wrn("=========@@@@@@@@@@@AXP199 Get Charge Data batvol =%d\n",batvol);
                    averPreVol += batvol;

                    byte_addr  = POWER19_BAT_AVERCHGCUR_H8;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    batcrnt = (tmp_value << 5);
                    byte_addr  = POWER19_BAT_AVERCHGCUR_L5;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    batcrnt += (tmp_value & 0x1f);
                    batcrnt /= 2;
                     __wrn("=========@@@@@@@@@@@AXP199 Get Charge Data batcrnt =%d\n",batcrnt);
                    averPreCur += batcrnt;

                    esKRNL_TimeDly(20);
                }
                averPreVol /= POWER_RDC_COUNT;
                averPreCur /= POWER_RDC_COUNT;
                //防止中途拔掉Adapter
                byte_addr          = POWER19_STATUS;
		            hstr.byte_addr  = &byte_addr;
		            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
		            hstr.data               = &tmp_value;
		            hstr.byte_count         = 1;  // data is 1 byte
		
		            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
		            {
		                break;
		            }
		            dcin_valid =(((tmp_value >> 6) & 0x01) | ((tmp_value >> 4) & 0x01));
                if(!dcin_valid)
              	{
                     goto _POWER_GET_RDC_;
              	}

                byte_addr  = POWER19_CHARGE1;
                hstr.byte_addr  = &byte_addr;
                eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr);
                tmp_value &= ~(1 << 7);
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                esKRNL_TimeDly(300);
								__wrn("=====AXP19x caculat RDC!Get Discharge Data!===== \n");
                for(i = 0; i< POWER_RDC_COUNT; i++)
                {
                    byte_addr  = POWER19_BAT_AVERVOL_H8;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    batvol =  (tmp_value << 4);
                    byte_addr  = POWER19_BAT_AVERVOL_L4;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    batvol +=  (tmp_value & 0x0f) ;
                    batvol *= 1.1;
                    __wrn("=========@@@@@@@@@@@AXP199 Get DisCharge Data batvol =%d\n",batvol);
                    averNextVol += batvol;

                    byte_addr  = POWER19_BAT_AVERCHGCUR_H8;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    batcrnt = (tmp_value << 5);
                    byte_addr  = POWER19_BAT_AVERCHGCUR_L5;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    batcrnt +=(tmp_value & 0x1f);
                    batcrnt /= 2;
                    __wrn("=========@@@@@@@@@@@AXP199 Get DisCharge Data batcrnt =%d\n",batcrnt);
                    averNextCur += batcrnt;

                    esKRNL_TimeDly(20);
                }
                averNextVol /= POWER_RDC_COUNT;
                averNextCur /= POWER_RDC_COUNT;
                //防止中途拔掉Adapter
                byte_addr          = POWER19_STATUS;
		            hstr.byte_addr  = &byte_addr;
		            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
		            hstr.data               = &tmp_value;
		            hstr.byte_count         = 1;  // data is 1 byte
		
		            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
		            {
		                break;
		            }
		            dcin_valid =(((tmp_value >> 6) & 0x01) | ((tmp_value >> 4) & 0x01));
                if(!dcin_valid)
              	{
                     goto _POWER_GET_RDC_;
              	}

                byte_addr  = POWER19_CHARGE1;
                hstr.byte_addr  = &byte_addr;
                eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr);
                tmp_value |=  (1 << 7);
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                esKRNL_TimeDly(50);

                if(ABS(averPreCur - averNextCur) > 300)
                {
                    temp = 1000 * ABS(averPreVol - averNextVol)/ABS(averPreCur + averNextCur);    //获得新的Rdc ,mOhm
                    if((temp < 70) || (temp >800))    //新的值在100mOhm和800mOhm之间?超出此范围返回原来的值
                    {
                        goto _POWER_GET_RDC_;
                    }
                    else
                    {
                        __u8  chr[3];

                        ret = temp;
                        __wrn("Current batrdc  = %d\n", ret);
                        pfile = eLIBs_fopen("D:\\DRV\\BATRDC.INI", "r+");
                        if( pfile == NULL )
                        {
                            __wrn("file cant be open or create\n");

                            break;
                        }

                        eLIBs_fread(chr, 1, 3, pfile);
                        __wrn("Previous batrdc  = %x\n", chr[0]);
                        __wrn("Previous batrdc  = %x\n", chr[1]);
                        __wrn("Previous batrdc  = %x\n", chr[2]);
                        if((chr[0] - 0xb0) > 0)                 //如果文件保存的是 上次正确检测的值，此次做平均
                        {
                            ret += ((chr[0]&0x7f)-'0') * 100 + (chr[1]-'0')*10 + (chr[2]-'0');
                            ret >>= 1;
                            __wrn("bat panduan  = %d\n", ret);
                        }

                        temp = ret;
                        key_value[0] = temp/100 + 0x80 + '0';
                        temp %= 100;
                        key_value[1] = temp/10 + '0';
                        temp %= 10;
                        key_value[2] = temp + '0';

                        eLIBs_fseek(pfile,-3, SEEK_CUR);
                        eLIBs_fwrite(key_value, 1, 3, pfile);   //此次后的值写回文件
                        eLIBs_fclose(pfile);

                        __wrn("bat  = %d\n", ret);
                    }
                }
                else
                {
                    goto  _POWER_GET_RDC_;
                }
            }

_POWER_GET_RDC_:
            pfile = eLIBs_fopen("D:\\DRV\\BATRDC.INI", "r+");
            if( pfile == NULL )
            {
                pfile = eLIBs_fopen("D:\\DRV\\BATRDC.INI", "w+");
                if(pfile == NULL)
                {
                    __wrn("create file fail\n");
                    break;
                }
//                __wrn("create file ok\n");
                key_value[0] = '2';
                key_value[1] = '0';
                key_value[2] = '0';
                eLIBs_fwrite(key_value, 1, 3, pfile);
            }

            eLIBs_fread(key_value, 1, 3, pfile);
            eLIBs_fclose(pfile);

            ret = ((key_value[0] & 0x7f) -'0') * 100 + (key_value[1]-'0') * 10 + (key_value[2]-'0');

//            __wrn("bat rdc = %d\n", ret);

            break;
        }

        case DRV_POWER_CMD_BATSTATUS_TABLE_GET:
        {
            __u16   dcin_vol;
            __u8    tmp_value[4];
            __drv_power_battery_status_t2  *bat_sts = (__drv_power_battery_status_t2 *)pbuffer;

            {
                __s32 Battery_cap;

                if(esCFG_GetKeyValue("power", "Battery_cap",  &Battery_cap, 1))
                {
                    __wrn("fail to get Battery cap, set one default value\n");

                    bat_sts->bat_cap  = 2200;
                }
                else
                {
                    bat_sts->bat_cap  = Battery_cap;
                }
            }

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER19_STATUS;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = tmp_value;
            hstr.byte_count         = 2;  // data is 2 bytes

//            __wrn("DRV_POWER_CMD_BATSTATUS_TABLE_GET\n");
            bat_sts->bat_rdc = this_rdc;

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            bat_sts->battery_exist = ((tmp_value[1] >> 5) & 0x01);
            bat_sts->dcin_valid = ((tmp_value[0] >> 4) & 0x01)  |  ((tmp_value[0] >> 6) & 0x01);
            bat_sts->battery_status =  (~(tmp_value[0] >> 2) & 0x01);           //Bat current direction,0:charge,1:discharge
            bat_sts->charge_status = ((tmp_value[1] >> 6) & 0x01);              //charge static,0:discharge
            bat_sts->rest_vol = fuelguage.rest_vol;
            bat_sts->rest_time = fuelguage.rest_time;
            bat_sts->bat_cap = fuelguage.bat_cap;

            hstr.byte_count = 4;
            hstr.data = tmp_value;
            byte_addr  = POWER19_ACIN_VOL_H8;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            if((tmp_value[0] >> 7) & 0x01)
            {
                dcin_vol =((tmp_value[0] << 4) + (tmp_value[1] & 0x0f))  * 1.7;  //acin exist then use acin vol
            }
            else
            {
                dcin_vol =((tmp_value[2] << 4) + (tmp_value[3] & 0x0f))  * 1.7;    //acin not exist then use vbus vol
            }
            if(dcin_vol < 4200)
            {
                bat_sts->dcin_not_enough = 1;
            }
            else
            {
                bat_sts->dcin_not_enough = 0;
            }

            if(bat_sts->battery_exist)
            {
                __u32   Ichgset, total_vol;

                hstr.byte_count = 2;
                hstr.data       = tmp_value;
                byte_addr  = POWER19_BAT_AVERVOL_H8;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                bat_sts->bat_vol = ((tmp_value[0] << 4) + (tmp_value[1] & 0x0f))  * 1.1;
                total_vol = bat_sts->bat_vol;

                hstr.byte_count = 4;
                hstr.data       = tmp_value;
                byte_addr  = POWER19_BAT_AVERCHGCUR_H8;//获取电池ADC检测电流值
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                bat_sts->bat_crnt = ((tmp_value[0]<<5) + (tmp_value[1]&0x1f) +(tmp_value[2]<<5) + (tmp_value[3]&0x1f))*0.5;

                hstr.byte_count = 2;
                byte_addr  = POWER19_CHARGE1;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                Ichgset = _reg2test(tmp_value[0] & 0x0f,AXP199_Version & 0x10);//700

                if((dcin_vol < (bat_sts->bat_vol + 200)) || (dcin_vol < 4200))
                {
                    if((bat_sts->bat_crnt < (3 * Ichgset / 5)) && (bat_sts->dcin_valid))
                    {
                        bat_sts->dcin_not_enough = 1;
                    }
                    else
                    {
                        bat_sts->dcin_not_enough = 0;
                    }
                }
                else
                {
                    bat_sts->dcin_not_enough = 0;
                }

                if(bat_sts->dcin_valid)//充电
                {
                    total_vol -= bat_sts->bat_crnt * bat_sts->bat_rdc * CHG_RDC_RATE/100000;
                }
                else  //放电
                {
                    __s32 Discharge_current_rate;

                    if(esCFG_GetKeyValue("power", "Discharge_rate",  &Discharge_current_rate, 1))
                    {
//                        __wrn("fail to get Discharge current rate, set one default value\n");

                        Discharge_current_rate  = 10;
                    }
                    bat_sts->bat_crnt = bat_sts->bat_crnt * Discharge_current_rate / 10 ;
                    total_vol += bat_sts->bat_crnt * (bat_sts->bat_rdc - Discharge_Rdc_Cal) / 1000;
                }
            }
            else
            {
                bat_sts->bat_vol = 2500;
                bat_sts->bat_crnt = 0;
            }

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_DCINADC_GET:
        {
            __drv_power_dcin_status_t   *dcin_sts = (__drv_power_dcin_status_t *)pbuffer;

            eLIBs_memset(dcin_sts, 0, sizeof(__drv_power_dcin_status_t));

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER19_STATUS;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 bytes

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            dcin_sts->dcin_exist =(((tmp_value >> 6) & 0x01) |((tmp_value >> 4) & 0x01));
            if(dcin_sts->dcin_exist)
            {
                __u32   acinvol,acincur;
                byte_addr  = POWER19_ADC_EN1;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                dcin_sts->dcin_voladc_enable  = ((tmp_value >> 5) & 0x01);
                dcin_sts->dcin_crntadc_enable = ((tmp_value >> 4) & 0x01);

                hstr.data = &tmp_value;
                if(dcin_sts->dcin_voladc_enable)
                {
                    byte_addr  = POWER19_ACIN_VOL_H8;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    acinvol = tmp_value << 4;
                    byte_addr  = POWER19_ACIN_VOL_L4;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    acinvol += tmp_value;
                    dcin_sts->dcin_vol = acinvol * 1.7;
                }

                if(dcin_sts->dcin_crntadc_enable)
                {
                    byte_addr  = POWER19_ACIN_CUR_H8;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    acincur = tmp_value << 4;
                    byte_addr  = POWER19_ACIN_CUR_L4;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    acincur += tmp_value;
                    dcin_sts->dcin_crnt = acincur * 0.625;
                }
            }

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_DCINADC_SET:
        {
            __drv_power_dcin_status_t   *dcin_sts = (__drv_power_dcin_status_t *)pbuffer;
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER19_ADC_EN1;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 bytes

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            if(!(dcin_sts->dcin_voladc_enable & 0x02))
            {
                tmp_value &= ~(1 << 5);
                tmp_value |= dcin_sts->dcin_voladc_enable << 5;
            }

            if(!(dcin_sts->dcin_crntadc_enable & 0x02))
            {
                tmp_value &= ~(1 << 4);
                tmp_value |= dcin_sts->dcin_crntadc_enable << 4;
            }

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_CHARGE_GET:
        {
            __drv_power_charge_status_t  *charge = (__drv_power_charge_status_t *)pbuffer;
            __u8    tmp1, tmp4, tmp5;

            eLIBs_memset(charge, 0, sizeof(__drv_power_charge_status_t));

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER19_MODE_CHGSTATUS;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp1;
            hstr.byte_count         = 1;  // data is 1 bytes

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            if(tmp1 & 0x20)        //bat exist?
            {
                tmp1 &= 0x01;
                hstr.data       = &tmp4;
                byte_addr  = POWER19_CHARGE1;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                hstr.data       = &tmp5;
                byte_addr  = POWER19_CHARGE2;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                if(tmp4 & 0x80)
                {
                    charge->charge_enable      = 1;
                    if(tmp1 & 0x40)
                    {
                        charge->battery_status = 1;//允许充电，正在充电
                    }
                    else
                    {
                        charge->battery_status = 2;//允许充电，正在放电
                    }
                }
                else
                {
                    charge->charge_enable  = 0;
                    charge->battery_status = 3;    //禁止充电
                }

                charge->charge_end         = (tmp4 >> 4) & 0x01;
                charge->charge_autolimit   = (tmp4 >> 3) & 0x01;

                tmp_value  = (tmp4 >> 5) & 0x03;
                if(tmp_value == 3)
                {
                    charge->charge_vtarget = 4360;
                }
                else if(tmp_value == 0)
                {
                    charge->charge_vtarget = 4100;
                }
                else if(tmp_value == 1)
                {
                    charge->charge_vtarget = 4150;
                }
                else
                {
                    charge->charge_vtarget = 4200;
                }

                tmp_value = (tmp4 >> 0) & 0x0f;
                tmp_value = (tmp4 >> 0) & 0x0f;
				if(tmp_value == 0x00)
				{
					charge->charge_current = 100;
				}
                else if(tmp_value == 0x01)
				{
					charge->charge_current = 190;
				}
				else if(tmp_value == 0x02)
				{
					charge->charge_current = 280;
				}
				else if(tmp_value == 0x03)
				{
					charge->charge_current = 360;
				}
				else if(tmp_value == 0x04)
				{
					charge->charge_current = 450;
				}
				else if(tmp_value == 0x05)
				{
					charge->charge_current = 550;
				}
				else if(tmp_value == 0x06)
				{
					charge->charge_current = 630;
				}
				else if(tmp_value == 0x07)
				{
					charge->charge_current = 700;
				}
//				else if(tmp_value == 0x08)
//				{
//					charge->charge_current = 780;
//				}
				else if(tmp_value == 0x09)
				{
					charge->charge_current = 880;
				}
				else if(tmp_value == 0x0a)
				{
					charge->charge_current = 960;
				}
				else if(tmp_value == 0x0b)
				{
					charge->charge_current = 1000;
				}
				else if(tmp_value == 0x0c)
				{
					charge->charge_current = 1080;
				}
				else if(tmp_value == 0x0d)
				{
					charge->charge_current = 1160;
				}
				else if(tmp_value == 0x0e)
				{
					charge->charge_current = 1240;
				}
				else if(tmp_value == 0x0f)
				{
					charge->charge_current = 1320;
				}
				else
				{
					charge->charge_current = 780;
				}

                tmp_value = (tmp5 >> 6) & 0x03;
                charge->charge_timer1      =  30 + tmp_value * 10;

                tmp_value                  = (tmp5 >> 0) & 0x03;
                charge->charge_timer2      = 420 + tmp_value * 60;
            }
            else
            {
                charge->battery_status = 0;
            }

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_CHARGE_SET:
        {
            __drv_power_charge_status_t  *charge = (__drv_power_charge_status_t *)pbuffer;

            __u8       tmp4;
            __u8    tmp5;

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER19_CHARGE1;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp4;
            hstr.byte_count         = 1;  // data is 1 bytes

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            hstr.data       = &tmp5;
            byte_addr  = POWER19_CHARGE2;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            if(charge->charge_end < 0x02)
            {
                tmp4 &= ~(1<< 4);
                tmp4 |=  charge->charge_end << 4;
            }
            if(charge->charge_enable < 0x02)
            {
                tmp4 &= ~(1 << 7);
                tmp4 |= charge->charge_enable << 7;
            }

            if(charge->charge_vtarget == 4360)
            {
                tmp4 &= ~(3 << 5);
                tmp4 |= 3 << 5;
            }
            else if(charge->charge_vtarget == 4200)
            {
                tmp4 &= ~(3 << 5);
                tmp4 |= 2 << 5;
            }
            else if(charge->charge_vtarget == 4100)
            {
                tmp4 &= ~(3 << 5);
                tmp4 |= 0 << 5;
            }
            else if(charge->charge_vtarget == 4150)
            {
                tmp4 &= ~(3 << 5);
                tmp4 |= 1 << 5;
            }

            tmp4 &= ~(0x0f << 0);
            if(charge->charge_current <100) //此时关掉充电
			{
				tmp4 &= 0x7f;
			}
			else if(charge->charge_current <150)//change_current set to 100
			{
				tmp4 |= 0x00;
			}
			else if(charge->charge_current <250) //change_current set to 190
			{
				tmp4 |= 0x01;
			}
            else if(charge->charge_current <300) //change_current set to 280
			{
				tmp4 |= 0x02;
			}
			else if(charge->charge_current <400) //change_current set to 360
			{
				tmp4 |= 0x03;
			}
			else if(charge->charge_current <500) //change_current set to 450
			{
				tmp4 |= 0x04;
			}
			else if(charge->charge_current <600) //change_current set to 550
			{
				tmp4 |= 0x05;
			}
			else if(charge->charge_current <650) //change_current set to 630
			{
				tmp4 |= 0x06;
			}
			else if(charge->charge_current <750) //change_current set to 700
			{
				tmp4 |= 0x07;
			}
			else if(charge->charge_current <800) //change_current set to 780
			{
				tmp4 |= 0x08;
			}
			else if(charge->charge_current <900) //change_current set to 880
			{
				tmp4 |= 0x09;
			}
			else if(charge->charge_current <970) //change_current set to 960
			{
				tmp4 |= 0x0a;
			}
			else if(charge->charge_current <1050) //change_current set to 1000
			{
				tmp4 |= 0x0b;
			}
			else if(charge->charge_current <1100) //change_current set to 1080
			{
				tmp4 |= 0x0c;
			}
			else if(charge->charge_current <1200) //change_current set to 1160
			{
				tmp4 |= 0x0d;
			}
			else if(charge->charge_current <1250) //change_current set to 1240
			{
				tmp4 |= 0x0e;
			}
			else  //change_current set to 1320
			{
				tmp4 |= 0x0f;
			}


            if(charge->charge_timer1 == 30)
            {
                tmp5 &= (~(3 << 6));
                tmp5 |= (0 << 6);
            }
            else if(charge->charge_timer1 == 40)
            {
                tmp5 &= (~(3 << 6));
                tmp5 |= (1 << 6);
            }
            else if(charge->charge_timer1 == 50)
            {
                tmp5 &= (~(3 << 6));
                tmp5 |= (2 << 6);
            }
            else if(charge->charge_timer1 == 60)
            {
                tmp5 &= (~(3 << 6));
                tmp5 |= (3 << 6);
            }
            if(charge->charge_timer2 == 420)
            {
                tmp5 &= (~(3 << 0));
                tmp5 |= (0 << 0);
            }
            else if(charge->charge_timer2 == 480)
            {
                tmp5 &= (~(3 << 0));
                tmp5 |= (1 << 0);
            }
            else if(charge->charge_timer2 == 540)
            {
                tmp5 &= (~(3 << 0));
                tmp5 |= (2 << 0);
            }
            else if(charge->charge_timer2 == 600)
            {
                tmp5 &= (~(3 << 0));
                tmp5 |= (3 << 0);
            }
            hstr.data       = &tmp4;
            byte_addr  = POWER19_CHARGE1;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }
            // for debug 20110718 add by zhwj
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }
//						__wrn("AXP19x REG33 = %x\n",tmp4);
//						__wrn("AXP19x REG34 = %x\n",tmp5);
            hstr.data       = &tmp5;
            byte_addr  = POWER19_CHARGE2;
            hstr.byte_addr  = &byte_addr;
            ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

            break;
        }

        case DRV_POWER_CMD_BASIC_STATUS:
        {
            __drv_power_basic_status_t  *pbasic = (__drv_power_basic_status_t *)pbuffer;
            __u8       tmp1, tmp2, tmp3, tmp4;

            eLIBs_memset(pbasic, 0, sizeof(__drv_power_basic_status_t));

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER19_STATUS;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp1;
            hstr.byte_count         = 1;  // data is 1 bytes

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            hstr.data       = &tmp2;
            byte_addr  = POWER19_IPS_SET;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            hstr.data       = &tmp3;
            byte_addr  = POWER19_MODE_CHGSTATUS;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            hstr.data       = &tmp4;
            byte_addr  = POWER19_CHARGE1;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            //pbasic->usb_charge  = usb_charge_exist;
            //pbasic->acin_input  = (tmp1 >> 7) & 0x01;
            pbasic->vout_exist  = ((tmp1 >> 7) & (tmp1 >> 5) ) & 0x01;
            pbasic->vout_enable = ((tmp1 >> 6) & (tmp1 >> 4)) & 0x01;
            pbasic->vout_choose = (tmp2 >> 7) & 0x01;
            pbasic->bat_exist   = (tmp3 >> 5) & 0x01;
            pbasic->bat_work_status   = (tmp3 >> 6) & 0x01;
            pbasic->bat_charge_status = (tmp1 >> 2) & 0x01;

            if(!pbasic->bat_charge_status)         //在充电
            {
                if((tmp1 >> 7) & 0x01)            //园口充电
                {
                    pbasic->charge_source = 0x01;
                }
                else
                {
                    pbasic->charge_source = 0x02 | (usb_charge_exist & 0x01);
                }
            }
            else
            {
                pbasic->charge_source = 0;
            }

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_IPS_GET:
        {
            __drv_power_ips_status_t  *pips = (__drv_power_ips_status_t *)pbuffer;

            eLIBs_memset(pips, 0, sizeof(__drv_power_ips_status_t));
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER19_IPS_SET;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 bytes

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            pips->dcin_choose  = ((tmp_value >> 7) & 0x01);
            pips->vhold_mode   = ((tmp_value >> 6) & 0x01);
            pips->vhold        = 4000 + 100 * ((tmp_value >> 3) & 0x07);

            byte_addr  = POWER19_APS_WARNING1;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            pips->vwarning     = 2867 +  5.6 * tmp_value;
            byte_addr  = POWER19_APS_WARNING2;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            pips->voff         = 2867 +  5.6 * tmp_value;

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_IPS_SET:
        {
            __drv_power_ips_status_t  *pips = (__drv_power_ips_status_t *)pbuffer;

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER19_IPS_SET;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 bytes

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            if(!(pips->dcin_choose & 0x02))
            {
                tmp_value &= ~(1 << 7);
                tmp_value |= pips->dcin_choose << 7;
            }
            if(!(pips->vhold_mode & 0x02))
            {
                tmp_value &= ~(1 << 6);
                tmp_value |= pips->vhold_mode << 6;
            }
            if((pips->vhold > 3900) && (pips->vhold < 4800))
            {
                tmp_value &= ~(0x07 << 3);
                tmp_value |= ((pips->vhold - 4000)/100) << 3;
            }
            eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

            if((pips->vwarning > 2866) && (pips->vwarning < 4000))
            {
                tmp_value = ((pips->vwarning - 2867)/5.6);
            }
            byte_addr  = POWER19_APS_WARNING1;
            hstr.byte_addr  = &byte_addr;
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

            if((pips->voff > 2866) && (pips->voff < 4000))
            {
                tmp_value = ((pips->voff - 2867)/5.6);
            }
            byte_addr  = POWER19_APS_WARNING2;
            hstr.byte_addr  = &byte_addr;
            eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            ret = EPDK_OK;

            break;
        }

        case DRV_POWER_CMD_GET_COULOMB:
        {
            __u8 temp[8];
            __u8  adc_freq,IC_type;
            __u32 rValue1,rValue2,rValue;
            eLIBs_memset(temp, 0, 8);

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER19_ADC_SPEED;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = temp;
            hstr.byte_count         = 1;  // data is 1 bytes

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
//            __inf("Cur_CoulombCounter  temp[0] = %d!\n",temp[0]);
            if(((temp[0] & 0xc0) >> 6) == 0)
            {
                adc_freq = 25;
            }
            else if(((temp[0] & 0xc0) >> 6) == 1)
            {
                adc_freq = 50;
            }
            else if(((temp[0] & 0xc0) >> 6) == 2)
            {
                adc_freq = 100;
            }
            else
            {
                adc_freq = 200;
            }
            hstr.byte_count = 1;
            hstr.data = temp;
            byte_addr =POWER19_IC_TYPE;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            if((temp[0] & 0x0f) == 0x03)
          	{
          		IC_type = 1;
          	}
          	else
        		{
        			IC_type = 0;
        		}

            __inf("==========================Cur_CoulombCounter  IC_type = %d!\n",IC_type);
            hstr.byte_count = 8;
            hstr.data = temp;
            byte_addr = POWER19_BAT_CHGCOULOMB3;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            if(IC_type)
            {
	            rValue1 = 65536*((temp[0] << 24) + (temp[1] << 16) + (temp[2] << 8) + temp[3]);
	            rValue2 = 65536*((temp[4] << 24) + (temp[5] << 16) + (temp[6] << 8) + temp[7]);
          	}
          	else
          	{
          		rValue1 = ((temp[0] << 24) + (temp[1] << 16) + (temp[2] << 8) + temp[3]);
	            rValue2 = ((temp[4] << 24) + (temp[5] << 16) + (temp[6] << 8) + temp[7]);
	          }
//            __wrn("Cur_CoulombCounter rValue1 = %d!\n",rValue1);
//            __wrn("Cur_CoulombCounter rValue2 = %d!\n",rValue2);
            if(rValue1 > rValue2)
            {
                coulomb_flag = 1;
                rValue = rValue1 - rValue2 ;
            }
            else
            {
                coulomb_flag = 0;
                rValue = rValue2 - rValue1 ;
            }

            ret = rValue /adc_freq / 3600 / 2;
            break;
        }

        case DRV_POWER_CMD_GET_DCIN:
        {
            __drv_power_battery_status_t3   *pstatus = (__drv_power_battery_status_t3 *)pbuffer;

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER19_STATUS;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 bytes

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            pstatus->acin_en = (tmp_value >> 7) & 0x01;
            pstatus->dcin_valid = ((tmp_value >> 4) & 0x01) | ((tmp_value >> 6) & 0x01);

            byte_addr  = POWER19_MODE_CHGSTATUS;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            pstatus->charge_status = (tmp_value >> 6) & 0x01;

            ret = EPDK_OK;
            break;
        }
        case DRV_POWER_CMD_GET_FUELGUAGE:
        {
            __drv_power_fuelguage_t   *pbat = (__drv_power_fuelguage_t *)pbuffer;

            pbat->charge_status = fuelguage.charge_status;
//            __inf("DRV_POWER_CMD_GET_FUELGUAGE Fuelguage.charge_status = %d!\n",fuelguage.charge_status);
            pbat->rest_vol = fuelguage.rest_vol;
            pbat->rest_time = fuelguage.rest_time;

            rest_time_cal_19(aux);
            pbat->rest_time_music = fuelguage.rest_time_music;
//            __inf("DRV_POWER_CMD_GET_FUELGUAGE Fuelguage.rest_time_music = %d!\n",fuelguage.rest_time_music);
            pbat->rest_time_video = fuelguage.rest_time_video;
//            __inf("DRV_POWER_CMD_GET_FUELGUAGE Fuelguage.rest_time_video = %d!\n",fuelguage.rest_time_video);
            pbat->rest_time_pic = fuelguage.rest_time_pic;
//            __inf("DRV_POWER_CMD_GET_FUELGUAGE Fuelguage.rest_time_pic = %d!\n",fuelguage.rest_time_pic);
            pbat->rest_time_book = fuelguage.rest_time_book;
//            __inf("DRV_POWER_CMD_GET_FUELGUAGE Fuelguage.rest_time_book = %d!\n",fuelguage.rest_time_book);
            pbat->rest_time_standby = fuelguage.rest_time_standby;
//            __inf("DRV_POWER_CMD_GET_FUELGUAGE Fuelguage.rest_time_standby = %d!\n",fuelguage.rest_time_standby);

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_SET_USB_CHARGE:
        {
            usb_charge_exist = ((__u32)pbuffer) & 0xff;

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_SET_STANDBY_FLAG:
        {
            __inf("set sdandby to  1 \n");
            standby = 1;
	  		ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_GET_PMU_TYPE:
        {
            //__inf("set sdandby to  1 \n");
            ret = PMU_type;
            break;
        }
		case DRV_POWER_CMD_SET_POWERON_TIME:
		{
			__u8 reg =(__u8 ) aux;
			
			hstr.slave_addr		 = POWER_BOARD1_DEV_ADDR_19;
			hstr.slave_addr_flag	 = TWI_SLAVE_ADDRESS_07BIT;
			hstr.byte_addr_width	 = 1;  // reg_addr is 1 byte
			hstr.data				 = &tmp_value;
			hstr.byte_count		 = 1;  // data is 1 byte
			
			byte_addr	= POWER19_POK_SET;
			hstr.byte_addr  = &byte_addr;
			if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
			{
				 break;
			}
			tmp_value &= 0x3f;
			tmp_value |= reg; 						
			eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
			
			break;
		} //added by zhangdonglu 110520
		case DRV_POWRE_CMD_BL_CTRL:
		{
			
			__u8 reg =(__u8 ) aux;
			__wrn("start control BL!\n");
			hstr.slave_addr		 = POWER_BOARD1_DEV_ADDR_19;
			hstr.slave_addr_flag	 = TWI_SLAVE_ADDRESS_07BIT;
			hstr.byte_addr_width	 = 1;  // reg_addr is 1 byte
			tmp_value = 0x16;
			hstr.data				 = &tmp_value;
			hstr.byte_count		 = 1;  // data is 1 byte
			byte_addr	= POWER19_PWM2_DUTY;
			hstr.byte_addr  = &byte_addr;
			if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
			{
				 break;
			}
			tmp_value = 0x16;
			hstr.data				 = &tmp_value;
			hstr.byte_count		 = 1;  // data is 1 byte
			byte_addr	= POWER19_PWM2_DUTYDE;
			hstr.byte_addr  = &byte_addr;
			if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
			{
				 break;
			}
			if(reg == POWER_OPEN)
			{
				hstr.slave_addr		 = POWER_BOARD1_DEV_ADDR_19;
				hstr.slave_addr_flag	 = TWI_SLAVE_ADDRESS_07BIT;
				hstr.byte_addr_width	 = 1;  // reg_addr is 1 byte
				tmp_value = 0x02;
				hstr.data				 = &tmp_value;
				hstr.byte_count		 = 1;  // data is 1 byte
				byte_addr	= POWER19_GPIO1_CTL;
				hstr.byte_addr  = &byte_addr;
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
				{
					 __wrn("read GPIO2 fail!\n");
					 break;
				}
				 __wrn("Set GPIO2 to PWM OK!\n");
			}
			else if(reg == POWER_CLOSE)
			{
				hstr.slave_addr		 = POWER_BOARD1_DEV_ADDR_19;
				hstr.slave_addr_flag	 = TWI_SLAVE_ADDRESS_07BIT;
				hstr.byte_addr_width	 = 1;  // reg_addr is 1 byte
				tmp_value = 0x07;
				hstr.data				 = &tmp_value;
				hstr.byte_count		 = 1;  // data is 1 byte
				byte_addr	= POWER19_GPIO1_CTL;
				hstr.byte_addr  = &byte_addr;
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
				{
					 __wrn("read GPIO2 fail!\n");
					 break;
				}
			}
			break;
		}
		case DRV_POWER_CMD_EXTCHG_CTRL://外部充电通路控制
		{
			__u8 reg =(__u8 ) aux;
			if(reg == POWER_OPEN)
			{
				hstr.slave_addr		 = POWER_BOARD1_DEV_ADDR_19;
				hstr.slave_addr_flag	 = TWI_SLAVE_ADDRESS_07BIT;
				hstr.byte_addr_width	 = 1;  // reg_addr is 1 byte
				hstr.data				 = &tmp_value;
				hstr.byte_count		 = 1;  // data is 1 byte
				byte_addr	= POWER19_CHARGE2;
				hstr.byte_addr  = &byte_addr;
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
				{
					 break;
				}
				tmp_value &= 0xfb;
				tmp_value |= 0x04;
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
				{
					 break;
				}
			}
			else if(reg == POWER_CLOSE)
			{
				hstr.slave_addr		 = POWER_BOARD1_DEV_ADDR_19;
				hstr.slave_addr_flag	 = TWI_SLAVE_ADDRESS_07BIT;
				hstr.byte_addr_width	 = 1;  // reg_addr is 1 byte
				hstr.data				 = &tmp_value;
				hstr.byte_count		 = 1;  // data is 1 byte
				byte_addr	= POWER19_CHARGE2;
				hstr.byte_addr  = &byte_addr;
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
				{
					 break;
				}
				tmp_value &= 0xfb;
				tmp_value |= 0x00;
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
				{
					 break;
				}
			}
			break;
		}
		case DRV_POWER_CMD_SET_EXTCHG_CURR:
		{
			__u32 tmp;
			tmp = (__u32) pbuffer;
			hstr.slave_addr		 = POWER_BOARD1_DEV_ADDR_19;
			hstr.slave_addr_flag	 = TWI_SLAVE_ADDRESS_07BIT;
			hstr.byte_addr_width	 = 1;  // reg_addr is 1 byte
			hstr.byte_count		 = 1;  // data is 1 byte
			hstr.data				 = &tmp_value;
			byte_addr	= POWER19_CHARGE2;
			hstr.byte_addr  = &byte_addr;
			if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
			{
				break;
			}
			if(tmp == 300)
			{
				tmp_value &= 0xc7;
				tmp_value |= 0x00;
				hstr.data				 = &tmp_value;
				byte_addr	= POWER19_CHARGE2;
				hstr.byte_addr  = &byte_addr;
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
				{
				 	break;
				}	
			}
			else if(tmp == 400)
			{
				tmp_value &= 0xc7;
				tmp_value |= 0x08;
				hstr.data				 = &tmp_value;
				byte_addr	= POWER19_CHARGE2;
				hstr.byte_addr  = &byte_addr;
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
				{
				 	break;
				}	
			}
			else if(tmp == 500)
			{
				tmp_value &= 0xc7;
				tmp_value |= 0x10;
				hstr.data				 = &tmp_value;
				byte_addr	= POWER19_CHARGE2;
				hstr.byte_addr  = &byte_addr;
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
				{
				 	break;
				}	
			}
			else if(tmp == 600)
			{
				tmp_value &= 0xc7;
				tmp_value |= 0x18;
				hstr.data				 = &tmp_value;
				byte_addr	= POWER19_CHARGE2;
				hstr.byte_addr  = &byte_addr;
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
				{
				 	break;
				}	
			}
			else if(tmp == 700)
			{
				tmp_value &= 0xc7;
				tmp_value |= 0x20;
				hstr.data				 = &tmp_value;
				byte_addr	= POWER19_CHARGE2;
				hstr.byte_addr  = &byte_addr;
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
				{
				 	break;
				}	
			}
			else if(tmp == 800)
			{
				tmp_value &= 0xc7;
				tmp_value |= 0x28;
				hstr.data				 = &tmp_value;
				byte_addr	= POWER19_CHARGE2;
				hstr.byte_addr  = &byte_addr;
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
				{
				 	break;
				}	
			}
			else if(tmp == 900)
			{
				tmp_value &= 0xc7;
				tmp_value |= 0x30;
				hstr.data		= &tmp_value;
				byte_addr		= POWER19_CHARGE2;
				hstr.byte_addr  = &byte_addr;
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
				{
				 	break;
				}	
			}
			else if(tmp == 1000)
			{
				tmp_value &= 0xc7;
				tmp_value |= 0x38;
				hstr.data		= &tmp_value;
				byte_addr		= POWER19_CHARGE2;
				hstr.byte_addr  = &byte_addr;
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
				{
				 	break;
				}	
			}
			else 
			{
				__wrn("This current can not be set!/n");
			}
			//for debug
			hstr.data		= &tmp_value;
			byte_addr		= POWER19_CHARGE2;
			hstr.byte_addr  = &byte_addr;
			if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
			{
				break;
			}
			__wrn("AXP19x REG34 = %x\n",tmp_value);
			break;
		}
		case DRV_POWER_CMD_SET_VBUS_CURR:
		{
			__u8 reg =(__u8 ) aux;
			
			hstr.slave_addr		    = POWER_BOARD1_DEV_ADDR_19;
			hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
			hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
			hstr.data               = &tmp_value;
			hstr.byte_count         = 1;  // data is 1 byte
			
			byte_addr	= POWER19_IPS_SET;
			hstr.byte_addr  = &byte_addr;
			if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
			{
				 break;
			}
			tmp_value &= 0xfc;
			tmp_value |= reg; 						
			eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
			
			break;
		}
		case DRV_POWER_CMD_SET_LONGKEY_TIME:
		{
			__u8 reg =(__u8 ) aux;
			
			hstr.slave_addr		 = POWER_BOARD1_DEV_ADDR_19;
			hstr.slave_addr_flag	 = TWI_SLAVE_ADDRESS_07BIT;
			hstr.byte_addr_width	 = 1;  // reg_addr is 1 byte
			hstr.data				 = &tmp_value;
			hstr.byte_count		 = 1;  // data is 1 byte
			
			byte_addr	= POWER19_POK_SET;
			hstr.byte_addr  = &byte_addr;
			if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
			{
				 break;
			}
			tmp_value &= 0xcf;
			tmp_value |= reg; 						
			eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
			
			break;
		} //added by zhwj 111008
		//add...
		case DRV_POWER_CMD_PWM_SET:
		{
			__drv_power_pwm_t  *pwm = (__drv_power_pwm_t *)pbuffer;
			__u32 para_x = 0,para_y = 0,para_z = 0;
			hstr.slave_addr		 = POWER_BOARD1_DEV_ADDR_19;
			hstr.slave_addr_flag	 = TWI_SLAVE_ADDRESS_07BIT;
			hstr.byte_addr_width	 = 1;  // reg_addr is 1 byte
			hstr.data				 = &tmp_value;
			hstr.byte_count		 = 1;  // data is 1 byte
			if(aux ==PWM1)
			{
				byte_addr	= POWER19_GPIO1_CTL;
				hstr.byte_addr  = &byte_addr;
				tmp_value = 0x02; 	 //set GPIO1 as PWM	1				
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
				{
					break;
				}
				para_y = 2250000/(pwm->frequency);
				if(para_y >= 255)
				{
					para_x = para_y/255;
					para_z  =255*(pwm->duty)/100;
					byte_addr	= POWER19_PWM1_DUTYDE; //set the duty denominator
					hstr.byte_addr  = &byte_addr;
					tmp_value = 0xff; 						
					if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
					{
						break;
					}
					byte_addr	= POWER19_PWM1_FREQ; //set the frequency control
					hstr.byte_addr  = &byte_addr;
					tmp_value = para_x; 						
					if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
					{
						break;
					}
					byte_addr	= POWER19_PWM1_DUTY; //set the duty numerator
					hstr.byte_addr  = &byte_addr;
					tmp_value = para_z; 						
					if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
					{
						break;
					}
				}
				else
				{
					para_x = 0x00;
					para_z = para_y*(pwm->duty)/100;
					byte_addr	= POWER19_PWM1_DUTYDE; //set the duty denominator
					hstr.byte_addr  = &byte_addr;
					tmp_value = para_y; 						
					if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
					{
						break;
					}
					byte_addr	= POWER19_PWM1_FREQ; //set the frequency control
					hstr.byte_addr  = &byte_addr;
					tmp_value = para_x; 						
					if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
					{
						break;
					}
					byte_addr	= POWER19_PWM1_DUTY; //set the duty numerator
					hstr.byte_addr  = &byte_addr;
					tmp_value = para_z; 						
					if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
					{
						break;
					}
				}
			}
			else if(aux == PWM2)
			{
				byte_addr	= POWER19_GPIO2_CTL;
				hstr.byte_addr  = &byte_addr;
				tmp_value = 0x02; 	 //set GPIO2 as PWM2				
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
				{
					break;
				}
				para_y = 2250000/(pwm->frequency);
				if(para_y >= 255)
				{
					para_x = para_y/255;
					para_z  =255*(pwm->duty)/100;
					byte_addr	= POWER19_PWM2_DUTYDE; //set the duty denominator
					hstr.byte_addr  = &byte_addr;
					tmp_value = 0xff; 						
					if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
					{
						break;
					}
					byte_addr	= POWER19_PWM2_FREQ; //set the frequency control
					hstr.byte_addr  = &byte_addr;
					tmp_value = para_x; 						
					if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
					{
						break;
					}
					byte_addr	= POWER19_PWM2_DUTY; //set the duty numerator
					hstr.byte_addr  = &byte_addr;
					tmp_value = para_z; 						
					if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
					{
						break;
					}
				}
				else
				{
					para_x = 0x00;
					para_z = para_y*(pwm->duty)/100;
					byte_addr	= POWER19_PWM2_DUTYDE; //set the duty denominator
					hstr.byte_addr  = &byte_addr;
					tmp_value = para_y; 						
					if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
					{
						break;
					}
					byte_addr	= POWER19_PWM2_FREQ; //set the frequency control
					hstr.byte_addr  = &byte_addr;
					tmp_value = para_x; 						
					if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
					{
						break;
					}
					byte_addr	= POWER19_PWM2_DUTY; //set the duty numerator
					hstr.byte_addr  = &byte_addr;
					tmp_value = para_z; 						
					if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
					{
						break;
					}
				}
			}
			else
			{
				__wrn("Unkonwn PWM channel...\n");
        ret = EPDK_FAIL;	
			}
			ret = EPDK_OK;
			
			break;	
		}
		//add...
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

__dev_devop_t power_19_dev_ops =
{
    DEV_Power_19_Open,
    DEV_Power_19_Close,
    DEV_Power_19_Read,
    DEV_Power_19_Write,
    DEV_Power_19_Ioctrl
};
