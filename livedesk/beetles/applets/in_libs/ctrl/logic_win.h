
#ifndef __LOGIC_WIN_H_
#define __LOGIC_WIN_H_


#include "beetles_app.h"


#include "man_win.h"


void *Win_Get_Public_Data(Man_Win *ManWin);


__s32 Win_Set_Public_Data(Man_Win *ManWin,void *Data);


__s32 Logic_Win_Set_Destory_CallBack(Man_Win *ManWin,__pCBK_t Destory);


int Delete_LogicWin(Logic_Win *LogicWin);


int Register_LogicWin(__u32 ID,Man_Win *ManWin,__pGUI_WIN_CB Proc);



#endif

