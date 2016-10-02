
#ifndef __LOGIC_WIN_H_
#define __LOGIC_WIN_H_


#include "beetles_app.h"


#include "man_win.h"


void *WinGetPublicData(Man_Win *ManWin);


__s32 WinSetPublicData(Man_Win *ManWin,void *Data);


__s32 LodicWinSetDestoryCallBack(Man_Win *ManWin,__pCBK_t Destory);


int Delete_LogicWin(Logic_Win *LogicWin);


int Register_LogicWin(__u32 ID,Man_Win *ManWin,__pGUI_WIN_CB Proc);



#endif

