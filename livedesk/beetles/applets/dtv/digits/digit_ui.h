
#ifndef __DIGIT_UI_H__
#define __DIGIT_UI_H__

#include "Beetles_app.h"

 

typedef struct digit_uipara
{
	RECT layer_rect;
	RECT digit_title_rect;  //找不到指定频道
	RECT digit_text_rect;   //输入频道
	RECT digit_number_rect;
	
	__u32 background_bmp;
	__u32 no_number_bmp;
	__u32 digit_bmp[10];
	
}digit_uipara_t;

 
digit_uipara_t *get_digit_uipara(void);
__s32 get_digit_layer_rect(RECT *layer_rect);
RECT * get_digit_line_rect(void);
RECT * get_digit_number_rect(void);

#endif 

