/*
**************************************************************************************************************************
*													        eMOD
*						            		the Easy Portable/Player Develop Kits
*									   				melix includes module 
*
*                             		 Copyright(C), 2011-2012, Softwinner Microelectronic Co., Ltd.
*									               All Rights Reserved
*
* File name		:	listview.h
* Author		: 	Derek
* Version 		: 	V1.00
* Date    		: 	2012-03-14
* Description	:
* Others 		:
* History 		:
* <Author>           <time>          <version>      <description>
* Derek Lai		    2012-03-14         1.0.0          Create File
*
**************************************************************************************************************************
*/
#ifndef __LISTVIEW_H__
#define __LISTVIEW_H__

typedef     void*   HLIST; 
typedef  __hdle  H_ITEM;

typedef enum GUI_LIST_MSG
{
	LIST_ITEM_ENTER = GUI_MSG_ + 100,			//������һ��Ŀ¼
	LIST_MOVE_STOP,					//����ֹͣ
}gui_list_msg_e;

/*typedef enum GUI_LIST_STATE
{
	LIST_STATE_MOVING,
	LIST_STATE_STOP
}gui_list_state_e;*/

typedef enum GUI_LIST_DRAM_MODE
{
    LIST_MODE_NORMAL    = 0,                    // ��ͨ��Ŀ����
    LIST_MODE_FOCUS,                            // ������Ŀ����
    LIST_MODE_INVALID,                          // ��Ч��Ŀ����
    LIST_MODE_VACANT,                           // �հ���Ŀ����
    LIST_MODE_DIFFUSE,                          // ��ɢ��Ŀ����
}gui_list_draw_mode_e;


typedef struct ITEM_DRAW_PARA
{
    __s32	                        item;
    gui_list_draw_mode_e            mode;
    //__u32                           aux;
    RECT                            invalidrect;	//�����������������list_buffer��ʼ��Ϊ��ʼ������������       
}item_draw_para_t;

/*typedef struct GUI_LIST_DRAW_PARA
{
    gui_list_draw_mode_e  	mode;                   // ���Ƶ�ģʽ
    __s32               	index;                  // ������Ŀ�ı��
    RECT                	rect;                   // ������Ŀ������
    void*               	attr;                   // Ӧ�õĴ����˽������
}gui_list_draw_para_t;*/

typedef struct GUI_LIST_SCENE
{
    __s32                   start_id;           // ҳ�����ʼλ����Ŀ    
    __s32                   focus_id;           // ҳ��Ľ�����Ŀ
}gui_list_scene_t;

typedef struct GUI_SCROLL_BAR
{
    RECT                    show_rect;          // ��ʾ����
    RECT                    scroll_rect;        // SCROLL����
    __u32                   head_height;        // HEADͼ��߶�
    __u32                   tail_height;        // TAILͼ��߶�
        
    void*                   back_bmp;           // BACKͼ��
    void*                   head_bmp;           // HEADͼ��
    void*                   body_bmp;           // BODYͼ��(��Ϊһ������)
    void*                   tail_bmp;           // TAILͼ��
}gui_scroll_bar_t;

/*
**********************************************************************************************************************
*                                               
*
* Description: ÿ����Ŀ�Ļ��ƻص�
*
* Arguments  : para: ���ƵĲ���
*
*
**********************************************************************************************************************
*/
typedef __s32 (* p_DrawItem)(item_draw_para_t * para);


typedef enum GUI_LIST_MODE
{
	GUI_LIST_ITEM_MODE = 0,				//itemģʽ
	GUI_LIST_SQUARE_MODE,				//�Ź���ģʽ
}gui_list_mode_e;

typedef struct GUI_LIST_CONFIG
{
    RECT                    list_rect;          // listview���ڴ�������
    __s32                   item_width;         // list��Ŀ�Ŀ�
    __s32                   item_height;        // list��Ŀ�ĸ�

	gui_list_mode_e			list_mode;			// listģʽ
	
    __s32                   start_id;           // ҳ�����ʼλ����Ŀ
    __s32                   focus_id;           // ҳ��Ľ�����Ŀ
    __u32                   bk_color;           // ����ɫ
    //__s32                   alpha_status;       // Alpha����
    
    __u32                   item_cnt;           // item ����Ŀ����
    void*                   list_attr;          // list�û�����
	p_DrawItem				item_cb;			// item�ص�
}gui_list_para_t;

typedef gui_list_para_t gui_list_config_t;

//set_item_cb(__s32 item, void cb, void parg);

#endif	/*ifndef __LISTVIEW_H__*/

