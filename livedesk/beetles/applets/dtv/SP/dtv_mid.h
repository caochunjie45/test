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


#include     "dtv_i.h"
#include     "dtv_frequency.h"



#define      DTV_DATABASE_NAME               "Z:\\program.db"

#define      DTV_BASEINFO_NAME               "Z:\\baseinfo.bin" 

#define      DTV_RECORD_ROOT_PATH	         "\\DTVRECORD\\"

#define      DTVRECORD_FILTER		         ".ts"

#define      QSIZE                           8

#define      MAX_EPG_DAY                     7

#define      MAX_SEPG_STRING                 128

#define      MAX_TMP_STRING                  256


#define      MAX_CHANNEL_NUM                 64

#define      SINGAL_LEVEL0                   0

#define      SINGAL_LEVEL1                   20

#define      SINGAL_LEVEL2                   40

#define      SINGAL_LEVEL3                   60

#define      SINGAL_LEVEL4                   80

#define      SINGAL_LEVEL5                   100


#define      G_DTV_current_area              0 //1;   //0:只用于深圳频率，1:全频段搜索


#define      G_TYPE                          EPDK_CHARSET_ENM_ISO_8859_15 //默认巴西,需要根据出往哪个国家进行设置


#define      PROGRAMTYPE                     SSET_NC_PROGRAMTYPE_TV



typedef struct
{    
    __s32                                    index;//第几个台
    
    maple_vd_win_ratio_mode_e                mode;
    
	__bool                                   antenna_status;

	__bool                                   LCN_status;

	__u8                                     channel_total;
		
	__u32                                    time_format;             //时制，0代表24小时制，1代表12小时制
	       
    __u8                                     audio_num_select[MAX_CHANNEL_NUM];
    
	__u32                                    reserverd[8];            //保留参数设置

	
}maple_save_para_t;




enum 
{
	DELETE_SEARCH_THREAD,
		
	UPDATE_FREQ,
	
	UPDATE_CHANNEL,

};




typedef	enum tag_programtype
{
	SSET_NC_PROGRAMTYPE_TV,
		
	SSET_NC_PROGRAMTYPE_RADIO,
	
	SSET_NC_PROGRAMTYPE_ALL,
	
	SSET_NC_PROGRAMTYPE_MAX,
	
}programtype_e;




typedef struct __maple_sl_cb_param_t_pOpaque
{
	void                             *pOpaque;
	
	__gui_msg_t                      *msg;
	

}maple_sl_cb_param_t_pOpaque;


typedef struct __maple_schdl_list_day
{
    __u32                             eventNum;       //* how many events totally in the list
    
	__u32                             MJD;
	
    __u32                             serviceid;
	
    maple_schdl_event_t               pSchdlEvents[512];   //* schedule event list

}maple_schdl_list_day;



typedef struct DTVMID_TIME 
{
    __u32                             year;
	
    __u32                             month;
	
    __u32                             day;
	
    __u32                             weekDay;
	
    __u32                             hour;
	
    __u32                             min;
	
	__u32                             sec;
	
	__u32                             MJD;
	
	
}dtvmid_time_t;


__s32 Maple_Set_Logic_Win(H_WIN win);

__s32 Maple_Get_Current_Search_Frequency(__s32 index);

__s32 Maple_Get_Total_Frequency(void);

void Maple_Check_Disk_Format(void);

maple_save_para_t *Maple_Get_Save_Data(void);

__s32 Maple_Program_Load(char *database_file_name);

__s32 Maple_Reg_Flush(void);

__s32 Maple_Program_Save(char *database_file_name);

__s32 Maple_Register_Schedule_Callback(__pCBK_t cb_search_schedule_event_callback,__pCBK_t cb_search_schedule_finish_callback);

__s32  Maple_Reset_Schedule_Event(__u32 serviceid);

__s32 Maple_Add_Schedule_Event(maple_schdl_event_t * event);

__s32 Maple_Get_Schedule(void);

__s32 Maple_Get_Current_ScheduleEvent_Index(__s32 day_index);

__s32 Maple_Get_ScheduleEvent_Num(__s32 day_index);

maple_schdl_event_t *Maple_Get_Schedule_Event(__s32 day_index,__s32 event_index);

__s32 Maple_Epg_Search(void);

__s32 Maple_Epg_Search_Stop(void);

__s32 Maple_Time_Monitor_Start(void);

__s32 Maple_Time_Monitor_Stop(void);

__s32 Maple_Time_Monitor_Get(maple_time_t *tv_time);

__s32 Maple_Set_Antenna_State(__bool state);

__s32 Maple_Reset_Audio_Num(void);

__u8 Maple_Get_Audio_Num(__s32 index);

__u8 Maple_Set_Audio_Num(__u8 num);

__s32 Maple_Get_Current_Channel_Austream_Cnt(void);

__s32 Maple_Get_Current_Austream(void);

maple_ad_info_t *Maple_Get_Current_Channel_Austream_Info(void);

char *Maple_Get_Current_Sepg(void);

char *Maple_Get_Next_Sepg(void);

__s32 Maple_Clear_Sepg(void);

__s32 Maple_Sepg_Search(void);

__s32 Maple_Check_Device(void);

__s32 Maple_Set_Record_Disk_Full_CB(__pCBK_t full_cb);

__s32 Maple_Get_Record_Staue(void);

__bool Maple_Control_TS_Record_Start(void);

__bool Maple_Control_TS_Record_Stop(void);

maple_ve_status_e *Maple_Get_Decode_Status(void);

__s32 Maple_Set_TV_Mode(__u8 flag);

__s32 Maple_Set_ScreenSize(maple_vd_win_ratio_mode_e usr_mode);

maple_cas_param_t *Maple_Get_Ca_Sn(void);

__s32 Maple_Set_Subtitle_CB(__pCBK_t isdbt_cb);

__s32 Maple_Set_Default_Char_Coding_Type(void);

__s32 Maple_Get_SS(maple_demod_ss_t *singal);

__s32 Maple_Play_Audstream(__u32 audioIdx);

__s32 Maple_Get_Current_Av_Info(void);

__s32 Maple_Service_List_Get(maple_serv_group_type_e type);

__s32 Maple_Program_Add_ServiceList(void);

__s32 Maple_Program_Get_ServiceList(__u32 programtype);

__s32 Maple_Get_Play_Status(void);

__s32 Maple_Get_Total_Services(void);

char  *Maple_Get_Name_Of_Channel(__s32 index);

__s32 Maple_Get_Current_Channel_Index(void);

__s32  Maple_Set_Current_Channel(__s32 index);

__s32 Maple_Get_Current_Channel_Index_By_LCN(__s32 nRemoteControlKeyId);

__s32 Maple_Set_Current_Channel_Index_By_LCN(__s32 nRemoteControlKeyId);

__s32  Maple_Get_Current_Channel_LCN_ID(__s32 index);

__s32 Maple_Play_Program(void);

__s32 Maple_Stop_Program(__u32 video_layer_state);

__s32 Maple_Search_List_Finish_Callback(void* arg);

__s32 Maple_Search_List_Event_Callback(void* arg);

__s32 Maple_Register_Search_Callback(__pCBK_t cb_search_list_event_callback,__pCBK_t cb_search_list_finish_callback);

__s32 Maple_Break_Search(void);

__s32 Maple_Search(__u32 *task_ID);

__s32 Maple_Search_Delete(__u32 thread_id);

__s32 Maple_Open(void);

__s32 Maple_Close(void);


#endif	/*	__LION_H__	*/

