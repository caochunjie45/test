
#ifndef  __LOGIC_MANAGER_H_
#define  __LOGIC_MANAGER_H_


#include "apps_i.h"

#include "SP\dtv_mid.h"




enum
{
	logic_windows = APP_DTV_ID + 30,	
};


typedef struct
{
	power_level_e               voltage_level;

	__u8                        thread_id;

	__s32                       strength;
	
	__s32                       quality;

	__u32						search_task_id;
	
	__bool                      weak_singal;
	
	__bool                      record_status;

	__krnl_event_t              *sem;

	maple_save_para_t          	*save_data;

	
}logic_data_t;


typedef enum
{
    SPEAKER_OFF,
    
    SPEAKER_ON,
    
}speaker_state;





enum Logic_Cmd
{
	LOGIC_GET_VOLUME,                  //获取音量
	
	LOGIC_SET_VLOUME,                  //设置音量
	
	LOGIC_GET_VOLTAGE,                 //获取电压等级
	
	LOGIC_GET_SINGNAL_STRENGTH,        //获取信号强度

	LOGIC_GET_SINGNAL_STRENGTH_LEVEL,  //获取信号强度等级
	
	LOGIC_GET_SINGNAL_QUALITY,         //获取信号质量
	
	LOGIC_GET_TITLE_CHANNEL_NAME,      //获取顶部节目名称
	
	LOGIC_GET_LIST_CHANNEL_NAME, 	   //获取列表节目名称	
	
	LOGIC_SET_ANTNNA_STATUS,           //设置天线的状态
	
	LOGIC_GET_ANTENA_STATUS,           //获取天线的状态
	
	LOGIC_SET_LCN_STATUS,              //设置LCN状态
	
	LOGIC_GET_LCN_STATUS,              //获取LCN状态
	
	LOGIC_GET_SINGAL_STATUS,           //获取信号的状态，弱信号还是强信号

	LOGIC_GET_SIMPLE_EPG,              //获取EPG信息

	LOGIC_GET_LONG_EPG,                //获取EPG详细信息

	LOGIC_GET_TOTAL_FREQUENCY,         //获取总的搜索频点
	
	LOGIC_GET_CURRENT_FREQUENCY,       //获取当前的搜索频点
	
	LOGIC_SET_SCREENSIZE,              //设置屏幕的显示方式
	
	LOGIC_GET_SCREENSIZE,              //获取屏幕的显示方式

	LOGIC_SET_BRIGHTNESS,              //设置屏幕的亮度
	
	LOGIC_GET_BRIGHTNESS,              //获取屏幕的亮度

	LOGIC_NEXT_CHANNEL,                //下一个节目
	
	LOGIC_PREV_CHANNEL,                //上一个节目

	LOGIC_CHANGE_CHANNEL,              //换台

	LOGIC_DIGIT_SELECT_CHANNEL,        //按数字键换台

	LOGIC_START_RECORD,                //开始录制

	LOGIC_STOP_RECORD,                 //停止录制
	
};





enum
{
	Creat_Loading_Win = 0,
		
	Destory_Loading_Win,

	Creat_Search_Win,

	Update_Search_Frequency,

	Update_Search_Channel,
	
	Destory_Search_Win,

	Creat_Destory_Win,
		
	Destory_Destory_Win,
	
	Create_Dialog_Win,

	Destory_Dialog_Win,

	Create_Record_Win,

	Destory_Record_Win,

	Create_Singal_Win,

	Destory_Singal_Win,
		

	CMD_FS_PART_PLUGOUT,
	
};




int Register_One_Logic_Win(__u32 ID,Man_Win *ManWin);


#endif

