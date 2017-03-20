/*
*********************************************************************************************************
*                                                    ePDK
*                                   the Easy Portable/Player Development Kit
*                                               eLibC sub-system
*
*                                    (c) Copyright 2010-2012, Sunny China
*                                              All Rights Reserved
*
* File    : elibs_bookengine.h
* By      : Sunny
* Version : V1.00
* Date    : 2011-5-2 13:24:12
*********************************************************************************************************
*/

#ifndef __ELIBS_BOOKENGINE_H__
#define __ELIBS_BOOKENGINE_H__

#include ".\..\..\emod\mod_bookengine.h"
Hebook ebook_open(char *filename, unsigned char *errno, EBOOK_LANGUAGE lang, EBOOK_CHARENCODE encode);			/* ��ebook */

Hebookbitmap ebook_get_page( Hebook host, int pageno, unsigned char * errno, unsigned char *destbuf);			/* ��ȡָ��ҳ�����ʾҳ */

Hebookbitmap ebook_goto_booktag(Hebook host, EBOOKTAG *tag, unsigned char * errno, unsigned char *destbuf);		/* ��ת����ǩλ��*/

Hebookbitmap ebook_goto_end(Hebook host);						/* ��ת�����һҳ */

Hebookbitmap ebook_find_text(Hebook host,char text[]);			/* ��ת��ָ���ı���ҳ�� */

void ebook_free_pagemem(Hebookbitmap surface);					/*�ͷŵ�ǰ��ʾҳ���mem��Դ*/

void ebook_close(Hebook host);									/*�˳����ĵ�*/

Hebookbitmap ebook_close_and_getbookcover(Hebook g_host, int width, int height, unsigned char * errno, unsigned char * destbuf);    /*��ȡ����BOOK��bookcover���˳�*/


/**********************************************/
int 	ebook_set_scnwidth(Hebook host,unsigned int scnwidth);		/* ������ʾ���Ŀ�� */

int 	ebook_set_scnheight(Hebook host,unsigned int scnheight);	/* ������ʾ���ĸ߶� */

int 	ebook_set_fontsize(Hebook host,unsigned int fontsize);		/* ���������С������PDF��Ч�� */

void 	ebook_set_reflow_mode(Hebook host);							/* ����reflow��ʾģʽ��PDF�ĵ��� */

void 	ebook_cancel_reflow_mode(Hebook host);						/* ȡ��reflow��ʾģʽ��PDF�ĵ���*/

void 	ebook_set_scale(Hebook host,float s);						/*����ҳ������ű���*/

void 	ebook_set_margin_left(double left);							/*������߿հ״�С*/

void 	ebook_set_margin_right(double right);						/*�����ұ߿հ״�С*/

void 	ebook_set_margin_top(double top);							/*�����ϱ߿հ״�С*/

void 	ebook_set_margin_bottom(double bottom);						/*�����±߿հ״�С*/

void 	ebook_setbackcolor(int red, int green, int blue);			/*���ñ���ɫ*/

void 	ebook_setfontcolor(JM_FONT_COLOR color);					/*��������ɫ*/

/****************************************************************/
Hebookbitmap 	ebook_get_bookcover( char * filename,int width, int height, unsigned char * errno, unsigned char *destbuf); /*��ȡ�鼮������Ϣ*/

void 			ebook_free_bookcover(Hebookbitmap surface);					/*�ͷŷ����mem��Դ*/

void 			ebook_get_bookinfo(char * filename,	EBOOKINFO * bookinfo,unsigned char * errno);	/*��ȡ�鼮�����Ϣ*/

int 			ebook_get_totalpagecount(Hebook host);						/* ��ȡ�ܵ�ҳ���� */

Hebookbitmap 	ebook_get_screen(Hebook host_1, int num,unsigned char * errno, unsigned char *destbuf);

void 			ebook_save_booktag(Hebook host, int num, EBOOKTAG *sbooktag,unsigned char * error);			/*������ǩ*/

void            ebook_settimezone(time_zone_e tz, int is_daylight);         /*����ʱ��*/            

JM_FONT_COLOR   ebook_get_fontcolor(Hebook host);                           /*��ȡ������ɫ*/    

int             ebook_get_fontsize(Hebook host);                            /*��ȡ�����С*/

int             ebook_get_backcolor(Hebook host);                            /*��ȡ����ɫֵ*/

void            ebook_set_language(Hebook host, EBOOK_LANGUAGE lang);          /*���õ����������*/  

void            ebook_set_charencode(Hebook host, EBOOK_CHARENCODE encode);       /*���õ�����ı����ʽ*/ 

EBOOK_LANGUAGE  ebook_get_language(Hebook host);                            /*��ȡ�����������*/ 

EBOOK_CHARENCODE     ebook_get_charencode(Hebook host);                    /*��ȡ������ı����ʽ*/  

BOOKENGINETOC   ebook_get_toc(Hebook host);                                /*��ȡ�������Ŀ¼��Ϣ*/

void            ebook_release_toc(Hebook host);                            /*�ͷŵ������Ŀ¼��Ϣ*/

char *          ebook_get_txtinscrn(Hebook host_1, int num,unsigned char * errorno);    /*��ȡ��ǰҳ���ڵ�������Ϣ*/

int             ebook_get_PDFheightscale(Hebook host, int screennum);       /*��ȡpdf�ļ���ǰ��ҳ��Ŀ�߱ȣ�����ֵΪ(height*1000/width)*/
 
void            ebook_RendFrame(EBOOK_RENDCORE_PTR rendcore);               /*��Ⱦ3D��ҳЧ��ʱ��ʹ��*/ 

void            ebook_setfontrgbcolor(int red, int green, int blue);        /*��������RGB��ɫֵ����0-255��*/

void            ebook_set_CHSFontStyle(CHINESE_FONT_STYLE fontstyle);        /*����������������*/

void            ebook_set_RUSFontStyle(RUSSIAN_FONT_STYLE fontstyle);       /*���ö���������ʽ*/
#endif /* __ELIBS_ebook_H__  */
