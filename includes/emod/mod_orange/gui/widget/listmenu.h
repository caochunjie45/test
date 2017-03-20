/*
*******************************************************************************************************************
*                                                         	Graphic Library 
*                                         			the graphic library support module
*
*                             				 Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name:       Listmenu.h
*
*Author:          William Wang
*
*Version :        1.1.0
*
*Date:            2008-10-15
*
*Description :    listmenu data structure and const data definition 
*
*Others :         No
* History :
*
* <Author>          <time>         <version>     <description>
*
* William Wang     2008-10-15         1.1.0        Create File
*
*******************************************************************************************************************
*/
#ifndef __LIST__MENU__H____
#define __LIST__MENU__H____

#define CTRL_LISTMENU         ("listmenu")

typedef  __hdle  H_ITEM;

typedef struct tag_DRAW_PARA
{
    H_ITEM                          item;
    __u32                           mode;
    __u32                           aux;
    RECT                            invalidrect;       
}__draw_para_t;

typedef __s32 (* __pITEM_DRAW_CB) (__draw_para_t * item_para);

typedef struct move_para
{
    __s32     dist;
    __u32     dly_time;    
}__move_para_t;

typedef struct __LSTM_ITEM_CTRL
{
    char                            *name;                  /*�ؼ���������*/
    
    char                            * spClassName;          /*�ؼ����ڵ�������*/
    char                            * spCaption;            /*�ؼ����ڵı�������*/        
    __u32                           dwStyle;                /*�ؼ����ڵķ��*/
    __u32                           dwExStyle;              /*�ؼ����ڵ���չ���*/
    __u16                           id;                     /*�ؼ����ڵı�ʶID*/
    RECT                            ctl_rect;               /*�ؼ�������Item�е�λ��*/
    void                            *attr;                  /*�ؼ����ڵ�˽����Ϣ*/
}__lstm_item_ctrl_t;

//------------------------------------------------------------------------------------------------------------
//Mode  �ƶ���ģʽ�������
//Steps һ���ƶ���steps
//CurStep ָʾ��ǰ������λ�õ�������0��ʾ��ʼ״̬������Steps-1��ʾ����״̬
//����ֵ��ʾ�ô�״̬��y��λ������һ��״̬y��λ�õ�ƫ�����Լ��ƶ���ƫ����֮����ӳ�ʱ��(��10msΪ��λ);
//------------------------------------------------------------------------------------------------------------
typedef __move_para_t *  (*__cb_LSTM_Move_t)(__u32 mode, __u32 Steps, __u32 CurStep);

typedef struct __LSTM_ITEM_ATTR
{
    char                          *name;
    __u32                          item_w_unfocs;              /* item�Ŀ��                                                 */
    __u32                          item_w_focs;                /* item�Ŀ�ȣ����㣩                                         */
    __pITEM_DRAW_CB                cb;                         /* item draw�ص�����                                          */
    void *                         p_arg;                      /* �û��Զ������������                                       */
    __u32                          nctrl;                    /* item��ʼ��Ŀ                                                 */                     
    __lstm_item_ctrl_t *           ctrls;                    /* item�����ָ��                                               */
}__lstm_item_attr_t;

typedef struct __LSTM_ATTR
{
    __u32                          nitem;                    /* item��ʼ��Ŀ                                                 */                     
    __lstm_item_attr_t *           items;                    /* item�����ָ��                                               */

    __cb_LSTM_Move_t               pf_cb_Move;               /* �˶��켣����                                                 */
    __u32                          steps;                    /* �˶��켣����ÿ����һ������Ҫ���е�steps����                */
    __u32                          mode;                     /* listmenu�Ĺ���ģʽ��ѭ����������......                       */
                                                             /* bit0   framebufferģʽ��ѭ�����������Թ���                   */
                                                             /* bit1~3 framebuffer�����ٶ�����                               */                                                          
                                                             /* bit3   framebuffer������ģʽ                                 */     
    __u32                          curitemno;                /*��ʼ������item���*/
    __u32                          bk_color;
}__lstm_para_t;

typedef enum roll_mode
{
   LINEAR_ROLL                    = 0,                            /*����framebufferģʽ                                           */
   CIRCLE_ROLL                    = 1 << 0                        /*ѭ�� framebuffer ģʽ                                         */   
}__roll_mode_e;

typedef enum roll_speed
{
   FAST_SPEED                     = 0 << 1,                       /*���� ����                                                     */
   LOW_SPEED                      = 1 << 1,                        /*���ٹ���ģʽ                                                  */
   SPD_LEVEL1					  = 2 << 1,
   SPD_LEVEL2					  = 3 << 1,
   SPD_LEVEL3					  = 4 << 1,
   SPD_LEVEL4					  = 5 << 1,
   SPD_LEVEL5					  = 6 << 1,
   SPD_LEVEL6					  = 7 << 1
}__roll_speed_e;


typedef enum lstm_mode
{
   HORIZONTAL_MOD                = 0 << 4,       
   VERITICAL_MOD                 = 1 << 4,
   LISTMENU_MOD                  = 0 << 5,
   LISTBOX_MOD                   = 1 << 5,
   KEYTONE_ON					 = 0 << 6,
   KEYTONE_OFF					 = 1 << 6,
   FOCUS_MOD                     = 0 << 7,
   UNFOCUS_MOD                   = 1 << 7
}__lstm_mode_e;

#define LSTM_DRAWCB_ERROR           0xffff
#define LSTM_DRAWCB_OK              0


#define LSTMN_SELCHANGE             1
#define LSTMN_DBCLCLK               2
#define LSTMN_SELCANCEL             3
#define LSTMN_SETFOCUS              4
#define LSTMN_KILLFOCUS             5
#define LSTMN_CLICKCHECKMARK        6
#define LSTMN_CLICKED               7
#define LSTMN_PUSHED                8
#define LSTMN_UNPUSHED              9

#define LSTMM_USERDEF               (GUI_MSG_CONTROL)
#define LSTMM_RUNUP                 (GUI_MSG_CONTROL + 0)
#define LSTMM_RUNDOWN               (GUI_MSG_CONTROL + 1)
#define LSTMM_RUNLEFT               (GUI_MSG_CONTROL + 2)
#define LSTMM_RUNRIGHT              (GUI_MSG_CONTROL + 3)
#define LSTMM_RUNSTOP               (GUI_MSG_CONTROL + 4)
#define LSTMM_MOVEUP                (GUI_MSG_CONTROL + 5)
#define LSTMM_MOVEDOWN              (GUI_MSG_CONTROL + 6)
#define LSTMM_MOVEMSG               (GUI_MSG_CONTROL + 7)

/*
**********************************************************************************************************************
*                                               FUNCTION
**********************************************************************************************************************
*/ 
H_ITEM      GUI_LstmGetCurItem      (H_WIN lstm);
__u32       GUI_LstmGetItemNo       (H_WIN H_Item);
__u32       GUI_LstmSetCurNo        (H_WIN lstm,__u32 curno);
__u32       GUI_LstmDrawCurNo       (H_WIN lstm,__u32 curno);
__u32       GUI_LstmStopSlip        (H_WIN lstm);
H_ITEM      GUI_LstmGetItem         (H_WIN lstm,__u32 no);
__s32       GUI_LstmUpdateItem      (H_WIN lstm,__u32 no);
__bool      GUI_LstmChildInFb       (H_WIN lstm,H_ITEM handle);
__u32   	GUI_LstmDelCurItem      (H_WIN lstm);
__u32       GUI_LstmAddItemTail     (H_WIN lstm,__lstm_item_attr_t *NewItem);
__u32       GUI_LstmGetItemCount    (H_WIN lstm);
//__u32       GUI_LstmRegSlaveLyr     (H_WIN lstm,H_LYR lyr);
H_LYR       GUI_LstmGetItemLyrWin   (H_WIN H_Item);
__s32       GUI_LstmGetItemRect     (H_WIN H_Item,RECT *rect);
__u32       GUI_LstmGetItemAttr     (H_WIN H_Item);
H_WIN  		GUI_LstmGetHostWin		(H_WIN H_Item);
__bool      RegisterListMenuControl (void);
#endif
