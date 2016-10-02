
#include "scan_proc_uipara.h"
	
	

dvb_scan_proc_uipara_t  dvb_scan_proc_uipara_480_272 =
{
	{0,100,480,20},
	{0,120,480,20},
	{0,145,480,35},
	300,
	20,
	{(480 - 300)/2,180},
};

dvb_scan_proc_uipara_t  dvb_scan_proc_uipara_800_480 =
{
	{0,(480-48-48-48-60)/2,800,48},
	{0,(480-48-48-48-60)/2 + 48,800,48},
	{0,(480-48-48-48-60)/2 + 48 +48,800,48},
	300,
	20,
	{(800 - 300)/2,((480-48-48-48-60)/2 + 48 +48 + 48)},
};

dvb_scan_proc_uipara_t  dvb_scan_proc_uipara_720_576 =
{
	{0,100,720,20},
	{0,120,720,20},
	{0,145,720,35},
	300,
	20,
	{(720 - 300)/2,180},
};

dvb_scan_proc_uipara_t  dvb_scan_proc_uipara_720_480 =
{
	{0,100,720,20},
	{0,120,720,20},
	{0,145,720,35},
	300,
	20,
	{(720 - 300)/2,180},
};

dvb_scan_proc_uipara_t  dvb_scan_proc_uipara_1024_600 =
{
	{0,(600-48-48-48-60)/2,1024,48},
	{0,(600-48-48-48-60)/2 + 48,1024,48},
	{0,(600-48-48-48-60)/2 + 48 +48,1024,48},
	300,
	20,
	{(1024 - 300)/2,((600-48-48-48-60)/2 + 48 +48 + 48)},
};


dvb_scan_proc_uipara_t* submenu_dvb_scan_proc_get_uipara()
{
    __s32           screen_width;
    __s32 			screen_height;
	dvb_scan_proc_uipara_t  dvb_scan_proc_uipara;

	dsk_display_get_size(&screen_width, &screen_height);
	
	if(screen_width == 800 && screen_height == 480)
		return &dvb_scan_proc_uipara_800_480;
		
	else if(screen_width == 480 && screen_height == 272)
		return &dvb_scan_proc_uipara_480_272;
		
	else if(screen_width == 720 && screen_height == 576)
		return &dvb_scan_proc_uipara_720_576;
		
	else if(screen_width == 720 && screen_height == 480)
		return &dvb_scan_proc_uipara_720_480;
		
	else if(screen_width == 1024 && screen_height == 600)
		return &dvb_scan_proc_uipara_1024_600;
	else
		return &dvb_scan_proc_uipara_800_480;
}


