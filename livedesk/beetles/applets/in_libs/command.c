#include "command.h"


__s32 Command_TO_Parent(H_WIN hwin,__u32 id,__u32 data1,__u32 data2)
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


__s32 Command_TO_Child_Win(H_WIN parent,__u32 win_ID,__u32 ID,__u32 data1,__u32 data2)
{
	__gui_msg_t msg;

	H_WIN  hwin = NULL;

	hwin = GUI_WinGetHandFromId(parent,win_ID);

	if(NULL == hwin)
	{
		eDbug("hwin is NULL \n");
		return EPDK_FAIL;
	}


	
	msg.h_deswin   = hwin;
	msg.h_srcwin   = NULL;
	msg.id         = GUI_MSG_COMMAND;
	msg.dwAddData1 = (__u32)MAKELONG(ID,data1);
	msg.dwAddData2 = data2;

	GUI_SendMessage(&msg);
}

