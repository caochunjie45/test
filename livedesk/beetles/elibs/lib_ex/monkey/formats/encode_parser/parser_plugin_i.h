/*
**************************************************************************************************************
*											        ePOS
*						           the Easy Portable/Player Operation System
*									          update sub-system
*
*						             (c) Copyright 2008, Andy.zhang China
*										  	  All Rights Reserved
*
* File		: parser_plugin_i.h
* Version	: V1.0 
* Data		: 2009-5-02
* By		: Andy.zhang
* Biref		: txt parser 
**************************************************************************************************************
*/

#ifndef __PARSER_PLUGIN_I_H__
#define __PARSER_PLUGIN_I_H__

#include "monkey_i.h"


#define	BufferLenght		4*1024							//8k����	8*1024

struct tag_txtEncodeParser;
typedef struct tag_txtEncodeParser 	txtEncodeParser;

typedef struct tag_ParserConfig
{
	ES_FILE		*fd;								// �ļ����	
	__s32		len;								// ��ǰ�ļ��ĳ���
	__s32		lineofpage;							// ��ǰ��ͼ����¿�����ʾ������
	MkEncode	encodeType;							// �����ʽ 
	
	__u32		xScrWidth;							// ��������Ŀ� 
	__u32		startOffset;						// ��ʼƫ����
	H_LYR       hlyr;								// ͼ����
	GUI_FONT    *pFont;								// ���־��
	
	void (*p_dispstr)(char *s, int len , int x, int y);	
}ParserConfig;


typedef void (*txt_parserConfig)(txtEncodeParser *thiz, 	ParserConfig *pconfig);
typedef void (*txt_parserCacheStart)(txtEncodeParser *thiz, int startoffset);
typedef void (*txt_parserCacheNext)(txtEncodeParser *thiz, 	int current_start);
typedef void (*txt_parserCachePrev)(txtEncodeParser *thiz, 	int current_start);
typedef __s32 (*txt_parserAnalysis)(txtEncodeParser *thiz, 	MkLine *line);
typedef void (*txt_parserDispstr)(char *s, int len , int x, int y);
typedef void (*txt_parserDestroy)(txtEncodeParser *thiz);

struct tag_txtEncodeParser
{
	txt_parserConfig				config;
	txt_parserCacheStart			start_seek;
	txt_parserCacheNext				next;
	txt_parserCachePrev				prev;
	txt_parserAnalysis				analysis;	
	txt_parserDispstr				dispstr;
	txt_parserDestroy				destroy;

	/* context */
	ES_FILE *fd;							// �ļ����
	__s32	txtLength;						// ��ǰ�ļ��ĳ���
	__s32	viewWidth;						// ��������Ŀ�	
	__s32	chinese_width;					// ���ֵ��ַ����(��Ϊ���ֵĿ��һ����С���ܹ����ٷ�������)
	
	__u8	*bufferTxt;						// �ļ�����	
	__s32	bufferTxtlen;					// �ļ����峤��	
	__s32	start;							// �ļ�������ʼƫ����, ע�� : start ��һ���� ParserConfig.startOffset��ͬ
	__s32 	cur_offset;						// analysis ��ǰƫ����
	__bool  eof;							// �����������Ƿ��ѵ��ĵ������
	__bool  bof;							// �����������Ƿ��ѵ��ĵ���ǰ��

	H_LYR       hlyr;							
	GUI_FONT    *pFont;		
	
};

#endif /* __PARSER_PLUGIN_I_H__ */
