/*
*******************************************************************************************************************
*                                                         	Graphic Library 
*                                         			the graphic library support module
*
*                             				 Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name:       ListMenu_i.h
*
*Author:          William Wang
*
*Version :        1.1.0
*
*Date:            2008-10-24
*
*Description :    listmenu head file definition 
*
*Others :         None at present.
*
* History :
*
* <Author>          <time>              <version>     <description>
*
* William Wang     2008-10-24    1.1.0          Create File
*
*******************************************************************************************************************
*/
#ifndef __LISTMENU_I_H__
#define __LISTMENU_I_H__

#include "ListMenu_e.h"
#include "ListLayer.h"
#include "GlideCurve.h"

#define GUI_MSG_USRDEF          (GUI_MSG_CONTROL + 100)

#define LISTMENU_FRM_NUMBER        64
#define LISTMENU_FRM_TXQ_DEPTH     16
#define LISTMENU_FRM_RXQ_DEPTH     64
#define LISTMENU_OTHER_NUM         2
/**********************************************************************************************************************/
/* listmenu֡��ʽ                                                                                                        */
#define LISTMENU_FRMBUF_FREEFLAG      0xff      /* ͨ��������buffer��ʹ���ˣ�flag������ΪIPIPE_FRMBUF_FREEFLAG           */
#define LISTMENU_FRMBUF_USEDFLAG      0x00
#define LISTMENU_FIXEDEXT_WIDTH       0x00      /*listmenu�̶�ģʽ����β��չ���*/

typedef enum LISTMENU_STATUS
{
    LISTMENU_ONRES      = 0,
    LISTMENU_ONDRAW     = 1 << 1,                
    LISTMENU_ONRUN      = 1 << 2,              /*�˶�״̬*/
    LISTMENU_ONLOAD     = 1 << 3,              /*����״̬*/
    LISTMENU_PUSHED     = 1 << 8,
    LISTMENU_UNPUSHED   = 1 << 9,
    LISTMENU_FOCUS      = 1 << 10
}LISTMENU_STATUS_E;

/*listmenu�ؼ�����ͼ���״̬��Ϣ*/
typedef enum LISTMENU_LYRSTS
{
    LISTMENU_WAKEUP,                    /*listmenuռ��framebuffer*/
    LISTMENU_SLEEP                      /*listbufferframebuffer�ͷ�*/
}LISTMENU_LYRSTS_E;

typedef enum LISTMENU_REQCMD
{
    LISTMENU_REQSTOP = 1<<0        /*Ӧ�ó�������ֹͣ*/        
}LISTMENU_REQCMD_e;


/*listmenu��Ϣ���нṹ*/
typedef struct LISTMENU_RUNMSG
{
    __u32       id;            
    __u32       dwAddData;
}__listmenu_runmsg_t;

#define STEP_ARRAY			  128

typedef enum tag_glide_state
{
	LSTMGLIDE_IDLE,
	LSTMGLIDE_START,
	LSTMGLIDE_MOVE,
	LSTMGLIDE_STOP	
}lstmglide_state_t;


/*listmenu������Ϣ�ṹ*/
typedef struct tag_LISTMENU_DATA
{
	LIST_LAYER_MAN_t	    *listlayer;                 /*ͼ�������Ϣ�ṹ*/
	__listmenu_runmsg_t     *msgbuff;                   /*��Ϣ����ṹ*/  
    __krnl_event_t*	         listmenu_RxQ;				/* ���е�ͨ������һ�����ͨ������                 */
    __u8                     listmenu_RxTID;            /*������Ϣ�߳�ID*/
    __u32                    mode;                      /*listmenuģʽ����*/
    __krnl_event_t*          run_sem;                   /*�������¼�������*/
    __krnl_event_t*          dist_sem;                   /*�������˶��ڼ��Ƿ���������Ϣ����*/
    __krnl_event_t*          load_sem;                  /*������ֹ֮ͣ��ļ������*/
    H_WIN                    handle;                    /*listmenu���ڴ��ڵľ��*/
    __pGUI_WIN_CB            user_cb;                   /*�û�����Ļص�����*/            
    __u16                    status;                    /*listmenu״̬��Ϣ*/
    __u32                    lastmsg;
	__u32                    lastkeyaction;             /*key action added by Derek 2011-1-5 14:27:17*/
    __pos_t                  last_pos;                  /*�ϴ������ߴ������ĵ����*/
    __u32                    last_touch;
    LISTMENU_LYRSTS_E        lyrsts;                    /*listmenuͼ��״̬��Ϣ*/
    H_LYR                    slave_lyr;
    __rect_t                 slyr_rect;                 /* slave layerԭʼλ�� */
    __rect_t                 slyr_crect;                /* slave ��ǰλ��*/

    /* �������� */
	int 		             step[STEP_ARRAY];           // touch ������������
	__krnl_event_t*          move_sem;                  /*������ֹ֮ͣ��ļ������*/
	int			             len;			            // ��������Ч���ݳ���
	int 		             index;			            // ͼ���ƶ�����	
	__pos_t		             win_start;		            // ��ʼ���Ӧ�Ĵ�������	
	__lstmgcuv_para_t        gcurv;		                // gcurv ����		
	
	lstmglide_state_t        gstate;		            // tbar �ƶ�״̬
	
    __u32                    request;
}__listmenu_data_t;

typedef __listmenu_data_t    *plistmenudata;

#define LSTM_STATUS(pCtrl)      (((plistmenudata)GUI_CtrlWinGetAddData(pCtrl))->status)

__u32  GUI_LstmOnMessage    (__listmenu_data_t *listmenu , __listmenu_runmsg_t * pMsg);
void   GUI_LstmCallBack     (__gui_msg_t*message);
__u32  GUI_LstmOnDraw       (__listmenu_data_t *listmenu);
__u32  GUI_LstmSetFoucs     (__listmenu_data_t *plistmenu);
__u32  GUI_LstmSetCurItemNo (__listmenu_data_t *plistmenu,__u32 no);
void   GUI_LstmStopProc     (__listmenu_data_t *plistmenu);
void   GUI_LstmPostRunMsg   (__listmenu_data_t *plistmenu,__u32 runid);
void   GUI_LstmStopOnLoad   (__listmenu_data_t *plistmenu);
void   GUI_LstmCurProc      (__listmenu_data_t *pData);
void   GUI_LstmStopOnLoad   (__listmenu_data_t *plistmenu);
#endif
