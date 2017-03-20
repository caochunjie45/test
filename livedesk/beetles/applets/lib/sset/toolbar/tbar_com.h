/*
************************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: tbar_com.c
* By      	: Andy.zhang
* Func		: toolbar component
* Version	: v1.0
* ============================================================================================================
* 2009-10-15 15:27:26 andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __TBAR_COM_H__
#define __TBAR_COM_H__

#define  	ID_TOOL_BAR		3030

////////////
#define DECLARE_BMP_RES_Pt(_name)   \
	void* _name; \
	HTHEME _name ## _h
// 
// #define Create_BMP_RES_Pt(_p, _id)   do {_p ## _h = dsk_theme_open(_id); _p = dsk_theme_hdl2buf(_p ## _h);}while(0)
// 
// #define Destroy_BMP_RES_Pt(_p)       dsk_theme_close(_p ## _h)
/////////////

/* toolbar ֪ͨ��   */
#define 	TB_PUSHED		0	
#define 	TB_SWITCH		1	
#define 	TB_UNPUSHED		2
#define 	TB_ESCAPE		3

/* toolbar ������   */
#define 	TB_SET_UPDATE_LANGUAGE	(GUI_MSG_USER_DEF+1)

/**
 * toolbar create para 
 */ 
typedef struct tag_tbar_item
{	
//	void		*focus_bmp;				// ����ͼƬ	
	DECLARE_BMP_RES_Pt(focus_bmp);
//  void		*unfocus_bmp;			// �ǽ���ͼƬ 
	DECLARE_BMP_RES_Pt(unfocus_bmp);
	char		text[GUI_TITLE_MAX];	// ��������	

	__s32      		type;
}tbar_item_t;

typedef struct tag_tbar_para
{	
	tbar_item_t *item;				// item info			
	int 		item_nr;			// item ��Ŀ	
	
	int 		focus_item;			// ���� item index
	SIZE		frame_size;			// ���ο��С
	__pos_t 	bmp_pos;			// ͼƬλ��(��Ծ��ο�)
	__pos_t		text_pos;			// �ı�λ��(��Ծ��ο�)
	int			scn_width;			// ��Ļ���
	
	__u32		focus_color;		// ����focus color
    __u32		unfocus_color;		// ����unfocus color	    
	GUI_FONT	*draw_font;			// font	
	
	int 		src_start;			// ��һ��item��ʼ��ʾλ��
	void*       bg_bmp;             //����ͼƬ
}tbar_para_t;

H_WIN tbar_frm_create(H_WIN parent, H_WIN layer, tbar_para_t *tb_para);

__s32 tbar_frm_delete(H_WIN frmwin);

__epdk_charset_enm_e get_language_charset( void );


#endif /* __TBAR_COM_H__ */


