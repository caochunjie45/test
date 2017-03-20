
/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
* File    	: 	music.h
* By      	: 	Libaiao
* Func	: 
* Version	: 	v1.0
* ============================================================================================================
* 2010-5-3 15:24:52  Libaiao  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __MUSIC_H__
#define	__MUSIC_H__

#include "beetles_app.h"


//
//MUSICPLAYER_NORMAL: 		�����������ֲ�����
//MUSICPLAYER_BACKGROUND: 	���ֲ�������Ϊ��̨����
/*
typedef enum musicplayer_mode_s
{
	MUSICPLAYER_NORMAL = 0,
	MUSICPLAYER_BACKGROUND = 1,	
}musicplayer_mode_e;
*/
typedef struct music_ctrl_s
{
	H_LYR music_layer;				//���ֽ���ͼ��
	H_LYR background_layer;						//background layer 8bpp(ע�ⱳ��ͼ��Ҫ����Ϊ8bpp)
	H_LYR list_layer;					//layer for listbar, 32bpp(32bpp,��Ϊlistbar����ͼΪ32bpp)
	H_LYR spectrum_layer;
  	H_WIN h_framewin;					//
  	
  	H_WIN h_spectrum_win;				//for spectrum window
  	
	__s32 root_type;					//USB Device, or  SDCard
	//explorer_view_mode_e view_mode;		//��ʾģʽ
	rat_media_type_t media_type;
	GUI_FONT *font;	
	musicplayer_mode_e   mode;	
    char root[4];                         //��¼��Ŀ¼
    __s32  explorer_play_music;				//�ļ�����������
    char*  music_file_name;    
}music_ctrl_t;




extern __s32 app_music_create(root_para_t  *para);
extern __s32 app_music_delete(void  *para);
//extern __s32 app_music_restore(H_WIN music_man_win, __s32 root_type);
extern __s32 app_music_restore(H_WIN music_man_win, root_para_t* root_para);

#endif
