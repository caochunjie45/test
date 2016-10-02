/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           calendar app sample
*
*						        	(c) Copyright 2006-2009, ANDY, China
*											 All Rights Reserved
*
* File    : mainmenu_uipara.h
* By      : James.lu
* Version : V1.00
*********************************************************************************************************
*/
#ifndef __MAINMENU_UIPARA_H__
#define __MAINMENU_UIPARA_H__

#define ITEM_NUM_ALL		8
#define STRING_HIGHT 		20
#define STRING_OFFSET 		10

#define LAYOUT_CNT			4//mainmenu layout cnt

#define MAINMENU_SHINE_NEW
#define MAINMENU_DRAW_FCUFBMP

#define SHINE_CORE_WIDTH 		100   
#define SHINE_CORE_HEIGHT 		100   
#define SHINE_BREATH_WIDTH 		100   
#define SHINE_BREATH_HEIGHT 	100   



#define MAINMENU_STR_COLOR		0xffEEEEEE

//theme buffer handle index
typedef	enum tag_MAINMENU_RESID_INDEX
{
	RESID_INDEX_TV = 0x0,
	RESID_INDEX_MOVIE,
	RESID_INDEX_MUSIC,
	RESID_INDEX_PHOTO,
	RESID_INDEX_FM,
    RESID_INDEX_EBOOK,
	RESID_INDEX_EXPLORER,
	RESID_INDEX_SETTING,
	RESID_INDEX_CALENDAR,
	RESID_INDEX_RECORD,
	RESID_INDEX_KARAOKE,

}__mainmenu_resid_index_e;            

typedef	enum tag_MAINMENU_MOVEPOS
{
	MAINMENU_FRMMOVE_LEFT =0x0,
	MAINMENU_FRMMOVE_RIGHT,
	MAINMENU_FRMMOVE_UP,
	MAINMENU_FRMMOVE_DOWN,
	
}__mainmenu_movepos_e;            

typedef	enum tag_MAINMENU_WINSTA
{
	MAINMENU_WINSTA_FCS =0x0,
	MAINMENU_WINSTA_UNFCS,
	
}__mainmenu_winsta_e;            

typedef struct 
{
	__u32	fid;
	__u32   	posx;
	__u32   	posy;
}__mainmenu_update_frmpos_t;

typedef struct 
{
	__u32	fid;
	__u32	res_index;
	__u32   posx;
	__u32   posy;
	__u32	posw;
	__u32	posh;
	__u32	iconw;
	__u32	iconh;
	__u32	state;////1-enable,0-disable
}__mainmenu_frm_para_t;

typedef	struct
{
	__u8	all_item;
	__u8	cur_layout;
	__mainmenu_frm_para_t* frm_para; 

}__mainmenu_ui_para_t;

typedef struct 
{  
    __s32   fb_w;
    __s32 	fb_h;
    
    __s32   src_x;
    __s32   src_y;
    __s32   src_w;
    __s32   src_h;
    
    __s32 	scn_x;
    __s32	scn_y;
    __s32   scn_w;
    __s32   scn_h;
    
    __u8	pipe;
}__mainmenu_layer_para_t;

typedef struct tag_MAINMENU_UIPARA
{   
    __mainmenu_ui_para_t	 mainmenu_para;
    __mainmenu_layer_para_t  mainmenu_lyr;

}__mainmenu_uipara_t;

typedef struct tag_mainmenu_para
{
	__s32 	cur_item;
	__s32 	cur_item_cnt;
	__s32 	first_flag;
	__s32 	breath_flag;
	HTHEME 	focus_bmp[ITEM_NUM_ALL];
	HTHEME 	unfocus_bmp[ITEM_NUM_ALL];

	__u32	fccolor;
	__u32	ufcolor;
	char 	item_str[ITEM_NUM_ALL][16];

	void*	sprite_buf;
	void*	effect_buf;
	void* 	focus_bmp_hdl[ITEM_NUM_ALL];
	void* 	unfocus_bmp_hdl[ITEM_NUM_ALL];
	H_LYR	mainmenu_layer;

	H_WIN	tv_fwin;
	H_WIN	movie_fwin;
	H_WIN	music_fwin;
	H_WIN	photo_fwin;
	H_WIN	ebook_fwin;
	H_WIN	fm_fwin;
	H_WIN	record_fwin;
	H_WIN	explorer_fwin;
	H_WIN	setting_fwin;

	__mainmenu_uipara_t*	uipara;
}mainmenu_para_t;

__mainmenu_uipara_t* mainmenu_init_uipara(void);
void mainmenu_deinit_uipara(void);

#endif


