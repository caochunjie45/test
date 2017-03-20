/*
*******************************************************************************************************************
*                                                         	Graphic Library 
*                                         			the graphic library support module
*
*                             				 Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name:       ListLayer_Process.h
*
*Author:          William Wang
*
*Version :        1.1.0
*
*Date:            2008-10-20
*
*Description :    ListLayer Process head file definition 
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
#ifndef __LIST_LAYER_PROCESS_H____
#define __LIST_LAYER_PROCESS_H____


#include "config.h"
#include "ListBuffer.h"
#include "ListMenu_i.h"
#include "MenuList.h"

typedef enum EVENT_LIST_FLAGS
{
   LISTLAYER_LOAD_FLAG            = 1<<0,      /*listmenu�����̵߳����ñ�־λ*/
   LISTLAYER_WAIT_FLAG            = 1<<1,      /*�����̵߳ĵȴ����ر�־λ*/
   LISTLAYER_FOUCS_FLAG           = 1<<2,      /*�轹�̵߳����б�־λ*/
   LISTLAYER_FOUCS_FINISH_FLAG    = 1<<3,      /*�轹�߳����ý������*/
   LISTLAYER_DRAW_FINISH_FLAG     = 1<<4,   
   LISTLAYER_DRAW_ITEM_FLAG       = 1<<5,      /*�轹�߳�(ͬʱ����ֹͣ����ʱ����ɢ����)*/
   LISTLAYER_WAIT_LOAD_FLAG       = 1<<6       /*�����߳�ֹͣʱ���͸������̵߳ı�־λ*/
}EVENT_LIST_FLAGS_E;

typedef struct list_event_trigger
{
    __u8         direct;
    __u8         num;
}list_event_trigger_t;

typedef enum LISTBUFFER_STATUS
{
   INMIDDLE     = 0,            //���Ӵ���λ������
   UPWARDS      = 1 << 0,       //���Ӵ���λ��ƫ��
   DOWNWARDS    = 1<<1,         //���Ӵ���λ��ƫ��
   WAITUP       = 1<<2,         //���Ӵ��ڲ������ϻ���
   WAITDOWN     = 1<<3,         //���Ӵ��ڲ������»���
   DOWNOVER     = 1<<4,
   UPOVER       = 1<<5
}LISTBUFFER_STATUS_E;

void 	_lstm_key_song                  (void);
__u32   InterMove                       (__u32 mode, __s32 range, __u32 Steps, __u32 CurStep,__move_para_t *Parameter);
void    ListLayer_MenuItemUpdateChild   (MENUITEM_t   *menuitem);
__u32   ListLayer_MenuItemCopy          (LIST_LAYER_MAN_t *listlayer,__u32 srcfbno,__u32 dstfbno,__u32 src_x,__u32 src_y,__u32 dst_x,__u32 dst_y);
__u32   ListLayer_LoadMenuItem          (LIST_LAYER_MAN_t *listlayer,MENUITEM_t *item,__u8       fbno,__u32 x,__u32 y);
__u32   ListLayer_UnFoucsMenuItem       (LIST_LAYER_MAN_t *listlayer,MENUITEM_t *item,__u8       fbno,__u32 x,__u32 y);
__u32   ListLayer_DrawFastFoucsMenuItem (LIST_LAYER_MAN_t *   listlayer,MENUITEM_t   *menuitem,__u8 fbno,__u32 x,__u32 y);
__u32   ListLayer_DrawFoucsMenuItem     (LIST_LAYER_MAN_t *   listlayer,MENUITEM_t   *menuitem,__u8 fbno,__u32 x,__u32 y);
__u32   ListLayer_FastFoucsMenuItem     (LIST_LAYER_MAN_t *listlayer,MENUITEM_t *item,__u8       fbno,__u32 x,__u32 y);
__u32   ListLayer_FastUnFoucsMenuItem   (LIST_LAYER_MAN_t *listlayer,MENUITEM_t *item,__u8       fbno,__u32 x,__u32 y);
__u32   ListLayer_FastLoadMenuItem      (LIST_LAYER_MAN_t *listlayer,MENUITEM_t *item,__u8       fbno,__u32 x,__u32 y);
__u32   ListLayer_DrawActivedMenuItem   (LIST_LAYER_MAN_t *   listlayer,MENUITEM_t   *menuitem,__u8 fbno,__u32 x,__u32 y);
__u32   ListLayer_DrawInActivedMenuItem (LIST_LAYER_MAN_t *   listlayer,MENUITEM_t   *menuitem,__u8 fbno,__u32 x,__u32 y);
__u32   ListLayer_GetBufferStatus       (LIST_LAYER_MAN_t *listlayer);
void    ListLayer_LoadItemThread        (void* p_arg);
__s32   ListLayer_SetFoucs              (LIST_LAYER_MAN_t *listlayer,MENUITEM_t *menuitem,__u32 x,__u32 y);
__u32   ListLayer_ClearMenuItem         (LIST_LAYER_MAN_t *listlayer,__u8   fbno,__u32 x,__u32 y);
__u32   ListLayer_SetLayerFoucs         (LIST_LAYER_MAN_t *listlayer,MENUITEM_t *menuitem);
__u32   ListLayer_SetLayerUnFoucs       (LIST_LAYER_MAN_t *listlayer);
__u32   ListLayer_SetLayerFastFoucs     (LIST_LAYER_MAN_t *   listlayer,MENUITEM_t   *menuitem);
void    ListLayer_FoucsItemThread       (void* p_arg);
__u32   ListLayer_KeyUpMsg              (LIST_LAYER_MAN_t *listlayer,__u32 dist,__u32  step);
__u32   ListLayer_KeyDownMsg            (LIST_LAYER_MAN_t *listlayer,__u32 dist,__u32 step);
__bool  ListLayer_GetLoadStatus         (LIST_LAYER_MAN_t *listlayer);
__u32   ListLayer_OnMesssage            (LIST_LAYER_MAN_t *   listlayer, __listmenu_runmsg_t*pMsg);
__u32   ListLayer_MoveDownMsg           (LIST_LAYER_MAN_t *listlayer,__u32 dist,__u32  step);
__u32   ListLayer_MoveUpMsg             (LIST_LAYER_MAN_t *listlayer,__u32 dist,__u32  step);
__s32   ListLayer_GetMoveStatus         (LIST_LAYER_MAN_t *listlayer);
#endif
