/*
**************************************************************************************************************
*											        ePOS
*						           the Easy Portable/Player Operation System
*									          update sub-system
*
*						             (c) Copyright 2008, Andy.zhang China
*										  	  All Rights Reserved
*
* File		: utf8_parser.c
* Version	: V1.0 
* Data		: 2009-5-02
* By		: Andy.zhang
* Biref		: utf8�������
**************************************************************************************************************
*/
#include "monkey_i.h"
#include "..\parser_plugin_i.h"
#include "utf8_parser.h"

static void utf8_config(txtEncodeParser *thiz, ParserConfig *pconfig);
static void utf8_start(txtEncodeParser *thiz, __s32 startoffset);
static void utf8_next(txtEncodeParser *thiz, int current_start);
static void utf8_prev(txtEncodeParser *thiz, int current_start);
static __s32 utf8_analysis(txtEncodeParser *thiz, MkLine *lines);
static void utf8_dispstr(char *s, int len , int x, int y);
static void utf8_destroy(txtEncodeParser *thiz);

txtEncodeParser *utf8_parserNew(void)
{
	txtEncodeParser *utf8_parser;
	
	utf8_parser = esMEMS_Malloc(0, sizeof(txtEncodeParser));
	if(!utf8_parser)
	{
		__inf("%s %d : utf8_parser_new malloc error \n", __FUNCTION__, __LINE__);
		return NULL;
	}
	eLIBs_memset(utf8_parser, 0, sizeof(txtEncodeParser));
	
	utf8_parser->config 		= utf8_config;
	utf8_parser->start_seek  	= utf8_start;
	utf8_parser->next			= utf8_next;
	utf8_parser->prev			= utf8_prev;
	utf8_parser->analysis		= utf8_analysis;
	utf8_parser->dispstr		= utf8_dispstr;
	utf8_parser->destroy		= utf8_destroy;
	utf8_parser->bufferTxt		= esMEMS_Palloc(2*BufferLenght/1024, 0);	//����2*8��ҳ�棬ÿ��ҳ���С1kbytesbufferTxt
	return utf8_parser;
}


void utf8_config(txtEncodeParser *thiz, ParserConfig *config)
{
	__u16 tmp_chinese;
	
	config->p_dispstr   = 	thiz->dispstr;
	
	thiz->fd			=	config->fd;
	thiz->txtLength 	= 	config->len;	
	thiz->viewWidth		=	config->xScrWidth;
	//thiz->viewHeight	=	config->yScrHeight;	
	//thiz->lineofpage	=	config->lineofpage;
	//thiz->FontHeight	=	config->FontHeight;	
	thiz->bufferTxtlen	=	0;	
	thiz->start			= 	config->startOffset;
	
	if( thiz->start <= 3 )	
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

/*
** fseek the start position of a word
** arg startoffset may be the middle part of a word
*/
void utf8_start(txtEncodeParser *thiz, int startoffset)
{
	__u8 *buff;
	int len, i;

	buff  = thiz->bufferTxt + BufferLenght;							//������ָ��

	if( startoffset <= 3 )
	{
		thiz->bof			=	EPDK_TRUE;
		thiz->start			=	3;
		eLIBs_fseek(thiz->fd, thiz->start , ELIBS_SEEK_SET);		
		return;
	}
	
	thiz->bof				=	EPDK_FALSE;
	
	eLIBs_fseek(thiz->fd, startoffset , ELIBS_SEEK_SET);
	len = eLIBs_fread(buff, sizeof(char), BufferLenght, thiz->fd);
	
	if( len != BufferLenght )
	{
		thiz->eof	= EPDK_TRUE;
	}

	i = 0;
	
	while( i<len )
	{		
		if( (buff[i] & 0xc0) == 0x80)
			i++;
		else
			break;		
	}
	
	if(i == len )
		thiz->start = startoffset;
	else
		thiz->start = startoffset + i;	
		
	return;	
}

void utf8_next(txtEncodeParser *thiz, int current_start)
{
	int len, end;
	
	thiz->start 		 = current_start;
	
	thiz->bof			 = (current_start < 3 )?EPDK_TRUE : EPDK_FALSE;
	
	thiz->bufferTxtlen 	 = 0;										//�ļ����ݻ�����0
	
	eLIBs_fseek( thiz->fd, thiz->start, ELIBS_SEEK_SET);					
	len = eLIBs_fread(thiz->bufferTxt, sizeof(char), BufferLenght, thiz->fd);		
	
	__inf("%s %d len = %d \n", __FUNCTION__, __LINE__, len);

    if (len != BufferLenght) 										//�Ѷ��������µ�β��
	{ 			
        thiz->eof = EPDK_TRUE;									//���ļ�β����־λ       
		thiz->bufferTxtlen = len;	
        return;
    }
	
    thiz->eof = EPDK_FALSE;										//ɾ�������������һ���س��Ժ���ַ�
    end = len;
    
	while(end > 0)
	{
		int v = thiz->bufferTxt[end - 1] & 0xff;
		
		if ( (v & 0xc0) == 0x80  ) 			// 1100 0000				
			end--;
		else
			break;	
	}
	
	if( 0 == end )													// û�ҵ�\n ����
	{		
		thiz->bufferTxtlen = len;							
		return;		
	}	

	thiz->bufferTxtlen = end;	
	return;	
}


void utf8_prev(txtEncodeParser *thiz, int current_start)
{
	int start, tmp;
	int len;
	__u8 *b;

	start 		= current_start - BufferLenght;
	thiz->eof 	= EPDK_FALSE;

	b 			= thiz->bufferTxt + BufferLenght;

	if(start <= 3)
	{
		start = 3;
		thiz->bof = EPDK_TRUE;

		eLIBs_fseek( thiz->fd, start, ELIBS_SEEK_SET);
		len = eLIBs_fread(thiz->bufferTxt, sizeof(char), current_start, thiz->fd);
		
		thiz->bufferTxtlen = current_start-3;
		thiz->start = 3;
		return;
	}	
	
	
	eLIBs_fseek( thiz->fd, start, ELIBS_SEEK_SET);
	len = eLIBs_fread(b, sizeof(char), BufferLenght, thiz->fd);	
	
	if (len != BufferLenght) 								//�Ѷ��������µ�β��
	{ 			
        thiz->eof = EPDK_TRUE;								//���ļ�β����־λ       
		thiz->bufferTxtlen = len;	        
    }

	tmp = 0;
	while (tmp < len) 
	{
		int v = b[tmp++] & 0xff;
		if ((v & 0xc0) != 0x80) 			
		{ 
			break;
		}
	}
	
	if (tmp >= len - 1)			
	{ 
		tmp = 0;
	}
		
	eLIBs_memcpy(thiz->bufferTxt, b+tmp, len-tmp+1);
	thiz->bufferTxtlen	= len-tmp;
	thiz->start		 	= start + tmp;	
}

/* 
 * return : 0: cache analysis is not finished;
 *		   -1: cache analysis is finished;
 */

static __s32 utf8_analysis(txtEncodeParser *thiz, MkLine *line)
{
	__u16 CharDist, CharWidth;
	
	int c = thiz->cur_offset;		//buffer����
	int w = 0;						//ÿ�е��ַ��ܿ��
	int l = 0;						//ÿ�а������ַ���

	GUI_LyrWinSel(thiz->hlyr);	
	GUI_SetTextMode(GUI_TM_TRANS);
       
	GUI_SetFont(thiz->pFont);	
	GUI_SetFontMode(GUI_FONTMODE_8BPP256);

		
	while( c < thiz->bufferTxtlen )
	{
		__u16 b = thiz->bufferTxt[c] & 0xff;		

		if ( (b == 13) || (b == 10))	//\r ����\r\n
		{ 
			__u16 tmp = thiz->bufferTxt[c+1] & 0xff;
			if(tmp == 10)
			{
				c+=2;
				l+=2;
			}
			else
			{
				c++;
				l++;
			}
			//tmp_line.len = l;
			//tmp_line.txt_off = thiz->start + c -l;	//ע��thiz->currentoffset�ĳ�ʼ����Ӧ��recordһ�¡�		
			//set_txt_line_value_push(lines, &tmp_line);	
			
			line->start 	= thiz->start + c -l;
			line->len   	= l;
			thiz->cur_offset = c;
			
			w = 0;			
			l = 0;
			return 0;
		}
		
		if ( (b & 0x80) == 0)				/* Single byte (ASCII)  */
		{ 
			if(b == 9)	b = 0x20;			//tab->�ո�	
				
			CharDist 	= b;			
			CharWidth 	= GUI_GetCharDistX(CharDist);	
			
			if (w + CharWidth > thiz->viewWidth) 
			{	
				//��ǰ���޷���ʾ����
				//tmp_line.len = l;
				//tmp_line.txt_off = thiz->start + c -l;
				//set_txt_line_value_push(lines, &tmp_line);
				line->start 	= thiz->start + c -l;
				line->len   	= l;
				thiz->cur_offset = c;
				
				w = 0;
				l = 0;
				return 0;
			} 
			else 
			{
				c++;
				l++;
				w += CharWidth;
			}
		} 
		else if( (b & 0xe0)== 0xc0 )				/* Double byte sequence */	
		{ 			
			/*
			CharDist = (b & 0x1f) << 6;
		    b  = thiz->bufferTxt[c+1] & 0xff;
		    b &= 0x3f;
		    CharDist |= b;	
		    */
		    
		    //CharWidth 	= GUI_GetCharDistX(CharDist);
		    
		    CharWidth = thiz->chinese_width	;
		    
		    if (w + CharWidth > thiz->viewWidth) 
			{	
				//��ǰ���޷���ʾ����
//				tmp_line.len = l;
//				tmp_line.txt_off = thiz->start + c -l;
//				set_txt_line_value_push(lines, &tmp_line);

				line->start 	= thiz->start + c -l;
				line->len   	= l;
				thiz->cur_offset = c;
				
				w = 0;
				l = 0;
				return 0;
			} 
			else 
			{
				c+=2;
				l+=2;
				w += CharWidth;
			}		    
		}							
		else if( (b & 0xf0) == 0xe0 )		/* 3 byte sequence    */
		{
			/*
			CharDist = (b & 0x0f) << 12;
		    b = thiz->bufferTxt[c+1] & 0xff;;
		    b &= 0x3f;
		    CharDist |= (b << 6);
		    b = thiz->bufferTxt[c+2] & 0xff;
		    b &= 0x3f;
		    CharDist |= b;
		    */
		    
		    //CharWidth 	= GUI_GetCharDistX(CharDist);
		    
		    CharWidth = thiz->chinese_width	;
		    
		    if (w + CharWidth > thiz->viewWidth) 
			{	
				//��ǰ���޷���ʾ����
//				tmp_line.len = l;
//				tmp_line.txt_off = thiz->start + c -l;
//				set_txt_line_value_push(lines, &tmp_line);
				
				line->start 	= thiz->start + c -l;
				line->len   	= l;
				thiz->cur_offset = c;
				
				w = 0;
				l = 0;
				return 0;
			} 
			else 
			{
				c+=3;
				l+=3;
				w += CharWidth;
			}
		    
		}
		else		//��֧�ֵı���
		{
			c++;			
			b = thiz->bufferTxt[c];
			while( (b & 0xc0) == 0x80)
			{
				c++;				
				b = thiz->bufferTxt[c];
			} 
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

static void utf8_dispstr(char *s, int len, int x, int y)
{
	int xdist;
	__u16 b, tmp;

	int i=0;

	xdist = x;

	while( i < len )
	{
		b = s[i] & 0xff;

		if ( (b & 0x80) == 0)	/* Single byte (ASCII)  */
		{
			if(b == 9)	b = 0x20;		//tab->�ո�

			GUI_DispCharAt(b, xdist, y);
			xdist += GUI_GetCharDistX(b);
			i++;
		}
		else if( (b & 0xe0)== 0xc0 )	/* Double byte sequence */
		{
			tmp = (b & 0x1f) << 6;
		    b  = s[i+1] & 0xff;
		    b &= 0x3f;
		    tmp |= b;

		    GUI_DispCharAt(tmp, xdist, y);
		    xdist += GUI_GetCharDistX(tmp);
		    i+=2;
		}
		else if( (b & 0xf0) == 0xe0 )
		{
			tmp = (b & 0x0f) << 12;
		    b = s[i+1] & 0xff;;
		    b &= 0x3f;
		    tmp |= (b << 6);
		    b = s[i+2] & 0xff;
		    b &= 0x3f;
		    tmp |= b;

		    GUI_DispCharAt(tmp, xdist, y);
		    xdist += GUI_GetCharDistX(tmp);
		    i+=3;
		}
		else
		{
			i++;
			b = s[i] & 0xff;
			while( (b & 0xc0) == 0x80)
			{
				i++;
				b = s[i];
			}
		}
	}
}

void utf8_destroy(txtEncodeParser *thiz)
{
	esMEMS_Pfree(thiz->bufferTxt, 2*BufferLenght/1024);	
	esMEMS_Mfree(0, thiz);	
}






