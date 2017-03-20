/*
**************************************************************************************************************************
*													        eMOD
*						            		the Easy Portable/Player Develop Kits
*									   			includes melix activity module 
*
*                             		 Copyright(C), 2011-2012, Softwinner Microelectronic Co., Ltd.
*									               All Rights Reserved
*
* File name		:	animation.h
* Author		: 	Derek
* Version 		: 	V1.00
* Date    		: 	2011-12-27
* Description	:
* Others 		:
* History 		:
* <Author>           <time>          <version>      <description>
* Derek Lai		    2011-12-27         1.0.0          Create File
*
**************************************************************************************************************************
*/
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

typedef enum GUI_ANIMATION_MODE
{
	gui_animation_none	= 0,				/*�����ж�������*/
	gui_animation_drag_left,				/*�����϶�*/
	gui_animation_drag_right,				/*�����϶�*/
	gui_animation_zoom_in,					/*�Ŵ�*/
	gui_animation_zoom_out,					/*��С*/
	gui_animation_fade_in,					/*����*/
	gui_animation_fade_out,					/*����*/
}gui_animation_mode_e;

#endif	/*__ANIMATION_H__*/

