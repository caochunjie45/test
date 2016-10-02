/*
************************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: explorerfrm.c
* By      	: James.lu
* Func	: explorerfrm
* Version	: v1.0
* ============================================================================================================
* 2009-10-15 15:27:26 andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __EXPLORERFRM_H__
#define __EXPLORERFRM_H__


typedef struct tag_explorerfrm_res
{	
	void*	sprite_buf;
	void*	focus_bmp;
	void*	unfocus_bmp;
	char*	item_str;
}explorerfrm_res_t;

typedef struct tag_explorerfrm_attr
{	
	__u16		fid;
	__u32 		lyr_type;
	H_WIN		pwin;
	H_LYR		cur_lyr;

	__u32		fccolor;
       __u32		ufcolor;
	GUI_FONT*	bfont;
	GUI_FONT*	sfont;

	//frm para
	SIZE		icon_size;
	SIZE		frame_size;

	explorerfrm_res_t res_normal;
}explorerfrm_attr_t;


H_WIN explorerfrm_frm_create(H_WIN parent, H_WIN layer,__u16 sid,__u32 type);
__s32 explorerfrm_frm_delete(H_WIN frmwin);

#endif


