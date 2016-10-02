#ifndef __LIB_STATICS_H_
#define __LIB_STATICS_H_

#include "beetles_app.h"

#include "lib_windows.h"







int Set_Statics_Bmp(__u32 statics_ID,windows_t *Windows,__u32 bmp_index);

dlist_t *Create_Statics_List(void);


int Register_Statics(__u32 ID,windows_t *Windows,statics_ui_t *statics_ui);


int Register_Statics_HostedFrom_Button(__u32 ID,windows_t *Windows,statics_ui_t *statics_ui);



#endif


