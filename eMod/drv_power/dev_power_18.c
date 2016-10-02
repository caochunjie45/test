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

extern __power_drv_t    powerdrv;
extern __u32            Tcv ;
extern __s32            this_rdc;

extern __drv_power_fuelguage_t  fuelguage;
extern __u8             standby;
extern __u8             PMU_type;
extern __sys_p1current_cfg_t    *pP1CurrentCfg;
static __u8             dc2set = 1, ldo1set = 1;
static __u8             usb_charge_exist = 0;

__u8    AXP189_Version = 0;
__u16   Bat_Pre_Cur = 1;
__u16   Iconst_current = 1;
__u8    DCIN_Pre_Presence = 0;
__u8    DCIN_Presence = 0;
__u8    Bat_Cur_Count = 0;


static __u32 _reg2test(__u32 value,__u8 version)
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
                ret = 80;
            }
            break;
        }
        case 1:
        {
            if(version)
            {
                ret = 300;
            }
            else
            {
                ret = 120;
            }
            break;
        }
        case 2:
        {
            if(version)
            {
                ret = 495;
            }
            else
            {
                ret = 218;
            }
            break;
        }
        case 3:
        {
            if(version)
            {
                ret = 684;
            }
            else
            {
                ret = 315;
            }
            break;
        }
        case 4:
        {
            if(version)
            {
                ret = 853;
            }
            else
            {
                ret = 412;
            }
            break;
        }
        case 5:
        {
            if(version)
            {
                ret = 1057;
            }
            else
            {
                ret = 510;
            }
            break;
        }
        case 6:
        {
            if(version)
            {
                ret = 1245;
            }
            else
            {
                ret = 607;
            }
            break;
        }
        case 7:
        {
            if(version)
            {
                ret = 1430;
            }
            else
            {
                ret = 700;
            }
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

__u8 Rest_Vol_Rate_18(__u16 vol)
{
    if(vol > 4150)
    {
        return 100;
    }
    else if(vol < 2700)
    {
        return 0;
    }
    else if(vol < 3200)
    {
        return (10 * (vol - 2700) / 5000);
    }
    else if(vol < 3650)
    {
        return (1500+ 17000 * (vol - 3200) / 450)/1000;
    }
    else if(vol < 3750)
    {
        return (18500 + 1500 * (vol - 3650) / 10)/1000;              //20%改为18%
    }
    else if(vol < 3830)
    {
        return (33500 + (1500 * (vol - 3750)/(383 - 375)))/1000;
    }
    else if(vol < 4000)
    {
        return (48500 + (4000 * (vol - 3830)/(400 - 383)))/1000;    //40%改为37%
    }
    else
    {
        if(vol > 4150)
        {
            vol = 4150;
        }
        return (855 + (150 * (vol - 4000)/150))/10;                 //4200-3950 = 250，13%改为15%
    }
}


//******************************************************************************
//通过脚本解析出来的电流值估算电池供电时间
//******************************************************************************
void rest_time_cal_18 (__u8 lcd_level)
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
            __wrn("parameter(%d) is invalid when calculate rest time!\n", lcd_level);
            break;
        }
    }

    return;
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
__hdle DEV_Power_18_Open(void * open_arg, __u32 mode)
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
__s32 DEV_Power_18_Close(__hdle hPower)
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
__u32 DEV_Power_18_Read(void *pdata, __u32 size, __u32 n, __hdle hPower)
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
__u32 DEV_Power_18_Write(const void *pdata, __u32 size, __u32 n, __hdle hPower)
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
__s32 DEV_Power_18_Ioctrl(__hdle hPower, __u32 cmd, __s32 aux, void *pbuffer)
{
    __powerdev_t        *hdev = (__powerdev_t *)hPower;
    __twi_dev_para_ex_t    hstr;
    __u8                tmp_value;
    __s32               ret;
    __u8 byte_addr = 0;

    ret = EPDK_FAIL;
    eLIBs_memset(&hstr, 0, sizeof(__twi_dev_para_ex_t));

    P1_OpLock();

    switch(cmd)
    {
        case DRV_POWER_CMD_READ_REG:
        {           
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT; 
            byte_addr = (__u8)aux;
            hstr.byte_addr          = &byte_addr;            
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
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;               
            byte_addr = aux;
            hstr.byte_addr          = &byte_addr;            
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
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;                
            byte_addr = POWER18_ONOFF;
            hstr.byte_addr          = &byte_addr;             
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
            __u8    temp;

            temp = (aux >> 0) & 0xff;
            if(temp)
            {            
                hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
                hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;                       
                byte_addr = POWER18_INTEN1;
                hstr.byte_addr          = &byte_addr;                   
                hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
                hstr.data               = &tmp_value;
                hstr.byte_count         = 1;  // data is 1 byte

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                tmp_value |= temp;
                tmp_value |= 0x08; //禁止关闭6S关机功能
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                {
                    break;
                }
            }

            temp = (aux >> 7) & 0xff;
            if(temp)
            {
                hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
                hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;  
                byte_addr = POWER18_INTEN2;
                hstr.byte_addr          = &byte_addr;            
                hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
                hstr.data               = &tmp_value;
                hstr.byte_count         = 1;  // data is 1 byte

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
                hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
                hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;               
                byte_addr               = POWER18_INTEN3;         
                hstr.byte_addr          = &byte_addr;
                hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
                hstr.data               = &tmp_value;
                hstr.byte_count         = 1;  // data is 1 byte

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

            temp = (aux >> 0) & 0xff;
            if(temp)
            {
                hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
                hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;     
                byte_addr               = POWER18_INTEN1;
                hstr.byte_addr          = &byte_addr;            
                hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
                hstr.data               = &tmp_value;
                hstr.byte_count         = 1;  // data is 1 byte

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                tmp_value  &= ~temp;
                tmp_value |= 0x08; //禁止关闭6S关机功能
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                {
                    break;
                }
            }

            temp = (aux >> 7) & 0xff;
            if(temp)
            {
                hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
                hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
                byte_addr = POWER18_INTEN2;
                hstr.byte_addr          = &byte_addr;            
                hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
                hstr.data               = &tmp_value;
                hstr.byte_count         = 1;  // data is 1 byte

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
                hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
                hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;  
                byte_addr               = POWER18_INTEN3;
                hstr.byte_addr          = &byte_addr;            
                hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
                hstr.data               = &tmp_value;
                hstr.byte_count         = 1;  // data is 1 byte
                
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
            
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;    
            byte_addr               = POWER18_INTSTS1;
            hstr.byte_addr          = &byte_addr;            
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
            hstr.data               = data + 0;
            hstr.byte_count         = 1;  // data is 1 byte

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            byte_addr = POWER18_INTSTS2;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = data + 1;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            byte_addr  = POWER18_INTSTS3;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = data + 2;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_INT_CLEAR:
        {
            tmp_value = *((__u8 *)pbuffer + 0);

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;               
            byte_addr          = POWER18_INTSTS1;         
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }

            tmp_value = *((__u8 *)pbuffer + 1);            
            byte_addr  = POWER18_INTSTS2;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = &tmp_value;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }

            tmp_value = *((__u8 *)pbuffer + 2);
            byte_addr  = POWER18_INTSTS3;
            hstr.byte_addr  = &byte_addr;
            hstr.data       = &tmp_value;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_VOUT_GET:
        {
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;  
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte     
        
            hstr.byte_count = 1;
            if(aux == POWER_VOL_LDO1)
            {
                if(ldo1set)
                {
                    ret = 3300;
                }
                else
                {
                    ret = 1250;
                }
            }
            else if((aux == POWER_VOL_DCDC1) || (aux == POWER_VOL_DCDC2))
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER18_DC12OUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                if(aux == POWER_VOL_DCDC1)
                {
                    ret = 2800 + ((tmp_value >> 4) & 0x07) * 100;
                }
                else
                {
                    if(dc2set)
                    {
                        ret =  800 + ((tmp_value >> 0) & 0x0f) * 40;
                    }
                    else
                    {
                        ret = 1400 + ((tmp_value >> 0) & 0x0f) * 40;
                    }
                }
            }
            else if((aux == POWER_VOL_LDO2) || (aux == POWER_VOL_LDO3) || (aux == POWER_VOL_DCDC3))
            {
                __u8 temp;

                hstr.data       = &temp;
                byte_addr  = POWER18_STATUS;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                temp = (temp >> 5) & 0x01;

                hstr.data       = &tmp_value;
                byte_addr  = POWER18_LDOOUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                if(aux == POWER_VOL_LDO2)
                {
                    ret = 2800 + ((tmp_value >> 5) & 0x03) * 100;
                }
                else if(aux == POWER_VOL_DCDC3)
                {
                    if(temp)
                    {
                        ret = 2000 + ((tmp_value >> 2) & 0x07) * 100;
                    }
                    else
                    {
                        ret = 1300 + ((tmp_value >> 2) & 0x07) * 100;
                    }
                }
                else
                {
                    if(temp)
                    {
                        ret = 1600 + ((tmp_value >> 0) & 0x03) * 100;
                    }
                    else
                    {
                        ret = 2300 + ((tmp_value >> 0) & 0x03) * 100;
                    }
                }
            }
            else if((aux == POWER_VOL_LDO4) || (aux == POWER_VOL_LDO5))
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER18_SW_CTL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                if(aux == POWER_VOL_LDO4)
                {
                    ret = 2700 + ((tmp_value >> 2) & 0x03) * 200;
                }
                else
                {
                    ret = 2700 + ((tmp_value >> 0) & 0x03) * 200;
                }
            }

            break;
        }

        case DRV_POWER_CMD_VOUT_SET:
        {
            __u32               tmp;
            
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;  
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte     

            tmp = (__u32)pbuffer;
            hstr.byte_count = 1;
            if((aux == POWER_VOL_DCDC1) || (aux == POWER_VOL_DCDC2))
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER18_DC12OUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                if(aux == POWER_VOL_DCDC1)
                {
                    if((!(tmp%100)) && (tmp >= 2800) && (tmp <= 3500))
                    {
                        tmp_value &= ~(0x07 << 4);
                        tmp_value |= ((tmp - 2800)/100) << 4;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    tmp_value &= ~(0x0f << 0);
                    if(!dc2set)
                    {
                        if((!(tmp%40)) && (tmp >= 1400) && (tmp <= 2000))
                        {
                            tmp_value |= ((tmp - 1400)/40) << 0;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else if(dc2set)
                    {
                        if((!(tmp%40)) && (tmp >= 800) && (tmp <= 1400))
                        {
                            tmp_value |= ((tmp - 800)/40) << 0;
                        }
                        else
                        {
                            break;
                        }
                    }
                }

                ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
            else if(aux == POWER_VOL_LDO1)
            {
                __wrn("P1 Voltage LDO1 cant be set!\n");
            }
            else if((aux == POWER_VOL_LDO2) || (aux == POWER_VOL_LDO3) || (aux ==POWER_VOL_DCDC3))
            {
                __u8    dc3_status;

                hstr.data       = &dc3_status;
                byte_addr  = POWER18_STATUS;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                dc3_status   = (dc3_status >> 5) & 0x01;

                hstr.data       = &tmp_value;
                byte_addr  = POWER18_LDOOUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                if(aux == POWER_VOL_DCDC3)
                {
                    if(dc3_status)
                    {
                        tmp_value &= ~(0x07 << 2);
                        if((!(tmp%100)) && (tmp >= 2000) && (tmp <= 2700))
                        {
                            tmp_value |= ((tmp - 2000)/100) << 2;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        if((!(tmp%100)) && (tmp >= 1300) && (tmp <= 1900))
                        {
                            tmp_value |= ((tmp - 1300)/100) << 2;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else if(aux == POWER_VOL_LDO2)
                {
                    if((!(tmp%100)) && (tmp >= 2800) && (tmp <= 3100))
                    {
                        tmp_value &= ~(0x03 << 5);
                        tmp_value |= ((tmp - 2800)/100) << 5;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    if(dc3_status)
                    {
                        tmp_value &= ~(0x03 << 0);
                        if((!(tmp%100)) && (tmp >= 1600) && (tmp <= 1900))
                        {
                            tmp_value |= ((tmp - 1600)/100) << 0;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        if((!(tmp%100)) && (tmp >= 2300) && (tmp <= 2600))
                        {
                            tmp_value |= ((tmp - 2300)/100) << 0;
                        }
                        else
                        {
                            break;
                        }
                    }
                }

                ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
            else if((aux == POWER_VOL_LDO4) || (aux == POWER_VOL_LDO5))
            {
                hstr.data       = &tmp_value;
                byte_addr  = POWER18_LDOOUT_VOL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                if(aux == POWER_VOL_LDO4)
                {
                    if((!(tmp%200)) && (tmp >= 2700) && (tmp <= 3300))
                    {
                        tmp_value &= ~(0x03 << 2);
                        tmp_value |= ((tmp - 2700)/200) << 2;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    if((!(tmp%200)) && (tmp >= 2700) && (tmp <= 3300))
                    {
                        tmp_value &= ~(0x03 << 0);
                        tmp_value |= ((tmp - 2700)/200) << 2;
                    }
                    else
                    {
                        break;
                    }
                }

                ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }

            break;
        }

        case DRV_POWER_CMD_VSTS_GET:
        {
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;  
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte     

            hstr.byte_count = 1;
            hstr.data       = &tmp_value;

            if(aux == POWER_VOL_LDO1)
            {
                ret = 1;
            }
            else if((aux == POWER_VOL_DCDC1) || (aux == POWER_VOL_DCDC2) || (aux == POWER_VOL_DCDC3) || (aux == POWER_VOL_LDO2))
            {
                byte_addr  = POWER18_DCDCCTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                if(aux == POWER_VOL_DCDC1)
                {
                    ret  = 1;
                    ret |= ((tmp_value >> 7) & 0x01) << 1;
                    ret |= ((tmp_value >> 2) & 0x01) << 2;
                }
                else if(aux == POWER_VOL_DCDC2)
                {
                    ret  = 1;
                    ret |= ((tmp_value >> 6) & 0x01) << 1;
                    ret |= ((tmp_value >> 1) & 0x01) << 2;
                }
                else if(aux == POWER_VOL_DCDC3)
                {
                    ret  = ((tmp_value >> 4) & 0x01) << 0;
                    ret |= ((tmp_value >> 5) & 0x01) << 1;
                    ret |= ((tmp_value >> 0) & 0x01) << 2;
                }
                else
                {
                    ret = ((tmp_value >> 3) & 0x01) << 0;
                }
            }
            else if(aux == POWER_VOL_LDO3)
            {
                byte_addr  = POWER18_LDOOUT_VOL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                ret = ((tmp_value >> 7) & 0x01) << 0;
            }
            else if((aux == POWER_VOL_LDO4) || (aux == POWER_VOL_LDO5) || (aux == POWER_VOL_SW1) || (aux == POWER_VOL_SW2))
            {
                byte_addr  = POWER18_SW_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                if(aux == POWER_VOL_LDO4)
                {
                    ret = ((tmp_value >> 5) & 0x01) << 0;
                }
                else if(aux == POWER_VOL_LDO5)
                {
                    ret = ((tmp_value >> 4) & 0x01) << 0;
                }
                else if(aux == POWER_VOL_SW1)
                {
                    ret = ((tmp_value >> 7) & 0x01) << 0;
                }
                else
                {
                    ret = ((tmp_value >> 6) & 0x01) << 0;
                }
            }

            break;
        }

        case DRV_POWER_CMD_VSTS_SET:
        {
            __u32   temp;

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;  
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte     

            temp            = (__u32)pbuffer;
            hstr.byte_count = 1;
            hstr.data       = &tmp_value;
            if((aux == POWER_VOL_DCDC1) || (aux == POWER_VOL_DCDC2) || (aux == POWER_VOL_DCDC3) || (aux == POWER_VOL_LDO2))
            {
                byte_addr  = POWER18_DCDCCTL;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                if(aux == POWER_VOL_DCDC1)
                {
                    tmp_value &= ~((1 << 7) | (1 << 2));
                    tmp_value |= ((temp >> 1) & 0x01) << 7;
                    tmp_value |= ((temp >> 2) & 0x01) << 2;
                }
                else if(aux == POWER_VOL_DCDC2)
                {
                    tmp_value &= ~((1 << 6) | (1 << 1));
                    tmp_value |= ((temp >> 1) & 0x01) << 6;
                    tmp_value |= ((temp >> 2) & 0x01) << 1;
                }
                else if(aux == POWER_VOL_DCDC3)
                {
                    tmp_value &= ~((1 << 5) | (1 << 0) | (1<<0));
                    tmp_value |= ((temp >> 1) & 0x01) << 5;
                    tmp_value |= ((temp >> 2) & 0x01) << 0;
                    tmp_value |= ((temp >> 0) & 0x01) << 4;
                }
                else
                {
                    tmp_value &= ~(1 << 3);
                    tmp_value |=  ((temp >> 0) & 0x01) << 3;
                }
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                {
                    break;
                }
            }
            else if(aux == POWER_VOL_LDO3)
            {
                byte_addr  = POWER18_LDOOUT_VOL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                tmp_value &= ~(1 << 7);
                tmp_value |=  ((temp >> 0) & 0x01) << 7;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                {
                    break;
                }
            }
            else if((aux == POWER_VOL_LDO4) || (aux == POWER_VOL_LDO5) || (aux == POWER_VOL_SW1) || (aux == POWER_VOL_SW2))
            {
                byte_addr  = POWER18_SW_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                if(aux == POWER_VOL_LDO4)
                {
                    tmp_value &= ~(1 << 5);
                    tmp_value |=  ((temp >> 0) & 0x01) << 5;
                }
                else if(aux == POWER_VOL_LDO5)
                {
                    tmp_value &= ~(1 << 4);
                    tmp_value |=  ((temp >> 0) & 0x01) << 4;
                }
                else if(aux == POWER_VOL_SW1)
                {
                    tmp_value &= ~(1 << 7);
                    tmp_value |=  ((temp >> 0) & 0x01) << 7;
                }
                else
                {
                    tmp_value &= ~(1 << 6);
                    tmp_value |=  ((temp >> 0) & 0x01) << 6;
                }
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                {
                    break;
                }
            }

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_PWREPT_GET:
        {        
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;               
            byte_addr          = POWER18_EPT_SW;   
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte                
           
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            if(aux == POWER_EPT_1)
            {
                ret  = ((tmp_value >> 6) & 0x01) << 0;
                ret |= ((tmp_value >> 7) & 0x01) << 2;
            }
            else if(aux == POWER_EPT_2)
            {
                ret  = ((tmp_value >> 4) & 0x01) << 0;
                ret |= ((tmp_value >> 5) & 0x01) << 2;
            }
            else if(aux == POWER_EPT_3)
            {
                ret  = ((tmp_value >> 2) & 0x01) << 0;
                ret |= ((tmp_value >> 3) & 0x01) << 2;
            }
            else if(aux == POWER_EPT_4)
            {
                ret  = ((tmp_value >> 0) & 0x01) << 0;
                ret |= ((tmp_value >> 1) & 0x01) << 2;
            }

            break;
        }
        case DRV_POWER_CMD_PWREPT_SET:
        {
            __u32   temp;

            temp            = (__u32)pbuffer;

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;               
            byte_addr          = POWER18_EPT_SW;    
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte 

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            if(aux == POWER_EPT_1)
            {
                if(!(temp & 0x08))
                {
                    tmp_value &= ~(1 << 7);
                    tmp_value |= ((temp >> 2) & 0x01) << 7;
                }
                if(!(temp & 0x02))
                {
                    tmp_value &= ~(1 << 6);
                    tmp_value |= ((temp >> 0) & 0x01) << 6;
                }
            }
            else if(aux == POWER_EPT_2)
            {
                if(!(temp & 0x08))
                {
                    tmp_value &= ~(1 << 5);
                    tmp_value |= ((temp >> 2) & 0x01) << 5;
                }
                if(!(temp & 0x02))
                {
                    tmp_value &= ~(1 << 4);
                    tmp_value |= ((temp >> 0) & 0x01) << 4;
                }
            }
            else if(aux == POWER_EPT_3)
            {
                if(!(temp & 0x08))
                {
                    tmp_value &= ~(1 << 3);
                    tmp_value |= ((temp >> 2) & 0x01) << 3;
                }
                if(!(temp & 0x02))
                {
                    tmp_value &= ~(1 << 2);
                    tmp_value |= ((temp >> 0) & 0x01) << 2;
                }
            }
            else if(aux == POWER_EPT_4)
            {
                if(!(temp & 0x08))
                {
                    tmp_value &= ~(1 << 1);
                    tmp_value |= ((temp >> 2) & 0x01) << 1;
                }
                if(!(temp & 0x02))
                {
                    tmp_value &= ~(1 << 0);
                    tmp_value |= ((temp >> 0) & 0x01) << 0;
                }
            }

            ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

            break;
        }

        case DRV_POWER_CMD_BATADC_GET:
        {
            __drv_power_battery_status_t1   *bat_sts = (__drv_power_battery_status_t1 *)pbuffer;

            eLIBs_memset(bat_sts, 0, sizeof(__drv_power_battery_status_t1));

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;               
            byte_addr          = POWER18_STATUS;    
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte 
            
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            bat_sts->battery_exist = ((tmp_value >> 0) & 0x01);
            if(bat_sts->battery_exist)
            {
                __u8 adcsw;

                hstr.data       = &adcsw;
                byte_addr  = POWER18_ADCSW_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                bat_sts->bat_voladc_enable  = ((adcsw >> 7) & 0x01);
                bat_sts->bat_crntadc_enable = ((adcsw >> 6) & 0x01);
                adcsw                       = ((adcsw >> 1) & 0x03);
                if(adcsw == 2)
                {
                    bat_sts->adc_freq    = 25;
                }
                else
                {
                    bat_sts->adc_freq    = 8 + 8 * adcsw;
                }
                hstr.data       = &tmp_value;
                if(bat_sts->bat_voladc_enable)
                {
                    byte_addr  = POWER18_BATTERY_VOL;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    bat_sts->bat_vol = 2500 + (tmp_value << 3);
                }

                if(bat_sts->bat_crntadc_enable)
                {
                    byte_addr  = POWER18_BATTERY_CURRENT;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    if(AXP189_Version & 0x10)
                    {
                        bat_sts->bat_crnt = tmp_value * 667 / 100;      //C版AXP189适用
                    }
                    else
                    {
                        bat_sts->bat_crnt = tmp_value << 2;
                    }
                }

                byte_addr  = POWER18_CHARGE1;
                hstr.byte_addr  = &byte_addr;

                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                bat_sts->bat_status = ((tmp_value >> 4) & 0x01);
            }

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_BATADC_SET:
        {
            __drv_power_battery_status_t1  *bat_sts = (__drv_power_battery_status_t1 *)pbuffer;

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;               
            byte_addr          = POWER18_ADCSW_CTL;        
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte 

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            if(bat_sts->adc_freq == 25)
            {
                tmp_value &= ~(0x03 << 2);
                tmp_value |=   0x02 << 2;
            }
            else if((bat_sts->adc_freq == 16) || (bat_sts->adc_freq == 8) || (bat_sts->adc_freq == 32))
            {
                tmp_value &= 0x03 << 2;
                tmp_value |= ((bat_sts->adc_freq>>3) - 1) << 2;
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

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;               
            byte_addr          = POWER18_STATUS;    
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte 

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            bat_exist = ((tmp_value >> 0) & 0x01);
            dcin_valid = ((tmp_value >> 4) & 0x01);
            if(!bat_exist)
            {
                __wrn("battery dosent exist\n");
                goto  _POWER_GET_RDC_;
            }

            if(dcin_valid)  //充电
            {
                __u32   i,temp;
                __s32    averPreVol = 0, averPreCur = 0,averNextVol = 0,averNextCur = 0;

                for(i = 0; i< POWER_RDC_COUNT; i++)
                {
                    byte_addr  = POWER18_BATTERY_VOL;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    averPreVol += 2500 + (tmp_value << 3);

                    byte_addr  = POWER18_BATTERY_CURRENT;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }

                    if(AXP189_Version & 0x10)
                    {
                        averPreCur += tmp_value * 667 / 100;        //C版AXP189适用
                    }
                    else
                    {
                        averPreCur += tmp_value << 2;
                    }

                    esKRNL_TimeDly(5);
                }
                averPreVol /= POWER_RDC_COUNT;
                averPreCur /= POWER_RDC_COUNT;

                byte_addr  = POWER18_CHARGE1;
                hstr.byte_addr  = &byte_addr;
                eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr);
                tmp_value &= ~(1 << 7);
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                esKRNL_TimeDly(50);

                for(i = 0; i< POWER_RDC_COUNT; i++)
                {
                    byte_addr  = POWER18_BATTERY_VOL;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    averNextVol += 2500 + (tmp_value << 3);

                    byte_addr  = POWER18_BATTERY_CURRENT;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    if(AXP189_Version & 0x10)
                    {
                        averNextCur += tmp_value * 667 / 100;       //C版AXP189适用
                    }
                    else
                    {
                        averNextCur += tmp_value << 2;
                    }

                    esKRNL_TimeDly(5);
                }
                averNextVol /= POWER_RDC_COUNT;
                averNextCur /= POWER_RDC_COUNT;

                byte_addr  = POWER18_CHARGE1;
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
                        goto  _POWER_GET_RDC_;
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
                        if((chr[0] - 0xb0) > 0)                //如果文件保存的是 上次正确检测的值，此次做平均
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
                        eLIBs_fwrite(key_value, 1, 3, pfile);        //此次后的值写回文件
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
                __wrn("create file ok\n");
                //key_value[0] = '3';
                key_value[0] = '2';
                key_value[1] = '0';
                key_value[2] = '0';
                eLIBs_fwrite(key_value, 1, 3, pfile);
            }

            eLIBs_fread(key_value, 1, 3, pfile);
            eLIBs_fclose(pfile);

            ret = ((key_value[0] & 0x7f) -'0') * 100 + (key_value[1]-'0') * 10 + (key_value[2]-'0');

            __wrn("bat rdc = %d\n", ret);

            break;
        }

        case DRV_POWER_CMD_BATSTATUS_TABLE_GET:
        {

            __u16 dcin_vol;
            __drv_power_battery_status_t2  *bat_sts = (__drv_power_battery_status_t2 *)pbuffer;

            {
                __s32 Battery_cap;

                if(esCFG_GetKeyValue("power", "Battery_cap",  &Battery_cap, 1))
                {
                    __wrn("fail to get Battery cap, set one default value\n");

                    fuelguage.bat_cap = 2200;
                    bat_sts->bat_cap  = 2200;
                }
                else
                {
                    fuelguage.bat_cap = Battery_cap;
                    bat_sts->bat_cap  = Battery_cap;
                }
            }
            bat_sts->bat_rdc = this_rdc;


            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;               
            byte_addr          = POWER18_STATUS;   
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte 
            
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            bat_sts->battery_exist = ((tmp_value >> 0) & 0x01);
            bat_sts->dcin_valid = ((tmp_value >> 4) & 0x01);

            hstr.data       = &tmp_value;
            byte_addr  = POWER18_DCIN_VOL;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            dcin_vol = 3750 + tmp_value * 12;
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
                __u32  Ichgset, total_vol, Iendchg ,Tcv_Rest_Vol;
                __u16  Internal_Ibat = 1;

                hstr.data       = &tmp_value;

                byte_addr  = POWER18_BATTERY_VOL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                bat_sts->bat_vol = 2500 + (tmp_value << 3);
                total_vol = bat_sts->bat_vol;


                hstr.data       = &tmp_value;
                byte_addr  = POWER18_BATTERY_CURRENT;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    bat_sts->bat_crnt = 1;
                    break;
                }
                else if(tmp_value)
                {
                    if(AXP189_Version & 0x10)
                    {
                        bat_sts->bat_crnt = tmp_value * 667 / 100;      //C版AXP189适用
                    }
                    else
                    {
                        bat_sts->bat_crnt = tmp_value << 2;
                    }
                }
                else
                {
                    bat_sts->bat_crnt = 1;
                }
                Internal_Ibat = bat_sts->bat_crnt;

                byte_addr  = POWER18_CHARGE1;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                bat_sts->battery_status = ((tmp_value >> 4) & 0x01);
                Ichgset = _reg2test(tmp_value & 0x07,AXP189_Version & 0x10);//700

                byte_addr  = POWER18_ONOFF;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                Iendchg = Ichgset * (10 + 5 * ((tmp_value >> 6) & 0x01))/100;//70
                bat_sts->charge_status = ((tmp_value >> 2) & 0x01);
//                Tcv     = 60 * 17 * bat_sts->bat_cap / (35 * Ichgset);//195, 612.857
                DCIN_Presence = tmp_value & 0x08;

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

                if(bat_sts->dcin_valid) //充电
                {
                    total_vol -= bat_sts->bat_crnt * bat_sts->bat_rdc * CHG_RDC_RATE/100000;
                    bat_sts->bat_vol_correct = total_vol; // battery voltage correct add ant 20100827
                }
                else  //放电
                {
                    __s32 Discharge_current_rate;

                    if(esCFG_GetKeyValue("power", "Discharge_rate",  &Discharge_current_rate, 1))
                    {
                        __wrn("fail to get Discharge current rate, set one default value\n");

                        Discharge_current_rate = 10;
                    }

                    bat_sts->bat_crnt = bat_sts->bat_crnt * Discharge_current_rate / 10 ;
                    if(bat_sts->bat_crnt > (Max_Bat_Cur * Ichgset / 10))
                    {
                        bat_sts->bat_crnt = 10 * bat_sts->bat_crnt / Discharge_current_rate;
                    }
                    bat_sts->bat_crnt = (bat_sts->bat_crnt + Bat_Pre_Cur) / 2;

                    if(DCIN_Pre_Presence != DCIN_Presence)
                    {
                        bat_sts->bat_crnt = Internal_Ibat;
                        Bat_Cur_Count = 3;
                    }
                    if(Bat_Cur_Count)
                    {
                        bat_sts->bat_crnt = Internal_Ibat;
                        Bat_Cur_Count --;
                    }
                    //total_vol += bat_sts->bat_crnt * bat_sts->bat_rdc /10/EXT_PMOS;
                    total_vol += bat_sts->bat_crnt * (bat_sts->bat_rdc - Discharge_Rdc_Cal) / 1000;
                    bat_sts->bat_vol_correct = total_vol; // battery voltage correct add ant 20100827
                }
                Bat_Pre_Cur = bat_sts->bat_crnt;
                DCIN_Pre_Presence = DCIN_Presence;

                __wrn("total_vol  = %d\n", total_vol);
                bat_sts->rest_vol = Rest_Vol_Rate_18(total_vol);

                __wrn("rest_vol  = %d\n", bat_sts->rest_vol);
                __wrn("dcin_valid  = %d\n", bat_sts->dcin_valid);
                __wrn("bat_rdc  = %d\n", bat_sts->bat_rdc);

                if(bat_sts->dcin_valid && bat_sts->charge_status)//充电
                {
                    if(bat_sts->bat_vol < 4190)
                    {
                        __wrn("<4200\n", bat_sts->bat_rdc);
                        Tcv_Rest_Vol = Rest_Vol_Rate_18(4200 - bat_sts->bat_crnt * bat_sts->bat_rdc / 1000);
                        Iconst_current = bat_sts->bat_crnt;
                        if(Tcv_Rest_Vol < 70)
                        {
                            Tcv = 60 * (100 - Tcv_Rest_Vol) * bat_sts->bat_cap / (45 * bat_sts->bat_crnt);
                        }
                        else
                        {
                            Tcv = 60 * (100 - Tcv_Rest_Vol) * bat_sts->bat_cap / (35 * bat_sts->bat_crnt);
                        }
                        bat_sts->rest_time = 6 * bat_sts->bat_cap * ABS(Tcv_Rest_Vol - bat_sts->rest_vol) / bat_sts->bat_crnt / 10 + Tcv ;
                    }
                    else
                    {
                        __wrn("TCV  = %d\n",Tcv);
                        if(Iconst_current == 1)
                        {
                            Iconst_current = Ichgset;
                        }
                        if(Tcv == 0)
                        {
                            Tcv_Rest_Vol = Rest_Vol_Rate_18(4200 - Ichgset * bat_sts->bat_rdc / 1000);
                            if(Tcv_Rest_Vol < 70)
                            {
                                Tcv = 60 * (100 - Tcv_Rest_Vol) * bat_sts->bat_cap / (45 * Ichgset);
                            }
                            else
                            {
                                Tcv = 60 * (100 - Tcv_Rest_Vol) * bat_sts->bat_cap / (35 * Ichgset);
                            }
                        }
                        if(bat_sts->bat_crnt < Iendchg)
                        {
                            bat_sts->rest_time = 1;
                        }
                        else
                        {
                            bat_sts->rest_time = Tcv * (90 + 100 * Iendchg/bat_sts->bat_crnt) * (90 +100 * Iendchg/bat_sts->bat_crnt)*ABS(bat_sts->bat_crnt - Iendchg) / Iconst_current /10000;
                        }
                    }
                }
                else  //放电
                {
                    if(total_vol < 3000)
                    {
                        bat_sts->rest_time = 0;
                    }
                    else
                    {
                        bat_sts->rest_time = (60 * bat_sts->bat_cap * ABS(bat_sts->rest_vol - 6) / bat_sts->bat_crnt + 50)/102;
                    }
                }
            }
            else
            {
                bat_sts->bat_vol = 2500;
                bat_sts->bat_crnt = 0;
                bat_sts->rest_time = 0;
                bat_sts->rest_vol = 0;
            }

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_DCINADC_GET:
        {
            __drv_power_dcin_status_t   *dcin_sts = (__drv_power_dcin_status_t *)pbuffer;

            eLIBs_memset(dcin_sts, 0, sizeof(__drv_power_dcin_status_t));

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;               
            byte_addr          = POWER18_ONOFF;     
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte 

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            dcin_sts->dcin_exist = ((tmp_value >> 3) & 0x01);
            if(dcin_sts->dcin_exist)
            {
                __u8 adcsw;

                hstr.data       = &adcsw;
                byte_addr  = POWER18_ADCSW_CTL;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }
                dcin_sts->dcin_voladc_enable  = ((adcsw >> 5) & 0x01);
                dcin_sts->dcin_crntadc_enable = ((adcsw >> 4) & 0x01);
                adcsw                         = ((adcsw >> 1) & 0x03);
                if(adcsw == 2)
                {
                    dcin_sts->adc_freq    = 25;
                }
                else
                {
                    dcin_sts->adc_freq    = 8 + 8 * adcsw;
                }
                hstr.data       = &tmp_value;
                if(dcin_sts->dcin_voladc_enable)
                {
                    byte_addr  = POWER18_DCIN_VOL;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    dcin_sts->dcin_vol = 3750 + tmp_value * 12;
                }

                if(dcin_sts->dcin_crntadc_enable)
                {
                    byte_addr  = POWER18_DCIN_CURRENT;
                    hstr.byte_addr  = &byte_addr;

                    if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    hstr.data       = &tmp_value;
                    if(AXP189_Version & 0x10)
                    {
                        dcin_sts->dcin_crnt = tmp_value * 667 / 100;    //C版AXP189适用
                    }
                    else
                    {
                        dcin_sts->dcin_crnt = tmp_value << 2;
                    }
                }
            }

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_DCINADC_SET:
        {
            __drv_power_dcin_status_t   *dcin_sts = (__drv_power_dcin_status_t *)pbuffer;

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;               
            byte_addr          = POWER18_ADCSW_CTL;   
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte 

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            if(dcin_sts->adc_freq == 25)
            {
                tmp_value &= ~(0x03 << 1);
                tmp_value |=   0x02 << 1;
            }
            else if((dcin_sts->adc_freq == 16) || (dcin_sts->adc_freq == 8) || (dcin_sts->adc_freq == 32))
            {
                tmp_value &= ~(0x03 << 1);
                tmp_value |= ((dcin_sts->adc_freq>>3) - 1) << 1;
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

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_CHARGE_GET:
        {
            __drv_power_charge_status_t  *charge = (__drv_power_charge_status_t *)pbuffer;
            __u8    tmp1, tmp3, tmp4, tmp5;

            eLIBs_memset(charge, 0, sizeof(__drv_power_charge_status_t));

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;               
            byte_addr          = POWER18_STATUS;      
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
            hstr.data               = &tmp1;
            hstr.byte_count         = 1;  // data is 1 byte 

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            if(tmp1 & 0x01)
            {
                tmp1 &= 0x01;

                hstr.data       = &tmp3;
                byte_addr  = POWER18_ONOFF;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                hstr.data       = &tmp4;
                byte_addr  = POWER18_CHARGE1;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                hstr.data       = &tmp5;
                byte_addr  = POWER18_CHARGE2;
                hstr.byte_addr  = &byte_addr;
                if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                {
                    break;
                }

                if(tmp4 & 0x80)
                {
                    charge->charge_enable      = 1;
                    if(tmp3 & 0x04)
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

                charge->charge_end         = (tmp3 >> 6) & 0x01;

                #if 0   //AXP189 has no auto-limit function
                charge->charge_autolimit   = (tmp4 >> 3) & 0x01;
                #else
                charge->charge_autolimit   = 0;
                #endif

                tmp_value                  = (tmp4 >> 5) & 0x03;
                if(tmp_value == 3)
                {
                    charge->charge_vtarget = 4360;
                }
                else if(tmp_value == 0)
                {
                    charge->charge_vtarget = 4100;
                }
                else
                {
                    charge->charge_vtarget = 4200;
                }

                tmp_value                  = (tmp4 >> 0) & 0x07;
                if(AXP189_Version & 0x10)        //C 版
                {
                    charge->charge_current = 100 + tmp_value * 200;
                }
                else                                            //B版
                {
                    if(tmp_value == 0)
                    {
                        charge->charge_current = 80;
                    }
                    else
                    {
                        charge->charge_current = 20 + tmp_value * 100;
                    }
                }

                tmp_value                  = (tmp5 >> 6) & 0x03;
                charge->charge_timer1      =  30 + tmp_value * 10;

                tmp_value                  = (tmp5 >> 0) & 0x03;
                charge->charge_timer2      = 330 + tmp_value * 30;
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
            __u8    tmp3, tmp4, tmp5;

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;               
            byte_addr          = POWER18_ONOFF;     
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
            hstr.data               = &tmp3;
            hstr.byte_count         = 1;  // data is 1 byte 

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            hstr.data       = &tmp4;
            byte_addr  = POWER18_CHARGE1;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            hstr.data       = &tmp5;
            byte_addr  = POWER18_CHARGE2;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }

            if(charge->charge_end < 0x02)
            {
                tmp3 &= ~(1<< 6);
                tmp3 |=  charge->charge_end << 6;
            }

            #if 0   //AXP189 has no auto-limit function
            if(charge->charge_autolimit < 0x02)
            {
                tmp4 &= ~(1<<3);
                tmp4  = charge->charge_autolimit << 3;
            }
            #endif

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

            if(AXP189_Version & 0x10)        //C 版
            {
                tmp4 &= ~(7 << 0);
                tmp4 |= (charge->charge_current - 100)/200;
            }
            else                            //B 版
            {
                if(charge->charge_current == 80)
                {
                    tmp4 &= ~(7 << 0);
                    tmp4 |= 0 << 0;
                }
                else if((charge->charge_current >= 120) && (charge->charge_current <= 720) \
                    && (!((charge->charge_current - 20)%100)))
                {
                    tmp4 &= ~(7 << 0);
                    tmp4 |= (charge->charge_current - 20)/100;
                }
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

            if(AXP189_Version & 0x10)        //C 版
            {
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
            }
            else
            {
                if(charge->charge_timer2 == 330)
                {
                    tmp5 &= (3 << 0);
                    tmp5 |= 0 << 0;
                }
                else if(charge->charge_timer2 == 360)
                {
                    tmp5 &= (3 << 0);
                    tmp5 |= 1 << 0;
                }
                else if(charge->charge_timer2 == 390)
                {
                    tmp5 &= (3 << 0);
                    tmp5 |= 2 << 0;
                }
                else if(charge->charge_timer2 == 420)
                {
                    tmp5 &= (3 << 0);
                    tmp5 |= 3 << 0;
                }
            }

            hstr.data       = &tmp3;
            byte_addr  = POWER18_ONOFF;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }

            hstr.data       = &tmp4;
            byte_addr  = POWER18_CHARGE1;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
            {
                break;
            }

            hstr.data       = &tmp5;
            byte_addr  = POWER18_CHARGE2;
            hstr.byte_addr  = &byte_addr;
            ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, POWER_BOARD1_DEV_ADDR_18, &hstr);

            break;
        }

        case DRV_POWER_CMD_BASIC_STATUS:
        {
            __drv_power_basic_status_t  *pbasic = (__drv_power_basic_status_t *)pbuffer;
            __u8       tmp1, tmp2, tmp3, tmp4;

            eLIBs_memset(pbasic, 0, sizeof(__drv_power_basic_status_t));
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;               
            byte_addr          = POWER18_STATUS;       
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
            hstr.data               = &tmp1;
            hstr.byte_count         = 1;  // data is 1 byte             

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            hstr.data       = &tmp2;
            byte_addr  = POWER18_IPS_SET;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            hstr.data       = &tmp3;
            byte_addr  = POWER18_ONOFF;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            hstr.data       = &tmp4;
            byte_addr  = POWER18_CHARGE1;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            //pbasic->usb_charge  = usb_charge_exist;
            //pbasic->acin_input  = (tmp1 >> 7) & 0x01;
            pbasic->vout_exist  = (tmp3 >> 3) & 0x01;
            pbasic->vout_enable = (tmp1 >> 4) & 0x01;
            pbasic->vout_choose = (tmp2 >> 7) & 0x01;
            pbasic->bat_exist   = (tmp1 >> 0) & 0x01;
            pbasic->bat_work_status   = (tmp3 >> 2) & 0x01;
            pbasic->bat_charge_status = (tmp4 >> 4) & 0x01;

            if(!pbasic->bat_charge_status)          //在充电
            {
                if((tmp1 >> 7) & 0x01)              //园口充电
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
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;               
            byte_addr          = POWER18_IPS_SET;      
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte           
  
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            pips->dcin_choose  = ((tmp_value >> 7) & 0x01);
            pips->vhold_mode   = ((tmp_value >> 6) & 0x01);
            pips->vhold        = 4400 + 100 * ((tmp_value >> 4) & 0x03);
            pips->vwarning     = 3350 +  50 * ((tmp_value >> 2) & 0x03);
            pips->voff         = 3150 +  50 * ((tmp_value >> 0) & 0x03);

            ret = EPDK_OK;
            break;
        }

        case DRV_POWER_CMD_IPS_SET:
        {
            __drv_power_ips_status_t  *pips = (__drv_power_ips_status_t *)pbuffer;

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;               
            byte_addr          = POWER18_IPS_SET;     
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte    

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
            if((pips->vhold == 4400) || (pips->vhold == 4500) || (pips->vhold == 4600) || (pips->vhold == 4700))
            {
                tmp_value &= ~(0x03 << 4);
                tmp_value |= ((pips->vhold - 4400)/100) << 4;
            }
            if((pips->vwarning == 3350) || (pips->vwarning == 3400) || (pips->vwarning == 3450) || (pips->vwarning == 3500))
            {
                tmp_value &= ~(0x03 << 2);
                tmp_value |= ((pips->vwarning - 3350)/50) << 2;
            }

            if(AXP189_Version & 0x10)
            {
                switch(pips->voff)
                {
                    case 2400:
                    {
                        tmp_value &= ~(0x03 << 0);
                        tmp_value |= 0x00;
                        break;
                    }
                    case 2900:
                    {
                        tmp_value &= ~(0x03 << 0);
                        tmp_value |= 0x01;
                        break;
                    }
                    case 3050:
                    {
                        tmp_value &= ~(0x03 << 0);
                        tmp_value |= 0x02;
                        break;
                    }
                    case 3200:
                    {
                        tmp_value &= ~(0x03 << 0);
                        tmp_value |= 0x03;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }

            else if((pips->voff == 3150) || (pips->voff == 3200) || (pips->voff == 3250) || (pips->voff == 3300))
            {
                tmp_value &= ~(0x03 << 0);
                tmp_value |= (pips->voff - 3150)/50;
            }
            ret = eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

            break;
        }

        case DRV_POWER_CMD_GET_DCIN:
        {
            __drv_power_battery_status_t3   *pstatus = (__drv_power_battery_status_t3 *)pbuffer;

            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;               
            byte_addr          = POWER18_STATUS;            
            hstr.byte_addr  = &byte_addr;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte       
            hstr.data               = &tmp_value;
            hstr.byte_count         = 1;  // data is 1 byte  

            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            pstatus->acin_en = (tmp_value >> 7) & 0x01;
            pstatus->dcin_valid = (tmp_value >> 4) & 0x01;

            byte_addr  = POWER18_ONOFF;
            hstr.byte_addr  = &byte_addr;
            if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
            {
                break;
            }
            pstatus->charge_status = (tmp_value >> 2) & 0x01;

            ret = EPDK_OK;
            break;
        }
        case DRV_POWER_CMD_GET_FUELGUAGE:
        {
            __drv_power_fuelguage_t   *pbat = (__drv_power_fuelguage_t *)pbuffer;

            pbat->charge_status = fuelguage.charge_status;
            pbat->rest_vol      = fuelguage.rest_vol;
            pbat->rest_time     = fuelguage.rest_time;

            rest_time_cal_18(aux);
            pbat->rest_time_music = fuelguage.rest_time_music;
            pbat->rest_time_video = fuelguage.rest_time_video;
            pbat->rest_time_pic = fuelguage.rest_time_pic;
            pbat->rest_time_book = fuelguage.rest_time_book;
            pbat->rest_time_standby = fuelguage.rest_time_standby;

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
			__u8 reg =(__u8 )aux;
			
			hstr.slave_addr		 = POWER_BOARD1_DEV_ADDR_18;
			hstr.slave_addr_flag	 = TWI_SLAVE_ADDRESS_07BIT;
			hstr.byte_addr_width	 = 1;  // reg_addr is 1 byte
			hstr.data				 = &tmp_value;
			hstr.byte_count		 = 1;  // data is 1 byte
			
			byte_addr	= POWER18_PEK;
			hstr.byte_addr  = &byte_addr;
			if(EPDK_FAIL == eLIBs_fioctrl(hdev->iic_file, TWI_READ_SPEC_RS, 0, &hstr))
			{
				 break;
			}
			tmp_value &= 0x3f;
			tmp_value |= reg; 						
			eLIBs_fioctrl(hdev->iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
			
			break;
		}//added by zhangdonglu 110520
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

__dev_devop_t power_18_dev_ops =
{
    DEV_Power_18_Open,
    DEV_Power_18_Close,
    DEV_Power_18_Read,
    DEV_Power_18_Write,
    DEV_Power_18_Ioctrl
};
