/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: circbuf.h
* By      	: Andy.zhang
* Version	: v1.0
* ============================================================================================================
* 2009-11-3 9:39:42  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __CIRCBUF_H__
#define __CIRCBUF_H__


#include "encode_parser\\parser_plugin.h"

typedef ParserConfig LineConfig; 


struct tag_CircularBuff;
typedef struct tag_CircularBuff CircularBuff;

/* �������λ�����   */
CircularBuff *circbuf_create(LineConfig *para);

/* �����������ò��� */
__s32	circbuf_setting(CircularBuff *cb, LineConfig *para);

/* ��ת��offsetȥ*/
__s32 	circbuf_seek( CircularBuff *cb, __u32 offset);


__s32 	circbuf_getStartLines(CircularBuff *cb, MkLine *line, int num);

/* ����ȡ num �ж�����Ϣ 	*/
__s32 	circbuf_getNextLines(CircularBuff *cb, MkLine *line, int num);

/* ��ǰ��ȡ num �ж�����Ϣ 	*/
__s32	circbuf_getPrevLines(CircularBuff *cb, MkLine *line, int num);

/* ɾ�����λ����� 			*/
__s32	circbuf_delete(CircularBuff *cb);

#endif /* __CIRCBUF_H__ */


