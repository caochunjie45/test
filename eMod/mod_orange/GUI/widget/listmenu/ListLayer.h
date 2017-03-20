/*
*******************************************************************************************************************
*                                                         	Graphic Library 
*                                         			the graphic library support module
*
*                             				 Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name:       ListLayer.h
*
*Author:          William Wang
*
*Version :        1.1.0
*
*Date:            2008-10-20
*
*Description :    ListLayer head definition 
*
*Others :         None at present.
*
* History :
*
* <Author>          <time>              <version>     <description>
*
* William Wang     2008-10-20    1.1.0          Create File
*
*******************************************************************************************************************
*/

#ifndef __LIST_LAYER__H____
#define __LIST_LAYER__H____

#include "config.h"
#include "ListBuffer.h"
#include "MenuList.h"

struct tag_LISTMENU_DATA;

#define   FIXED_MODE   1
#define   VAR_MODE     2

#define   KEY_FOCUS     0
#define   TOUCH_FOCUS   1

#define   NO_FOUCS     0
#define   REAL_FOUCS   1     /*ʵ�����ý���*/
#define   VAR_FOUCS    2     /*�������ý���(���ٻ���ʱ����)*/

#define   WAIT_LOAD    0x01

#define   LISTLAYER_ROUNDUP   0x01
#define   LISTLAYER_MOVE      0x02
#define   LISTLAYER_ROUNDDOWN 0x03
//------------------------------------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------------------------------------
typedef struct tag_LIST_LAYER_MAN
{
    __u8                        LoadTID;        /*item�����̵߳�id*/
    __u8                        LoadPrio;       /*item�����̵߳�*/
    __u8                        FoucsTID;
    __u8                        FoucsPrio;
    __s8                        fillup;         /*only used by linear mode*/
    __u8                        filldown;       /*only used by linear mode*/
    __u8                        rollspeed;
    __u8                        mode;
    __u8                        scnnum;         /*ʵ������*/
    __u8                        fb_depth;
    __u8                        upcount;        /*��ʼλ����Ҫ����item����*/
    __u8                        downcount;      /*����λ����Ҫ����item����*/                  
    __bool                      adjust_wm;
	H_LYR	    	            layer;
    __u32                       msgupcount;      /*��Ϣ����*/
    __u32                       msgdowncount;      /*��Ϣ����*/
    __u32                       bk_color;
    __u32                       dir;
    __u32                       msgcount;
    __u8                        cursteps;      /*ÿ����һ��item��ǰ�Ĳ���*/
    __u8                        nextsteps;     /*ÿ����һ��item��һ������*/
    __u16                       scn_width;     /*��ǰlistmenu������Ļ�Ŀ��*/
    __u16                       scn_height;     /*��ǰlistmenu������Ļ�ĸ߶�*/
    __bool                      msgflush;
    __u8                        setfoucs; 
    __rect_t                    scnrect;
    __rect_t                    userscnrect;
	__rect_t			        viewrect;
    __pos_t                     head_pos;       /*��fixed���ԣ�������ʼʱ����Ļ�е�λ��
                                                  ��variable����,�����̨���ص���ʼλ��*/       
    __pos_t                     tail_pos;       /*��fixed���ԣ��������ʱ����Ļ�е�λ��
                                                  ��variable����,�����̨���صĽ���λ��*/
    __pos_t                     headlyr_pos;       /*��fixed���ԣ�������ʼʱ����Ļ�е�λ��
                                                  ��variable����,�����̨���ص���ʼλ��*/       
    __pos_t                     taillyr_pos;       /*��fixed���ԣ��������ʱ����Ļ�е�λ��
                                                  ��variable����,�����̨���صĽ���λ��*/
    __pos_t                     foucs_pos;
    __pos_t                     unfoucs_pos;
	LIST_BUFFER_MAN_t *         listbuffer;
	MENULIST_t*                 menulist;   
    __krnl_flag_grp_t*          load_flags;
    __krnl_flag_grp_t*          wait_flags;
    __krnl_flag_grp_t*          foucs_flags;
    __krnl_event_t*             pos_sem;
    __krnl_event_t*             load_sem;
    struct tag_LISTMENU_DATA*   plistmenu;
    __u32                       status;  
    __u8                        focus_type;
}LIST_LAYER_MAN_t;

__s32          ListLayer_GetBPP         (LIST_LAYER_MAN_t *plist_layer);
__s32          ListLayer_SwitchFB       (LIST_LAYER_MAN_t *listlayer,__bool fbno,__u32 x,__u32 y);
__s32          ListLayer_SwitchLyr      (LIST_LAYER_MAN_t *listlayer,__u32 height,__u32 y);
__hdle         ListLayer_Create         (H_LYR listlayer,__hdle listmenu,__u32 item_w,__u32 foucs_w);
__u32          ListLayer_Destroy        (LIST_LAYER_MAN_t *listlayer);
__u32          ListLayer_Wakeup         (LIST_LAYER_MAN_t *list);
__u32          ListLayer_Sleep          (LIST_LAYER_MAN_t *list);
__u32          ListLayer_OnDraw         (LIST_LAYER_MAN_t *listlayer);
__u32          ListLayer_SetWinFoucs    (LIST_LAYER_MAN_t *listlayer);
__u32          ListLayer_SetWinUnFoucs  (LIST_LAYER_MAN_t *   listlayer);
#endif //__LIST_LAYER__H____





