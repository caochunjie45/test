/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: play_sta.h
* By      	: Bayden.chen
* Func		:
* Version	: v1.0
* ============================================================================================================
* 2011-05-06  create this file
**************************************************************************************************************
*/

#ifndef __MOVIE_PLAYSTA_H__
#define  __MOVIE_PLAYSTA_H__

#include "beetles_app.h"
#include "play_sta_ui.h"

//���������ڵ���Ϣ
typedef enum
{
	movie_playsta_scene_msg_timeout=0,
	movie_playsta_scene_msg_clicked,
	movie_playsta_scene_msg_num
}movie_playsta_scene_msg_t;

typedef enum
{
	movie_playsta_play=0,
	movie_playsta_pause,
	movie_playsta_num
}movie_playsta_t;

typedef struct  
{	
	//�������
	H_WIN hparent;
	__s32 scene_id;
	movie_playsta_t playsta;

	//�ڲ�����

}movie_playsta_create_para_t;

void* movie_playsta_scene_create(movie_playsta_create_para_t* create_para);
__s32 movie_playsta_scene_delete(void* handle);
__s32 movie_playsta_scene_set_focus(void* handle);
__s32 movie_playsta_scene_get_hwnd(void* handle);
__s32 movie_playsta_scene_get_lyr_scn(RECT* rect);

#endif

