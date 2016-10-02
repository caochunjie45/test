#include "msg.h"


__s32 TranslateMessage(__gui_msg_t	*msg)
{

	if(GUI_WinGetFocusChild(msg->h_deswin))
	{
		__gui_msg_t   new_msg;

		new_msg.id				= msg->id;
		new_msg.dwAddData1		= msg->dwAddData1;
		new_msg.h_deswin		= GUI_WinGetFocusChild(msg->h_deswin);
		new_msg.h_srcwin		= msg->h_deswin;
		new_msg.dwAddData2		= msg->dwAddData2;
		new_msg.p_arg			= NULL;
		new_msg.dwReserved      = msg->dwReserved;

		memcpy(msg,&new_msg,sizeof(__gui_msg_t));
	}
	
    return 0;
}




