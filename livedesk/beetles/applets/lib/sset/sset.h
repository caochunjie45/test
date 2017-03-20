/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           calendar app sample
*
*						        	(c) Copyright 2006-2009, ANDY, China
*											 All Rights Reserved
*
* File    : sset.h
* By      :lyn
* Version : V1.00
*********************************************************************************************************
*/
#ifndef __SSET_H__
#define __SSET_H__


/********** sset   widget  id 	 ***********/
#define		SSET_SUBMENU_ID		3000				/* �ؼ�listmenu ��ID �� 			*/
#define		SSET_MENU_ID		ID_TOOL_BAR			/* �ؼ�toolbar  ��ID �� 			*/

#define		SSET_NAME_MAX			128
#define		SSET_TEXT_MAX			128	
#define		MENU_CNT				5					/* �˵����� 		*/

typedef enum tag_SSET_MENU_TYPE
{
	MENU_TYPE_TBAR,
	MENU_TYPE_LSTM
	
}__sset_menu_type_e;

/********** ssetting in message ***********/
typedef enum tag_SSET_IN_MSG 
{
	SSET_IN_UPDATE_INFO = 0x4050,
	SSET_IN_INFO_UPDATE,
	SSET_STRING_UPDATE,
	
	SSET_IN_INFO_NUM,
	
}__sset_in_msg_e;

/**********app send msg to setting***********/
typedef enum tag_APP_TO_SSET_MSG
{
	SSET_APP2SSET_PREV = SSET_IN_INFO_NUM,					/*������һ���˵�*/
	SSET_APP2SSET_TO_TBAR,			/*���ص���һ���˵�*/

	
	SSET_APP2SET_NUM,
}__app_to_sset_msg;



/************ item type define  ***********/
typedef enum tag_SSET_ITEM_TYPE
{
	TYPE_NORMAL,
	TYPE_SPIN,
	TYPE_SLIDE,
	TYPE_SWITCH,
	TYPE_OPT,
	TYPE_TBAR,
	
}__sset_item_type_t;

/********** user define message ***********/
typedef	struct tag_SSET_ITEM_NCMSG
{
	__u16			msg_id;
	__u32			msg_value;
}__sset_item_ncmsg_t;


typedef	struct tag_SSET_ITEM_ATTR
{
	__u32 						icon[8];				/* ITEMͼ����ԴID��		*/  
/*********************************************************************************	
	type = TYPE_TBAR
	icon[0] = active focus bmp
	icon[1] = active unfocus bmp
	icon[2] = unactive focus bmp
	icon[3] = unactive unfocus bmp
***********************************************
	type = TYPE_NORMAL
	icon[0] = item focus bmp
	icon[1] = item unfocus bmp
***********************************************
	type = TYPE_SPIN
	icon[0] = item focus bmp
	icon[1] = item unfocus bmp
	icon[2] = spinbox back bmp
	icon[3] = spinbox dec fcs bmp
	icon[4] = spinbox inc fcs bmp
	icon[5] = spinbox dec ufcs bmp
	icon[6] = spinbox inc ufcs bmp
***********************************************
	type = TYPE_BTTN
	icon[0] = item focus bmp
	icon[1] = item unfocus bmp
	icon[2] = button focus bmp
	icon[3] = button unfocus bmp
***********************************************
	type = TYPE_SLIDE
	icon[0] = item focus bmp
	icon[1] = item unfocus bmp
	icon[2] = slide head bmp
	icon[3] = slide tail bmp
	icon[4] = slide cursor bmp
***********************************************
	type = TYPE_OPT
	icon[0] = item focus bmp
	icon[1] = item unfocus bmp
	icon[2] = item opt bmp
***********************************************	
*********************************************************************************/
	__u32 						string[2];				/* ITEM�ı���ԴID��		*/

	__u32 						cur_value;				/* ��ǰֵ				*/  
	__u32 						max_value;				/* ���ֵ				*/
	__u32 						min_value;				/* ��Сֵ				*/
	
	__u32						opt;					/* OPTѡ�ֱ�ʾ			*/
	char						text[SSET_TEXT_MAX];	/* ITEM�ı�				*/

}__sset_item_attr_t;


/**********  sset  date  struct  ***********/
typedef struct tag_ITEM
{
	char						name[SSET_NAME_MAX];	/* ITEM����Ŀ����		*/ 
	__u32			            item_sta;				/* ITEM����Ŀ�Ƿ���Ч	*/

    __sset_item_type_t          item_type;              /* ITEM�ķ��			*/ 

	__sset_item_attr_t			item_attr;			    /* ITEM����Ŀ˽������	*/ 
    
	__sset_item_ncmsg_t			item_msg;			    /* ITEM����Ϣ��			*/  

	struct tag_ITEM 			*father;
	struct tag_ITEM 			*childlst;
	struct tag_ITEM 			*prev;
	struct tag_ITEM 			*next;
		
}__sset_item_t;

/*�ж�sset tbar ��ͼ���Ƿ������ػ�����ʵ״̬״̬*/
__bool SSET_tbar_is_show(void);

/*�жϵ�ǰSSET �Ƿ���TBAR  ״̬��1Ϊ�ǣ�0Ϊ����*/
__bool SSET_court_is_tbar(void);

/*��tbar show ����*/
void SSET_show_tbar(void);

void SSET_hide_tbar(void);

#endif	/* __SSET_H__ */

