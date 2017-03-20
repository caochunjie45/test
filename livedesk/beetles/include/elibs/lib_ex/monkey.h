/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: monkey.h
* By      	: Andy.zhang
* Func		: monkey core interface 
* Version	: v1.0
* ============================================================================================================
* 2009-11-3 9:39:42  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __MONKEY_H__
#define __MONKEY_H__

typedef void* H_MKC; 

/* �����ʽ */
typedef enum
{
	MK_CHAR_ENCODE_UNKNOWN = 0,
	MK_CHAR_ENCODE_UTF8,
	MK_CHAR_ENCODE_GBK,
	MK_CHAR_ENCODE_UNICODE,
	MK_CHAR_ENCODE_UNICODE_BIG	
}MkEncode;

/* ��ת���� */	
typedef enum
{
	MK_ROTATE_0 = 1,
	MK_ROTATE_90,
	MK_ROTATE_180,
	MK_ROTATE_270	
}MkRotate;

/* ��ǩѡ�� */
typedef enum
{
	MK_BMK_1 = 1,
	MK_BMK_2,
	MK_BMK_3,
	MK_BMK_4,
	MK_BMK_5,
	MK_BMK_6,
	MK_BMK_7,
	MK_BMK_8,
	MK_BMK_
}MkBmk;

/* ���� */
typedef enum
{
	MK_FONT_0 = 1,
	MK_FONT_1,	
	MK_FONT_
}MkFont;

/* ��ɫ */
typedef enum
{
	MK_COLOR_BLACK = 1,
	MK_COLOR_BLUE,
	MK_COLOR_GREEN,
	MK_COLOR_RED,
	MK_COLOR_WHITE,
	MK_COLOR_
}MkColor;

/* ҳ�л�ģʽ */
typedef enum
{
	MK_SWITCH_NO = 1,		// ���л�Ч��
	MK_SWITCH_GLIDE			// �����л�	
}MkSwitch;

/* ���� */
typedef enum
{
	MK_BRIGHT_0 = 1,
	MK_BRIGHT_1,
	MK_BRIGHT_2,
	MK_BRIGHT_3,
	MK_BRIGHT_4,
	MK_BRIGHT_
}MkBright;

/* ���ñ���ͼƬ */
typedef enum
{
	MK_BACK_1 = 1,
	MK_BACK_2,
	MK_BACK_3,
	MK_BACK_4,
	MK_BACK_
}MkBack;

/* �Զ��л�ʱ��*/
typedef enum
{
	MK_AS_0 = 1,		// ��ֹ�Զ��л�
	MK_AS_10,		// 10s�Զ��л�
	MK_AS_15,		// 15s�Զ��л�
	MK_AS_20,		// 20s�Զ��л�
	MK_AS_25,		// 25s�Զ��л�
	MK_AS_30		// 30s�Զ��л�
}MkAutoP;

/* ����Ϣ */
typedef struct 
{
	__s32 start;
	__s32 len;
}MkLine;


typedef struct 
{
	/* gui ��Դ */	
	H_LYR       hlyr_1;										//����ͼ����
	H_LYR       hlyr_2;										//����ͼ����	
	int 		contentLyrWidth;							//ͼ����
	int 		contentLyrHeight;							//ͼ��߶�
	int 		sWidth;										//��Ļ���
	int 		sHeight;									//��Ļ�߶�

	GUI_FONT    *pFont0;									//������1
	GUI_FONT    *pFont1;									//������2	
			
	/* ��Ļ����*/
	__u32		xStart, yStart;								//���Ͻ�����
	__u32		xScrWidth, yScrHeight, lineofspace;			//��Ļ��ʾ����			
}MkcConfig;


/* �򿪾��*/
H_MKC mkc_open(void);

/* ��ѯ������Ϣ */
MkFont   mkc_getFont(H_MKC mCore);		/* ��ѯ��ǰ�������� */
MkColor  mkc_getCharColor(H_MKC mCore); /* ��ѯ��ǰ������ɫ */
MkRotate mkc_getRotate(H_MKC mCore);	/* ��ѯ��ת����     */
MkSwitch mkc_getSwitch(H_MKC mCore);	/* ��ѯ�л���ʽ		*/
MkEncode mkc_getDefEncode(H_MKC mCore);	/* ��ѯĬ�ϱ��뷽ʽ	*/
MkBright mkc_getBright(H_MKC mCore);	/* ��ѯ����ǿ��		*/
MkAutoP  mkc_getAutoPage(H_MKC mCore);	/* ��ѯ�Զ���ҳʱ��	*/
MkBack	 mkc_getBack(H_MKC mCore);		/* ��ѯ����ͼƬ���	*/

/* ��ѯ�ļ���Ϣ */
__s32  mkc_getFileName(H_MKC mCore, char *fileName);	/* ��ѯ��ǰ���ļ�����*/

/* ��ѯ��ǩѡ�� */
__s32  mkc_openBmk(H_MKC mCore, 		MkBmk 	 mBmk);		/* ��ת��ָ����ǩָ��λ�� */ 
__s32  mkc_saveBmk(H_MKC mCore, 		MkBmk 	 mBmk);		/* ���浱ǰƫ����Ϊ��ǩ */
__s32  mkc_getBmkOffset(H_MKC mCore, 	MkBmk 	 mBmk);		/* ��ѯ��ǰ��ǩƫ�ưٷֱ�, ƫ������λ: ���֮һ */

/* ��ʾ��� */
__s32  mkc_showConfig(H_MKC mCore,		MkcConfig *para);	/* ������ʾ������� */
__s32  mkc_showStart(H_MKC mCore);							/* ��ҳ, ����ҳ��ƫ����, ƫ������λ: ���֮һ  */		
__s32  mkc_showNext(H_MKC mCore);							/* ��һҳ, ����ҳ��ƫ����, ƫ������λ: ���֮һ*/
__s32  mkc_showPrev(H_MKC mCore);							/* ��һҳ, ����ҳ��ƫ����, ƫ������λ: ���֮һ*/
__s32  mkc_showSeek(H_MKC mCore, 		__s32 offset);		/* ��ת����, ����ʵ��ҳ��ƫ����, ƫ������λ: ���֮һ */

/* ����ѡ�� */
__s32  mkc_setFont(H_MKC mCore, 		MkFont   mFont);	/* ���������С*/
__s32  mkc_setCharColor(H_MKC mCore,	MkColor  mColor);	/* ����������ɫ*/
__s32  mkc_setRotate(H_MKC mCore, 		MkRotate mRotate);	/* ������ת�Ƕ�*/
__s32  mkc_setSwitchMode(H_MKC mCore,	MkSwitch mMode);	/* ����ҳ�л�ģʽ*/
__s32  mkc_setDefEncode(H_MKC mCore, 	MkEncode mEncode);	/* ����Ĭ�ϱ����ʽ*/
__s32  mkc_setBright(H_MKC mCore, 		MkBright mBright);	/* �������� */
__s32  mkc_setAutoPage(H_MKC mCore, 	MkAutoP  mAutoP);	/* �Զ���ҳʱ�� */
__s32  mkc_setBack(H_MKC mCore, 		MkBack   mBack);	/* ���汳��ͼƬ���� */

__s32  mkc_getCurPageOffset(H_MKC mCore, __s32 *start, __s32 *end); /* ��ѯ��ǰҳ��ҳͷҳβƫ����*/
__bool mkc_isHead(H_MKC mCore);										/* �Ƿ�ͷ */
__bool mkc_isTail(H_MKC mCore);										/* �Ƿ�β */


/* ɾ����� */
__s32  mkc_close(H_MKC mCore);

#endif /* __MONKEY_H__ */


