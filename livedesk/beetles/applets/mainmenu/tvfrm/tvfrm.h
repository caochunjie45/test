/*
************************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: tvfrm.c
* By      	: James.lu
* Func	: tvfrm
* Version	: v1.0
* ============================================================================================================
* 2009-10-15 15:27:26 andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __TVFRM_H__
#define __TVFRM_H__


typedef struct tag_tvfrm_res
{	
	void*	sprite_buf;
	void*	focus_bmp;
	void*	unfocus_bmp;
	char*	item_str;
}tvfrm_res_t;

typedef struct tag_tvfrm_attr
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

	tvfrm_res_t res_normal;
}tvfrm_attr_t;


H_WIN tvfrm_frm_create(H_WIN parent, H_WIN layer,__u16 sid,__u32 type);
__s32 tvfrm_frm_delete(H_WIN frmwin);

#endif


