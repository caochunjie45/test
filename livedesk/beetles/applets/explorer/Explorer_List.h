

#ifndef __EXPLORER_LIST_H__
#define __EXPLORER_LIST_H__

#include "FileList.h"

//typedef void* H_KAK;

typedef enum exp_icon_status_s
{
	UNFOCUSED = 0,
	FOCUSED = 1,
	PRESSED = 2,
}exp_icon_status_e;


//����rat �ļ��������ݽṹ
typedef struct tag_rat_ctrl
{
	HRAT  handle;				//�ļ��������
	__s32 index;				//��ǰ�ļ�������
	__s32 total;				//�ļ�����	
	//rat_media_type_t media_type;//�����ļ�������
}rat_ctrl_t;


typedef struct tag_explr_list_para
{
	H_WIN      			list_win;			//���ھ��
	GUI_FONT 			*explr_list_font;
	//H_LYR 				BG_layer;			//����ͼ��//cai modify
	H_LYR 				list_layer;				//listbar ͼ��
	void 				*core;
	__s32				root_type;
	__u32 				last_start_id;		//�ϴ������ʼID
	__u32 				last_focused_id;	//�ϴ�ѡ���ļ�id
	char  				*last_filename;		//�ϴ�����ļ� ��
	
	GUI_FONT			*font;
	
	char 				search_path[RAT_MAX_FULL_PATH_LEN];				//����·��
	H_LBAR 				listbar_handle;
	
	__hdle				long_str_handle;		//���ַ����������
	
	rat_ctrl_t 			rat;					//�ļ��������ݽṹ	
	rat_media_type_t 	media_type;				//�����ļ�������
	explorer_view_mode_e view_mode;				//��ʾģʽ,List or square

	file_list_t*        top_file_list;			//top filelist 
	file_list_t*        cur_file_list;			//current filelist
	file_item_t*        file_item;				//��icon��file_item;

	H_WIN				h_dialog;
	H_WIN				h_dialog_msg;
	
	__bool				del_dlg_open;			//ɾ���Ի����Ƿ�open

    H_WIN				h_fw_update_dlg;        //�̼������Ի���
	
	char 				music_play_file[RAT_MAX_FULL_PATH_LEN];				//�ļ��������ļ�
	char 				fw_update_file[RAT_MAX_FULL_PATH_LEN];				//�̼������ļ�
	root_para_t  *root_para	;		//��¼��root �������Ĳ���
	//char        kak_path[RAT_MAX_FULL_PATH_LEN];
	//H_KAK  hkak;
	//..__s32 karaoke_lab ;

	
	//..char** ppstr;//..����karaoke���ֵ�ָ������
	
}explr_list_para_t;



#define EXPLR_LIST_ENTER			0
#define EXPLR_LIST_RETURN			1

#define MSG_EXPLR_UPDATE			(GUI_MSG_USER_DEF+0)


H_WIN explorer_list_win_create(H_WIN h_parent, explr_list_para_t *para);
__s32 explorer_list_win_delete(H_WIN list_win);

void  explorer_cmd2parent(H_WIN hwin, __s32 id, __s32 data1, __s32 data2);
#endif 







