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
__u8        AXP209_Version = 0;
__u16       AXP20x_dc2_target = 0;

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

void rest_time_cal_20 (__u8 lcd_level)
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
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 2:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level2;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level2;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level2;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 3:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level3;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level3;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level3;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 4:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level4;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level4;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level4;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 5:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level5;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level5;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level5;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 6:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level6;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level6;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level6;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 7:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level7;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level7;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level7;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 8:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level8;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level8;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level8;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 9:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level9;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level9;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level9;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
 //           __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 10:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level10;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level10;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level10;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 11:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level11;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level11;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level1;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 12:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level12;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level12;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level12;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 13:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level13;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level13;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level13;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 14:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level14;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level14;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level14;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 15:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level15;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level15;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level15;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 16:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level16;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level16;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level16;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 17:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level17;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level17;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level17;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 18:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level18;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level18;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level18;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 19:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level19;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level19;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level19;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
            break;
        }
        case 20:
        {
            fuelguage.rest_time_video = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->videocurrent.level20;
            fuelguage.rest_time_pic = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->picturecurrent.level20;
            fuelguage.rest_time_book = 60 * (fuelguage.rest_vol - 2) * fuelguage.bat_cap / 100 / pP1CurrentCfg->ebookcurrent.level20;
//            __inf("p1 rest_time_video %d!\n",fuelguage.rest_time_video);
//            __inf("p1 rest_time_pic %d!\n",fuelguage.rest_time_pic);
//            __inf("p1 rest_time_book %d!\n",fuelguage.rest_time_book);
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
__hdle DEV_Power_20_Open(void * open_arg, __u32 mode)
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
__s32 DEV_Power_20_Close(__hdle hPower)
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
__u32 DEV_Power_20_Read(void *pdata, __u32 size, __u32 n, __hdle hPower)
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
__u32 DEV_Power_20_Write(const void *pdata, __u32 size, __u32 n, __hdle hPower)
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
__s32 DEV_Power_20_Ioctrl(__hdle hPower, __u32 cmd, __s32 aux, void *pbuffer)
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
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
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
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
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
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER20_OFF_CTL;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            tmp_value |=  1<<7 ;
            ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

            break;
        }

        case DRV_POWER_CMD_INT_ENABLE:
        {
            __u8  temp;

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte

            temp = (aux >> 0) & 0xff;
            if(temp)
            {
                hstr.byte_count = 1;
                hstr.data       = &tmp_value;
                byte_addr  = POWER20_INTEN1;
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
                byte_addr  = POWER20_INTEN2;
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
                byte_addr  = POWER20_INTEN3;
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
                byte_addr  = POWER20_INTEN4;
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
            
            temp = (aux >> 31) & 0xff;
            if(temp)
            {
                hstr.byte_count = 1;
                hstr.data       = &tmp_value;
                byte_addr  = POWER20_INTEN5;
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

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte

            temp = (aux >> 0) & 0xff;
            if(temp)
            {
                hstr.byte_count = 1;
                hstr.data       = &tmp_value;
                byte_addr  = POWER20_INTEN1;
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
                byte_addr  = POWER20_INTEN2;
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
                byte_addr  = POWER20_INTEN3;
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
                byte_addr  = POWER20_INTEN4;
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
            temp = (aux >> 31) & 0xff;
            if(temp)
            {
                hstr.byte_count = 1;
                hstr.data       = &tmp_value;
                byte_addr  = POWER20_INTEN5;
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

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.byte_count         = 1;  // data is 1 byte
            
            byte_addr          = POWER20_INTEN1;
            hstr.byte_addr  = &byte_addr;
            hstr.data               = &tmp_value;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                __inf("p1 POWER20_INTEN1 error!\n");
                break;
            }
            __inf("p1 POWER20_INTEN1 %d!\n",tmp_value);
            
            byte_addr  = POWER20_INTEN2;
            hstr.byte_addr  = &byte_addr;
            hstr.data       =  &tmp_value;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                __inf("p1 POWER20_INTEN2 error!\n");
                break;
            }
            __inf("p1 POWER20_INTEN2 %d!\n",tmp_value);
            
            byte_addr  = POWER20_INTEN3;
            hstr.byte_addr  = &byte_addr;
            hstr.data       =  &tmp_value;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                __inf("p1 POWER20_INTEN31 error!\n");
                break;
            }
            __inf("p1 POWER20_INTEN3 %d!\n",tmp_value);
            
            byte_addr  = POWER20_INTEN4;
            hstr.byte_addr  = &byte_addr;
            hstr.data       =  &tmp_value;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                __inf("p1 POWER20_INTEN4 error!\n");
                break;
            }
            __inf("p1 POWER20_INTEN4 %d!\n",tmp_value);
            
             byte_addr  = POWER20_INTEN5;
            hstr.byte_addr  = &byte_addr;
            hstr.data       =  &tmp_value;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                __inf("p1 POWER20_INTEN5 error!\n");
                break;
            }
            __inf("p1 POWER20_INTEN5 %d!\n",tmp_value);

            byte_addr  = POWER20_INTSTS1;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = data + 0;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                __inf("p1 int query1 error!\n");
                break;
            }
            __inf("data0 %d!\n", data[0]);
            __inf("p1 int query1!\n");

            byte_addr  = POWER20_INTSTS2;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = data + 1;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            __inf("data1 %d!\n", data[1]);

            __inf("p1 int query2!\n");
            byte_addr  = POWER20_INTSTS3;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = data + 2;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            __inf("data2 %d!\n", data[2]);
            __inf("p1 int query3!\n");
            byte_addr  = POWER20_INTSTS4;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = data + 3;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            __inf("data3 %d!\n", data[3]);
            __inf("p1 int query4!\n");
            
            byte_addr  = POWER20_INTSTS5;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = data + 4;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            __inf("data4 %d!\n", data[4]);
            __inf("p1 int query5!\n");
            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_INT_CLEAR:
        {
            tmp_value = *((__u8 *)pbuffer + 0);

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER20_INTSTS1;
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
            byte_addr  = POWER20_INTSTS2;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = &tmp_value;
            __inf("p1 int clear intsts2 = %d!\n",tmp_value);
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }

            tmp_value = *((__u8 *)pbuffer + 2);
            byte_addr  = POWER20_INTSTS3;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = &tmp_value;
            __inf("p1 int clear intsts3 = %d!\n",tmp_value);
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }

            tmp_value = *((__u8 *)pbuffer + 3);
            byte_addr  = POWER20_INTSTS4;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = &tmp_value;
            __inf("p1 int clear intsts4 = %d!\n",tmp_value);
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }
            
            tmp_value = *((__u8 *)pbuffer + 4);
            byte_addr  = POWER20_INTSTS5;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = &tmp_value;
            __inf("p1 int clear intsts5 = %d!\n",tmp_value);
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_VOUT_GET:
        {
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.byte_count         = 1;  // data is 1 byte

            if(aux == POWER_VOL_LDO1)
            {
                ret = 1250;
            }
            else if(aux == POWER_VOL_DCDC1)
            {
               __inf("AXP20x has no DCDC1!\n");
            }
            else if(aux == POWER_VOL_DCDC2)
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER20_DC2OUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret = 700 + tmp_value  * 25;
                if((ret <= (AXP20x_dc2_target + 25))  &&  (ret >= (AXP20x_dc2_target)))
                    ret = AXP20x_dc2_target;
                __inf("Voltage Scale! Vout_Getdc2_%d\n",ret);
            }
            else if(aux == POWER_VOL_DCDC3)
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER20_DC3OUT_VOL;
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
                byte_addr  = POWER20_GPIO0_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                
                if((tmp_value & 0x07) == 0x00)
              	{
              		ret = 1800; 
              	}
              	else if((tmp_value & 0x07) == 0x01)
            		{
            			ret = 2500;
            		}
            		else if((tmp_value & 0x07) == 0x02)
            		{
            			ret = 2800;
            		}
            		else if((tmp_value & 0x07) == 0x03)
            		{
            			ret = 3000;
            		}
            		else if((tmp_value & 0x07) == 0x04)
            		{
            			ret = 3100;
            		}
            		else if((tmp_value & 0x07) == 0x05)
            		{
            			ret = 3300;
            		}
            		else if((tmp_value & 0x07) == 0x06)
            		{
            			ret = 3400;
            		}
            		else if((tmp_value & 0x07) == 0x07)
            		{
            			ret = 3500;
            		}
            }
            else if(aux == POWER_VOL_LDO2)
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER20_LDO24OUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret = 1800 + ((tmp_value >> 4) & 0x0f) * 100;
            }
            else if(aux == POWER_VOL_LDO3)
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER20_LDO3OUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret = 700 + tmp_value  * 25;
            }
						else if(aux == POWER_VOL_LDO4)
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER20_LDO24OUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                if((tmp_value&0x0f) == 0x00)
                {
                	ret = 1250;
                }
                else if((tmp_value&0x0f) == 0x01)
                {
                	ret = 1300;
                }
                else if((tmp_value&0x0f) == 0x02)
                {
                	ret = 1400;
                }
                else if((tmp_value&0x0f) == 0x03)
                {
                	ret = 1500;
                }
                else if((tmp_value&0x0f) == 0x04)
                {
                	ret = 1600;
                }
                else if((tmp_value&0x0f) == 0x05)
                {
                	ret = 1700;
                }
                else if((tmp_value&0x0f) == 0x06)
                {
                	ret = 1800;
                }
                else if((tmp_value&0x0f) == 0x07)
                {
                	ret = 1900;
                }
                else if((tmp_value&0x0f) == 0x08)
                {
                	ret = 2000;
                }
                else if((tmp_value&0x0f) == 0x09)
                {
                	ret = 2500;
                }
                else if((tmp_value&0x0f) == 0x0a)
                {
                	ret = 2700;
                }
                else if((tmp_value&0x0f) == 0x0b)
                {
                	ret = 2800;
                }
                else if((tmp_value&0x0f) == 0x0c)
                {
                	ret = 3000;
                }
                else if((tmp_value&0x0f) == 0x0d)
                {
                	ret = 3100;
                }
                else if((tmp_value&0x0f) == 0x0e)
                {
                	ret = 3200;
                }
                else if((tmp_value&0x0f) == 0x0f)
                {
                	ret = 3300;
                }
                
            }
            else if(aux == POWER_VOL_GPIO0)
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER20_GPIO0_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret = 1800 + ((tmp_value >> 4) & 0xf0) * 100;
            }

            else if((aux == POWER_VOL_LDO5))                        //接受POWER_VOL_LDO5 当GPIO0处理，做VMIC
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER20_GPIO0_VOL;
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

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.byte_count = 1;

            __wrn("Voltage Scale! Vout_Set\n");
            if(aux == POWER_VOL_DCDC1 )
            {
                __inf("AXP209 has no DCDC1! can not be seted!");
            }

            if(aux == POWER_VOL_DCDC2 )
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER20_DC2OUT_VOL;
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

                    AXP20x_dc2_target = tmp;
                }
                else
                {
                    break;
                }
            }

            if(aux == POWER_VOL_DCDC3 )
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER20_DC3OUT_VOL;
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
                byte_addr  = POWER20_GPIO0_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                if(tmp == 1800)
                {
                    tmp_value &= 0xf8;
                    tmp_value |= 0x00 ;
                }
                else if(tmp == 2500)
                {
                    tmp_value &= 0xf8;
                    tmp_value |= 0x01 ;
                }
                else if(tmp == 2800)
                {
                    tmp_value &= 0xf8;
                    tmp_value |= 0x02 ;
                }
                else if(tmp == 3000)
                {
                    tmp_value &= 0xf8;
                    tmp_value |= 0x03 ;
                }
                else if(tmp == 3100)
                {
                    tmp_value &= 0xf8;
                    tmp_value |= 0x04 ;
                }
                else if(tmp == 3200)
                {
                    tmp_value &= 0xf8;
                    tmp_value |= 0x05 ;
                }
                else if(tmp == 3300)
                {
                    tmp_value &= 0xf8;
                    tmp_value |= 0x06 ;
                }
                else if(tmp == 3400)
                {
                    tmp_value &= 0xf8;
                    tmp_value |= 0x06 ;
                }
                else if(tmp == 3500)
                {
                    tmp_value &= 0xf8;
                    tmp_value |= 0x07 ;
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
            
            else if(aux == POWER_VOL_LDO2)
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER20_LDO24OUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                if((!(tmp%100)) && (tmp >= 1800) && (tmp <= 3300))
                {
                    tmp_value &= 0x0f;
                    tmp_value |= ((tmp - 1800)/100) ;
                }
                else
                {
                    break;
                }
                ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
			else if((aux == POWER_VOL_LDO3))
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER20_LDO3OUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

				//eLIBs_printf("-------  tmp = %d-------\n", tmp);
                if((!(tmp%25)) && (tmp >= 700) && (tmp <= 3500))
                {
                
					//eLIBs_printf("======1 ====  tmp_value  = %d ===========\n", tmp_value);
                    tmp_value &= 0x80;
					
					//eLIBs_printf("======2 ====  tmp_value  = %d ===========\n", tmp_value);
                    tmp_value = ((tmp - 700)/25) ;

					//eLIBs_printf("==========  tmp_value  = %d ===========\n", tmp_value);
                }   else
                {
                    break;
                }
                ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
            
            else if((aux == POWER_VOL_LDO5))                        //接受POWER_VOL_LDO5 当GPIO0处理，做VMIC
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER20_GPIO0_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                if((!(tmp%100)) && (tmp >= 1800) && (tmp <= 3300))
                {
                    tmp_value &= 0x0f;
                    tmp_value |= (((tmp - 1800)/100)  << 4);
                }
                else
                {
                    break;
                }
                ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
						else if(aux == POWER_VOL_LDO4)
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER20_LDO24OUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                if(tmp == 1250)
                {
                    tmp_value &= 0x0f;
                    tmp_value |= 0x00;
                }
                else if(tmp == 1300)
                {
                    tmp_value &= 0x0f;
                    tmp_value |= 0x01;
                }
                else if(tmp == 1400)
                {
                    tmp_value &= 0x0f;
                    tmp_value |= 0x02;
                }
                else if(tmp == 1500)
                {
                    tmp_value &= 0x0f;
                    tmp_value |= 0x03;
                }
                else if(tmp == 1600)
                {
                    tmp_value &= 0x0f;
                    tmp_value |= 0x04;
                }
                else if(tmp == 1700)
                {
                    tmp_value &= 0x0f;
                    tmp_value |= 0x05;
                }
                else if(tmp == 1800)
                {
                    tmp_value &= 0x0f;
                    tmp_value |= 0x06;
                }
                else if(tmp == 1900)
                {
                    tmp_value &= 0x0f;
                    tmp_value |= 0x07;
                }
                else if(tmp == 2000)
                {
                    tmp_value &= 0x0f;
                    tmp_value |= 0x08;
                }
                else if(tmp == 2500)
                {
                    tmp_value &= 0x0f;
                    tmp_value |= 0x09;
                }
                else if(tmp == 2700)
                {
                    tmp_value &= 0x0f;
                    tmp_value |= 0x0a;
                }
                else if(tmp == 2800)
                {
                    tmp_value &= 0x0f;
                    tmp_value |= 0x0b;
                }
                else if(tmp == 3000)
                {
                    tmp_value &= 0x0f;
                    tmp_value |= 0x0c;
                }
                else if(tmp == 3100)
                {
                    tmp_value &= 0x0f;
                    tmp_value |= 0x0d;
                }
                else if(tmp == 3200)
                {
                    tmp_value &= 0x0f;
                    tmp_value |= 0x0e;
                }
                else if(tmp == 3300)
                {
                    tmp_value &= 0x0f;
                    tmp_value |= 0x0f;
                }
                else
                {
                    break;
                }
                ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
          break;  
        }     
            
        case DRV_POWER_CMD_VSTS_GET:                    //Power Rail On/Off control
        {
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte

            hstr.byte_count = 1;
            hstr.data       = &tmp_value;

            if(aux == POWER_VOL_LDO1)
            {
                ret = 1;
            }
            else if((aux == POWER_VOL_DCDC1) || (aux == POWER_VOL_LDO4) || (aux == POWER_VOL_DCDC3) || (aux == POWER_VOL_LDO2))
            {
                byte_addr  = POWER20_OUTPUT_CTL;
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
                else if(aux == POWER_VOL_LDO4)
                {
                    ret = (tmp_value >> 3) & 0x01;
                }
                else
                {
                    ret = (tmp_value >> 2) & 0x01;
                }
            }
            else if((aux == POWER_VOL_DCDC2) || (aux == POWER_VOL_LDO3))
            {
                byte_addr  = POWER20_LDO3_DC2_CTL;
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
                byte_addr  = POWER20_GPIO0_CTL;
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
                byte_addr  = POWER20_GPIO1_CTL;
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

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.byte_count = 1;
            hstr.data       = &tmp_value;

            if((aux == POWER_VOL_DCDC1) || (aux == POWER_VOL_LDO4) || (aux == POWER_VOL_DCDC3) || (aux == POWER_VOL_LDO2))
            {
                byte_addr  = POWER20_OUTPUT_CTL;
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
                else if(aux == POWER_VOL_LDO4)
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
            else if((aux == POWER_VOL_DCDC2) || (aux == POWER_VOL_LDO3))
            {
                byte_addr  = POWER20_LDO3_DC2_CTL;
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
                    tmp_value &= 0xbf;
                    tmp_value |= (temp << 6);
                }
                ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
            else if(aux == POWER_VOL_LDO5)                    //接受POWER_VOL_LDO5 当GPIO0处理，做VMIC
            {
                byte_addr  = POWER20_GPIO0_CTL;
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
                byte_addr  = POWER20_GPIO1_CTL;
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
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.byte_count = 1;
            hstr.data       = &tmp_value;

            __inf("CMD_PWREPT_GET!\n");
            if(aux == POWER_EPT_1)
            {
                __inf("Control GPIO0_get\n");
                byte_addr  = POWER20_GPIO0_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value  &= 0xf8;
                tmp_value |= 0x02;                         //Set GPIO0 as input
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr  = POWER20_GPIO012_SIGNAL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret  |= ((tmp_value >> 4) & 0x01) << 2;
            }
            else if(aux == POWER_EPT_2)
            {
                __inf("Control GPIO1_get\n");
                byte_addr  = POWER20_GPIO1_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value  &= 0xf8;
                tmp_value |= 0x02;                         //Set GPIO1 as input
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr  = POWER20_GPIO012_SIGNAL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret  |= ((tmp_value >> 5) & 0x01) << 2;
            }
            else if(aux == POWER_EPT_3)
            {
                __inf("Control GPIO2_get_get\n");
                byte_addr  = POWER20_GPIO2_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value &= 0xf8;
                tmp_value |= 0x02;                         //Set GPIO2 as input
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr  = POWER20_GPIO012_SIGNAL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret  |= ((tmp_value >> 6) & 0x01) << 2;
            }
            else if(aux == POWER_EPT_4)
            {
                __inf("Control N_RSTO_get\n");
                byte_addr  = POWER20_GPIO3_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value  &= 0xfb;
                tmp_value |= 0x04;                         //Set GPIO3 as input
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                byte_addr  = POWER20_GPIO3_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret  |= ((tmp_value >> 0) & 0x01) << 2;
            }
            else 
          	{
          		__inf("AXP20x has no this GPIO!");
          	}
            break;
        }

        case DRV_POWER_CMD_PWREPT_SET:
        {
            __u32   temp;
            temp            = (__u32)pbuffer;

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte

            if(aux == POWER_EPT_1)
            {
                __msg("Control POWER_EPT_1 %d\n",temp);
                byte_addr  = POWER20_GPIO0_CTL;
                hstr.byte_addr  = &byte_addr;
                if(temp&0x01)
                {
	                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
	                {
	                    break;
	                }
	                tmp_value &= ~0x06;
	                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
	                
	                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
	                {
	                    break;
	                }
	                tmp_value |= 0x01;
	                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            	}
            	else
            	{
	                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
	                {
	                    break;
	                }
	                tmp_value &= ~0x03;
	                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            	}
            }
            else if(aux == POWER_EPT_2)
            {
                __msg("Control POWER_EPT_2 %d\n",temp);
                byte_addr  = POWER20_GPIO1_CTL;
                hstr.byte_addr  = &byte_addr;
                if(temp&0x01)
                {
	                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
	                {
	                    break;
	                }
	                tmp_value &= ~0x06;
	                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
	                
	                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
	                {
	                    break;
	                }
	                tmp_value |= 0x01;
	                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            	}
            	else
            	{
	                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
	                {
	                    break;
	                }
	                tmp_value &= ~0x03;
	                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            	}            
            }
            else if(aux == POWER_EPT_3)
            {
                __wrn("Control GPIO2_set\n");
                byte_addr  = POWER20_GPIO2_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
#if 0
                tmp_value &= 0xf8;
                tmp_value |= (((temp >> 2) & 0x01) << 0);
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
#else

                tmp_value &= ~0x06;
			/*
    			if( ((temp>>4)&0x01) == 0)	//input
    			{
    				tmp_value |= 0x02;
    			}
			*/
    			if( (temp&0x01) == 1 )	//output high
    			{
    				tmp_value |= 0x01;
    			}
			else
			{
    				tmp_value &= 0x00;
			}
                //__wrn("-----------------tmp_value = 0x%x,hstr.data = 0x%x\n",tmp_value,*hstr.data);
    			//else output low
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
#endif
            }
            else if(aux == POWER_EPT_4)
            {
                __wrn("Control BATSENSE_set\n");
                byte_addr  = POWER20_GPIO3_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
#if 0
                tmp_value &= 0xf9;
                tmp_value |= (((temp >> 2) & 0x01) << 0);
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
#else
                tmp_value &= ~0x06;
    			if( ((temp>>4)&0x01) == 0)	//input
    			{
    				tmp_value |= 0x04;
    			}
    			else if( (temp&0x01) == 1 )	//output high
    			{
    				tmp_value |= 0x02;
    			}
    			//else output low
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
#endif
            }
            else 
          	{
          		__inf("AXP20x has no this GPIO!");
          	}
            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_CHGSTAT_GET:
        {
            __drv_power_charge_stat     *Charge_Stat = (__drv_power_charge_stat *)pbuffer;
            __u8    temp_value[4];

            eLIBs_memset(Charge_Stat, 0, sizeof(__drv_power_charge_stat));

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER20_STATUS;
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
            byte_addr  = POWER20_BAT_AVERCHGCUR_H8;
            hstr.byte_addr  = &byte_addr;

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            Charge_Stat->battery_current = ((temp_value[0]<<4) + (temp_value[1]&0x0f) + (temp_value[2]<<5) + (temp_value[3]&0x1f))*0.5;
//            __inf("Charge_Stat->battery_current = %d\n",Charge_Stat->battery_current);

            hstr.byte_count = 2;
            hstr.data       = temp_value;
            byte_addr  = POWER20_BAT_AVERVOL_H8;//获取电池电压
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            Charge_Stat->battery_vol = ((temp_value[0]<<4) + (temp_value[1]&0x0f))*1.1;
//            __inf("Charge_Stat->battery_vol = %d\n",Charge_Stat->battery_vol);

            hstr.byte_count = 1;
            byte_addr  = POWER20_CHARGE1;  //获取充电电流设置值以及充电结束比率
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            Charge_Stat->Ichgset = _reg2test(temp_value[0] & 0x0f,AXP209_Version & 0x10);
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
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER20_MODE_CHGSTATUS;
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
                byte_addr  = POWER20_ADC_EN1;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                bat_sts->bat_voladc_enable  = ((adcsw >> 7) & 0x01);
                bat_sts->bat_crntadc_enable = ((adcsw >> 6) & 0x01);
                byte_addr  = POWER20_ADC_SPEED;
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
                    byte_addr  = POWER20_BAT_AVERVOL_H8;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    bat_sts->bat_vol =  (tmp_value << 4);
                    byte_addr  = POWER20_BAT_AVERVOL_L4;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    bat_sts->bat_vol +=  tmp_value ;
                    bat_sts->bat_vol *= 1.1;
                }

                byte_addr  = POWER20_STATUS;
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
                        byte_addr  = POWER20_BAT_AVERCHGCUR_H8;
                        hstr.byte_addr  = &byte_addr;
                        if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                        {
                            break;
                        }
                        bat_sts->bat_crnt = (tmp_value << 4);
                        byte_addr  = POWER20_BAT_AVERCHGCUR_L4;
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
                        byte_addr  = POWER20_BAT_AVERDISCHGCUR_H8;
                        hstr.byte_addr  = &byte_addr;
                        if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                        {
                            break;
                        }
                        bat_sts->bat_crnt = (tmp_value << 5);
                        byte_addr  = POWER20_BAT_AVERDISCHGCUR_L5;
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
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER20_ADC_SPEED;
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

            byte_addr  = POWER20_ADC_EN1;
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

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER20_STATUS;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            dcin_valid =(((tmp_value >> 6) & 0x01) | ((tmp_value >> 4) & 0x01));
            byte_addr  = POWER20_MODE_CHGSTATUS;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            bat_exist = ((tmp_value >> 5) & 0x01);
            if(!bat_exist)
            {
                __wrn("battery dosent exist\n");
                goto  _POWER_GET_RDC_;
            }
            if(dcin_valid)//充电
            {
                __u32   i,temp;
                __s32   averPreVol = 0, averPreCur = 0,averNextVol = 0,averNextCur = 0;
                __u32   batvol,batcrnt;

                for(i = 0; i< POWER_RDC_COUNT; i++)
                {
                    byte_addr  = POWER20_BAT_AVERVOL_H8;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    batvol =  (tmp_value << 4);
                    byte_addr  = POWER20_BAT_AVERVOL_L4;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    batvol += (tmp_value & 0x0f) ;
                    batvol *= 1.1;
                    averPreVol += batvol;

                    byte_addr  = POWER20_BAT_AVERCHGCUR_H8;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    batcrnt = (tmp_value << 4);
                    byte_addr  = POWER20_BAT_AVERCHGCUR_L4;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    batcrnt += (tmp_value & 0x0f);
                    batcrnt /= 2;
                    averPreCur += batcrnt;

                    esKRNL_TimeDly(20);
                }
                averPreVol /= POWER_RDC_COUNT;
                averPreCur /= POWER_RDC_COUNT;

                byte_addr  = POWER20_CHARGE1;
                hstr.byte_addr  = &byte_addr;
                eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr);
                tmp_value &= ~(1 << 7);
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                esKRNL_TimeDly(300);

                for(i = 0; i< POWER_RDC_COUNT; i++)
                {
                    byte_addr  = POWER20_BAT_AVERVOL_H8;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    batvol =  (tmp_value << 4);
                    byte_addr  = POWER20_BAT_AVERVOL_L4;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    batvol +=  (tmp_value & 0x0f) ;
                    batvol *= 1.1;
                    averNextVol += batvol;

                    byte_addr  = POWER20_BAT_AVERCHGCUR_H8;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    batcrnt = (tmp_value << 4);
                    byte_addr  = POWER20_BAT_AVERCHGCUR_L4;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    batcrnt +=(tmp_value & 0x0f);
                    batcrnt /= 2;
                    averNextCur += batcrnt;

                    esKRNL_TimeDly(20);
                }
                averNextVol /= POWER_RDC_COUNT;
                averNextCur /= POWER_RDC_COUNT;

                byte_addr  = POWER20_CHARGE1;
                hstr.byte_addr  = &byte_addr;
                eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr);
                tmp_value |=  (1 << 7);
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                esKRNL_TimeDly(50);

                if(ABS(averPreCur - averNextCur) > 200)
                {
                    temp = 1000 * ABS(averPreVol - averNextVol)/ABS(averPreCur - averNextCur);    //获得新的Rdc ,mOhm
                    if((temp < 5) || (temp >5000))    //新的值在100mOhm和5000mOhm之间?超出此范围返回原来的值
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

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER20_STATUS;
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

#if 0
       		__log("bat_cap:%d\n", bat_sts->bat_cap);							//电池实际容量，单位-毫安时
       		__log("dcin_valid:%d\n", bat_sts->dcin_valid);                 //外部电源是否存在？1-存在；0-不存在
       		__log("charge_status:%d\n", bat_sts->charge_status);              //充电状态，1-正在充电；0-不在充电
       		__log("battery_exist:%d\n", bat_sts->battery_exist);              //电池是否存在，1-存在；0-不存在
       		__log("battery_status:%d\n", bat_sts->battery_status);             //0: 充电  1：放电
       		__log("rest_vol:%d\n", bat_sts->rest_vol);                   //剩余电池电量，百分比
       		__log("rest_time:%d\n", bat_sts->rest_time);                  //剩余电池时间或者剩余充电时间
       		__log("bat_vol:%d\n", bat_sts->bat_vol);					//电池实时电压值？单位-伏
       		//__log("bat_crnt:%d\n", bat_sts->bat_crnt);					//电池实时充放电电流值？单位-毫安
       		__log("bat_cap:%d\n", bat_sts->bat_cap);					//电池实际容量，单位-毫安时
       		__log("bat_rdc:%d\n", bat_sts->bat_rdc);					//电池通路直流阻抗-毫欧
       		//__log("dcin_not_enough:%d\n", bat_sts->dcin_not_enough);            //表示 DCIN 负载能力不足
       		//__log("bat_vol_correct:%d\n", bat_sts->bat_vol_correct);			//电池校正后电压，单位-伏
#endif
			hstr.byte_count = 1;
			hstr.data = tmp_value;
			byte_addr  = 0x33;
			hstr.byte_addr  = &byte_addr;
			if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
			{
				break;
			}
			__log("axp209 REG33 =:%x\n", tmp_value[0]);
            hstr.byte_count = 4;
            hstr.data = tmp_value;
            byte_addr  = POWER20_ACIN_VOL_H8;
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
                byte_addr  = POWER20_BAT_AVERVOL_H8;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                bat_sts->bat_vol = ((tmp_value[0] << 4) + (tmp_value[1] & 0x0f))  * 1.1;
                total_vol = bat_sts->bat_vol;

                hstr.byte_count = 4;
                hstr.data       = tmp_value;
                byte_addr  = POWER20_BAT_AVERCHGCUR_H8;//获取电池ADC检测电流值
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                bat_sts->bat_crnt = ((tmp_value[0]<<4) + (tmp_value[1]&0x0f) +(tmp_value[2]<<5) + (tmp_value[3]&0x1f))*0.5;

                hstr.byte_count = 2;
                byte_addr  = POWER20_CHARGE1;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                Ichgset = _reg2test(tmp_value[0] & 0x0f,AXP209_Version & 0x10);//700

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
                        __wrn("fail to get Discharge current rate, set one default value\n");

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

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER20_STATUS;
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
                byte_addr  = POWER20_ADC_EN1;
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
                    byte_addr  = POWER20_ACIN_VOL_H8;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    acinvol = tmp_value << 4;
                    byte_addr  = POWER20_ACIN_VOL_L4;
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
                    byte_addr  = POWER20_ACIN_CUR_H8;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    acincur = tmp_value << 4;
                    byte_addr  = POWER20_ACIN_CUR_L4;
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
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER20_ADC_EN1;
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

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER20_MODE_CHGSTATUS;
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
                byte_addr  = POWER20_CHARGE1;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                hstr.data       = &tmp5;
                byte_addr  = POWER20_CHARGE2;
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
					charge->charge_current = 300;
				}
                else if(tmp_value == 0x01)
				{
					charge->charge_current = 400;
				}
				else if(tmp_value == 0x02)
				{
					charge->charge_current = 500;
				}
				else if(tmp_value == 0x03)
				{
					charge->charge_current = 600;
				}
				else if(tmp_value == 0x04)
				{
					charge->charge_current = 700;
				}
				else if(tmp_value == 0x05)
				{
					charge->charge_current = 800;
				}
				else if(tmp_value == 0x06)
				{
					charge->charge_current = 900;
				}
				else if(tmp_value == 0x07)
				{
					charge->charge_current = 1000;
				}
				else if(tmp_value == 0x08)
				{
					charge->charge_current = 1100;
				}
				else if(tmp_value == 0x09)
				{
					charge->charge_current = 1200;
				}
				else if(tmp_value == 0x0a)
				{
					charge->charge_current = 1300;
				}
				else if(tmp_value == 0x0b)
				{
					charge->charge_current = 1400;
				}
				else if(tmp_value == 0x0c)
				{
					charge->charge_current = 1500;
				}
				else if(tmp_value == 0x0d)
				{
					charge->charge_current = 1600;
				}
				else if(tmp_value == 0x0e)
				{
					charge->charge_current = 1700;
				}
				else if(tmp_value == 0x0f)
				{
					charge->charge_current = 1800;
				}
				else
				{
					charge->charge_current = 1800;
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

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER20_CHARGE1;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp4;
            hstr.byte_count         = 1;  // data is 1 bytes

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            hstr.data       = &tmp5;
            byte_addr  = POWER20_CHARGE2;
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
            if(charge->charge_current <300) //此时关掉充电
			{
				tmp4 &= 0x7f;
			}
			else if(charge->charge_current <400)//change_current set to 300
			{
				tmp4 |= 0x00;
			}
			else if(charge->charge_current <500) //change_current set to 400
			{
				tmp4 |= 0x01;
			}
			else if(charge->charge_current <600) //change_current set to 500
			{
				tmp4 |= 0x02;
			}
			else if(charge->charge_current <700) //change_current set to 600
			{
				tmp4 |= 0x03;
			}
			else if(charge->charge_current <800) //change_current set to 700
			{
				tmp4 |= 0x04;
			}
			else if(charge->charge_current <900) //change_current set to 800
			{
				tmp4 |= 0x05;
			}
			else if(charge->charge_current <1000) //change_current set to 900
			{
				tmp4 |= 0x06;
			}
			else if(charge->charge_current <1100) //change_current set to 1000
			{
				tmp4 |= 0x07;
			}
			else if(charge->charge_current <1200) //change_current set to 1100
			{
				tmp4 |= 0x08;
			}
			else if(charge->charge_current <1300) //change_current set to 1200
			{
				tmp4 |= 0x09;
			}
			else if(charge->charge_current <1400) //change_current set to 1300
			{
				tmp4 |= 0x0a;
			}
			else if(charge->charge_current <1500) //change_current set to 1400
			{
				tmp4 |= 0x0b;
			}
			else if(charge->charge_current <1600) //change_current set to 1500
			{
				tmp4 |= 0x0c;
			}
			else if(charge->charge_current <1700) //change_current set to 1600
			{
				tmp4 |= 0x0d;
			}
			else if(charge->charge_current <1800) //change_current set to 1700
			{
				tmp4 |= 0x0e;
			}
			else  //change_current set to 1800
			{
				tmp4 |= 0x0f;
			}


            if(charge->charge_timer1 == 30)
            {
                tmp5 &= (3 << 6);
                tmp5 |= 0 << 6;
            }
            else if(charge->charge_timer1 == 40)
            {
                tmp5 &= (3 << 6);
                tmp5 |= 1 << 6;
            }
            else if(charge->charge_timer1 == 50)
            {
                tmp5 &= (3 << 6);
                tmp5 |= 2 << 6;
            }
            else if(charge->charge_timer1 == 60)
            {
                tmp5 &= (3 << 6);
                tmp5 |= 3 << 6;
            }
            if(charge->charge_timer2 == 420)
            {
                tmp5 &= (3 << 0);
                tmp5 |= 0 << 0;
            }
            else if(charge->charge_timer2 == 480)
            {
                tmp5 &= (3 << 0);
                tmp5 |= 1 << 0;
            }
            else if(charge->charge_timer2 == 540)
            {
                tmp5 &= (3 << 0);
                tmp5 |= 2 << 0;
            }
            else if(charge->charge_timer2 == 600)
            {
                tmp5 &= (3 << 0);
                tmp5 |= 3 << 0;
            }
            hstr.data       = &tmp4;
            byte_addr  = POWER20_CHARGE1;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }

            hstr.data       = &tmp5;
            byte_addr  = POWER20_CHARGE2;
            hstr.byte_addr  = &byte_addr;
            ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

            break;
        }

        case DRV_POWER_CMD_BASIC_STATUS:
        {
            __drv_power_basic_status_t  *pbasic = (__drv_power_basic_status_t *)pbuffer;
            __u8       tmp1, tmp2, tmp3, tmp4;

            eLIBs_memset(pbasic, 0, sizeof(__drv_power_basic_status_t));

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER20_STATUS;
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.data               = &tmp1;
            hstr.byte_count         = 1;  // data is 1 bytes

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            hstr.data       = &tmp2;
            byte_addr  = POWER20_IPS_SET;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            hstr.data       = &tmp3;
            byte_addr  = POWER20_MODE_CHGSTATUS;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            hstr.data       = &tmp4;
            byte_addr  = POWER20_CHARGE1;
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
                if((tmp1 >> 7) & 0x01)            //圆口火牛充电
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
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER20_IPS_SET;
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

            byte_addr  = POWER20_APS_WARNING1;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            pips->vwarning     = 2867 +  5.6 * tmp_value;
            byte_addr  = POWER20_APS_WARNING2;
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

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER20_IPS_SET;
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
            byte_addr  = POWER20_APS_WARNING1;
            hstr.byte_addr  = &byte_addr;
                eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

            if((pips->voff > 2866) && (pips->voff < 4000))
            {
                tmp_value = ((pips->voff - 2867)/5.6);
            }
            byte_addr  = POWER20_APS_WARNING2;
            hstr.byte_addr  = &byte_addr;
            eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            ret = EPDK_OK;

            break;
        }

        case DRV_POWER_CMD_GET_COULOMB:
        {
            __u8 temp[8];
            __u8  adc_freq;
            __u32 rValue1,rValue2,rValue;
            eLIBs_memset(temp, 0, 8);

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER20_ADC_SPEED;
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

//            __inf("Cur_CoulombCounter  adc_freq = %d!\n",adc_freq);
            hstr.byte_count = 8;
            hstr.data = temp;
            byte_addr = POWER20_BAT_CHGCOULOMB3;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            rValue1 = 65536*((temp[0] << 24) + (temp[1] << 16) + (temp[2] << 8) + temp[3]);
            rValue2 = 65536*((temp[4] << 24) + (temp[5] << 16) + (temp[6] << 8) + temp[7]);
          	
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

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER20_STATUS;
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

            byte_addr  = POWER20_MODE_CHGSTATUS;
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

            rest_time_cal_20(aux);
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
			
			hstr.slave_addr		 = POWER_BOARD1_DEV_ADDR_20;
			hstr.slave_addr_flag	 = TWI_SLAVE_ADDRESS_07BIT;
			hstr.byte_addr_width	 = 1;  // reg_addr is 1 byte
			hstr.data				 = &tmp_value;
			hstr.byte_count		 = 1;  // data is 1 byte
			
			byte_addr	= POWER20_POK_SET;
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
		case DRV_POWER_CMD_SET_VBUS_CURR:
		{
			__u8 reg =(__u8 ) aux;
			
			hstr.slave_addr		 	= POWER_BOARD1_DEV_ADDR_20;
			hstr.slave_addr_flag	= TWI_SLAVE_ADDRESS_07BIT;
			hstr.byte_addr_width	= 1;  // reg_addr is 1 byte
			hstr.byte_count		 	= 1;  // data is 1 byte
			if((reg & 0x03) == 0x00)  //not_limit
			{
				hstr.data		= &tmp_value;
				byte_addr		= POWER20_IPS_SET;
				hstr.byte_addr  = &byte_addr;
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
				{
					 break;
				}
				tmp_value &= 0xfc;
				tmp_value |= 0x03; 						
				eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
			}
			else if((reg & 0x03) == 0x01)  //900mA_limit
			{
				hstr.data		= &tmp_value;
				byte_addr		= POWER20_IPS_SET;
				hstr.byte_addr  = &byte_addr;
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
				{
					 break;
				}
				tmp_value &= 0xfc;
				tmp_value |= 0x00; 						
				eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
			}
			else if((reg & 0x03) == 0x02)  //500mA_limit
			{
				hstr.data		= &tmp_value;
				byte_addr		= POWER20_IPS_SET;
				hstr.byte_addr  = &byte_addr;
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
				{
					 break;
				}
				tmp_value &= 0xfc;
				tmp_value |= 0x01; 						
				eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
			}
			else   //100mA_limit
			{
				hstr.data		= &tmp_value;
				byte_addr		= POWER20_IPS_SET;
				hstr.byte_addr  = &byte_addr;
				if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
				{
					 break;
				}
				tmp_value &= 0xfc;
				tmp_value |= 0x02; 						
				eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
			}
			break;
		}

		case DRV_POWER_CMD_SET_81H:
		{
			 tmp_value = (__u32)pbuffer;
			 
			 hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            byte_addr          = POWER20_VOUT_MONITOR; //81H
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 8;  // reg_addr is 1 byte
            hstr.data               = &tmp_value;
            hstr.byte_count         = 8;  // data is 1 byte

            if(EPDK_OK == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                ret = tmp_value;
            }

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

__dev_devop_t power_20_dev_ops =
{
    DEV_Power_20_Open,
    DEV_Power_20_Close,
    DEV_Power_20_Read,
    DEV_Power_20_Write,
    DEV_Power_20_Ioctrl
};
