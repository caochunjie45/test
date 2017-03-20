
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
	LOGIC_GET_VOLUME,                  //��ȡ����
	
	LOGIC_SET_VLOUME,                  //��������
	
	LOGIC_GET_VOLTAGE,                 //��ȡ��ѹ�ȼ�
	
	LOGIC_GET_SINGNAL_STRENGTH,        //��ȡ�ź�ǿ��

	LOGIC_GET_SINGNAL_STRENGTH_LEVEL,  //��ȡ�ź�ǿ�ȵȼ�
	
	LOGIC_GET_SINGNAL_QUALITY,         //��ȡ�ź�����
	
	LOGIC_GET_TITLE_CHANNEL_NAME,      //��ȡ������Ŀ����
	
	LOGIC_GET_LIST_CHANNEL_NAME, 	   //��ȡ�б��Ŀ����	
	
	LOGIC_SET_ANTNNA_STATUS,           //�������ߵ�״̬
	
	LOGIC_GET_ANTENA_STATUS,           //��ȡ���ߵ�״̬
	
	LOGIC_SET_LCN_STATUS,              //����LCN״̬
	
	LOGIC_GET_LCN_STATUS,              //��ȡLCN״̬
	
	LOGIC_GET_SINGAL_STATUS,           //��ȡ�źŵ�״̬�����źŻ���ǿ�ź�

	LOGIC_GET_SIMPLE_EPG,              //��ȡEPG��Ϣ

	LOGIC_GET_LONG_EPG,                //��ȡEPG��ϸ��Ϣ

	LOGIC_GET_TOTAL_FREQUENCY,         //��ȡ�ܵ�����Ƶ��
	
	LOGIC_GET_CURRENT_FREQUENCY,       //��ȡ��ǰ������Ƶ��
	
	LOGIC_SET_SCREENSIZE,              //������Ļ����ʾ��ʽ
	
	LOGIC_GET_SCREENSIZE,              //��ȡ��Ļ����ʾ��ʽ

	LOGIC_SET_BRIGHTNESS,              //������Ļ������
	
	LOGIC_GET_BRIGHTNESS,              //��ȡ��Ļ������

	LOGIC_NEXT_CHANNEL,                //��һ����Ŀ
	
	LOGIC_PREV_CHANNEL,                //��һ����Ŀ

	LOGIC_CHANGE_CHANNEL,              //��̨

	LOGIC_DIGIT_SELECT_CHANNEL,        //�����ּ���̨

	LOGIC_START_RECORD,                //��ʼ¼��

	LOGIC_STOP_RECORD,                 //ֹͣ¼��
	
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

