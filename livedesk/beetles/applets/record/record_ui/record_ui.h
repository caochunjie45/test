/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system
*
*
* File    	: record_ui.h
* By      	: Kingvan
* Func		: record view function
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  Kingvan  create this file, implements the fundemental interface;
**************************************************************************************************************
*/
#ifndef __RECORD_UI_H__
#define __RECORD_UI_H__

#include "beetles_app.h"
#include "apps.h"

#define RECORD_ROOT_PATH				"\\RECORD\\"


#define RECORD_NAME_MAX					40

enum
{
	//����ͼƬ
	RECORD_0_BMP,RECORD_1_BMP,RECORD_2_BMP,RECORD_3_BMP,RECORD_4_BMP,
	RECORD_5_BMP,RECORD_6_BMP,RECORD_7_BMP,RECORD_8_BMP,RECORD_9_BMP,RECORD_10_BMP,
	//���ֲ��Ž���
	RECORD_66_BMP,RECORD_66_LEFT_BMP,RECORD_66_RIGHT_BMP,RECORD_66_MID_BMP,
	//¼������
	RECORD_ANI_BG_BMP, RECORD_ANI_0_BMP,RECORD_ANI_1_BMP,RECORD_ANI_2_BMP,RECORD_ANI_3_BMP,RECORD_ANI_4_BMP,
	//�ײ��Ͷ���bar
	RECORD_BAR_BOTTOM_BMP,RECORD_BAR_TOP_BMP,
	//���״̬
	RECORD_BATTERY_0_BMP,RECORD_BATTERY_1_BMP, RECORD_BATTERY_2_BMP,RECORD_BATTERY_3_BMP,RECORD_BATTERY_4_BMP,RECORD_BATTERY_5_BMP,
	RECORD_BATTERY_ADAPTER_BMP,RECORD_BATTERY_ADD0_BMP,RECORD_BATTERY_ADD1_BMP,RECORD_BATTERY_ADD2_BMP,RECORD_BATTERY_ADD3_BMP, RECORD_BATTERY_ADD4_BMP, RECORD_BATTERY_ADD5_BMP, 
	RECORD_BATTERY_NO_BMP,
	//�������
	RECORD_VOL_OFF_BMP,RECORD_VOL_ON_BMP,
	//ɾ���Ի���
	RECORD_DEL_BOX_BMP,RECORD_DEL_SEL0_0_BMP,RECORD_DEL_SEL0_1_BMP,
	//�����Ի���
	RECORD_QUALITY_BK_BMP,RECORD_QUALITY_FLAG_BMP,
	//¼����ʽ
	RECORD_FORMAT_BK_BMP,
	//�����б����
	RECORD_MUSIC_PROGRESS_BG_BMP,RECORD_MUSIC_PROGRESS_DOWN_BMP,
	RECORD_MUSIC_PROGRESS_FLAG_BMP,RECORD_MUSIC_PROGRESS_UP_BMP,RECORD_MUSIC_PROGRESS_FOCUS_BMP,
	//����
	RECORD_MUSIC_PARTICLE, RECORD_MUSIC_BOUY,
	//¼��ֹͣ, ¼����ʼ, ¼����ͣ
	RECORD_MENU_RECORD_STOP, RECORD_MENU_RECORD_START,RECORD_MENU_RECORD_PAUSE,
	//��������
	RECORD_VOL_BAR_BG,RECORD_VOL_BAR_BODY,RECORD_VOL_LEFT,
	RECORD_VOL_MID,RECORD_VOL_RIGHT, RECORD_VOL_SPEAKER_RIGHT,
	//�߽�
	RECORD_RESOUCE_MAX
};
enum
{
	//����
	STRING_POOR_QUALITY,
	STRING_NORMAL_QUALITY,
	STRING_GOOD_QUALITY,
	//¼����,���Ż�
	STRING_RECORDER,
	STRING_RECORD_PLAY,
	//��¼��ʱ��
	STRING_RECORD_TIME,
	//��ͣ,������..
	STRING_PAUSE,
	STRING_PLAYING,
	//ֹͣ
	STRING_STOP,
	//ɾ���Ի���
	STRING_DELETE,
	STRING_MSG4,
	STRING_YES,
	STRING_NO,
	//¼��ֹͣ, ¼����ʼ, ¼����ͣ
	STRING_MENU_STOP,STRING_MENU_RECORD,STRING_MENU_PAUSE,
	//��¼���ļ�
	STRING_MSG3,
	//�߽�
	STRING_RECORD_MAX
};

typedef enum RECORD_MENU_MODE
{
	RECORD_MAIN_MENU = 0,
	RECORD_SUB_MENU,
	RECORD_MODE_MAX
	
}RECORD_MENU_MODE;

typedef enum RECORD_MENU
{
	MENU_QUALITY	= 0,
	MENU_RECORD,
	MENU_SAVE,
	MENU_MUSIC_LIST,
	MENU_FORMAT,
	MENU_MAX

}RECORD_MENU;

typedef enum RECORD_SUBMENU
{
	SUBMENU_QUALITY = 0x11,
	SUBMENU_FORMAT,
	SUBMENU_UNKNOW
	
}RECORD_SUBMENU;


typedef enum MUSIC_LIST_MENU
{
	MUSIC_MENU_BACK	=	0,
	MUSIC_MENU_BACKWARD,
	MUSIC_MENU_PLAY,
	MUSIC_MENU_PAUSE,
	MUSIC_MENU_FORWARD,
	MUSIC_MENU_DEL,
	MUSIC_MENU_MAX
}MUSIC_LIST_MENU;

typedef enum FOCUSE_STATE
{
	FOCUSE_NORMAL,
	FOCUSE_OVER,
	FOCUSE_PRESS,
	FOCUSE_UP,

	FOCUSE_STATE_MAX
}FOCUSE_STATE;

typedef enum QUALITY_STATE
{
	QUALITY_NORMAL,
	QUALITY_GOOD,
	QUALITY_VERY_GOOD,
	QUALITY_STATE_MAX
}QUALITY_STATE;

#define MAX_RECORD_MAPPING_ITEM		RECORD_MAPPING_MAX
#define MAX_RECORD_BMP_ITEM			RECORD_RESOUCE_MAX
#define MAX_RECORD_LANG_ITEM		STRING_RECORD_MAX


typedef struct tag_res_mapping_info
{
	__s32	lang;
	__s32	unfocus;
	__s32	focus;
}res_mapping_info_t, *pres_mapping_info_t;

typedef struct tag_res_mapping_bmp_info
{
	HTHEME	unfocus;
	HTHEME	focus;
}res_mapping_bmp_info_t, *pres_mapping_bmp_info_t;

typedef struct tag_record_ui
{
	res_mapping_bmp_info_t 	mapping_bmp[MENU_MAX];
	char 					mapping_lang[MENU_MAX][GUI_NAME_MAX];
	res_mapping_bmp_info_t 	music_mapping_bmp[MUSIC_MENU_MAX];
	char 					music_mapping_lang[MUSIC_MENU_MAX][GUI_NAME_MAX];

	char	lang[MAX_RECORD_LANG_ITEM][RECORD_NAME_MAX];
	HTHEME	bmp[MAX_RECORD_BMP_ITEM];
}record_ui_t, *precord_ui_t;

extern __s32 init_record_res( precord_ui_t pui );
extern __s32 uninit_record_res( precord_ui_t pui );

extern void draw_record_mapping( precord_ui_t pui, __s32 mappingIndex, FOCUSE_STATE focus, __s16 mappOffset );
extern void clean_record_mapping( precord_ui_t pui, __s32 mappingIndex );

extern void draw_record_music_mapping( precord_ui_t pui, __s32 mappingIndex, FOCUSE_STATE focus );

extern void draw_record_bmp( precord_ui_t pui, __s32 index );
extern void draw_record_bmp_ext( precord_ui_t pui, __s32 index, __s16 offsetX, __s16 offsetY );
extern void draw_record_bmp_coor( precord_ui_t pui, __s32 index, __s16 X, __s16 Y );
extern void draw_record_bmp_cut( precord_ui_t pui, __s32 index, __s16 xCoor, __s16 yCoor, __s16 height );
extern void draw_record_lang( precord_ui_t pui, __s32 index, __s32 fnt_color );
extern void draw_record_lang_wrap( precord_ui_t pui, __s32 index, __s32 fnt_color );
extern void draw_record_lang_ext( precord_ui_t pui, __s32 index, __s32 fnt_color, GUI_RECT* rect );
extern void draw_record_str( precord_ui_t pui, char* str, __s32 fnt_color, GUI_RECT* rect );
extern void clean_record_bmp( precord_ui_t pui, __s32 index );
extern void clean_record_lang( precord_ui_t pui, __s32 index );
extern void clean_record_str( precord_ui_t pui, const GUI_RECT* rect );

extern __s32 flush_record_res( precord_ui_t pui );

#endif//__RECORD_UI_H__
