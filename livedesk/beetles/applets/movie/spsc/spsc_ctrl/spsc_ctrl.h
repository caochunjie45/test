/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: spsc_ctrl.h
* By      	: Bayden.chen
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2011-05-06 Bayden.chen  create this file
**************************************************************************************************************
*/

#ifndef __MOVIE_SPSC_CTRL_H__
#define  __MOVIE_SPSC_CTRL_H__

#include "beetles_app.h"

#if 0
//����spsc ctrl�ӳ�����ͼ������
typedef enum
{
	movie_spsc_ctrl_icon_prev=0,
	movie_spsc_ctrl_icon_next,	
	movie_spsc_ctrl_icon_rr,
	movie_spsc_ctrl_icon_ff,
	movie_spsc_ctrl_icon_playpause,
	movie_spsc_ctrl_icon_channelset,
	movie_spsc_ctrl_icon_trackset,
	movie_spsc_ctrl_icon_timeset,
	movie_spsc_ctrl_icon_playmode,	
	movie_spsc_ctrl_icon_brightset,
	movie_spsc_ctrl_icon_screenset,	
	movie_spsc_ctrl_icon_tvout,	
	movie_spsc_ctrl_icon_num
}movie_spsc_ctrl_icon_t;

//spsc ctrl scene���������ڵ���Ϣ
typedef enum
{
	spsc_ctrl_scene_msg_prev = 0,
	spsc_ctrl_scene_msg_next,	
	spsc_ctrl_scene_msg_rr,
	spsc_ctrl_scene_msg_ff,
	spsc_ctrl_scene_msg_playpause,
	spsc_ctrl_scene_msg_channelset,
	spsc_ctrl_scene_msg_trackset,
	spsc_ctrl_scene_msg_timeset,
	spsc_ctrl_scene_msg_playmode,	
	spsc_ctrl_scene_msg_brightset,
	spsc_ctrl_scene_msg_screenset,		
	spsc_ctrl_scene_msg_tvout,		
	spsc_ctrl_scene_msg_num
}movie_spsc_ctrl_scene_msg_t;

//����spsc ctrl�ӳ����ľ�̬�ؼ�����
typedef enum
{
	movie_spsc_ctrl_static_icon_playmode=0,
	movie_spsc_ctrl_static_icon_bright,	
	movie_spsc_ctrl_static_icon_bright_text,
	movie_spsc_ctrl_static_icon_screen_mode,
	movie_spsc_ctrl_static_icon_file_info,
	movie_spsc_ctrl_static_icon_video_info,
	movie_spsc_ctrl_static_icon_audio_info,
	movie_spsc_ctrl_static_icon_filename,
	movie_spsc_ctrl_static_icon_btn_info,
	movie_spsc_ctrl_static_icon_num
}movie_spsc_ctrl_static_icon_t;

#else
//����spsc ctrl�ӳ�����ͼ������
typedef enum
{

	movie_spsc_ctrl_icon_channelset=0,
	movie_spsc_ctrl_icon_trackset,
	movie_spsc_ctrl_icon_timeset,
	movie_spsc_ctrl_icon_playmode,	
	movie_spsc_ctrl_icon_brightset,
	movie_spsc_ctrl_icon_screenset,	
#if BEETLES_TVOUT_ENABLE	
	movie_spsc_ctrl_icon_tvout,
#endif	
	movie_spsc_ctrl_icon_num,

	movie_spsc_ctrl_icon_prev,
	movie_spsc_ctrl_icon_next,	
	movie_spsc_ctrl_icon_rr,
	movie_spsc_ctrl_icon_ff,
	movie_spsc_ctrl_icon_playpause,	
}movie_spsc_ctrl_icon_t;
//spsc ctrl scene���������ڵ���Ϣ
typedef enum
{

	spsc_ctrl_scene_msg_channelset = 0,
	spsc_ctrl_scene_msg_trackset,
	spsc_ctrl_scene_msg_timeset,
	spsc_ctrl_scene_msg_playmode,	
	spsc_ctrl_scene_msg_brightset,
	spsc_ctrl_scene_msg_screenset,		
#if BEETLES_TVOUT_ENABLE	
	spsc_ctrl_scene_msg_tvout,
#endif
	spsc_ctrl_scene_msg_num,

	spsc_ctrl_scene_msg_prev,
	spsc_ctrl_scene_msg_next,	
	spsc_ctrl_scene_msg_rr,
	spsc_ctrl_scene_msg_ff,
	spsc_ctrl_scene_msg_playpause,
}movie_spsc_ctrl_scene_msg_t;

//����spsc ctrl�ӳ����ľ�̬�ؼ�����
typedef enum
{
	movie_spsc_ctrl_static_icon_playmode=0,
	movie_spsc_ctrl_static_icon_bright,	
	movie_spsc_ctrl_static_icon_bright_text,
	movie_spsc_ctrl_static_icon_screen_mode,
	movie_spsc_ctrl_static_icon_file_info,
	movie_spsc_ctrl_static_icon_video_info,
	movie_spsc_ctrl_static_icon_audio_info,
	movie_spsc_ctrl_static_icon_filename,
	movie_spsc_ctrl_static_icon_btn_info,
	movie_spsc_ctrl_static_icon_num
}movie_spsc_ctrl_static_icon_t;
#endif

//�����ڷ����ô��ڵ���Ϣ
typedef enum
{
	movie_spsc_ctrl_scene_update_fileinfo = GUI_MSG_+1,
    movie_spsc_ctrl_scene_draw_difuse

}movie_spsc_ctrl_scene_recv_msg_t;


typedef struct  
{	
	// �������
	H_WIN hparent;
	__s32 scene_id;
	H_LYR hlyr;

	//�ڲ�����

}movie_spsc_ctrl_create_para_t;

void* movie_spsc_ctrl_scene_create(movie_spsc_ctrl_create_para_t* create_para);
__s32 movie_spsc_ctrl_scene_set_focus(void* handle);
__s32 movie_spsc_ctrl_scene_delete(void* handle);
H_WIN movie_spsc_ctrl_scene_get_hwnd(void* handle);

#endif

