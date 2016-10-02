
#ifndef  __SUBMENU_DVB_SCAN_H_
#define  __SUBMENU_DVB_SCAN_H_

#include "beetles_app.h"


typedef	struct tag_dvb_scan_proc_para
{
	H_LYR		layer;
	GUI_FONT 	*font;

}dvb_scan_proc_para_t;

H_WIN search_create(H_WIN hparent,H_LYR layer);
void  dvb_scan_proc_win_delete(H_WIN dvb_scan_proc_win);


#endif
