/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           record app sample
*
*						        	(c) Copyright 2006-2009, China
*											 All Rights Reserved
*
* File    : mbook_analysis.c
* By      : lyn
* Version : V1.00
*********************************************************************************************************
*/

#include "mbook_private.h"

typedef struct tag_MBOOK_ANALYSIS
{
    ES_FILE *           file_hdle;      // �ļ����
    __u32               block_size;     // һ��block�Ĵ�С 4 byte
    __u32               block_len;      // һ��block�Ĵ�С 4 byte
    
    __u8*               block_buf;      // block���ڴ��ַ
    __one_page_t*       page_info;      // page info���ڴ��ַ
    __one_page_t*       p_operate;      // ָ��ǰ������info
    
    __u32               total_page;     // ��ҳ��
    __u32               last_row;       // �ϴ��˳�ʱ����β���ڵ��к�
    __u32               last_start;     // �ϴ��˳�ʱ����β�����е�ƫ����
    __u32               last_end;       // �ϴ��˳�ʱ���ļ���ƫ����

    __analysis_config_t     config;         // info��������Ϣ
}__mbook_analysis_t;


/*
************************************************************************************************************************
*                       				__analysis_page_utf8
*
*Description: ��ҳ������ݣ����з������õ�����ҳ����Ϣ��
*
*Arguments  : hdle: ҳ�ṹ��˽������.  
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
static  __s32 __analysis_page_utf8(__mbook_analysis_t * hdle, __u32 len)
{
    __u32                   tmp;
    __u32                   tmp_ofs;      
    __u32                   tmp_row;
    __u32                   tmp_x;  
    __u32                   tmp_w; 
    __u8*                   p_data;
    __one_page_t*           p_one;
    __analysis_config_t*    p_config;
    
    tmp_ofs = 0;
    tmp_row = hdle->last_row;
    tmp_x   = hdle->last_start;
    p_data  = hdle->block_buf;
    p_one   = hdle->p_operate;
    p_config = &hdle->config;
    
    while(1)
    {
        tmp = *(p_data + tmp_ofs) & 0xff;   
        if((tmp == 0x0a)||(tmp == 0x0d))
        {
            tmp_row++;      // ������1
            tmp_x = 0;
            if(*(p_data + tmp_ofs + 1) == 0x0a)    
            {
                tmp_ofs = tmp_ofs + 2;
            }
            else
            {
                tmp_ofs = tmp_ofs + 1;
            }
            if(tmp_row >= p_config->page_row)
            {
                hdle->total_page++;
                p_one->page_end = hdle->last_end + tmp_ofs;
                p_one->page_no = hdle->total_page;
                (p_one + 1)->page_start = p_one->page_end;
                p_one++;        
             
                tmp_row = 0;
            }              
        }
        else if((tmp & 0xf0) == 0xe0)
        {
            tmp_w = p_config->col_width;               // ��ƫ�������ʼλ��1������ƫ����
            tmp_ofs = tmp_ofs + 3;            
        }
        else if((tmp & 0xe0) == 0xc0)
        {
            tmp_w = p_config->col_width;               // ��ƫ�������ʼλ��1������ƫ����
            tmp_ofs = tmp_ofs + 2;                
        }
        else
        {
            tmp_w = *(p_config->char_table + tmp);      // ��ƫ�������ʼλ��1���ַ�ƫ����
            tmp_ofs = tmp_ofs + 1;          
        }
        
        if((tmp_x + tmp_w) > p_config->show_width)
        {
            tmp_row++;
            tmp_x = tmp_w;
            if(tmp_row >= p_config->page_row)
            {
                hdle->total_page++;
                p_one->page_end = hdle->last_end + tmp_ofs;
                p_one->page_no  = hdle->total_page;
                (p_one + 1)->page_start = p_one->page_end;
                p_one++;        
         
                tmp_row = 0;
            }
        }
        else
        {
            tmp_x = tmp_x + tmp_w;
        }
        if((tmp_ofs + 3) >= len)
        {
            break;
        }        
    }
    
    hdle->last_end  = hdle->last_end + tmp_ofs;
    hdle->p_operate = p_one;
    
    hdle->last_row   = tmp_row;
    hdle->last_start = tmp_x; 
    
    return EPDK_OK;   
}

/*
************************************************************************************************************************
*                       				__analysis_page_utf16_be
*
*Description: ��ҳ������ݣ����з������õ�����ҳ����Ϣ��
*
*Arguments  : hdle: ҳ�ṹ��˽������.  
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
static  __s32 __analysis_page_utf16_be(__mbook_analysis_t * hdle, __u32 len)
{
    __u32                   tmp;
    __u32                   tmp_ofs;    
    __u32                   tmp_row;
    __u32                   tmp_x;  
    __u32                   tmp_w; 
    __u8*                   p_data;
    __one_page_t*           p_one;
    __analysis_config_t*    p_config;
    
    tmp_ofs = 0;
    tmp_row = hdle->last_row;
    tmp_x   = hdle->last_start;
    p_data  = hdle->block_buf;
    p_one   = hdle->p_operate;
    p_config = &hdle->config;

    while(1)
    {
        tmp = ((*(p_data + tmp_ofs) & 0xff) << 8) + (*(p_data + tmp_ofs + 1) & 0xff);
        if((tmp == 0x0d) || (tmp == 0x0a))
        {
            tmp_row++;
		    tmp_x = 0; 
		    if((*(p_data + tmp_ofs + 3) & 0xff) == 0x0a)
		    {
		        tmp_ofs = tmp_ofs + 4;
            }
            else
            {
                tmp_ofs = tmp_ofs + 2;
            }	
            if(tmp_row >= p_config->page_row)
            {
                hdle->total_page++;
                p_one->page_end = hdle->last_end + tmp_ofs;
                p_one->page_no = hdle->total_page;
                (p_one + 1)->page_start = p_one->page_end;
                p_one++;        
             
                tmp_row = 0;
            }        
        }
        else if(tmp < 0x80)
        {
            tmp_w   = *(p_config->char_table + tmp);    // ��ƫ�������ʼλ��1���ַ�ƫ����
            tmp_ofs = tmp_ofs + 2;             
        }
        else
        {
            tmp_w   = p_config->col_width;             // ��ƫ�������ʼλ��1������ƫ����
            tmp_ofs = tmp_ofs + 2;                
        }

        if((tmp_x + tmp_w) > p_config->show_width)
        {
            tmp_row++;
            tmp_x = tmp_w;
            if(tmp_row >= p_config->page_row)
            {
                hdle->total_page++;
                p_one->page_end = hdle->last_end + tmp_ofs;
                p_one->page_no  = hdle->total_page;
                (p_one + 1)->page_start = p_one->page_end;
                p_one++;        
         
                tmp_row = 0;
            }
        }
        else
        {
            tmp_x = tmp_x + tmp_w;
        }
        if((tmp_ofs + 4) >= len)
        {
            break;
        }
    }
    
    hdle->last_end  = hdle->last_end + tmp_ofs;
    hdle->p_operate = p_one;
    
    hdle->last_row = tmp_row;
    hdle->last_start = tmp_x; 
    
    return EPDK_OK;   
}

/*
************************************************************************************************************************
*                       				__analysis_page_utf16_le
*
*Description: ��ҳ������ݣ����з������õ�����ҳ����Ϣ��
*
*Arguments  : hdle: ҳ�ṹ��˽������.  
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
static  __s32 __analysis_page_utf16_le(__mbook_analysis_t * hdle, __u32 len)
{
    __u32                   tmp;
    __u32                   tmp_ofs;
    __u32                   tmp_row;
    __u32                   tmp_x;  
    __u32                   tmp_w; 
    __u8*                   p_data;
    __one_page_t*           p_one;
    __analysis_config_t*    p_config;
    
    tmp_ofs = 0;
    tmp_row = hdle->last_row;
    tmp_x   = hdle->last_start;
    p_data  = hdle->block_buf;
    p_one   = hdle->p_operate;
    p_config = &hdle->config;
    
    while(1)
    {
        tmp = (*(p_data + tmp_ofs) & 0xff) + ((*(p_data + tmp_ofs + 1) & 0xff) << 8);
        if((tmp == 0x0d) || (tmp == 0x0a))
        {
            tmp_row++;
		    tmp_x = 0; 
		    if((*(p_data + tmp_ofs + 2) & 0xff) == 0x0a)
		    {
		        tmp_ofs = tmp_ofs + 4;
            }
            else
            {
                tmp_ofs = tmp_ofs + 2;
            }	
            if(tmp_row >= p_config->page_row)
            {
                hdle->total_page++;
                p_one->page_end = hdle->last_end + tmp_ofs;
                p_one->page_no = hdle->total_page;
                (p_one + 1)->page_start = p_one->page_end;
                p_one++;        
             
                tmp_row = 0;
            }        
        }
        else if(tmp < 0x80)
        {
            tmp_w   = *(p_config->char_table + tmp);        // ��ƫ�������ʼλ��1���ַ�ƫ����
            tmp_ofs = tmp_ofs + 2;             
        }
        else
        {
            tmp_w   = p_config->col_width;                 // ��ƫ�������ʼλ��1������ƫ����
            tmp_ofs = tmp_ofs + 2;                
        }

        if((tmp_x + tmp_w) > p_config->show_width)
        {
            tmp_row++;
            tmp_x = tmp_w;
            if(tmp_row >= p_config->page_row)
            {
                hdle->total_page++;
                p_one->page_end = hdle->last_end + tmp_ofs;
                p_one->page_no  = hdle->total_page;
                (p_one + 1)->page_start = p_one->page_end;
                p_one++;        
         
                tmp_row = 0;
            }
        }
        else
        {
            tmp_x = tmp_x + tmp_w;
        }
        if((tmp_ofs + 4) >= len)
        {
            break;
        }
    }
    
    hdle->last_end  = hdle->last_end + tmp_ofs;
    hdle->p_operate = p_one;
    
    hdle->last_row = tmp_row;
    hdle->last_start = tmp_x; 
    
    return EPDK_OK;   
}

/*
************************************************************************************************************************
*                       				__analysis_page_gbk
*
*Description: ��ҳ������ݣ����з������õ�����ҳ����Ϣ��
*
*Arguments  : hdle: ҳ�ṹ��˽������.  
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
static  __s32 __analysis_page_gbk(__mbook_analysis_t * hdle, __u32 len)
{
    __u32                   tmp;
    __u32                   tmp_ofs;
    __u32                   tmp_row;
    __u32                   tmp_x;  
    __u32                   tmp_w; 
    __u8*                   p_data;
    __one_page_t*           p_one;
    __analysis_config_t*    p_config;
    
    tmp_ofs = 0;
    tmp_row = hdle->last_row;
    tmp_x   = hdle->last_start;
    p_data  = hdle->block_buf;
    p_one   = hdle->p_operate;
    p_config = &hdle->config;
    
    while(1)
    {
        tmp = *(p_data + tmp_ofs) & 0xff;
        if((tmp == 0x0d) || (tmp == 0x0a))
        {
            tmp_row++;
		    tmp_x = 0; 
		    if((*(p_data + tmp_ofs + 1) & 0xff) == 0x0a)
		    {
		        tmp_ofs = tmp_ofs + 2;
            }
            else
            {
                tmp_ofs = tmp_ofs + 1;
            }	
            if(tmp_row >= p_config->page_row)
            {
                hdle->total_page++;
                p_one->page_end = hdle->last_end + tmp_ofs;
                p_one->page_no = hdle->total_page;
                (p_one + 1)->page_start = p_one->page_end;
                p_one++;        
             
                tmp_row = 0;
            }        
        }
        else if(tmp > 0x7f)
        {
            tmp_w   = p_config->col_width;                 // ��ƫ�������ʼλ��1������ƫ����
            tmp_ofs = tmp_ofs + 2;                
        }
        else
        {
            tmp_w   = *(p_config->char_table + tmp);        // ��ƫ�������ʼλ��1���ַ�ƫ����
            tmp_ofs = tmp_ofs + 1;             
        }
        
        if((tmp_x + tmp_w) > p_config->show_width)
        {
            tmp_row++;
            tmp_x = tmp_w;
            if(tmp_row >= p_config->page_row)
            {
                hdle->total_page++;
                p_one->page_end = hdle->last_end + tmp_ofs;
                p_one->page_no  = hdle->total_page;
                (p_one + 1)->page_start = p_one->page_end;
                p_one++;        
         
                tmp_row = 0;
            }
        }
        else
        {
            tmp_x = tmp_x + tmp_w;
        }
        if((tmp_ofs + 4) >= len)
        {
            break;
        }
    }
    
    hdle->last_end  = hdle->last_end + tmp_ofs;
    hdle->p_operate = p_one;
    
    hdle->last_row = tmp_row;
    hdle->last_start = tmp_x; 
    
    return EPDK_OK;   
}

/*
************************************************************************************************************************
*                       				__analysis_page_big5
*
*Description: ��ҳ������ݣ����з������õ�����ҳ����Ϣ��
*
*Arguments  : hdle: ҳ�ṹ��˽������.  
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
static  __s32 __analysis_page_big5(__mbook_analysis_t * hdle, __u32 len)
{
    __u32                   tmp;
    __u32                   tmp_ofs;
    __u32                   tmp_row;
    __u32                   tmp_x;  
    __u32                   tmp_w; 
    __u8*                   p_data;
    __one_page_t*           p_one;
    __analysis_config_t*    p_config;
    
    tmp_ofs = 0;
    tmp_row = hdle->last_row;
    tmp_x   = hdle->last_start;
    p_data  = hdle->block_buf;
    p_one   = hdle->p_operate;
    p_config = &hdle->config;
    
    while(1)
    {
        tmp = *(p_data + tmp_ofs) & 0xff;
        if((tmp == 0x0d) || (tmp == 0x0a))
        {
            tmp_row++;
		    tmp_x = 0; 
		    if((*(p_data + tmp_ofs + 1) & 0xff) == 0x0a)
		    {
		        tmp_ofs = tmp_ofs + 2;
            }
            else
            {
                tmp_ofs = tmp_ofs + 1;
            }	
            if(tmp_row >= p_config->page_row)
            {
                hdle->total_page++;
                p_one->page_end = hdle->last_end + tmp_ofs;
                p_one->page_no = hdle->total_page;
                (p_one + 1)->page_start = p_one->page_end;
                p_one++;        
             
                tmp_row = 0;
            }        
        }
        else if(tmp > 0x7f)
        {
            tmp_w   = p_config->col_width;                 // ��ƫ�������ʼλ��1������ƫ����
            tmp_ofs = tmp_ofs + 2;                        
        }
        else
        {
            tmp_w   = *(p_config->char_table + tmp);        // ��ƫ�������ʼλ��1���ַ�ƫ����
            tmp_ofs = tmp_ofs + 1;             
        }
                
        if((tmp_x + tmp_w) > p_config->show_width)
        {
            tmp_row++;
            tmp_x = tmp_w;
            if(tmp_row >= p_config->page_row)
            {
                hdle->total_page++;
                p_one->page_end = hdle->last_end + tmp_ofs;
                p_one->page_no  = hdle->total_page;
                (p_one + 1)->page_start = p_one->page_end;
                p_one++;        
         
                tmp_row = 0;
            }
        }
        else
        {
            tmp_x = tmp_x + tmp_w;
        }
        if((tmp_ofs + 4) >= len)
        {
            break;
        }
    }
    
    hdle->last_end  = hdle->last_end + tmp_ofs;
    hdle->p_operate = p_one;
    
    hdle->last_row = tmp_row;
    hdle->last_start = tmp_x; 
    
    return EPDK_OK;   
}

/*
************************************************************************************************************************
*                       				__analysis_page_info_utf8
*
*Description: ��ҳ������ݣ����з������õ�����ҳ����Ϣ��
*
*Arguments  : hdle: ҳ�ṹ��˽������.  
*              
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
static __s32 __analysis_page_info_utf8(__mbook_analysis_t * hdle)
{
    __u32                   len;
    __mbook_analysis_t*     p_analysis;
    
    p_analysis = hdle;     
    while(1)
    {
        // ��λ�ļ�ָ��
        eLIBs_fseek(p_analysis->file_hdle, p_analysis->last_end, ELIBS_SEEK_SET); 
        // ��һ������
        len = eLIBs_fread(p_analysis->block_buf, 1, p_analysis->block_len, p_analysis->file_hdle);
    
        __analysis_page_utf8(hdle, len);
             
        if(len < p_analysis->block_len)
        {  
            p_analysis->total_page++;
            p_analysis->p_operate->page_no = p_analysis->total_page;
            p_analysis->p_operate->page_end = eLIBs_ftell(p_analysis->file_hdle);

            break;
        }
    }
    p_analysis->p_operate = p_analysis->page_info;
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				__analysis_page_info_utf16_be
*
*Description: ��ҳ������ݣ����з������õ�����ҳ����Ϣ��
*
*Arguments  : hdle: ҳ�ṹ��˽������.  
*              
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
static __s32 __analysis_page_info_utf16_be(__mbook_analysis_t * hdle)
{
    __u32                   len;
    __mbook_analysis_t*     p_analysis;
    
    p_analysis = hdle; 
    while(1)
    {
        // ��λ�ļ�ָ��
        eLIBs_fseek(p_analysis->file_hdle, p_analysis->last_end, ELIBS_SEEK_SET); 
        // ��һ������
        len = eLIBs_fread(p_analysis->block_buf, 1, p_analysis->block_len, p_analysis->file_hdle);
      
        __analysis_page_utf16_be(hdle, len);
             
        if(len < p_analysis->block_len)
        {  
            p_analysis->total_page++;
            p_analysis->p_operate->page_no = p_analysis->total_page;
            p_analysis->p_operate->page_end = eLIBs_ftell(p_analysis->file_hdle);

            break;
        }
    }
    p_analysis->p_operate = p_analysis->page_info;
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				__analysis_page_info_utf16_le
*
*Description: ��ҳ������ݣ����з������õ�����ҳ����Ϣ��
*
*Arguments  : hdle: ҳ�ṹ��˽������.  
*              
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
static __s32 __analysis_page_info_utf16_le(__mbook_analysis_t * hdle)
{
    __u32                   len;
    __mbook_analysis_t*     p_analysis;
    
    p_analysis = hdle;
    while(1)
    {
        // ��λ�ļ�ָ��
        eLIBs_fseek(p_analysis->file_hdle, p_analysis->last_end, ELIBS_SEEK_SET); 
        // ��һ������
        len = eLIBs_fread(p_analysis->block_buf, 1, p_analysis->block_len, p_analysis->file_hdle);
        
        __analysis_page_utf16_le(hdle, len);
             
        if(len < p_analysis->block_len)
        {  
            p_analysis->total_page++;
            p_analysis->p_operate->page_no = p_analysis->total_page;
            p_analysis->p_operate->page_end = eLIBs_ftell(p_analysis->file_hdle);

            break;
        }
    }
    p_analysis->p_operate = p_analysis->page_info;
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				__analysis_page_info_gbk
*
*Description: ��ҳ������ݣ����з������õ�����ҳ����Ϣ��
*
*Arguments  : hdle: ҳ�ṹ��˽������.  
*              
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
static __s32 __analysis_page_info_gbk(__mbook_analysis_t * hdle)
{
    __u32                   len;
    __mbook_analysis_t*     p_analysis;
    
    p_analysis = hdle;
    while(1)
    {
        // ��λ�ļ�ָ��
        eLIBs_fseek(p_analysis->file_hdle, p_analysis->last_end, ELIBS_SEEK_SET); 
        // ��һ������
        len = eLIBs_fread(p_analysis->block_buf, 1, p_analysis->block_len, p_analysis->file_hdle);
        
        __analysis_page_gbk(hdle, len);
             
        if(len < p_analysis->block_len)
        {  
            p_analysis->total_page++;
            p_analysis->p_operate->page_no = p_analysis->total_page;
            p_analysis->p_operate->page_end = eLIBs_ftell(p_analysis->file_hdle);

            break;
        }
    }
    p_analysis->p_operate = p_analysis->page_info;
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				__analysis_page_info_big5
*
*Description: ��ҳ������ݣ����з������õ�����ҳ����Ϣ��
*
*Arguments  : hdle: ҳ�ṹ��˽������.  
*              
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
static __s32 __analysis_page_info_big5(__mbook_analysis_t * hdle)
{
    __u32                   len;
    __mbook_analysis_t*     p_analysis;
    
    p_analysis = hdle;
    while(1)
    {
        // ��λ�ļ�ָ��
        eLIBs_fseek(p_analysis->file_hdle, p_analysis->last_end, ELIBS_SEEK_SET); 
        // ��һ������
        len = eLIBs_fread(p_analysis->block_buf, 1, p_analysis->block_len, p_analysis->file_hdle);
        
        __analysis_page_big5(hdle, len);
             
        if(len < p_analysis->block_len)
        {  
            p_analysis->total_page++;
            p_analysis->p_operate->page_no = p_analysis->total_page;
            p_analysis->p_operate->page_end = eLIBs_ftell(p_analysis->file_hdle);

            break;
        }
    }
    p_analysis->p_operate = p_analysis->page_info;
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				MBOOK_Analysis_Init
*
*Description: ��ʼ��ҳ�����ģ��
*
*Arguments  : fd: �ļ��������.
*             len: �ļ��ܳ���    
*
*Return     : H_ANALYSIS: ����PAGE�������
*
************************************************************************************************************************
*/
H_ANALYSIS   MBOOK_Analysis_Init(ES_FILE * fd)
{
    __mbook_analysis_t*     p_analysis;
    
	p_analysis = (__mbook_analysis_t *)My_Malloc(0, sizeof(__mbook_analysis_t));	// �����ڴ�
	if(p_analysis == 0)
	{
		__err(" malloc error \n");
		return NULL;
	}
	eLIBs_memset(p_analysis, 0, sizeof(__mbook_analysis_t));
	
	p_analysis->file_hdle = fd;
	p_analysis->block_size = 1 * 1024;
	p_analysis->block_len  = 1 * 1024 * 1024;
	
    p_analysis->block_buf = (void *)My_Palloc(p_analysis->block_size, 0);		// �����ڴ�	  
	if(p_analysis->block_buf == 0)
	{
		__err(" balloc error \n");
		goto ERROR_1;
	}   
	
    p_analysis->page_info = (void *)My_Palloc((sizeof(__one_page_t) * MAX_SHOW_PAGE + 1023) / 1024, 0);		// �����ڴ�	2 M  
	if(p_analysis->page_info == 0)
	{
		__err(" balloc error \n");
		goto ERROR_2;
	} 	
	p_analysis->p_operate = p_analysis->page_info;
	p_analysis->p_operate->page_no = 0;
	p_analysis->p_operate->page_start = 0;
	p_analysis->last_end    = 0;    	    
   
    return p_analysis;
    
ERROR_2:
    if(p_analysis->block_buf)
    {
        My_Pfree(p_analysis->block_buf, p_analysis->block_size);
        p_analysis->block_buf = NULL;
    }
    
ERROR_1:
    if(p_analysis)
    {
        My_Mfree(0, p_analysis);
        p_analysis = NULL;
    }

    return NULL; 
}

/*
************************************************************************************************************************
*                       				MBOOK_Analysis_Config
*
*Description: ����ҳ�����ģ����ز���
*
*Arguments  : hdle: �������  
*             config: ����������Ϣָ��
*        
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Analysis_Config(H_ANALYSIS hdle, __analysis_config_t * config)
{
    __mbook_analysis_t*     p_analysis;
   
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_analysis = (__mbook_analysis_t *)hdle;
   
    eLIBs_memcpy(&(p_analysis->config), config, sizeof(__analysis_config_t));
    
    p_analysis->last_row    = 0;
    p_analysis->last_start  = 2 * p_analysis->config.col_width;
    switch(p_analysis->config.charset)
    {
        case EPDK_CHARSET_ENM_UTF8 :
    	{
        	p_analysis->p_operate->page_start = 3;
        	p_analysis->last_end    = 3;    	    
    	}   
    	    break; 
    	case EPDK_CHARSET_ENM_UTF16BE:
    	case EPDK_CHARSET_ENM_UTF16LE:
    	{
        	p_analysis->p_operate->page_start = 2;
        	p_analysis->last_end    = 2;
    	}   
    	    break;
    	default:
    	{
        	p_analysis->p_operate->page_start = 0;
        	p_analysis->last_end    = 0; 
    	}   
    	    break;    	        
    }

    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				MBOOK_Analysis_Work
*
*Description: ������ǰ�ļ���ҳ����Ϣ
*
*Arguments  : hdle: �������  
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Analysis_Work(H_ANALYSIS hdle)
{
    __mbook_analysis_t*     p_analysis;
   
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_analysis = (__mbook_analysis_t *)hdle;
    p_analysis->total_page  = 0;
    p_analysis->p_operate   = p_analysis->page_info;
    p_analysis->p_operate->page_no = 0;
    p_analysis->p_operate->page_end = eLIBs_ftell(p_analysis->file_hdle); 
    switch(p_analysis->config.charset)
    {
        case EPDK_CHARSET_ENM_UTF8 :
    	{
        	__analysis_page_info_utf8(p_analysis);	    
    	}   
    	    break; 
    	case EPDK_CHARSET_ENM_UTF16BE:
    	{
        	__analysis_page_info_utf16_be(p_analysis);	    	    
    	}   
    	    break;
    	case EPDK_CHARSET_ENM_UTF16LE:
    	{
        	__analysis_page_info_utf16_le(p_analysis);
    	}   
    	    break;
    	case EPDK_CHARSET_ENM_GBK:
    	{
        	__analysis_page_info_gbk(p_analysis);
    	}   
    	    break;
    	case EPDK_CHARSET_ENM_BIG5:
    	{
        	__analysis_page_info_big5(p_analysis);
    	}   
    	    break;    	        	    
    	default: 
    	    break;    	        
    }   

    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				MBOOK_Analysis_GetInfo
*
*Description: ��ȡһҳ��ҳ����Ϣ��
*
*Arguments  : hdle: �������.
*             page_no: Ҫ��ȡ��ҳ���
*             page_info: ҳ����Ϣ��ŵĵ�ַ    
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Analysis_GetInfo(H_ANALYSIS hdle, __u32 page_no, __one_page_t * page_info)
{
    __one_page_t*           p_one;
    __mbook_analysis_t*     p_analysis;

    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_analysis = (__mbook_analysis_t *)hdle;
    if((page_no == 0)||(page_no > p_analysis->total_page))
    {
        return EPDK_FAIL;
    }
    p_one = p_analysis->page_info + page_no - 1;
    eLIBs_memcpy(page_info, p_one, sizeof(__one_page_t));
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				MBOOK_Analysis_GetPage
*
*Description: ��ȡָ��ƫ������Ӧ��ҳ��š�
*
*Arguments  : hdle: �������.
*             offset: �ļ�ƫ����        
*
*Return     : total_page: �ɹ������ص�ǰ�ļ�ҳ��
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Analysis_GetPage(H_ANALYSIS hdle, __u32 offset)
{
    __u32                   i;
    __one_page_t*           p_one=NULL;
    __mbook_analysis_t*     p_analysis;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }
    p_analysis = (__mbook_analysis_t *)hdle;   
    
    if(offset < 10)
    {
        return 1;
    }
    if(offset >= ((p_analysis->page_info + p_analysis->total_page - 1)->page_end))
    {
        return 1;
    }
    for(i = 0; i < p_analysis->total_page; i++)
    {
        p_one = p_analysis->page_info + i;
        if((p_one->page_start <= offset)&&(offset < p_one->page_end))
        {
            break;
        }
    }    
        
    return p_one->page_no;    
}

/*
************************************************************************************************************************
*                       				MBOOK_Analysis_GetTotalPage
*
*Description: ɾ��ҳ�����ģ��
*
*Arguments  : hdle: �������.    
*
*Return     : total_page: �ɹ������ص�ǰ�ļ���ҳ��
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Analysis_GetTotalPage(H_ANALYSIS hdle)
{
    __mbook_analysis_t*     p_analysis;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_analysis = (__mbook_analysis_t *)hdle;
        
    return p_analysis->total_page;    
}

/*
************************************************************************************************************************
*                       				MBOOK_Analysis_Uninit
*
*Description: ɾ��ҳ�����ģ��
*
*Arguments  : hdle: �������.    
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Analysis_Uninit(H_ANALYSIS hdle)
{
    __mbook_analysis_t*     p_analysis;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }
    p_analysis = (__mbook_analysis_t *)hdle;
    if(p_analysis->block_buf)
    {
        My_Pfree(p_analysis->block_buf, p_analysis->block_size);
        p_analysis->block_buf = NULL;
    }
    if(p_analysis->page_info)
    {
        My_Pfree(p_analysis->page_info, (sizeof(__one_page_t) * MAX_SHOW_PAGE + 1023) / 1024);
        p_analysis->page_info = NULL;
        p_analysis->p_operate = NULL;
    }
    
    My_Mfree(0, p_analysis);   
    
    return EPDK_OK;
}



