/*
*******************************************************************************************************************
*                                                         	Mod orange 
*                                         			the embedded graphic support module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name�� 	winclass.h
*
*Author�� 		William Wang
*
*Version :   	1.1.0
*
*Date:  		2009-8-4
*
*Description : 	GUI ����������ݽṹ�ͳ������� 
*
*Others : 		None at present.
*
* History :
*
* <Author>          <time>       <version>     <description>
*
* William Wang     2009-8-4         1.1.0          Create File
*
*******************************************************************************************************************
*/
#ifndef __GUI_WINCLASS_H__
#define __GUI_WINCLASS_H__

typedef struct _GUI_WNDCLASS
{
    char*           spClassName;            /** �������� */
    __u32           opMask;                 /*�ڲ�������*/
    __u32           dwStyle;                /*��Ĵ��ڷ��*/
    __u32           dwExStyle;              /*���������չ���*/
    __pGUI_WIN_CB   WinProc;                /*������Ĵ��ڴ�����*/
    __u32           dwAddData;              /*������ĸ�������*/
}__gui_winclass_t;

typedef __gui_winclass_t* pwinclass;


extern __s32 GUI_AddNewControlClass (pwinclass	pWndClass);
extern __s32 GUI_SetCtrlClassAddData (const char* szClassName, __u32 dwAddData);
extern void  GUI_EmptyControlClassInfoTable (void);
extern __s32 GUI_GetCtrlClassAddData (const char* szClassName, __u32* pAddData);
extern __s32 GUI_DeleteControlClass (const char* szClassName);
extern void GUI_TerminateControlClass (void);

#endif
