
#ifndef  __LOGIC_MANAGER_H_
#define  __LOGIC_MANAGER_H_


#include "app_touch_i.h"


typedef struct
{
	__u32                       volume;

	__u32                       light;

	__u32                       voltage;
	
}logic_pulic_data_t;



int Register_One_Logic_Win(__u32 ID,Man_Win *ManWin);


#endif

