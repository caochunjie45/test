/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: sub_set.h
* By      	: Bayden.chen
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2011-05-06 Bayden.chen  create this file
**************************************************************************************************************
*/

#ifndef __MOVIE_SUB_SET_H__
#define  __MOVIE_SUB_SET_H__

#include "beetles_app.h"
#define MOVIE_SUB_MAX_ITEM_TEXT_NR 5
#define MOVIE_SUB_COLOR_NR  3

//�����ӳ�����item����
typedef enum
{
	movie_sub_set_item_state=0,
	movie_sub_set_item_color,	
	movie_sub_set_item_pos,
	movie_sub_set_item_stream,//�л���Ļ��
	movie_sub_set_item_num
}movie_sub_set_item_e;

//���������ڵ���Ϣ
typedef enum
{
    sub_set_scene_msg_timeout=0,
	sub_set_scene_msg_sub_state,
	sub_set_scene_msg_sub_color,	
	sub_set_scene_msg_sub_pos,		
	sub_set_scene_msg_sub_stream,		
	sub_set_scene_msg_num
}movie_sub_set_scene_msg_t;

//�����ӳ����ľ�̬�ؼ�����
typedef enum
{
	movie_sub_set_static_icon_state=0,
	movie_sub_set_static_icon_color,	
	movie_sub_set_static_icon_pos,
	movie_sub_set_static_icon_stream,
	movie_sub_set_static_icon_num
}movie_sub_set_static_icon_t;

//�����ڷ����ô��ڵ���Ϣ
typedef enum
{
	movie_sub_set_scene_null_msg = GUI_MSG_+1

}movie_sub_set_scene_recv_msg_t;


typedef struct  
{	
	// �������
	H_WIN hparent;
	__s32 scene_id;	
    __u32 sub_state;    
    __u32 sub_pos;
    __u32 sub_color;
    __u32 sub_stream;

	//�ڲ�����

}movie_sub_set_create_para_t;

void* movie_sub_set_scene_create(movie_sub_set_create_para_t* create_para);
__s32 movie_sub_set_scene_set_focus(void* handle);
__s32 movie_sub_set_scene_delete(void* handle);
H_WIN movie_sub_set_scene_get_hwnd(void* handle);
__s32 movie_sub_set_color2index(__u32 color);//������ɫ������ڱ�����±�
__s32 movie_sub_set_scene_set_lyr_top(void* handle);

#endif


