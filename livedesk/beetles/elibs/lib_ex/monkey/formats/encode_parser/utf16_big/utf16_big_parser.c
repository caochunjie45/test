/*
**************************************************************************************************************
*											        ePOS
*						           the Easy Portable/Player Operation System
*									          update sub-system
*
*						             (c) Copyright 2008, Andy.zhang China
*										  	  All Rights Reserved
*
* File		: utf16_big_parser.h
* Version	: V1.0 
* Data		: 2009-5-02
* By		: Andy.zhang
* Biref		: utf16_big�������
**************************************************************************************************************
*/
#include "monkey_i.h"
#include "..\parser_plugin_i.h"
#include "utf16_big_parser.h"

static void utf16big_config(txtEncodeParser *thiz, ParserConfig *pconfig);
static void utf16big_start(txtEncodeParser *thiz, __s32 startoffset);
static void utf16big_next(txtEncodeParser *thiz, int current_start);
static void utf16big_prev(txtEncodeParser *thiz, int current_start);
static __s32 utf16big_analysis(txtEncodeParser *thiz, MkLine *lines);
static void utf16big_dispstr(char *s, int len , int x, int y);
static void utf16big_destroy(txtEncodeParser *thiz);

txtEncodeParser *utf16big_parserNew(void)
{
	txtEncodeParser *utf16big_parser;
	
	utf16big_parser = esMEMS_Malloc(0, sizeof(txtEncodeParser));
	if(!utf16big_parser)
	{
		__inf("%s %d : utf16big_parser_new malloc error \n", __FUNCTION__, __LINE__);
		return NULL;
	}
	eLIBs_memset(utf16big_parser, 0, sizeof(txtEncodeParser));
	
	utf16big_parser->config 		= utf16big_config;
	utf16big_parser->start_seek  	= utf16big_start;
	utf16big_parser->next			= utf16big_next;
	utf16big_parser->prev			= utf16big_prev;
	utf16big_parser->analysis		= utf16big_analysis;
	utf16big_parser->dispstr		= utf16big_dispstr;
	utf16big_parser->destroy		= utf16big_destroy;
	utf16big_parser->bufferTxt		= esMEMS_Palloc(2*BufferLenght/1024, 0); //����2*8��ҳ�棬ÿ��ҳ���С1kbytes
	return utf16big_parser;
}

static void utf16big_config(txtEncodeParser *thiz, ParserConfig *config)
{
	__u16 tmp_chinese;
	config->p_dispstr   = 	thiz->dispstr;

	thiz->fd			=	config->fd;
	thiz->txtLength 	= 	config->len;	
	thiz->viewWidth		=	config->xScrWidth;
//	thiz->viewHeight	=	config->yScrHeight;	
//	thiz->lineofpage	=	config->lineofpage;
//	thiz->FontHeight	=	config->FontHeight;	
	
	thiz->bufferTxtlen	=	0;	
	thiz->start			= 	config->startOffset;
	
	if( thiz->start <= 2 )	
		thiz->bof = EPDK_TRUE;	
	else					
		thiz->bof = EPDK_FALSE;			
	
	thiz->eof			=	EPDK_FALSE;	
	thiz->pFont			=	config->pFont;
	thiz->hlyr			=	config->hlyr;
	
	GUI_LyrWinSel(thiz->hlyr);	
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(thiz->pFont);	
	GUI_SetFontMode(GUI_FONTMODE_8BPP256);
	
	tmp_chinese 		= eLIBs_GB2312_to_Unicode(0xced2);
	thiz->chinese_width	= GUI_GetCharDistX(tmp_chinese);

	
}

static void utf16big_start(txtEncodeParser *thiz, int startoffset)
{
	__u8 *buff;
	int len;

	buff  = thiz->bufferTxt + BufferLenght;							//������ָ��

	if( startoffset <= 2 )
	{
		thiz->bof			=	EPDK_TRUE;
		thiz->start			=	2;
		eLIBs_fseek(thiz->fd, thiz->start , ELIBS_SEEK_SET);		//����--------------
		return;
	}
	
	thiz->bof				=	EPDK_FALSE;
	
	startoffset = (startoffset%2)?(startoffset+1):(startoffset);
		
	eLIBs_fseek(thiz->fd, startoffset , ELIBS_SEEK_SET);
	len = eLIBs_fread(buff, sizeof(char), BufferLenght, thiz->fd);
	
	if( len != BufferLenght)
	{
		thiz->eof	= EPDK_TRUE;
	}
	
	thiz->eof = EPDK_FALSE;
	thiz->start = startoffset;
	return;	
}

static void utf16big_next(txtEncodeParser *thiz, int current_start)
{
	int len, end;
	
	thiz->start 		 = current_start;
	thiz->bof			 = (current_start < 2)?EPDK_TRUE : EPDK_FALSE;
	thiz->bufferTxtlen 	 = 0;										//�ļ����ݻ�����0
	
	eLIBs_fseek( thiz->fd, thiz->start, ELIBS_SEEK_SET);					
	len = eLIBs_fread(thiz->bufferTxt, sizeof(char), BufferLenght, thiz->fd);	

    if (len != BufferLenght) 									//�Ѷ��������µ�β��
	{ 			
        thiz->eof = EPDK_TRUE;									//���ļ�β����־λ       
		thiz->bufferTxtlen = len;	
		
		__inf("%s %d len = %d \n", __FUNCTION__, __LINE__, len);
        return;
    }
	
    thiz->eof = EPDK_FALSE;										//ɾ�������������һ���س��Ժ���ַ�
    end = (len%2)?(len+1):len;
    		
	thiz->bufferTxtlen = end;	
	__inf("%s %d len = %d \n", __FUNCTION__, __LINE__, end);
	return;	
}

static void utf16big_prev(txtEncodeParser *thiz, int current_start)
{
	int start;
	int len;
	//__u8 *b;

	start 		= current_start - BufferLenght;
	thiz->bof 	= EPDK_FALSE;

	//b 			= thiz->bufferTxt + BufferLenght;

	if(start < 0)
	{
		start = 2;
		thiz->bof = EPDK_TRUE;

		eLIBs_fseek( thiz->fd, start, ELIBS_SEEK_SET);
		len = eLIBs_fread(thiz->bufferTxt, sizeof(char), current_start, thiz->fd);
		
		thiz->bufferTxtlen = current_start;
		thiz->start = 0;
		return;
	}	
	
	start = (start%2)?(start+1):start;
	
	thiz->start = start;
	
	__inf(" %s %d start = %d end =  %d \n", __FUNCTION__, __LINE__, start, current_start);
	eLIBs_fseek( thiz->fd, start, ELIBS_SEEK_SET);
	len = eLIBs_fread(thiz->bufferTxt, sizeof(char), BufferLenght, thiz->fd);
	
	__inf(" %s %d len = %d \n", __FUNCTION__, __LINE__, len);
	if (len != BufferLenght) 									//�Ѷ��������µ�β��
	{ 			
        thiz->eof = EPDK_TRUE;								//���ļ�β����־λ  
    }

	thiz->bufferTxtlen = len;	
}

static __s32 utf16big_analysis(txtEncodeParser *thiz, MkLine *line)
{
	__u16 CharWidth;
	
	int c = thiz->cur_offset;		//buffer����
	int w = 0;		//ÿ�е��ַ��ܿ��
	int l = 0;		//ÿ�а������ַ���

	GUI_LyrWinSel(thiz->hlyr);	
	GUI_SetTextMode(GUI_TM_TRANS);  
 	
	GUI_SetFont(thiz->pFont);	
	GUI_SetFontMode(GUI_FONTMODE_8BPP256);
	
	
	while( c < thiz->bufferTxtlen )
	{
		__u16 b = thiz->bufferTxt[c+1] & 0xff;	
			
		b += (thiz->bufferTxt[c]&0xff)<<8;	//��˸�ʽ����λ��ǰ, ��λ�ں�
		
		if ( (b == 13) || (b == 10))	//\r ����\r\n
		{ 			
			__u16 tmp;
			tmp  = thiz->bufferTxt[c+3] & 0xff;				
			tmp += (thiz->bufferTxt[c+2]&0xff)<<8;	//��˸�ʽ����λ��ǰ, ��λ�ں�
			
			if( tmp == 10 )
			{
				c+=4;
				l+=4;
			}
			else
			{
				c+=2;
				l+=2;
			}
			
//			tmp_line.len = l;
//			tmp_line.txt_off = thiz->start + c -l;	//ע��thiz->currentoffset�ĳ�ʼ����Ӧ��recordһ�¡�		
//			set_txt_line_value_push(lines, &tmp_line);	
			
			line->start 	= thiz->start + c -l;
			line->len   	= l;
			thiz->cur_offset = c;

			
			w = 0;
			l = 0;
			//c +=2;
			return 0;
		}
		
		if( b< 0x80)
			CharWidth = GUI_GetCharDistX(b);
		else
			CharWidth = thiz->chinese_width;
		
		if (w + CharWidth > thiz->viewWidth) 
		{
			//��ǰ���޷���ʾ����
//			tmp_line.len = l;
//			tmp_line.txt_off = thiz->start + c -l;
//			set_txt_line_value_push(lines, &tmp_line);
			
			line->start 	= thiz->start + c -l;
			line->len   	= l;
			thiz->cur_offset = c;

			w = 0;
			l = 0;
			//c +=2;
			return 0;
		} 
		else 
		{
			c += 2;
			l += 2;
			w += CharWidth;
		}				
	}
	
	thiz->cur_offset = 0;	
	
	if( l > 0 )
	{
//		tmp_line.len = l;
//		tmp_line.txt_off = thiz->start + c -l;
//		set_txt_line_value_push(lines, &tmp_line);
		
		line->start 	= thiz->start + c -l;
		line->len   	= l;		
		return -2;
	}
	
	return -1;
}

static void utf16big_dispstr(char *s, int len , int x, int y)
{
	__u16 xdist;
	__u16 b, tmp;

	int i=0;

	xdist = x;

	while( i < len )
	{
		b 	= s[i] & 0xff;
		tmp = s[i+1] & 0xff;

		tmp += b<<8;

		if( (tmp == 13) || (tmp == 10))	// ����\r\n
		{
			i+=2;
			continue;
		}

		if( tmp == 9)	tmp = 0x20;

		GUI_DispCharAt(tmp, xdist, y);
		xdist += GUI_GetCharDistX(tmp);
		i += 2;
	}
}

static void utf16big_destroy(txtEncodeParser *thiz)
{
	esMEMS_Pfree(thiz->bufferTxt, 2*BufferLenght/1024);	
	esMEMS_Mfree(0, thiz);	
}



