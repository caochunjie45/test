#ifndef __COMMAND_H_
#define __COMMAND_H_

#include "beetles_app.h"



__s32 Command_TO_Parent(H_WIN hwin,__u32 id,__u32 data1,__u32 data2);


__s32 Command_TO_Child_Win(H_WIN parent,__u32 win_ID,__u32 ID,__u32 data1,__u32 data2);


#endif


