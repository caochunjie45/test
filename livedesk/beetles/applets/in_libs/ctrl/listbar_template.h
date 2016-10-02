#ifndef __LISTBAR_TEMPLATE_H_
#define __LISTBAR_TEMPLATE_H_

#include "beetles_app.h"

#include "lib_listbar.h"




int Get_Listbar_Height(__u32 ID);


int Get_Listbar_Source(listbar_t *listbar,__u32 ID,listbar_ui_t *listbar_ui,__u8 focus_ID);


int Relase_Listbar_Source(listbar_t *listbar,__u32 ID);


#endif


