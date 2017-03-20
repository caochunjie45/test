#ifndef __MUSIC_LYRICS_H__
#define __MUSIC_LYRICS_H__

#include "beetles_app.h"

#define		C_LYRICS_LINE_MAX		6			//��ֵ��СӦ����UI�������,

typedef struct music_lyrics_ui_s
{
	H_LYR 		layer;					//Ҫ��ʾ��ͼ��
	RECT  		area;					//��ʾ����
	GUI_FONT 	*font;					//����
	__u32      	view_mode;
	GUI_COLOR  	font_color;				//������ɫ
	GUI_COLOR 	hight_light_color;		//����������ɫ		
	
}music_lyrics_ui_t;

typedef music_lyrics_ui_t  music_lyrics_para_t;


typedef struct lyrics_line_s
{
	GUI_COLOR  font_color;
	GUI_RECT   area;
	char       text[128];				//ÿ�������ʾ60���ַ�
}lyrics_line_t;
	
//�������ĿΪ��λ�����з���
typedef struct music_lyrics_s
{
		music_lyrics_ui_t  UI;						//UI����

		
	    __u32 cur_item;								//��ǰ������ʾ����Ŀ��
		__u32 total_item;							//�������Ŀ��		
		__cedar_subtitle_item_t *items;				//�����Ŀ�ṹָ��		

		__cedar_subtitle_item_t *last_item;			//��һ�μ�⵽����Ŀ��,�����ж��Ƿ�õ����µ���Ŀ
		__cedar_subtitle_item_t *last_focus_item;	//�ڶ������������Ŀ
		__cedar_subtitle_item_t *last_normal_item;	//�ڶ������������Ŀ
		__u32 line_count;							//����UI�����������������ʾ������
		__u32 hl_count;								//high light count ������ʾ������
		lyrics_line_t  line[C_LYRICS_LINE_MAX];		//
		
		__epdk_charset_enm_e	char_code;	//�������	
}music_lyrics_t;
typedef  music_lyrics_t* H_LYRIC;


extern H_LYRIC new_music_lyrics(music_lyrics_para_t *para);
extern __s32 delete_music_lyrics(H_LYRIC hLyric);
extern __s32 clear_music_lyrics_area(H_LYRIC hLyric);
extern __s32 update_music_lyrics(H_LYRIC hLyric);

#endif
