/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
* File    	: 	Explorer.c,
*			Explorer.h
* By      	: 	Libaiao
* Func	: 
* Version	: 	v1.0
* ============================================================================================================
* 2010-4-14 15:24:52  Libaiao  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "digit_ui.h"



digit_uipara_t digit_480_272 =
{
	{140,(272-100),200,100},
	{20,10, 200-20,28},
	{20,20, 200-20,28},
	{10, 50 ,180, 30},
	
	ID_DTV_DIG_BG_BMP,
	ID_DTV_SEL_NUM_NO_NUM_BMP,

	{
		ID_DTV_SM_0_BMP,
		ID_DTV_SM_1_BMP,
		ID_DTV_SM_2_BMP,
		ID_DTV_SM_3_BMP,
		ID_DTV_SM_4_BMP,
		ID_DTV_SM_5_BMP,
		ID_DTV_SM_6_BMP,
		ID_DTV_SM_7_BMP,
		ID_DTV_SM_8_BMP,
		ID_DTV_SM_9_BMP,
	},

};


digit_uipara_t digit_800_480 =
{
	{(800-200)/2,(480-100),200,100},
	{20,10, 200-20,28},
	{20,20, 200-20,28},
	{10, 50 ,180, 30},
	
	ID_DTV_DIG_BG_BMP,
	ID_DTV_SEL_NUM_NO_NUM_BMP,

	{
		ID_DTV_SM_0_BMP,
		ID_DTV_SM_1_BMP,
		ID_DTV_SM_2_BMP,
		ID_DTV_SM_3_BMP,
		ID_DTV_SM_4_BMP,
		ID_DTV_SM_5_BMP,
		ID_DTV_SM_6_BMP,
		ID_DTV_SM_7_BMP,
		ID_DTV_SM_8_BMP,
		ID_DTV_SM_9_BMP,
	},

};



digit_uipara_t digit_1024_600 =
{
	{(1024-240)/2,(600-136),240,136},
	{20,10, 200-20,28},
	{20,20, 200-20,28},
	{10, 50 ,180, 30},
	
	ID_DTV_DIG_BG_BMP,
	ID_DTV_SEL_NUM_NO_NUM_BMP,

	{
		ID_DTV_SM_0_BMP,
		ID_DTV_SM_1_BMP,
		ID_DTV_SM_2_BMP,
		ID_DTV_SM_3_BMP,
		ID_DTV_SM_4_BMP,
		ID_DTV_SM_5_BMP,
		ID_DTV_SM_6_BMP,
		ID_DTV_SM_7_BMP,
		ID_DTV_SM_8_BMP,
		ID_DTV_SM_9_BMP,
	},

};


digit_uipara_t *get_digit_uipara(void)
{
	SIZE rect;
	dsk_display_get_size(&rect.width,&rect.height);

	if((rect.width == 800) && (rect.height == 480))
	{
		return &digit_800_480;
	}
	else if((rect.width == 480) && (rect.height == 272))
	{
		return &digit_480_272;
	}
	
	else if((rect.width == 1024) && (rect.height == 600))
	{
		return &digit_1024_600;
	}
}

__s32 get_digit_layer_rect(RECT *layer_rect)
{
	SIZE rect;
	dsk_display_get_size(&rect.width,&rect.height);
	
	if((rect.width == 800) && (rect.height == 480))
	{	
		layer_rect->x = digit_800_480.layer_rect.x;
		layer_rect->y = digit_800_480.layer_rect.y;
		layer_rect->width = digit_800_480.layer_rect.width;
		layer_rect->height = digit_800_480.layer_rect.height;
	}

	else if((rect.width == 480) && (rect.height == 272))
	{	
		layer_rect->x      = digit_480_272.layer_rect.x;
		layer_rect->y      = digit_480_272.layer_rect.y;
		layer_rect->width  = digit_480_272.layer_rect.width;
		layer_rect->height = digit_480_272.layer_rect.height;
	}
	
	else if((rect.width == 1024) && (rect.height == 600))
	{	
		layer_rect->x = digit_1024_600.layer_rect.x;
		layer_rect->y = digit_1024_600.layer_rect.y;
		layer_rect->width = digit_1024_600.layer_rect.width;
		layer_rect->height = digit_1024_600.layer_rect.height;
	}

}

RECT * get_digit_title_rect(void)
{
	SIZE rect;
	dsk_display_get_size(&rect.width,&rect.height);

	if((rect.width == 800) && (rect.height == 480))
	{
		return &digit_800_480.digit_title_rect;
	}
	
	else if((rect.width == 480) && (rect.height == 272))
	{
		return &digit_480_272.digit_title_rect;
	}

	else if((rect.width == 1024) && (rect.height == 600))
	{
		return &digit_1024_600.digit_title_rect;
	}

}

RECT * get_digit_text_rect(void)
{
	SIZE rect;
	dsk_display_get_size(&rect.width,&rect.height);

	if((rect.width == 800) && (rect.height == 480))
	{
		return &digit_800_480.digit_text_rect;
	}
	
	else if((rect.width == 480) && (rect.height == 272))
	{
		return &digit_480_272.digit_text_rect;
	}

	else if((rect.width == 1024) && (rect.height == 600))
	{
		return &digit_1024_600.digit_text_rect;
	}

}

RECT * get_digit_number_rect(void)
{
	SIZE rect;

	dsk_display_get_size(&rect.width,&rect.height);

	if((rect.width == 800) && (rect.height == 480))
	{
		return &digit_800_480.digit_number_rect;
	}

	else if((rect.width == 480) && (rect.height == 272))
	{
		return &digit_480_272.digit_number_rect;
	}

	else if((rect.width == 1024) && (rect.height == 600))
	{
		return &digit_1024_600.digit_number_rect;
	}


}


