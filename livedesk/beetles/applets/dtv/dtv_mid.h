/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: ebook_i.h
* By      	: Andy.zhang
* Version	: v1.0
* ============================================================================================================
* 2009-11-3 9:39:42  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/
#ifndef __DTV_MID_H__
#define __DTV_MID_H__

#include "epdk.h"
#include "mod_maple.h"
#include "apps.h"

//以下为CMMB/ISDBT需要配置的信息
//可选项MAPLE_TV_CMMB
// MAPLE_TV_DVB_T
//MAPLE_TV_ISDB_T
//MAPLE_TV_DTMB
//MAPLE_TV_ATSC 
#define  MAPLE_TV_STANDAR   MAPLE_TV_ISDB_T


//CMMB需要配置的信息
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
#define  IS_CA_EXIST    1 //是否有解密模块        add by hot.lee

#endif

#define  EN_DTV_RECORD 	1 //是否允许刻录



#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
enum
{
	CCTV1=601,                     
	QING_CHAI_DIAN_YIN,  //晴彩电影
	CCTV5,
	QING_CHAI_DIAN_TIANXIA,//晴彩天下
	CCTV_NEWS,             //CCTV-新闻
};
#endif

#define  DTV_DATABASE_NAME   "Z:\\program.db"
#define  DTV_BASEINFO_NAME   "Z:\\baseinfo.bin" 

#define TRUE 1

#define FALSE 0

#define MAX_EPG_DAY      7
#define MAX_SEPG_STRING  128
#define MAX_TMP_STRING   256
#define MAX_VOLUME       30
#define MAX_LIGHT        4

#define MAX_CHANNEL_NUM  64

typedef struct
{
    __s32                     flag; //有效标志
    
    __s32                     index;//第几个台
    
    maple_vd_win_ratio_mode_e mode;
    
    maple_serv_list_t service_list;            //这两个参数由中间件保存
    
	maple_serv_item_t service_item[64];        //最多128个台
		
	__u32   time_format;                       //时制，0代表24小时制，1代表12小时制
	
    __bool  lcn_state;
        
    __u8    audio_num_select[MAX_CHANNEL_NUM];
    
	__u32   reserverd[8];                      //保留参数设置
	
}reg_cmmb_para_t;

typedef enum tag_scan_s
{
    SLIDE_UPDATE_FREQ = 1,//更新频点的同时更新进度条
	SLIDE_UPDATE_CH,
	
}tag_scan_e;





typedef	enum tag_areaset
{
	SSET_NC_AREASET_ALLFREQ,
	SSET_NC_AREASET_SHENZHEN,
	SSET_NC_AREASET_AREAMAX,
}areaset_e;

typedef	enum tag_programtype
{
	SSET_NC_PROGRAMTYPE_TV,
	SSET_NC_PROGRAMTYPE_RADIO,
	SSET_NC_PROGRAMTYPE_ALL,
	SSET_NC_PROGRAMTYPE_MAX,
}programtype_e;

typedef enum        /* user define msg id */
{
    DVBSCAN_ID_SET_SLIDE=GUI_MSG_,
    DVBSCAN_ID_SET_FREQ,
    DVBSCAN_ID_SET_PROG_LIST,
    DVBSCAN_ID_SET_BW,
    
}__dvbscan_cmdid_t;

typedef enum        /* user define msg id */
{
    DVBEPG_ID_SET_SLIDE=GUI_MSG_,
    DVBEPG_ID_SET_FREQ,
    DVBEPG_ID_SET_PROG_LIST,
    DVBEPG_ID_SET_BW,
    DVBEPG_ID_SET_EPG_LIST,
    DVBEPG_ID_CREATE_EPG_LIST,
    
}__dvbepg_cmdid_t;

typedef enum        /* user define msg id */
{
    DVBADJUST_ID_SET_ADD=GUI_MSG_,
    DVBADJUST_ID_SET_DEC,

}__dvbadjust_cmdid_t;


typedef struct __maple_sl_cb_param_t_pOpaque
{
	void        *pOpaque;
	
	__gui_msg_t *msg;
	

}maple_sl_cb_param_t_pOpaque;


typedef struct __maple_schdl_list_day
{
    __u32                 eventNum;       //* how many events totally in the list 
	__u32                 MJD;
    __u32                 serviceid;
    maple_schdl_event_t  pSchdlEvents[512];   //* schedule event list

}maple_schdl_list_day;

typedef struct DTVMID_TIME 
{
    __u32   year;
    __u32   month;
    __u32    day;
    __u32    weekDay;
    __u32    hour;
    __u32    min;
	__u32    sec;
	__u32   MJD;
	
}dtvmid_time_t;


//extern   void sprintf(char *buffer, const char *str, ...);
#define sprintf eLIBs_sprintf //modify by Bayden, 140223, use melis interface.


#define MALLOC(x)                       esMEMS_Malloc(0, x)
#define malloc(x)                       esMEMS_Malloc(0, x)
#define FREE(x)                         esMEMS_Mfree(0, x)
#define free(x)                         esMEMS_Mfree(0, x)
#define palloc(x, y)                    esMEMS_Palloc(x, y)
#define pfree(x, y)                     esMEMS_Pfree(x, y)

#define MEMSET(buf, val, size)          eLIBs_memset(buf, val, size)
#define memset(buf, val, size)          eLIBs_memset(buf, val, size)
#define MEMCPY(dst, src, size)          eLIBs_memcpy(dst, src, size)
#define memcpy(dst, src, size)          eLIBs_memcpy(dst, src, size)

#define TOUPPER(n)                      eLIBs_toupper(n)
#define toupper(n)                      eLIBs_toupper(n)
#define STR_NCMP(dst, src, size)        eLIBs_strncmp(dst, src, size)
#define strncmp(dst, src, size)         eLIBs_strncmp(dst, src, size)
#define STR_CMP(s1, s2)                 eLIBs_strcmp(s1, s2)
#define strcmp(s1, s2)                  eLIBs_strcmp(s1, s2)
#define STR_LEN(s)                      eLIBs_strlen(s)
#define strlen(s)                       eLIBs_strlen(s)
#define STR_NCPY(s1, s2, n)             eLIBs_strncpy(s1, s2, n)
#define strncpy(s1, s2, n)              eLIBs_strncpy(s1, s2, n)
#define STR_CPY(s1, s2)                 eLIBs_strcpy(s1, s2)
#define strcpy(s1, s2)                  eLIBs_strcpy(s1, s2)
#define STR_CHR(s, ch)                  eLIBs_strchr(s,ch)
#define strchr(s, ch)                   eLIBs_strchr(s,ch)
#define STR_STR(s1, s2)                 eLIBs_strstr(s1, s2)
#define strstr(s1, s2)                  eLIBs_strstr(s1, s2)
#define strcat(s1, s2)                    eLIBs_strcat(s1, s2)


typedef struct Region_TPSects_s   
{
    __u32 StartFreq;            /*Start Transponder Frequency*/
    __u32 BandWidth;            /*Transponder Bandwidth*/
    
    
}Region_TPSects_t;

typedef struct DVBT_AreaTable_s
{
    __u32 CountryIndex; 
    const Region_TPSects_t * pTPRegion_Area;
    const char *const*pChannelName;
    __u32 TPIndex;
    char CountryCode[3];
}DVBT_AreaTable_t;

__s32 maple_open(void);

__s32 maple_close(void);

__s32 maple_connect_signal(maple_demod_info_t* tp);
__s32 maple_register_search_callback(__pCBK_t  cb_search_list_event_callback,__pCBK_t cb_search_list_finish_callback);
__s32 maple_search_stop(void);

__s32 maple_get_play_status(void);

__s32 maple_program_load(char *database_file_name);

__s32 maple_program_add_srvlist(void);
__s32 maple_get_save_program_index(void);
__s32 maple_set_save_program_index(__s32 index);

__s32 maple_cmmb_reg_flush(void);

__s32 maple_program_save(char *database_file_name);

__s32 maple_play_program(void);

__s32 maple_stop_program(__u32 video_layer_state);

__s32 maple_register_epg_callback(__pCBK_t cb_search_schedule_event_callback,__pCBK_t cb_search_schedule_finish_callback);
__s32 maple_epg_search(void);
__s32 maple_epg_search_stop(void);
void maple_epg_reset_schdl_list_day(__u32 serviceid);
__s32 maple_epg_getdata(void);
__s32 maple_epg_get_eventnum(__s32 day_index);
maple_schdl_event_t *maple_epg_get_schdl_event(__s32 day_index,__s32 event_index);
__s32 maple_get_cur_schedule_index(__s32 day_index);


__u32 UI_epg_info_delete(char* item_path);
__u32 UI_epg_info_add(char* item_path);



__u32 UI_item_delete(char* item_path);
__u32 UI_manual_scan_info_add(char* item_path,DVBT_AreaTable_t*areatable);
__s32 maple_thread_delete(__u32 thread_id);
__s32 maple_area_search(void);

void maple_time_monitor_start(void);
__s32 maple_time_deal_thai(maple_time_t *tv_time);
__s32 maple_time_deal(maple_time_t *tv_time);

void maple_time_monitor_stop(void);
__s32 maple_time_monitor_get(maple_time_t *tv_time);

__s32 maple_register_sepg_callback(__pCBK_t search_sepg_finish_callback);
__s32 maple_sepg_clean(void);
__s32 maple_sepg_search(void);
//__s32 maple_sepg_search_stop(void);

//__s32 maple_get_ss_status(int *strength,int*quality);

__s32 maple_get_ss(void);

#if(EN_DTV_RECORD == 1)
//__s32 MAPLE_CMD_GET_RECORD_STATUS(void);

__bool maple_control_TS_record_start(void);
__bool maple_control_TS_record_stop(void);
#endif

__u8  maple_control_get_lock_status( void );
maple_ve_status_e *maple_get_decode_status(void);

__s32 maple_set_tv_mode(__u8 flag);



__s32 maple_save_reg_current_program(void);
__s32 maple_load_reg_current_program(void);

__s32 maple_save_reg_video_window_ratio_mode(void);
__s32 maple_load_reg_video_window_ratio_mode(void);

__s32 maple_inc_volume(void);
__s32 maple_dec_volume(void);
__s32 maple_get_volume(void);


__s32 maple_set_screensize(maple_vd_win_ratio_mode_e mode);
__s32 maple_set_init_screensize(void);

__s32 maple_set_area(__u32 area);
__s32 maple_get_area(void);

__s32 maple_clear_database(char *database_file_name);

__u32 ymd2mjd(__u32 year, __u32 month, __u32 date);
void mjd2ymd(__u32 MJD, __u32* pYear, __u32* pMonth, __u32* pDate);
void getweekday(__u32 year, __u32 month, __u32 day, __u32* pWeekDay);

#if ((MAPLE_TV_STANDAR == MAPLE_TV_ISDB_T)||(MAPLE_TV_STANDAR == MAPLE_TV_DTMB)\
			||(MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)||(MAPLE_TV_STANDAR == MAPLE_TV_ATSC))
maple_cas_param_t *maple_ca_get_sn(void);
#endif
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB) 
__s32 maple_ca_get_sn(void);
__s32 maple_register_cmmb_datasvcupdate_begin_callback(__pCBK_t cmmb_data_svc_update_begin_cb);
__s32 maple_register_cmmb_datasvcupdate_end_callback(__pCBK_t cmmb_data_svc_update_end_cb);
#endif


__s32 maple_get_cur_search_freq(__s32 index);//add by hot.lee
__s32 maple_get_cur_search_freq_tatol(void);


//获取当前台数
__s32 maple_get_cur_service_list_num(void);
__s32 maple_get_cur_program_index(void);
// 获取频道名称
char  *maple_get_name_of_channel(__s32 index);
char *maple_get_channel_of_curprogramname(void);
char *maple_get_channel_of_nextprogramname(void);
char *maple_get_channel_of_enventcontent(void);


__s32 maple_service_list_get(maple_serv_group_type_e type);

void maple_set_video_window_ratio_mode(maple_vd_win_ratio_mode_e radio_mode);
__s32  maple_get_video_window_ratio_mode(void);

__s32  maple_set_cur_channel(__s32 index);

void maple_break_search_task(void);
__s32 maple_check_device(void);
__s32 maple_set_record_disk_full_cb(__pCBK_t full_cb);

__s32 maple_set_default_char_coding_type(void);

__s32 maple_get_current_av_info(void);
void maple_reset_audio_num(void);
__u8 maple_get_audio_num(__s32 index);
__u8 maple_set_audio_num(__u8 num);
__s32 maple_get_cur_channel_austream_cnt(void);
__s32 maple_get_current_austream(void);
maple_ad_info_t *maple_get_cur_channel_austream_info(void);
__s32  maple_get_cur_channel_lcn_id(__s32 index);
__bool maple_get_lcn_state(void);
void maple_set_lcn_state(__bool state);
__bool maple_get_antenna_state(void);
void maple_set_antenna_state(__bool state);
__s32 maple_set_cur_channel_index_by_lcn(__s32 nRemoteControlKeyId);
__s32 maple_get_cur_channel_index_by_lcn(__s32 nRemoteControlKeyId);


#endif	/*	__LION_H__	*/

