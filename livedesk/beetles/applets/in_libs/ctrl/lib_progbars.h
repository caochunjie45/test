
#ifndef __LIB_PROGBARS_H_
#define __LIB_PROGBARS_H_

#include "beetles_app.h"

#include "lib_windows.h"





int Set_Progbars_Return_CallBack(__u32 progbars_ID,windows_t *Windows,__pCBK_t Progbars_Return);

dlist_t *Create_Progbars_List(void);

int Register_Progbars(__u32 ID,windows_t *Windows,progbars_ui_t *progbars_ui);

int Register_Progbars_HostedFrom_Button(__u32 ID,windows_t *Windows,progbars_ui_t *progbars_ui);

#endif


