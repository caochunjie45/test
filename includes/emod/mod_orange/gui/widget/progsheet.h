/*
*******************************************************************************************************************
*                                                         	ProgSheet  Framework
*                                         			the display dirver support module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name�� 	ProgSheet.h
*
*Author�� 		Terry Zeng
*
*Version :   	1.1.0
*
*Date:  		2009-09-02
*
*Description : 	ProgSheet control implement 
*
*Others : 		None at present.
*
* History :
*
* <Author>          <time>         <version>     <description>
*
* Terry.Zeng     2009-09-02         1.1.0          Create File
*
*******************************************************************************************************************
*/
#ifndef __GUI_PROGSHEET_H__
#define __GUI_PROGSHEET_H__

#define CTRL_PROGSHEET      ("propsheet")


#define	PROGSHEET_ADD_PAGE				0x100
#define	PROGSHEET_SET_ACTIVEINDEX		0x101
#define PROGSHEET_PAINT_PAGE            0x102

typedef struct
{
	void   						*focus_bmp;      /*����bmpλͼ*/
    void   						*unfocus_bmp;    /*�ǽ���bmpλͼ*/
    __pos_t     				bmp_pos;         /*bmpλͼ�ڿؼ��е�λ��*/
    char        				*text;           /*page�ı���*/
    __pos_t     				text_pos;        /*page�����ڿؼ��е�λ��*/
    __u32			   			ftxt_color;      /*����ʱ��������ɫ*/
    __u32		   				uftxt_color;     /*�ǽ���ʱ��������ɫ*/
	GUI_FONT*                   draw_font;       /*�����Ӧ������*/
    __epdk_charset_enm_e        draw_code;       /*�����ַ�����*/
    RECT						head;
}page_para_t;

typedef struct page_s
{
	
	__u32 data;                                  /*page��Ӧ�ģ�page_para_t *������*/
	RECT  rect;                 				/* page��Ӧ�ı�������*/
    H_WIN hwnd;                  				/* page��Ӧ�ľ�̬���ڵľ��*/
    __pGUI_WIN_CB proc;               			/* page ��Ӧ�ľ�̬���ڵĻص�*/
	struct page_s *next;	                     /*��һpage��ָ��*/
}page_t;


typedef struct
{
	RECT      					head_rect;        /*progsheet��Ӧ��ͷ������Ҳ��page���������ڵ�����*/
	void 						*bmp;             /*bmpλͼ*/
	__pos_t  					bmp_pos;          /*bmpλͼ�ڿؼ��е�λ��*/
	char 						*text;        /*page�ı���*/
	__pos_t 					text_pos;        /*page�����ڿؼ��е�λ��*/
	GUI_FONT*       			draw_font;       /*�����Ӧ������*/
    __epdk_char_encoding_e      draw_code;       /*�����ַ�����*/
}progsheet_para_t;


typedef struct
{   
	GUI_MEMDEV_Handle   hmem;                    /*progsheet��Ӧ���ڴ��豸���*/              
	__s32 page_count;                           /*��ǰ�м���page*/
	page_t *active_page;                        /*��ǰactive��page*/
	page_t *head;                               /*page����ĵ�һ��pageָ��*/
	__s32 data;									/*progsheet���ԣ���Ӧprogsheet_para_t **/
}progsheet_data_t;


__bool RegisterProgSheetControl (void);

#endif /*#ifndef __GUI_PROGSHEET_H__*/

