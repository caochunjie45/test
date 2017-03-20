/*
*******************************************************************************************************************
*                                                         	Module Orange
*                                         			the embedded Graphical module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name�� 	sicons_i.h
*
*Author�� 		William Wang
*
*Version :   	1.1.0
*
*Date:  		2010-5-20
*
*Description : 	sicons internal head definition 
*
*Others : 		None at present.
*
* History :
*
* <Author>          <time>         <version>     <description>
*
* William Wang     2010-5-20         1.1.0          Create File
*
*******************************************************************************************************************
*/
#ifndef __SICONS_I_H__
#define __SICONS_I_H__

#include "..\\winclass_i.h"

#define sicons_malloc(size)           esMEMS_Malloc(0,size)
#define sicons_free(ptr)              esMEMS_Mfree(0,ptr)
#define sicons_balloc(size)           esMEMS_Balloc(size)
#define sicons_bfree(ptr,size)        esMEMS_Bfree(ptr,size)
#define sicons_palloc(size)           esMEMS_Palloc(((size + 1023 )/1024), MEMS_PALLOC_MODE_BND_1K);
#define sicons_pfree(ptr,size)        esMEMS_Pfree(ptr,((size + 1023 )/1024));
#define sicons_memcpy                 eLIBs_memcpy
#define sicons_memset                 eLIBs_memset

typedef __hdle  h_iconssitem;

#define LISTITEMST_PUSHED              (1<<1)

#define LISTITEMST_FOCUS               (1<<2)

/*ȫ���������û�iconitem����*/
typedef struct tag_ICONS_MEMUITEM
{
	struct tag_ICONS_MEMUITEM *         Prev;
	struct tag_ICONS_MEMUITEM *         Next;
    __u32                               itemno;
    IconsItem_DrawCB                    cb;                         /* item draw�ص�����                                          */
    H_WIN                               handle;                     /*iconitem����listicon�ľ��*/
    void*                               attr;
    __u32                               item_w;
    __bool                              enable;
}__icons_iconitem_t;

#define   SICONS_ItemH2P(h_item)       ((__icons_iconitem_t *)h_item)
#define   SICONS_ItemP2H(pitem)        ((h_iconssitem)pitem) 

/*������Ҫ���Ƶ�Item����ͻ��Ʋ���*/
typedef struct tag_ICONS_SITEM
{
    RECT	                            rect;
    IconsItem_DrawCB                  cb;
    __icons_iconitem_t*               iconitem;
}__icons_sitem_t;

//------------------------------------------------------------------------------------------------------------
typedef struct tag_ICONS_ICONLIST
{
    /*last current item*/
	__icons_iconitem_t *              head;
	__icons_iconitem_t *              tail;
	__u32			                    ItemCount;

    /*only used in icons mod*/
    __icons_iconitem_t *              HeadItem;

    /*the number of page item num*/
    __u32                               pagenum;
}__icons_iconlist_t;

typedef struct tag_SLITBOX_DATA
{
    __u8                                scnnum;         /*ʵ������*/                
    __u32                               bk_color;
    __u32                               status; 
    __u32                               draw_sts;
	__icons_iconlist_t*               iconlist; 
    H_WIN                               handle;        /*listicon���ڴ��ڵľ��*/          
    __u32                               lastmsg;
    void*                               bk_bmp;
    __u32                               bmp_pos;
    __bool                              bk_draw;
    h_iconssitem                      *sitem_array;
    
    /*��ǰ����icons����ʾ��Ч��Ŀ��*/
    __u32                               valid_num;
    __u32                               cur_index;
    __u32                               last_index; 
    GUI_MEMDEV_Handle                   hmem;

    __u32                               bpp;
    __u32                               lyr_w;
    __u32                               lyr_h;
    void*                               fb_addr;
    H_LYR                               lyr_win;
}__sicons_data_t;

typedef __sicons_data_t* piconsdata;

#define ICONS_DRAWSTATUS(pCtrl)                    (((piconsdata)GUI_CtrlWinGetAddData(pCtrl))->draw_sts)
#define ICONS_STATUS(pCtrl)                        (((piconsdata)GUI_CtrlWinGetAddData(pCtrl))->status)
//#define BUTTON_STSTRANS(pCtrl)                     (BUTTON_OLDSTATUS(pCtrl) != BUTTON_STATUS(pCtrl))
#define ICONS_DATA(pCtrl)                          (((piconsdata)GUI_CtrlWinGetAddData(pCtrl))->data)

__u32                   SIconItem_Destroy(__icons_iconitem_t *iconitem);
__s32                   SShowItem_Destory               (__icons_sitem_t* sitem);
__icons_sitem_t *       SShowItem_Create(__icons_sitem_t *sitem_attr);
__icons_iconitem_t *  SIconItem_Create             (__sicons_item_attr_t *item_attr,H_WIN  hWnd);
__icons_iconitem_t *  SIconItem_GetPrev            (__icons_iconitem_t *iconitem);
__icons_iconitem_t *  SIconItem_GetNext            (__icons_iconitem_t *iconitem);
                                                     
__u32                   SIconList_InsertAfter        (__icons_iconlist_t *iconlist  , __icons_iconitem_t *after , __icons_iconitem_t *iconitem);
__icons_iconitem_t *  SIconList_GetIconitem        (__icons_iconlist_t *iconlist,__s32 index);
__u32	                SIconList_DelItem            (__icons_iconlist_t *iconlist , __icons_iconitem_t *iconitem);
__u32	                SIconList_DelAll             (__icons_iconlist_t *iconlist);
__u32	                SIconList_AddTail            (__icons_iconlist_t *iconlist,  __icons_iconitem_t *iconitem);
__u32	                SIconList_AddHead            (__icons_iconlist_t *iconlist , __icons_iconitem_t *iconitem);
__icons_iconlist_t*   SIconList_Create             (void);
__icons_iconitem_t *  SIconList_GetHead            (__icons_iconlist_t *iconlist);
H_WIN                   SIconList_GetCurItem         (__icons_iconlist_t *iconlist);
__u32                   SIconList_SetCurItem         (__icons_iconlist_t *iconlist , __icons_iconitem_t *iconitem);
__icons_iconitem_t *  SIconList_GetTail            (__icons_iconlist_t *iconlist);
__u32	                SIconList_GetItemCount       (__icons_iconlist_t *iconlist);
__u32                   SIconList_Destroy            (__icons_iconlist_t *iconlist);
__icons_iconitem_t*   SIconList_FindNextEnable     (__icons_iconlist_t* iconlist,__icons_iconitem_t *cur_item);
__icons_iconitem_t*   SIconList_FindPrevEnable     (__icons_iconlist_t* iconlist,__icons_iconitem_t *cur_item);
__s32                   SIconList_SetHeadItemFromItem(__icons_iconlist_t* iconlist,__icons_iconitem_t *cur_item);
__s32                   SIconList_GetCurPageNum      (__icons_iconlist_t* iconlist);
#endif

