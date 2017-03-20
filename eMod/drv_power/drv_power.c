/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                               hello world sample
*
*                                (c) Copyright 2006-2007, Steven.ZGJ China
*                                            All    Rights Reserved
*
* File    : drv_power.c
* By      : evan
* Version : V1.00
*********************************************************************************************************
*/
#include "drv_power_i.h"
#include "power_script.h"
#include "powerkey_dev.h"

#define ABS(x)                  ( (x) >0 ? (x) : -(x) )
	#define POWER20_VOL_MAX         50
#define POWER20_TIME_MAX        5
#define POWER19_VOL_MAX         50
#define POWER19_TIME_MAX        5
#define POWER18_VOL_MAX         1200
#define POWER18_TIME_MAX        100
#define POWER_AVER_MAX          10
#define P1CURRENT_KEYWORD_CNT   (62)
#define BAT_FULL_VOLT						4150
#define BAT_FULL_CURR						120
#define BAT_FULL_CORRECT_RATE		98
#define BAT_LOW_VOLT						3560
#define BAT_ULOW_VOLT						3530
#define BAT_LOW_CORRECT_RATE		4
#define MAX_BUFFER							50


#define GUARD_OF_ROV       (10)
static  __bool  first_setup_system = EPDK_TRUE;

extern __u8             AXP209_Version;
extern __u8             AXP199_Version;
extern __u8             AXP189_Version;
extern __u8             standby;

__power_drv_t               powerdrv;
__drv_power_fuelguage_t     fuelguage;

__s32       PMU_exist = 0;
__u8        PMU_type = 0;
__u32       short_key_value;
__s32       board;
__s32       this_rdc;
__u8        coulomb_flag;
__u32       Tcv = 0;
__u8        standby = 0;


static __krnl_event_t   *p1lock;
static __s32            oldisr;
static __u8             tid;


//******************************************************************************
//获取脚本信息
//******************************************************************************
static __p1current_script_psr_t     psr_script;
__sys_p1current_cfg_t        P1Cfg, *pP1CurrentCfg;

static __s32 _kw_videocurrent_proc(void);
static __s32 _kw_musiccurrent_proc(void);
static __s32 _kw_picturecurrent_proc(void);
static __s32 _kw_ebookcurrent_proc(void);
static __s32 _kw_standbycurrent_proc(void);
static __s32 _skip_remark_line(void);
static __s32 _P1currentScriptExit(void);
static __s32 _GetScriptStr(void);
static __s32 _ProcScriptStr(void);
static __s32 _P1currentScriptInit(void);
static __s32 InitP1currentCfgPar(void);
static void  Init_AXP19x_MainTask(void);
static void  Init_AXP20x_MainTask(void);
static __u16 Get_Rdc(void);
static void Config_for_gedmei(void);

const __s8 valid_str_tbl[256] =
{
    //      0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
    /*0*/   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*1*/   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*2*/   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*3*/   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    /*4*/   0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /*5*/   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1,
    /*6*/   0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /*7*/   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
};


__p1current_script_func_mapt_t  func_map[] =
{
    {"video_current_level01",_kw_videocurrent_proc  },
    {"video_current_level02",_kw_videocurrent_proc  },
    {"video_current_level03",_kw_videocurrent_proc  },
    {"video_current_level04",_kw_videocurrent_proc  },
    {"video_current_level05",_kw_videocurrent_proc  },
    {"video_current_level06",_kw_videocurrent_proc  },
    {"video_current_level07",_kw_videocurrent_proc  },
    {"video_current_level08",_kw_videocurrent_proc  },
    {"video_current_level09",_kw_videocurrent_proc  },
    {"video_current_level10",_kw_videocurrent_proc  },
    {"video_current_level11",_kw_videocurrent_proc  },
    {"video_current_level12",_kw_videocurrent_proc  },
    {"video_current_level13",_kw_videocurrent_proc  },
    {"video_current_level14",_kw_videocurrent_proc  },
    {"video_current_level15",_kw_videocurrent_proc  },
    {"video_current_level16",_kw_videocurrent_proc  },
    {"video_current_level17",_kw_videocurrent_proc  },
    {"video_current_level18",_kw_videocurrent_proc  },
    {"video_current_level19",_kw_videocurrent_proc  },
    {"video_current_level20",_kw_videocurrent_proc  },

    {"music_current",_kw_musiccurrent_proc  },

    {"picture_current_level01",_kw_picturecurrent_proc  },
    {"picture_current_level02",_kw_picturecurrent_proc  },
    {"picture_current_level03",_kw_picturecurrent_proc  },
    {"picture_current_level04",_kw_picturecurrent_proc  },
    {"picture_current_level05",_kw_picturecurrent_proc  },
    {"picture_current_level06",_kw_picturecurrent_proc  },
    {"picture_current_level07",_kw_picturecurrent_proc  },
    {"picture_current_level08",_kw_picturecurrent_proc  },
    {"picture_current_level09",_kw_picturecurrent_proc  },
    {"picture_current_level10",_kw_picturecurrent_proc  },
    {"picture_current_level11",_kw_picturecurrent_proc  },
    {"picture_current_level12",_kw_picturecurrent_proc  },
    {"picture_current_level13",_kw_picturecurrent_proc  },
    {"picture_current_level14",_kw_picturecurrent_proc  },
    {"picture_current_level15",_kw_picturecurrent_proc  },
    {"picture_current_level16",_kw_picturecurrent_proc  },
    {"picture_current_level17",_kw_picturecurrent_proc  },
    {"picture_current_level18",_kw_picturecurrent_proc  },
    {"picture_current_level19",_kw_picturecurrent_proc  },
    {"picture_current_level20",_kw_picturecurrent_proc  },

    {"ebook_current_level01",_kw_ebookcurrent_proc  },
    {"ebook_current_level02",_kw_ebookcurrent_proc  },
    {"ebook_current_level03",_kw_ebookcurrent_proc  },
    {"ebook_current_level04",_kw_ebookcurrent_proc  },
    {"ebook_current_level05",_kw_ebookcurrent_proc  },
    {"ebook_current_level06",_kw_ebookcurrent_proc  },
    {"ebook_current_level07",_kw_ebookcurrent_proc  },
    {"ebook_current_level08",_kw_ebookcurrent_proc  },
    {"ebook_current_level09",_kw_ebookcurrent_proc  },
    {"ebook_current_level10",_kw_ebookcurrent_proc  },
    {"ebook_current_level11",_kw_ebookcurrent_proc  },
    {"ebook_current_level12",_kw_ebookcurrent_proc  },
    {"ebook_current_level13",_kw_ebookcurrent_proc  },
    {"ebook_current_level14",_kw_ebookcurrent_proc  },
    {"ebook_current_level15",_kw_ebookcurrent_proc  },
    {"ebook_current_level16",_kw_ebookcurrent_proc  },
    {"ebook_current_level17",_kw_ebookcurrent_proc  },
    {"ebook_current_level18",_kw_ebookcurrent_proc  },
    {"ebook_current_level19",_kw_ebookcurrent_proc  },
    {"ebook_current_level20",_kw_ebookcurrent_proc  },

    {"standby_current",_kw_standbycurrent_proc  },
};


/*
*********************************************************************************************************
*                                     GET PARAMETER VALUE
*
* Description: get parameter value.
*
* Arguments  : none
*
* Returns    : parameter value;
*********************************************************************************************************
*/
static __u32 _get_para_value(void)
{
    __u32       value = 0;

    //skip space or table
    while((*(char *)psr_script.cur_ptr == '\t') || (*(char *)psr_script.cur_ptr == ' '))
    {
        psr_script.cur_ptr++;
        if(psr_script.cur_ptr >= psr_script.script_end)
        {
            return 0;
        }
    }

    //check '='
    if(*psr_script.cur_ptr != '=')
    {
        return 0;
    }
    psr_script.cur_ptr++;

    //skip space or table
    while((*(char *)psr_script.cur_ptr == '\t') || (*(char *)psr_script.cur_ptr == ' '))
    {
        psr_script.cur_ptr++;
        if(psr_script.cur_ptr >= psr_script.script_end)
        {
            return 0;
        }
    }

    //get parameter value
    while((*(char *)psr_script.cur_ptr >= '0') && (*(char *)psr_script.cur_ptr <= '9'))
    {
        value = value*10 + (*psr_script.cur_ptr - '0');

        psr_script.cur_ptr ++;
    }

    return value;
}



/*
*********************************************************************************************************
*                                     KEY WORD VIDEOCURRENT PROCESS
*
* Description: key word videocurrent process.
*
* Arguments  : none
*
* Returns    : key word videocurrent process result;
*               EPDK_OK     -   key word videocurrent process successed,
*               EPDK_FAIL   -   key word videocurrent process failed;
*********************************************************************************************************
*/
static __s32 _kw_videocurrent_proc(void)
{
//    __inf("P1 videocurrent process! \n");
    if((psr_script.string[19] == '0') && (psr_script.string[20] == '1'))
    {
        pP1CurrentCfg->videocurrent.level1 = _get_para_value();
//        __inf("p1 after videocurrent.level1 %d!\n",pP1CurrentCfg->videocurrent.level1);
    }
    else if ((psr_script.string[19] == '0') && (psr_script.string[20] == '2'))
    {
        pP1CurrentCfg->videocurrent.level2 = _get_para_value();
//        __inf("p1 videocurrent.level2 %d!\n",pP1CurrentCfg->videocurrent.level2);
    }
    else if ((psr_script.string[19] == '0') && (psr_script.string[20] == '3'))
    {
        pP1CurrentCfg->videocurrent.level3 = _get_para_value();
//        __inf("p1 videocurrent.level3 %d!\n",pP1CurrentCfg->videocurrent.level3);
    }
    else if ((psr_script.string[19] == '0') && (psr_script.string[20] == '4'))
    {
        pP1CurrentCfg->videocurrent.level4 = _get_para_value();
//        __inf("p1 videocurrent.level4 %d!\n",pP1CurrentCfg->videocurrent.level4);
    }
    else if ((psr_script.string[19] == '0') && (psr_script.string[20] == '5'))
    {
        pP1CurrentCfg->videocurrent.level5 = _get_para_value();
//        __inf("p1 videocurrent.level5 %d!\n",pP1CurrentCfg->videocurrent.level5);
    }
    else if ((psr_script.string[19] == '0') && (psr_script.string[20] == '6'))
    {
        pP1CurrentCfg->videocurrent.level6 = _get_para_value();
//        __inf("p1 videocurrent.level6 %d!\n",pP1CurrentCfg->videocurrent.level6);
    }
    else if ((psr_script.string[19] == '0') && (psr_script.string[20] == '7'))
    {
        pP1CurrentCfg->videocurrent.level7 = _get_para_value();
//         __inf("p1 videocurrent.level7 %d!\n",pP1CurrentCfg->videocurrent.level7);
    }
    else if ((psr_script.string[19] == '0') && (psr_script.string[20] == '8'))
    {
        pP1CurrentCfg->videocurrent.level8 = _get_para_value();
//        __inf("p1 videocurrent.level8 %d!\n",pP1CurrentCfg->videocurrent.level8);
    }
    else if ((psr_script.string[19] == '0') && (psr_script.string[20] == '9'))
    {
        pP1CurrentCfg->videocurrent.level9 = _get_para_value();
//        __inf("p1 videocurrent.level9 %d!\n",pP1CurrentCfg->videocurrent.level9);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '0'))
    {
        pP1CurrentCfg->videocurrent.level10 = _get_para_value();
//         __inf("p1 videocurrent.level10 %d!\n",pP1CurrentCfg->videocurrent.level10);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '1'))
    {
        pP1CurrentCfg->videocurrent.level11 = _get_para_value();
//        __inf("p1 videocurrent.level11 %d!\n",pP1CurrentCfg->videocurrent.level11);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '2'))
    {
        pP1CurrentCfg->videocurrent.level12 = _get_para_value();
//        __inf("p1 videocurrent.level12 %d!\n",pP1CurrentCfg->videocurrent.level12);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '3'))
    {
        pP1CurrentCfg->videocurrent.level13 = _get_para_value();
//        __inf("p1 videocurrent.level13 %d!\n",pP1CurrentCfg->videocurrent.level13);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '4'))
    {
        pP1CurrentCfg->videocurrent.level14 = _get_para_value();
//        __inf("p1 videocurrent.level14 %d!\n",pP1CurrentCfg->videocurrent.level14);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '5'))
    {
        pP1CurrentCfg->videocurrent.level15 = _get_para_value();
//        __inf("p1 videocurrent.level15 %d!\n",pP1CurrentCfg->videocurrent.level15);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '6'))
    {
        pP1CurrentCfg->videocurrent.level16 = _get_para_value();
//        __inf("p1 videocurrent.level16 %d!\n",pP1CurrentCfg->videocurrent.level16);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '7'))
    {
        pP1CurrentCfg->videocurrent.level17 = _get_para_value();
//         __inf("p1 videocurrent.level17 %d!\n",pP1CurrentCfg->videocurrent.level17);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '8'))
    {
        pP1CurrentCfg->videocurrent.level18 = _get_para_value();
//        __inf("p1 videocurrent.level18 %d!\n",pP1CurrentCfg->videocurrent.level18);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '9'))
    {
        pP1CurrentCfg->videocurrent.level19 = _get_para_value();
//        __inf("p1 videocurrent.level19 %d!\n",pP1CurrentCfg->videocurrent.level19);
    }
    else if ((psr_script.string[19] == '2') && (psr_script.string[20] == '0'))
    {
        pP1CurrentCfg->videocurrent.level20 = _get_para_value();
//        __inf("p1 videocurrent.level20 %d!\n",pP1CurrentCfg->videocurrent.level20);
    }
    else
    {
        __inf("p1 videocurrent fail!\n");
        return  EPDK_FAIL;
    }

    return EPDK_OK;
}


/*
*********************************************************************************************************
*                                     KEY WORD MUSICCURRENT PROCESS
*
* Description: key word musiccurrent process.
*
* Arguments  : none
*
* Returns    : key word musiccurrent process result;
*               EPDK_OK     -   key word musiccurrent process successed,
*               EPDK_FAIL   -   key word musiccurrent process failed;
*********************************************************************************************************
*/
static __s32 _kw_musiccurrent_proc(void)
{
//    __inf("P1 musiccurrent process! \n");
    pP1CurrentCfg->musiccurrent = _get_para_value();
//    __inf("P1 musiccurrent=%d \n",pP1CurrentCfg->musiccurrent);
    return EPDK_OK;
}


/*
*********************************************************************************************************
*                                     KEY WORD PICTURECURRENT PROCESS
*
* Description: key word picturecurrent process.
*
* Arguments  : none
*
* Returns    : key word picturecurrent process result;
*               EPDK_OK     -   key word picturecurrent process successed,
*               EPDK_FAIL   -   key word picturecurrent process failed;
*********************************************************************************************************
*/

static __s32 _kw_picturecurrent_proc(void)
{
    if ((psr_script.string[21] == '0') && (psr_script.string[22] == '1'))
    {
        pP1CurrentCfg->picturecurrent.level1 = _get_para_value();
//         __inf("p1 picturecurrent.level1 %d!\n",pP1CurrentCfg->picturecurrent.level1);
    }
    else if ((psr_script.string[21] == '0') && (psr_script.string[22] == '2'))
    {
        pP1CurrentCfg->picturecurrent.level2 = _get_para_value();
//        __inf("p1 picturecurrent.level2 %d!\n",pP1CurrentCfg->picturecurrent.level12);
    }
    else if((psr_script.string[21] == '0') && (psr_script.string[22] == '3'))
    {
        pP1CurrentCfg->picturecurrent.level3 = _get_para_value();
//        __inf("p1 picturecurrent.level3 %d!\n",pP1CurrentCfg->picturecurrent.level3);
    }
    else if ((psr_script.string[21] == '0') && (psr_script.string[22] == '4'))
    {
        pP1CurrentCfg->picturecurrent.level4 = _get_para_value();
//        __inf("p1 picturecurrent.level4 %d!\n",pP1CurrentCfg->picturecurrent.level4);
    }
    else if ((psr_script.string[21] == '0') && (psr_script.string[22] == '5'))
    {
        pP1CurrentCfg->picturecurrent.level5 = _get_para_value();
//        __inf("p1 picturecurrent.level5 %d!\n",pP1CurrentCfg->picturecurrent.level5);
    }
    else if ((psr_script.string[21] == '0') && (psr_script.string[22] == '6'))
    {
        pP1CurrentCfg->picturecurrent.level6 = _get_para_value();
//        __inf("p1 picturecurrent.level6 %d!\n",pP1CurrentCfg->picturecurrent.level6);
    }
    else if ((psr_script.string[21] == '0') && (psr_script.string[22] == '7'))
    {
        pP1CurrentCfg->picturecurrent.level7 = _get_para_value();
//        __inf("p1 picturecurrent.level7 %d!\n",pP1CurrentCfg->picturecurrent.level7);
    }
    else if ((psr_script.string[21] == '0') && (psr_script.string[22] == '8'))
    {
        pP1CurrentCfg->picturecurrent.level8 = _get_para_value();
//        __inf("p1 picturecurrent.level8 %d!\n",pP1CurrentCfg->picturecurrent.level8);
    }
    else if ((psr_script.string[21] == '0') && (psr_script.string[22] == '9'))
    {
        pP1CurrentCfg->picturecurrent.level9 = _get_para_value();
//        __inf("p1 picturecurrent.level9 %d!\n",pP1CurrentCfg->picturecurrent.level9);
    }
    else if ((psr_script.string[21] == '1') && (psr_script.string[22] == '0'))
    {
        pP1CurrentCfg->picturecurrent.level10 = _get_para_value();
//        __inf("p1 picturecurrent.level10 %d!\n",pP1CurrentCfg->picturecurrent.level10);
    }
    else if ((psr_script.string[21] == '1') && (psr_script.string[22] == '1'))
    {
        pP1CurrentCfg->picturecurrent.level11 = _get_para_value();
//        __inf("p1 picturecurrent.level11 %d!\n",pP1CurrentCfg->picturecurrent.level11);
    }
    else if ((psr_script.string[21] == '1') && (psr_script.string[22] == '2'))
    {
        pP1CurrentCfg->picturecurrent.level12 = _get_para_value();
//        __inf("p1 picturecurrent.level12 %d!\n",pP1CurrentCfg->picturecurrent.level12);
    }
    else if ((psr_script.string[21] == '1') && (psr_script.string[22] == '3'))
    {
        pP1CurrentCfg->picturecurrent.level13 = _get_para_value();
//        __inf("p1 picturecurrent.level13 %d!\n",pP1CurrentCfg->picturecurrent.level13);
    }
    else if ((psr_script.string[21] == '1') && (psr_script.string[22] == '4'))
    {
        pP1CurrentCfg->picturecurrent.level14 = _get_para_value();
//        __inf("p1 picturecurrent.level14 %d!\n",pP1CurrentCfg->picturecurrent.level14);
    }
    else if ((psr_script.string[21] == '1') && (psr_script.string[22] == '5'))
    {
        pP1CurrentCfg->picturecurrent.level15 = _get_para_value();
//        __inf("p1 picturecurrent.level15 %d!\n",pP1CurrentCfg->picturecurrent.level15);
    }
    else if ((psr_script.string[21] == '1') && (psr_script.string[22] == '6'))
    {
        pP1CurrentCfg->picturecurrent.level16 = _get_para_value();
//        __inf("p1 picturecurrent.level16 %d!\n",pP1CurrentCfg->picturecurrent.level16);
    }
    else if((psr_script.string[21] == '1') && (psr_script.string[22] == '7'))
    {
        pP1CurrentCfg->picturecurrent.level17 = _get_para_value();
//        __inf("p1 picturecurrent.level17 %d!\n",pP1CurrentCfg->picturecurrent.level17);
    }
    else if ((psr_script.string[21] == '1') && (psr_script.string[22] == '8'))
    {
        pP1CurrentCfg->picturecurrent.level18 = _get_para_value();
//        __inf("p1 picturecurrent.level18 %d!\n",pP1CurrentCfg->picturecurrent.level18);
    }
    else if ((psr_script.string[21] == '1') && (psr_script.string[22] == '9'))
    {
        pP1CurrentCfg->picturecurrent.level19 = _get_para_value();
//        __inf("p1 picturecurrent.level19 %d!\n",pP1CurrentCfg->picturecurrent.level19);
    }
    else if ((psr_script.string[21] == '2') && (psr_script.string[22] == '0'))
    {
        pP1CurrentCfg->picturecurrent.level20 = _get_para_value();
//        __inf("p1 picturecurrent.level20 %d!\n",pP1CurrentCfg->picturecurrent.level20);
    }
    else
    {
        return  EPDK_FAIL;
    }

    return EPDK_OK;
}


/*
*********************************************************************************************************
*                                     KEY WORD EBOOKCURRENT PROCESS
*
* Description: key word ebookcurrent process.
*
* Arguments  : none
*
* Returns    : key word ebookcurrent process result;
*               EPDK_OK     -   key word ebookcurrent process successed,
*               EPDK_FAIL   -   key word ebookcurrent process failed;
*********************************************************************************************************
*/

static __s32 _kw_ebookcurrent_proc(void)
{
    if ((psr_script.string[19] == '0') && (psr_script.string[20] == '1'))
    {
        pP1CurrentCfg->ebookcurrent.level1 = _get_para_value();
//        __inf("p1 ebookcurrent.level1 %d!\n",pP1CurrentCfg->ebookcurrent.level1);
    }
    else if ((psr_script.string[19] == '0') && (psr_script.string[20] == '2'))
    {
        pP1CurrentCfg->ebookcurrent.level2 = _get_para_value();
//        __inf("p1 ebookcurrent.level2 %d!\n",pP1CurrentCfg->ebookcurrent.level2);
    }
    else if ((psr_script.string[19] == '0') && (psr_script.string[20] == '3'))
    {
        pP1CurrentCfg->ebookcurrent.level3 = _get_para_value();
//        __inf("p1 ebookcurrent.level3 %d!\n",pP1CurrentCfg->ebookcurrent.level3);
    }
    else if ((psr_script.string[19] == '0') && (psr_script.string[20] == '4'))
    {
        pP1CurrentCfg->ebookcurrent.level4 = _get_para_value();
//        __inf("p1 ebookcurrent.level4 %d!\n",pP1CurrentCfg->ebookcurrent.level4);
    }
    else if ((psr_script.string[19] == '0') && (psr_script.string[20] == '5'))
    {
        pP1CurrentCfg->ebookcurrent.level5 = _get_para_value();
//        __inf("p1 ebookcurrent.level5 %d!\n",pP1CurrentCfg->ebookcurrent.level5);
    }
    else if ((psr_script.string[19] == '0') && (psr_script.string[20] == '6'))
    {
        pP1CurrentCfg->ebookcurrent.level6 = _get_para_value();
//        __inf("p1 ebookcurrent.level6 %d!\n",pP1CurrentCfg->ebookcurrent.level6);
    }
    else if ((psr_script.string[19] == '0') && (psr_script.string[20] == '7'))
    {
        pP1CurrentCfg->ebookcurrent.level7 = _get_para_value();
//        __inf("p1 ebookcurrent.level7 %d!\n",pP1CurrentCfg->ebookcurrent.level7);
    }
    else if ((psr_script.string[19] == '0') && (psr_script.string[20] == '8'))
    {
        pP1CurrentCfg->ebookcurrent.level8 = _get_para_value();
//        __inf("p1 ebookcurrent.level8 %d!\n",pP1CurrentCfg->ebookcurrent.level8);
    }
    else if ((psr_script.string[19] == '0') && (psr_script.string[20] == '9'))
    {
        pP1CurrentCfg->ebookcurrent.level9 = _get_para_value();
//        __inf("p1 ebookcurrent.level9 %d!\n",pP1CurrentCfg->ebookcurrent.level9);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '0'))
    {
        pP1CurrentCfg->ebookcurrent.level10 = _get_para_value();
//        __inf("p1 ebookcurrent.level10 %d!\n",pP1CurrentCfg->ebookcurrent.level10);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '1'))
    {
        pP1CurrentCfg->ebookcurrent.level11 = _get_para_value();
//        __inf("p1 ebookcurrent.level11 %d!\n",pP1CurrentCfg->ebookcurrent.level11);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '2'))
    {
        pP1CurrentCfg->ebookcurrent.level12 = _get_para_value();
//        __inf("p1 ebookcurrent.level12 %d!\n",pP1CurrentCfg->ebookcurrent.level12);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '3'))
    {
        pP1CurrentCfg->ebookcurrent.level13 = _get_para_value();
//        __inf("p1 ebookcurrent.level13 %d!\n",pP1CurrentCfg->ebookcurrent.level13);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '4'))
    {
        pP1CurrentCfg->ebookcurrent.level14 = _get_para_value();
//        __inf("p1 ebookcurrent.level14 %d!\n",pP1CurrentCfg->ebookcurrent.level14);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '5'))
    {
        pP1CurrentCfg->ebookcurrent.level15 = _get_para_value();
//        __inf("p1 ebookcurrent.level15 %d!\n",pP1CurrentCfg->ebookcurrent.level15);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '6'))
    {
        pP1CurrentCfg->ebookcurrent.level16 = _get_para_value();
//        __inf("p1 ebookcurrent.level16 %d!\n",pP1CurrentCfg->ebookcurrent.level16);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '7'))
    {
        pP1CurrentCfg->ebookcurrent.level17 = _get_para_value();
//        __inf("p1 ebookcurrent.level17 %d!\n",pP1CurrentCfg->ebookcurrent.level17);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '8'))
    {
        pP1CurrentCfg->ebookcurrent.level18 = _get_para_value();
//        __inf("p1 ebookcurrent.level18 %d!\n",pP1CurrentCfg->ebookcurrent.level18);
    }
    else if ((psr_script.string[19] == '1') && (psr_script.string[20] == '9'))
    {
        pP1CurrentCfg->ebookcurrent.level19 = _get_para_value();
//        __inf("p1 ebookcurrent.level19 %d!\n",pP1CurrentCfg->ebookcurrent.level19);
    }
    else if ((psr_script.string[19] == '2') && (psr_script.string[20] == '0'))
    {
        pP1CurrentCfg->ebookcurrent.level20 = _get_para_value();
//        __inf("p1 ebookcurrent.level20 %d!\n",pP1CurrentCfg->ebookcurrent.level20);
    }
    else
    {
        return  EPDK_FAIL;
    }

    return EPDK_OK;
}


/*
*********************************************************************************************************
*                                     KEY WORD STANDBYCURRENT PROCESS
*
* Description: key word standbycurrent process.
*
* Arguments  : none
*
* Returns    : key word standbycurrent process result;
*               EPDK_OK     -   key word standbycurrent process successed,
*               EPDK_FAIL   -   key word standbycurrent process failed;
*********************************************************************************************************
*/
static __s32 _kw_standbycurrent_proc(void)
{
    pP1CurrentCfg->standbycurrent = _get_para_value();
//    __inf("P1 standbycurrent=%d \n",pP1CurrentCfg->standbycurrent);
    return EPDK_OK;
}


/*
*********************************************************************************************************
*                                     KEY WORD MODE PROCESS
*
* Description: key word mode process.
*
* Arguments  : none
*
* Returns    : key word mode process result;
*               EPDK_OK     -   key word mode process successed,
*               EPDK_FAIL   -   key word mode process failed;
*
* Note       : key word mode need be layout like this'[MODEXX]'
*********************************************************************************************************
*/
static __s32 _skip_remark_line(void)
{
    //skip remark text
    while((*(char *)psr_script.cur_ptr != NEWLINE) && (*(char *)psr_script.cur_ptr != ENTER))
    {
        psr_script.cur_ptr++;
        if(psr_script.cur_ptr >= psr_script.script_end)
        {
            return SCRIPT_PARSE_ERR_EOF;
        }
    }

    //skip blank lines
    while((*(char *)psr_script.cur_ptr == NEWLINE) || (*(char *)psr_script.cur_ptr == ENTER))
    {
        psr_script.cur_ptr++;
        if(psr_script.cur_ptr >= psr_script.script_end)
        {
            return SCRIPT_PARSE_ERR_EOF;
        }
    }

    return EPDK_OK;
}


/*
*********************************************************************************************************
*                                     P1CRRENT SCRIPT INIT
*
* Description: p1current script init.
*
* Arguments  : none
*
* Returns    : p1current script init result;
*               EPDK_OK     -   p1current script init successed,
*               EPDK_FAIL   -   p1current script init failed;
*********************************************************************************************************
*/
static __s32 _P1currentScriptInit(void)
{
    ES_FILE     *tmpFp;
    __s64       tmpSize;

    //clear script parser parameter
    eLIBs_memset(&psr_script, 0, sizeof(__p1current_script_psr_t));

    //open p1current script file
    tmpFp = eLIBs_fopen(P1CURRENT_SCRIPT_PATH, "rb");
    if(!tmpFp)
    {
        __err("Try to open p1current script file failed!\n");
    }

    eLIBs_fseek(tmpFp, 0, SEEK_END);
    tmpSize = eLIBs_ftell(tmpFp);
    if(!tmpSize)
    {
        __err("p1current script file is invalid!\n");
    }
    eLIBs_fseek(tmpFp, 0, SEEK_SET);
    psr_script.buf_size = (tmpSize+1023)/1024;

    psr_script.buf_start = esMEMS_Palloc(psr_script.buf_size, MEMS_PALLOC_MODE_BND_NONE|MEMS_PALLOC_MODE_BNK_NONE);
    if(!psr_script.buf_start)
    {
        __err("Request memory for read p1current script file failed!\n");
    }

    if(eLIBs_fread(psr_script.buf_start, 1, tmpSize, tmpFp) != tmpSize)
    {
        __err("Read p1current script file failed!\n");
    }
    psr_script.script_end = psr_script.buf_start + tmpSize;
    psr_script.cur_ptr    = psr_script.buf_start;

    //close p1current script
    eLIBs_fclose(tmpFp);

    return EPDK_OK;
}


/*
*********************************************************************************************************
*                                     P1CURRENT SCRIPT EXIT
*
* Description: p1current script exit.
*
* Arguments  : none
*
* Returns    : p1current script exit result;
*               EPDK_OK     -   p1current script exit successed,
*               EPDK_FAIL   -   p1current script exit failed;
*********************************************************************************************************
*/
static __s32 _P1currentScriptExit(void)
{
    if(psr_script.buf_start && psr_script.buf_size)
    {
        esMEMS_Pfree(psr_script.buf_start, psr_script.buf_size);
        psr_script.buf_start = 0;
        psr_script.buf_size = 0;
    }

    return EPDK_OK;
}


/*
*********************************************************************************************************
*                                     P1CURRENT SCRIPT INIT
*
* Description: p1current script init.
*
* Arguments  : none
*
* Returns    : p1current script init result;
*               EPDK_OK     -   p1current script init successed,
*               EPDK_FAIL   -   p1current script init failed;
*********************************************************************************************************
*/
static __s32 _GetScriptStr(void)
{
    __u8    *tmpStr = psr_script.string;

    //skip remark lines
    while(*psr_script.cur_ptr == REMARK)
    {
        if(_skip_remark_line() != EPDK_OK)
        {
            return EPDK_FAIL;
        }
    }

    //skip blank
    while((*(char *)psr_script.cur_ptr == '\t') || (*(char *)psr_script.cur_ptr == ' '))
    {
        psr_script.cur_ptr++;
        if(psr_script.cur_ptr >= psr_script.script_end)
        {
            return SCRIPT_PARSE_ERR_EOF;
        }
    }

    //get string
    while(valid_str_tbl[*(char *)psr_script.cur_ptr])
    {
        *tmpStr++ = *psr_script.cur_ptr;
        psr_script.cur_ptr++;
        if(psr_script.cur_ptr >= psr_script.script_end)
        {
            return SCRIPT_PARSE_ERR_EOF;
        }
    }

    *tmpStr = '\0';

    return EPDK_OK;
}



/*
*********************************************************************************************************
*                                     PROCESS P1CURRENT SCRIPT STRING
*
* Description: process p1current script string.
*
* Arguments  : none
*
* Returns    : process p1current string result;
*               EPDK_OK     -   process p1current script string successed,
*               EPDK_FAIL   -   process p1current script string failed;
*********************************************************************************************************
*/
static __s32 _ProcScriptStr(void)
{
    __s32       i;

    for(i=0; i<P1CURRENT_KEYWORD_CNT; i++)
    {
        if(eLIBs_strncmp(func_map[i].keystr, (const char *)psr_script.string, eLIBs_strlen(func_map[i].keystr)) == 0)
        {
            func_map[i].func();
            break;
        }
    }

    return _skip_remark_line();
}


/*
*********************************************************************************************************
*                                     INITIATE P1CURRENT PARAMETER WITH SCRIPT
*
* Description: initiate p1current parameter with script.
*
* Arguments  : none
*
* Returns    : initiate p1current parameter result;
*               EPDK_OK     -   initiate p1current parameter successed,
*               EPDK_FAIL   -   initiate p1current parameter failed;
*********************************************************************************************************
*/
__s32 InitP1currentCfgPar(void)
{
    __s32       result;

    //init script parser
    if(_P1currentScriptInit() != EPDK_OK)
    {
        __err("P1current script parser init failed!\n");
    }

    //parser script
    do
    {
        //get script string
        result = _GetScriptStr();
        if(result != EPDK_OK)
        {
            if(result == SCRIPT_PARSE_ERR_EOF)
            {
                break;
            }

            __err("get script string failed!\n");
        }

        //process script parameter
        result = _ProcScriptStr();
        if(result != EPDK_OK)
        {
            __wrn("process p1current script finished!\n");
            break;
        }

    }while(1);

    //exit script parser
    _P1currentScriptExit();

    return EPDK_OK;
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
static __u16 Get_Ocv_Bat_Vol(__u8 Flag,__u16 Bat_Vol,__u16 Bat_Cur,__u16 Rdc)
{
    if(Flag)
    {
        return Bat_Vol- (Bat_Cur*Rdc/1000);
    }
    else
    {
        return Bat_Vol+ (Bat_Cur*Rdc/1000);
    }
}

static __u8 Get_Rest_Vol_Rate(__u16 Bat_Ocv_Vol)
{
    if(Bat_Ocv_Vol > FUELGUAGE_TOP_VOL)         //4160
    {
        return FUELGUAGE_TOP_LEVEL;
    }
    else if(Bat_Ocv_Vol < FUELGUAGE_LOW_VOL)    //<3400
    {
        return FUELGUAGE_LOW_LEVEL;
    }
    else if(Bat_Ocv_Vol < FUELGUAGE_VOL1)       //3500
    {
        return FUELGUAGE_LOW_LEVEL + (FUELGUAGE_LEVEL1 - FUELGUAGE_LOW_LEVEL) * (Bat_Ocv_Vol - FUELGUAGE_LOW_VOL) / (FUELGUAGE_VOL1 - FUELGUAGE_LOW_VOL);
    }
    else if(Bat_Ocv_Vol < FUELGUAGE_VOL2)       //3600
    {
        return FUELGUAGE_LEVEL1 + (FUELGUAGE_LEVEL2 - FUELGUAGE_LEVEL1) * (Bat_Ocv_Vol - FUELGUAGE_VOL1) / (FUELGUAGE_VOL2 - FUELGUAGE_VOL1);
    }
    else if(Bat_Ocv_Vol < FUELGUAGE_VOL3)       //3700
    {
        return FUELGUAGE_LEVEL2 + (FUELGUAGE_LEVEL3 - FUELGUAGE_LEVEL2) * (Bat_Ocv_Vol - FUELGUAGE_VOL2) / (FUELGUAGE_VOL3 - FUELGUAGE_VOL2);
    }
    else if(Bat_Ocv_Vol < FUELGUAGE_VOL4)       //3800
    {
        return FUELGUAGE_LEVEL3 + (FUELGUAGE_LEVEL4 - FUELGUAGE_LEVEL3) * (Bat_Ocv_Vol - FUELGUAGE_VOL3) / (FUELGUAGE_VOL4 - FUELGUAGE_VOL3);
    }
    else if(Bat_Ocv_Vol < FUELGUAGE_VOL5)       //3900
    {
        return FUELGUAGE_LEVEL4 + (FUELGUAGE_LEVEL5 - FUELGUAGE_LEVEL4) * (Bat_Ocv_Vol - FUELGUAGE_VOL4) / (FUELGUAGE_VOL5 - FUELGUAGE_VOL4);
    }
    else if(Bat_Ocv_Vol < FUELGUAGE_VOL6)       //4000
    {
        return FUELGUAGE_LEVEL5 + (FUELGUAGE_LEVEL6 - FUELGUAGE_LEVEL5) * (Bat_Ocv_Vol - FUELGUAGE_VOL5) / (FUELGUAGE_VOL6 - FUELGUAGE_VOL5);
    }
    else if(Bat_Ocv_Vol < FUELGUAGE_VOL7)       //4100
    {
        return FUELGUAGE_LEVEL6 + (FUELGUAGE_LEVEL7 - FUELGUAGE_LEVEL6) * (Bat_Ocv_Vol - FUELGUAGE_VOL6) / (FUELGUAGE_VOL7 - FUELGUAGE_VOL6);
    }
    else if(Bat_Ocv_Vol < FUELGUAGE_TOP_VOL)    //4100
    {
        return FUELGUAGE_LEVEL7 + (FUELGUAGE_TOP_LEVEL - FUELGUAGE_LEVEL7) * (Bat_Ocv_Vol - FUELGUAGE_VOL7) / (FUELGUAGE_TOP_VOL - FUELGUAGE_VOL7);
    }
    else
    {
        return 0;
    }
}

static __u32 Get_Bat_RestTime(__u8 chg_status,__u8 rest_vol,__u16 Rdc,\
															__u16 Bat_Cap,__drv_power_charge_stat charge_status,__u32 Bat_Power)
{
    __u8    Tcv_Rest_Vol = 0;
    __u16   Iconst_current = 1;
    __u32   rest_time = 0;
    __u64   tmpvalue = 0;
//    __wrn("chg_status =%d\n",chg_status);
    if(charge_status.battery_current == 0)  charge_status.battery_current = 1;
    if(chg_status > 0x03)//充电,外部电源存在
    {
        if(charge_status.battery_vol < 4195)  // 在恒流充电时
        {
//            __wrn("enter fast charging!\n");
            Tcv_Rest_Vol = Get_Rest_Vol_Rate(4200 - charge_status.battery_current * Rdc / 1000);
            Iconst_current = charge_status.battery_current;
            if(Tcv_Rest_Vol < 70)
            {
                Tcv = 60 * (100 - Tcv_Rest_Vol) * Bat_Cap / (45 * charge_status.battery_current);
            }
            else
            {
                Tcv = 60 * (100 - Tcv_Rest_Vol) * Bat_Cap / (35 * charge_status.battery_current);
            }
            rest_time = 6 * Bat_Cap * ABS(Tcv_Rest_Vol - rest_vol) / charge_status.battery_current / 10 + Tcv ;
        }
        //进入恒压充电时
        else
        {
            if(Iconst_current == 1)
            {
                Iconst_current = charge_status.Ichgset;
            }
            if(Tcv == 0)
            {
                Tcv_Rest_Vol = Get_Rest_Vol_Rate(4200 - charge_status.Ichgset * Rdc / 1000);
               // __inf("Tcv_Rest_Vol = %d",Tcv_Rest_Vol);
                if(Tcv_Rest_Vol < 70)
                {
                    Tcv = 60 * (100 - Tcv_Rest_Vol) * Bat_Cap / (45 * charge_status.Ichgset);
                }
                else
                {
                    Tcv = 60 * (100 - Tcv_Rest_Vol) * Bat_Cap / (35 * charge_status.Ichgset);
                }
            }
            if(charge_status.battery_current < charge_status.Iendchg)
            {
                rest_time = 1;
            }
            else
            {
                rest_time = Tcv * (90 + 110 * charge_status.Iendchg/charge_status.battery_current) * (90 +100 * charge_status.Iendchg/charge_status.battery_current) * ABS(charge_status.battery_current - charge_status.Iendchg) / Iconst_current /10000;
            }
        }
    }
    else  //放电
    {
        __u8  End_Vol_Rate = Get_Rest_Vol_Rate(END_VOLTAGE_APS + (charge_status.battery_current * (Rdc + 110) / 1000));
//        __wrn("End_Vol_Rate = %d\n",End_Vol_Rate);
        if(Bat_Power)
        {
//            __wrn("Bat_Power = %d\n",Bat_Power);
//            __wrn("Bat_Cap = %d\n",Bat_Cap);
//            __wrn("rest_vol = %d\n",rest_vol);
            tmpvalue = BAT_AVER_VOL * Bat_Cap * 6; //防止溢出，分步计算
            tmpvalue = tmpvalue * ABS(rest_vol - End_Vol_Rate);
            tmpvalue = tmpvalue / (Bat_Power * 10);
            rest_time = tmpvalue;
//            __wrn("BAT_power rest_time=%d\n",rest_time);
        }
/*        
        if(Bat_Ocv_Vol)
        {
            rest_time *= charge_status.battery_vol;                                            //对OCV功率修正
            rest_time /= Bat_Ocv_Vol;
        }
*/        
        rest_time *= 100;           //对电池电压变低后效率提高的修正
        rest_time /= 99;
    }
//		__wrn("cerrect rest_time=%d\n",rest_time);
    return rest_time;
}

static __s32 _cb_p1_nmi_irq_handle(void * pArg)
{
		__u8 buffer[5];
		__inf("Int query!\n");
		
		if(PMU_type == 3)                            //AXP20X
    {
        /* 查询，处理，并清除中断位  (p1 only support 1 byte read/write            */
        __msg("intc nmi config   : %x\n", READ_REG32(0xf1c2040c));
        __msg("intc type sel0    : %x\n", READ_REG32(0xf1c20430));
        __msg("intc irq pending0 : %x\n", READ_REG32(0xf1c20410));
        __msg("intc fiq pending0 : %x\n", READ_REG32(0xf1c20420));
        if(DEV_Power_20_Ioctrl((__hdle)&(powerdrv.power_dev), DRV_POWER_CMD_INT_QUERY, 255, (void *)buffer) == EPDK_FAIL)
        {
            __wrn("p1 can not query IRQ status!\n");
        }

        __inf("Int query finished!\n");
        if((buffer[2]>>1) & 0x01)   //短按
        {
            PowerKeyDevSendEvent(KPAD_POWER, 1);
            PowerKeyDevSendEvent(KPAD_POWER, 0);
        }

        if((buffer[2]>>0) & 0x01)   //长按
        {
            PowerKeyDevSendEvent(KPAD_POWEROFF, 1);
            PowerKeyDevSendEvent(KPAD_POWEROFF, 0);
        }

//        __inf("p1 = %d, p2 = %d, p3 = %d, p4 = %d\n",buffer[0], buffer[1], buffer[2], buffer[3]);
        if(buffer[0] || buffer[1] || buffer[2] || buffer[3] ||buffer[4])
        {
            if(DEV_Power_20_Ioctrl((__hdle)&(powerdrv.power_dev), DRV_POWER_CMD_INT_CLEAR, 0, (void *)buffer) == EPDK_FAIL)
            {
                __wrn("p1 can not clear IRQ status!\n");
            }
        }
        WRITE_REG32(0x1, 0xf1c20420);
        __msg(">>intc nmi config   : %x\n", READ_REG32(0xf1c2040c));
        __msg(">>intc type sel0    : %x\n", READ_REG32(0xf1c20430));
        __msg(">>intc irq pending0 : %x\n", READ_REG32(0xf1c20410));
        __msg(">>intc fiq pending0 : %x\n", READ_REG32(0xf1c20420));
    }
    if(PMU_type == 2)                            //AXP19/17X
    {
        /* 查询，处理，并清除中断位  (p1 only support 1 byte read/write            */
        if(DEV_Power_19_Ioctrl((__hdle)&(powerdrv.power_dev), DRV_POWER_CMD_INT_QUERY, 255, (void *)buffer) == EPDK_FAIL)
        {
            __wrn("p1 can not query IRQ status!\n");
        }

        __inf("Int query finished!\n");
        if((buffer[2]>>1) & 0x01)   //短按
        {
            PowerKeyDevSendEvent(KPAD_POWER, 1);
            PowerKeyDevSendEvent(KPAD_POWER, 0);
        }

        if((buffer[2]>>0) & 0x01)   //长按
        {
            PowerKeyDevSendEvent(KPAD_POWEROFF, 1);
            PowerKeyDevSendEvent(KPAD_POWEROFF, 0);
        }

//        __inf("p1 = %d, p2 = %d, p3 = %d, p4 = %d\n",buffer[0], buffer[1], buffer[2], buffer[3]);
        if(buffer[0] || buffer[1] || buffer[2] || buffer[3])
        {
            if(DEV_Power_19_Ioctrl((__hdle)&(powerdrv.power_dev), DRV_POWER_CMD_INT_CLEAR, 0, (void *)buffer) == EPDK_FAIL)
            {
                __wrn("p1 can not clear IRQ status!\n");
            }
        }
    }
    else if(PMU_type == 1)
    {
        if(DEV_Power_18_Ioctrl((__hdle)&(powerdrv.power_dev), DRV_POWER_CMD_INT_QUERY, 0, (void *)buffer) == EPDK_FAIL)
        {
            __wrn("p1 can not query IRQ status!\n");
        }

        if((buffer[1]>>3) & 0x01)   //短按
        {
            PowerKeyDevSendEvent(KPAD_POWER, 1);
            PowerKeyDevSendEvent(KPAD_POWER, 0);
        }

        if((buffer[1]>>2) & 0x01)   //长按
        {
            PowerKeyDevSendEvent(KPAD_POWEROFF, 1);
            PowerKeyDevSendEvent(KPAD_POWEROFF, 0);
        }

        if(buffer[0] || buffer[1] || buffer[2])
        {
            if(DEV_Power_18_Ioctrl((__hdle)&(powerdrv.power_dev), DRV_POWER_CMD_INT_CLEAR, 0, (void *)buffer) == EPDK_FAIL)
            {
                __wrn("p1 can not clear IRQ status!\n");
            }
        }
	   }

    return EPDK_TRUE;
}


/*
****************************************************************************************************
*
*             KEYPAD_irq_handle
*
*  Description:
*       irkey_init
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
static __bool P1_NMI_irq_handle(void *arg)
{
    return EPDK_TRUE;
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
void P1_OpLock(void)
{
    __u8    err;
    esKRNL_SemPend(p1lock, 0, &err);
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
void P1_OpUnlock(void)
{
    esKRNL_SemPost(p1lock);
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
static void  power_main_task(void *p_arg)
{
    if(PMU_exist)
    {
        __u8 byte_addr = 0;
				
		if(PMU_type == 3)                            //AXP19/17X
        {
//            ES_FILE     *p_disp = NULL;
            __u8        temp_value[8];
            __drv_power_charge_stat     Charge_Stat;
            __drv_power_fuelguage_t     rt_Fuelguage,ocv_Fuelguage;
            __u16       Bat_Rdc,Bat_Vol,Bat_Ocv_Vol;
            __u16       i = 0,j = 0,k = 0;
            __u32 			Cur_CoulombCounter = 0;
            __u32				Bat_Time_Buffer[POWER19_TIME_MAX];
            __u32				Full_Correct_Buffer[MAX_BUFFER],Empty_Correct_Buffer[MAX_BUFFER];
            __u32				Total_Time;
            __twi_dev_para_ex_t            hstr;
            __s32 			Discharge_current_rate =10,Battery_cap = 1000;
            __u8 				IsNot_First_PowerUp_Flag = 0,Real_Cou_Flag = 0;
            __u8 				full_buffer_fill_Flag = 0,empty_buffer_fill_Flag = 0,coulomb_correct_Flag = 0;
            __u32				total_rest_volt = 0;
            __u16				correct_coulomb_rate = 0;


            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
		    //获取初始电池容量
			hstr.byte_count = 2;
            hstr.data       = temp_value;
            byte_addr  = POWER20_DATA_BUFFER2;
            hstr.byte_addr  = &byte_addr;
            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
            fuelguage.bat_cap = 5 * (((temp_value[0] & 0x7f) << 4) + ((temp_value[1] & 0xf0) >> 4));
            Real_Cou_Flag = (temp_value[0] & 0x80);
            __wrn("AXP209 POWER20_DATA_BUFFER2 =%x\n",temp_value[0]);
            __wrn("AXP209 POWER20_DATA_BUFFER3 =%x\n",temp_value[1]);
			__wrn("AXP209 REG fuelguage.bat_cap =%d\n",fuelguage.bat_cap);
            if(!Real_Cou_Flag || fuelguage.bat_cap < 200)
            {
              if(esCFG_GetKeyValue("power", "Battery_cap",  &Battery_cap, 1))
              {
                  __wrn("fail to get Battery cap, set one default value\n");

                  fuelguage.bat_cap = 2200;
              }
              else
              {
                  fuelguage.bat_cap = Battery_cap;
              }
             }
           	Init_AXP20x_MainTask();
            __wrn("AXP209 final fuelguage.bat_cap =%d\n",fuelguage.bat_cap);
            for(k=0; k<POWER20_TIME_MAX; k++)
            {
                Bat_Time_Buffer[k] = 0;
            }
            k = 0;
            Total_Time = 0;
            Bat_Rdc = Get_Rdc();
			__wrn("AXP209 Bat_Rdc =%d\n",Bat_Rdc);
			//获取初始电量百分比
						
			hstr.byte_count = 1;
            hstr.data  = temp_value;
            byte_addr  = POWER20_DATA_BUFFER1;
            hstr.byte_addr  = &byte_addr;
            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
            IsNot_First_PowerUp_Flag =(temp_value[0] & 0x40);
            if(IsNot_First_PowerUp_Flag) // 如果不是第一次接电池上电
          	{
          		ocv_Fuelguage.rest_vol = ((temp_value[0] & 0x1f) << 2);
	            __msg("AXP209 ocv_Fuelguage.rest_vol =%d\n",ocv_Fuelguage.rest_vol);
          	}
          	else //第一次接电池上电
          	{
	            DEV_Power_20_Ioctrl((__hdle)&powerdrv.power_dev, DRV_POWER_CMD_CHGSTAT_GET, 0, &Charge_Stat);
	            Bat_Vol = Charge_Stat.battery_vol;
	            Bat_Ocv_Vol = Get_Ocv_Bat_Vol(Charge_Stat.ext_power_valid && Charge_Stat.bat_current_direction,\
	            							Bat_Vol,Charge_Stat.battery_current,Bat_Rdc);//获取开路电压
	            __wrn("AXP209 Bat_Vol =%d\n",Bat_Vol);
	            __wrn("AXP209 Bat_Ocv_Vol =%d\n",Bat_Ocv_Vol);
	            ocv_Fuelguage.rest_vol = Get_Rest_Vol_Rate(Bat_Ocv_Vol);//基于开路电池电压获取剩余电池电量百分比
	            //将当前电量百分比存入Buffer中
	            hstr.byte_count = 1;
	            hstr.data       = temp_value;
	            byte_addr  = POWER20_DATA_BUFFER1; //REG06
	            hstr.byte_addr  = &byte_addr;
	            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
	            temp_value[0] &= 0xc0;
	            temp_value[0] |= (((ocv_Fuelguage.rest_vol >> 2) & 0x1f ) | 0x40);
	            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

	            //清空库仑计
	            hstr.byte_count = 1;
                hstr.data       = temp_value;
                byte_addr  = POWER20_COULOMB_CTL;
                hstr.byte_addr  = &byte_addr;
                eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);

                temp_value[0] |= 0x80;
                temp_value[0] |= 0x20;
                eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            }
            
            __wrn("AXP209 ocv_Fuelguage.rest_vol =%d\n",ocv_Fuelguage.rest_vol);
            //获取脚本文件的放电系数
            if(esCFG_GetKeyValue("power", "Discharge_rate",  &Discharge_current_rate, 1))
            {
                __wrn("fail to get Discharge current rate, set one default value\n");
                Discharge_current_rate  = 10;
            }
            __wrn("AXP209 Discharge_current_rate =%d\n",Discharge_current_rate);
	          
            while(1)
            {

                hstr.byte_count = 1;
                hstr.data       = temp_value;
                byte_addr  = POWER20_STATUS;//
                hstr.byte_addr  = &byte_addr;
                eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
                __wrn("POWER20_STATUS[00H] temp_value[0] is 0x%x \n",temp_value[0]);

                if(temp_value[0] && 0xf0 == 0xf0)
                {
                    hstr.byte_count = 1;
                    hstr.data       = temp_value;
                    byte_addr  = POWER20_IPS_SET;//
                    hstr.byte_addr  = &byte_addr;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
                    __wrn("POWER20_IPS_SET[30H] temp_value[0] is 0x%x \n",temp_value[0]);

                    if(temp_value[0] != 0x63)
                    {
                        __u8 value;
                        value = 0x63;
                        hstr.byte_count = 1;
                        hstr.data       = &value;
                        byte_addr  = POWER20_IPS_SET;//
                        hstr.byte_addr  = &byte_addr;
                        eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                    }
                }
            
                DEV_Power_20_Ioctrl((__hdle)&powerdrv.power_dev, DRV_POWER_CMD_CHGSTAT_GET, 0, &Charge_Stat);
                rt_Fuelguage.charge_status = (Charge_Stat.ext_power_valid << 2) |\
                														(Charge_Stat.battery_exist << 1) | Charge_Stat.charge_indication;
                Cur_CoulombCounter = DEV_Power_20_Ioctrl((__hdle)&powerdrv.power_dev, DRV_POWER_CMD_GET_COULOMB, 0, 0);//获取库仑计值  
                __wrn("AXP209 Charge_Stat.battery_current =%d\n",Charge_Stat.battery_current);

				if(coulomb_flag)//充电库仑计值比放电库仑计值大
				{
					//__wrn("AXP209 discharging ocv_Fuelguage.rest_vol =%d\n",ocv_Fuelguage.rest_vol);
					rt_Fuelguage.rest_vol = ocv_Fuelguage.rest_vol + 100*Cur_CoulombCounter/fuelguage.bat_cap;
					//__wrn("AXP209 charging rt_Fuelguage.rest_vol =%d\n",rt_Fuelguage.rest_vol);
				}
				else
				{
//									__wrn("AXP209 discharging ocv_Fuelguage.rest_vol =%d\n",ocv_Fuelguage.rest_vol);
					rt_Fuelguage.rest_vol = ocv_Fuelguage.rest_vol - 100*Cur_CoulombCounter/fuelguage.bat_cap;
					//__wrn("AXP209 discharging rt_Fuelguage.rest_vol =%d\n",rt_Fuelguage.rest_vol);
				}
				if(Charge_Stat.charge_indication) //充电过程
				{
					//__wrn("AXP209 Charge_Stat.battery_vol =%d\n",Charge_Stat.battery_vol);
//									__wrn("AXP209 Charge_Stat.battery_current =%d\n",Charge_Stat.battery_current);
					if((Charge_Stat.battery_vol > BAT_FULL_VOLT) && (Charge_Stat.battery_current < BAT_FULL_CURR)) //边界条件修正
					{
						if(rt_Fuelguage.rest_vol >= BAT_FULL_CORRECT_RATE)//如果OCV电压获得的电量百分比赋初值量偏大
						{
							rt_Fuelguage.rest_vol = BAT_FULL_CORRECT_RATE;
							ocv_Fuelguage.rest_vol = rt_Fuelguage.rest_vol;
							//取校正电池容量标志位
                            hstr.byte_count = 1;
				            hstr.data       = temp_value;
				            byte_addr  = POWER20_DATA_BUFFER1; //REG06
				            hstr.byte_addr  = &byte_addr;
				            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
				            coulomb_correct_Flag = (temp_value[0] &0x80) ;
							if(coulomb_correct_Flag)
							{
								//校正库仑计，将可用的库伦容量放到Buffer中
		                        hstr.byte_count = 2;
    		                    hstr.data       = temp_value;
    		                    byte_addr  = POWER20_DATA_BUFFER2;
    		                    hstr.byte_addr  = &byte_addr;
    		                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
    		
    		                    temp_value[0] = ((((Cur_CoulombCounter /5) & 0xff0) | 0x800) >> 4);
    		                    temp_value[1] &= 0x0f;
    		                    temp_value[1] |= (((Cur_CoulombCounter /5) & 0x0f) << 4) ;
    		                    hstr.byte_count = 1;
    		                    hstr.data       = temp_value;
    		                    byte_addr  = POWER20_DATA_BUFFER2;
    		                    hstr.byte_addr  = &byte_addr;
    		                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
    		
    		                    hstr.data       = temp_value+1;
    		                    byte_addr  = POWER20_DATA_BUFFER3;
    		                    hstr.byte_addr  = &byte_addr;
    		                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
								fuelguage.bat_cap = Cur_CoulombCounter;//zhangdonglu 改变当前电池容量 110518
      	                    }
							//将当前电量百分比存入Buffer中
				            hstr.byte_count = 1;
				            hstr.data       = temp_value;
				            byte_addr  = POWER20_DATA_BUFFER1; //REG06
				            hstr.byte_addr  = &byte_addr;
				            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
				            temp_value[0] &= 0x40;//0xc0; //zhangdonglu 把coulomb_correct_Flag标志位置0，以免再次把库仑计里的值再赋给reg07~08。还有1位没用 110518
				            temp_value[0] |= (((ocv_Fuelguage.rest_vol >> 2) & 0x1f) | 0x40);
				            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
				            //清空库仑计
				            hstr.byte_count = 1;
                            hstr.data       = temp_value;
                            byte_addr  = POWER20_COULOMB_CTL;
                            hstr.byte_addr  = &byte_addr;
                            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);

                            temp_value[0] |= 0x80;
                            temp_value[0] |= 0x20;
                            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
              
						}
						else  //如果OCV电压获得的电量百分比赋初值量偏小
						{
							if(!full_buffer_fill_Flag)
							{
								for(i = 0;i < MAX_BUFFER;i++)
								{
									Full_Correct_Buffer[i] = rt_Fuelguage.rest_vol;
								}
								full_buffer_fill_Flag = 1;
							}
							else
							{
								if(i == MAX_BUFFER)
								{
									i = 0;
								}
								Full_Correct_Buffer[i] = BAT_FULL_CORRECT_RATE;
								i++;
								j = 0;
								for(j = 0;j < MAX_BUFFER;j++)
								{
									total_rest_volt += Full_Correct_Buffer[j];
								}
								rt_Fuelguage.rest_vol = total_rest_volt/MAX_BUFFER;
								ocv_Fuelguage.rest_vol = rt_Fuelguage.rest_vol;
								total_rest_volt = 0;


					            //标志可以校正电池容量
				                hstr.byte_count = 1;
					            hstr.data       = temp_value;
					            byte_addr  = POWER20_DATA_BUFFER1; //REG06
					            hstr.byte_addr  = &byte_addr;
					            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
				            	coulomb_correct_Flag = (temp_value[0] &0x80) ;

					            if(coulomb_correct_Flag)
					            {
						            //校正库仑计，将可用的库伦容量放到Buffer中
						            hstr.byte_count = 2;
    			                    hstr.data       = temp_value;
    			                    byte_addr  = POWER20_DATA_BUFFER2;
    			                    hstr.byte_addr  = &byte_addr;
    			                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
    			
    			                    temp_value[0] = ((((Cur_CoulombCounter /5) & 0xff0) | 0x800) >> 4);
    			                    temp_value[1] &= 0x0f;
    			                    temp_value[1] |= (((Cur_CoulombCounter /5) & 0x0f) << 4) ;
    			                    hstr.byte_count = 1;
    			                    hstr.data       = temp_value;
    			                    byte_addr  = POWER20_DATA_BUFFER2;
    			                    hstr.byte_addr  = &byte_addr;
    			                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
    			
    			                    hstr.data       = temp_value+1;
    			                    byte_addr  = POWER20_DATA_BUFFER3;
    			                    hstr.byte_addr  = &byte_addr;
    			                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
									fuelguage.bat_cap = Cur_CoulombCounter;//zhangdonglu 改变当前电池容量 110518
          	                    }

                                //将当前电量百分比存入Buffer中
                                hstr.byte_count = 1;
                                hstr.data       = temp_value;
                                byte_addr  = POWER20_DATA_BUFFER1; //REG06
                                hstr.byte_addr  = &byte_addr;
                                eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
                                temp_value[0] |= 0x40;//0xc0; //zhangdonglu 把coulomb_correct_Flag标志位置0，以免再次把库仑计里的值再赋给reg07~08。还有1位没用 110518
                                temp_value[0] |= (((ocv_Fuelguage.rest_vol >> 2) & 0x1f) | 0x40);
                                eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                                //清空库仑计
                                hstr.byte_count = 1;
                                hstr.data       = temp_value;
                                byte_addr  = POWER20_COULOMB_CTL;
                                hstr.byte_addr  = &byte_addr;
                                eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);

                                temp_value[0] |= 0x80;
                                temp_value[0] |= 0x20;
                                eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
							}
						 }
						esKRNL_TimeDly(2000);
					 }
                else
                {
        			if(rt_Fuelguage.rest_vol >= BAT_FULL_CORRECT_RATE)
        				rt_Fuelguage.rest_vol = BAT_FULL_CORRECT_RATE;//zhangdonglu 以免未到充电限制时，超过限制 110520
                }
			}
								else if(rt_Fuelguage.charge_status ==6 &&(Charge_Stat.battery_vol > BAT_FULL_VOLT)) //充电结束
								{
									//__wrn("AXP209 Charge_Stat.battery_vol =%d\n",Charge_Stat.battery_vol);//zhangdonglu 110518
									//__wrn("AXP209 Charge_Stat.battery_current =%d\n",Charge_Stat.battery_current);//zhangdonglu 110518
									rt_Fuelguage.rest_vol = 100;
									ocv_Fuelguage.rest_vol = rt_Fuelguage.rest_vol;
									//将当前电量百分比存入Buffer中
            			            hstr.byte_count = 1;
            			            hstr.data       = temp_value;
            			            byte_addr  = POWER20_DATA_BUFFER1; //REG06
            			            hstr.byte_addr  = &byte_addr;
            			            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
            			            temp_value[0] &= 0xc0;
            			            temp_value[0] |= (((ocv_Fuelguage.rest_vol >> 2) & 0x1f) | 0x40);
            			            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            			            //清空库仑计
            			            hstr.byte_count = 1;
                                    hstr.data       = temp_value;
                                    byte_addr  = POWER20_COULOMB_CTL;
                                    hstr.byte_addr  = &byte_addr;
                                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);

                                    temp_value[0] |= 0x80;
                                    temp_value[0] |= 0x20;
                                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
								}
								else // 放电过程
								{
									//__wrn("AXP209 Charge_Stat.battery_vol =%d\n",Charge_Stat.battery_vol);
									//__wrn("AXP209 Charge_Stat.battery_current =%d\n",Charge_Stat.battery_current);
									correct_coulomb_rate = Get_Rest_Vol_Rate(3500+Charge_Stat.battery_current*Bat_Rdc/1000);
									if(!Real_Cou_Flag)// && output_type ==DISP_OUTPUT_TYPE_LCD)  //由于Rdc存在不能完全使用电池能量，修正电池容量
									{
										//__wrn("AXP209 before correct fuelguage.bat_cap =%d\n",fuelguage.bat_cap);
										fuelguage.bat_cap = fuelguage.bat_cap *(100 -correct_coulomb_rate)/100;
										//__wrn("AXP209  correct fuelguage.bat_cap =%d\n",fuelguage.bat_cap);
										hstr.byte_count = 2;
                                        hstr.data       = temp_value;
                                        byte_addr  = POWER20_DATA_BUFFER2;
                                        hstr.byte_addr  = &byte_addr;
                                        eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);

                                        temp_value[0] = ((((fuelguage.bat_cap /5) & 0xff0) | 0x800) >> 4);
                                        temp_value[1] &= 0x0f;
                                        temp_value[1] |= (((fuelguage.bat_cap /5) & 0x0f) << 4) ;
                                        hstr.byte_count = 1;
                                        hstr.data       = temp_value;
                                        byte_addr  = POWER20_DATA_BUFFER2;
                                        hstr.byte_addr  = &byte_addr;
                                        eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                                        hstr.data       = temp_value+1;
                                        byte_addr  = POWER20_DATA_BUFFER3;
                                        hstr.byte_addr  = &byte_addr;
                                        eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                                        Real_Cou_Flag = 1;
									} 
									if(Charge_Stat.battery_vol < BAT_LOW_VOLT) //修正边界条件
									{
										if(rt_Fuelguage.rest_vol > BAT_LOW_CORRECT_RATE) //如果OCV电压获得的电量百分比赋初值量偏大
										{
											if(!empty_buffer_fill_Flag)
											{
												for(i = 0;i < MAX_BUFFER;i++)
												{
													Empty_Correct_Buffer[i] = rt_Fuelguage.rest_vol;
												}
												empty_buffer_fill_Flag = 1;
											}
											else
											{
												if(i == MAX_BUFFER)
												{
													i = 0;
												}
												Empty_Correct_Buffer[i] = BAT_LOW_CORRECT_RATE;
												i++;
												j = 0;
												for(j = 0;j < MAX_BUFFER;j++)
												{
													total_rest_volt += Empty_Correct_Buffer[j];
												}
												rt_Fuelguage.rest_vol = total_rest_volt/MAX_BUFFER;
												ocv_Fuelguage.rest_vol = rt_Fuelguage.rest_vol;
												total_rest_volt = 0;
												
												//将当前电量百分比存入Buffer中
            						            hstr.byte_count = 1;
            						            hstr.data       = temp_value;
            						            byte_addr  = POWER20_DATA_BUFFER1; //REG06
            						            hstr.byte_addr  = &byte_addr;
            						            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
            						            temp_value[0] &= 0x40;//0xc0;//zhangdonglu 把coulomb_correct_Flag标志位置0，以免为充满拔电使得出的电池电量过小 110520
            						            temp_value[0] |= (((ocv_Fuelguage.rest_vol >> 2) & 0x1f) | 0x40);
            						            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            						            //清空库仑计
            						            hstr.byte_count = 1;
                                                hstr.data       = temp_value;
                                                byte_addr  = POWER20_COULOMB_CTL;
                                                hstr.byte_addr  = &byte_addr;
                                                eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
                                                temp_value[0] |= 0x80;
                                                temp_value[0] |= 0x20;
                                                eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
					             
					                            esKRNL_TimeDly(2000);
											}
										}
										else //如果OCV电压获得的电量百分比赋初值量偏小
										{
											rt_Fuelguage.rest_vol = BAT_LOW_CORRECT_RATE;
											ocv_Fuelguage.rest_vol = rt_Fuelguage.rest_vol;
            					            //将当前电量百分比存入Buffer中
            					            hstr.byte_count = 1;
            					            hstr.data       = temp_value;
            					            byte_addr  = POWER20_DATA_BUFFER1; //REG06
            					            hstr.byte_addr  = &byte_addr;
            					            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
            					            temp_value[0] &= 0x40;//0xc0;//zhangdonglu 把coulomb_correct_Flag标志位置0，以免为充满拔电使得出的电池电量过小 110520
            					            temp_value[0] |= (((ocv_Fuelguage.rest_vol >> 2) & 0x1f) | 0x40);
            					            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            					            //清空库仑计
            					            hstr.byte_count = 1;
                                            hstr.data       = temp_value;
                                            byte_addr  = POWER20_COULOMB_CTL;
                                            hstr.byte_addr  = &byte_addr;
                                            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
                                            temp_value[0] |= 0x80;
                                            temp_value[0] |= 0x20;
                                            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                                            esKRNL_TimeDly(2000);
										}
										if(Charge_Stat.battery_vol < BAT_ULOW_VOLT)  //电池几乎为空，修正电量计量
										{
											rt_Fuelguage.rest_vol = 2 ;
											ocv_Fuelguage.rest_vol = rt_Fuelguage.rest_vol;
											//将当前电量百分比存入Buffer中
            					            hstr.byte_count = 1;
            					            hstr.data       = temp_value;
            					            byte_addr  = POWER20_DATA_BUFFER1; //REG06
            					            hstr.byte_addr  = &byte_addr;
            					            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
            					            temp_value[0] &= 0xc0;
            					            temp_value[0] |= (((ocv_Fuelguage.rest_vol >> 2) & 0x1f) | 0x40);
            					            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    			            //清空库仑计
                    			            hstr.byte_count = 1;
                                            hstr.data       = temp_value;
                                            byte_addr  = POWER20_COULOMB_CTL;
                                            hstr.byte_addr  = &byte_addr;
                                            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
                                            temp_value[0] |= 0x80;
                                            temp_value[0] |= 0x20;
                                            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                                            //标志可以校正电池容量
                                            coulomb_correct_Flag = 1;
                                            hstr.byte_count = 1;
                    			            hstr.data       = temp_value;
                    			            byte_addr  = POWER20_DATA_BUFFER1; //REG06
                    			            hstr.byte_addr  = &byte_addr;
                    			            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
                    			            temp_value[0] &= 0x7f;
                    			            temp_value[0] |= 0x80;
                    			            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
										}
									}
									else
									{
										if (rt_Fuelguage.rest_vol <= BAT_LOW_CORRECT_RATE)
											rt_Fuelguage.rest_vol = BAT_LOW_CORRECT_RATE;//zhangdonglu 以免未到条件时低于剩余容量门限 110520
									}
								}
								
								if(rt_Fuelguage.rest_vol > 100)
								{
									rt_Fuelguage.rest_vol = 100;
								}
								else if(rt_Fuelguage.rest_vol < 1)
								{
									rt_Fuelguage.rest_vol = 1;
								}
								
								fuelguage.rest_vol = rt_Fuelguage.rest_vol;
								fuelguage.bat_power = Charge_Stat.battery_current * Charge_Stat.battery_vol*Discharge_current_rate / 10;
                                //__wrn("AXP209 fuelguage.bat_power =%d\n",fuelguage.bat_power);
                                //__wrn("AXP209 fuelguage.rest_vol =%d\n",fuelguage.rest_vol);
                                //__wrn("AXP209 Bat_Ocv_Vol =%d\n",Bat_Ocv_Vol);
                                //__wrn("AXP209 Bat_Rdc =%d\n",Bat_Rdc);
                                //__wrn("AXP209 fuelguage.bat_cap =%d\n",fuelguage.bat_cap);
                fuelguage.charge_status = rt_Fuelguage.charge_status;
                rt_Fuelguage.rest_time = Get_Bat_RestTime(fuelguage.charge_status,fuelguage.rest_vol,Bat_Rdc,\
                    													          fuelguage.bat_cap,Charge_Stat,fuelguage.bat_power);
                Total_Time -= Bat_Time_Buffer[k];
                Bat_Time_Buffer[k] = rt_Fuelguage.rest_time;
                Total_Time += Bat_Time_Buffer[k];
                k++;
                if(k == POWER20_TIME_MAX)
                {
                    k = 0;
                }

                fuelguage.rest_time = Total_Time / POWER20_TIME_MAX;
//                __wrn("AXP209 aver fuelguage.rest_time =%d\n",fuelguage.rest_time);
                
           		esKRNL_TimeDly(500);
            }
        }
        if(PMU_type == 2)                            //AXP19/17X
        {
//            ES_FILE     *p_disp = NULL;
            __u8        temp_value[8];
            __drv_power_charge_stat     Charge_Stat;
            __drv_power_fuelguage_t     rt_Fuelguage,ocv_Fuelguage;
            __u16       Bat_Rdc,Bat_Vol,Bat_Ocv_Vol;
            __u16       i = 0,j = 0,k = 0;
            __u32 			Cur_CoulombCounter = 0;
            __u32				Bat_Time_Buffer[POWER19_TIME_MAX];
            __u32				Full_Correct_Buffer[MAX_BUFFER],Empty_Correct_Buffer[MAX_BUFFER];
            __u32				Total_Time;
            __twi_dev_para_ex_t            hstr;
            __s32 			Discharge_current_rate =10,Battery_cap = 1000;
            __u8 				Is_First_PowerUp_Flag = 1,Real_Cou_Flag = 0;
            __u8 				full_buffer_fill_Flag = 0,empty_buffer_fill_Flag = 0,coulomb_correct_Flag = 1;
            __u32				total_rest_volt = 0;
            __u32				tmp_bat_vol[10] = {0};
            __u32				tmp_bat_current[10] = {0};
            __u32				Bat_Current = 0;


						hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
		    		hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
		    		hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
		    		//获取初始电池容量
						hstr.byte_count = 2;
          	hstr.data       = temp_value;
          	byte_addr  = POWER19_DATA_BUFFER2;
          	hstr.byte_addr  = &byte_addr;
          	eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
          	fuelguage.bat_cap = 5 * (((temp_value[0] & 0x7f) << 4) + ((temp_value[1] & 0xf0) >> 4));
          	Real_Cou_Flag = (temp_value[0] & 0x80);
          	if(!Real_Cou_Flag || fuelguage.bat_cap < 200)
          	{
          	  if(esCFG_GetKeyValue("power", "Battery_cap",  &Battery_cap, 1))
          	  {
          	      fuelguage.bat_cap = 2800;
          	  }
          	  else
          	  {
          	      fuelguage.bat_cap = Battery_cap;
          	  }
          	 }
         		Init_AXP19x_MainTask();
          	for(k=0; k<POWER19_TIME_MAX; k++)
          	{
          	    Bat_Time_Buffer[k] = 0;
          	}
          	k = 0;
          	Total_Time = 0;
          	Bat_Rdc = 200;

        		//获取脚本文件的放电系数
        		if(esCFG_GetKeyValue("power", "Discharge_rate",  &Discharge_current_rate, 1))
        		{
//      		          __wrn("fail to get Discharge current rate, set one default value\n");
        		    Discharge_current_rate  = 10;
        		}
//					__wrn("AXP199 Discharge_current_rate =%d\n",Discharge_current_rate);
				

        		while(1)
        		{
        	  		DEV_Power_19_Ioctrl((__hdle)&powerdrv.power_dev, DRV_POWER_CMD_CHGSTAT_GET, 0, &Charge_Stat);
     	  				if(!Charge_Stat.battery_exist)
    	  				{	
  	  							__inf("@@@@@@@@@@@AXP199 battery has been removed @@@@@@@@@@\n");
  	  							fuelguage.rest_vol = 0;
  	  							fuelguage.rest_time = 0;
  	  							hstr.byte_count = 1;
		        		    hstr.data       = temp_value;
		        		    byte_addr  = POWER19_DATA_BUFFER1; //REG06
		        		    hstr.byte_addr  = &byte_addr;
		        		    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
		        		    temp_value[0] |= 0x40;
		        		    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
		        		    esKRNL_TimeDly(100);
  	  					}
      	  			else
      	  			{
      	  			 		//获取初始电量百分比
										hstr.byte_count = 1;
		        		    hstr.data       = temp_value;
		        		    byte_addr  = POWER19_DATA_BUFFER1;
		        		    hstr.byte_addr  = &byte_addr;
		        		    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);


							Is_First_PowerUp_Flag =(temp_value[0] & 0x40);
							/*读取基于开路电压剩余的电量的百分比*/
							if(!Is_First_PowerUp_Flag && EPDK_TRUE==first_setup_system)
							{
							  __s32  rest_vol =  ((temp_value[0] & 0x1f) << 2);;							  	
			      		      esKRNL_TimeDly(30);
			      		      Bat_Rdc = Get_Rdc();
			      		      for(i =0;i <10;i++)
			      		      {
			      		      	DEV_Power_19_Ioctrl((__hdle)&powerdrv.power_dev, DRV_POWER_CMD_CHGSTAT_GET, 0, &Charge_Stat);
			      		      	tmp_bat_vol[i] = Charge_Stat.battery_vol;
			      		      	tmp_bat_current[i] = Charge_Stat.battery_current;
			      		      	__wrn("AXP19 tmp_bat_vol[i] = %d\n",tmp_bat_vol[i]);
			      		      	__wrn("AXP19 tmp_bat_current[i] = %d\n",tmp_bat_current[i]);
			      		      	esKRNL_TimeDly(3);
			      		      }
			      		      Bat_Vol = 0;
			      		      Bat_Current = 0;
			      		      for(i = 0;i <10;i++)
			      		      {
			      		      	Bat_Vol += tmp_bat_vol[i];
			      		      }
			      		      for(i = 0;i <10;i++)
			      		      {
			      		      	Bat_Current += tmp_bat_current[i];
			      		      }
			      		      Bat_Vol = (Bat_Vol/10);
			      		      Bat_Current =(Bat_Current/10);
//							            __wrn("@@@@@@@@@@@AXP199 is The First Time Insert battery!@@@@@@@@@@\n",Bat_Vol);
			      		      DEV_Power_19_Ioctrl((__hdle)&powerdrv.power_dev, DRV_POWER_CMD_CHGSTAT_GET, 0, &Charge_Stat);
			      		      Bat_Ocv_Vol = Get_Ocv_Bat_Vol(Charge_Stat.ext_power_valid && Charge_Stat.bat_current_direction,\
			      		      							Bat_Vol,Bat_Current,Bat_Rdc);//获取开路电压			      		
			      		      __wrn("AXP199 Bat_Ocv_Vol =%d\n",Bat_Ocv_Vol);	
			      		      ocv_Fuelguage.rest_vol = Get_Rest_Vol_Rate(Bat_Ocv_Vol);//基于开路电池电压获取剩余电池电量百分比

							 if(ocv_Fuelguage.rest_vol>rest_vol)
							 {	
							  	if(ocv_Fuelguage.rest_vol - rest_vol >= GUARD_OF_ROV)
								{
									Is_First_PowerUp_Flag = 1;
								}	
							 }
							 else
							 {
							 	if(rest_vol - ocv_Fuelguage.rest_vol >= GUARD_OF_ROV)
								{
									Is_First_PowerUp_Flag = 1;
								}	
							 }	
							 first_setup_system = EPDK_FALSE;
							}
		        		    
	        				__msg("----------AXP199 REG06 =%d,Is_First_PowerUp_Flag = %d\n",temp_value[0], Is_First_PowerUp_Flag);
							
		        		    if(!Is_First_PowerUp_Flag) // 如果不是第一次接电池上电
		        		  	{
		        		  		ocv_Fuelguage.rest_vol = ((temp_value[0] & 0x1f) << 2);
		        		  	}
		        		  	else //第一次接电池
		        		  	{
			      		      esKRNL_TimeDly(30);
			      		      Bat_Rdc = Get_Rdc();
			      		      for(i =0;i <10;i++)
			      		      {
			      		      	DEV_Power_19_Ioctrl((__hdle)&powerdrv.power_dev, DRV_POWER_CMD_CHGSTAT_GET, 0, &Charge_Stat);
			      		      	tmp_bat_vol[i] = Charge_Stat.battery_vol;
			      		      	tmp_bat_current[i] = Charge_Stat.battery_current;
			      		      	__wrn("AXP19 tmp_bat_vol[i] = %d\n",tmp_bat_vol[i]);
			      		      	__wrn("AXP19 tmp_bat_current[i] = %d\n",tmp_bat_current[i]);
			      		      	esKRNL_TimeDly(3);
			      		      }
			      		      Bat_Vol = 0;
			      		      Bat_Current = 0;
			      		      for(i = 0;i <10;i++)
			      		      {
			      		      	Bat_Vol += tmp_bat_vol[i];
			      		      }
			      		      for(i = 0;i <10;i++)
			      		      {
			      		      	Bat_Current += tmp_bat_current[i];
			      		      }
			      		      Bat_Vol = (Bat_Vol/10);
			      		      Bat_Current =(Bat_Current/10);
//							            __wrn("@@@@@@@@@@@AXP199 is The First Time Insert battery!@@@@@@@@@@\n",Bat_Vol);
			      		      DEV_Power_19_Ioctrl((__hdle)&powerdrv.power_dev, DRV_POWER_CMD_CHGSTAT_GET, 0, &Charge_Stat);
			      		      Bat_Ocv_Vol = Get_Ocv_Bat_Vol(Charge_Stat.ext_power_valid && Charge_Stat.bat_current_direction,\
			      		      							Bat_Vol,Bat_Current,Bat_Rdc);//获取开路电压
			      		      __wrn("AXP199 Bat_Vol =%d\n",Bat_Vol);
			      		      __wrn("AXP199 Bat_Ocv_Vol =%d\n",Bat_Ocv_Vol);
			      		      ocv_Fuelguage.rest_vol = Get_Rest_Vol_Rate(Bat_Ocv_Vol);//基于开路电池电压获取剩余电池电量百分比
			      		      //将当前电量百分比存入Buffer中
			      		      hstr.byte_count = 1;
			      		      hstr.data       = temp_value;
			      		      byte_addr  = POWER19_DATA_BUFFER1; //REG06
			      		      hstr.byte_addr  = &byte_addr;
			      		      eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
			      		      temp_value[0] &= 0x80;   //标志不是第一插上电，同时清除bit0~bit4，保留bit7
			      		      temp_value[0] |= ((ocv_Fuelguage.rest_vol >> 2) & 0x1f );
			      		      eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
		        		
			      		      //清空库仑计
			      		      hstr.byte_count = 1;
		        		      hstr.data       = temp_value;
		        		      byte_addr  = POWER19_COULOMB_CTL;
		        		      hstr.byte_addr  = &byte_addr;
		        		      eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
		        		
		        		      temp_value[0] |= 0x80;
		        		      temp_value[0] |= 0x20;
		        		      eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
			      		      __wrn("===@@@First Init AXP199 ocv_Fuelguage.rest_vol =%d\n",ocv_Fuelguage.rest_vol);
			      		  }
        	  			rt_Fuelguage.charge_status = (Charge_Stat.ext_power_valid << 2) |\
            																		(Charge_Stat.battery_exist << 1) | Charge_Stat.charge_indication;
        	  			Cur_CoulombCounter = DEV_Power_19_Ioctrl((__hdle)&powerdrv.power_dev, DRV_POWER_CMD_GET_COULOMB, 0, 0);//获取库仑计值  
            	  
									if(coulomb_flag)//充电库仑计值比放电库仑计值大
									{
										rt_Fuelguage.rest_vol = ocv_Fuelguage.rest_vol + 100*Cur_CoulombCounter/fuelguage.bat_cap;
										//__wrn("AXP199 charging rt_Fuelguage.rest_vol =%d\n",rt_Fuelguage.rest_vol);
									}
									else
									{
										rt_Fuelguage.rest_vol = ocv_Fuelguage.rest_vol - 100*Cur_CoulombCounter/fuelguage.bat_cap;
										//__wrn("AXP199 discharging rt_Fuelguage.rest_vol =%d\n",rt_Fuelguage.rest_vol);
									}
									__wrn("=============AXP19x ocv_Fuelguage.rest_vol@@@@@@@@@@ =%d\n",ocv_Fuelguage.rest_vol);
									__wrn("*************AXP19x rt_Fuelguage.rest_vol@@@@@@@@@@ =%d\n",rt_Fuelguage.rest_vol);
									__wrn("@@@@@@@@@@@@@AXP19x Cur_CoulombCounter@@@@@@@@@@ =%d\n",Cur_CoulombCounter);
									if(Charge_Stat.charge_indication) //充电过程
									{
										__wrn("=======@@@@@@@@@AXP199 Is Charging@@@@@@@@@@======\n");
										__wrn("AXP199 Charge_Stat.battery_vol =%d\n",Charge_Stat.battery_vol);
										__wrn("AXP199 Charge_Stat.battery_current =%d\n",Charge_Stat.battery_current);
											if((Charge_Stat.battery_vol > BAT_FULL_VOLT) && (Charge_Stat.battery_current < BAT_FULL_CURR)) //边界条件修正
											{
												if(rt_Fuelguage.rest_vol >= BAT_FULL_CORRECT_RATE)//如果OCV电压获得的电量百分比赋初值量偏大
												{
//												__wrn("=======@@@@@@@@@AXP199  rest.volt is over correct@@@@@@@@@@======\n");
													rt_Fuelguage.rest_vol = BAT_FULL_CORRECT_RATE;
													ocv_Fuelguage.rest_vol = rt_Fuelguage.rest_vol;
													//取校正电池容量标志位
													hstr.byte_count = 1;
													hstr.data       = temp_value;
													byte_addr  = POWER19_DATA_BUFFER1; //REG06
													hstr.byte_addr  = &byte_addr;
													eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
													coulomb_correct_Flag = (temp_value[0] &0x80) ;
													if(!coulomb_correct_Flag)
													{
//														__wrn("=======@@@@@@@@@AXP199 Charging , correct coulomb@@@@@@@@@@======\n");
															//校正库仑计，将可用的库伦容量放到Buffer中
															hstr.byte_count = 2;
		    					    				hstr.data       = temp_value;
		    					    				byte_addr  = POWER19_DATA_BUFFER2;
		    					    				hstr.byte_addr  = &byte_addr;
		    					    				eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
		    					    				temp_value[0] &= 0x00;
		    					    				temp_value[0] = ((((Cur_CoulombCounter /5) & 0xff0) | 0x800) >> 4);
		    					    				temp_value[1] &= 0x0f;
		    					    				temp_value[1] |= (((Cur_CoulombCounter /5) & 0x0f) << 4) ;
		    					    				hstr.byte_count = 1;
		    					    				hstr.data       = temp_value;
		    					    				byte_addr  = POWER19_DATA_BUFFER2;
		    					    				hstr.byte_addr  = &byte_addr;
		    					    				eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
		    					    				
		    					    				hstr.data       = temp_value+1;
		    					    				byte_addr  = POWER19_DATA_BUFFER3;
		    					    				hstr.byte_addr  = &byte_addr;
		    					    				eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
															fuelguage.bat_cap = Cur_CoulombCounter;//zhangdonglu 改变当前电池容量 110518
	      					      	}
															//将当前电量百分比存入Buffer中
													hstr.byte_count = 1;
													hstr.data       = temp_value;
													byte_addr  = POWER19_DATA_BUFFER1; //REG06
													hstr.byte_addr  = &byte_addr;
													eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
													temp_value[0] |= 0x80; //zhangdonglu 把coulomb_correct_Flag标志位置1，以免再次把库仑计里的值再赋给reg07~08。还有1位没用 110518
													temp_value[0] &= 0xe0; 
													temp_value[0] |= ((ocv_Fuelguage.rest_vol >> 2) & 0x1f);
													temp_value[0] &= 0xbf; //标志不是第一次上电,需将REG06 bit6置0
													eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
													//清空库仑计
													hstr.byte_count = 1;
													hstr.data       = temp_value;
													byte_addr  = POWER19_COULOMB_CTL;
													hstr.byte_addr  = &byte_addr;
													eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
													
													temp_value[0] |= 0x80;
													temp_value[0] |= 0x20;
													eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
									      	        
												}
												else  //如果OCV电压获得的电量百分比赋初值量偏小
												{
//													__wrn("=======@@@@@@@@@AXP199 Is rest.volt   correct is not enough@@@@@@@@@@======\n");
														if(!full_buffer_fill_Flag)
														{
															for(i = 0;i < MAX_BUFFER;i++)
															{
																Full_Correct_Buffer[i] = rt_Fuelguage.rest_vol;
															}
															full_buffer_fill_Flag = 1;
														}
														else
														{
															if(i == MAX_BUFFER)
															{
																i = 0;
															}
															Full_Correct_Buffer[i] = BAT_FULL_CORRECT_RATE;
															i++;
															j = 0;
															for(j = 0;j < MAX_BUFFER;j++)
															{
																total_rest_volt += Full_Correct_Buffer[j];
															}
															rt_Fuelguage.rest_vol = total_rest_volt/MAX_BUFFER;
															ocv_Fuelguage.rest_vol = rt_Fuelguage.rest_vol;
															total_rest_volt = 0;
			  					        	  //标志可以校正电池容量
		    					        	  hstr.byte_count = 1;
			  					        	  hstr.data       = temp_value;
			  					        	  byte_addr  = POWER19_DATA_BUFFER1; //REG06
			  					        	  hstr.byte_addr  = &byte_addr;
			  					        	  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
		    					        		coulomb_correct_Flag = (temp_value[0] &0x80) ;
			  					        	  if(!coulomb_correct_Flag)
			  					        	  {
									        	    //校正库仑计，将可用的库伦容量放到Buffer中
//													            __wrn("=======@@@@@@@@@AXP199  correct coulomb@@@@@@@@@@======\n");
									        	    hstr.byte_count = 2;
        					        	    hstr.data       = temp_value;
        					        	    byte_addr  = POWER19_DATA_BUFFER2;
        					        	    hstr.byte_addr  = &byte_addr;
        					        	    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
        					        	    temp_value[0] &= 0x00;
        					        	    temp_value[0] = ((((Cur_CoulombCounter /5) & 0xff0) | 0x800) >> 4);
        					        	    temp_value[1] &= 0x0f;
        					        	    temp_value[1] |= (((Cur_CoulombCounter /5) & 0x0f) << 4) ;
        					        	    hstr.byte_count = 1;
        					        	    hstr.data       = temp_value;
        					        	    byte_addr  = POWER19_DATA_BUFFER2;
        					        	    hstr.byte_addr  = &byte_addr;
        					        	    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
        					        	    
        					        	    hstr.data       = temp_value+1;
        					        	    byte_addr  = POWER19_DATA_BUFFER3;
        					        	    hstr.byte_addr  = &byte_addr;
        					        	    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
										      	  	fuelguage.bat_cap = Cur_CoulombCounter;//zhangdonglu 改变当前电池容量 110518
        					        		}
															//将当前电量百分比存入Buffer中
												  	  hstr.byte_count = 1;
			  					        	  hstr.data       = temp_value;
			  					        	  byte_addr  = POWER19_DATA_BUFFER1; //REG06
			  					        	  hstr.byte_addr  = &byte_addr;
			  					        	  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
			  					        	  temp_value[0] |= 0x80;//0xc0; //zhangdonglu 把coulomb_correct_Flag标志位置1，以免再次把库仑计里的值再赋给reg07~08。还有1位没用 110518
			  					        	  temp_value[0] &= 0xe0;
			  					        	  temp_value[0] |= ((ocv_Fuelguage.rest_vol >> 2) & 0x1f);
															temp_value[0] &= 0xbf; //标志不是第一次上电,需将REG06 bit6置0
			  					        	  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
			  					        	    //清空库仑计
			  					        	  hstr.byte_count = 1;
	      					        	  hstr.data       = temp_value;
	      					        	  byte_addr  = POWER19_COULOMB_CTL;
	      					        	  hstr.byte_addr  = &byte_addr;
	      					        	  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
	      					        	  
	      					        	  temp_value[0] |= 0x80;
	      					        	  temp_value[0] |= 0x20;
	      					        	  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
														}
												}
												esKRNL_TimeDly(200);
											}
											else
											{
												if(rt_Fuelguage.rest_vol >= BAT_FULL_CORRECT_RATE)
													rt_Fuelguage.rest_vol = BAT_FULL_CORRECT_RATE;//zhangdonglu 以免未到充电限制时，超过限制 110520
											}
														 
									}
									else if(rt_Fuelguage.charge_status ==6 &&(Charge_Stat.battery_vol > BAT_FULL_VOLT)) //充电结束
									{
										 __wrn("=======@@@@@@@@@AXP199 Is charg Finish@@@@@@@@@@======\n");
										 rt_Fuelguage.rest_vol = 100;
										 ocv_Fuelguage.rest_vol = rt_Fuelguage.rest_vol;
										 //将当前电量百分比存入Buffer中
	      					   hstr.byte_count = 1;
	      					   hstr.data       = temp_value;
	      					   byte_addr  = POWER19_DATA_BUFFER1; //REG06
	      					   hstr.byte_addr  = &byte_addr;
	      					   eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
	      					   temp_value[0] |= 0x80;
	      					   temp_value[0] &= 0xe0;
	      					   temp_value[0] |= ((ocv_Fuelguage.rest_vol >> 2) & 0x1f);
										 temp_value[0] &= 0xbf; //标志不是第一次上电,需将REG06 bit6置0
	      					   eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
	      					   //清空库仑计
	      					   hstr.byte_count = 1;
        					   hstr.data       = temp_value;
        					   byte_addr  = POWER19_COULOMB_CTL;
        					   hstr.byte_addr  = &byte_addr;
        					   eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
        					   
        					   temp_value[0] |= 0x80;
        					   temp_value[0] |= 0x20;
        					   eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
									}
									else // 放电过程
									{
										__wrn("=======@@@@@@@@@AXP199 Is DisCharging@@@@@@@@@@======\n");
										__wrn("AXP199 Charge_Stat.battery_vol =%d\n",Charge_Stat.battery_vol);
										__wrn("AXP199 Charge_Stat.battery_current =%d\n",Charge_Stat.battery_current);
										if(Charge_Stat.battery_vol < BAT_LOW_VOLT) //修正边界条件
										{
											if(rt_Fuelguage.rest_vol > BAT_LOW_CORRECT_RATE) //如果OCV电压获得的电量百分比赋初值量偏大
											{
//												__wrn("=======@@@@@@@@@AXP199 Is rest.volt is over correct@@@@@@@@@@======\n");
												if(!empty_buffer_fill_Flag)
												{
													for(i = 0;i < MAX_BUFFER;i++)
													{
														Empty_Correct_Buffer[i] = rt_Fuelguage.rest_vol;
													}
													empty_buffer_fill_Flag = 1;
												}
												else
												{
													if(i == MAX_BUFFER)
													{
														i = 0;
													}
													Empty_Correct_Buffer[i] = BAT_LOW_CORRECT_RATE;
													i++;
													j = 0;
													for(j = 0;j < MAX_BUFFER;j++)
													{
														total_rest_volt += Empty_Correct_Buffer[j];
													}
													rt_Fuelguage.rest_vol = total_rest_volt/MAX_BUFFER;
													ocv_Fuelguage.rest_vol = rt_Fuelguage.rest_vol;
													total_rest_volt = 0;
													
													//将当前电量百分比存入Buffer中
									        hstr.byte_count = 1;
									        hstr.data       = temp_value;
									        byte_addr  = POWER19_DATA_BUFFER1; //REG06
									        hstr.byte_addr  = &byte_addr;
									        eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
									        temp_value[0] |= 0x80;//0xc0;//zhangdonglu 把coulomb_correct_Flag标志位置1，以免为充满拔电使得出的电池电量过小 110520
									        temp_value[0] &= 0xe0;
									        temp_value[0] |= ((ocv_Fuelguage.rest_vol >> 2) & 0x1f);
													temp_value[0] &= 0xbf; //标志不是第一次上电,需将REG06 bit6置0
									        eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
									        //清空库仑计
									        hstr.byte_count = 1;
		    					        hstr.data       = temp_value;
		    					        byte_addr  = POWER19_COULOMB_CTL;
		    					        hstr.byte_addr  = &byte_addr;
		    					        eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
		    					        temp_value[0] |= 0x80;
		    					        temp_value[0] |= 0x20;
		    					        eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
		    					         
		    					        esKRNL_TimeDly(200);
												}
											}
											else //如果OCV电压获得的电量百分比赋初值量偏小
											{
//											__wrn("=======@@@@@@@@@AXP199 Is rest.volt  correct is not enough @@@@@@@@@@======\n");
												rt_Fuelguage.rest_vol = BAT_LOW_CORRECT_RATE;
												ocv_Fuelguage.rest_vol = rt_Fuelguage.rest_vol;
	      					      //将当前电量百分比存入Buffer中
	      					      hstr.byte_count = 1;
	      					      hstr.data       = temp_value;
	      					      byte_addr  = POWER19_DATA_BUFFER1; //REG06
	      					      hstr.byte_addr  = &byte_addr;
	      					      eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
	      					      temp_value[0] |= 0x80;//zhangdonglu 把coulomb_correct_Flag标志位置1，以免为充满拔电使得出的电池电量过小 110520
	      					      temp_value[0] &= 0xe0;
			  					      temp_value[0] |= ((ocv_Fuelguage.rest_vol >> 2) & 0x1f);
												temp_value[0] &= 0xbf; //标志不是第一次上电,需将REG06 bit6置0
	      					      eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
	      					      //清空库仑计
	      					      hstr.byte_count = 1;
        					      hstr.data       = temp_value;
        					      byte_addr  = POWER19_COULOMB_CTL;
        					      hstr.byte_addr  = &byte_addr;
        					      eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
        					      temp_value[0] |= 0x80;
        					      temp_value[0] |= 0x20;
        					      eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
        					      
        					      esKRNL_TimeDly(200);
											}
											if(Charge_Stat.battery_vol < BAT_ULOW_VOLT)  //电池几乎为空，修正电量计量
											{
//																__wrn("=======@@@@@@@@@AXP199 Is empty ，set the coulomb correct flag！@@@@@@@@@@======\n");
												rt_Fuelguage.rest_vol = 2 ;
												ocv_Fuelguage.rest_vol = rt_Fuelguage.rest_vol;
												//将当前电量百分比存入Buffer中
	      					      hstr.byte_count = 1;
	      					      hstr.data       = temp_value;
	      					      byte_addr  = POWER19_DATA_BUFFER1; //REG06
	      					      hstr.byte_addr  = &byte_addr;
	      					      eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
	      					      temp_value[0] &= 0x00;
	      					      temp_value[0] |= ((ocv_Fuelguage.rest_vol >> 2) & 0x1f);
	      					      eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
	      					      //清空库仑计
	      					    	hstr.byte_count = 1;
        					      hstr.data       = temp_value;
        					      byte_addr  = POWER19_COULOMB_CTL;
        					      hstr.byte_addr  = &byte_addr;
        					      eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
        					      temp_value[0] |= 0x80;
        					      temp_value[0] |= 0x20;
        					      eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
        					      //标志可以校正电池容量
        					      coulomb_correct_Flag = 0;
        					      hstr.byte_count = 1;
		    					      hstr.data       = temp_value;
		    					      byte_addr  = POWER19_DATA_BUFFER1; //REG06
		    					      hstr.byte_addr  = &byte_addr;
		    					      eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
		    					      temp_value[0] &= 0x7f; //将bit7清零，表示可以校正电池容量
		    					      eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
											}
										}
										else
										{
											if (rt_Fuelguage.rest_vol <= BAT_LOW_CORRECT_RATE)
												rt_Fuelguage.rest_vol = BAT_LOW_CORRECT_RATE;//zhangdonglu 以免未到条件时低于剩余容量门限 110520
										}
									}
									
									if(rt_Fuelguage.rest_vol > 100)
									{
										rt_Fuelguage.rest_vol = 100;
//										__wrn("^^^^^^^^^^^^^^^^^AXP19x final rt_Fuelguage.rest_vol =%d\n",rt_Fuelguage.rest_vol);
									}
									else if(rt_Fuelguage.rest_vol <= 2)
									{
										rt_Fuelguage.rest_vol = 2;
//										__wrn("!!!!!!!!!!!!!!AXP19x final rt_Fuelguage.rest_vol =%d\n",rt_Fuelguage.rest_vol);
									}
//									__wrn("=======@@@@@@@@@AXP199 caculat Rest Time！@@@@@@@@@@======\n");
									fuelguage.rest_vol = rt_Fuelguage.rest_vol;
									__wrn("%%%%%%%%%%%%%%%%%%*******AXP19x fuelguage.rest_vol =%d\n",fuelguage.rest_vol);
									fuelguage.bat_power = Charge_Stat.battery_current * Charge_Stat.battery_vol*Discharge_current_rate / 10;
        					
        					fuelguage.charge_status = rt_Fuelguage.charge_status;
        					rt_Fuelguage.rest_time = Get_Bat_RestTime(fuelguage.charge_status,fuelguage.rest_vol,Bat_Rdc,\
        					    													          fuelguage.bat_cap,Charge_Stat,fuelguage.bat_power);
        					Total_Time -= Bat_Time_Buffer[k];
        					Bat_Time_Buffer[k] = rt_Fuelguage.rest_time;
        					Total_Time += Bat_Time_Buffer[k];
        					k++;
        					if(k == POWER19_TIME_MAX)
        					{
        					    k = 0;
        					}
        					
        					fuelguage.rest_time = Total_Time / POWER19_TIME_MAX;
//                            __wrn("AXP199 aver fuelguage.rest_time =%d\n",fuelguage.rest_time);
                    
        			//esKRNL_TimeDly(500);
   		            esKRNL_TimeDly(100);
    					}
  					}
				}
        else if(PMU_type == 1)     //AXP18X
        {
            ES_FILE     *pfile;
            __s32       batcap_count = 0, battime_count = 0;
            __u16       batcap[POWER18_VOL_MAX], battime[POWER18_TIME_MAX];
						__u16		table_get_rest_time[5] ;
            __u16       pre_batcap = 0;
            __u8        key_value[3], rdc_flag = 0, tmp_value;
            __u8        pre_charge_status = 0,average_ADC_index = 0;
            __u16       batcap_index = 0, battime_index = 0;
            __u16       i = 0;
            __u16       Rdc_Buffer = 0;
            __u32       total_vol = 0, total_time = 0,total_table_get_rest_time = 0;
            __twi_dev_para_ex_t                hstr;
            __drv_power_battery_status_t1   bat_sts;
            __drv_power_battery_status_t2   btr_state;

            eLIBs_memset(&hstr, 0, sizeof(__twi_dev_para_ex_t));

            __POWER_MAIN_TASK_:

            for( i = 0; i < POWER18_VOL_MAX; i++ )
            {
                batcap[i] = 0;
            }

            for( i = 0; i < POWER18_TIME_MAX; i++ )
            {
                battime[i] = 0;
            }
			for(i = 0;i < 5; i++)
			{
				table_get_rest_time[i] = 0;
			}

            pfile = eLIBs_fopen("D:\\DRV\\BATRDC.INI", "r+");
            if( pfile == NULL )
            {
                pfile = eLIBs_fopen("D:\\DRV\\BATRDC.INI", "w+");
                if(pfile == NULL)
                {
                    __wrn("create file fail\n");
                    esKRNL_TimeDly(10);
                    goto __POWER_MAIN_TASK_;
                }
             //   __wrn("create file ok\n");

                //如果无此文件，说明系统第一次上电，看AXP189BUF里面是否有此数据，从buf里读数据
                 hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
                 hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
                 byte_addr          = POWER18_DATA_BUFFER1;
                 hstr.byte_addr  = &byte_addr;
                 hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
                 hstr.data               = &tmp_value;
                 hstr.byte_count         = 1;  // data is 1 bytes

                Rdc_Buffer = 0;

                eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);

                Rdc_Buffer = tmp_value << 8;
                byte_addr  = POWER18_DATA_BUFFER2;
                hstr.byte_addr  = &byte_addr;
                eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);

                Rdc_Buffer += tmp_value;
                if(Rdc_Buffer < 500)
                {
                    key_value[0] = Rdc_Buffer/100 + '0';
                    Rdc_Buffer %= 100;
                    key_value[1] = Rdc_Buffer/10 + '0';
                    Rdc_Buffer %= 10;
                    key_value[2] = Rdc_Buffer + '0';
                }
                else
                {
                    key_value[0] = '2';
                    key_value[1] = '0';
                    key_value[2] = '0';
                }

                eLIBs_fwrite(key_value, 1, 3, pfile);
            }
            eLIBs_fread(key_value, 1, 3, pfile);
            eLIBs_fclose(pfile);

            this_rdc = (((key_value[0] & 0x7f) - '0') * 100) + (key_value[1]-'0') * 10 + (key_value[2]-'0');
//            __wrn("main task bat rdc = %d\n", this_rdc);

            //有此文件，则将此RDC值写入AXP189 buf
            hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
            hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
            hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
            hstr.byte_count = 1;
            tmp_value = this_rdc & 0xff00;
            hstr.data       = &tmp_value;
            byte_addr  = POWER18_DATA_BUFFER1;
            hstr.byte_addr  = &byte_addr;

            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
            tmp_value = this_rdc & 0x00ff;
            byte_addr  = POWER18_DATA_BUFFER2;
            hstr.byte_addr  = &byte_addr;
            eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

            while(1)
            {
                if(esKRNL_TDelReq(OS_PRIO_SELF) == OS_TASK_DEL_REQ)
                {
                   esKRNL_TDel(EXEC_prioself);       //杀线程;
                }

                {
                    hstr.byte_count = 1;
                    hstr.data       = &tmp_value;
                    byte_addr  = POWER18_ONOFF;
                    hstr.byte_addr  = &byte_addr;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
                    tmp_value = (tmp_value >> 2) & 0x01;
                    if((tmp_value) && (!rdc_flag))
                    {
                        //正在充电
                        DEV_Power_18_Ioctrl((__hdle)&powerdrv.power_dev, DRV_POWER_CMD_BATADC_GET, 0, (void *)&bat_sts);
                        if(bat_sts.bat_crnt > 220)
                        {
                            rdc_flag = 1;
                            this_rdc = (DEV_Power_18_Ioctrl((__hdle)&powerdrv.power_dev, DRV_POWER_CMD_GET_BAT_RDC, 0, 0));
//                            __wrn("bat rdc = %d\n", this_rdc);
                        }
                        esKRNL_TimeDly(200);
                    }
                }

                btr_state.bat_rdc = this_rdc;
//                __wrn("bat rdc = %d\n", this_rdc);
                DEV_Power_18_Ioctrl((__hdle)&powerdrv.power_dev, DRV_POWER_CMD_BATSTATUS_TABLE_GET, 0, (void *)&btr_state);
                fuelguage.charge_status = (btr_state.battery_exist << 2) | (btr_state.dcin_valid << 1) | btr_state.charge_status;
                //111:充电，110:充电完成, 10X:放电, 0XX:没电池
                if(fuelguage.charge_status < 4)
                {
                    fuelguage.rest_vol = 0;
                    fuelguage.rest_time = 0;
                    esKRNL_TimeDly(200);
                    continue;
                }


                if(standby == 1)
                {
//                    __inf("standby == 1  clear buffer\n ");
//					__wrn("standby batcap_count = %d\n", batcap_count);
					if(average_ADC_index < 5)
					{
						table_get_rest_time[average_ADC_index] = btr_state.rest_vol;
						total_table_get_rest_time += table_get_rest_time[average_ADC_index];
						average_ADC_index ++;
//						__wrn("standby table get btr_state.rest_vol = %d\n", btr_state.rest_vol);
//						__wrn("standby average total_table_get_rest_time = %d\n", total_table_get_rest_time);
					}
					else
					{
						btr_state.rest_vol = total_table_get_rest_time/5 ;
//						__wrn("standby average btr_state.rest_vol = %d\n", btr_state.rest_vol);
						total_time = 0;
						total_vol = 0;
	    				for( i = 0; i < batcap_count; i++ )
	    				{
	    					batcap[i] = btr_state.rest_vol;
							total_vol += batcap[i];
//							 __wrn("standby total_vol = %d\n", total_vol);
	    				}

	    				for( i = 0; i < battime_count; i++ )
	    				{
	    					battime[i] = btr_state.rest_time;
							total_time += battime[i];
	//						 __wrn("standby total_time = %d\n", total_time);
	    				}
	    				pre_batcap = 	btr_state.rest_vol;
	    				standby = 0;
						total_table_get_rest_time = 0;
						average_ADC_index = 0;
					}
                }

                if(battime_index == POWER18_TIME_MAX)
                {
                    battime_index = 0;
                }

                if(battime_count < POWER18_TIME_MAX)
                {
                    battime[battime_index ++ ] = btr_state.rest_time;
                    total_time += btr_state.rest_time;
                    battime_count ++;
                }
                else
                {
                    total_time -= battime[battime_index];
                    total_time += btr_state.rest_time;
                    battime[battime_index ++ ] = btr_state.rest_time;
                }
                fuelguage.rest_time = total_time/battime_count;
//                __wrn("fuelguage.rest_time = %d\n", fuelguage.rest_time);
                if(batcap_index == POWER18_VOL_MAX)
                {
                    batcap_index = 0;
                }

                if(batcap_count < POWER18_VOL_MAX)
                {
                    batcap[batcap_index ++ ] = btr_state.rest_vol;
                    total_vol += btr_state.rest_vol;
                    batcap_count ++;
                }
                else
                {
                    total_vol -= batcap[batcap_index];
                    total_vol += btr_state.rest_vol;
                    batcap[batcap_index ++ ] = btr_state.rest_vol;
                }

                fuelguage.rest_vol = total_vol/batcap_count;
//                __wrn("fuelguage.rest_vol = %d\n", fuelguage.rest_vol);

                if((fuelguage.charge_status == 7) && ( fuelguage.rest_vol == 100))                //如果正在充电，但返回的电量是100%，则赋值为99%
                {
                     fuelguage.rest_vol = 99;
                }

                if((fuelguage.charge_status == 7)  &&  (batcap_count > POWER18_TIME_MAX))                //在充电，则电量不能比之前少，49是为了配合从standby唤醒时有50次显示打点。
                {
                    if( fuelguage.rest_vol < pre_batcap)
                    {
                        fuelguage.rest_vol = pre_batcap;
                    }
                }

                if((fuelguage.charge_status != 7) && (batcap_count > POWER18_TIME_MAX))                                                                //如果没有充电，则电量不能比之前多
                {
                    if(fuelguage.rest_vol > pre_batcap)
                    {
                        fuelguage.rest_vol = pre_batcap;
                    }
                }

                if((pre_charge_status == 7) && (fuelguage.charge_status == 6))                //结束充电时刷新为100%
                {
                    fuelguage.rest_vol = total_vol/batcap_count;
                }
                pre_charge_status = fuelguage.charge_status;
                pre_batcap = fuelguage.rest_vol ;

                esKRNL_TimeDly(200);
            }
        }
    }
    else
    {
        while(1)
        {
            fuelguage.charge_status =0x07;
            fuelguage.rest_vol =0;
            fuelguage.rest_time = 0;
            esKRNL_TimeDly(500);
       }
    }
}


/*
****************************************************************************************************
*
*             DRV_Power_MInit
*
*  Description:
*       DRV_Power_MInit
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DRV_Power_MInit(void)
{
    __twi_dev_para_ex_t        hstr;
    __u8                    tmp_value;
    __u8                byte_addr = 0;
    __s32               power_key_msg;

    p1lock   = esKRNL_SemCreate(1);
    powerdrv.used = 0;

    //获取脚本电流值
    pP1CurrentCfg = &P1Cfg;
    InitP1currentCfgPar();

    //获取全部配置信息
    //获取全部配置信息
    esCFG_GetKeyValue("power_para", "power_used", &PMU_exist, 1);
    esCFG_GetKeyValue("power_para", "power_msg",  &power_key_msg, 1);
    esCFG_GetKeyValue("power_para", "power_twi",  &board, 1);

    __wrn("power used = %d\n", PMU_exist);
    __wrn("power message = %d\n", power_key_msg);
    __wrn("power twi channal = %d\n", board);
    
    if (PMU_exist == 0)
    {
        __wrn("no PMU config\n");
        return EPDK_OK;
    }

    /*获取按键消息*/
    short_key_value = KMSG_USR_CLS_KEY | (power_key_msg & 0xff);
    __inf("power start to open iic device\n");
    if(!board)
    {
        powerdrv.power_dev.iic_file = eLIBs_fopen("b:\\BUS\\TWI0", "r+");
    }
    else
    {
        powerdrv.power_dev.iic_file = eLIBs_fopen("b:\\BUS\\TWI1", "r+");
    }
    if(!powerdrv.power_dev.iic_file)
    {
        __err("Open IIC Dev File Failed\n");
    }

    // initialise power key device
    PowerKeyDevInit();

    eLIBs_memset(&hstr, 0, sizeof(__twi_dev_para_ex_t));

    hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
    hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
    byte_addr          = 0x3; //Judge_Month the AXP19X exist or not,slave addr 0x68 and 0x0c REG value lower  than  0100
    hstr.byte_addr  = &byte_addr;
    hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
    hstr.data               = &tmp_value;
    hstr.byte_count         = 1;  // data is 1 bytes

    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))                //访问失败，表明无AXP19/17存在？
    {
        __inf("IIC read 0x68 REGISTER 0x03 fail,to Judge_Month the tmp_value = %x\n",tmp_value);
        hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
        if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))            //访问失败，表明无PMU axp18 存在？
        {
            __err("power iic read fail,no pmu exist!");
            PMU_type = 0;   //no pmu
        }
        else if((tmp_value & 0x0f) > 0x07)
        {
            __inf("IIC read 0x2c REGISTER 0x03 sucess,to Judge_Month the tmp_value = %x\n",tmp_value);
            PMU_type = 1;   //AXP18X
        }
        else
        {
            __wrn("read spec data from pmu failed!\n");
        }
    }
    else if((tmp_value & 0x0f)  == 0x01)                                                        
   {
        __inf("IIC read 0x68 REGISTER 0x03 sucess,to Judge_Month the tmp_value = %x\n",tmp_value);
        PMU_type = 3;               //AXP20X
   }
    else
    {
        __inf("IIC read 0x68 REGISTER 0x03 sucess,to Judge_Month the tmp_value = %x\n",tmp_value); //访问成功，表明AXP19/17存在？
        PMU_type = 2;
    }
    __inf("PMU type  = %x\n",PMU_type);

    if(PMU_exist)
    {
        oldisr = esINT_InsFIR(INTC_IRQNO_FIQ, P1_NMI_irq_handle, _cb_p1_nmi_irq_handle, 0);
        esINT_EnableINT(INTC_IRQNO_FIQ);
    }
		else
		{
			PMU_type = 0;
		}

    return EPDK_OK;
}


/*
****************************************************************************************************
*
*             DRV_Power_MExit
*
*  Description:
*       DRV_Power_MExit
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DRV_Power_MExit(void)
{
    __u8 err;

    esKRNL_SemDel(p1lock, OS_DEL_ALWAYS, &err);

    if(PMU_exist)
    {
        esINT_DisableINT(INTC_IRQNO_FIQ);
        esINT_UniISR(INTC_IRQNO_FIQ, (__pISR_t)oldisr);
    }
    return EPDK_OK;
}


/*
****************************************************************************************************
*
*             DRV_Power_MOpen
*
*  Description:
*       DRV_Power_MOpen
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__mp *DRV_Power_MOpen(__u32 mid, __u32 mod)
{
    powerdrv.mid = mid;
    powerdrv.used ++;

    return (__mp *)&powerdrv;
}


/*
****************************************************************************************************
*
*             DRV_Power_MClose
*
*  Description:
*       DRV_Power_MClose
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DRV_Power_MClose(__mp *mp)
{
    if(powerdrv.used)
    {
        powerdrv.used --;
    }

    return EPDK_OK;
}


/*
****************************************************************************************************
*
*             DRV_Power_MRead
*
*  Description:
*       DRV_Power_MRead
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DRV_Power_MRead(void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return 0;
}


/*
****************************************************************************************************
*
*             DRV_Power_MWrite
*
*  Description:
*       DRV_IRKEY_MWrite
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DRV_Power_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return 0;
}


/*
****************************************************************************************************
*
*             DRV_Power_MIoctrl
*
*  Description:
*       DRV_IRKEY_MIoctrl
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DRV_Power_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer)
{
    __u8 byte_addr  = 0 ;
    switch(cmd)
    {
        case DRV_CMD_PLUGIN:
        {
			first_setup_system = EPDK_TRUE;
            if(PMU_exist)
            {
                __twi_dev_para_ex_t   hstr;
                __u8               tmp_value;
                __drv_power_pwm_t  pwm;

                eLIBs_memset(&hstr, 0, sizeof(__twi_dev_para_ex_t));

                hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
                hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
								
				//20110621 add by zhwj 		
				__msg("PMU_type=%d\n",PMU_type);
				if(PMU_type == 3)        //AXP20x
                {
                    /*对于存在AXP20的情况，注册可以操作AXP209的设备*/
                    powerdrv.hReg = esDEV_DevReg("HID", "POWER", &power_20_dev_ops, 0);
                    if(!powerdrv.hReg)
                    {
                        __wrn("Power registered Error!\n");
                        eLIBs_fclose(powerdrv.power_dev.iic_file);
                        return EPDK_FAIL;
                    }
                    hstr.slave_addr = POWER_BOARD1_DEV_ADDR_20;
                    hstr.byte_count = 1;
                    hstr.data       = &tmp_value;
                    byte_addr  = POWER20_HOTOVER_CTL;  //Open Internal over temperature shut down
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }

                    tmp_value |= 1<<2;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    //Add By Evan, Initial clear  the long and short KEY IRQ
                    hstr.data       = &tmp_value;
                    tmp_value = 0x00;
                    byte_addr  = POWER20_INTEN1;
                    hstr.byte_addr  = &byte_addr;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    tmp_value = 0x00;
                    byte_addr  = POWER20_INTEN2;
                    hstr.byte_addr  = &byte_addr;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    tmp_value = 0x03;
                    byte_addr  = POWER20_INTEN3;
                    hstr.byte_addr  = &byte_addr;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                    
                    tmp_value = 0x00;
                    byte_addr  = POWER20_INTEN4;
                    hstr.byte_addr  = &byte_addr;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                    
                    tmp_value = 0x00;
                    byte_addr  = POWER20_INTEN5;
                    hstr.byte_addr  = &byte_addr;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    hstr.data       = &tmp_value;
                    byte_addr  = POWER20_INTSTS1;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    tmp_value &= 0xff;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    byte_addr  = POWER20_INTSTS2;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    tmp_value = 0xff;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    byte_addr  = POWER20_INTSTS3;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    tmp_value = 0xff;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    byte_addr  = POWER20_INTSTS4;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    tmp_value = 0xff;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                    
                    byte_addr  = POWER20_INTSTS5;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    tmp_value = 0xff;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    //Added By Evan ,V B and C Compaliant,20090517
                    byte_addr  = POWER20_VERSION;
                    hstr.byte_addr  = &byte_addr;
                    hstr.data       = &tmp_value;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
					AXP209_Version = tmp_value;
                    if(AXP209_Version & 0x10)
                    {
                        __inf("AXP209 version B\n");
                        
                        byte_addr  = POWER20_COULOMB_RESULT;
	                    hstr.byte_addr  = &byte_addr;
	                    hstr.data       = &tmp_value;
	                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
	                    {
	                        break;
	                    }
	                    tmp_value &= 0x7f;
	                    tmp_value |= 0x80; //Fuel guage logic Hold
	                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
	                    {
	                        break;
	                    }
	                    byte_addr  = POWER20_RDC_H5;
	                    hstr.byte_addr  = &byte_addr;
	                    hstr.data       = &tmp_value;
	                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
	                    {
	                        break;
	                    }
	                    tmp_value &= 0x7f;
	                    tmp_value |= 0x00; // disable RDC calculation 
	                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
	                    {
	                        break;
	                    }
	                    byte_addr  = POWER20_COULOMB_RESULT;
	                    hstr.byte_addr  = &byte_addr;
	                    hstr.data       = &tmp_value;
	                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
	                    {
	                        break;
	                    }
	                    tmp_value &= 0x7f;
	                    tmp_value |= 0x00; //Fuel guage logic Normal
	                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
	                    {
	                        break;
		                }
                    }
                    else
                    {
                        __inf("AXP209 version C\n");
                    }

					hstr.data       = &tmp_value;
                    byte_addr  = POWER20_CHARGE1;  //设置1200mA充电电流
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }

                    __inf("Power Init!\n");
                    if(AXP209_Version & 0x10)           //B 1200mA
                    {
                        tmp_value &= ~(0x0f << 0);
                        tmp_value |= ( 0xc0 + (0x0f << 0));
                        eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                        __inf("B Charge Cur %x\n",tmp_value);
                    }
                    else                                //C 1200mA
                    {
                        tmp_value &= ~(0x0f << 0);
                        tmp_value |= ( 0xc0 + (0x0f << 0));
                        eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                        __inf("B Charge Cur %x\n",tmp_value);
                    }

                    byte_addr  = POWER20_APS_WARNING1; //限压，调整voff, vwarning电压到最低
                    hstr.byte_addr  = &byte_addr;
                    tmp_value = 0x5f;                       //Level1 3400
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                    byte_addr  = POWER20_APS_WARNING2; //限压，调整voff, vwarning电压到最低
                    hstr.byte_addr  = &byte_addr;
                    tmp_value = 0x56;                       //Level2 3350
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
				}
//add by zhwj 20110621								
                if(PMU_type == 2)        //AXP19/17X
                {
                    /*对于存在AXP19的情况，注册可以操作AXP199的设备*/
                    powerdrv.hReg = esDEV_DevReg("HID", "POWER", &power_19_dev_ops, 0);
                    if(!powerdrv.hReg)
                    {
                        __wrn("Power registered Error!\n");
                        eLIBs_fclose(powerdrv.power_dev.iic_file);
                        return EPDK_FAIL;
                    }
                    //add...
                    pwm.frequency = 50;
                    pwm.duty = 50;
                    DEV_Power_19_Ioctrl((__hdle)&powerdrv.power_dev, DRV_POWER_CMD_PWM_SET, PWM2, &pwm);
                    //add...
                    hstr.slave_addr = POWER_BOARD1_DEV_ADDR_19;
                    hstr.byte_count = 1;
                    hstr.data       = &tmp_value;
                    byte_addr  = POWER19_HOTOVER_CTL;  //Open Internal over temperature shut down
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }

                    tmp_value |= 1<<2;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    //Add By Evan, Initial clear  the long and short KEY IRQ
                    hstr.data       = &tmp_value;
                    tmp_value = 0x00;
                    byte_addr  = POWER19_INTEN1;
                    hstr.byte_addr  = &byte_addr;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    tmp_value = 0x00;
                    byte_addr  = POWER19_INTEN2;
                    hstr.byte_addr  = &byte_addr;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    tmp_value = 0x03;
                    byte_addr  = POWER19_INTEN3;
                    hstr.byte_addr  = &byte_addr;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                    tmp_value = 0x00;
                    byte_addr  = POWER19_INTEN4;
                    hstr.byte_addr  = &byte_addr;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    hstr.data       = &tmp_value;
                    byte_addr  = POWER19_INTSTS1;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    tmp_value &= 0xff;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    byte_addr  = POWER19_INTSTS2;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    tmp_value = 0xff;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    byte_addr  = POWER19_INTSTS3;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    tmp_value = 0xff;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    byte_addr  = POWER19_INTSTS4;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    tmp_value = 0xff;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    byte_addr  = 0xe0;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
//                    __inf("0xe0 = %x",tmp_value);

                    byte_addr  = 0xe1;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
//                    __inf("0xe1 = %x",tmp_value);

                    byte_addr  = 0xe2;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
//                    __inf("0xe2 = %x",tmp_value);

                    byte_addr  = 0xe3;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
//                    __inf("0xe3 = %x",tmp_value);

                    //Added By Evan ,V B and C Compaliant,20090517
                    byte_addr  = POWER19_VERSION;
                    hstr.byte_addr  = &byte_addr;
                    hstr.data       = &tmp_value;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
										AXP199_Version = tmp_value;
                    if(AXP199_Version & 0x10)
                    {
                        __inf("AXP199 version C\n");
                    }
                    else
                    {
                        __inf("AXP199 version B\n");
                    }

										hstr.data       = &tmp_value;
                    byte_addr  = POWER19_CHARGE1;  //设置780mA充电电流
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }

                    __inf("Power Init!\n");
                    if(AXP199_Version & 0x10)           //C 700mA
                    {
                        tmp_value &= ~(0x07 << 0);
                        tmp_value |= ( 0x80 + (0x03 << 0));
                        eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                        __inf("C Charge Cur %x\n",tmp_value);
                    }
                    else                                //B 720mA
                    {
                        tmp_value |= (0x01 << 7);
                        eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                        __inf("b Charge Cur %x\n",tmp_value);
                    }

                    byte_addr  = POWER19_APS_WARNING1; //限压，调整voff, vwarning电压到最低
                    hstr.byte_addr  = &byte_addr;
                    tmp_value = 0x5f;                       //Level1 3400
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                    byte_addr  = POWER19_APS_WARNING2; //限压，调整voff, vwarning电压到最低
                    hstr.byte_addr  = &byte_addr;
                    tmp_value = 0x56;                       //Level2 3350
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    //设置EXTEN为3.3V，打开EXTEN，拉低GPIO7（N_VBUSEN）
                    byte_addr  = POWER19_GPIO67_CTL;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    tmp_value  &= 0xf9;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    //add...
					//for neostra to set PIO3 to 0
					byte_addr  = POWER19_SENSE_CTL;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    tmp_value  &= 0xfc;
					tmp_value  |= 0x81;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

					byte_addr  = POWER19_SENSE_SIGNAL;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    tmp_value  &= 0xfe;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
					__here__;
					//add...
				
                    byte_addr  = POWER19_LDO3OUT_VOL;
                    hstr.byte_addr  = &byte_addr;
                    tmp_value = 0x68;                       //set exten 3.3V
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    byte_addr  = POWER19_LDO3_DC2_CTL;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }

                    tmp_value &= 0xfb;
                    tmp_value |= (0x01 << 2);
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
					eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
                    //__wrn("Read Secend Exten Value = %x\n" ,tmp_value);

                    Config_for_gedmei(); // add by zhwj 20110618 
                    byte_addr	= POWER19_CHARGE2;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                     break;
                    }
                    tmp_value &= 0xfb;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                    {
                     break;
                    }
				}

                else if(PMU_type == 1)
                {
                    /*对于存在AXP18的情况，注册可以操作AXP189的设备*/
                    powerdrv.hReg = esDEV_DevReg("HID", "POWER", &power_18_dev_ops, 0);
                    if(!powerdrv.hReg)
                    {
                        __wrn("Power registered Error!\n");
                        eLIBs_fclose(powerdrv.power_dev.iic_file);
                        return EPDK_FAIL;
                    }
                    hstr.slave_addr = POWER_BOARD1_DEV_ADDR_18;
                    hstr.byte_count = 1;

                    hstr.data       = &tmp_value;
                    byte_addr  = POWER18_DC12OUT_VOL;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    tmp_value |= 1<<7;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    //Add By Evan, On/Off delay,512-128mS,1.5S-1S,20091117
                    hstr.data       = &tmp_value;
                    byte_addr  = POWER18_PEK;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    tmp_value &= 0x0f;
                    tmp_value |= 0x90;  //set 1s power on  bit[7:6 ]=[00 ]128ms ,[01 ]512ms, [10] 1s, [11 ]2s
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    //Add By Evan, Initial clear  the long and short KEY IRQ,20091119
                    hstr.data       = &tmp_value;
                    byte_addr  = POWER18_INTSTS2;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    //Added By Evan ,V B and C Compaliant,20090517
                    byte_addr  = POWER18_VENDER_USED1;
                    hstr.byte_addr  = &byte_addr;
                    hstr.data       = &AXP189_Version;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    if(AXP189_Version & 0x10)
                    {
                        __inf("AXP189 version C\n");
                    }
                    else
                    {
                        __inf("AXP189 version B\n");
                    }
                    hstr.data = &tmp_value;

                    if(AXP189_Version & 0x10)       //C 版本 Enale 6S关机
                    {
                        tmp_value = 0x48 ;
                        byte_addr  = POWER18_INTEN1;
                        hstr.byte_addr  = &byte_addr;
                        eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                    }
                    else
                    {
                        tmp_value = 0x40;
                        byte_addr  = POWER18_INTEN1;
                        hstr.byte_addr  = &byte_addr;
                        eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                    }

                    tmp_value = 0x03 << 2;
                    byte_addr  = POWER18_INTEN2;
                    hstr.byte_addr  = &byte_addr;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    tmp_value = 0x00;
                    byte_addr  = POWER18_INTEN3;
                    hstr.byte_addr  = &byte_addr;
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    byte_addr  = POWER18_ADCSW_CTL;    //打开电池检测使能,包括电流和电压检测
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    tmp_value &= ~(0x03 << 6);
                    tmp_value |=  (0x03 << 6);
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

                    byte_addr  = POWER18_CHARGE1;      //设置720mA充电电流
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }

                    __inf("Power Init!\n");
                    if(AXP189_Version & 0x10)           //C 700mA
                    {
                        tmp_value &= ~(0x07 << 0);
                        tmp_value |= ( 0x80 + (0x03 << 0));
                        eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                        __inf("C Charge Cur %x\n",tmp_value);
                    }
                    else                                //B 720mA
                    {
                        tmp_value &= ~(0x07 << 0);
                        tmp_value |= (0x80 +  (0x07 << 0));
                        eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                        __inf("b Charge Cur %x\n",tmp_value);
                    }

                    byte_addr  = POWER18_IPS_SET;  //不限压，调整voff, vwarning电压到最低
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }

                    if(AXP189_Version & 0x10)           //C VOFF/VWARNING default
                    {
                        tmp_value |= (0x01 << 6);
                        eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                        __inf("C Vwarning/Voff %x\n",tmp_value);
                    }
                    else                                //B VOFF/VWARNING Lowest
                    {
                        tmp_value |=  (0x01 << 6);      //修改VHOLD为限压模式
                        tmp_value &= ~(0x0f << 0);
                        eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                        __inf("B Vwarning/Voff %x\n",tmp_value);
                    }

                    tmp_value = 300 & 0xff;
                    byte_addr  = POWER18_DATA_BUFFER1;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }

                    tmp_value = (300>>8) & 0xff;
                    byte_addr  = POWER18_DATA_BUFFER2;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    //add by zhwj for DCDC2 work in PWM mode at 20101009
                    byte_addr  = POWER18_DCDCCTL;
                    hstr.byte_addr  = &byte_addr;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                    tmp_value &= 0xbd;
                    tmp_value |= 0x40;
                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }
                }
            }
            else
            {
                /*对于不存在AXP的情况，注册对AXP199进行空操作的设备*/
                powerdrv.hReg = esDEV_DevReg("HID", "POWER", &nopower_dev_ops, 0);
                if(!powerdrv.hReg)
                {
                    __wrn("Power registered Error!\n");
                    eLIBs_fclose(powerdrv.power_dev.iic_file);
                    return EPDK_FAIL;
                }
            }
						__here__;
            tid = esKRNL_TCreate(power_main_task, 0, 0x800, KRNL_priolevel3);

            break;
        }

        case DRV_CMD_PLUGOUT:
        {
            if(PMU_exist)
            {
                __twi_dev_para_ex_t   hstr;
                __u8               tmp_value;

                eLIBs_memset(&hstr, 0, sizeof(__twi_dev_para_ex_t));
                if(PMU_type == 2)        //AXP19/17X
                {
                    hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
                    hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
                    byte_addr          = POWER19_HOTOVER_CTL;
                    hstr.byte_addr  = &byte_addr;
                    hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
                    hstr.data               = &tmp_value;
                    hstr.byte_count         = 1;  // data is 1 bytes

                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }

                    tmp_value &= ~(1<<7);
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                }
                else if(PMU_type == 1)
                {
                    hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_18;
                    hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
                    byte_addr          = POWER18_DC12OUT_VOL;
                    hstr.byte_addr  = &byte_addr;
                    hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
                    hstr.data               = &tmp_value;
                    hstr.byte_count         = 1;  // data is 1 bytes

                    if(EPDK_FAIL == eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr))
                    {
                        break;
                    }

                    tmp_value &= ~(1<<7);
                    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
                }
            }

            if(powerdrv.power_dev.iic_file)
            {
                eLIBs_fclose(powerdrv.power_dev.iic_file);
            }

            if(!powerdrv.hReg)
            {
                __wrn("Dev not plugin!\n");
                return EPDK_FAIL;
            }
            esDEV_DevUnreg(powerdrv.hReg);

            esKRNL_TDel(tid);

            break;
        }

        case DRV_CMD_GET_STATUS:
        {
            if(powerdrv.used)
            {
                return DRV_STA_BUSY;
            }
            else
            {
                return DRV_STA_FREE;
            }
        }
        default:
            return EPDK_FAIL;
    }

    return EPDK_OK;
}
/*
****************************************************************************************************
*
*             Init_AXP19x_MainTask
*
*  Description:
*       Init_AXP19x_MainTask
*
*  Parameters: NULL
*
*  Return value:
*       NULL
****************************************************************************************************
*/
static	void Init_AXP19x_MainTask()
{
		__u8 byte_addr = 0;
		__twi_dev_para_ex_t            hstr = {0};
		 __u8 temp_value[3] ;
    
		hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
    hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
    byte_addr          = POWER19_ADC_EN1;  //使能充电电流与电压ADC
    hstr.byte_addr  = &byte_addr;
    hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
    hstr.data               = temp_value;
    hstr.byte_count         = 3;  // data is 1 bytes
    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);

    temp_value[0] |= 0xfe;
    temp_value[2] &= 0x3f;
    temp_value[2] |= 0x80;
    hstr.byte_count = 1;
    hstr.data       = temp_value;
    byte_addr  = POWER19_ADC_EN1;//使能充电电流与电压ADC
    hstr.byte_addr  = &byte_addr;
    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

    hstr.data       = temp_value+2;
    byte_addr  = POWER19_ADC_SPEED;//Set voltage and current ADC Sample Rate
    hstr.byte_addr  = &byte_addr;
    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

   // hstr.byte_count = 3;
   // hstr.data       = temp_value;
   // byte_addr  = POWER19_ADC_EN1;//使能充电电流与电压ADC 并且设置他们的采样率
   // hstr.byte_addr  = &byte_addr;
   // eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);

    hstr.byte_count = 1;
    hstr.data       = temp_value;
    temp_value[0] = 0x80;
    byte_addr  = POWER19_TIMER_CTL;//定时器clear
    hstr.byte_addr  = &byte_addr;
    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

    hstr.byte_count = 1;
    hstr.data       = temp_value;
    byte_addr  = POWER19_COULOMB_CTL;//使能库仑计
    hstr.byte_addr  = &byte_addr;
    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);

	temp_value[0] |= 0x80;
    hstr.byte_count = 1;
    hstr.data       = temp_value;
    byte_addr  = POWER19_COULOMB_CTL;//使能库仑计
    hstr.byte_addr  = &byte_addr;
    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
}

/*
****************************************************************************************************
*
*             Get_Rdc
*
*  Description:
*       Get_Rdc
*
*  Parameters: NULL
*
*  Return value: NULL
*       
****************************************************************************************************
*/
static __u16 Get_Rdc()
{
		__u8 byte_addr = 0,Pre_Rdc_Flag = 0;
		__twi_dev_para_ex_t            hstr;
		__u16 Bat_Rdc = 0;
		__u16 Rdc_temp =0 ;
		__drv_power_charge_stat     Charge_Stat;
		__u8 temp_value[2] ;
		if(PMU_type == 2)
		{
			hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
	    	hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
	    	hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
//			__wrn("AXP19x start calculat Rdc!\n");	
			
		  hstr.byte_count = 2;
		  hstr.data       = temp_value;
		  byte_addr  = POWER19_DATA_BUFFER3;//获取Buffer中的RDC
		  hstr.byte_addr  = &byte_addr;
		  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
		  Rdc_temp = ((temp_value[0] & 0x0f) << 8) + temp_value[1];
		  Pre_Rdc_Flag = (Rdc_temp & 0x800) >> 11;
		  if(Pre_Rdc_Flag)
		  {
		      Rdc_temp = (Rdc_temp & 0x7ff);
		      Bat_Rdc = Rdc_temp*3;
		      if(Bat_Rdc > 70 && Bat_Rdc < 800)
	      	{
	      		Bat_Rdc = Bat_Rdc;
	      	}
	      	else
      		{
      			Bat_Rdc 				= 200;
      			temp_value[0] 	&= 0xf7;
      			hstr.byte_count = 1;
					  hstr.data       = temp_value;
					  byte_addr  			= POWER19_DATA_BUFFER3;//清楚RDC已经计算正确标志位
					  hstr.byte_addr  = &byte_addr;
					  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
      		}
		  }
		  else
		  {
		      DEV_Power_19_Ioctrl((__hdle)&powerdrv.power_dev, DRV_POWER_CMD_CHGSTAT_GET, 0, &Charge_Stat);
				  if(Charge_Stat.bat_current_direction && Charge_Stat.charge_indication && \
				  	(Charge_Stat.battery_current > 300))            //charge and current > 300mA
				  {
					  hstr.byte_count = 2;
					  hstr.data       = temp_value;
					  byte_addr  = POWER19_DATA_BUFFER3;//获取Buffer中的RDC
					  hstr.byte_addr  = &byte_addr;
					  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr); 
				    Bat_Rdc = DEV_Power_19_Ioctrl((__hdle)&powerdrv.power_dev, DRV_POWER_CMD_GET_BAT_RDC, 0, 0);
//						__wrn("Bat_Rdc = %d\n",Bat_Rdc);
				    Rdc_temp = ((10 * Bat_Rdc + 15) / 30);
				    Rdc_temp |=	0x800 ; //标志RDC已经计算完成
				    Rdc_temp &= 0x0f00 ;
						temp_value[0] &= 0xf0;//zhangdonglu 110519,can't remove high 4 bits
				    temp_value[0] |= (Rdc_temp >> 8) ;
//				    __wrn("Rdc temp_value[0] = %x\n",temp_value[0]);
				    Rdc_temp = ((10 * Bat_Rdc + 15) / 30);
				    temp_value[1] &= 0x00;
				    temp_value[1] = Rdc_temp & 0x00ff ;
//				    __wrn("Rdc temp_value[1] = %x\n",temp_value[1]);
				    hstr.byte_count = 1;
				    hstr.data =  temp_value;
				    byte_addr  = POWER19_DATA_BUFFER3;//将Rdc 存入Buffer 中,其中将RDC的高3bit放到Buffer3的低3bit中，Buffer 4存放低8bit，Buffer3 的bit 3存放是否已经校正电池内阻
				    hstr.byte_addr  = &byte_addr;
				    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
				    hstr.data =  temp_value+1;
				    byte_addr  = POWER19_DATA_BUFFER4;
				    hstr.byte_addr  = &byte_addr;
				    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
				  }
				  else
				  {
		      	Bat_Rdc = 200;
		      }
		  }
		}
		else if(PMU_type == 3)
		{
			hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
	    hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
	    hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
//			__wrn("AXP19x start calculat Rdc!\n");	
			
		  hstr.byte_count = 2;
		  hstr.data       = temp_value;
		  byte_addr  = POWER20_DATA_BUFFER3;//获取Buffer中的RDC
		  hstr.byte_addr  = &byte_addr;
		  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
		  Rdc_temp = ((temp_value[0] & 0x0f) << 8) + temp_value[1];
		  Pre_Rdc_Flag = (Rdc_temp & 0x800) >> 11;
		  if(Pre_Rdc_Flag)
		  {
		      Rdc_temp = (Rdc_temp & 0x7ff);
		      Bat_Rdc = Rdc_temp*3;
		      if(Bat_Rdc > 70 && Bat_Rdc < 800)
	      	{
	      		Bat_Rdc = Bat_Rdc;
	      	}
	      	else
      		{
      			Bat_Rdc 				= 200;
      			temp_value[0] 	&= 0xf7;
      			hstr.byte_count = 1;
					  hstr.data       = temp_value;
					  byte_addr  			= POWER20_DATA_BUFFER3;//清楚RDC已经计算正确标志位
					  hstr.byte_addr  = &byte_addr;
					  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
      		}
		  }
		  else
		  {
		      DEV_Power_20_Ioctrl((__hdle)&powerdrv.power_dev, DRV_POWER_CMD_CHGSTAT_GET, 0, &Charge_Stat);
				  if(Charge_Stat.bat_current_direction && Charge_Stat.charge_indication && \
				  	(Charge_Stat.battery_current > 300))            //charge and current > 300mA
				  {
					  hstr.byte_count = 2;
					  hstr.data       = temp_value;
					  byte_addr  = POWER20_DATA_BUFFER3;//获取Buffer中的RDC
					  hstr.byte_addr  = &byte_addr;
					  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr); 
				    Bat_Rdc = DEV_Power_20_Ioctrl((__hdle)&powerdrv.power_dev, DRV_POWER_CMD_GET_BAT_RDC, 0, 0);
//						__wrn("Bat_Rdc = %d\n",Bat_Rdc);
				    Rdc_temp = ((10 * Bat_Rdc + 15) / 30);
				    Rdc_temp |=	0x800 ; //标志RDC已经计算完成
				    Rdc_temp &= 0x0f00 ;
						temp_value[0] &= 0xf0;//zhangdonglu 110519,can't remove high 4 bits
				    temp_value[0] |= (Rdc_temp >> 8) ;
//				    __wrn("Rdc temp_value[0] = %x\n",temp_value[0]);
				    Rdc_temp = ((10 * Bat_Rdc + 15) / 30);
				    temp_value[1] &= 0x00;
				    temp_value[1] = Rdc_temp & 0x00ff ;
//				    __wrn("Rdc temp_value[1] = %x\n",temp_value[1]);
				    hstr.byte_count = 1;
				    hstr.data =  temp_value;
				    byte_addr  = POWER20_DATA_BUFFER3;//将Rdc 存入Buffer 中,其中将RDC的高3bit放到Buffer3的低3bit中，Buffer 4存放低8bit，Buffer3 的bit 3存放是否已经校正电池内阻
				    hstr.byte_addr  = &byte_addr;
				    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
				    hstr.data =  temp_value+1;
				    byte_addr  = POWER20_DATA_BUFFER4;
				    hstr.byte_addr  = &byte_addr;
				    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
				  }
				  else
				  {
		      	Bat_Rdc = 200;
		      }
		  }
		}  
	  return Bat_Rdc;
}

/*
****************************************************************************************************
*
*             Config_for_gedmei
*
*  Description:
*      (1) set SYSEN to GPIO7
*			 (2) set PWREN to GPIO6
*			 (3) set LDO2 to 3.0V
*			 (4) pull down GPIO7
*  Parameters: NULL
*
*  Return value: NULL
*       
****************************************************************************************************
*/

static void Config_for_gedmei()
{
	__twi_dev_para_ex_t            hstr = {0};
	__u8    temp_addr;
	__u8    temp_value[1],byte_addr = 0;
	
	hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_19;
  hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
  hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
	hstr.byte_count = 1;
	temp_value[0] = 0xc0;
  hstr.data       = temp_value;
  byte_addr  = POWER19_LDO24OUT_VOL;//调整LDO2为3.0V，LDO4为1.8V
  hstr.byte_addr  = &byte_addr;
  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr); 
  
  temp_value[0] = 0x06;
  temp_addr 			= 0xf4;
  hstr.data       = temp_value;
  byte_addr  = temp_addr;//开锁
  hstr.byte_addr  = &byte_addr;
  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
  
  temp_value[0] = 0x04;
  temp_addr 			= 0xf5;
  hstr.data       = temp_value;
  byte_addr  = temp_addr;//开锁
  hstr.byte_addr  = &byte_addr;
  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr); 
  
  hstr.data       = temp_value;
  temp_addr 			= 0xe0;
  byte_addr  = temp_addr;//设置SYSEN、PWREN作为GPIO
  hstr.byte_addr  = &byte_addr;
  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
  temp_value[0] &= 0xfe;
  temp_value[0] |= 0x01;
  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
  
  temp_value[0] = 0x00;
  temp_addr 			= 0xf5;
  hstr.data       = temp_value;
  byte_addr  = temp_addr;//关锁
  hstr.byte_addr  = &byte_addr;
  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);
  
  temp_value[0] = 0x00;
  temp_addr 			= 0xf4;
  hstr.data       = temp_value;
  byte_addr  = temp_addr;//关锁
  hstr.byte_addr  = &byte_addr;
  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr); 
  
  temp_value[0] = 0x20;
  temp_addr 			= 0x9f;
  hstr.data       = temp_value;
  byte_addr  = temp_addr;//将GPIO7设置为低电平输出
  hstr.byte_addr  = &byte_addr;
  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr); 
  
  temp_value[0] = 0x45;
  temp_addr 			= 0x34;
  hstr.data       = temp_value;
  byte_addr  = temp_addr;//设置外部通路电流为300mA
  hstr.byte_addr  = &byte_addr;
  eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);  
  
}

/*
****************************************************************************************************
*
*             Init_AXP19x_MainTask
*
*  Description:
*       Init_AXP19x_MainTask
*
*  Parameters: NULL
*
*  Return value:
*       NULL
****************************************************************************************************
*/
static	void Init_AXP20x_MainTask()
{
		__u8 byte_addr = 0;
		__twi_dev_para_ex_t            hstr = {0};
		 __u8 temp_value[3] ;
         __u8 value;
	hstr.slave_addr         = POWER_BOARD1_DEV_ADDR_20;
    hstr.slave_addr_flag    = TWI_SLAVE_ADDRESS_07BIT;
    
    byte_addr          = POWER20_ADC_EN1;  //使能充电电流与电压ADC
    hstr.byte_addr  = &byte_addr;
    hstr.byte_addr_width    = 1;  // reg_addr is 1 byte
    hstr.data               = temp_value;
    hstr.byte_count         = 3;  // data is 1 bytes
    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);

    temp_value[0] |= 0xfe;
    temp_value[2] &= 0x3f;
    temp_value[2] |= 0x80;
    hstr.byte_count = 1;
    hstr.data       = temp_value;
    byte_addr  = POWER20_ADC_EN1;//使能充电电流与电压ADC
    hstr.byte_addr  = &byte_addr;
    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

    hstr.data       = temp_value+2;
    byte_addr  = POWER20_ADC_SPEED;//Set voltage and current ADC Sample Rate
    hstr.byte_addr  = &byte_addr;
    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

   // hstr.byte_count = 3;
   // hstr.data       = temp_value;
   // byte_addr  = POWER19_ADC_EN1;//使能充电电流与电压ADC 并且设置他们的采样率
   // hstr.byte_addr  = &byte_addr;
   // eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);

    hstr.byte_count = 1;
    hstr.data       = temp_value;
    temp_value[0] = 0x80;
    byte_addr  = POWER20_TIMER_CTL;//定时器clear
    hstr.byte_addr  = &byte_addr;
    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

    hstr.byte_count = 1;
    hstr.data       = temp_value;
    byte_addr  = POWER20_COULOMB_CTL;//使能库仑计
    hstr.byte_addr  = &byte_addr;
    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);

	temp_value[0] |= 0x80;
    hstr.byte_count = 1;
    hstr.data       = temp_value;
    byte_addr  = POWER20_COULOMB_CTL;//使能库仑计
    hstr.byte_addr  = &byte_addr;
    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

    value = 0x40;
    hstr.byte_count = 1;
    hstr.data       = &value;
    byte_addr  = POWER20_OFF_CTL;//
    hstr.byte_addr  = &byte_addr;
    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);


    value = 0x63;
    hstr.byte_count = 1;
    hstr.data       = &value;
    byte_addr  = POWER20_IPS_SET;//
    hstr.byte_addr  = &byte_addr;
    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_WRITE_SPEC_RS, 0, &hstr);

    hstr.byte_count = 1;
    hstr.data       = temp_value;
    byte_addr  = POWER20_CHARGE1;//使能库仑计
    hstr.byte_addr  = &byte_addr;
    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
    __msg("POWER20_CHARGE1 temp_value[0] is 0x%x \n",temp_value[0]);

    hstr.byte_count = 1;
    hstr.data       = temp_value;
    byte_addr  = POWER20_IPS_SET;//使能库仑计
    hstr.byte_addr  = &byte_addr;
    eLIBs_fioctrl(powerdrv.power_dev.iic_file, TWI_READ_SPEC_RS, 0, &hstr);
    __msg("POWER20_IPS_SET temp_value[0] is 0x%x \n",temp_value[0]);


}


