/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: gscene_headbar.h
* By      	: Andy.zhang
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2010-9-25 19:19:34 andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __GSCENE_HEADBAR_H__
#define __GSCENE_HEADBAR_H__

typedef enum _HBarState
{
	HBAR_ST_SHOW,
	HBAR_ST_HIDE	
}HBarState;

typedef enum _HBarApp
{
	HBAR_APP_SHOW,
	HBAR_APP_HIDE	
}HBarApp;

typedef struct 
{
	char 			*name;		// ³¡¾°Ãû³Æ
	__u16 			sid;		// ³¡¾°id	
	H_WIN 			parent;		// ¸¸´°¿Ú
	H_WIN 			layer;		// Í¼²ã	
	GUI_FONT    	*font;		// ×ÖÌå¾ä±ú	
}HBarCreatePara;

__s32 gscene_hbar_create(H_WIN parent, GUI_FONT *font);
__s32 gscene_hbar_delete(void);
__s32 gscene_hbar_set_state(HBarState state);
__s32 gscene_hbar_get_state(HBarState *p_state);
__s32 gscene_hbar_set_title(char *title, __u32 len);	/* title Îªutf8±àÂë¸ñÊ½×Ö·û´®£¬len<=32 ,ÉèÖÃhbar title ÇøÓò×Ö·û´®*/
__s32 gscene_hbar_current_disk(__u32 cur_type, __u32 root_type);
__s32 gscene_hbar_set_appstate(HBarApp appstate);
__s32 gscene_hbar_get_appstate(HBarApp *appstate);
__s32 gscene_hbar_set_disk(__u32 cur_type, __u32 root_type);
__s32 gscene_hbar_get_disk(__u32 *cur_type, __u32 *root_type);

#endif /* __GSCENE_HEADBAR_H__ */


