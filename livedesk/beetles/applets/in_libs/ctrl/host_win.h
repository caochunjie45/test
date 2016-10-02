
#ifndef __HOST_WIN_H_
#define __HOST_WIN_H_

#include "beetles_app.h"


#include "man_win.h"


int Add_New_Hosted_Win(Mixed_CtrlWin_T **pFirstHosted, Mixed_CtrlWin_T *pHosted);

Mixed_CtrlWin_T *  Get_Last_Hosted_Win(Mixed_CtrlWin_T *pFirstHosted);

Mixed_CtrlWin_T *  Get_Next_Hosted_Win(Mixed_CtrlWin_T *pHosted);

Mixed_CtrlWin_T *  Get_Prev_Hosted_Win(Mixed_CtrlWin_T *pFirstHosted,Mixed_CtrlWin_T *pHosted);

Mixed_CtrlWin_T *  Find_Hosted_Win(Mixed_CtrlWin_T *pFirstHosted,__u32 ID);

int destory_Hosting_Win(Mixed_CtrlWin_T **pFirstHosted);

int destory_Last_Hosted_Win(Mixed_CtrlWin_T **pFirstHosted);




#endif


