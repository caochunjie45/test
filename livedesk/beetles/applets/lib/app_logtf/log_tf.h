#ifndef __LOG_TF_H__
#define __LOG_TF_H__

#include "beetles_app.h"

__s32 log_init(void);
__s32 log_deinit(void);
__s32 log_printf(const char *format, ...);
__s32 log_flush(void);
__s32 eLIBs_log_set_dump_flag(__s32 flag);


#endif