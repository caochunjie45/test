/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           calendar app sample
*
*						        	(c) Copyright 2006-2009, ANDY, China
*											 All Rights Reserved
*
* File    : sset_e.h
* By      :lyn
* Version : V1.00
*********************************************************************************************************
*/
#ifndef __SSET_E_H__
#define __SSET_E_H__

#include "apps.h"
#include "../common/palette.h"
#include "sset.h"



//////////////////////////////////////////////////////////////////////////
#define SSET_DEFAULLT_TIME_SPEED    500


////////////
// #define DECLARE_BMP_RES_Pt(_name)   \
// 	void* _name; \
// 	HTHEME _name ## _h
// 
// #define Create_BMP_RES_Pt(_p, _id)   do {_p ## _h = dsk_theme_open(_id); _p = dsk_theme_hdl2buf(_p ## _h);}while(0)
// 
// #define Destroy_BMP_RES_Pt(_p)       dsk_theme_close(_p ## _h)
/////////////
//////////////////////////////////////////////////////////////////////////

/***********************************************************************************/
typedef	enum tag_SSET_ITEM_INSERT_MODE
{
	SSET_INSERT_CHLD,
	SSET_INSERT_PREV,
	SSET_INSERT_NEXT
	
}__sset_item_insert_mode_e;

typedef	enum tag_SSET_ITEM_DELETE_MODE
{
	SSET_DELETE_CHLD,
	SSET_DELETE_SELF
		
}__sset_item_delete_mode_e;

/********** sset notify  message ***********/
typedef enum tag_SSET_NC_MSG 
{
	SSET_NC_STA_CHG		= 0x5000,
	SSET_NC_STA_NEXT,
	SSET_NC_SET_EXIT,
	SSET_NC_TIMER
	
}__sset_nc_msg_e;


typedef enum tag_SSET_NODELIST_STA
{
	NODELIST_ON,
	NODELIST_OFF,
	NODELIST_DEL,
	
}__sset_nodelist_sta_e;


/***********************************************************************************/

/************ item para define  ***********/
typedef struct tag_SSET_ITEM_PARA
{
	char						name[SSET_NAME_MAX];	/* ITEM����Ŀ����		*/ 
	__u32			            item_sta;				/* ITEM����Ŀ�Ƿ���Ч	*/ 

    __sset_item_type_t          item_type;              /* ITEM�ķ��			*/ 

	__sset_item_attr_t			item_attr;			    /* ITEM����Ŀ˽������	*/ 
    
	__sset_item_ncmsg_t			item_msg;			    /* ITEM����Ϣ��			*/  
	                                                                                 
	__u32 						n;						/* ITEM�Ӳ˵��е���Ŀ��	*/
	struct tag_SSET_ITEM_PARA 	*childarray;			/* ITEM�Ӳ˵���ַ		*/

}__sset_item_para_t;


/********** create para define  ***********/
typedef	struct tag_SSET_CREATE
{
	char						*name;					/* ��������			  */
	__u16						sid;					/* ����ID			  */
	H_WIN						pwin;					/* �����ĸ�����		  */
	H_WIN						powner;					/* ��������������	  */
	__sset_item_para_t			*attr;					/* ������˽������	  */
	                                                                          
	GUI_FONT 					*bfont;					/* �����Ĵ�����		  */
	GUI_FONT 					*sfont;					/* ������С����		  */

	HRES						icon_res;				/* ������ͼ����Դ��� */
	HLANG						lang_res;				/* ������������Դ��� */
	__u32						icon_style;				/* ������ͼ����Դ��� */
	__u32 						lang_style;				/* ������������Դ��� */
	__epdk_charset_enm_e		draw_code;				/* ������������뷽ʽ */
	
	__u32						time_speed;
}__sset_create_para_t;

/***********************************************************************************/
/********** functions define  ***********/
/*
*************************************************************************************
*                       				SSET_Create
*
*Description: SSET��������
*
*Arguments  : para	������������ָ��
*
*Return     : SSET�����Ĺ�����
*            
*************************************************************************************
*/
H_WIN SSET_Create( __sset_create_para_t *para );
/*
*************************************************************************************
*                       				_menu_task
*
*Description: SSET��������
*
*Arguments  : 
*
*Return     : NULL
*            
*************************************************************************************
*/
__s32 SSET_Destroy( H_WIN h_win );
/*
*************************************************************************************
*                       				SSET_UserPara_Insert
*
*Description: ����һ����Ŀ
*
*Arguments  : pitem Ҫ������Ŀ�Ĳ���ڵ�ָ��
*			  para	������Ŀ��ͷָ��
*			  num   �������Ŀ��
*
*Return     : NULL
*            
************************************************************************************
*/
__s32 SSET_Insert( char *name, __sset_item_para_t *para, __u32 num, __sset_item_insert_mode_e mode );
/*
************************************************************************************
*                       				SSET_UserPara_Delete
*
*Description: ɾ��һ����Ŀ
*
*Arguments  : pitem ɾ����Ŀ�����صĽڵ�ָ��
*
*Return     : NULL
*            
************************************************************************************
*/
__s32 SSET_Delete(char *name, __sset_item_delete_mode_e mode);
/*
************************************************************************************
*                       				SSET_EnnableNode
*
*Description: ����һ���ڵ���Ŀ��Ϣ
*
*Arguments  : pitem Ҫ���µĽڵ�
*			  para  ����ָ��
*Return     : 
*            
************************************************************************************
*/
__s32 SSET_EnnableNode(char *name);
/*
************************************************************************************
*                       				SSET_DisableNode
*
*Description: ����һ���ڵ���Ŀ��Ϣ
*
*Arguments  : pitem Ҫ���µĽڵ�
*			  para  ����ָ��
*Return     : 
*            
************************************************************************************
*/
__s32 SSET_DisableNode(char *name);
/*
************************************************************************************
*                       				SSET_SetAttr
*
*Description: ����һ���ڵ���Ŀ��Ϣ
*
*Arguments  : pitem Ҫ���µĽڵ�
*			  para  ����ָ��
*Return     : 
*            
************************************************************************************
*/
__s32 SSET_SetAttr(char *name, __sset_item_attr_t *attr);
/*
************************************************************************************
*                       				SSET_GetAttr
*
*Description: ����һ���ڵ���Ŀ��Ϣ
*
*Arguments  : pitem Ҫ���µĽڵ�
*			  para  ����ָ��
*Return     : 
*            
************************************************************************************
*/
__s32 SSET_GetAttr(char *name, __sset_item_attr_t *attr);

/*
************************************************************************************
*                       				SSET_Get_Nodelist
*
*Description: 
*
*Arguments  :  
*			    
*Return     : 
*            
************************************************************************************
*/
__s32 SSET_Get_Nodelist(char *name, __sset_nodelist_sta_e *sta);

/*
************************************************************************************
*                       				SSET_Set_Nodelist
*
*Description: 
*
*Arguments  :  
*			    
*Return     : 
*            
************************************************************************************
*/
__s32 SSET_Set_Nodelist(char *name, __sset_nodelist_sta_e sta);


#endif	/* __SSET_E_H__ */
