
#ifndef __SUBMENU_DVB_SCAN_PARA_H_
#define  __SUBMENU_DVB_SCAN_PARA_H_
#include "scan_proc_i.h"

typedef struct dvb_scan_proc_uipara_s
{
    RECT dvb_scan_str;
    RECT dvb_scan_freq; //Ƶ��
	RECT dvb_scan_ch;   //Ƶ��

	__s32 search_proc_width;
	__u8 search_proc_height;
	
	OFFSET dvb_scan_proc_bmp_pos; //������ƫ��
}dvb_scan_proc_uipara_t;

dvb_scan_proc_uipara_t* submenu_dvb_scan_proc_get_uipara(void);

#endif

