#ifndef __MUSIC_LIST_H__
#define __MUSIC_LIST_H__

#include "beetles_app.h"

typedef struct music_list_para_s
{
	H_WIN 	framewin;
	H_LYR 	layer;	
	__bool	visiable;												//�Ƿ񴴽�����
	GUI_FONT *font;													//����
	
	__s32   root_type;
	
	rat_media_type_t	media_type;									//ý������
	
    __s32  explorer_play_music;										//�ļ�����������
    char*  music_file_name;    										//�ļ������������ļ���
}music_list_para_t;

typedef struct music_list_ctrl_s
{
	H_WIN 	framewin;
	H_LYR 	layer;	
	__bool	visiable;												//�Ƿ񴴽�����

	__s32   root_type;
	__u32   last_start_id;
	__u32   last_focused_id;
	char 	*last_filename;											//���һ������ļ���
    //__s32   explorer_play_music;									//�ļ�����������
    //char*   music_file_name;    									//�ļ������������ļ���    
	
	char 	search_path[RAT_MAX_FULL_PATH_LEN];				//����·��
	rat_media_type_t	media_type;									//ý������
	
	
	RECT 		area;												//������ʾ����
	GUI_FONT 	*font;
	
	H_LBAR 		listbar_handle;										//listbar ���
	HRAT   		rat_handle;											//�������
	__hdle      long_str_handle;									//���ַ����������
	__u32   	total;												//��Ŀ����
	__u32    	play_index;											//��ǰ�����ļ�������
	__listbar_scene_t	listbar_play_scene;							//��ǰ�����б����ʼ�ź�focusid��
	
	__s32   (*OnDraw)(void *list_ctrl);         					//ģ�����
	__s32   (*OnNextKey)(void *list_ctrl);         					//ģ�����
	__s32   (*OnPreviousKey)(void *list_ctrl);     					//ģ�����
	__s32   (*OnNextPageKey)(void *list_ctrl);     					//ģ�����
	__s32   (*OnPreviousPageKey)(void *list_ctrl); 					//ģ�����
	__s32   (*OnEnterKey)(void *list_ctrl); 						//ģ�����		
}music_list_ctrl_t;


//��һ�������ļ��ĸ���
#define		C_SEARCH_FILE_CNT_AT_FIRST			0								

extern void clear_music_list_area(void);
extern music_list_ctrl_t* new_music_list(music_list_para_t *list_para);
extern __s32 delete_music_list(music_list_ctrl_t *list_ctrl);
extern __s32 music_listbar_restore(music_list_ctrl_t *list_ctrl);

extern __s32 show_music_list(music_list_ctrl_t *list_ctrl);
extern __s32 music_list_set_focus_item(music_list_ctrl_t *list_ctrl, __u32 item_num);

extern __s32 MusicGetListItemFileFullPath(HRAT rat_handle, __s32 ItemIndex, char *FilePath);

//filename Ϊ�ⲿ������ڴ�����
extern __s32 get_music_list_focus_item_full_filename(music_list_ctrl_t *list_ctrl, char *filename);
extern __s32 music_list_long_string_stop_roll(music_list_ctrl_t *this);

extern __s32 music_list_long_string_stop(music_list_ctrl_t *this);


__s32 music_list_set_item_scene(music_list_ctrl_t *this, __listbar_scene_t * scene);
__s32 music_list_get_item_scene(music_list_ctrl_t *this, __listbar_scene_t * scene);


#endif



