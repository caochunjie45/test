/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: app_setting_i.h
* By      	: Andy.zhang
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/
#ifndef __APP_SETTINGS_I_H_
#define  __APP_SETTINGS_I_H_

#include "beetles_app.h"

#define SET_ITEM_START_X  140
#define SET_ITEM_START_Y  6
#define SET_ITEM_H        24
#define SET_ITEM_W        330

#define SET_CONT_START_X  360
#define SET_CONT_START_Y  6
#define SET_CONT_H        24
#define SET_CONT_W        90
#define SET_TIPS_W        191
#define SET_TIPS_H        101

#define GENERAL_MENU_ID  				(APP_SETTING_ID +100)
#define POWER_MANAGE_MENU_ID			(APP_SETTING_ID +200)
#define GENERAL_TIPS_ID					(APP_SETTING_ID+300)	// 弹出产品信息时用的
#define SETTING_TIPS_ID					(APP_SETTING_ID+400)	//弹出恢复出厂设置时用的

#define ID_SWITCH_ITEM		0
#define ID_OP_UP			1
#define ID_OP_DOWN			2
#define ID_OP_ENTER			3
#define ID_OP_LEFT			4
#define ID_OP_RIGHT			5
#define ID_OP_EXIT			6
#define ID_ENTER_SMENU  	7
#define ID_EXIT_SMENU   	8 
#define ID_SWITCH_TO_GENERAL 9
#define ID_SWITCH_TO_TIPS 10

#define TIPS_PRODUCT_INFO				0
#define TIPS_FACTORY_DEFAULT			1
typedef enum 
{
	MSG_OP_UP=GUI_MSG_USER_DEF,
	MSG_OP_DOWN,
	MSG_OP_ENTER,
	MSG_OP_LEFT,
	MSG_OP_RIGHT,
	MSG_OP_EXIT,
	MSG_RESTORE_FACTORY,
}set_msg_t;


#endif



