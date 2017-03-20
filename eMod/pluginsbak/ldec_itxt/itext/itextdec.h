/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_cedar sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : itextdec.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-9-25
* Description:
    ��ͨ�������ı���Ļ������,һ����utf8����.
********************************************************************************
*/
#ifndef _ITEXTDEC_H_
#define _ITEXTDEC_H_

#define ITEXTDEC_MAX_LBSBUF_SIZE    (1024 * 2)  //lbsbufsize, ��ͬ����Ļ������,ֵ��һ��
#define ITEXTDEC_MAX_FRM_SIZE       (1024 * 1)  //һ֡�ĳ���,����itext��˵,1024�ֽ��㹻��
#define ITEXTDEC_MAX_ANCI_SIZE      (1024 * 1)  //����lbsbuffer��ͷ��buffer,�����ַ��������Ļ,��frameһ����͹���.


typedef struct tag_ItextDec
{
    ItxtDec base;
}ItextDec;
extern __s32 ItextDec_Init(ItextDec *thiz);
extern __s32 ItextDec_Exit(ItextDec *thiz);
extern ItextDec *newItextDec(void);
extern void deleteItextDec(ItextDec *thiz);

#endif  /* _ITEXTDEC_H_ */

