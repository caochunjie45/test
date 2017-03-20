/*
*********************************************************************************************************
*											        eMOD
*						           the Easy Portable/Player Operation System
*									           mod_orange sub-system
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File   : mod_ornage.h
* Version: V1.0
* By     : miracle
*********************************************************************************************************
*/
#ifndef _MOD_ORANGE_H_
#define _MOD_ORANGE_H_

#include "typedef.h"
#include "cfgs.h"

typedef  __hdle H_WIN;
typedef  __hdle H_LYR;
    #define   ORANGE_OK			        EPDK_OK
    #define   ORANGE_FAIL		        EPDK_FAIL
    #define   ORANGE_TRUE		        EPDK_TRUE
    #define   ORANGE_FALSE		        EPDK_FALSE
   #define   ORANGE_WRN                __wrn
    #define   ORANGE_INF                __inf
    #define   ORANGE_ERR                __err
    #define   ORANGE_MSG                __msg


    #define  orange_memcpy              eLIBs_memcpy
    #define  orange_memset              eLIBs_memset
    #define  orange_memclr              eLIBs_memclr
    #define  orange_strlen              eLIBs_strlen
    #define  orange_strcpy              eLIBs_strcpy
    #define  orange_strcat              eLIBs_strcat
    #define  orange_strncat             eLIBs_strncat
    #define  orange_strcmp              eLIBs_strcmp
    #define  orange_strncpy             eLIBs_strncpy

    #define  orange_malloc(size)        esMEMS_Malloc(0,size)
    #define  orange_mfree(ptr)          esMEMS_Mfree(0,ptr)
    #define  orange_bmalloc(size)       esMEMS_Balloc(size)
    #define  orange_bfree(ptr,size)     esMEMS_Bfree(ptr,size)
    #define  orange_pmalloc(size)       esMEMS_Palloc((size + 1023)>>10,0)
    #define  orange_pfree(ptr,size)     esMEMS_Pfree(ptr,(size + 1023)>>10)
    #define  orange_Dalay(time)         esKRNL_TimeDly(time)
    #define  orange_flush(addr,size)    esMEMS_CleanFlushDCacheRegion(addr,size)

/**********************************************************/

typedef enum tag_DISP_LAYER_OUTPUT_CHN
{
    DISP_LAYER_OUTPUT_CHN_DE_CH1,
    DISP_LAYER_OUTPUT_CHN_DE_CH2
}__disp_layer_output_chn_t;

typedef struct tag_DISP_LAYER_PARA
{
    __disp_layer_work_mode_t       mode;			/*set layer work mode                                           */
    __bool                         ck_mode;         /*layer color mode                                              */
    __bool                         alpha_en;        /*layer alpha enable mode switch                                */
    __u16                          alpha_val;       /*layer alpha value                                             */
    __u8                           pipe;            /*layer pipe                                                    */
    __u8                           prio;            /* layer priority                                               */
    RECT                           scn_win;         /* sceen region                                                 */
    RECT                           src_win;         /* framebuffer source window                                    */
    __disp_layer_output_chn_t      channel;         /*only use by internal framebuffer mode 0:  DE_CH1, 1:DE_CH2    */
    FB                            *fb;
}__disp_layer_para_t;

/************************************************************/
#include "mod_orange\\gui\\gui_config.h"

/*������غ��������ݽṹ�Ķ���*/
#include "mod_orange\\gui\\core\\guiconf.h"
#include "mod_orange\\gui\\core\\guitouchconf.h"
#include "mod_orange\\gui\\core\\guidebug.h"
#include "mod_orange\\gui\\core\\guitype.h"
#include "mod_orange\\gui\\core\\guiversion.h"
#include "mod_orange\\gui\\core\\gui_core.h"
#include "mod_orange\\gui\\core\\gui_confdefaults.h"
#include "mod_orange\\gui\\core\\lcd.h"

#include "mod_orange\\gui\\core\\gui_block.h" //modified be Derek,2011-1-15 12:36:18

/*ͼ��ʹ�ú��������ݽṹ*/
#include "mod_orange\\gui\\layer\\layer.h"
#include "mod_orange\\gui\\layer\\lyr_lock.h"

/*��Ϣ���ͽ��պ�������Ϣ���ݽṹ�ͳ�������*/
#include "mod_orange\\gui\\message\\message.h"

/*���ڲ���������������ݽṹ����*/
#include "mod_orange\\gui\\win\\win.h"

/*widgetϵͳ��������������ݽṹ����,����ʵ�ֶԿؼ����ע��ж�ػ��ƽ��й���*/
#include "mod_orange\\gui\\widget\\winclass.h"


/*��Ӧ�ؼ������ݽṹ�Ͳ�����������*/
#include "mod_orange\\gui\\widget\\widget.h"
/**********************************************************/



#include "mod_orange\\res\\themdec.h"
#include "mod_orange\\res\\langdec.h"

#include "mod_orange\\orange_mod_api.h"

enum
{
	ORANGE_CMD_GET_FBADDR = 0,
	ORANGE_CMD_SET_KEYID
};

#ifdef SIM_PC_WIN
#define  AWAPI __declspec(dllimport)
extern AWAPI __u32 g_nCurOCBID;
extern __s32 g_nModualID;
#define __SAVE_MOD_EVN__  g_nCurOCBID = g_nModualID;
#define __RESTORE_MOD_EVN__ g_nCurOCBID = 0;
#else
#define __SAVE_MOD_EVN__
#define __RESTORE_MOD_EVN__
#endif


#endif /*_MOD_ORANGE_H_*/
