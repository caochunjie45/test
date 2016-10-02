#ifndef __LIB_SLIDER_H_
#define __LIB_SLIDER_H_

#include "beetles_app.h"

#include "lib_windows.h"





int Set_Slider_HostedFrom_Button_Return_CallBack(__u32 slider_ID,windows_t *Windows,__pCBK_t Slider_Return);

dlist_t *Create_Slider_List(void);

int Register_Slider(__u32 ID,windows_t *Windows,slider_ui_t *slider_ui);

int Register_Slider_HostedFrom_Button(__u32 ID,windows_t *Windows,slider_ui_t *slider_ui);

#endif


