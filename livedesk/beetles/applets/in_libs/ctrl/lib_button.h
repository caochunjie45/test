#ifndef __LIB_BUTTON_H_
#define __LIB_BUTTON_H_

#include "beetles_app.h"

#include "lib_windows.h"






int Set_Button_Left_CallBack(__u32 button_ID,windows_t *Windows,__pCBK_t Button_Left);

int Set_Button_Right_CallBack(__u32 button_ID,windows_t *Windows,__pCBK_t Button_Right);

int Set_Button_Enter_CallBack(__u32 button_ID,windows_t *Windows,__pCBK_t Button_Enter);

int Set_Button_Return_CallBack(__u32 button_ID,windows_t *Windows,__pCBK_t Button_Return);

dlist_t *Create_Button_List(void);

int Register_Button(__u32 ID,windows_t *Windows,button_ui_t *button_ui);

int Register_Button_HostedFrom_Button(__u32 ID,windows_t *Windows,button_ui_t *button_ui);

#endif


