

#ifndef __LIB_LISTBAR_H_
#define __LIB_LISTBAR_H_

#include "beetles_app.h"

#include "lib_windows.h"





int Set_Listbar_HostedFrom_Button_Left_CallBack(__u32 Listbar_ID,windows_t *Windows,__pCBK_t Listbar_Left);

int Set_Listbar_HostedFrom_Button_Right_CallBack(__u32 Listbar_ID,windows_t *Windows,__pCBK_t Listbar_Right);

int Set_Listbar_HostedFrom_Button_Enter_CallBack(__u32 Listbar_ID,windows_t *Windows,__pCBK_t Listbar_Enter);

int Set_Listbar_HostedFrom_Button_Return_CallBack(__u32 Listbar_ID,windows_t *Windows,__pCBK_t Listbar_Return);

dlist_t *Create_Listbar_List(void);


int Register_Listbar(__u32 ID,windows_t *Windows,listbar_ui_t *listbar_ui,__u8 focus_ID);

int Register_Listbar_HostedFrom_Button(__u32 ID,windows_t *Windows,listbar_ui_t *listbar_ui,__u8 focus_ID);

#endif


