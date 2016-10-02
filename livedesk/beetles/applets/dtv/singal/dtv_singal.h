/*
***************************************
* hot.lee  2013 05 27
*
***************************************
*/

#ifndef __DTV_SINGAL_H__
#define __DTV_SINGAL_H__

#include "beetles_app.h"

typedef	struct tag_dtv_singal_para
{
	H_LYR		layer;

}dtv_singal_para_t;


H_WIN dtv_singal_create(H_WIN hparent,H_LYR *layer);


__s32 dtv_singal_onff(H_WIN win,__gui_lyrwin_sta_t status);


#endif



