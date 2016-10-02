
#ifndef __DIGIT_H__
#define __DIGIT_H__

#include "Beetles_app.h"


#define background_color 0xffa64103
#define DIGIT_COUNT   10


typedef enum explorer_digit_command_s
{
    SWITCH_TO_DIGIT_SELECT = 1,
    ESCAPE_TO_EXPLORER_LIST,
}explorer_digit_command_t;


typedef struct digit_frmwin_para
{
	H_LYR  digit_layer;
	
}digit_frmwin_para_t;


typedef struct digit_frmwin_attr
{
	digit_frmwin_para_t *digit_para;
	H_LYR  digit_layer;
	__s32 digit_timmer_id;
	__s32 digit_timmer_cnt;
	__u32 ndigit_index;
	
	HTHEME background_bmp;
	HTHEME no_number_bmp;
	
	HTHEME digit_bmp[DIGIT_COUNT];

}digit_frmwin_attr_t;
 
__s32 draw_title(digit_frmwin_attr_t *attr);
H_LYR digit_layer_create(void);
H_WIN digit_win_create(H_WIN h_parent, digit_frmwin_para_t *para);
__s32 digit_win_delete(H_WIN list_win);

#endif 

