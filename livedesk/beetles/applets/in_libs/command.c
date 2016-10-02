#include "command.h"


__s32 Command_TO_Parent(H_WIN hwin,__s32 id,__s32 data1,__s32 data2)
{
	__gui_msg_t msg;

	msg.h_deswin   = GUI_WinGetParent(hwin);
	msg.h_srcwin   = NULL;
	msg.id         = GUI_MSG_COMMAND;	
	msg.dwAddData1 = MAKELONG(GUI_WinGetItemId(hwin), id);
	msg.dwAddData2 = data1;
	msg.dwReserved = data2;

	return (GUI_SendMessage(&msg));
}


__s32 Command_TO_Point_Win(H_WIN hwin,__u32 ID,__u32 msgcode,__u32 data1,__u32 data2)
{
	__gui_msg_t msg;

	msg.h_deswin   = hwin;
	msg.h_srcwin   = NULL;
	msg.id         = GUI_MSG_COMMAND;
	msg.dwAddData1 = (__u32)MAKELONG(ID,msgcode);
	msg.dwAddData2 = data1;
	msg.dwReserved = data2;

	GUI_SendMessage(&msg);
}

