/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           record app sample
*
*						        	(c) Copyright 2006-2010, China
*											 All Rights Reserved
*
* File    : listbar.c
* By      : lyn
* Version : V1.00
*********************************************************************************************************
*/

#include "listbar_private.h"

typedef	enum tag_DECODE_MODE
{
	DECODE_NULL = 0,								// �������κ�ҳ			
	DECODE_CURR,									// ���뵱ǰҳ			
	DECODE_PREV,									// ���뵱ǰҳ��ǰ��ҳ	
	DECODE_NEXT,									// ��������ҳ�ĺ���ҳ	
	DECODE_ITEM,                                    // ֻ��ʾ��ǰҳ
}__decode_mode_e;

typedef struct tag_ONE_PAGE
{
    __s32       page_sta;                           // ҳ�����ɹ���ʾ��1 �ɹ��� 0 ʧ��
    __s32       page_no;                            // �������ҳ���
    H_WIN       page_lyr;                           // ����ҳ��Ӧ����ͼ��    
}__one_page_t;

typedef struct  tag_LISTBAR
{
    H_WIN                   fwin_hdle;              // ���ڲ������
    H_WIN                   lwin_hdle;              // MENU����ͼ����

    __one_page_t            lbar_page[PAGE_NUM];    // ����ҳ��
    // ���Ʋ���       
    RECT                    lbar_rect;              // MENU���ڴ������� 
    
    __s32                   lbar_page_num;          // ��ҳ��������Ŀ��
    __s32                   lbar_page_row;          // ��
    __s32                   lbar_page_col;          // ��
    __s32                   lbar_page_width;        // ��Чҳ��
    __s32                   lbar_page_height;       // ��Чҳ��

    __s32                   lbar_current_move;      // ��ǰ�ƶ�����    
    __s32                   lbar_current_page;      // ��ǰ��ʾҳ���
    __s32                   lbar_current_total;     // ��ǰ����Ŀ��
    __s32                   lbar_current_focus;     // ��ǰ������Ŀ��
    
    __s32                   lbar_item;              // ������Ŀ����ʾҳ��ı�ţ���0��ʼ��
    __s32                   lbar_move;              // ҳ���ƶ�ָʾ
    
    __s32                   lbar_focus_sta;         // ������Ϣ״̬λ
    __s32                   lbar_touch_sta;         // ������Ϣ״̬λ
    __pos_t                 lbar_last_pos;          // ������Ϣ��λ��
    __s32                   lbar_last_direct;       // �����ķ���

    // Ԥ�ⲿ��
   	__s32				    lbar_cnt;               // Ԥ����Ŀ��
    __decode_mode_e         lbar_dec_state;         // Ԥ���߳�״̬
    __u8				    lbar_dec_tsk;           // Ԥ���߳�
    __krnl_event_t*		    lbar_dec_sem;           // Ԥ������ź���
        
    // ��ɢ�߳�
    __s32                   lbar_dif_page[PAGE_NUM * ITEM_NUM];     // �洢������ҳ
    __s32                   lbar_dif_num;           // ������ʼ����
    __u8				    lbar_dif_tsk;           // Ԥ���߳�	
    __krnl_event_t*		    lbar_dif_sem;           // Ԥ������ź���	
    
    // ���ƻص������ò���	
    H_WIN                   item_layer;             // item����ͼ��
    __draw_item             lbar_draw;	            // ���ƺ���
	__listbar_config_t      config;                 // ������Ϣ
	
	// ����������
	H_WIN                   scroll_b_layer;         // ��������ͼ��
	H_WIN                   scroll_f_layer;         // ǰ������ͼ��
	__u32                   scroll_state;           // ������״̬
	__scroll_bar_t          scroll_para;            // ����������
	
	// �����߳�
	__s32                   lbar_sld_speed;         // �����ٶ�
	__s32                   lbar_sld_state;         // ����״̬
    __u8				    lbar_sld_tsk;           // �����߳�	
    __krnl_event_t*		    lbar_sld_sem;           // ���������ź���
}__listbar_t;


/*
**********************************************************************************************************************
*                                               __listbar_layer_create
*
* Description:
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static H_WIN __listbar_layer_create(__u32 width, __u32 height, __s32 pipe)
{
	FB  fb =
	{
	    {0, 0},                                   		/* size      */
	    {0, 0, 0},                                      /* buffer    */
	    {FB_TYPE_RGB, {PIXEL_COLOR_ARGB8888, 0, 0}},    /* fmt       */
	};
	__disp_layer_para_t para =
	{
	    DISP_LAYER_WORK_MODE_NORMAL,                    /* mode      */
	    0,                                              /* ck_mode   */
	    0,                                              /* alpha_en  */
	    0xff,                                           /* alpha_val */
	    1,                                              /* pipe      */
	    0xff,                                           /* prio      */
	    {0, 0, 0, 0},                           		/* screen    */
	    {0, 0, 0, 0},                               	/* source    */
	    DISP_LAYER_OUTPUT_CHN_DE_CH1,                   /* channel   */
	    NULL                                            /* fb        */
	};
		
	__layerwincreate_para_t create_info = 
	{
		"layer",
		NULL,
		GUI_LYRWIN_STA_SLEEP,
		0
	};
		
	fb.size.width  = width;
	fb.size.height = height;

	para.pipe			= pipe;
	
	para.scn_win.x 		= 0;
	para.scn_win.y 		= 0;
	para.scn_win.width 	= width;	
	para.scn_win.height = height;	

	para.src_win.x 		= 0;
	para.src_win.y 		= 0;
	para.src_win.width 	= width;	
	para.src_win.height = height;	

	para.fb = &fb;
	create_info.lyrpara = &para;	
	
	return (GUI_LyrWinCreate(&create_info));
}

/*
**********************************************************************************************************************
*                                               __listbar_brush_page
*
* Description: ˢ�»����е�ҳ������
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __listbar_brush_page(__listbar_t * hdle, __decode_mode_e mode)
{
    __s32               i;
    __one_page_t        tmp_page;
	__listbar_t*	    p_listbar;
    
    p_listbar = hdle;    
       
	if(mode == DECODE_PREV)
	{
		tmp_page.page_no	= p_listbar->lbar_page[PAGE_NUM - 1].page_no;
		tmp_page.page_lyr	= p_listbar->lbar_page[PAGE_NUM - 1].page_lyr;
		for(i = PAGE_NUM - 1; i > 0; i--)
		{
			p_listbar->lbar_page[i].page_sta	= p_listbar->lbar_page[i - 1].page_sta;
			p_listbar->lbar_page[i].page_no	    = p_listbar->lbar_page[i - 1].page_no;
			p_listbar->lbar_page[i].page_lyr	= p_listbar->lbar_page[i - 1].page_lyr;
		}
		p_listbar->lbar_page[0].page_sta	= 0;
		p_listbar->lbar_page[0].page_no	    = tmp_page.page_no;
		p_listbar->lbar_page[0].page_lyr	= tmp_page.page_lyr;
		
		for(i = p_listbar->lbar_page_num * PAGE_NUM - 1; i >= p_listbar->lbar_page_num; i--)
		{
		    p_listbar->lbar_dif_page[i] = p_listbar->lbar_dif_page[i - p_listbar->lbar_page_num];
		}
		for(i = p_listbar->lbar_page_num - 1; i >= 0; i--)
		{
		    p_listbar->lbar_dif_page[i] = -1;
		}		
		p_listbar->lbar_dif_num = p_listbar->lbar_page_num + p_listbar->lbar_item - 1;
	}
	else if(mode == DECODE_NEXT)
	{
		tmp_page.page_no	= p_listbar->lbar_page[0].page_no; 
		tmp_page.page_lyr	= p_listbar->lbar_page[0].page_lyr;
		for(i = 0; i < PAGE_NUM - 1; i++)
		{
			p_listbar->lbar_page[i].page_sta	= p_listbar->lbar_page[i + 1].page_sta; 
			p_listbar->lbar_page[i].page_no	    = p_listbar->lbar_page[i + 1].page_no; 
			p_listbar->lbar_page[i].page_lyr	= p_listbar->lbar_page[i + 1].page_lyr; 
		}
		p_listbar->lbar_page[PAGE_NUM - 1].page_sta	= 0;
		p_listbar->lbar_page[PAGE_NUM - 1].page_no	= tmp_page.page_no; 
		p_listbar->lbar_page[PAGE_NUM - 1].page_lyr	= tmp_page.page_lyr;
		
		for(i = 0; i < p_listbar->lbar_page_num * (PAGE_NUM - 1); i++)
		{
		    p_listbar->lbar_dif_page[i] = p_listbar->lbar_dif_page[p_listbar->lbar_page_num + i];
		}
		for(i = p_listbar->lbar_page_num * (PAGE_NUM - 1); i < p_listbar->lbar_page_num * PAGE_NUM; i++)
		{
		    p_listbar->lbar_dif_page[i] = -1;
		}		
		p_listbar->lbar_dif_num = p_listbar->lbar_page_num + p_listbar->lbar_item - 1;
	}	
	else if(mode == DECODE_CURR)
	{	
		for(i = 0; i < PAGE_NUM; i++)		
		{	
			p_listbar->lbar_page[i].page_sta = 0;
			
            GUI_LyrWinSel(p_listbar->lbar_page[i].page_lyr);
            GUI_SetBkColor(p_listbar->config.bk_color);
            GUI_Clear();
		}
		for(i = 0; i < p_listbar->lbar_page_num * PAGE_NUM; i++)
		{
		    p_listbar->lbar_dif_page[i] = -1;
		}
		p_listbar->lbar_dif_num = p_listbar->lbar_page_num + p_listbar->lbar_item - 1;
	}   
}

/*
**********************************************************************************************************************
*                                               __listbar_show_scroll
*
* Description: ��ʾ������
*
* Arguments  :
*
* Returns    :

* Notes      :		//modified by libaiao,2011.5.10
*
**********************************************************************************************************************
*/
static void __listbar_show_scroll(__listbar_t * hdle)
{
    __s32               i;
    FB					fb;
    __s32               tmp, tmp1, tmp2, tmp_x, tmp_y, tmp_h;
    RECT                fb_rect1, fb_rect2, cp_rect1, cp_rect2;
    HBLOCK              l_block1; 
    HBLOCK              l_block2; 
	__listbar_t*	    p_listbar;
    
    p_listbar = hdle;   

//modified by libaiao,2011.5.10


	GUI_LyrWinSel(p_listbar->lwin_hdle); 					

	if(p_listbar->scroll_para.head_bmp != NULL)
	{
		tmp_x = p_listbar->scroll_para.show_rect.x;
		tmp_y = p_listbar->scroll_para.show_rect.y; 		
		GUI_ARGB_Draw(p_listbar->scroll_para.head_bmp, tmp_x,tmp_y);	
	}

	if(p_listbar->scroll_para.back_bmp != NULL)
    {   // ����ͼ��
		tmp_x = p_listbar->scroll_para.scroll_rect.x;
		tmp_y = p_listbar->scroll_para.scroll_rect.y; 		    
        GUI_ARGB_Draw(p_listbar->scroll_para.back_bmp, tmp_x, tmp_y);
    }
    else
    {
        //GUI_SetBkColor(p_listbar->config.bk_color);
        //GUI_ClearRect);
        __msg("no bmp file for scrollbar!\n");
        return ;
    }

	if(p_listbar->scroll_para.tail_bmp != NULL)
	{
		tmp_x = p_listbar->scroll_para.show_rect.x;
		tmp_y = p_listbar->scroll_para.show_rect.y + p_listbar->scroll_para.show_rect.height - p_listbar->scroll_para.tail_height; 		
		GUI_ARGB_Draw(p_listbar->scroll_para.tail_bmp, tmp_x,tmp_y);	
	}
	
    
	tmp_h = p_listbar->scroll_para.scroll_rect.height;
	if(p_listbar->lbar_current_total <= p_listbar->lbar_page_num)
	{   // ������һ������
	    
	    tmp_x = p_listbar->scroll_para.scroll_rect.x;
	    tmp_y = p_listbar->scroll_para.scroll_rect.y;
	    for(i = 0; i < tmp_h; i++)
	    {
    	    GUI_ARGB_Draw(p_listbar->scroll_para.body_bmp, tmp_x, tmp_y + i);//body �ĸ߶ȱ���Ϊ1
	    }
	}
	else
	{
	    tmp1  = p_listbar->lbar_page_num * p_listbar->scroll_para.scroll_rect.height / p_listbar->lbar_current_total;
	    if(tmp1 < 1)
	    {
	        tmp1 = 1;
	    }
	    tmp   = p_listbar->lbar_current_move * p_listbar->scroll_para.scroll_rect.height / 
	            ((p_listbar->lbar_current_total + p_listbar->lbar_page_col - 1) / p_listbar->lbar_page_col * p_listbar->config.item_height);
        if(p_listbar->lbar_current_move < 0)
        {
            tmp = 0;
        }
        else if(tmp > (p_listbar->scroll_para.scroll_rect.height - tmp1))
        {
            tmp = p_listbar->scroll_para.scroll_rect.height - tmp1;
        }
        
                
	    tmp_x = p_listbar->scroll_para.scroll_rect.x;
	    tmp_y = p_listbar->scroll_para.scroll_rect.y + tmp;
	    tmp2  = tmp1;
	    for(i = 0; i < tmp2; i++)
	    {
    	    GUI_ARGB_Draw(p_listbar->scroll_para.body_bmp, tmp_x, tmp_y + i);
	    }

	}

//	GUI_LyrWinGetFB(p_listbar->scroll_b_layer, &fb);
//	fb_rect1.x       = 0;
//	fb_rect1.y       = 0;
//	fb_rect1.width   = fb.size.width;
//	fb_rect1.height  = fb.size.height;
//	l_block1 = GUI_BlockCreateFrom(&fb_rect1, fb.fmt.fmt.rgb.pixelfmt, fb.size.width, fb.size.height, fb.addr[0], fb.fmt.fmt.rgb.pixseq);
//
//	GUI_LyrWinGetFB(p_listbar->scroll_f_layer, &fb);
//	fb_rect2.x       = 0;
//	fb_rect2.y       = 0;
//	fb_rect2.width   = fb.size.width;
//	fb_rect2.height  = fb.size.height;
//	l_block2 = GUI_BlockCreateFrom(&fb_rect2, fb.fmt.fmt.rgb.pixelfmt, fb.size.width, fb.size.height, fb.addr[0], fb.fmt.fmt.rgb.pixseq);
//	    		
//	cp_rect1.x       = p_listbar->scroll_para.show_rect.x;
//	cp_rect1.y       = p_listbar->scroll_para.show_rect.y;
//	cp_rect1.width   = p_listbar->scroll_para.show_rect.width;
//	cp_rect1.height  = p_listbar->scroll_para.show_rect.height;
//	
//	cp_rect2.x       = p_listbar->scroll_para.show_rect.x;
//	cp_rect2.y       = p_listbar->scroll_para.show_rect.y;
//	cp_rect2.width   = p_listbar->scroll_para.show_rect.width;
//	cp_rect2.height  = p_listbar->scroll_para.show_rect.height;
//
//	GUI_BlockLayerBlit(p_listbar->lwin_hdle, &cp_rect1, 0, 0, l_block1, NULL, BLITFLAG_COLORSET, NULL);
//  	GUI_BlockLayerBlit(p_listbar->lwin_hdle, &cp_rect2, 0, 0, l_block2, NULL, BLITFLAG_COLORALPHA, NULL);
//    
//    GUI_BlockDelete(l_block1);
//    GUI_BlockDelete(l_block2);
}

/*
**********************************************************************************************************************
*                                               __listbar_draw_page
*
* Description: ����һҳ
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __listbar_draw_page(__listbar_t * hdle)
{
    __u32                   i;    
	__u32                   num;
	__s32                   tmp_page;
	__listbar_t*	        p_listbar;
    __lbar_draw_para_t      draw_para;
    
    p_listbar = hdle;
    
    if(p_listbar->lbar_page[p_listbar->lbar_cnt].page_sta == 0)
    {
        tmp_page = p_listbar->lbar_current_page + (p_listbar->lbar_cnt - PAGE_NUM / 2) * p_listbar->lbar_page_num;
        GUI_LyrWinSel(p_listbar->lbar_page[p_listbar->lbar_cnt].page_lyr);
        GUI_SetBkColor(p_listbar->config.bk_color);
        GUI_Clear();             
        
        if(tmp_page < 0)
        {
            p_listbar->lbar_page[p_listbar->lbar_cnt].page_no   = 0;
            p_listbar->lbar_page[p_listbar->lbar_cnt].page_sta  = 1;     
        }
        else if(tmp_page >= p_listbar->lbar_current_total)
        {
            p_listbar->lbar_page[p_listbar->lbar_cnt].page_no   = 0;
            p_listbar->lbar_page[p_listbar->lbar_cnt].page_sta  = 1;    
        }
        else
        {
            GUI_LyrWinSel(p_listbar->lbar_page[p_listbar->lbar_cnt].page_lyr);
                        
            num = p_listbar->lbar_page_num * p_listbar->lbar_cnt;
            
            for(i = 0; i < p_listbar->lbar_page_num; i++)
            {   
                p_listbar->lbar_dif_page[num + i] = tmp_page + i;       // ���ú�̨��־λ
                
                draw_para.rect.x      = (i % p_listbar->lbar_page_col) * p_listbar->config.item_width;
                draw_para.rect.y      = (i / p_listbar->lbar_page_col) * p_listbar->config.item_height;
                draw_para.rect.width  = p_listbar->config.item_width;
                draw_para.rect.height = p_listbar->config.item_height;
                
                draw_para.index = tmp_page + i;
                draw_para.attr  = p_listbar->config.list_attr;
                if(draw_para.index >= p_listbar->lbar_current_total)
                {   // ��������Ŀ�ų��������Ŀʱ�����հ׻�����Ϣ
                    draw_para.mode = LBAR_MODE_VACANT;
                    
                    p_listbar->lbar_draw(&draw_para);
                }
                else
                {   // ����ͨ������Ϣ
                    draw_para.mode = LBAR_MODE_NORMAL;
                    
                    p_listbar->lbar_draw(&draw_para);
                }
            }
            GUI_LyrWinFlushFB(p_listbar->lbar_page[p_listbar->lbar_cnt].page_lyr);

            p_listbar->lbar_page[p_listbar->lbar_cnt].page_no   = tmp_page;
            p_listbar->lbar_page[p_listbar->lbar_cnt].page_sta  = 1;
        }
    }
}


/*
**********************************************************************************************************************
*                                               __listbar_draw_focus
*
* Description: ����һ��������Ŀ
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __listbar_draw_focus(__listbar_t * hdle)
{
    FB					    fb;
    RECT                    fb_rect;
    RECT                    cp_rect;
    HBLOCK                  l_block;  
	__listbar_t*	        p_listbar;
    __lbar_draw_para_t      draw_para;
    
    p_listbar = hdle;

    p_listbar->lbar_item = p_listbar->lbar_current_focus - 
                            p_listbar->lbar_current_move / p_listbar->config.item_height * p_listbar->lbar_page_col;
    if(p_listbar->lbar_item < 0)
    {
        p_listbar->lbar_item = 0;
    }  

    if(p_listbar->lbar_focus_sta == 1)
    {
        p_listbar->lbar_focus_sta = 0;
                
        GUI_LyrWinSel(p_listbar->lwin_hdle);
        
        draw_para.rect.x      = p_listbar->lbar_rect.x + (p_listbar->lbar_item % p_listbar->lbar_page_col) * p_listbar->config.item_width;
        draw_para.rect.y      = p_listbar->lbar_rect.y + (p_listbar->lbar_item / p_listbar->lbar_page_col) * p_listbar->config.item_height;		
		
        draw_para.rect.width  = p_listbar->config.item_width;
        draw_para.rect.height = p_listbar->config.item_height;
            
        draw_para.index = p_listbar->lbar_current_focus;
        draw_para.attr  = p_listbar->config.list_attr;
        draw_para.mode = LBAR_MODE_FOCUS;
        
        p_listbar->lbar_draw(&draw_para);

//        GUI_LyrWinSel(p_listbar->item_layer);
//        GUI_SetBkColor(p_listbar->config.bk_color);
//        GUI_Clear();
//        
//        draw_para.rect.x      = 0;
//        draw_para.rect.y      = 0;
//        draw_para.rect.width  = p_listbar->config.item_width;
//        draw_para.rect.height = p_listbar->config.item_height;
//    	        
//        draw_para.index = p_listbar->lbar_current_focus;
//        draw_para.attr  = p_listbar->config.list_attr;
//        draw_para.mode  = LBAR_MODE_FOCUS;
//    
//        p_listbar->lbar_draw(&draw_para);
//    
//        GUI_LyrWinFlushFB(p_listbar->item_layer);
//        
//    	GUI_LyrWinGetFB(p_listbar->item_layer, &fb);
//    	fb_rect.x       = 0;
//    	fb_rect.y       = 0;
//    	fb_rect.width   = fb.size.width;
//    	fb_rect.height  = fb.size.height;
//    	l_block = GUI_BlockCreateFrom(&fb_rect, fb.fmt.fmt.rgb.pixelfmt, fb.size.width, fb.size.height, fb.addr[0], fb.fmt.fmt.rgb.pixseq);
//    
//        cp_rect.x      = p_listbar->lbar_rect.x + (p_listbar->lbar_item % p_listbar->lbar_page_col) * p_listbar->config.item_width;
//        cp_rect.y      = p_listbar->lbar_rect.y + (p_listbar->lbar_item / p_listbar->lbar_page_col) * p_listbar->config.item_height;
//        cp_rect.width  = p_listbar->config.item_width;
//        cp_rect.height = p_listbar->config.item_height;
//    
//    	GUI_BlockLayerBlit(p_listbar->lwin_hdle, &cp_rect, 0, 0, l_block, NULL, BLITFLAG_COLORSET, NULL);
//        GUI_BlockDelete(l_block);
    }
}


/*
**********************************************************************************************************************
*                                               __listbar_draw_miniature
*
* Description: ����һ����ǰ�����Ӧ������ͼ( ����ͼƬ����Ч����Ӧ�������ж�)
*			Ӧ�����жϸý�������ͼ�Ƿ��Ѿ������ˣ��Ա����ظ���������ͼ
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __listbar_draw_miniature(__listbar_t * hdle)
{
	__listbar_t*	        p_listbar;
    __lbar_draw_para_t      draw_para;
    
    p_listbar = hdle;

    GUI_LyrWinSel(p_listbar->lwin_hdle);
    
    draw_para.index = p_listbar->lbar_current_focus;
    draw_para.attr  = p_listbar->config.list_attr;
    draw_para.mode = LBAR_MODE_MINIATURE;
    
    p_listbar->lbar_draw(&draw_para);

}

/*
**********************************************************************************************************************
*                                               __listbar_show_page
*
* Description: ��ʾһҳ
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __listbar_show_page(__listbar_t * hdle, __decode_mode_e mode)
{
    FB					fb;
    FB					fb2;
    RECT                fb_rect;
    RECT                cp_rect;
    HBLOCK              l_block;
	__listbar_t*	    p_listbar;
    
    p_listbar = hdle;   
    GUI_LyrWinCacheOn();
    
    __listbar_brush_page(p_listbar, mode);
    
    p_listbar->lbar_dec_state = mode;
	p_listbar->lbar_cnt = PAGE_NUM / 2;
	
	if(p_listbar->lbar_current_move < 0)
	{
    	p_listbar->lbar_move = (0 - p_listbar->lbar_current_move) % p_listbar->lbar_page_height;
    	  	
        __listbar_draw_page(p_listbar);

        GUI_LyrWinSel(p_listbar->lwin_hdle); 		
		
		GUI_LyrWinGetFB(p_listbar->lbar_page[PAGE_NUM / 2 - 1].page_lyr, &fb);
		// FB��ָ�����Ƶ�ָ��λ��
		fb.addr[0]      = (void *)((__u32)fb.addr[0] + ((fb.size.height - p_listbar->lbar_move) * fb.size.width * 4));
		fb.size.width   = fb.size.width;
		fb.size.height  = p_listbar->lbar_move;
		GUI_BitString_DrawEx(&fb, p_listbar->lbar_rect.x, p_listbar->lbar_rect.y);
		
		GUI_LyrWinGetFB(p_listbar->lbar_page[PAGE_NUM / 2].page_lyr, &fb2);
		fb2.size.width   = fb2.size.width;
		fb2.size.height  = fb2.size.height - p_listbar->lbar_move;
		GUI_BitString_DrawEx(&fb2, p_listbar->lbar_rect.x, p_listbar->lbar_rect.y + fb.size.height);

//		GUI_LyrWinGetFB(p_listbar->lbar_page[PAGE_NUM / 2 - 1].page_lyr, &fb);
//		fb_rect.x = 0;
//		fb_rect.y = fb.size.height - p_listbar->lbar_move;
//		fb_rect.width = fb.size.width;
//		fb_rect.height = p_listbar->lbar_move;
//		l_block = GUI_BlockCreateFrom(&fb_rect, fb.fmt.fmt.rgb.pixelfmt, fb.size.width, fb.size.height, fb.addr[0], fb.fmt.fmt.rgb.pixseq);    		
//		cp_rect.x = p_listbar->lbar_rect.x;
//		cp_rect.y = p_listbar->lbar_rect.y;
//		cp_rect.width = fb.size.width;
//		cp_rect.height = p_listbar->lbar_move;
//		GUI_BlockLayerBlit(p_listbar->lwin_hdle, &cp_rect, 0, 0, l_block, NULL, BLITFLAG_COLORSET, NULL);    		
//        GUI_BlockDelete(l_block);
//        
//		GUI_LyrWinGetFB(p_listbar->lbar_page[PAGE_NUM / 2].page_lyr, &fb2);
//		fb_rect.x = 0;
//		fb_rect.y = 0;
//		fb_rect.width = fb2.size.width;
//		fb_rect.height = fb2.size.height - p_listbar->lbar_move;
//		l_block = GUI_BlockCreateFrom(&fb_rect, fb2.fmt.fmt.rgb.pixelfmt, fb2.size.width, fb2.size.height, fb2.addr[0], fb2.fmt.fmt.rgb.pixseq);    		
//		cp_rect.x = p_listbar->lbar_rect.x;
//		cp_rect.y = p_listbar->lbar_rect.y + p_listbar->lbar_move;
//		cp_rect.width = fb2.size.width;
//		cp_rect.height = fb2.size.height - p_listbar->lbar_move;
//		GUI_BlockLayerBlit(p_listbar->lwin_hdle, &cp_rect, 0, 0, l_block, NULL, BLITFLAG_COLORSET, NULL);   
//		GUI_BlockDelete(l_block);
	}
	else if(p_listbar->lbar_current_move > (p_listbar->lbar_current_total + p_listbar->lbar_page_col - 1) / p_listbar->lbar_page_col * p_listbar->config.item_height)
	{
    	p_listbar->lbar_move = p_listbar->lbar_current_move % p_listbar->lbar_page_height;

        __listbar_draw_page(p_listbar);

        GUI_LyrWinSel(p_listbar->lwin_hdle); 		
		
		GUI_LyrWinGetFB(p_listbar->lbar_page[PAGE_NUM / 2].page_lyr, &fb);
		// FB��ָ�����Ƶ�ָ��λ��
		fb.addr[0]      = (void *)((__u32)fb.addr[0] + (p_listbar->lbar_move * fb.size.width * 4));
		fb.size.width   = fb.size.width;
		fb.size.height  = fb.size.height - p_listbar->lbar_move;
		GUI_BitString_DrawEx(&fb, p_listbar->lbar_rect.x, p_listbar->lbar_rect.y);
		
		GUI_LyrWinGetFB(p_listbar->lbar_page[PAGE_NUM / 2 + 1].page_lyr, &fb2);
		fb2.size.width   = fb2.size.width;
		fb2.size.height  = p_listbar->lbar_move;
		GUI_BitString_DrawEx(&fb2, p_listbar->lbar_rect.x, p_listbar->lbar_rect.y + fb.size.height);
        
//		GUI_LyrWinGetFB(p_listbar->lbar_page[PAGE_NUM / 2].page_lyr, &fb);
//		fb_rect.x = 0;
//		fb_rect.y = p_listbar->lbar_move;
//		fb_rect.width = fb.size.width;
//		fb_rect.height = fb.size.height - p_listbar->lbar_move;
//		l_block = GUI_BlockCreateFrom(&fb_rect, fb.fmt.fmt.rgb.pixelfmt, fb.size.width, fb.size.height, fb.addr[0], fb.fmt.fmt.rgb.pixseq);    		
//		cp_rect.x = p_listbar->lbar_rect.x;
//		cp_rect.y = p_listbar->lbar_rect.y;
//		cp_rect.width = fb.size.width;
//		cp_rect.height = fb.size.height - p_listbar->lbar_move;
//		GUI_BlockLayerBlit(p_listbar->lwin_hdle, &cp_rect, 0, 0, l_block, NULL, BLITFLAG_COLORSET, NULL);    		
//        GUI_BlockDelete(l_block);
//        
//		GUI_LyrWinGetFB(p_listbar->lbar_page[PAGE_NUM / 2 + 1].page_lyr, &fb2);
//		fb_rect.x = 0;
//		fb_rect.y = 0;
//		fb_rect.width = fb2.size.width;
//		fb_rect.height = p_listbar->lbar_move;
//		l_block = GUI_BlockCreateFrom(&fb_rect, fb2.fmt.fmt.rgb.pixelfmt, fb2.size.width, fb2.size.height, fb2.addr[0], fb2.fmt.fmt.rgb.pixseq);    		
//		cp_rect.x = p_listbar->lbar_rect.x;
//		cp_rect.y = p_listbar->lbar_rect.y + fb2.size.height - p_listbar->lbar_move;
//		cp_rect.width = fb2.size.width;
//		cp_rect.height = p_listbar->lbar_move;
//		GUI_BlockLayerBlit(p_listbar->lwin_hdle, &cp_rect, 0, 0, l_block, NULL, BLITFLAG_COLORSET, NULL);   
//		GUI_BlockDelete(l_block);
	}
	else
	{
    	p_listbar->lbar_move = p_listbar->lbar_current_move % p_listbar->lbar_page_height;
   	
        if(p_listbar->lbar_move != 0)
        {
            __listbar_draw_page(p_listbar);
            
            p_listbar->lbar_cnt++;
            __listbar_draw_page(p_listbar);
        }
        else
        {
            __listbar_draw_page(p_listbar);
        }
        
        GUI_LyrWinSel(p_listbar->lwin_hdle);
        __here__
    	if(p_listbar->lbar_move == 0)
    	{	// ��ҳ����ʾ
    		GUI_LyrWinGetFB(p_listbar->lbar_page[PAGE_NUM / 2].page_lyr, &fb);
    		GUI_BitString_DrawEx(&fb, p_listbar->lbar_rect.x, p_listbar->lbar_rect.y);
    		    	    
//    		GUI_LyrWinGetFB(p_listbar->lbar_page[PAGE_NUM / 2].page_lyr, &fb);
//    		fb_rect.x = 0;
//    		fb_rect.y = 0;
//    		fb_rect.width = fb.size.width;
//    		fb_rect.height = fb.size.height;
//    		l_block = GUI_BlockCreateFrom(&fb_rect, fb.fmt.fmt.rgb.pixelfmt, fb.size.width, fb.size.height, fb.addr[0], fb.fmt.fmt.rgb.pixseq);
//    		    		
//    		cp_rect.x = p_listbar->lbar_rect.x;
//    		cp_rect.y = p_listbar->lbar_rect.y;
//    		cp_rect.width = p_listbar->lbar_rect.width;
//    		cp_rect.height = p_listbar->lbar_rect.height;
//    		GUI_BlockLayerBlit(p_listbar->lwin_hdle, &cp_rect, 0, 0, l_block, NULL, BLITFLAG_COLORSET, NULL);
//    	    GUI_BlockDelete(l_block);
    	}
    	else
    	{
    		GUI_LyrWinGetFB(p_listbar->lbar_page[PAGE_NUM / 2].page_lyr, &fb);
    		// FB��ָ�����Ƶ�ָ��λ��
    		fb.addr[0]      = (void *)((__u32)fb.addr[0] + (p_listbar->lbar_move * fb.size.width * 4));
    		fb.size.width   = fb.size.width;
    		fb.size.height  = fb.size.height - p_listbar->lbar_move;
    		GUI_BitString_DrawEx(&fb, p_listbar->lbar_rect.x, p_listbar->lbar_rect.y);
    		
    		GUI_LyrWinGetFB(p_listbar->lbar_page[PAGE_NUM / 2 + 1].page_lyr, &fb2);
    		fb2.size.width   = fb2.size.width;
    		fb2.size.height  = p_listbar->lbar_move;
    		GUI_BitString_DrawEx(&fb2, p_listbar->lbar_rect.x, p_listbar->lbar_rect.y + fb.size.height);    	
    		    	    
//    		GUI_LyrWinGetFB(p_listbar->lbar_page[PAGE_NUM / 2].page_lyr, &fb);
//    		fb_rect.x = 0;
//    		fb_rect.y = p_listbar->lbar_move;
//    		fb_rect.width = fb.size.width;
//    		fb_rect.height = fb.size.height - p_listbar->lbar_move;
//    		l_block = GUI_BlockCreateFrom(&fb_rect, fb.fmt.fmt.rgb.pixelfmt, fb.size.width, fb.size.height, fb.addr[0], fb.fmt.fmt.rgb.pixseq);    		
//    		cp_rect.x = p_listbar->lbar_rect.x;
//    		cp_rect.y = p_listbar->lbar_rect.y;
//    		cp_rect.width = fb.size.width;
//    		cp_rect.height = fb.size.height - p_listbar->lbar_move;
//    		GUI_BlockLayerBlit(p_listbar->lwin_hdle, &cp_rect, 0, 0, l_block, NULL, BLITFLAG_COLORSET, NULL);    		
//            	GUI_BlockDelete(l_block);
//            
//    		GUI_LyrWinGetFB(p_listbar->lbar_page[PAGE_NUM / 2 + 1].page_lyr, &fb2);
//    		fb_rect.x = 0;
//    		fb_rect.y = 0;
//    		fb_rect.width = fb2.size.width;
//    		fb_rect.height = p_listbar->lbar_move;
//    		l_block = GUI_BlockCreateFrom(&fb_rect, fb2.fmt.fmt.rgb.pixelfmt, fb2.size.width, fb2.size.height, fb2.addr[0], fb2.fmt.fmt.rgb.pixseq);    		
//    		cp_rect.x = p_listbar->lbar_rect.x;
//    		cp_rect.y = p_listbar->lbar_rect.y + fb2.size.height - p_listbar->lbar_move;
//    		cp_rect.width = fb2.size.width;
//    		cp_rect.height = p_listbar->lbar_move;
//    		GUI_BlockLayerBlit(p_listbar->lwin_hdle, &cp_rect, 0, 0, l_block, NULL, BLITFLAG_COLORSET, NULL);   
//    		GUI_BlockDelete(l_block); 		  	    
    	}
	}
	__here__
    // ���ƽ�����
    if(p_listbar->lbar_focus_sta == 1)
    {
        __listbar_draw_focus(p_listbar);
    }
    // ���ƽ�����
    if(p_listbar->scroll_state == 1)
    {
        __listbar_show_scroll(p_listbar);    
    }
    GUI_LyrWinFlushFB(p_listbar->lwin_hdle);
    GUI_LyrWinCacheOff();
}

/*
**********************************************************************************************************************
*                                               __listbar_move_page
*
* Description: ҳ���ƶ�
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __listbar_move_page(__listbar_t * hdle, __s32 step)
{
    __u8                err;
    __listbar_t*	    p_listbar;
       
    p_listbar = hdle;
	
	esKRNL_SemPend(p_listbar->lbar_dec_sem, 0, &err);
	
    p_listbar->lbar_current_move = p_listbar->lbar_current_move - step;
       
    if(p_listbar->lbar_current_move < 0)
    {   
	    if(p_listbar->lbar_current_move < (p_listbar->config.item_height - p_listbar->lbar_page_height))
	    {
	        if(p_listbar->lbar_sld_speed > 0)
	        {
	            p_listbar->lbar_sld_speed = 0;
	        }
	        p_listbar->lbar_current_move = p_listbar->config.item_height - p_listbar->lbar_page_height;
	    }
	            
        __listbar_show_page(p_listbar, DECODE_ITEM);
    }
    else if((p_listbar->lbar_current_move + p_listbar->config.item_height) 
             > ((p_listbar->lbar_current_total + p_listbar->lbar_page_col - 1) / p_listbar->lbar_page_col * p_listbar->config.item_height))
    {
        if(p_listbar->lbar_sld_speed > 0)
        {
            p_listbar->lbar_sld_speed = 0;
        }        
        p_listbar->lbar_current_move = (p_listbar->lbar_current_total + p_listbar->lbar_page_col - 1) 
                                        / p_listbar->lbar_page_col * p_listbar->config.item_height - p_listbar->config.item_height;
        
        if((p_listbar->lbar_current_move - p_listbar->lbar_page_height) >= (p_listbar->lbar_current_page / p_listbar->lbar_page_col * p_listbar->config.item_height))
        {               
            p_listbar->lbar_current_page = p_listbar->lbar_current_page + p_listbar->lbar_page_num;
    
            __listbar_show_page(p_listbar, DECODE_NEXT);
        }       
        else if(p_listbar->lbar_current_move < (p_listbar->lbar_current_page / p_listbar->lbar_page_col * p_listbar->config.item_height))
        {   
            p_listbar->lbar_current_page = p_listbar->lbar_current_page - p_listbar->lbar_page_num;
    
            __listbar_show_page(p_listbar, DECODE_PREV);
        }
        else
        {
            __listbar_show_page(p_listbar, DECODE_ITEM);
        }   
    } 
    else if((p_listbar->lbar_current_move - p_listbar->lbar_page_height) >= (p_listbar->lbar_current_page / p_listbar->lbar_page_col * p_listbar->config.item_height))
    {               
        p_listbar->lbar_current_page = p_listbar->lbar_current_page + p_listbar->lbar_page_num;

        __listbar_show_page(p_listbar, DECODE_NEXT);
    }       
    else if(p_listbar->lbar_current_move < (p_listbar->lbar_current_page / p_listbar->lbar_page_col * p_listbar->config.item_height))
    {   
        p_listbar->lbar_current_page = p_listbar->lbar_current_page - p_listbar->lbar_page_num;

        __listbar_show_page(p_listbar, DECODE_PREV);
    }
    else
    {   
        __listbar_show_page(p_listbar, DECODE_ITEM);
    }
    
    esKRNL_SemPost(p_listbar->lbar_dec_sem);
}

/*
**********************************************************************************************************************
*                                               __listbar_decode_task
*
* Description: Ԥ���߳�
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __listbar_decode_task(void * p_arg)
{
    __u8 			    err;
	__listbar_t*	    p_listbar;
	
	p_listbar = (__listbar_t *)p_arg;		
	while(1)
	{
		if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
		{
   			goto EXIT_DEC_TASK;
		}
		esKRNL_TimeDly(2);

		if(p_listbar->lbar_dec_state != DECODE_NULL)
		{
		    if(p_listbar->lbar_page[p_listbar->lbar_cnt].page_sta == 0)
		    {
    		    esKRNL_SemPend(p_listbar->lbar_dec_sem, 0, &err);
    		
    		    // ��һҳ����
    		    __listbar_draw_page(p_listbar);
    	        
    	        // ҳ��������仯
    	        p_listbar->lbar_cnt++;
    	        if(p_listbar->lbar_cnt == PAGE_NUM)
    	        {
    	            p_listbar->lbar_cnt = 0;
    	        }
    	            		    
    		    esKRNL_SemPost(p_listbar->lbar_dec_sem);
    		}
		    else
		    {
    	        // ҳ��������仯
    	        p_listbar->lbar_cnt++;
    	        if(p_listbar->lbar_cnt == PAGE_NUM)
    	        {
    	            p_listbar->lbar_cnt = 0;
    	        }		        
		    }
		}
	}
	
EXIT_DEC_TASK:
	esKRNL_TDel(EXEC_prioself);   
}

/*
**********************************************************************************************************************
*                                               __listbar_diffuse_task
*
* Description: �����߳�
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __listbar_diffuse_task(void * p_arg)
{
    __u8 			    err;
    __s32               tmp_cnt;
    __s32               tmp_num;
    __s32		        tmp_page;    
    OS_SEM_DATA         pdata;
	__listbar_t*	    p_listbar;
	__lbar_draw_para_t  draw_para;
	
	p_listbar = (__listbar_t *)p_arg;		
	while(1)
	{
		if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
		{
   			goto EXIT_DIF_TASK;
		}
		esKRNL_TimeDly(1);

		esKRNL_SemQuery(p_listbar->lbar_dif_sem, &pdata);
		if( pdata.OSCnt == 1 )
		{
			esKRNL_SemPend(p_listbar->lbar_dif_sem,0,&err);
		    if(p_listbar->lbar_dec_state != DECODE_NULL)
			{		
	            if(p_listbar->lbar_sld_state == 0)
	            {   // �����жϵ�ǰ״̬
	        		esKRNL_SemQuery(p_listbar->lbar_dec_sem, &pdata);
	        		if(pdata.OSCnt == 1)
	    	        {   // �����й���Ԥ���̡߳�         
	    	            esKRNL_SemPend(p_listbar->lbar_dec_sem, 0, &err);
	    	            
	    	            tmp_cnt = p_listbar->lbar_dif_num / p_listbar->lbar_page_num;
	    	            tmp_page = p_listbar->lbar_dif_page[p_listbar->lbar_dif_num];
	    	            
	                    if((tmp_page >= 0) && (tmp_page < p_listbar->lbar_current_total))
	            		{   
	            			/* �ڵ�ǰ��Ҫֱ����ʾ����Ļ�ϵķ�Χ��*/
	            		    if((tmp_page >= (p_listbar->lbar_current_focus - p_listbar->lbar_item)) && 
	            		       (tmp_page <= (p_listbar->lbar_current_focus - p_listbar->lbar_item + p_listbar->lbar_page_num - 1)))
	            		    {
	                            GUI_LyrWinSel(p_listbar->lwin_hdle);
	                            
	                            tmp_num               = tmp_page - (p_listbar->lbar_current_focus - p_listbar->lbar_item);
	                            draw_para.rect.x      = p_listbar->lbar_rect.x + (tmp_num % p_listbar->lbar_page_col) * p_listbar->config.item_width;
	                            draw_para.rect.y      = p_listbar->lbar_rect.y + (tmp_num / p_listbar->lbar_page_col) * p_listbar->config.item_height;
	                            draw_para.rect.width  = p_listbar->config.item_width;
	                            draw_para.rect.height = p_listbar->config.item_height;
	    
	                            draw_para.index = tmp_page;
	                            draw_para.attr  = p_listbar->config.list_attr;     
	                            draw_para.mode = LBAR_MODE_DIFFUSE;
	    
	                            p_listbar->lbar_draw(&draw_para);                       
	                        }
	            			/* ���ڵ�ǰ��Ҫֱ����ʾ����Ļ�ϵķ�Χ��*/
	    					GUI_LyrWinSel(p_listbar->lbar_page[tmp_cnt].page_lyr);
	    
	                        tmp_num = tmp_page % p_listbar->lbar_page_num;
	                        draw_para.rect.x      = 0 + (tmp_num % p_listbar->lbar_page_col) * p_listbar->config.item_width;
	                        draw_para.rect.y      = 0 + (tmp_num / p_listbar->lbar_page_col) * p_listbar->config.item_height;
	                        draw_para.rect.width  = p_listbar->config.item_width;
	                        draw_para.rect.height = p_listbar->config.item_height;
	                        
	                        draw_para.index = tmp_page;
	                        draw_para.attr  = p_listbar->config.list_attr;     
	                        draw_para.mode = LBAR_MODE_DIFFUSE;
	                                                
	                        p_listbar->lbar_draw(&draw_para);
	                                                        
	                        p_listbar->lbar_dif_page[p_listbar->lbar_dif_num] = -1;
	            		} 
	                    p_listbar->lbar_dif_num++;
	                    if(p_listbar->lbar_dif_num >= (p_listbar->lbar_page_num * PAGE_NUM))
	                    {
	                        p_listbar->lbar_dif_num = 0;
	                    }
	                    __listbar_draw_miniature(p_listbar);	//////
	                    esKRNL_SemPost(p_listbar->lbar_dec_sem);
	    	        }
		        }
	    	}
			
			esKRNL_SemPost(p_listbar->lbar_dif_sem);
		}
        
	}
	
EXIT_DIF_TASK:
	esKRNL_TDel(EXEC_prioself);   
}

/*
**********************************************************************************************************************
*                                               __listbar_slider_task
*
* Description: �����߳�
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __listbar_slider_task(void * p_arg)
{
    __u8 			    err; 
    __s32               tmp = 0;
    __s32               tmp_height = 0;
    __s32               tmp_number = -1;    
    OS_SEM_DATA         pdata;
	__listbar_t*	    p_listbar;
	
	p_listbar = (__listbar_t *)p_arg;		
	while(1)
	{
		if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
		{
   			goto EXIT_SLD_TASK;
		}

		esKRNL_SemQuery(p_listbar->lbar_sld_sem, &pdata);
		if(pdata.OSCnt == 1)
        {
            if(p_listbar->lbar_sld_speed >= 0)
            {   
                esKRNL_SemPend(p_listbar->lbar_sld_sem, 0, &err);
                	
            	if(p_listbar->lbar_sld_speed == 0)
            	{   // �ٶ�Ϊ0ʱ������΢����
                    if(tmp_height == 0)
                    {
                        tmp_height = (p_listbar->lbar_current_total + p_listbar->lbar_page_col - 1) 
                                      / p_listbar->lbar_page_col * p_listbar->config.item_height;
                	}            

                	if(p_listbar->lbar_current_move < 0)
                	{   // �Ϸ������
                	    if(tmp_number == -1)
                	    {
                	        tmp_number = (0 - p_listbar->lbar_current_move) / MAX_GLIDE_STEP;
                	    }
                        if(tmp_number)
                        {
                            tmp_number--;
                            __listbar_move_page(p_listbar, 0 - MAX_GLIDE_STEP);                         
                        }
                        else
                        {   
                            tmp_number--;
                            p_listbar->lbar_current_move = 0;
                            __listbar_move_page(p_listbar, 0);
                        } 
                	}
                	else if(p_listbar->lbar_current_move >= (tmp_height - p_listbar->lbar_page_height))
                	{   // �·������
                	    if((tmp_height - p_listbar->lbar_page_height) < 0)
                	    {
                    	    if(tmp_number == -1)
                    	    {
                    	        tmp_number = p_listbar->lbar_current_move / MAX_GLIDE_STEP;
                    	    }
                            if(tmp_number)
                            {
                                tmp_number--;
                                __listbar_move_page(p_listbar, MAX_GLIDE_STEP);
                            }
                            else
                            {   
                                tmp_number--;
                                p_listbar->lbar_current_move = 0;
                                __listbar_move_page(p_listbar, 0);
                            }                            
                	    }
                	    else
                	    {
                    	    if(tmp_number == -1)
                    	    {
                    	        tmp_number = (p_listbar->lbar_current_move - (tmp_height - p_listbar->lbar_page_height)) / MAX_GLIDE_STEP;
                    	    }
                            if(tmp_number)
                            {
                                tmp_number--;
                                __listbar_move_page(p_listbar, MAX_GLIDE_STEP);
                            }
                            else
                            {   
                                tmp_number--;
                                p_listbar->lbar_current_move = tmp_height - p_listbar->lbar_page_height;
                                __listbar_move_page(p_listbar, 0);
                            }                            
                        }
                	}
                	else
                	{
                	    if(tmp == 0)
                	    {      	    
                            tmp = p_listbar->lbar_current_move % p_listbar->config.item_height;
                        }
                        if(tmp != 0)
                        {
                    	    if(p_listbar->lbar_last_direct > 0)
                    	    {
                        	    if(tmp_number == -1)
                        	    {
                        	        tmp_number = tmp / (p_listbar->config.item_height / 5);
                        	    }
                                if(tmp_number)
                                {
                                    tmp_number--;
                                    __listbar_move_page(p_listbar, p_listbar->config.item_height / 5);
                                }
                                else
                                {
                                    tmp_number--;
                                    p_listbar->lbar_current_move = p_listbar->lbar_current_move 
                                                                   / p_listbar->config.item_height * p_listbar->config.item_height;
                                    __listbar_move_page(p_listbar, 0);                                   
                                }
                    	    }
                    	    else if(p_listbar->lbar_last_direct < 0)
                    	    {
                    	        if(tmp_number == -1)
                        	    {
                        	        tmp_number = (p_listbar->config.item_height - tmp) / (p_listbar->config.item_height / 5);
                        	    }
                                if(tmp_number)
                                {
                                    tmp_number--;
                                    __listbar_move_page(p_listbar, (0 - p_listbar->config.item_height / 5));
                                }
                                else
                                {
                                    tmp_number--;
                                    p_listbar->lbar_current_move = (p_listbar->lbar_current_move + p_listbar->config.item_height / 2)
                                                                   / p_listbar->config.item_height * p_listbar->config.item_height;                                    
                                    __listbar_move_page(p_listbar, 0);                          
                                }
                    	    }
                    	    else
                    	    {
                    	        tmp_number = -1;
                    	    }
                        }
                        else
                        {
                            tmp_number = -1;
                        }
                    }
               
                    if(tmp_number == -1)
                    {   // �������
                        tmp = 0;
                        tmp_height = 0;
                        tmp_number = -1;  
                        p_listbar->lbar_sld_speed = -1;
                                                
                        if(p_listbar->lbar_focus_sta == 0)
                        {
                            p_listbar->lbar_focus_sta = 1;
                            p_listbar->lbar_current_focus = p_listbar->lbar_current_move / p_listbar->config.item_height * p_listbar->lbar_page_col;
                            __listbar_draw_focus(p_listbar);
                        }
                    }
            	}
            	else
            	{   // ���ݻ����ٶȽ��й��Ի�����
            	    if(p_listbar->lbar_last_direct > 0)
            	    {
                	    if(p_listbar->lbar_sld_speed)
                	    {
                	        p_listbar->lbar_sld_speed--;
                            __listbar_move_page(p_listbar, MAX_GLIDE_STEP);
                        }
            	    }
            	    else if(p_listbar->lbar_last_direct < 0)
            	    {
                	    if(p_listbar->lbar_sld_speed)
                	    {
                	        p_listbar->lbar_sld_speed--;
                            __listbar_move_page(p_listbar, 0 - MAX_GLIDE_STEP);
                        }
            	    }
            	    else
            	    {
            	        p_listbar->lbar_sld_speed = 0;
            	    }
            	    
            	    if(p_listbar->lbar_sld_speed == 0)
            	    {
                        tmp = 0;
                        tmp_height = 0;
                        tmp_number = -1;          	        
            	    }
            	}
                	
                esKRNL_SemPost(p_listbar->lbar_sld_sem);
                                               
                if(p_listbar->lbar_sld_speed == 0)
                {
                    esKRNL_TimeDly(4);
                }
                else if(p_listbar->lbar_sld_speed < 6)
                {
                    esKRNL_TimeDly(2);
                }
                else
                {
                    esKRNL_TimeDly(1);
                }
            }
            else
            {
                if(p_listbar->lbar_sld_state == -1)
                {   // ���������߳�
                    p_listbar->lbar_sld_state = 0;
                }
                tmp = 0;
                tmp_height = 0;
                tmp_number = -1;
                             
                esKRNL_TimeDly(5);                    
            }
        }
        else
        {
            tmp = 0;
            tmp_height = 0;
            tmp_number = -1; 
                       
            esKRNL_TimeDly(5);
        }
	}
	
EXIT_SLD_TASK:
	esKRNL_TDel(EXEC_prioself);   
}

/*
**********************************************************************************************************************
*                                               LISTBAR_Create
*
* Description: ����һ�����Ʋ������
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
H_LBAR  LISTBAR_Create(H_WIN hwin)
{
	__listbar_t*	    p_listbar;
	
	p_listbar = (__listbar_t *)esMEMS_Malloc(0, sizeof(__listbar_t));	// �����ڴ�
	if(p_listbar == NULL)
	{
		__err(" frame ctrl malloc error \n");
		return NULL;
	}
	eLIBs_memset(p_listbar, 0, sizeof(__listbar_t));
		
    p_listbar->fwin_hdle = hwin;

    // ��ȡ��ǰ����ͼ����
    p_listbar->lwin_hdle = GUI_WinGetLyrWin(hwin);

   	p_listbar->lbar_dec_state   = DECODE_NULL;	            // ����״̬λ
	p_listbar->lbar_cnt         = PAGE_NUM / 2;             // ����ҳ������

    // ������̨Ԥ���߳�
	p_listbar->lbar_dec_sem     = esKRNL_SemCreate(1);	    // Ԥ������ź���
	p_listbar->lbar_dec_tsk     = esKRNL_TCreate(__listbar_decode_task, (void*)p_listbar, 0x8000, KRNL_priolevel4);		// ��̨Ԥ���߳�

	// ������̨�����߳�
	p_listbar->lbar_sld_speed   = -1;
	p_listbar->lbar_sld_state   = 0;
	p_listbar->lbar_sld_sem     = esKRNL_SemCreate(1);	    // ���������ź���
    p_listbar->lbar_sld_tsk     = esKRNL_TCreate(__listbar_slider_task, (void*)p_listbar, 0x8000, KRNL_priolevel5);	
    		
	// ������̨�����߳�
	p_listbar->lbar_dif_sem = esKRNL_SemCreate(1);	    // ��ɢ�����ź���
    p_listbar->lbar_dif_tsk     = esKRNL_TCreate(__listbar_diffuse_task, (void*)p_listbar, 0x8000, KRNL_priolevel5);    // ��ʾ��ɢ�߳�
    
    __msg("task 1 = %d, task2 = %d, task3 = %d \n", p_listbar->lbar_sld_tsk, p_listbar->lbar_dec_tsk, p_listbar->lbar_dif_tsk);
           
	return p_listbar;
}

/*
**********************************************************************************************************************
*                                               LISTBAR_Config
*
* Description: ����listbar�������
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
__s32   LISTBAR_Config(H_LBAR hdle, __draw_item draw_cb, __listbar_config_t * config)
{
    __u32               i;
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle;    

    p_listbar->lbar_draw = draw_cb;

    eLIBs_memcpy(&p_listbar->config, config, sizeof(__listbar_config_t));
    
    p_listbar->lbar_rect = config->list_rect;
    
    __msg("x = %d, y = %d, w = %d, h = %d \n", config->list_rect.x, config->list_rect.y, config->list_rect.width, config->list_rect.height);
    
    __msg("item widht = %d, item height = %d \n", config->item_width, config->item_height);
    
    p_listbar->item_layer = __listbar_layer_create(p_listbar->config.item_width, p_listbar->config.item_height, 0);
    
    for(i = 0; i < PAGE_NUM; i++)
    {
        p_listbar->lbar_page[i].page_lyr = __listbar_layer_create(p_listbar->lbar_rect.width, p_listbar->lbar_rect.height, 0);
    }   
    p_listbar->lbar_page_col        = p_listbar->lbar_rect.width / config->item_width;
    p_listbar->lbar_page_row        = p_listbar->lbar_rect.height / config->item_height;
    p_listbar->lbar_page_num        = p_listbar->lbar_page_row * p_listbar->lbar_page_col;
    p_listbar->lbar_page_width      = p_listbar->lbar_page_col * config->item_width;
    p_listbar->lbar_page_height     = p_listbar->lbar_page_row * config->item_height;

    if(config->start_id == -1)
    {
        p_listbar->lbar_current_page    = config->focus_id / p_listbar->lbar_page_num * p_listbar->lbar_page_num;
        p_listbar->lbar_current_focus   = config->focus_id;
        p_listbar->lbar_current_move    = p_listbar->lbar_current_page / p_listbar->lbar_page_col * config->item_height;
        p_listbar->lbar_current_total   = config->item_cnt;
    }
    else
    {
        p_listbar->lbar_current_page    = config->start_id / p_listbar->lbar_page_num * p_listbar->lbar_page_num;
        p_listbar->lbar_current_focus   = config->focus_id;
        p_listbar->lbar_current_move    = config->start_id / p_listbar->lbar_page_col * config->item_height;
        p_listbar->lbar_current_total   = config->item_cnt;
    }
    
    p_listbar->lbar_focus_sta   = 1;
    p_listbar->lbar_touch_sta   = 0;										       
	
    return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                               LISTBAR_ScrollConfig
*
* Description: ����listbar�������
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
__s32   LISTBAR_ScrollConfig(H_LBAR hdle, __scroll_bar_t * scroll_para)
{
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle;    

    eLIBs_memcpy(&p_listbar->scroll_para, scroll_para, sizeof(__scroll_bar_t));
    
    p_listbar->scroll_state = 1;

 	p_listbar->scroll_b_layer = __listbar_layer_create(p_listbar->scroll_para.show_rect.width, p_listbar->scroll_para.show_rect.height, 0);
	p_listbar->scroll_f_layer = __listbar_layer_create(p_listbar->scroll_para.show_rect.width, p_listbar->scroll_para.show_rect.height, 0);
	
	GUI_LyrWinSel(p_listbar->scroll_b_layer);
	if(p_listbar->scroll_para.back_bmp != NULL)
    {   // ����ͼ��
        GUI_ARGB_Draw(p_listbar->scroll_para.back_bmp, 0, 0);
    }
    else
    {
        GUI_SetBkColor(p_listbar->config.bk_color);
        GUI_Clear();
    }
	
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				LISTBAR_ShowPage
*
*Description: ��ʾ��ǰҳ��
*
*Arguments  : hdle: �������  
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_ShowPage(H_LBAR hdle)
{
    __u8                err;
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle; 

    esKRNL_SemPend(p_listbar->lbar_dec_sem, 0, &err);
    
    p_listbar->lbar_focus_sta   = 1;
	__listbar_show_page(p_listbar, DECODE_CURR);    

    esKRNL_SemPost(p_listbar->lbar_dec_sem);
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				LISTBAR_NextPage
*
*Description: ���·�һҳ
*
*Arguments  : hdle: �������  
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_NextPage(H_LBAR hdle)
{
    __u8                err;
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle; 
    
    esKRNL_SemPend(p_listbar->lbar_dec_sem, 0, &err);
    
    p_listbar->lbar_focus_sta   = 1;
    if(p_listbar->lbar_current_total > p_listbar->lbar_page_num)
    {
    	p_listbar->lbar_current_page= p_listbar->lbar_current_focus;	//add by libiaoao,2011.4.26
        p_listbar->lbar_current_page = p_listbar->lbar_current_page + p_listbar->lbar_page_num;
        if(p_listbar->lbar_current_page >= p_listbar->lbar_current_total)
        {
            p_listbar->lbar_current_page    = 0;
            
            p_listbar->lbar_current_focus   = p_listbar->lbar_current_page;
            p_listbar->lbar_current_move    = p_listbar->lbar_current_page / p_listbar->lbar_page_col * p_listbar->config.item_height;
            
        	__listbar_show_page(p_listbar, DECODE_CURR);            
        }
        else
        {
            p_listbar->lbar_current_focus   = p_listbar->lbar_current_page;
            p_listbar->lbar_current_move    = p_listbar->lbar_current_page / p_listbar->lbar_page_col * p_listbar->config.item_height;
            
        	__listbar_show_page(p_listbar, DECODE_NEXT);
        }
    }
    
    esKRNL_SemPost(p_listbar->lbar_dec_sem); 
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				LISTBAR_PrevPage
*
*Description: ���Ϸ�һҳ
*
*Arguments  : hdle: �������  
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_PrevPage(H_LBAR hdle)
{
    __u8                err;
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle; 
    
    esKRNL_SemPend(p_listbar->lbar_dec_sem, 0, &err);
    
    p_listbar->lbar_focus_sta   = 1;
    if(p_listbar->lbar_current_total > p_listbar->lbar_page_num)
    {
    	p_listbar->lbar_current_page= p_listbar->lbar_current_focus;	//add by libiaoao,2011.4.26
		if(p_listbar->lbar_current_page < p_listbar->lbar_page_num)
		{
			return EPDK_FAIL; 	//����ǰ��ҳ
		}
        p_listbar->lbar_current_page = p_listbar->lbar_current_page - p_listbar->lbar_page_num;        
		if(p_listbar->lbar_current_page < p_listbar->lbar_page_num)
		{
			p_listbar->lbar_current_page = 0;	
		}
        if(p_listbar->lbar_current_page < 0)
        {
            p_listbar->lbar_current_page = (p_listbar->lbar_current_total - 1) / p_listbar->lbar_page_num * p_listbar->lbar_page_num;
        
            p_listbar->lbar_current_focus   = p_listbar->lbar_current_page;
            p_listbar->lbar_current_move    = p_listbar->lbar_current_page / p_listbar->lbar_page_col * p_listbar->config.item_height;
                
        	__listbar_show_page(p_listbar, DECODE_CURR);        
        }
        else
        {
            p_listbar->lbar_current_focus   = p_listbar->lbar_current_page;
            p_listbar->lbar_current_move    = p_listbar->lbar_current_page / p_listbar->lbar_page_col * p_listbar->config.item_height;
                
        	__listbar_show_page(p_listbar, DECODE_PREV);
        }
    }
    
    esKRNL_SemPost(p_listbar->lbar_dec_sem); 
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				LISTBAR_NextItem
*
*Description: ���Ƹ���Ŀ
*
*Arguments  : hdle: �������  
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_NextItem(H_LBAR hdle)
{
    __u8                err;
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle; 
    
    esKRNL_SemPend(p_listbar->lbar_dec_sem, 0, &err);

    p_listbar->lbar_focus_sta   = 1;
    // ��������һ����Ŀ
    p_listbar->lbar_current_focus++;
    
    if(p_listbar->lbar_current_focus >= p_listbar->lbar_current_total)
    {   // �������һ����Ŀ����ת����һ����Ŀ
        if(p_listbar->lbar_current_total <= p_listbar->lbar_page_num)
        {
            p_listbar->lbar_current_focus   = 0;
            p_listbar->lbar_current_page    = 0;
            p_listbar->lbar_current_move    = 0;
            
            __listbar_show_page(p_listbar, DECODE_ITEM);
        }        
        else
        {
            p_listbar->lbar_current_focus   = 0;
            p_listbar->lbar_current_page    = 0;
            p_listbar->lbar_current_move    = 0;
            
            __listbar_show_page(p_listbar, DECODE_CURR);
        }
    }
    else
    {
        if(p_listbar->lbar_current_focus >= (p_listbar->lbar_current_page + p_listbar->lbar_page_num + p_listbar->lbar_page_num - p_listbar->lbar_page_col))
        {
            p_listbar->lbar_current_page    = p_listbar->lbar_current_page + p_listbar->lbar_page_num;
            p_listbar->lbar_current_move    = p_listbar->lbar_current_move + p_listbar->config.item_height;

            __listbar_show_page(p_listbar, DECODE_NEXT);
        }
        else
        {
            if(p_listbar->lbar_item < (p_listbar->lbar_page_num - 1))
            {
                __listbar_show_page(p_listbar, DECODE_ITEM);
            }
            else
            {   
                if(p_listbar->lbar_page_col == 1)
                {
                    p_listbar->lbar_current_move    = p_listbar->lbar_current_move + p_listbar->config.item_height;
                }
                else
                {
                    if((p_listbar->lbar_current_focus % p_listbar->lbar_page_col) == 0)
                    {
                        p_listbar->lbar_current_move    = p_listbar->lbar_current_move + p_listbar->config.item_height;
                    }
                }
                __listbar_show_page(p_listbar, DECODE_ITEM);                
            }
        }
    }

    esKRNL_SemPost(p_listbar->lbar_dec_sem); 
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				LISTBAR_PrevItem
*
*Description: ���Ƹ���Ŀ
*
*Arguments  : hdle: �������  
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_PrevItem(H_LBAR hdle)
{
    __u8                err;
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle; 
    
    esKRNL_SemPend(p_listbar->lbar_dec_sem, 0, &err);

    p_listbar->lbar_focus_sta   = 1;
    // ��������һ����Ŀ
    p_listbar->lbar_current_focus--;

    if(p_listbar->lbar_current_focus < 0)
    {   // ���ǵ�һ����Ŀ����ת�����һ����Ŀ
        if(p_listbar->lbar_current_total <= p_listbar->lbar_page_num)
        {
            p_listbar->lbar_current_focus   = p_listbar->lbar_current_total - 1;
            p_listbar->lbar_current_page    = 0;
            p_listbar->lbar_current_move    = 0;
            
            __listbar_show_page(p_listbar, DECODE_ITEM);
        }
        else
        {
            p_listbar->lbar_current_focus   = p_listbar->lbar_current_total - 1;
            p_listbar->lbar_current_page    = ((p_listbar->lbar_current_total - p_listbar->lbar_page_num) / p_listbar->lbar_page_num) * p_listbar->lbar_page_num;
            p_listbar->lbar_current_move    = (p_listbar->lbar_current_total - p_listbar->lbar_page_num + p_listbar->lbar_page_col - 1) / p_listbar->lbar_page_col * p_listbar->config.item_height;
            
            __listbar_show_page(p_listbar, DECODE_CURR);
        }
    }
    else
    {
        if(p_listbar->lbar_current_focus < p_listbar->lbar_current_page)
        {
            p_listbar->lbar_current_page    = p_listbar->lbar_current_page - p_listbar->lbar_page_num;
            p_listbar->lbar_current_move    = p_listbar->lbar_current_move - p_listbar->config.item_height;
            
            __listbar_show_page(p_listbar, DECODE_PREV);
        }
        else
        {
            if(p_listbar->lbar_item > 0)
            {
                __listbar_show_page(p_listbar, DECODE_ITEM);
            }
            else
            {
                if(p_listbar->lbar_page_col == 1)
                {
                    p_listbar->lbar_current_move    = p_listbar->lbar_current_move - p_listbar->config.item_height;
                }
                else
                {
                    if((p_listbar->lbar_current_focus % p_listbar->lbar_page_col) == (p_listbar->lbar_page_col - 1))
                    {
                        p_listbar->lbar_current_move    = p_listbar->lbar_current_move - p_listbar->config.item_height;
                    }
                }
                
                __listbar_show_page(p_listbar, DECODE_ITEM);                
            }
        }
    }
    
    esKRNL_SemPost(p_listbar->lbar_dec_sem);
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				LISTBAR_NextRow
*
*Description: �����ƶ�(һ��)
*
*Arguments  : hdle: �������  
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_NextRow(H_LBAR hdle)
{
    __u8                err;
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle; 
    
    esKRNL_SemPend(p_listbar->lbar_dec_sem, 0, &err);

    p_listbar->lbar_focus_sta   = 1;
    // ��������һ����Ŀ
    p_listbar->lbar_current_focus = p_listbar->lbar_current_focus + p_listbar->lbar_page_col;
    
    if(p_listbar->lbar_current_focus >= p_listbar->lbar_current_total)
    {   // �������һ����Ŀ����ת����һ����Ŀ
        if(p_listbar->lbar_current_total <= p_listbar->lbar_page_num)
        {
            p_listbar->lbar_current_focus   = p_listbar->lbar_current_focus % p_listbar->lbar_page_col;
            p_listbar->lbar_current_page    = 0;
            p_listbar->lbar_current_move    = 0;
                        
            __listbar_show_page(p_listbar, DECODE_ITEM);
        }
        else
        {        
            p_listbar->lbar_current_focus   = p_listbar->lbar_current_focus % p_listbar->lbar_page_col;
            p_listbar->lbar_current_page    = 0;
            p_listbar->lbar_current_move    = 0;
                    
            __listbar_show_page(p_listbar, DECODE_CURR);
        }
    }
    else
    {
        if(p_listbar->lbar_current_focus >= (p_listbar->lbar_current_page + p_listbar->lbar_page_num + p_listbar->lbar_page_num - 1))
        {
            p_listbar->lbar_current_page    = p_listbar->lbar_current_page + p_listbar->lbar_page_num;
            p_listbar->lbar_current_move    = p_listbar->lbar_current_move + p_listbar->config.item_height;
            
            __listbar_show_page(p_listbar, DECODE_NEXT);
        }
        else
        {
            if(p_listbar->lbar_item < (p_listbar->lbar_page_num - p_listbar->lbar_page_col))
            {
                __listbar_show_page(p_listbar, DECODE_ITEM);
            }
            else
            {   
                p_listbar->lbar_current_move    = p_listbar->lbar_current_move + p_listbar->config.item_height;
                
                __listbar_show_page(p_listbar, DECODE_ITEM);                
            }
        }
    }

    esKRNL_SemPost(p_listbar->lbar_dec_sem);
    
    return EPDK_OK;    
}

/*
************************************************************************************************************************
*                       				LISTBAR_PrevRow
*
*Description: �����ƶ�(һ��)
*
*Arguments  : hdle: �������  
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
2************************************************************************************************************************
*/
__s32   LISTBAR_PrevRow(H_LBAR hdle)
{
    __u8                err;
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle; 
    
    esKRNL_SemPend(p_listbar->lbar_dec_sem, 0, &err);

    p_listbar->lbar_focus_sta   = 1;
    // ��������һ����Ŀ
    p_listbar->lbar_current_focus = p_listbar->lbar_current_focus - p_listbar->lbar_page_col;
    
    if(p_listbar->lbar_current_focus < 0)
    {   // ���ǵ�һ����Ŀ����ת�����һ����Ŀ
        if(p_listbar->lbar_current_total <= p_listbar->lbar_page_num)
        {
            p_listbar->lbar_current_focus   = (p_listbar->lbar_current_total + p_listbar->lbar_page_col - 1) / p_listbar->lbar_page_col * p_listbar->lbar_page_col
                                              + p_listbar->lbar_current_focus;
            if(p_listbar->lbar_current_focus >= p_listbar->lbar_current_total)
            {
                p_listbar->lbar_current_focus = p_listbar->lbar_current_focus - p_listbar->lbar_page_col;
            }
            p_listbar->lbar_current_page    = 0;
            p_listbar->lbar_current_move    = p_listbar->lbar_current_page / p_listbar->lbar_page_col * p_listbar->config.item_height;
                        
            __listbar_show_page(p_listbar, DECODE_ITEM);
        }
        else
        {
            p_listbar->lbar_current_focus   = (p_listbar->lbar_current_total + p_listbar->lbar_page_col - 1) / p_listbar->lbar_page_col * p_listbar->lbar_page_col
                                              + p_listbar->lbar_current_focus;
            if(p_listbar->lbar_current_focus >= p_listbar->lbar_current_total)
            {
                p_listbar->lbar_current_focus = p_listbar->lbar_current_focus - p_listbar->lbar_page_col;
            }

            p_listbar->lbar_current_page    = ((p_listbar->lbar_current_focus - p_listbar->lbar_page_num) / p_listbar->lbar_page_num) * p_listbar->lbar_page_num;
            p_listbar->lbar_current_move    = (p_listbar->lbar_current_total - p_listbar->lbar_page_num + p_listbar->lbar_page_col - 1) / p_listbar->lbar_page_col * p_listbar->config.item_height;
                                               
            __listbar_show_page(p_listbar, DECODE_CURR);
        }
    }
    else
    {
        if(p_listbar->lbar_current_focus < p_listbar->lbar_current_page)
        {
            p_listbar->lbar_current_page    = p_listbar->lbar_current_page - p_listbar->lbar_page_num;
            p_listbar->lbar_current_move    = p_listbar->lbar_current_move - p_listbar->config.item_height;
            
            __listbar_show_page(p_listbar, DECODE_PREV);
        }
        else
        {
            if(p_listbar->lbar_item >= p_listbar->lbar_page_col)
            {
                __listbar_show_page(p_listbar, DECODE_ITEM);
            }
            else
            {
                p_listbar->lbar_current_move    = p_listbar->lbar_current_move - p_listbar->config.item_height;
                
                __listbar_show_page(p_listbar, DECODE_ITEM);
            }
        }
    }

    esKRNL_SemPost(p_listbar->lbar_dec_sem);
    
    return EPDK_OK;    
}

/*
************************************************************************************************************************
*                       				LISTBAR_TouchDown
*
*Description: ��������
*
*Arguments  : hdle: ��������� x: X�����꣬ y��Y������ (x, y �Ƿ���Ч���ж�) 
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_TouchDown(H_LBAR hdle, __s32 x, __s32 y, __s32 direct, __s32 speed)
{
    __u8                err;
    __s32               tmp;
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle; 

    if(p_listbar->lbar_current_total == 0)
    {   // ����Ŀ��Ϊ��ʱ��������Ӧ��
        return EPDK_OK;
    }
    if((x < p_listbar->config.list_rect.x) || (y < p_listbar->config.list_rect.y)
       || (x > (p_listbar->config.list_rect.x + p_listbar->config.list_rect.width))
       || (y > (p_listbar->config.list_rect.y + p_listbar->config.list_rect.height)))  
    {   // ���겻�Ϸ�
        p_listbar->lbar_touch_sta = -1;
        
        return EPDK_FAIL;
    }

    esKRNL_SemPend(p_listbar->lbar_sld_sem, 0, &err);

    if(p_listbar->lbar_sld_state == 0)
    {   // ���������߳�
        p_listbar->lbar_sld_state = 1;
    }
    
    if(p_listbar->lbar_sld_speed == -1)
    {   // ��ǰ���ھ�ֹ״̬
    	p_listbar->lbar_last_pos.x  = x;
    	p_listbar->lbar_last_pos.y  = y;         
        if(p_listbar->lbar_touch_sta == 0)
        {   // ���»��ƽ���״̬
            esKRNL_SemPend(p_listbar->lbar_dec_sem, 0, &err);
            
            p_listbar->lbar_touch_sta   = 1;
            
            tmp = p_listbar->lbar_current_move / p_listbar->config.item_height * p_listbar->lbar_page_col
                  + ((x - p_listbar->config.list_rect.x) / p_listbar->config.item_width) 
                  + ((y - p_listbar->config.list_rect.y) / p_listbar->config.item_height * p_listbar->lbar_page_col);           
            
            if(tmp < p_listbar->lbar_current_total)
            {   // �㵽��Ŀ�ڲ�                             
                p_listbar->lbar_focus_sta   = 1;
                p_listbar->lbar_current_focus = tmp;
                __listbar_show_page(p_listbar, DECODE_ITEM);
            }
            esKRNL_SemPost(p_listbar->lbar_dec_sem);
        }
        else if(p_listbar->lbar_touch_sta == 1)
        {   // ��ֹ����ʱ�ظ�����
            p_listbar->lbar_touch_sta   = 2;
        }
    }
    else
    {   // ��ǰ���ڻ���״̬
    	p_listbar->lbar_last_pos.x = x;
    	p_listbar->lbar_last_pos.y = y;
  
        if(p_listbar->lbar_touch_sta == 0)
        {   // ��������
            p_listbar->lbar_touch_sta   = 1;
        }
        else if(p_listbar->lbar_touch_sta == 1)
        {   // ������ͣ
            p_listbar->lbar_touch_sta   = 2;
    	    p_listbar->lbar_sld_speed   = -1;    	    
    	    
            p_listbar->lbar_current_move = (p_listbar->lbar_current_move + p_listbar->config.item_height / 2)
                                           / p_listbar->config.item_height * p_listbar->config.item_height;
            p_listbar->lbar_move = p_listbar->lbar_current_move % p_listbar->lbar_page_height;
            __listbar_move_page(p_listbar, 0);
            
            tmp = p_listbar->lbar_current_move / p_listbar->config.item_height * p_listbar->lbar_page_col
                  + ((x - p_listbar->config.list_rect.x) / p_listbar->config.item_width) 
                  + ((y - p_listbar->config.list_rect.y) / p_listbar->config.item_height * p_listbar->lbar_page_col);                  
            if(tmp < p_listbar->lbar_current_total)
            {   // �㵽��Ŀ�ڲ�
                p_listbar->lbar_focus_sta   = 1;
                p_listbar->lbar_current_focus = tmp;
                __listbar_draw_focus(p_listbar);
            }        
        }
    }

	//dsk_keytone_on();
    esKRNL_SemPost(p_listbar->lbar_sld_sem);
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				LISTBAR_TouchMove
*
*Description: �����ƶ�
*
*Arguments  : hdle: ��������� x: X�����꣬ y��Y������
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_TouchMove(H_LBAR hdle, __s32 x, __s32 y, __s32 direct, __s32 speed)
{
    __u8                err;
    __s32               step;
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle;
    
    if(p_listbar->lbar_current_total == 0)
    {   // ����Ŀ��Ϊ��ʱ��������Ӧ��
        return EPDK_OK;
    }    
    if(p_listbar->lbar_touch_sta == -1)
    {   
        return EPDK_FAIL;
    }

    esKRNL_SemPend(p_listbar->lbar_sld_sem, 0, &err);
    
    if(p_listbar->lbar_sld_speed == -1)
    {   // û�й��Ի���ʱ������С��10����������
        step = y - p_listbar->lbar_last_pos.y;
    
        if((step > 10) || (step < -10))
        { 
            p_listbar->lbar_touch_sta = 3;
            p_listbar->lbar_last_pos.x = x;
            p_listbar->lbar_last_pos.y = y;
            p_listbar->lbar_last_direct = step;
            
        	__listbar_move_page(p_listbar, step);
        }
    }
    else
    {   // �й��Ի���ʱ��������ǰ��������
        step = y - p_listbar->lbar_last_pos.y;
        
        if((step > 10) || (step < -10))
        { 
            p_listbar->lbar_touch_sta = 3;
            p_listbar->lbar_last_pos.x = x;
            p_listbar->lbar_last_pos.y = y;
            p_listbar->lbar_last_direct = step;
        }  
    }
    
    esKRNL_SemPost(p_listbar->lbar_sld_sem);
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				LISTBAR_TouchUp
*
*Description: ����̧��
*
*Arguments  : hdle: ��������� x: X�����꣬ y��Y������  
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_TouchUp(H_LBAR hdle, __s32 x, __s32 y, __s32 direct, __s32 speed)
{
    __u8                err;
     __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle;

    if(p_listbar->lbar_current_total == 0)
    {   // ����Ŀ��Ϊ��ʱ��������Ӧ��
        return EPDK_OK;
    }    
    if(p_listbar->lbar_touch_sta == -1)
    {   
        p_listbar->lbar_touch_sta = 0;
        
        return EPDK_FAIL;
    }

	p_listbar->lbar_last_pos.x = x;
	p_listbar->lbar_last_pos.y = y;
    
    if(p_listbar->lbar_touch_sta == 3)
    {   // �ǳ���̧��������Ի�����
        p_listbar->lbar_touch_sta = 0;
        
        esKRNL_SemPend(p_listbar->lbar_sld_sem, 0, &err);
        
        if(p_listbar->lbar_sld_speed == -1)
        {
            p_listbar->lbar_sld_speed = speed * 2;
        }
        else
        {
            p_listbar->lbar_sld_speed = p_listbar->lbar_sld_speed + speed;
            if(p_listbar->lbar_sld_speed > 200)
            {
                p_listbar->lbar_sld_speed = 200;
            }
        }
        if(p_listbar->lbar_sld_state == 1)
        {    
            p_listbar->lbar_sld_state = -1;
        }   
        
        esKRNL_SemPost(p_listbar->lbar_sld_sem);
    }
    else
    {
        p_listbar->lbar_touch_sta = 0;
        p_listbar->lbar_focus_sta = 1;
        if(p_listbar->lbar_sld_state == 1)
        {    
            p_listbar->lbar_sld_state = 0;
        } 
    } 

    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				LISTBAR_GetTotalItem
*
*Description: ��ȡ��ǰ������Ŀ
*
*Arguments  : hdle: �������
*             
*
*Return     : �����ܱ�ţ�ʧ�ܷ���-1��
*			  
*
************************************************************************************************************************
*/
__s32   LISTBAR_GetTotalItem(H_LBAR hdle)
{
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return -1;
    }    
    p_listbar = (__listbar_t *)hdle;
        
    return p_listbar->lbar_current_total;
}

/*
************************************************************************************************************************
*                       				LISTBAR_GetListAttr
*
*Description: ��ȡlistbar˽������
*
*Arguments  : hdle: �������
*             
*
*Return     : ����list˽�����ݣ�ʧ�ܷ���NULL��
*			  
*
************************************************************************************************************************
*/
void*   LISTBAR_GetListAttr(H_LBAR hdle)
{
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return NULL;
    }    
    p_listbar = (__listbar_t *)hdle;
        
    return p_listbar->config.list_attr;    
}

/*
************************************************************************************************************************
*                       				LISTBAR_GetFocusItem
*
*Description: ��ȡ��ǰ������Ŀ
*
*Arguments  : hdle: �������
*             
*
*Return     : ���ؽ�����Ŀ��ţ�ʧ�ܷ���-1��
*			  
*
************************************************************************************************************************
*/
__s32   LISTBAR_GetFocusItem(H_LBAR hdle)
{
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return -1;
    }    
    p_listbar = (__listbar_t *)hdle;
        
    return p_listbar->lbar_current_focus;
}

/*
************************************************************************************************************************
*                       				LISTBAR_GetItemRect
*
*Description: ��ȡָ����Ŀ��RECT(ֻ����������ʾ����Ŀ��)��
*
*Arguments  : hdle: ���������item��Ҫ��ȡ����Ŀ�ţ�rect����ȡ�������
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��	
*
************************************************************************************************************************
*/
__s32   LISTBAR_GetItemRect(H_LBAR hdle, __s32 item, RECT * rect)
{
    __s32               tmp;
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle;

    tmp = item - p_listbar->lbar_current_move / p_listbar->config.item_height * p_listbar->lbar_page_col;
    
    if((tmp < 0) || (tmp >= p_listbar->lbar_page_num))
    {
        __wrn("input item error !\n");
        return EPDK_FAIL;
    }
    
    rect->x      = p_listbar->lbar_rect.x + (tmp % p_listbar->lbar_page_col) * p_listbar->config.item_width;
    rect->y      = p_listbar->lbar_rect.y + (tmp / p_listbar->lbar_page_col) * p_listbar->config.item_height;
    rect->width  = p_listbar->config.item_width;
    rect->height = p_listbar->config.item_height;
    
    return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                             static void LISTBAR_LostFocus(__listbar_t * hdle)
*
* Description: ����ý���ĺ���ʧȥ����
*
* Arguments  :
*
* Returns    :
* Author	    :  libaiao, 2011.5.14
* Notes      :
*
**********************************************************************************************************************
*/
void LISTBAR_LostFocus(H_LBAR * hdle)
{
    FB					    fb;
    RECT                    fb_rect;
    RECT                    cp_rect;
    HBLOCK                  l_block;  
	__listbar_t*	        p_listbar;
    __lbar_draw_para_t      draw_para;

	if(hdle == NULL)
	{
		return;
	}
    p_listbar = (__listbar_t*)hdle;

    p_listbar->lbar_item = p_listbar->lbar_current_focus - 
                            p_listbar->lbar_current_move / p_listbar->config.item_height * p_listbar->lbar_page_col;
    if(p_listbar->lbar_item < 0)
    {
        p_listbar->lbar_item = 0;
    }  

    //if(p_listbar->lbar_focus_sta == 1)
    {
        p_listbar->lbar_focus_sta = 0;
                
        GUI_LyrWinSel(p_listbar->lwin_hdle);
        
        draw_para.rect.x      = p_listbar->lbar_rect.x + (p_listbar->lbar_item % p_listbar->lbar_page_col) * p_listbar->config.item_width;
        draw_para.rect.y      = p_listbar->lbar_rect.y + (p_listbar->lbar_item / p_listbar->lbar_page_col) * p_listbar->config.item_height;		
		
        draw_para.rect.width  = p_listbar->config.item_width;
        draw_para.rect.height = p_listbar->config.item_height;
		
        GUI_ClearRect(draw_para.rect.x, draw_para.rect.y,draw_para.rect.x + draw_para.rect.width, draw_para.rect.y+draw_para.rect.height);
		
        draw_para.index = p_listbar->lbar_current_focus;
        draw_para.attr  = p_listbar->config.list_attr;
        draw_para.mode = LBAR_MODE_NORMAL;
        
        p_listbar->lbar_draw(&draw_para);
    }
}


/*
************************************************************************************************************************
*                       				LISTBAR_SetFocusItem
*
*Description: ����һ����Ŀ������Ŀ
*
*Arguments  : hdle: ��������� item����Ŀ���
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_SetFocusItem(H_LBAR hdle, __s32 item)
{
    __u8                err;
    __s32               tmp_num;
    FB					fb;
    RECT                fb_rect;
    RECT                cp_rect;
    HBLOCK              l_block; 
    __listbar_t*	    p_listbar;
    __lbar_draw_para_t  draw_para;
    

    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle;
    
    if((item < 0) || (item >= p_listbar->lbar_current_total))
    {
        return EPDK_FAIL;
    }
    
    if(p_listbar->lbar_dec_state == DECODE_NULL)
    {
        p_listbar->lbar_current_focus   = item;
        p_listbar->lbar_current_page    = p_listbar->lbar_current_focus / p_listbar->lbar_page_num * p_listbar->lbar_page_num;
        if(p_listbar->lbar_current_move > p_listbar->lbar_current_focus / p_listbar->lbar_page_col * p_listbar->config.item_height)
        {
            p_listbar->lbar_current_move    = (p_listbar->lbar_current_focus + p_listbar->lbar_page_col - 1) 
                                              / p_listbar->lbar_page_col * p_listbar->config.item_height;            
        }
    }
    else
    {
        if((item >= (p_listbar->lbar_current_focus - p_listbar->lbar_item)) && 
           (item <= (p_listbar->lbar_current_focus - p_listbar->lbar_item + p_listbar->lbar_page_num - 1)))
        {
            tmp_num = item - (p_listbar->lbar_current_focus - p_listbar->lbar_item);
            p_listbar->lbar_current_focus = item;

			//---------------------------------------------------------------
			//add by libaiao
			p_listbar->lbar_focus_sta = 1;
		    //if(p_listbar->lbar_focus_sta == 1)
		    {
		        __listbar_draw_focus(p_listbar);
		    }
			//---------------------------------------------------------------
			/*
            GUI_LyrWinSel(p_listbar->item_layer);
            //GUI_LyrWinSel(p_listbar->lwin_hdle);			//modified by libaiao,2011.5.14
            GUI_SetBkColor(p_listbar->config.bk_color);
            GUI_Clear();
                                    
            draw_para.rect.x      = 0;
            draw_para.rect.y      = 0;
            draw_para.rect.width  = p_listbar->config.item_width;
            draw_para.rect.height = p_listbar->config.item_height;
        	        
            draw_para.index = p_listbar->lbar_current_focus;
            draw_para.attr  = p_listbar->config.list_attr;
            draw_para.mode  = LBAR_MODE_FOCUS;
            
            p_listbar->lbar_draw(&draw_para);
			*/
			/*
	        	GUI_LyrWinGetFB(p_listbar->item_layer, &fb);
	        	fb_rect.x = 0;
	        	fb_rect.y = 0;
	        	fb_rect.width = fb.size.width;
	        	fb_rect.height = fb.size.height;
	        	l_block = GUI_BlockCreateFrom(&fb_rect, fb.fmt.fmt.rgb.pixelfmt, fb.size.width, fb.size.height, fb.addr[0], fb.fmt.fmt.rgb.pixseq);
	        
	            cp_rect.x      = p_listbar->lbar_rect.x + (tmp_num % p_listbar->lbar_page_col) * p_listbar->config.item_width;
	            cp_rect.y      = p_listbar->lbar_rect.y + (tmp_num / p_listbar->lbar_page_col) * p_listbar->config.item_height;
	            cp_rect.width  = p_listbar->config.item_width;
	            cp_rect.height = p_listbar->config.item_height;	    		
	        
	        	GUI_BlockLayerBlit(p_listbar->lwin_hdle, &cp_rect, 0, 0, l_block, NULL, BLITFLAG_COLORSET, NULL);
	             GUI_BlockDelete(l_block);
            		*/
        }
        else
        {
            esKRNL_SemPend(p_listbar->lbar_dec_sem, 0, &err);
            
            p_listbar->lbar_current_focus   = item;
            p_listbar->lbar_current_page    = p_listbar->lbar_current_focus / p_listbar->lbar_page_num * p_listbar->lbar_page_num;
            p_listbar->lbar_current_move    = (p_listbar->lbar_current_focus + p_listbar->lbar_page_col - 1) 
                                              / p_listbar->lbar_page_col * p_listbar->config.item_height;
            p_listbar->lbar_focus_sta = 1;		//add by libaiao,2011.5.14
        	__listbar_show_page(p_listbar, DECODE_CURR);    
            //p_listbar->lbar_focus_sta = 0;		//add by libaiao,2011.5.14        
            esKRNL_SemPost(p_listbar->lbar_dec_sem); 
            
            tmp_num               = 0;
            
            GUI_LyrWinSel(p_listbar->item_layer);
            //GUI_LyrWinSel(p_listbar->lwin_hdle);			//modified by libaiao,2011.5.14
            GUI_SetBkColor(p_listbar->config.bk_color);
            GUI_Clear();
                                    
            draw_para.rect.x      = 0;
            draw_para.rect.y      = 0;
            draw_para.rect.width  = p_listbar->config.item_width;
            draw_para.rect.height = p_listbar->config.item_height;
        	        
            draw_para.index = p_listbar->lbar_current_focus;
            draw_para.attr  = p_listbar->config.list_attr;
            draw_para.mode  = LBAR_MODE_FOCUS;
            
            p_listbar->lbar_draw(&draw_para);

			/*
	        	GUI_LyrWinGetFB(p_listbar->item_layer, &fb);
	        	fb_rect.x = 0;
	        	fb_rect.y = 0;
	        	fb_rect.width = fb.size.width;
	        	fb_rect.height = fb.size.height;
	        	l_block = GUI_BlockCreateFrom(&fb_rect, fb.fmt.fmt.rgb.pixelfmt, fb.size.width, fb.size.height, fb.addr[0], fb.fmt.fmt.rgb.pixseq);
	        
	            cp_rect.x      = p_listbar->lbar_rect.x + (tmp_num % p_listbar->lbar_page_col) * p_listbar->config.item_width;
	            cp_rect.y      = p_listbar->lbar_rect.y + (tmp_num / p_listbar->lbar_page_col) * p_listbar->config.item_height;
	            cp_rect.width  = p_listbar->config.item_width;
	            cp_rect.height = p_listbar->config.item_height;	    		
	        
	        	GUI_BlockLayerBlit(p_listbar->lwin_hdle, &cp_rect, 0, 0, l_block, NULL, BLITFLAG_COLORSET, NULL);
	            GUI_BlockDelete(l_block);       
            		*/
        }
    }
    
    return EPDK_OK;    
}


/*
************************************************************************************************************************
*                       				LISTBAR_SetFocusItem
*
*Description: ����һ����Ŀ������Ŀ
*
*Arguments  : hdle: ��������� item����Ŀ���
*             
*author		: libaiao, û����ʾ��ֻ����focus item
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_SetFocusItemExt(H_LBAR hdle, __s32 item)
{
    __u8                err;
    __s32               tmp_num;
    FB					fb;
    RECT                fb_rect;
    RECT                cp_rect;
    HBLOCK              l_block; 
    __listbar_t*	    p_listbar;
    __lbar_draw_para_t  draw_para;
    

    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle;
    
    if((item < 0) || (item >= p_listbar->lbar_current_total))
    {
        return EPDK_FAIL;
    }
	
    esKRNL_SemPend(p_listbar->lbar_dec_sem, 0, &err);
    if(p_listbar->lbar_dec_state == DECODE_NULL)
    {    	
        p_listbar->lbar_current_focus   = item;
        p_listbar->lbar_current_page    = p_listbar->lbar_current_focus / p_listbar->lbar_page_num * p_listbar->lbar_page_num;
        if(p_listbar->lbar_current_move > p_listbar->lbar_current_focus / p_listbar->lbar_page_col * p_listbar->config.item_height)
        {
            p_listbar->lbar_current_move    = (p_listbar->lbar_current_focus + p_listbar->lbar_page_col - 1) 
                                              / p_listbar->lbar_page_col * p_listbar->config.item_height;            
        }
		
    }
    else
    {
        if((item >= (p_listbar->lbar_current_focus - p_listbar->lbar_item)) && 
           (item <= (p_listbar->lbar_current_focus - p_listbar->lbar_item + p_listbar->lbar_page_num - 1)))
        {
            tmp_num = item - (p_listbar->lbar_current_focus - p_listbar->lbar_item);
            p_listbar->lbar_current_focus = item;

			/*
            GUI_LyrWinSel(p_listbar->item_layer);
            //GUI_LyrWinSel(p_listbar->lwin_hdle);			//modified by libaiao,2011.5.14
            GUI_SetBkColor(p_listbar->config.bk_color);
            GUI_Clear();
                                    
            draw_para.rect.x      = 0;
            draw_para.rect.y      = 0;
            draw_para.rect.width  = p_listbar->config.item_width;
            draw_para.rect.height = p_listbar->config.item_height;
        	        
            draw_para.index = p_listbar->lbar_current_focus;
            draw_para.attr  = p_listbar->config.list_attr;
            draw_para.mode  = LBAR_MODE_FOCUS;
            
            p_listbar->lbar_draw(&draw_para);
			*/
			/*
	        	GUI_LyrWinGetFB(p_listbar->item_layer, &fb);
	        	fb_rect.x = 0;
	        	fb_rect.y = 0;
	        	fb_rect.width = fb.size.width;
	        	fb_rect.height = fb.size.height;
	        	l_block = GUI_BlockCreateFrom(&fb_rect, fb.fmt.fmt.rgb.pixelfmt, fb.size.width, fb.size.height, fb.addr[0], fb.fmt.fmt.rgb.pixseq);
	        
	            cp_rect.x      = p_listbar->lbar_rect.x + (tmp_num % p_listbar->lbar_page_col) * p_listbar->config.item_width;
	            cp_rect.y      = p_listbar->lbar_rect.y + (tmp_num / p_listbar->lbar_page_col) * p_listbar->config.item_height;
	            cp_rect.width  = p_listbar->config.item_width;
	            cp_rect.height = p_listbar->config.item_height;	    		
	        
	        	GUI_BlockLayerBlit(p_listbar->lwin_hdle, &cp_rect, 0, 0, l_block, NULL, BLITFLAG_COLORSET, NULL);
	             GUI_BlockDelete(l_block);
            		*/
        }
        else
        {
            //esKRNL_SemPend(p_listbar->lbar_dec_sem, 0, &err);
            
            p_listbar->lbar_current_focus   = item;
            p_listbar->lbar_current_page    = p_listbar->lbar_current_focus / p_listbar->lbar_page_num * p_listbar->lbar_page_num;
            p_listbar->lbar_current_move    = (p_listbar->lbar_current_focus + p_listbar->lbar_page_col - 1) 
                                              / p_listbar->lbar_page_col * p_listbar->config.item_height;
            //p_listbar->lbar_focus_sta = 1;		//add by libaiao,2011.5.14
        	//__listbar_show_page(p_listbar, DECODE_CURR);    
            //p_listbar->lbar_focus_sta = 0;		//add by libaiao,2011.5.14        
            //esKRNL_SemPost(p_listbar->lbar_dec_sem); 
            /*
	            tmp_num               = 0;
	            
	            GUI_LyrWinSel(p_listbar->item_layer);
	            //GUI_LyrWinSel(p_listbar->lwin_hdle);			//modified by libaiao,2011.5.14
	            GUI_SetBkColor(p_listbar->config.bk_color);
	            GUI_Clear();
	                                    
	            draw_para.rect.x      = 0;
	            draw_para.rect.y      = 0;
	            draw_para.rect.width  = p_listbar->config.item_width;
	            draw_para.rect.height = p_listbar->config.item_height;
	        	        
	            draw_para.index = p_listbar->lbar_current_focus;
	            draw_para.attr  = p_listbar->config.list_attr;
	            draw_para.mode  = LBAR_MODE_FOCUS;
	            
	            p_listbar->lbar_draw(&draw_para);
			*/
			/*
	        	GUI_LyrWinGetFB(p_listbar->item_layer, &fb);
	        	fb_rect.x = 0;
	        	fb_rect.y = 0;
	        	fb_rect.width = fb.size.width;
	        	fb_rect.height = fb.size.height;
	        	l_block = GUI_BlockCreateFrom(&fb_rect, fb.fmt.fmt.rgb.pixelfmt, fb.size.width, fb.size.height, fb.addr[0], fb.fmt.fmt.rgb.pixseq);
	        
	            cp_rect.x      = p_listbar->lbar_rect.x + (tmp_num % p_listbar->lbar_page_col) * p_listbar->config.item_width;
	            cp_rect.y      = p_listbar->lbar_rect.y + (tmp_num / p_listbar->lbar_page_col) * p_listbar->config.item_height;
	            cp_rect.width  = p_listbar->config.item_width;
	            cp_rect.height = p_listbar->config.item_height;	    		
	        
	        	GUI_BlockLayerBlit(p_listbar->lwin_hdle, &cp_rect, 0, 0, l_block, NULL, BLITFLAG_COLORSET, NULL);
	            GUI_BlockDelete(l_block);       
            		*/
        }
    }
    esKRNL_SemPost(p_listbar->lbar_dec_sem); 
    return EPDK_OK;    
}

/*
************************************************************************************************************************
*                       				LISTBAR_SetNormalItem
*
*Description: ���һ����ĿΪ��ͨ��Ŀ
*
*Arguments  : hdle: ��������� item����Ŀ���
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_SetNormalItem(H_LBAR hdle, __s32 item)
{
    __s32               tmp_num;
    FB					fb;
    RECT                fb_rect;
    RECT                cp_rect;
    HBLOCK              l_block; 
    __listbar_t*	    p_listbar;
    __lbar_draw_para_t  draw_para;
   

    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle;
    
    if((item < 0) || (item >= p_listbar->lbar_current_total))
    {
        return EPDK_FAIL;
    }
   
    if((item >= (p_listbar->lbar_current_focus - p_listbar->lbar_item)) && 
       (item <= (p_listbar->lbar_current_focus - p_listbar->lbar_item + p_listbar->lbar_page_num - 1)))
    {
        tmp_num     = item - (p_listbar->lbar_current_focus - p_listbar->lbar_item);

		//add by libaiao, 2011.5.14
        GUI_LyrWinSel(p_listbar->lwin_hdle);
        
        draw_para.rect.x      = p_listbar->lbar_rect.x + (p_listbar->lbar_item % p_listbar->lbar_page_col) * p_listbar->config.item_width;
        draw_para.rect.y      = p_listbar->lbar_rect.y + (p_listbar->lbar_item / p_listbar->lbar_page_col) * p_listbar->config.item_height;		
		
        draw_para.rect.width  = p_listbar->config.item_width;
        draw_para.rect.height = p_listbar->config.item_height;
            
        draw_para.index = p_listbar->lbar_current_focus;
        draw_para.attr  = p_listbar->config.list_attr;
		
        draw_para.mode  = LBAR_MODE_NORMAL;        
        p_listbar->lbar_draw(&draw_para);		
/*
        GUI_LyrWinSel(p_listbar->item_layer);
        GUI_SetBkColor(p_listbar->config.bk_color);
        GUI_Clear();
                                
        draw_para.rect.x      = 0;
        draw_para.rect.y      = 0;
        draw_para.rect.width  = p_listbar->config.item_width;
        draw_para.rect.height = p_listbar->config.item_height;
    	        
        draw_para.index = p_listbar->lbar_current_focus;
        draw_para.attr  = p_listbar->config.list_attr;
        draw_para.mode  = LBAR_MODE_NORMAL;
        
        p_listbar->lbar_draw(&draw_para);
*/
		/*
	    	GUI_LyrWinGetFB(p_listbar->item_layer, &fb);
	    	fb_rect.x = 0;
	    	fb_rect.y = 0;
	    	fb_rect.width = fb.size.width;
	    	fb_rect.height = fb.size.height;
	    	l_block = GUI_BlockCreateFrom(&fb_rect, fb.fmt.fmt.rgb.pixelfmt, fb.size.width, fb.size.height, fb.addr[0], fb.fmt.fmt.rgb.pixseq);
	    
	        cp_rect.x      = p_listbar->lbar_rect.x + (tmp_num % p_listbar->lbar_page_col) * p_listbar->config.item_width;
	        cp_rect.y      = p_listbar->lbar_rect.y + (tmp_num / p_listbar->lbar_page_col) * p_listbar->config.item_height;
	        cp_rect.width  = p_listbar->config.item_width;
	        cp_rect.height = p_listbar->config.item_height;	    		
	    
	    	GUI_BlockLayerBlit(p_listbar->lwin_hdle, &cp_rect, 0, 0, l_block, NULL, BLITFLAG_COLORSET, NULL);
	      GUI_BlockDelete(l_block);      
            */
    }
    return EPDK_OK;     
}

/*
************************************************************************************************************************
*                       				LISTBAR_SetInvalidItem
*
*Description: ���һ����ĿΪ��Ч��Ŀ
*
*Arguments  : hdle: ��������� item����Ŀ���
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_SetInvalidItem(H_LBAR hdle, __s32 item)
{
    __s32               tmp_num;
    __listbar_t*	    p_listbar;
    __lbar_draw_para_t  draw_para;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle;
    
    if((item >= (p_listbar->lbar_current_focus - p_listbar->lbar_item)) && 
       (item <= (p_listbar->lbar_current_focus - p_listbar->lbar_item + p_listbar->lbar_page_num - 1)))
    {
        GUI_LyrWinSel(p_listbar->lwin_hdle);
        
        if(p_listbar->config.alpha_status == 1)
        {
            GUI_OpenAlphaBlend();
        }
        
        tmp_num               = item - (p_listbar->lbar_current_focus - p_listbar->lbar_item);
        draw_para.rect.x      = p_listbar->lbar_rect.x + (tmp_num % p_listbar->lbar_page_col) * p_listbar->config.item_width;
        draw_para.rect.y      = p_listbar->lbar_rect.y + (tmp_num / p_listbar->lbar_page_col) * p_listbar->config.item_height;
        draw_para.rect.width  = p_listbar->config.item_width;
        draw_para.rect.height = p_listbar->config.item_height;

        draw_para.index = item;
        draw_para.attr  = p_listbar->config.list_attr;     
        draw_para.mode  = LBAR_MODE_INVALID;

        p_listbar->lbar_draw(&draw_para);
        
        if(p_listbar->config.alpha_status == 1)
        {
            GUI_LyrWinFlushFB(p_listbar->lwin_hdle);
            GUI_CloseAlphaBlend();
        }        
    }
          
    return EPDK_OK;     
}

/*
************************************************************************************************************************
*                       				LISTBAR_SetTotalItem
*
*Description: �����ܵ���Ŀ��
*
*Arguments  : hdle: ��������� num����Ŀ����
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_SetTotalItem(H_LBAR hdle, __u32 num)
{
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    

	LISTBAR_SuspendPage(hdle);				//add by libaiao,2011.4.22
	
    p_listbar = (__listbar_t *)hdle;    
    p_listbar->lbar_current_total = num; 
	
	LISTBAR_ResumePage(hdle);				//add by libaiao,2011.4.22
	
	LISTBAR_ShowPage(hdle);	
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				LISTBAR_GetListAttr
*
*Description: ��ȡlistbar˽������
*
*Arguments  : hdle: ���������attr���û�˽������
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��	  
*
************************************************************************************************************************
*/
__s32   LISTBAR_SetListAttr(H_LBAR hdle, void * attr)
{
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle;
        
    p_listbar->config.list_attr = attr;
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				LISTBAR_ChangeMode
*
*Description: �ı䵱ǰ��ʾ��ʽ
*
*Arguments  : hdle: ���������item_width����Ŀ�Ŀ�ȣ�item_height����Ŀ�ĸ߶�
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_ChangeMode(H_LBAR hdle, __s32 item_width, __s32 item_height)
{
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle; 

	LISTBAR_SuspendPage(hdle);			//add by libaiao,2011.4.22
    // �����µ���Ŀ�����Ϣ
    p_listbar->config.item_width  = item_width;
    p_listbar->config.item_height = item_height;

    if(p_listbar->item_layer)
    {   // ɾ��ITEM����ͼ��
        GUI_LyrWinDelete(p_listbar->item_layer);
        p_listbar->item_layer = NULL;
    }    
    p_listbar->item_layer = __listbar_layer_create(p_listbar->config.item_width, p_listbar->config.item_height, 0);
    
    // ���¼��㵱ǰ���м���ҳ��Ŀ����Ϣ
    p_listbar->lbar_page_col        = p_listbar->lbar_rect.width / p_listbar->config.item_width;
    p_listbar->lbar_page_row        = p_listbar->lbar_rect.height / p_listbar->config.item_height;
    p_listbar->lbar_page_num        = p_listbar->lbar_page_row * p_listbar->lbar_page_col;
    p_listbar->lbar_page_width      = p_listbar->lbar_page_col * p_listbar->config.item_width;
    p_listbar->lbar_page_height     = p_listbar->lbar_page_row * p_listbar->config.item_height;
    
    // ������ǰ�����ֳ�
    p_listbar->lbar_current_page    = p_listbar->lbar_current_focus / p_listbar->lbar_page_num * p_listbar->lbar_page_num;
    p_listbar->lbar_current_move    = p_listbar->lbar_current_focus / p_listbar->lbar_page_col * p_listbar->config.item_height;

	LISTBAR_ResumePage(hdle);			//add by libaiao,2011.4.22    	
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				LISTBAR_SuspendPage
*
*Description: ����ǰ��ʾ����
*
*Arguments  : hdle: �������  
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_SuspendPage(H_LBAR hdle)
{
    __u8                err;
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle; 
    
    esKRNL_SemPend(p_listbar->lbar_sld_sem, 0, &err);
        
    // ֹͣ�����߳�
    if(p_listbar->lbar_sld_speed != -1)
    {
        p_listbar->lbar_sld_speed   = -1;    	    
        
        p_listbar->lbar_current_move = (p_listbar->lbar_current_move + p_listbar->config.item_height / 2)
                                       / p_listbar->config.item_height * p_listbar->config.item_height;
        p_listbar->lbar_move = p_listbar->lbar_current_move % p_listbar->lbar_page_height;
        __listbar_move_page(p_listbar, 0);
        
        p_listbar->lbar_focus_sta   = 1;
        p_listbar->lbar_current_focus = p_listbar->lbar_current_move / p_listbar->config.item_height * p_listbar->lbar_page_col;
        
        __listbar_draw_focus(p_listbar);
    }
    
    esKRNL_SemPost(p_listbar->lbar_sld_sem);
    esKRNL_SemPend(p_listbar->lbar_dec_sem, 0, &err);
    // ֹͣԤ��
    p_listbar->lbar_dec_state = DECODE_NULL;
    esKRNL_SemPost(p_listbar->lbar_dec_sem);
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				LISTBAR_ResumePage
*
*Description: �ָ���ǰ��ʾ����
*
*Arguments  : hdle: �������  
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_ResumePage(H_LBAR hdle)
{
    __u8                err;
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle; 

    esKRNL_SemPend(p_listbar->lbar_dec_sem, 0, &err);
    
    // ����Ԥ���̣߳�����ˢ���ѻ��ƺõ�ҳ�棬���Ҹ��½�����
    p_listbar->lbar_dec_state = DECODE_ITEM;
    __listbar_show_scroll(p_listbar);
    
    esKRNL_SemPost(p_listbar->lbar_dec_sem); 
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				LISTBAR_UpdatePage
*
*Description: ���µ�ǰ��ʾ����
*
*Arguments  : hdle: �������  
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/

__s32   LISTBAR_UpdatePage(H_LBAR hdle)
{
    __u8                err;
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle; 
    
    esKRNL_SemPend(p_listbar->lbar_dec_sem, 0, &err);
    
    // ˢ��Ԥ���̣߳�����ˢ��ҳ����ʾ
    p_listbar->lbar_focus_sta   = 1;

    __listbar_show_page(p_listbar, DECODE_CURR);

    esKRNL_SemPost(p_listbar->lbar_dec_sem); 
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				LISTBAR_SetScene
*
*Description: ����listbar�ڲ��ֳ�
*
*Arguments  : hdle: ��������� scene���ֳ�����
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_SetScene(H_LBAR hdle, __listbar_scene_t * scene)
{
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle; 
  
    if(scene->start_id == -1)
    {
        p_listbar->lbar_current_page    = scene->focus_id / p_listbar->lbar_page_num * p_listbar->lbar_page_num;
        p_listbar->lbar_current_focus   = scene->focus_id;
        p_listbar->lbar_current_move    = scene->focus_id / p_listbar->lbar_page_col * p_listbar->config.item_height;        
    }
    else
    {
        p_listbar->lbar_current_page    = scene->start_id / p_listbar->lbar_page_num * p_listbar->lbar_page_num;
        p_listbar->lbar_current_focus   = scene->focus_id;
        p_listbar->lbar_current_move    = scene->start_id / p_listbar->lbar_page_col * p_listbar->config.item_height;
    }
	
    return EPDK_OK;	
}

/*
************************************************************************************************************************
*                       				LISTBAR_GetScene
*
*Description: ��ȡlistbar�ڲ��ֳ�
*
*Arguments  : hdle: ��������� scene���ֳ�����
*             
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   LISTBAR_GetScene(H_LBAR hdle, __listbar_scene_t * scene)
{
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle; 
    
    scene->start_id     = p_listbar->lbar_current_move / p_listbar->config.item_height * p_listbar->lbar_page_col;
    scene->focus_id		= p_listbar->lbar_current_focus;
    
    return EPDK_OK;	
}

/*
**********************************************************************************************************************
*                                               LISTBAR_Delete
*
* Description: ɾ��listbar�������
*
* Arguments  : hdle���������
*
* Returns    : EPDK_OK / EPDK_FAIL

* Notes      :
*
**********************************************************************************************************************
*/
__s32   LISTBAR_Delete(H_LBAR hdle)
{
    __u32               i;
    __u8                err;
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }    
    p_listbar = (__listbar_t *)hdle; 

    if(p_listbar->lbar_dif_tsk)
    {	// ɾ�������߳�
		while(1)
		{
			if(esKRNL_TDelReq(p_listbar->lbar_dif_tsk) == OS_TASK_NOT_EXIST)
		    {
				break;
			}
			esKRNL_TimeDly(1);
		}
		p_listbar->lbar_dif_tsk = 0;
	}  
	if(p_listbar->lbar_dif_sem)
	{
	    esKRNL_SemDel(p_listbar->lbar_dif_sem, OS_DEL_ALWAYS, &err);  
		p_listbar->lbar_dif_sem = NULL;

	}
    if(p_listbar->lbar_sld_tsk)
    {	// ɾ�������߳�
		while(1)
		{
			if(esKRNL_TDelReq(p_listbar->lbar_sld_tsk) == OS_TASK_NOT_EXIST)
		    {
				break;
			}
			esKRNL_TimeDly(1);
		}
		p_listbar->lbar_sld_tsk = 0;
	}
	if(p_listbar->lbar_sld_sem)
	{   // ɾ�������̷߳��ʿ����ź���
    	esKRNL_SemDel(p_listbar->lbar_sld_sem, OS_DEL_ALWAYS, &err);  
		p_listbar->lbar_sld_sem = NULL;
	}
	if(p_listbar->lbar_dec_tsk)
    {	// ɾ��Ԥ���߳�
		while(1)
		{
			if(esKRNL_TDelReq(p_listbar->lbar_dec_tsk) == OS_TASK_NOT_EXIST)
			{
				break;
			}
			esKRNL_TimeDly(1);
		}
		p_listbar->lbar_dec_tsk = 0;
	}
	if(p_listbar->lbar_dec_sem)
	{   // ɾ��Ԥ���̷߳��ʿ����ź���
    	esKRNL_SemDel(p_listbar->lbar_dec_sem, OS_DEL_ALWAYS, &err);  
		p_listbar->lbar_dec_sem = NULL;
	}
    if(p_listbar->item_layer)
    {   // ɾ��ITEM����ͼ��
        GUI_LyrWinDelete(p_listbar->item_layer);
        p_listbar->item_layer = NULL;
    }	
    for(i = 0; i < PAGE_NUM; i++)
    {   // ɾ������ͼ��
        if(p_listbar->lbar_page[i].page_lyr)
        {
            GUI_LyrWinDelete(p_listbar->lbar_page[i].page_lyr);
            p_listbar->lbar_page[i].page_lyr = NULL;
        }
    }
    if(p_listbar->scroll_b_layer)
    {   // ɾ����������������ͼ��
        GUI_LyrWinDelete(p_listbar->scroll_b_layer);
        p_listbar->scroll_b_layer = NULL;
    }
    if(p_listbar->scroll_f_layer)
    {   // ɾ��������ǰ������ͼ��
        GUI_LyrWinDelete(p_listbar->scroll_f_layer);
        p_listbar->scroll_f_layer = NULL;
    }    

    esMEMS_Mfree(0, p_listbar);
    p_listbar = NULL;
    
    return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                               LISTBAR_GetState
*
* Description: ��ȡ��ǰ����״̬
*
* Arguments  : hdle���������
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
**********************************************************************************************************************
*/
__lbar_state_e   LISTBAR_GetState(H_LBAR hdle)
{
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return LBAR_STATE_STOP;
    }    
    p_listbar = (__listbar_t *)hdle; 
    
    if(p_listbar->lbar_sld_state == 0)
	{
	    return LBAR_STATE_STOP;
	}

	return LBAR_STATE_MOVING;
}

/*
**********************************************************************************************************************
*                                               LISTBAR_GetItemNo
*
* Description: ��ȡָ��λ�õ���Ŀ��
*
* Arguments  : hdle���������
*
*Return     : EPDK_OK: �ɹ�
*			  EPDK_FAIL: ʧ��
*
**********************************************************************************************************************
*/
__s32   LISTBAR_GetItemNo(H_LBAR hdle, __pos_t pos)
{
    __s32               num;
    __listbar_t*	    p_listbar;
    
    if(hdle == NULL)
    {
        return -1;
    }    
    p_listbar = (__listbar_t *)hdle;
    
    num = p_listbar->lbar_current_move / p_listbar->config.item_height * p_listbar->lbar_page_col
          + ((pos.x - p_listbar->config.list_rect.x) / p_listbar->config.item_width) 
          + ((pos.y - p_listbar->config.list_rect.y) / p_listbar->config.item_height * p_listbar->lbar_page_col);
    
    return num;
}

/*
**********************************************************************************************************************
*                                               LISTBAR_GetPageItemCount
*
* Description	: ���ÿҳ�ж��ٸ���Ŀ
*
* Arguments  : hdle���������
*
*Return     	:��Ŀ��
*			  EPDK_FAIL: ʧ��
*
*Author		: add by Libaiao, 2011.04.21
**********************************************************************************************************************
*/
__s32 LISTBAR_GetPageItemCount(H_LBAR handle)
{
	__listbar_t*	    p_listbar;
	
    if(handle == NULL)
    {
    	return EPDK_FAIL;
    }
	p_listbar = (__listbar_t *)handle;
	
	return 	p_listbar->lbar_page_num;
}  

/*
**********************************************************************************************************************
*                                               LISTBAR_GetRowItemNumber
*
* Description	: ���ÿ���ж��ٸ���Ŀ
*
* Arguments  : hdle���������
*
*Return     	: ��Ŀ��
*			  EPDK_FAIL: ʧ��
*
*Author		: add by Libaiao, 2011.04.21
**********************************************************************************************************************
*/
__s32 LISTBAR_GetRowItemCount(H_LBAR handle)
{
	__listbar_t*	    p_listbar;
	
    if(handle == NULL)
    {
    	return EPDK_FAIL;
    }
	p_listbar = (__listbar_t *)handle;
	
	return 	p_listbar->lbar_page_col;
}   

__s32 LISTBAR_Suspend_dif_task(H_LBAR handle)
{
    __u8 			    err; 
    OS_SEM_DATA         pdata;
	__listbar_t*	    p_listbar;

	if(handle == NULL)
    {
    	return EPDK_FAIL;
    }
	p_listbar = (__listbar_t *)handle;

	esKRNL_SemQuery(p_listbar->lbar_dif_sem, &pdata);
	if(pdata.OSCnt == 1)
	{
		__here__ ;
		esKRNL_SemPend(p_listbar->lbar_dif_sem, 0, &err);
	}
}   
__s32 LISTBAR_Resume_dif_task(H_LBAR handle)
{
    __u8 			    err; 
    OS_SEM_DATA         pdata;
	__listbar_t*	    p_listbar;

	if(handle == NULL)
    {
    	return EPDK_FAIL;
    }
	p_listbar = (__listbar_t *)handle;

	esKRNL_SemQuery(p_listbar->lbar_dif_sem, &pdata);
	if(pdata.OSCnt == 0)
	{
	__here__ ;
   		esKRNL_SemPost( p_listbar->lbar_dif_sem );
	}
}   


