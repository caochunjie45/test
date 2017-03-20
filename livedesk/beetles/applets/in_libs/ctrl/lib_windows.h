
#ifndef __LIB_WINDOWS_H_
#define __LIB_WINDOWS_H_


#include "beetles_app.h"

#include "man_win.h"

#include "host_win.h"

#include "layer.h"


extern dlist_t *Create_Statics_List(void);

extern dlist_t *Create_Button_List(void);

extern dlist_t *Create_Slider_List(void);

extern dlist_t *Create_Listbar_List(void);

extern dlist_t *Create_Progbars_List(void);





int Set_Windows_Show_CallBack(__u32 Windows_ID,Man_Win *ManWin,__pCBK_t Windows_Show);

int Set_Windows_Hide_CallBack(__u32 Windows_ID,Man_Win *ManWin,__pCBK_t Windows_Hide);

int Set_Windows_Update_CallBack(__u32 Windows_ID,Man_Win *ManWin,__pCBK_t Windows_Update);

int Set_Windows_Button_Next(windows_t *Windows);

int This_Windows_Button_Show(__u32 Button_ID,windows_t *Windows);

int Set_Windows_Button_Prev(windows_t *Windows);

int Set_Windows_Slider_Next(windows_t *Windows);

int This_Windows_Slider_Show(__u32 Slider_ID,windows_t *Windows);

int Set_Windows_Slider_Prev(windows_t *Windows);

int Set_Windows_Statics_Next(windows_t *Windows);

int This_Windows_Statics_Show(__u32 Statics_ID,windows_t *Windows);

int Set_Windows_Statics_Prev(windows_t *Windows);

int This_Win_Delete_Hosted_CtrlWin(windows_t *Windows);

int This_Win_Delete_All_Hosted_CtrlWin(windows_t *Windows);

dlist_t *Create_Windows_List(void);

int Delete_One_Windows(__u32 Windows_ID,Man_Win *ManWin);

int Set_Windows_Next(Man_Win *ManWin);

int This_Windows_Show(__u32 Windows_ID,Man_Win *ManWin);

int Set_Windows_Prev(Man_Win *ManWin);

int Update_Windows_Focus_Data(__u32 Windows_ID,Man_Win *ManWin);

int Register_Windows(__u32 ID,Man_Win *ManWin,__pCBK_t callback,int x,int y,int width,int height);




#endif


