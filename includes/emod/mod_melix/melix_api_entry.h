/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, DEREK, China
*											 All Rights Reserved
*
* File    		: orange_api_entry.h
* By      		: derek.lai
* Version	: v1.0
* ============================================================================================================
*/

#ifndef __MOD_MELIX_API_H__
#define __MOD_MELIX_API_H__

typedef struct MELIXMOD_API_ENTRY
{
	Activity			(*ptr_gui_activity_register)(gui_activity_create_t *para);
	__s32				(*ptr_gui_activity_unregister)(Activity hdle);
	__s32				(*ptr_gui_activity_push)(Activity hdle, gui_animation_mode_e animation_mode);
	__s32				(*ptr_gui_activity_pop)(Activity hdle, gui_animation_mode_e animation_mode);
	Hview				(*ptr_gui_activity_get_rootview)(Activity hdle);
	HXFB				(*ptr_gui_get_top_activity_xfb)(void);
	HXFB				(*ptr_gui_get_second_activity_xfb)(void);
	__s32				(*ptr_gui_activity_update)(void);
	Hview				(*ptr_gui_gview_create)(RECT view_rect);
	__s32				(*ptr_gui_gview_delete)(Hview g_view_hdle);
	__s32				(*ptr_gui_gview_add_view)(Hview g_view_hdle, Hview view, GUI_EVENT_PROCESS proc);
	__s32				(*ptr_gui_gview_remove_view)(Hview g_view_hdle, Hview view);
	__s32				(*ptr_gui_gview_set_focus_view)(Hview g_view_hdle, Hview view);
	Hview				(*ptr_gui_gview_get_focus_view)(Hview g_view_hdle);
	void*				(*ptr_gui_get_view_proc)(Hview view);
	__s32				(*ptr_gui_get_message)(gui_msg_t *msg);
	__s32				(*ptr_gui_dispatch_message)(gui_msg_t *msg, pGUI_MSG_CB msg_call_back);
	__s32				(*ptr_gui_send_message)(gui_msg_t *msg);
	__s32				(*ptr_gui_post_message)(gui_msg_t *msg);
	__bool 				(*ptr_gui_set_timer)(Hview view, __u16 id, __u32 speed, pGUI_TIMERPROC timer_proc);
	__s32 				(*ptr_gui_kill_timer)(Hview view, __u16 id);
	__bool  			(*ptr_gui_reset_timer)(Hview view, __u16 id, __u32 speed, pGUI_TIMERPROC timer_proc);
	__bool 				(*ptr_gui_is_timer_installed)(Hview view, __u16 id);
	__bool 				(*ptr_gui_have_free_timer)(void);
	__u32 				(*ptr_gui_get_tick_count)(void);
	__u32 				(*ptr_gui_get_timer_expired)(Hview view, __u16 id);
	__s32				(*ptr_gui_get_timer_slot)(Hview view, __u16 id);
	__s32 				(*ptr_gui_uc_calc_char_size)(__u16 Char);
	__u16 				(*ptr_gui_uc_get_char_codeinc)(const char ** ps);
	__s32 				(*ptr_gui_uc_numchars_to_numbytes)(const char * s, __s32 NumChars);
	__s32 				(*ptr_gui_uc_numbytes_to_numchars)(const char * s, __s32 NumBytes);
	__s32 				(*ptr_gui_uc_get_char_size)(const char * s);
	__u16				(*ptr_gui_uc_get_char_code)(const char * s);
	__s32				(*ptr_gui_uc_encode)(char* s, __u16 Char);
	GUI_FONT* 			(*ptr_gui_get_default_font)(void);
	__u8 				(*ptr_gui_set_frame_color_8bpp32)(__u8 frameColor);
	GUI_FONT*			(*ptr_gui_sft_create_font)( unsigned int  pixelSize,const char  *font_file );
	void				(*ptr_gui_sft_release_font)(GUI_FONT *pFont);
	GUI_FONT*  			(*ptr_gui_ttf_create_font)( GUI_TTF_ATTR * pTTF_ATTR);
	void				(*ptr_gui_ttf_done)(GUI_FONT *pFont);
	HDC					(*ptr_gui_create_hdc)(void);
	__s32				(*ptr_gui_destroy_hdc)(HDC hdc);
	__s32				(*ptr_gui_select_back_color)(HDC hdc, __u32 color);
	__u32				(*ptr_gui_get_back_color)(HDC hdc);
	__s32				(*ptr_gui_select_pen_color)(HDC hdc, __u32 color);
	__u32				(*ptr_gui_get_pen_color)(HDC hdc);
	__s32				(*ptr_gui_select_text_color)(HDC hdc, __u32 color);
	__u32				(*ptr_gui_get_text_color)(HDC hdc);
	__s32				(*ptr_gui_select_font)(HDC hdc, GUI_FONT *font);
	GUI_FONT*			(*ptr_gui_get_font)(HDC hdc);
	__s32				(*ptr_gui_select_char_set)(HDC hdc, GuiCharset char_set);
	GuiCharset			(*ptr_gui_get_char_set)(HDC hdc);
	__s32				(*ptr_gui_select_draw_mode)(HDC hdc, __u8 draw_mode);
	__u8				(*ptr_gui_get_draw_mode)(HDC hdc);
	__s32				(*ptr_gui_select_font_mode)(HDC hdc, __u8 font_mode);
	__u8				(*ptr_gui_get_font_mode)(HDC hdc);
	__s32				(*ptr_gui_select_text_style)(HDC hdc, __u8 style);
	__u8				(*ptr_gui_get_text_style)(HDC hdc);
	__s32				(*ptr_gui_select_text_mode)(HDC hdc, __u32 text_mode);
	__u32				(*ptr_gui_get_text_mode)(HDC hdc);
	__s32				(*ptr_gui_select_text_align)(HDC hdc, __u32 text_align);
	__u32				(*ptr_gui_get_text_align)(HDC hdc);
	__pos_t				(*ptr_gui_get_text_pos)(HDC hdc);
	__s32				(*ptr_gui_select_text_pos)(HDC hdc, __pos_t pos);
	__s32				(*ptr_gui_draw_bitmap)(HDC hdc,HBITMAP hbitmap, __s32 x, __s32 y);
	void				(*ptr_gui_draw_text_at)(HDC hdc, char *s, int x, int y);
	void				(*ptr_gui_draw_text_in_rect)(HDC hdc, char *s, RECT* pRect, int TextAlign);
	void				(*ptr_gui_draw_text_in_rect_wrap)(HDC hdc, char *s, RECT* pRect, int TextAlign, gui_wrap_mode_t WrapMode);
	__s32				(*ptr_gui_draw_circle)(HDC hdc, __s32 x0, __s32 y0, __s32 r);
	__s32				(*ptr_gui_fill_rect)(HDC hdc, RECT fill_rect);
	HRES  				(*ptr_gui_open_res)( char * szAppFile, __u32 mode);
	__s32				(*ptr_gui_close_res)(HRES hRes);
	__u32				(*ptr_gui_read_res)(HRES hRes, __u32 address, __u32 length, void * buffer);
	__u32				(*ptr_gui_get_res_size)(HRES hRes, __u16 StyleID, __u16 ID);
	__u32				(*ptr_gui_get_res_addr)(HRES hRes, __u16 StyleID, __u16 ID);
	__u16				(*ptr_gui_get_res_type)(HRES hRes, __u16 StyleID, __u16 ID);
	__s32				(*ptr_gui_get_res)(HRES hRes, __u16 StyleID, __u16 ID, void * Buffer, __u32 Length);
	HLANG				(*ptr_gui_lang_open)(char *szAppFile, __u32 mode);
	int					(*ptr_gui_lang_read)(HLANG hLang, int address, int length, char *buffer);
	int 				(*ptr_gui_lang_get_string_address)(HLANG hLang, short LangID, short StringID);
	int 				(*ptr_gui_lang_get_string_size)(HLANG hLang, short LangID, short StringID);
	int 				(*ptr_gui_lang_get_string)(HLANG hLang, short LangID, short StringID, char *buffer, int length);
	int 				(*ptr_gui_lang_close)(HLANG hLang);
	HBUTTON				(*ptr_gui_button_create)(gui_button_create_para_t * para);
	__s32				(*ptr_gui_button_delete)(HBUTTON hbutton);
	__s32				(*ptr_gui_button_paint)(HBUTTON hbutton);
	void 				(*ptr_gui_button_set_paint_flag)(gui_button_paint_flag_e flag);
	__u32 				(*ptr_gui_get_scn_width)(void);
	__u32 				(*ptr_gui_get_scn_height)(void);
	void				(*ptr_gui_msg_looper)(void);
	__s32				(*ptr_gui_activity_set_add_data)(Activity hdle, __u32 add_data);
	__u32 				(*ptr_gui_activity_get_add_data)(Activity hdle);
	__s32				(*ptr_gui_view_set_add_data)(Hview view, __u32 add_data);
	__u32				(*ptr_gui_view_get_add_data)(Hview view);
	__u32				(*ptr_gui_get_res_them)(HRES h_them, void **buff, __u16 StyleID, __u16 id);
	void				(*ptr_gui_free_res_them)(void *buff, __u32 size);
	__s32				(*ptr_gui_set_palette)(__u32 * pal_table);
	__s32				(*ptr_gui_select_dst_xfb)(HDC hdc, __u32 xfb_addr, __u32 xfb_width, __u32 xfb_height, gui_pixel_fmt_e pixel_fmt);
	void				(*ptr_gdi_set_rect_mem)(HDC hdc, RECT *pDestRect, void *pBuffer, __u32 bufferXSize, __u32 bufferYSize, __u32 rectX0, __u32 rectY0);
	void				(*ptr_gui_get_rect_mem)(HDC hdc, RECT *pSrcRect, void *pBuffer, __u32 bufferXSize, __u32 bufferYSize, __u32 rectX0, __u32 rectY0);
	__s32				(*ptr_gui_rotate_buffer)(__u32 dest_addr, __u32 src_addr, __s32 src_width, __s32 src_height, GuiDisplayDirection angle);
	HLIST				(*ptr_gui_list_create)(gui_list_para_t * para);
	__s32				(*ptr_gui_list_config)(HLIST hdle, gui_list_config_t * config);
	__s32				(*ptr_gui_list_scroll_config)(HLIST hdle, gui_scroll_bar_t * scroll_para);
	__s32				(*ptr_gui_list_delete)(HLIST hdle);
	__s32				(*ptr_gui_list_show_page)(HLIST hdle);
	__s32				(*ptr_gui_list_touch_up)(HLIST hdle, __s32 x, __s32 y, __s32 direct, __s32 speed);
	__s32				(*ptr_gui_list_touch_down)(HLIST hdle, __s32 x, __s32 y, __u32 down_type);
	__s32				(*ptr_gui_list_touch_move)(HLIST hdle, __s32 x, __s32 y);
	__s32				(*ptr_gui_list_next_page)(HLIST hdle);
	__s32				(*ptr_gui_list_prev_page)(HLIST hdle);
	__s32				(*ptr_gui_list_next_item)(HLIST hdle);
	__s32				(*ptr_gui_list_prev_item)(HLIST hdle);
	__u32				(*ptr_gui_list_get_buf_addr)(HLIST hdle);
	__u32				(*ptr_gui_list_get_buf_width)(HLIST hdle);	
	__u32				(*ptr_gui_list_get_buf_height)(HLIST hdle);
	__u32				(*ptr_gui_list_get_attr)(HLIST hdle);
	__s32				(*ptr_gui_draw_hline)(HDC hdc, __s32 y0, __s32 x0, __s32 x1);
	__s32				(*ptr_gui_draw_vline)(HDC hdc, __s32 x0, __s32 y0, __s32 y1);
	__s32				(*ptr_gui_draw_bitstring)(HDC hdc, __u32 buf_addr, __u32 buf_width, __u32 buf_height, __s32 x, __s32 y);
	void				(*ptr_gui_set_animation_invalid_height)(__s32 height);
	__s32				(*ptr_gui_fill_circle)(HDC hdc, __s32 x0, __s32 y0, __s32 r);
	__s32				(*ptr_gui_draw_rect)(HDC hdc, RECT draw_rect);
	__s32 				(*ptr_gui_draw_line)(HDC hdc, __s32 x0, __s32 y0, __s32 x1, __s32 y1);
	__s32 				(*ptr_gui_draw_ellipse)(HDC hdc, __s32 x0, __s32 y0, __s32 rx, __s32 ry);
	__s32 				(*ptr_gui_fill_ellipse)(HDC hdc, __s32 x0, __s32 y0, __s32 rx, __s32 ry);
	__s32 				(*ptr_gui_draw_poly_line)(HDC hdc, GUI_POINT* pPoints, __s32 NumPoints, __s32 x0, __s32 y0);
	__s32 				(*ptr_gui_draw_polygon)(HDC hdc, GUI_POINT* pPoints, __s32 NumPoints, __s32 x0, __s32 y0);
	__s32 				(*ptr_gui_fill_polygon)(HDC hdc, GUI_POINT* pPoints, __s32 NumPoints, __s32 x0, __s32 y0);
	__s32 				(*ptr_gui_draw_point)(HDC hdc, __s32 x, __s32 y);
	__s32 				(*ptr_gui_draw_line_to)(HDC hdc, __s32 x, __s32 y);
	__s32 				(*ptr_gui_move_to)(HDC hdc, __s32 x, __s32 y);
	__s32 				(*ptr_gui_move_rel)(HDC hdc, __s32 dx, __s32 dy);
	__s32 				(*ptr_gui_draw_arc)(HDC hdc, __s32 x0, __s32 y0, __s32 rx, __s32 ry, __s32 a0, __s32 a1);
	__s32 				(*ptr_gui_draw_pie)(HDC hdc, __s32 x0, __s32 y0, __s32 r, __s32 a0, __s32 a1, __s32 type);
	__s32				(*ptr_gui_select_pen_size)(HDC hdc, __u8 size);
	__s32				(*ptr_gui_select_line_style)(HDC hdc, __u8 style);
} melixmod_api_entry_t;

#ifndef USED_BY_MELIXMOD	//not used by orangemod module itsef
#define MELIXMOD_ENTRY_TBL   (0xe1800000)   //this address is depended on the link address of mod_orangemod
#define MELIXMOD_FUNC_ENTRY  ((melixmod_api_entry_t *)MELIXMOD_ENTRY_TBL)
#define gui_activity_register  	                        (MELIXMOD_FUNC_ENTRY->ptr_gui_activity_register    	    	)
#define gui_activity_unregister                         (MELIXMOD_FUNC_ENTRY->ptr_gui_activity_unregister    	    	)
#define gui_activity_push                           	(MELIXMOD_FUNC_ENTRY->ptr_gui_activity_push       	)
#define gui_activity_pop	                           	(MELIXMOD_FUNC_ENTRY->ptr_gui_activity_pop       	)
#define gui_activity_get_rootview                      	(MELIXMOD_FUNC_ENTRY->ptr_gui_activity_get_rootview       	)
#define gui_get_top_activity_xfb                       	(MELIXMOD_FUNC_ENTRY->ptr_gui_get_top_activity_xfb      	)
#define gui_get_second_activity_xfb                    	(MELIXMOD_FUNC_ENTRY->ptr_gui_get_second_activity_xfb             	)
#define gui_activity_update                            	(MELIXMOD_FUNC_ENTRY->ptr_gui_activity_update           	)
#define gui_gview_create	                           	(MELIXMOD_FUNC_ENTRY->ptr_gui_gview_create      	)
#define gui_gview_delete                               	(MELIXMOD_FUNC_ENTRY->ptr_gui_gview_delete           	)
#define gui_gview_add_view                              (MELIXMOD_FUNC_ENTRY->ptr_gui_gview_add_view           	)
#define gui_gview_remove_view                          	(MELIXMOD_FUNC_ENTRY->ptr_gui_gview_remove_view           	)
#define gui_gview_set_focus_view                       	(MELIXMOD_FUNC_ENTRY->ptr_gui_gview_set_focus_view       	)
#define gui_gview_get_focus_view                       	(MELIXMOD_FUNC_ENTRY->ptr_gui_gview_get_focus_view          	)
#define gui_get_view_proc                              	(MELIXMOD_FUNC_ENTRY->ptr_gui_get_view_proc          	)
#define gui_get_message                                	(MELIXMOD_FUNC_ENTRY->ptr_gui_get_message             	)
#define gui_dispatch_message                           	(MELIXMOD_FUNC_ENTRY->ptr_gui_dispatch_message          	)
#define gui_send_message	                        	(MELIXMOD_FUNC_ENTRY->ptr_gui_send_message    	)
#define gui_post_message		                       	(MELIXMOD_FUNC_ENTRY->ptr_gui_post_message   	)
#define gui_set_timer		                       		(MELIXMOD_FUNC_ENTRY->ptr_gui_set_timer   	)
#define gui_kill_timer		                       		(MELIXMOD_FUNC_ENTRY->ptr_gui_kill_timer   	)
#define gui_reset_timer		                       		(MELIXMOD_FUNC_ENTRY->ptr_gui_reset_timer   	)
#define gui_is_timer_installed		                    (MELIXMOD_FUNC_ENTRY->ptr_gui_is_timer_installed   	)
#define gui_have_free_timer		                       	(MELIXMOD_FUNC_ENTRY->ptr_gui_have_free_timer   	)
#define gui_get_tick_count		                       	(MELIXMOD_FUNC_ENTRY->ptr_gui_get_tick_count   	)
#define gui_get_timer_expired		                    (MELIXMOD_FUNC_ENTRY->ptr_gui_get_timer_expired   	)
#define gui_get_timer_slot		                       	(MELIXMOD_FUNC_ENTRY->ptr_gui_get_timer_slot   	)
#define gui_uc_calc_char_size                       	(MELIXMOD_FUNC_ENTRY->ptr_gui_uc_calc_char_size   	)
#define gui_uc_get_char_codeinc		                    (MELIXMOD_FUNC_ENTRY->ptr_gui_uc_get_char_codeinc   	)
#define gui_uc_numchars_to_numbytes		                (MELIXMOD_FUNC_ENTRY->ptr_gui_uc_numchars_to_numbytes   	)
#define gui_uc_numbytes_to_numchars		                (MELIXMOD_FUNC_ENTRY->ptr_gui_uc_numbytes_to_numchars   	)
#define gui_uc_get_char_size		                    (MELIXMOD_FUNC_ENTRY->ptr_gui_uc_get_char_size   	)
#define gui_uc_get_char_code		                    (MELIXMOD_FUNC_ENTRY->ptr_gui_uc_get_char_code   	)
#define gui_uc_encode			                       	(MELIXMOD_FUNC_ENTRY->ptr_gui_uc_encode   	)
#define gui_get_default_font	                       	(MELIXMOD_FUNC_ENTRY->ptr_gui_get_default_font   	)
#define gui_set_frame_color_8bpp32                     	(MELIXMOD_FUNC_ENTRY->ptr_gui_set_frame_color_8bpp32   	)
#define gui_sft_create_font                     		(MELIXMOD_FUNC_ENTRY->ptr_gui_sft_create_font   	)
#define gui_sft_release_font                     		(MELIXMOD_FUNC_ENTRY->ptr_gui_sft_release_font   	)
#define gui_ttf_create_font                     		(MELIXMOD_FUNC_ENTRY->ptr_gui_ttf_create_font   	)
#define gui_ttf_done                     				(MELIXMOD_FUNC_ENTRY->ptr_gui_ttf_done   	)
#define gui_create_hdc                     				(MELIXMOD_FUNC_ENTRY->ptr_gui_create_hdc   	)
#define gui_destroy_hdc                     			(MELIXMOD_FUNC_ENTRY->ptr_gui_destroy_hdc   	)
#define gui_select_back_color              				(MELIXMOD_FUNC_ENTRY->ptr_gui_select_back_color   	)
#define gui_get_back_color                     			(MELIXMOD_FUNC_ENTRY->ptr_gui_get_back_color   	)
#define gui_select_pen_color                     		(MELIXMOD_FUNC_ENTRY->ptr_gui_select_pen_color   	)
#define gui_get_pen_color                     			(MELIXMOD_FUNC_ENTRY->ptr_gui_get_pen_color   	)
#define gui_select_text_color                     		(MELIXMOD_FUNC_ENTRY->ptr_gui_select_text_color   	)
#define gui_get_text_color                     			(MELIXMOD_FUNC_ENTRY->ptr_gui_get_text_color  	)
#define gui_select_font                     			(MELIXMOD_FUNC_ENTRY->ptr_gui_select_font   	)
#define gui_get_font                     				(MELIXMOD_FUNC_ENTRY->ptr_gui_get_font   	)
#define gui_select_char_set                     		(MELIXMOD_FUNC_ENTRY->ptr_gui_select_char_set   	)
#define gui_get_char_set                     			(MELIXMOD_FUNC_ENTRY->ptr_gui_get_char_set   	)
#define gui_select_draw_mode                     		(MELIXMOD_FUNC_ENTRY->ptr_gui_select_draw_mode   	)
#define gui_get_draw_mode                     			(MELIXMOD_FUNC_ENTRY->ptr_gui_get_draw_mode   	)
#define gui_select_font_mode                     		(MELIXMOD_FUNC_ENTRY->ptr_gui_select_font_mode   	)
#define gui_get_font_mode                     			(MELIXMOD_FUNC_ENTRY->ptr_gui_get_font_mode   	)
#define gui_select_text_style                     		(MELIXMOD_FUNC_ENTRY->ptr_gui_select_text_style   	)
#define gui_get_text_style                     			(MELIXMOD_FUNC_ENTRY->ptr_gui_get_text_style   	)
#define gui_select_text_mode                     		(MELIXMOD_FUNC_ENTRY->ptr_gui_select_text_mode   	)
#define gui_get_text_mode                     			(MELIXMOD_FUNC_ENTRY->ptr_gui_get_text_mode   	)
#define gui_select_text_align                     		(MELIXMOD_FUNC_ENTRY->ptr_gui_select_text_align   	)
#define gui_get_text_align                     			(MELIXMOD_FUNC_ENTRY->ptr_gui_get_text_align   	)
#define gui_get_text_pos                     			(MELIXMOD_FUNC_ENTRY->ptr_gui_get_text_pos   	)
#define gui_select_text_pos                     		(MELIXMOD_FUNC_ENTRY->ptr_gui_select_text_pos   	)
#define gui_draw_bitmap                             	(MELIXMOD_FUNC_ENTRY->ptr_gui_draw_bitmap         	)
#define gui_draw_text_at                              	(MELIXMOD_FUNC_ENTRY->ptr_gui_draw_text_at          	)
#define gui_draw_text_in_rect                          	(MELIXMOD_FUNC_ENTRY->ptr_gui_draw_text_in_rect           	)
#define gui_draw_text_in_rect_wrap                     	(MELIXMOD_FUNC_ENTRY->ptr_gui_draw_text_in_rect_wrap     	)
#define gui_draw_circle		                           	(MELIXMOD_FUNC_ENTRY->ptr_gui_draw_circle       	)
#define gui_fill_rect                                 	(MELIXMOD_FUNC_ENTRY->ptr_gui_fill_rect             	)
#define gui_open_res                                 	(MELIXMOD_FUNC_ENTRY->ptr_gui_open_res             	)
#define gui_close_res                                 	(MELIXMOD_FUNC_ENTRY->ptr_gui_close_res             	)
#define gui_read_res                                 	(MELIXMOD_FUNC_ENTRY->ptr_gui_read_res             	)
#define gui_get_res_size                                (MELIXMOD_FUNC_ENTRY->ptr_gui_get_res_size             	)
#define gui_get_res_addr                                (MELIXMOD_FUNC_ENTRY->ptr_gui_get_res_addr             	)
#define gui_get_res_type                                (MELIXMOD_FUNC_ENTRY->ptr_gui_get_res_type             	)
#define gui_get_res                                 	(MELIXMOD_FUNC_ENTRY->ptr_gui_get_res             	)
#define gui_lang_open                                 	(MELIXMOD_FUNC_ENTRY->ptr_gui_lang_open             	)
#define gui_lang_read                                 	(MELIXMOD_FUNC_ENTRY->ptr_gui_lang_read             	)
#define gui_lang_get_string_address                     (MELIXMOD_FUNC_ENTRY->ptr_gui_lang_get_string_address             	)
#define gui_lang_get_string_size                        (MELIXMOD_FUNC_ENTRY->ptr_gui_lang_get_string_size             	)
#define gui_lang_get_string                             (MELIXMOD_FUNC_ENTRY->ptr_gui_lang_get_string             	)
#define gui_lang_close                                 	(MELIXMOD_FUNC_ENTRY->ptr_gui_lang_close             	)
#define gui_button_create                               (MELIXMOD_FUNC_ENTRY->ptr_gui_button_create             	)
#define gui_button_delete                               (MELIXMOD_FUNC_ENTRY->ptr_gui_button_delete             	)
#define gui_button_paint                                (MELIXMOD_FUNC_ENTRY->ptr_gui_button_paint             	)
#define gui_button_set_paint_flag                       (MELIXMOD_FUNC_ENTRY->ptr_gui_button_set_paint_flag             	)
#define gui_get_scn_width                       		(MELIXMOD_FUNC_ENTRY->ptr_gui_get_scn_width             	)
#define gui_get_scn_height                       		(MELIXMOD_FUNC_ENTRY->ptr_gui_get_scn_height             	)
#define gui_msg_looper		                       		(MELIXMOD_FUNC_ENTRY->ptr_gui_msg_looper             	)
#define gui_activity_set_add_data		                (MELIXMOD_FUNC_ENTRY->ptr_gui_activity_set_add_data             	)
#define gui_activity_get_add_data		                (MELIXMOD_FUNC_ENTRY->ptr_gui_activity_get_add_data             	)
#define gui_view_set_add_data		                    (MELIXMOD_FUNC_ENTRY->ptr_gui_view_set_add_data             	)
#define gui_view_get_add_data		                    (MELIXMOD_FUNC_ENTRY->ptr_gui_view_get_add_data             	)
#define gui_get_res_them		                    	(MELIXMOD_FUNC_ENTRY->ptr_gui_get_res_them             	)
#define gui_free_res_them		                    	(MELIXMOD_FUNC_ENTRY->ptr_gui_free_res_them             	)
#define gui_set_palette		                    		(MELIXMOD_FUNC_ENTRY->ptr_gui_set_palette             	)
#define gui_select_dst_xfb	                    		(MELIXMOD_FUNC_ENTRY->ptr_gui_select_dst_xfb             	)
#define gdi_set_rect_mem	                    		(MELIXMOD_FUNC_ENTRY->ptr_gdi_set_rect_mem             	)
#define gui_get_rect_mem	                    		(MELIXMOD_FUNC_ENTRY->ptr_gui_get_rect_mem             	)
#define gui_rotate_buffer	                    		(MELIXMOD_FUNC_ENTRY->ptr_gui_rotate_buffer             	)
#define gui_list_create	                    			(MELIXMOD_FUNC_ENTRY->ptr_gui_list_create             	)
#define gui_list_config	                    			(MELIXMOD_FUNC_ENTRY->ptr_gui_list_config             	)
#define gui_list_scroll_config	                    	(MELIXMOD_FUNC_ENTRY->ptr_gui_list_scroll_config             	)
#define gui_list_delete	                    			(MELIXMOD_FUNC_ENTRY->ptr_gui_list_delete             	)
#define gui_list_show_page	                    		(MELIXMOD_FUNC_ENTRY->ptr_gui_list_show_page             	)
#define gui_list_touch_up	                    		(MELIXMOD_FUNC_ENTRY->ptr_gui_list_touch_up             	)
#define gui_list_touch_down	                    		(MELIXMOD_FUNC_ENTRY->ptr_gui_list_touch_down             	)
#define gui_list_touch_move	                    		(MELIXMOD_FUNC_ENTRY->ptr_gui_list_touch_move             	)
#define gui_list_next_page	                    		(MELIXMOD_FUNC_ENTRY->ptr_gui_list_next_page             	)
#define gui_list_prev_page	                    		(MELIXMOD_FUNC_ENTRY->ptr_gui_list_prev_page             	)
#define gui_list_next_item	                    		(MELIXMOD_FUNC_ENTRY->ptr_gui_list_next_item             	)
#define gui_list_prev_item	                    		(MELIXMOD_FUNC_ENTRY->ptr_gui_list_prev_item             	)
#define gui_list_get_buf_addr	                    	(MELIXMOD_FUNC_ENTRY->ptr_gui_list_get_buf_addr             	)
#define gui_list_get_buf_width	                    	(MELIXMOD_FUNC_ENTRY->ptr_gui_list_get_buf_width             	)
#define gui_list_get_buf_height	                    	(MELIXMOD_FUNC_ENTRY->ptr_gui_list_get_buf_height             	)
#define gui_list_get_attr	                    		(MELIXMOD_FUNC_ENTRY->ptr_gui_list_get_attr             	)
#define gui_draw_hline	                    			(MELIXMOD_FUNC_ENTRY->ptr_gui_draw_hline             	)
#define gui_draw_vline	                    			(MELIXMOD_FUNC_ENTRY->ptr_gui_draw_vline             	)
#define gui_draw_bitstring	                    		(MELIXMOD_FUNC_ENTRY->ptr_gui_draw_bitstring             	)
#define gui_set_animation_invalid_height	            (MELIXMOD_FUNC_ENTRY->ptr_gui_set_animation_invalid_height     	)
#define gui_fill_circle						            (MELIXMOD_FUNC_ENTRY->ptr_gui_fill_circle     	)
#define gui_draw_rect						            (MELIXMOD_FUNC_ENTRY->ptr_gui_draw_rect     	)
#define gui_draw_line						            (MELIXMOD_FUNC_ENTRY->ptr_gui_draw_line     	)
#define gui_draw_ellipse					            (MELIXMOD_FUNC_ENTRY->ptr_gui_draw_ellipse     	)
#define gui_fill_ellipse					            (MELIXMOD_FUNC_ENTRY->ptr_gui_fill_ellipse     	)
#define gui_draw_poly_line					            (MELIXMOD_FUNC_ENTRY->ptr_gui_draw_poly_line     	)
#define gui_draw_polygon					            (MELIXMOD_FUNC_ENTRY->ptr_gui_draw_polygon     	)
#define gui_fill_polygon					            (MELIXMOD_FUNC_ENTRY->ptr_gui_fill_polygon     	)
#define gui_draw_point						            (MELIXMOD_FUNC_ENTRY->ptr_gui_draw_point     	)
#define gui_draw_line_to					            (MELIXMOD_FUNC_ENTRY->ptr_gui_draw_line_to     	)
#define gui_move_to							            (MELIXMOD_FUNC_ENTRY->ptr_gui_move_to     	)
#define gui_move_rel						            (MELIXMOD_FUNC_ENTRY->ptr_gui_move_rel     	)
#define gui_draw_arc						            (MELIXMOD_FUNC_ENTRY->ptr_gui_draw_arc     	)
#define gui_draw_pie						            (MELIXMOD_FUNC_ENTRY->ptr_gui_draw_pie     	)
#define gui_select_pen_size						        (MELIXMOD_FUNC_ENTRY->ptr_gui_select_pen_size     	)
#define gui_select_line_style						    (MELIXMOD_FUNC_ENTRY->ptr_gui_select_line_style     	)

#endif

#endif
