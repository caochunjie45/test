
#ifndef ISDB_INTERFACE_H
#define ISDB_INTERFACE_H

//#include "maple_common.h"
#include "mod_cedar_i.h"


//*****************************************************************************//
//*************** Functions outside provide to ISDB SI library ****************//
//*****************************************************************************//

//* functions isdb si lib use to create and delete task.
//* task function prototype must be 'void function_name(void* param)'
//* function isdb si lib use to create a task.
extern __u8  isdb_create_task(__pTHD_t task, void* param, __u16 level);
//* function si lib use to delete a task.
extern __u8  isdb_delete_task(__u8 task_id);
//* function si lib use to check task quit request.
extern __s32 isdb_check_task_delete_request(__u8 taskId);

//* isdb si lib need this function to open demux section filter
extern __s32 isdb_open_section_filter(__u32 pid,__u32 servid, __s32 *pnVirtualChanId);
//* isdb si lib need this function to close demux section filter  
extern __s32 isdb_close_section_filter(__u32 pid, __s32 nVirtualChanId);
//* isdb si lib need this function to read section data
extern __s32 isdb_read_section_data(__u32 pid, __u8* buf, __u8 checkCrcFlag);    //废弃

//* event callback functions isdb si lib will call when event happen.

//* callback function to notify one service searched.
extern void  isdb_search_one_service_cb(maple_serv_item_t* newItem);
//* callback function to notify service search finish.
extern void  isdb_search_service_finish_cb(void);
//* callback function to notify simple EPG searchi finish.
extern void  isdb_search_sepg_finish_cb(maple_sepg_t* sepg);    //废弃
//* callback function to notify one schefule event searched.
extern void  isdb_search_one_schedule_cb(maple_schdl_event_t* newEvent);    //废弃
//* callback function to notify schedule search1 finish.
extern void  isdb_search_schedule_finish_cb(void);  //废弃

//* function use to control the tuner
//* function use to tune.
extern __s32 isdb_tune(maple_demod_info_t* demod_pm);
//* function use to get current tuner parameter.
extern __s32 isdb_get_tuner_param(maple_demod_info_t* demod_pm);


//*****************************************************************************//
//**************** Functions ISDB SI library provide to outside ***************//
//*****************************************************************************//

//* open isdb si library.
__s32 isdb_open(void);
//* close isdb si library. 
__s32 isdb_close(void);

//* function for service list managing.
//* open service list from file.
__s32 isdb_open_list(__u8* listFilePath);
//* save list to file.
__s32 isdb_save_list(__u8* listFilePath);
//* delete service list.
__s32 isdb_delete_list(void);
//* sort service list.
__s32 isdb_sort_list(__s32 nSortMode);  //nSortMode:0:按remote control key id排序,相同的id号的service再按service number排序.默认从小到大排序;
//* add one service item to list.
__s32 isdb_add_serv_item(maple_serv_item_t* item, __u32 check);
//* delete one service item
__s32 isdb_delete_serv_item(__u32 channelId);
//* add list to isdb si lib.
__s32 isdb_add_list(maple_serv_list_t* newList);
//* copy list from isdb si lib.
__s32 isdb_get_list(maple_serv_list_t* list, __u32 type);
//* copy service item from isdb si lib.
__s32 isdb_get_service_item(maple_serv_item_t* item, __u32 channelId);
//* set service item favourate flag
__s32 isdb_set_service_item_favourate_flag(__u32 channelId, __u32 isFavourate);
//* lock service item, key == -1 means unlock
__s32 isdb_set_service_item_lock(__u32 channelId, __s32 key);

//* function for service search.
//* start manual service search
__s32 isdb_search_service(maple_demod_info_t* tp, maple_serv_list_t* servList);
__s32 isdb_search_service_by_serviceId(maple_serv_item_t* pServItem);

//* stop service search
__s32 isdb_stop_service_search(void);

//* function for simple EPG search.
//* start simple EPG search
__s32 isdb_search_sepg(__s32 servIndex, maple_sepg_t* pSEpg);
//* stop simple EPG search
__s32 isdb_stop_sepg_search(void);

//* function for schedule EPG search
//* start schedule search
__s32 isdb_search_schedule(__s32 servIndex, maple_schdl_list_t* pSchedule);
//* stop schedule search
__s32 isdb_stop_schedule_search(void);

//* function for tv stream time monitor
//* start time monitor
__s32 isdb_open_time_monitor(void);
//* stop time monitor
__s32 isdb_close_time_monitor(void);
//* get current stream time
__s32 isdb_get_time(maple_time_t* curTime);

//* funciton for dvb stream converting
__s32 isdb_convert_str_to_utf8(__u8* src, __u32 srcLen, __u8* dst, __u32 dstLen, __u8* lang_code);
//* set default charactor coding type
__s32 isdb_set_default_char_code_type(__epdk_charset_enm_e codeType);

//* function for epg monitor
//* start epg monitor
__s32 isdb_open_epg_monitor(void);
//* stop epg monitor
__s32 isdb_close_epg_monitor(void);
//* get simple epg of specified service
__s32 isdb_get_sepg(__s32 channelId, maple_sepg_t* pSEpg);
//* get schedule of specified service
__s32 isdb_get_schedule(__s32 channelId, maple_schdl_list_t* pSchedule);

//* set current playing service
__s32 isdb_set_current_service(__u32 channelId);

//* get service number of service list
__s32 isdb_get_service_num(void);
//* get current service id
__s32 isdb_get_cur_service_id(void);

#endif

