
#ifndef __AUDIO_PARA_H__
#define __AUDIO_PARA_H__
#include "audio_uipara.h"




audio_uipara_t audio_480_272 = 
{
	{(480 - 120)/2,0,120,24},
	{(480 - 120)/2,24,120,20},
	6,
	20 * 6,
	120
};

audio_uipara_t audio_720_576 = 
{
	{(720 - 120)/2,0,120,24},
	{(720 - 120)/2,24,120,20},
	6,
	20 * 6,
	120
};

audio_uipara_t audio_720_480 = 
{
	{(720 - 120)/2,0,120,24},
	{(720 - 120)/2,24,120,20},
	6,
	20 * 6,
	120
};

audio_uipara_t audio_800_480 = 
{
	{(800 - 180)/2,0,180,44},
	{(800 - 180)/2,44,180,38},
	6,
	38 * 6,
	180
};

audio_uipara_t audio_1024_600 = 
{
	{(1024 - 180)/2,0,180,44},
	{(1024 - 180)/2,44,180,38},
	6,
	38 * 6,
	180
};

audio_uipara_t* audio_get_uipara(void)
{
	SIZE rect;

	dsk_display_get_size(&rect.width,&rect.height);

	if((rect.width == 800) && (rect.height == 480))
	{
		return &audio_800_480;
	}
	else if((rect.width == 480) && (rect.height == 272))
	{
		return &audio_480_272;
	}	
	
	else if((rect.width == 720) && (rect.height == 576))
	{
		return &audio_720_576;
	}	
	else if((rect.width == 720) && (rect.height == 480))
	{
		return &audio_720_480;
	}	
	else if((rect.width == 1024) && (rect.height == 600))
	{
		return &audio_1024_600;
	}	
	else
	{
		return &audio_800_480;
	}
}

#endif


