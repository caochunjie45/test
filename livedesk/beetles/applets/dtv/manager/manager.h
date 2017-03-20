
#ifndef  __MANAGER_H_
#define  __MANAGER_H_


#include "apps_i.h"
 
#include "logic\logic_manager.h"
#include "interface\interface_i.h"



__s32 Manager_Ioctrl(H_WIN win,__u32 cmd, __s32 aux, void *pbuffer);


H_WIN App_Touch_Create(root_para_t *para);



#endif

