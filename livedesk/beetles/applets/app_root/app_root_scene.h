/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    		: app_root_win_scene.h
* By      		: Bayden
* Func		: app_root applets plugin interface define
* Version	: v1.0
* ============================================================================================================
* 2010-9-28 17:11:20  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

/**********************************************************************************************************************/
typedef enum
{
	KEY_TO_TV = 0,
	KEY_TO_MOVIE,
	KEY_TO_MUSIC,
	KEY_TO_PHOTO,
	KEY_TO_FM,
	KEY_TO_RECORD,	
	KEY_TO_CALENDAR,
	KEY_TO_SETTING,
	KEY_TO_EXPLOER,
}key_to_app_t;	

H_WIN app_root_wincreate(Activity *activity);


