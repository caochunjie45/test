#ifndef __STATICS_TEMPLATE_H_
#define __STATICS_TEMPLATE_H_

#include "beetles_app.h"

#include "lib_statics.h"


#define STRING_MAX  64


int Get_Statics_Source(statics_info_t *statics_info,__u32 ID,statics_ui_t *statics_ui);


int Relase_Statics_Source(statics_info_t *statics_info,__u32 ID);


#endif


