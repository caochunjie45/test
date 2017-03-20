/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           calendar app sample
*
*						        	(c) Copyright 2006-2009, ANDY, China
*											 All Rights Reserved
*
* File    : sset_menu.h
* By      :lyn
* Version : V1.00
*********************************************************************************************************
*/
#ifndef __SSET_MENU_TBAR_H__
#define __SSET_MENU_TBAR_H__


/*
*************************************************************************************
*                       				SSET_MenuTbar_Create
*
*Description: SSET�Ӳ˵�����
*
*Arguments  : hparent	
*			  pitem
*			  lyrsta
*
*Return     : NULL
*            
*************************************************************************************
*/
H_WIN SSET_MenuTbar_Create( H_WIN hparent, H_WIN hlayer, __sset_item_t *pitem );
/*
*************************************************************************************
*                       				SSET_MenuTbar_Destroy
*
*Description: SSET�Ӳ˵�����
*
*Arguments  : h_win  : Ҫ���ٵĴ��ھ��
*
*Return     : NULL
*            
*************************************************************************************
*/
__s32 SSET_MenuTbar_Destroy( H_WIN h_win );
/*
************************************************************************************************************************
*                       				SSET_MenuTbar_SetFocus
*
*Description: ����MENU�Ľ���
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/
__s32 SSET_MenuTbar_SetFocus(H_WIN h_win, __u32 fcs_id);
/*
************************************************************************************************************************
*                       				SSET_MenuTbar_UpdateItem
*
*Description: 
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/
__s32 SSET_MenuTbar_UpdateItem( H_WIN h_win, __u32 item_no );


#endif	/* __SSET_MENU_TBAR_H__ */
