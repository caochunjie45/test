/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              eMOD Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : widget
* File    : button.h
*
* By      : Steven
* Version : v1.0
* Date    : 2008-10-4 11:28:56
**********************************************************************************************************************
*/
#ifndef __GUI_BUTTON_H__
#define __GUI_BUTTON_H__

#include "ResAdapter.h"

#define CTRL_BUTTON         ("button")

#define BS_PUSHBUTTON       0x00000000L
#define BS_USERBUTTON       0x00000008L

#define BS_TYPEMASK         0x0000000FL
#define BST_PUSHED          (1<<1)

#define BST_FOCUS           (1<<2)
#define BM_GETSTATE         0xF0F2
#define BM_CLICK            0xF0F5

#define BN_CLICKED          0
#define BN_PUSHED           1
#define BN_UNPUSHED         2
#define BN_DISABLE          4   
#define BN_DOUBLECLICKED    5

#define BN_DBLCLK           BN_DOUBLECLICKED
#define BN_SETFOCUS         6
#define BN_KILLFOCUS        7

typedef struct tagbtnpara
{
	DECLARE_BMP_RES_Pt          (focus_bmp);
	DECLARE_BMP_RES_Pt          (unfocus_bmp);
	__pos_t     				bmp_pos;
    char        				*text;
    __pos_t     				text_pos;
    __u32			   			ftxt_color;
    __u32		   				uftxt_color;
	GUI_FONT*                   draw_font;
    __epdk_charset_enm_e        draw_code;   /*�ַ�����*/
    __u32                       bk_color;
    __u32                       txt_align;
    __bool                      alpha_en;
    
}button_para_t;



typedef struct button_action
{
	__pGUI_WIN_CB                    left_action;
	
	__pGUI_WIN_CB                    right_action;
	
	__pGUI_WIN_CB                    enter_action;
	
	__pGUI_WIN_CB                    return_action;
	
	
}button_action_t;



typedef struct button_attribute
{
	button_para_t               button_paint;
	
	button_action_t             button_action;

	
}button_attribute_t;


typedef struct tagBUTTONDATA
{
    GUI_MEMDEV_Handle   hmem;
    __u32               draw_sts;
    __u32               status;           /* button flags */
    __u32               data;             /* bitmap or icon of button. */
} button_data_t;

typedef button_data_t* pbuttondata;  


#define BUTTON_DRAWSTATUS(pCtrl) (((pbuttondata)GUI_CtrlWinGetAddData(pCtrl))->draw_sts)
#define BUTTON_STATUS(pCtrl)   (((pbuttondata)GUI_CtrlWinGetAddData(pCtrl))->status)
//#define BUTTON_STSTRANS(pCtrl)  (BUTTON_OLDSTATUS(pCtrl) != BUTTON_STATUS(pCtrl))
#define BUTTON_DATA(pCtrl)     (((pbuttondata)GUI_CtrlWinGetAddData(pCtrl))->data)

__bool RegisterButtonControl        (void);
__s32  GUI_ButtonSetFocusBmp        (H_WIN hbutton,void *bmp);
__s32  GUI_ButtonSetUnFocusBmp      (H_WIN hbutton,void *bmp);
__s32  GUI_ButtonSetText            (H_WIN hbutton,char *text);
__s32  GUI_ButtonSetUnFcsTxtColor   (H_WIN hbutton,__u32 color);
__s32  GUI_ButtonSetFcsTxtColor     (H_WIN hbutton,__u32  color);
__s32  GUI_ButtonSetBkColor     	(H_WIN hbutton,__u32  color);
__s32  GUI_ButtonSetBmpPos          (H_WIN hbutton,__pos_t *pos);
__s32  GUI_ButtonSetTxtPos          (H_WIN hbutton,__pos_t *pos);
#endif /* #ifndef __GUI_BUTTON_H__ */
