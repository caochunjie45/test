/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           record app sample
*
*						        	(c) Copyright 2006-2009, China
*											 All Rights Reserved
*
* File    : mbook_show.c
* By      : lyn
* Version : V1.00
*********************************************************************************************************
*/

#include "mbook_private.h"

typedef struct tag_MBOOK_SHOW
{   
    ES_FILE*            disp_hdle;      // ��ʾ�����������
    __show_config_t     config;         // ������Ϣ
}__mbook_show_t;

/*
**********************************************************************************************************************
*                                               __show_page_utf8
*
* Description: дһҳ����ͼ���ϣ�UTF8�����ʽ��
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __show_page_utf8(__mbook_show_t * hdle, __hdle lyr, void * data)
{
    __u16                   tmp;
    __u32                   tmp_w;
    __u32                   x, y;
    __u32                   row;
    __u8*                   p_data;
    __mbook_show_t*         p_show;
    __show_config_t*        p_config;

    p_data = data;
    p_show = hdle;
    p_config = &p_show->config;

	GUI_LyrWinSel(lyr);	
	
//	GUI_SetTextMode(GUI_TM_TRANS); 
	GUI_SetFont(p_config->char_font);	
//	GUI_SetFontMode(GUI_FONTMODE_8BPP256);
	GUI_SetBkColor(0x0);
	GUI_SetColor(p_config->font_color);
	GUI_Clear();	
	
	x = p_config->show_start;
    y = 0;
    row = 0;
    while(row < p_config->page_row)
    {
        if((*p_data == 0x0a)||(*p_data == 0x0d))
        {   //���з����
            row++;
		    x = p_config->show_start;
		    y = y + p_config->row_height;
            if(*(p_data + 1) == 0x0a)    
            {
                p_data = p_data + 2;  
            }
            else
            {
                p_data = p_data + 1;  
            }		    

		    continue;               
        }        
        else if((tmp == 0xff)&&((*(p_data + 1) & 0xff) == 0xfe))
        {
            break;
        }              
        else if((*p_data & 0xf0)== 0xe0)
        {   // ���ֽ�������
			tmp = ((*p_data & 0x0f) << 12) | ((*(p_data + 1) & 0x3f) << 6) | (*(p_data + 2) & 0x3f);
		    tmp_w = p_config->col_width;
		    p_data = p_data + 3;
        }
        else if((*p_data & 0xe0)== 0xc0)
        {   // ˫�ֽ�������
            tmp = ((*p_data & 0x3f) << 6) | (*(p_data + 1) & 0x3f);
		    tmp_w = p_config->col_width;
		    p_data = p_data + 2;     
        }
        else
        {   // ���ֽ�������
            tmp = *p_data & 0xff;
            tmp_w = *(p_config->char_table + tmp);
		    p_data = p_data + 1;
        }

        if((x + tmp_w) > p_config->show_width)
        {
            row++;
            if(row >= p_config->page_row)
            {
                break;
            }            
            x = p_config->show_start;
            y = y + p_config->row_height;
            GUI_DispCharAt(tmp, x, y);
            x = x + tmp_w;
        }
        else
        {
            GUI_DispCharAt(tmp, x, y);
            x = x + tmp_w;
        }
    }
}

/*
**********************************************************************************************************************
*                                               __show_page_utf16_be
*
* Description: дһҳ����ͼ���ϣ�UTF16_BE�����ʽ��
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __show_page_utf16_be(__mbook_show_t * hdle, __hdle lyr, void * data)
{
    __u16                   tmp;
    __u32                   tmp_w;
    __u32                   x, y;
    __u32                   row;
    __u8*                   p_data;
    __mbook_show_t*         p_show;
    __show_config_t*        p_config;

    p_data = data;
    p_show = hdle;
    p_config = &p_show->config;

	GUI_LyrWinSel(lyr);	
	
//	GUI_SetTextMode(GUI_TM_TRANS); 
	GUI_SetFont(p_config->char_font);	
//	GUI_SetFontMode(GUI_FONTMODE_8BPP256);
	GUI_SetBkColor(0x0);
	GUI_SetColor(p_config->font_color);
	GUI_Clear();	
      	 
    x = p_config->show_start;  	  
    y = 0;
    row = 0;
    while(row < p_config->page_row)
    {       
        tmp = ((*p_data & 0xff) << 8) + (*(p_data + 1) & 0xff);
        if((tmp == 0x0d) || (tmp == 0x0a))
        {
            row++;
		    x = p_config->show_start;
		    y = y + p_config->row_height; 
		    if((*(p_data + 3) & 0xff) == 0x0a)
		    {
		        p_data = p_data + 4;
            }
            else
            {
                p_data = p_data + 2;
            }		 
            continue;              
        }
        else if(tmp == 0xfffe)
        {
            break;
        }              
        else if(tmp < 0x80)
        {
            tmp_w = *(p_config->char_table + tmp);
            p_data = p_data + 2;                
        }
        else
        {
		    tmp_w = p_config->col_width;
		    p_data = p_data + 2;               
        }
        
        if((x + tmp_w) > p_config->show_width)
        {
            row++;
            if(row >= p_config->page_row)
            {
                break;
            }                 
            x = p_config->show_start;
            y = y + p_config->row_height;
            GUI_DispCharAt(tmp, x, y);
            x = x + tmp_w;
        }
        else
        {
            GUI_DispCharAt(tmp, x, y);
            x = x + tmp_w;
        }
    }      
}

/*
**********************************************************************************************************************
*                                               __show_page_utf16_le
*
* Description: дһҳ����ͼ���ϣ�UTF16_LE�����ʽ��
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __show_page_utf16_le(__mbook_show_t * hdle, __hdle lyr, void * data)
{
    __u16                   tmp;
    __u32                   tmp_w;
    __u32                   x, y;
    __u32                   row;
    __u8*                   p_data;
    __mbook_show_t*         p_show;
    __show_config_t*        p_config;

    p_data = data;
    p_show = hdle;
    p_config = &p_show->config;

	GUI_LyrWinSel(lyr);	
	
//	GUI_SetTextMode(GUI_TM_TRANS); 
	GUI_SetFont(p_config->char_font);	
//	GUI_SetFontMode(GUI_FONTMODE_8BPP256);
	GUI_SetBkColor(0x0);
	GUI_SetColor(p_config->font_color);
	GUI_Clear();	
    
    x = p_config->show_start;
    y = 0;
    row = 0;
    while(row < p_config->page_row)
    {       
        tmp = (*p_data & 0xff) + ((*(p_data + 1) & 0xff) << 8);
        if((tmp == 0x0d) || (tmp == 0x0a))
        {
            row++;
		    x = p_config->show_start;
		    y = y + p_config->row_height; 
		    if((*(p_data + 2) & 0xff) == 0x0a)
		    {
		        p_data = p_data + 4;
            }
            else
            {
                p_data = p_data + 2;
            }		 
            continue;              
        }
        else if(tmp == 0xfeff)
        {
            break;
        }        
        else if(tmp < 0x80)
        {
            tmp_w = *(p_config->char_table + tmp);
            p_data = p_data + 2;                
        }
        else
        {
		    tmp_w = p_config->col_width;
		    p_data = p_data + 2;               
        }
        
        if((x + tmp_w) > p_config->show_width)
        {
            row++;
            if(row >= p_config->page_row)
            {
                break;
            }                 
            x = p_config->show_start;
            y = y + p_config->row_height;
            GUI_DispCharAt(tmp, x, y);
            x = x + tmp_w;
        }
        else
        {
            GUI_DispCharAt(tmp, x, y);
            x = x + tmp_w;
        }
    }        
}

/*
**********************************************************************************************************************
*                                               __show_page_gbk
*
* Description: дһҳ����ͼ���ϣ�GBK�����ʽ��
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __show_page_gbk(__mbook_show_t * hdle, __hdle lyr, void * data)
{
    __u16                   tmp;
    __u32                   tmp_w;
    __u32                   x, y;
    __u32                   row;
    __u8*                   p_data;
    __mbook_show_t*         p_show;
    __show_config_t*        p_config;

    p_data = data;
    p_show = hdle;
    p_config = &p_show->config;

	GUI_LyrWinSel(lyr);	
	
	GUI_SetFont(p_config->char_font);	
    GUI_SetBkColor(0);
	GUI_SetColor(p_config->font_color);
	GUI_Clear();	

    x = p_config->show_start;
    y = 0;
    row = 0;
    while(row < p_config->page_row)
    {       
        tmp = *p_data & 0xff;
        if((tmp == 0x0d) || (tmp == 0x0a))
        {
            row++;
		    x = p_config->show_start;
		    y = y + p_config->row_height; 
		    if((*(p_data + 1) & 0xff) == 0x0a)
		    {
		        p_data = p_data + 2;
            }
            else
            {
                p_data = p_data + 1;
            }		 
            continue;              
        }
        else if(tmp == 0xff)
        {
            if((*(p_data + 1) & 0xff) == 0xfe)
            {
                break;
            }
            else
            {
                tmp_w = *(p_config->char_table + 0x3f);     // ?
                p_data = p_data + 1;                 
            }
        }
        else if(tmp > 0x7f)
        {
            tmp = (tmp << 8) + *(p_data + 1);
           
			{
				__u16 t;

				t = tmp;

	            tmp = eLIBs_GBK_to_Unicode(t);
				if(0xffff == tmp)
				{
					__msg("eLIBs_GBK_to_Unicode fail, t = %x\n", t);					
				}
        	}
		    tmp_w = p_config->col_width;
		    p_data = p_data + 2;               
        }
        else
        {
            tmp_w = *(p_config->char_table + tmp);
            p_data = p_data + 1;                
        }
                
        if((x + tmp_w) > p_config->show_width)
        {
            row++;
            if(row >= p_config->page_row)
            {
                break;
            }           
            x = p_config->show_start;
            y = y + p_config->row_height;
            GUI_DispCharAt(tmp, x, y);
            x = x + tmp_w;
        }
        else
        {
            GUI_DispCharAt(tmp, x, y);
            x = x + tmp_w;
        }
    }        
}

/*
**********************************************************************************************************************
*                                               __show_page_big5
*
* Description: дһҳ����ͼ���ϣ�GBK�����ʽ��
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __show_page_big5(__mbook_show_t * hdle, __hdle lyr, void * data)
{
    __u16                   tmp;
    __u32                   tmp_w;
    __u32                   x, y;
    __u32                   row;
    __u8*                   p_data;
    __mbook_show_t*         p_show;
    __show_config_t*        p_config;

    p_data = data;
    p_show = hdle;
    p_config = &p_show->config;

	GUI_LyrWinSel(lyr);	
	
//	GUI_SetTextMode(GUI_TM_TRANS); 
	GUI_SetFont(p_config->char_font);	
//	GUI_SetFontMode(GUI_FONTMODE_8BPP256);
	GUI_SetBkColor(0x0);
	GUI_SetColor(p_config->font_color);
	GUI_Clear();	
    
    x = p_config->show_start;
    y = 0;
    row = 0;
    while(row < p_config->page_row)
    {       
        tmp = *p_data & 0xff;
        if((tmp == 0x0d) || (tmp == 0x0a))
        {
            row++;
		    x = p_config->show_start;
		    y = y + p_config->row_height; 
		    if((*(p_data + 1) & 0xff) == 0x0a)
		    {
		        p_data = p_data + 2;
            }
            else
            {
                p_data = p_data + 1;
            }		 
            continue;              
        }
        else if(tmp == 0xff)
        {
            if((*(p_data + 1) & 0xff) == 0xfe)
            {
                break;
            }
            else
            {
                tmp_w = *(p_config->char_table + 0x3f);     // ?
                p_data = p_data + 1;                 
            }            
        }
        else if(tmp > 0x7f)
        {
            tmp = (tmp << 8) + *(p_data + 1);
            tmp = eLIBs_BIG5_to_Unicode(tmp);
		    tmp_w = p_config->col_width;
		    p_data = p_data + 2;               
        }
        else
        {
            tmp_w = *(p_config->char_table + tmp);
            p_data = p_data + 1;                
        }
        
        if((x + tmp_w) > p_config->show_width)
        {
            row++;
            if(row >= p_config->page_row)
            {
                break;
            }                 
            x = p_config->show_start;
            y = y + p_config->row_height;
            GUI_DispCharAt(tmp, x, y);
            x = x + tmp_w;
        }
        else
        {
            GUI_DispCharAt(tmp, x, y);
            x = x + tmp_w;
        }
    }        
}

/*
************************************************************************************************************************
*                       				MBOOK_Show_Init
*
*Description: ҳ����ʾģ��ĳ�ʼ��
*
*Arguments  : 
*            
*
*Return     : H_SHOW: ����ҳ����ʾģ��������
*
************************************************************************************************************************
*/
H_SHOW  MBOOK_Show_Init(void)
{
    __mbook_show_t*          p_show;
    
	p_show = (__mbook_show_t *)My_Malloc(0, sizeof(__mbook_show_t));	// �����ڴ�
	if(p_show == 0)
	{
		__wrn(" malloc error \n");
		return NULL;
	}
	eLIBs_memset(p_show, 0, sizeof(__mbook_show_t)); 

	p_show->disp_hdle = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");	// ����ʾ����
	if(p_show->disp_hdle == NULL)
	{
		goto SHOW_ERROR_1;
	}
	
			
	return p_show;

SHOW_ERROR_2:
	eLIBs_fclose(p_show->disp_hdle);	
	p_show->disp_hdle = NULL;

SHOW_ERROR_1:
    My_Mfree(0, p_show);
    
    return NULL;
}

/*
************************************************************************************************************************
*                       				MBOOK_Show_Config
*
*Description: ҳ����ʾģ��Ĳ�������
*
*Arguments  : hdle: �������
*             config: ������Ϣ����ָ��
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Show_Config(H_SHOW hdle, __show_config_t * config)
{
    __mbook_show_t*          p_show;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_show = (__mbook_show_t *)hdle;    

    eLIBs_memcpy( &(p_show->config), config, sizeof(__mbook_show_t));
    
    return EPDK_OK;    
}

/*
************************************************************************************************************************
*                       				MBOOK_Show_Page
*
*Description: дһҳ���ݵ�ָ������ͼ����
*
*Arguments  : hdle: �������
*             lyr: ͼ����
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Show_Page(H_SHOW hdle, __hdle lyr, void * data)
{
    __mbook_show_t*          p_show;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }
    p_show = (__mbook_show_t *)hdle;    

    switch(p_show->config.charset)
    {
        case EPDK_CHARSET_ENM_UTF8 :
    	{
        	__show_page_utf8(p_show, lyr, data);    	    
    	}   
    	    break; 
    	case EPDK_CHARSET_ENM_UTF16BE:
    	{
        	__show_page_utf16_be(p_show, lyr, data);
    	}   
    	    break;    	    
    	case EPDK_CHARSET_ENM_UTF16LE:
    	{
        	__show_page_utf16_le(p_show, lyr, data);
    	}   
    	    break;
    	case EPDK_CHARSET_ENM_GBK:
    	{
        	__show_page_gbk(p_show, lyr, data);
    	}   
    	    break;
    	case EPDK_CHARSET_ENM_BIG5:
    	{
        	__show_page_big5(p_show, lyr, data);
    	}   
    	    break;    	    	    
    	default:
    	    break;           
    }
    
    return EPDK_OK; 
}

/*
************************************************************************************************************************
*                       				MBOOK_Show_Uninit
*
*Description: ɾ��ҳ����ʾģ��
*
*Arguments  : hdle: �������
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Show_Uninit(H_SHOW hdle)
{
    __mbook_show_t*          p_show;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }        
    
	p_show = (__mbook_show_t *)hdle;

	if(p_show->disp_hdle)
	{
		eLIBs_fclose(p_show->disp_hdle);	
		p_show->disp_hdle = NULL;
	}	
	
    My_Mfree(0, p_show);   
    
    return EPDK_OK;    
}

