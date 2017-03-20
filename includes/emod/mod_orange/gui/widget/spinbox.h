/*
*******************************************************************************************************************
*                                                         	Listmenu  Framework
*                                         			the display dirver support module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name�� 	change_private.h
*
*Author�� 		William Wang
*
*Version :   	1.1.0
*
*Date:  		2009-8-4
*
*Description : 	gui button ˽�����ݽṹ�ͳ������� 
*
*Others : 		None at present.
*
* History :
*
* <Author>          <calendar>       <version>     <description>
*
* William Wang     2009-8-4         1.1.0          Create File
*
*******************************************************************************************************************
*/
#ifndef __GUI_SPINBOX_PRIVATE_H__
#define __GUI_SPINBOX_PRIVATE_H__

#include "ResAdapter.h"

#define CTRL_SPINBOX         ("spinbox")
	

#define BST_PUSHED          (1<<1)
#define BST_FOCUS           (1<<2)
#define BST_LEFT            (1<<3)

	
#define SPN_CHG          	 0
#define SPN_STOPCHG          1

	
typedef struct tagchapara
{
		__u32						spin_sta;				/*����ͼ��λ��*/	
//		void						*fdn_bmp;				/*����ͼ�� ����*/	
		DECLARE_BMP_RES_Pt(fdn_bmp);
//		void						*fup_bmp;				/*����ͼ�� ����*/
		DECLARE_BMP_RES_Pt(fup_bmp);
//		void						*ufdn_bmp;				/*����ͼ�� �ǽ���*/
		DECLARE_BMP_RES_Pt(ufdn_bmp);
//		void						*ufup_bmp;				/*����ͼ�� �ǽ���*/
		DECLARE_BMP_RES_Pt(ufup_bmp);
		__pos_t 					upbmp_pos;				/*����ͼ��λ��*/	
		__pos_t 					dnbmp_pos;				/*����ͼ��λ��*/
		__s16						cur_value;				/*��ǰֵ*/
		__s16						max_value;				/*���ֵ*/
		__s16						min_value;				/*��Сֵ*/
		__pos_t 					text_pos;				/*�ı�λ��*/
		__u32						ftxt_color;				/*�ı�����ɫ*/
		__u32						uftxt_color;			/*�ı��ǽ���ɫ*/
		GUI_FONT*					draw_font;				/*�ַ�����*/
		__epdk_charset_enm_e		draw_code;	 			/*�ַ�����*/
    	__u32                       bk_color;				/*����ɫ*/
}spinbox_para_t;

typedef struct tagSPINBOXDATA
{
	GUI_MEMDEV_Handle	hmem;
	__u32				draw_sts;
	__u32				status; 
	__u32				draw_flag;	
	__u32				data;
}spinbox_data_t;

typedef spinbox_data_t* pspinboxdata;  
	
#define SPINBOX_DRAWSTATUS(pCtrl) (((pspinboxdata)GUI_CtrlWinGetAddData(pCtrl))->draw_sts)
#define SPINBOX_STATUS(pCtrl)     (((pspinboxdata)GUI_CtrlWinGetAddData(pCtrl))->status)
#define SPINBOX_DRAWFLAG(pCtrl)  (((pspinboxdata)GUI_CtrlWinGetAddData(pCtrl))->draw_flag)
#define SPINBOX_DATA(pCtrl)       (((pspinboxdata)GUI_CtrlWinGetAddData(pCtrl))->data)

__bool RegisterSpinboxControl (void);

__s32  GUI_SpinboxGetCurValue(H_WIN hspin, __s16  *value);
__s32  GUI_SpinboxSetCurValue(H_WIN hspin, __s16  value);
__s32  GUI_SpinboxSetMaxValue(H_WIN hspin, __s16  value);
__s32  GUI_SpinboxSetMinValue(H_WIN hspin, __s16  value);
__s32  GUI_SpinboxSetBkColor(H_WIN hspin,__u32 color);
__s32  GUI_SpinboxSetUnFcsTxtColor(H_WIN hspin,__u32 color);
__s32  GUI_SpinboxSetFcsTxtColor(H_WIN hspin,__u32  color);
__s32  GUI_SpinboxSetFocusUpBmp(H_WIN hspin,void *bmp);
__s32  GUI_SpinboxSetFocusDownBmp(H_WIN hspin,void *bmp);
__s32  GUI_SpinboxSetUnFocusUpBmp(H_WIN hspin,void *bmp);
__s32  GUI_SpinboxSetUnFocusDownBmp(H_WIN hspin,void *bmp);
__s32  GUI_SpinboxSetUpBmpPos(H_WIN hspin,__pos_t *pos);
__s32  GUI_SpinboxSetDownBmpPos(H_WIN hspin,__pos_t *pos);

#endif /* #ifndef __GUI_SPINBOX_PRIVATE_H__ */
