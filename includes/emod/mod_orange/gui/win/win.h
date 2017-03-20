/*
*******************************************************************************************************************
*                                                         	Mod orange 
*                                         			the embedded graphic support module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name�� 	win.h
*
*Author�� 		William Wang
*
*Version :   	1.1.0
*
*Date:  		2009-8-1
*
*Description : 	Orange Win function and variable definition 
*
*Others : 		None at present.
*
* History :
*
* <Author>          <time>        <version>     <description>
*
* William Wang     2009-8-1         1.1.0        Create File
*
*******************************************************************************************************************
*/
#ifndef _GUI_WIN_H_
#define _GUI_WIN_H_

#define TYPE_HWND               0x01
    #define TYPE_LAYERWIN       0x11
    #define TYPE_MANWIN         0x12
    #define TYPE_FRAMEWIN       0x13
    #define TYPE_CONTROL        0x14
#define TYPE_HDC                0x08
    #define TYPE_SCRDC          0x81
    #define TYPE_GENDC          0x82
    #define TYPE_MEMDC          0x83

#define LOBYTE(w)               ((__u8)(w))
#define HIBYTE(w)               ((__u8)(((__u16)(w) >> 8) & 0xFF))
#define MAKEWORD(low, high)     ((__u16)(((__u8)(low)) | (((__u16)((__u8)(high))) << 8)))
#define LOWORD(l)               ((__u16)(__u32)(l))
#define HIWORD(l)               ((__u16)((((__u32)(l)) >> 16) & 0xFFFF))
#define LOSWORD(l)              ((__s16)(__u32)(l))
#define HISWORD(l)              ((__s16)((((__u32)(l)) >> 16) & 0xFFFF))
#define MAKELONG(low, high)     ((__u32)(((__u16)(low)) | (((__u32)((__u16)(high))) << 16)))

#define GUI_PARSEX(dwAddData2)  (LOSWORD(dwAddData2))
#define GUI_PARSEY(dwAddData2)  (HISWORD(dwAddData2))

typedef enum tag_GUI_LYRWIN_STA
{
    GUI_LYRWIN_STA_ON=0,                              /* on                                                           */
    GUI_LYRWIN_STA_OFF,                               /* off                                                          */
    GUI_LYRWIN_STA_SLEEP,                             /* off, layer released, but frame buffer not free               */
    GUI_LYRWIN_STA_SUSPEND,                           /* off, layer released, frame buffer free                       */
    GUI_LYRWIN_STA_ERR
}__gui_lyrwin_sta_t;

typedef enum __GUI_PAIN_MOD
{
    GUI_PAIN_MODE_UNFOCUS=0,
    GUI_PAIN_MODE_FOCUS,
    GUI_PAIN_MODE_FASTDRAW,
    GUI_PAIN_MODE_DRAW,
    GUI_PAIN_MODE_FASTFOCUS,
    GUI_PAIN_MODE_FASTUNFOCUS,
    GUI_PAIN_MODE_ACTIVED,
    GUI_PAIN_MODE_INACTIVED
}__gui_painmod_t;

/*reserver for future*/
typedef enum  WINSTATUS
{
    WS_NONE                         = 0,
    WS_VISIBLE                      = (1<<0),
    WS_CHILD                        = (1<<1),
    WS_DISABLED                     = (1<<2),
    WS_MEMDEV                       = (1<<3),
    WS_VSCROLL                      = (1<<4),
    WS_HSCROLL                      = (1<<5),
    WS_TABSTOP                      = (1<<6),
    WS_LYRWIN                       = (1<<7),
    WS_EX_NONE                      = (0),
    WS_EX_TOPMOST                   = (1<<16),
    WS_EX_TRANSPARENT               = (1<<17),
    WS_EX_NOPARENTNOTIFY            = (1<<18),
    WS_EX_CHILDPROC                 = (1<<19)       /**/
}__gui_winstatus_t;

/* ��׼�ؼ�ID�� */
typedef enum CONTROLIDS
{  
    IDC_STATIC                      = 0,
    IDOK                            = 1,
    IDCANCEL                        = 2,
    IDABORT                         = 3,
    IDRETRY                         = 4,
    IDIGNORE                        = 5,
    IDYES                           = 6,
    IDNO                            = 7,
    IDM_TIMEROUT                    = 8
}__gui_ctrlid_t;

typedef enum MSGBOX_TYPE
{
    MB_OK                           = 0,
    MB_OKCANCEL                     = 1,
    MB_YESNO                        = 2,
    MB_RETRYCANCEL                  = 3,
    MB_ABORTRETRYIGNORE             = 4,
    MB_YESNOCANCEL                  = 5
}__gui_msgboxtype_t;


#define GUI_SW_HIDE                 0x0000
#define GUI_SW_SHOW                 0x0010
#define GUI_SW_SHOWNORMAL           0x0100


typedef struct gui_rect
{
    __s32  left;
    __s32  top;
    __s32  right;
    __s32  bottom;
}__gui_rect_t;

typedef __gui_rect_t *pguirect;

typedef struct __show_info_move
{
    H_LYR                 hlayer;
    RECT                  region;
    GUI_FONT             *pFont;
    __epdk_charset_enm_e  encode_id;
    char                 *string;
    GUI_COLOR             bkColor;
    void                 *bmp;
    __pos_t               bmp_pos;
    __bool                alpha_en;
    GUI_COLOR             fontColor;
    __u32                 align;
}__show_info_move_t;

typedef __show_info_move_t *pshowinfo;

typedef struct __GUI_WIN_PAINPARA
{
    __gui_painmod_t                 mode;                          /* ����ģʽ���� foucs,unfoucs......                             */
    __s32                           aux;
    RECT                            invalidrect;
}__gui_win_painpara_t;

typedef struct __GUI_TIMER_PROCPARA
{
    H_WIN                           hWnd;
    __u16                           id;
    __u32                           dwAddData;
    __u32                           dwReserved;                      
}__gui_timer_procpara_t;

/*����֪ͨ�������壬����֪ͨ��������ʵ�ִ��ڵľ����������(������һ�׵�)��
����֪ͨ�����Ƿ�Ϊ����ȷ���Ƿ���Ҫ�򸸴��ڷ���֪ͨ��*/
typedef void  (* NOTIFPROC)         (__gui_notify_msg_t *msg);
typedef __s32 (* __pGUI_WIN_CB)     (__gui_msg_t * msg);
typedef __s32 (* __pGUI_TIMERPROC)  (__gui_timer_procpara_t *para);


typedef enum
{
    GUI_LYRWIN_NORMAL = 0,
    GUI_LYRWIN_SPRITE
}__lyrwin_type_e;

typedef struct __CURSOR_ATTR
{
    void *                pat_mem;
    void *                palette;
}__gui_cursor_t;

/*ͼ��Ĵ�����Ϣ�ṹ*/
typedef struct __LAYERWIN_ATTR
{
    char                            *name;
    
    __disp_layer_para_t *           lyrpara;                /* used to request a hw layer                                   */  
    __gui_lyrwin_sta_t              initsta;                /* init status of lion layer                                    */

    __u8                            type;
}__layerwincreate_para_t;

typedef __layerwincreate_para_t     *playerwincreate;

/*framewin���ڵĴ�����Ϣ�ṹ*/
typedef struct _FRAMECREATE
{
    char                            *name;
    
    __u32                           dwStyle;                /*���ڷ��*/
    __u32                           dwExStyle;              /*������չ���*/
    const char*                     spCaption;              /*���ڵı���*/
    H_WIN                           hOwner;                 /*framewin��ӵ���߾��*/
    H_WIN                           hHosting;               /*framewin�������ھ��*/
    H_WIN                           hLayer;                 /*framewin��ͼ�㴰��*/
    __pGUI_WIN_CB                   FrameWinProc;           /*framewin����Ϣ�������*/
    __u16                           id;                     /*framewin��ID*/
    RECT                            rect;                   /*framewin��ͼ�㴰���е�λ��*/
    __color_t                       BkColor;                /*framewin�ı���ɫ*/
    void                            *attr;                  /*framewin��˽��������Ϣ*/

    /*�������������Բ���*/
    __u32                           dwReserved;
}__gui_framewincreate_para_t;

typedef __gui_framewincreate_para_t *pframewincreate;

/*�����ڵĴ�����Ϣ�ṹ*/
typedef struct __MAINCREATE
{
    char                            *name;                  /*����������*/

    __u16                           id;                     /*�����ڵĴ���ID*/
    H_WIN                           hParent;                /*�����ڵĸ����ھ��*/
    H_WIN                           hHosting;               /*�����ڵ�ӵ���ߴ���*/
    __pGUI_WIN_CB                   ManWindowProc;         /*�����ڵ���Ϣ�������*/
    void*                           attr;                   /*�����ڵ�˽������*/
}__gui_manwincreate_para_t;

typedef __gui_manwincreate_para_t *pmanwincreate;



/*�ؼ����ڵĴ�����Ϣ�ṹ*/
typedef struct __CONTROLCREATE
{
    char                            *name;                  /*�ؼ���������*/
    
    char                            * spClassName;          /*�ؼ����ڵ�������*/
    char                            * spCaption;            /*�ؼ����ڵı�������*/        
    __u32                           dwStyle;                /*�ؼ����ڵķ��*/
    __u32                           dwExStyle;              /*�ؼ����ڵ���չ���*/
    __u16                           id;                     /*�ؼ����ڵı�ʶID*/
    RECT                            ctl_rect;               /*�ؼ������ڸ������е�λ��*/
    H_WIN                           hParent;                /*�ؼ����ڵĸ�����*/
    H_LYR                           hLayer;                 /*�ؼ���ͼ������ֻ�жԴ��ڷ��
                                                            ΪWS_LYRWINʱ��Ч*/
    void                            *attr;                  /*�ؼ����ڵ�˽����Ϣ*/
}__gui_ctlwincreate_para_t;

typedef __gui_ctlwincreate_para_t   *pctlwincreate;
/**
 * Structure which defines a control.
 */
typedef struct _CTRLDATA
{
    char*                           class_name;             /** �ؼ������� */
    char*                           name;                   /*  �ؼ�����*/
    __u32                           dwStyle;                /** �ؼ��ķ�� */
    RECT                            ctl_rect;               /** �ؼ���framewin�еľ������� */
    __u16                           id;                     /** �ؼ�ID **/
    char*                           caption;                /** �ؼ��ı��� */
    __u32                           dwAddData;              /** �ؼ��ĸ������� */
    __u32                           dwExStyle;              /** �ؼ�����չ��� */
} __gui_ctrldata_t;

typedef __gui_ctrldata_t* pctrldata;

typedef struct _DLGTEMPLATE
{
    __u32                           dwStyle;                /** �Ի����� */
    __u32                           dwExStyle;              /** �Ի�����չ��� */
    RECT                            dlg_rect;               /** �Ի�����framebuffer�еľ�������*/
    const char*                     caption;                /** �Ի������ */
    __u32                           controlnr;              /** �Ի���ؼ����� */
    pctrldata                       controls;               /** �Ի���Ŀؼ����� */
    __u32                           dwAddData;              /** �Ի���Ķ������� */
} __gui_dlgtemplate_t;

typedef __gui_dlgtemplate_t* pdlgtemplate;

typedef struct __DIALOGCREATE
{
    char                            *name;                  /** �Ի������� */
    __gui_dlgtemplate_t             *pDlgTemplate;          /** �Ի��򴴽�ģ�� */
    H_WIN                           hOwner;                 /** �Ի���Ի���Ĵ������ھ�� */
    H_WIN                           hHosting;               /* �Ի��������ھ��*/
    H_WIN                           hLayer;                 /** �Ի����ͼ���� */
    __u16                           id;                     /*�ؼ����ڵı�ʶID*/
    __pGUI_WIN_CB                   DialogProc;             /** �Ի����Ĭ�ϴ����� */
    __u32                           dwAddData;              /** �Ի��򴴽��������� */
}__gui_dialogcreate_para_t;

typedef __gui_dialogcreate_para_t *pdialogcreate;

typedef struct __GUI_BlitInfo
{
    void*           src_addr;
    __pos_t         srcstart;
    __pos_t         srcend;
    __u32           src_width;
    __u32           src_height;
    __u8            bpp;
    void*           dst_addr;
    __pos_t         dststart;
    __pos_t         dstend;
    __u32           dst_width;
    __u32           dst_height;
}__gui_blitinfo_t;

/*ͨ�ô��ڴ�����ɾ������*/
extern H_WIN     GUI_FrmWinCreate           (pframewincreate create_info);              /*����framewin*/
extern __s32     GUI_FrmWinDelete           (H_WIN  hframewin);                         /*ɾ��framewin*/
extern H_WIN     GUI_ManWinCreate           (pmanwincreate create_info);                /*���������ڴ���*/
extern __s32     GUI_ManWinDelete           (H_WIN  hmanwin);                           /*ɾ��������*/
extern H_WIN     GUI_LyrWinCreate    	    (playerwincreate create_info);              /*����ͼ�㴰��*/
extern __s32     GUI_LyrWinDelete    	    (H_WIN  hlayer);                            /*ɾ��ͼ�㴰��*/
extern H_WIN     GUI_CtrlWinCreate          (__gui_ctlwincreate_para_t *create_info);   /*�����ؼ�����*/
extern __s32     GUI_CtrlWinDelete          (H_WIN  hWnd);                              /*ɾ���ؼ�����*/
extern void      GUI_WinThreadCleanup       (H_WIN hManWnd);                            /*��������Ϣѭ����ص����ݽṹ�����*/

/*�����轹������*/
extern H_WIN     GUI_SetActiveManWin        (H_WIN  hWnd);                              /*����active �����ڣ����������ⲿ��Ϣ�������ڣ�����ֵΪԭ����active������*/
extern H_WIN     GUI_GetActiveManWin        (void);                                     /*��ȡactive �����ھ��*/
extern H_WIN     GUI_WinSetFocusChild       (H_WIN hWnd);                               /*����focus������,��������Ϊ�������͵Ĵ���(�������֮��ô�����ϵͳ�Ľ����Ӵ���)*/
extern H_WIN     GUI_WinGetFocusChild       (H_WIN hWnd);                               /*��ȡĳ�����ڵĽ����Ӵ���*/
extern H_WIN     GUI_WinSetActiveChild      (H_WIN hWnd);                               /*���ô���Ϊĳһ���ڵĽ����Ӵ���(�������֮��ô��ڲ�һ����ϵͳ�Ľ����Ӵ���) */


/*�������ͻ�ȡ�����ڹ�ϵ��ȡ���жϺ���*/
extern __s32     GUI_WinGetType             (H_WIN hWnd);                               /*��ȡĳ�����ڵĴ�������*/
extern H_WIN     GUI_WinGetLyrWin           (H_WIN hWnd);                               /*��ȡĳ�����ڵ�ͼ����*/
extern H_WIN     GUI_WinGetMainManWin       (H_WIN hWnd);                               /*��ȡ�������ڵĴ��ھ��*/
extern H_WIN     GUI_WinGetManWin           (H_WIN hWnd);                               /*��ȡ���ڵ������ھ��*/
extern H_WIN     GUI_WinGetFrmWin           (H_WIN hWnd);                               /*��ȡĳ�����ڵ�framewin���*/
extern H_WIN     GUI_WinGetParent           (H_WIN hWnd);                               /*�õ�ĳ�����ڵĸ�����*/
extern H_WIN     GUI_WinGetFirstChild       (H_WIN hWnd);                               /*�õ�ĳ�����ڵĵ�һ���Ӵ���*/
extern H_WIN     GUI_WinGetNextBro          (H_WIN hWnd);                               /*�õ�ĳ�����ڵ��ֵܴ���*/
extern __bool    GUI_WinIsAncestor          (H_WIN hWnd,H_WIN hChild);                  /*�ж�ĳ�������Ƿ�����һ�����ڵ�����*/
extern __bool    GUI_WinIsChild             (H_WIN HWnd,H_WIN hParent);                 /*�ж�ĳ�������Ƿ�������һ�����ڵĶ��Ӵ���*/
extern H_WIN     GUI_WinGetRootWin          (void);                                     /*�õ������ڵĴ��ھ��*/
extern H_WIN     GUI_WinGetNextHostedWin    (H_WIN  hWnd);
extern H_WIN     GUI_WinGetFirstHostedWin   (H_WIN  hWnd);
extern H_WIN     GUI_WinGetOwnerWin         (H_WIN  hWnd);
extern H_WIN     GUI_WinGetDlgItem          (H_WIN hDlg, __s32 nIDDlgItem);             /*��ȡһ��frmwin�Ĵ��ھ�������*/
extern __u32     GUI_WinGetItemId           (H_WIN hItem);                              /*���ݴ��ڵľ����ȡ���ڵ�ID*/
extern H_WIN     GUI_WinGetPrimitiveWin     (H_WIN hWnd);
extern H_WIN     GUI_WinGetHandFromId       (H_WIN hParent, __s32 nID);

/*��ȡ�����ô��ڵ����Ժ���*/
extern __s32     GUI_WinGetName             (H_WIN hWnd, char * name);                  /*�õ�ĳ�����ڵĴ�������*/
extern H_WIN     GUI_WinGetHandFromName     (char * name);                              /*���ݴ���������ȡ���ھ��*/                                  
extern void *    GUI_WinGetAttr             (H_WIN hWnd);                               /*�õ�ĳ�����ڵ�˽������*/
extern __s32     GUI_WinSetAttr             (H_WIN hWnd,void *attr);                    /*����ĳ�����ڵ�˽������*/
extern __u32     GUI_WinGetAddData          (H_WIN hWnd);                               /*�õ�ĳ�����ڵ�˽�п�����Ϣ����*/
extern __s32     GUI_WinSetAddData          (H_WIN hWnd,__u32 dwAddData);               /*����ĳ�����ڵ�˽�п�����Ϣ����*/
extern __u32     GUI_WinGetStyle            (H_WIN hWnd);                               /*��ȡ���ڵķ��*/
extern __s32     GUI_WinSetStyle            (H_WIN hWnd,__u32 style);                   /*���ô��ڷ��*/
extern __u32     GUI_WinGetExStyle          (H_WIN hWnd);                               /*��ȡ���ڵ���չ���*/
extern __s32     GUI_WinSetExStyle          (H_WIN hWnd,__u32 style);                   /*���ô��ڵ���չ���*/
extern __u32     GUI_CtrlWinGetAddData      (H_WIN hWnd);                               /*��ȡcontrol�ĸ�������*/
extern __s32     GUI_CtrlWinSetAddData      (H_WIN hWnd,__u32 dwAddData);               /*����control�ĸ�������*/
extern __bool    GUI_WinIsVisible           (H_WIN hWnd);                               /*�жϴ����Ƿ�ɼ�*/
extern __s32     GUI_CtrlWinGetClassName    (H_WIN hWnd,char *name);                    /*��ȡ�ؼ�������*/
extern H_WIN     GUI_DlgGetFirstButton      (H_WIN hDlg);

/*��ȡ�����ô��ڵ�ִ�к���*/
extern NOTIFPROC GUI_WinSetNotifyCallback   (H_WIN hwnd, NOTIFPROC notif_proc);         /*���ô��ڵ�NotifyCallback����ָ��*/
extern NOTIFPROC GUI_WinGetNotifyCallback   (H_WIN hwnd);                               /*��ȡ���ڵ�NotifyCallback����ָ��*/
extern __pGUI_WIN_CB GUI_WinSetCallback     (H_WIN hWnd, __pGUI_WIN_CB cb);             /*���ô��ڵ�callback����ָ��*/
extern __pGUI_WIN_CB GUI_WinGetCallback     (H_WIN hWnd);                               /*��ȡ���ڵ�callback����ָ��*/


/*���ڲ���������*/
extern void      GUI_NotifyParent           (__gui_notify_msg_t *msg);                  /*֪ͨ�����ڵ�ִ�к���*/
extern void      GUI_WinUpdate              (H_WIN hWnd, __bool fErase);                /*���´�������*/
extern __bool    GUI_InvalidateRect         (H_WIN hWnd, const RECT* prc, __bool bEraseBkgnd);
extern H_WIN     GUI_WinSetCaptureWin       (H_WIN hWnd);                               /*����ĳ������Ϊcapture����*/
extern H_WIN     GUI_WinGetCaptureWin       (void);                                     /*��ȡ��ǰ��capture����*/
extern void      GUI_WinReleaseCapture      (void);                                     /*�ͷŵ�ǰ��capture����*/
extern H_WIN     GUI_WinSetTouchFocus       (H_WIN hWnd);                               /*����ĳ������Ϊcapture����*/
extern H_WIN     GUI_WinGetTouchFocus       (void);                                     /*��ȡ��ǰ��capture����*/
extern void      GUI_WinReleaseTouchFocus   (void);                                     /*�ͷŵ�ǰ��capture����*/
extern __s32     GUI_ManWinDefaultProc      (__gui_msg_t *msg);
extern __s32     GUI_CtrlWinDefaultProc     (__gui_msg_t *msg);
extern __s32     GUI_FrmWinDefaultProc      (__gui_msg_t *msg);
extern H_WIN     GUI_GetTouchFocusWin       (__s32 x,__s32 y);
extern __bool    GUI_SceenPointInWin        (H_WIN hWnd,__s32 x,__s32 y);

/*�������������ת��������*/
extern __s32     GUI_WinGetClient           (H_WIN hWnd, RECT *rect);                   /*��ȡ���ڿͻ�����������*/
extern __s32     GUI_WinGetWinRECT          (H_WIN hWnd,RECT *Win_Rect);                /*��ȡ���ڵĴ��ھ�������*/
extern __s32     GUI_WinSetWinRECT          (H_WIN hWnd,RECT *Win_Rect);                /*���ô��ڵĴ��ھ������򣬲����������ƶ�����
                                                                                        ��Ҫ����һ�θ��´���ʱ�Ż��ƶ�*/
extern __s32     GUI_WinGetClientRECT       (H_WIN hWnd,RECT *Client_Rect);
extern __s32     GUI_WinSetClientRECT       (H_WIN hWnd,RECT *Win_Rect);                /*���ô��ڵĴ��ڿͻ����򣬲����������ƶ�����
                                                                                        ��Ҫ����һ�θ��´���ʱ�Ż��ƶ�*/
extern void      GUI_WinGetClientFBRect     (H_WIN hWnd,RECT *fb_rect);
extern void      GUI_WinGetWinFBRect        (H_WIN hWnd,RECT *fb_rect);
extern void      GUI_ClientToScreen         (H_WIN hWnd,__pos_t *client_pos,__pos_t *sc_pos);
extern void      GUI_ScreenToClient         (H_WIN hWnd,__pos_t *sc_pos,__pos_t *client_pos);
extern void      GUI_WindowToScreen         (H_WIN hWnd,__pos_t *win_pos,__pos_t *sc_pos);
extern void      GUI_ScreenToWindow         (H_WIN hWnd,__pos_t *sc_pos,__pos_t *win_pos);
extern void      GUI_FBToScreen             (H_WIN hWnd,__pos_t *fb_pos,__pos_t *sc_pos);
extern void      GUI_ScreenToFB             (H_WIN hWnd,__pos_t *sc_pos,__pos_t *fb_pos);

/*�Ի�����صĺ����ӿ�ʵ��*/
extern __s32     GUI_ModuleDialogCreate     (__gui_dialogcreate_para_t *create_info);   /*����һ��ģʽ�Ի���*/
extern __bool    GUI_EndDialog              (H_WIN hDlg, __s32 endCode);                /*�˳�һ��ģʽ�Ի���*/
extern H_WIN     GUI_GetDlgItem             (H_WIN hDlg, __s32 nIDDlgItem);             /*��ȡDialog�е�ĳ��Item�ľ��*/
extern __s32     GUI_DialogDefaultProc      (__gui_msg_t    *msg);                      /*Ĭ�ϵĶԻ�������*/
extern H_WIN     GUI_ModulelessDialogCreate (__gui_dialogcreate_para_t *create_info);   /*����һ����ģʽ�Ի���*/
extern __bool    GUI_ModulelessDialogDelete (H_WIN hManWin);                           /*ɾ��һ����ģʽ�Ի���*/                              
extern H_WIN     GUI_GetNextDlgTabItem      (H_WIN hDlg, H_WIN hCtl, __bool bPrevious);

/*ͼ�㴰�ڵ���ز���������*/
extern __s32     GUI_LyrWinSetSrcWindow	    (H_WIN lyrwin, const RECT * rect);
extern __s32     GUI_LyrWinSetScnWindow 	(H_WIN lyrwin, const RECT * rect);
extern __s32     GUI_LyrWinGetSrcWindow 	(H_WIN lyrwin, RECT * rect);
extern __s32     GUI_LyrWinGetScnWindow 	(H_WIN lyrwin, RECT * rect);
extern __s32     GUI_LyrWinGetFB		 	(H_WIN lyrwin, FB * fb);
extern __s32     GUI_LyrWinSetFB		 	(H_WIN lyrwin, const FB * fb);
extern __s32     GUI_LyrWinSetSta		 	(H_WIN lyrwin, __gui_lyrwin_sta_t status);  /*����ĳһ��ͼ���״̬*/
extern __gui_lyrwin_sta_t GUI_LyrWinGetSta  (H_WIN lyrwin);                             /*��ȡĳһ��ͼ���״̬*/
extern __s32     GUI_LyrWinSel 		 		(H_WIN lyrwin);                             /*ѡ��ĳһ��ͼ��Ĳ���*/
extern __s32     GUI_LyrWinCacheOn		 	(void);                                     /*����ʾ��cache����*/
extern __s32     GUI_LyrWinCacheOff     	(void);                                     /*�ر���ʾ��cache����*/
extern __s32     GUI_LyrWinFlushFB      	(H_WIN lyrwin);                             /*ˢͼ���framgbuffercache*/
extern __s32     GUI_LyrWinSetAlpha     	(H_WIN lyrwin, __u8 alpha);                 /*����ͼ���alpha*/
extern __s32     GUI_LyrWinAlphaOn      	(H_WIN lyrwin);                             /*��ͼ���alpha*/
extern __s32     GUI_LyrWinAlphaOff	 		(H_WIN lyrwin);                             /*�ر�ͼ���alpha*/
extern __s32     GUI_LyrWinCKOn		 		(H_WIN lyrwin);                             /*��ͼ���colorkey*/
extern __s32     GUI_LyrWinCKOff		 	(H_WIN lyrwin);                             /*�ر�ͼ���colorkey*/
extern __s32     GUI_LyrWinSetBottom    	(H_WIN lyrwin);                             /*ͼ���õ�*/
extern __s32     GUI_LyrWinSetTop		 	(H_WIN lyrwin);                             /*ͼ���ö�*/


/*timer��صĽӿڶ���*/
extern __bool    GUI_SetTimer               (H_WIN hWnd, __u16 id, 
                                            __u32  speed, __pGUI_TIMERPROC timer_proc);/*����Timer*/
extern __s32     GUI_KillTimer              (H_WIN hWnd, __u16 id);                    /*ɾ��Timer*/
extern __bool    GUI_ResetTimer             (H_WIN hWnd, __u16 id, 
                                            __u32 speed, __pGUI_TIMERPROC timer_proc);  /*��������Timer*/
extern __bool    GUI_IsTimerInstalled       (H_WIN hWnd, __u16 id);                     /*�ж�ĳһ��Timer�Ƿ��Ѿ�����*/
extern __bool    GUI_HaveFreeTimer          (void);                                     /*�ж��Ƿ��п��е�Timer����*/
extern __u32     GUI_GetTickCount           (void);                                     /*�õ���ǰtick count����*/
extern __u32     GUI_GetTimerExpired        (H_WIN hWnd, __u16 id);

extern __s32     GUI_WinInit                (void);
extern __s32     GUI_WinExit                (void);
extern __s32     GUI_CharSetToEncode        ( __s32 charset_enm );
extern __hdle    GUI_LongStringCreate       (__show_info_move_t *str_move);
extern __s32     GUI_LongStringDelete       (__hdle hString);
extern __s32     GUI_LongStringStart        (__hdle hString);
extern __s32     GUI_LongStringStop         (__hdle hString);
extern __s32  	 GUI_LongStringReset		(__hdle hString,__show_info_move_t *str_move);

extern __s32     GUI_CursorCreate           (__gui_cursor_t *create_info);
extern __s32     GUI_CursorDelete           (void);
extern __s32     GUI_CursorShow             (void);
extern __s32     GUI_CursorHide             (void);
extern __s32     GUI_CursorSetPos           (__pos_t *pos);
extern __s32     GUI_CursorGetPos           (__pos_t *pos);
extern __s32     GUI_CursorSetPara          (__gui_cursor_t *para);

extern int       GUI_GetScnDir              (void);
extern int       GUI_SetScnDir              (__u8  scn_dir);
extern __s32     GUI_GetScreenSize          (__s32 *p_width, __s32 *p_height);
extern __s32     SysRectToGuiRect           (RECT *SysRect,__gui_rect_t *GuiRect);
#endif /* #ifndef _GUI_WIN_H_ */
