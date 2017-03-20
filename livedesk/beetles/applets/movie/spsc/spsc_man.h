/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: spsc_man.h
* By      	: Bayden.chen
* Func		: 
* Version	: v1.0
* ============================================================================================================
* v  create this file
**************************************************************************************************************
*/

#ifndef __MOVIE_SPSC_H__
#define  __MOVIE_SPSC_H__

#include "beetles_app.h"
#include "spsc_ui.h"

//spsc_scene���������ڵ���Ϣ
typedef enum
{
	spsc_scene_msg_timeout=0,
	spsc_scene_msg_brightset,
	spsc_scene_msg_volumeset,
	spsc_scene_msg_tvout,
	spsc_scene_msg_rr,
	spsc_scene_msg_num
}movie_spsc_scene_msg_t;

//�����ڷ����ô��ڵ���Ϣ
typedef enum
{
	movie_spsc_scene_update_fileinfo = GUI_MSG_+1

}movie_spsc_scene_recv_msg_t;


typedef struct  
{	
	H_WIN hparent;
	__s32 scene_id;
}movie_spsc_create_para_t;

void* movie_spsc_scene_create(movie_spsc_create_para_t* create_para);
__s32 movie_spsc_scene_delete(void* handle);
__s32 movie_spsc_scene_suspend(void* handle);
__s32 movie_spsc_scene_reseum(void* handle);
H_WIN movie_spsc_scene_get_hwnd(void* handle);

#endif
