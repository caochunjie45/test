#ifndef __MUSIC_UI_H__
#define __MUSIC_UI_H__

typedef enum music_list_icon_id_s			
{
	//for music explorer
	ID_MUSIC_PALETTE = 0,
  ID_MUSIC_BACKGROUND,
	ID_MUSIC_AUDIO_TYPE,
	ID_MUSIC_FILE_UNFOCUSED_ICON,		
	ID_MUSIC_FILE_FOCUSED_ICON,
	ID_MUSIC_FOCUS_ITEM_BORDER,
	ID_MUSIC_LONG_STR_SCROLL,

	//Headbar
	ID_MUSIC_PLAY_ALL_STATUS_ICON,
	ID_MUSIC_PLAY_ONE_STATUS_ICON,
	ID_MUSIC_PLAY_RANDOM_STATUS_ICON,	
	ID_MUSIC_REPEAT_ALL_STATUS_ICON,
	ID_MUSIC_REPEAT_ONE_STATUS_ICON,

	ID_MUSIC_EQ_NORMAL_STATUS_ICON,
	ID_MUSIC_EQ_CLASSIC_STATUS_CION,
	ID_MUSIC_EQ_ROCK_STATUS_CION,
	ID_MUSIC_EQ_JAZZ_STATUS_CION,
	ID_MUSIC_EQ_POP_STATUS_CION,

	ID_MUSIC_BL_ON_CION,
	ID_MUSIC_BL_TIME_10S_CION,
	ID_MUSIC_BL_TIME_30S_CION,	
	ID_MUSIC_BL_TIME_60S_CION,		
	ID_MUSIC_BL_TIME_90S_CION,			
	
	ID_MUSIC_VOL_STATUS_ICON,
	ID_MUSIC_VOL_MUTE_ICON,
	ID_MUSIC_BATTERY_0_ICON,
	ID_MUSIC_BATTERY_1_ICON,
	ID_MUSIC_BATTERY_2_ICON,
	ID_MUSIC_BATTERY_3_ICON,
	ID_MUSIC_BATTERY_4_ICON,
	ID_MUSIC_BATTERY_5_ICON,

    ID_MUSIC_CHARGE_BATTERY_0_ICON,
	ID_MUSIC_CHARGE_BATTERY_1_ICON,
	ID_MUSIC_CHARGE_BATTERY_2_ICON,
	ID_MUSIC_CHARGE_BATTERY_3_ICON,
	ID_MUSIC_CHARGE_BATTERY_4_ICON,
	ID_MUSIC_CHARGE_BATTERY_5_ICON,
	
	ID_MUSIC_PLAY_ALL_BTN_UNFOCUSED_ICON,	
	ID_MUSIC_PLAY_ALL_BTN_FOCUSED_ICON,	
	ID_MUSIC_PLAY_ALL_BTN_PRESSED_ICON,
		
	ID_MUSIC_PLAY_ONE_BTN_UNFOCUSED_ICON,
	ID_MUSIC_PLAY_ONE_BTN_FOCUSED_ICON,
	ID_MUSIC_PLAY_ONE_BTN_PRESSED_ICON,
		
	ID_MUSIC_PLAY_RANDOM_BTN_UNFOCUSED_ICON,	
	ID_MUSIC_PLAY_RANDOM_BTN_FOCUSED_ICON,
	ID_MUSIC_PLAY_RANDOM_BTN_PRESSED_ICON,
		
	ID_MUSIC_REPEAT_ALL_BTN_UNFOCUSED_ICON,	
	ID_MUSIC_REPEAT_ALL_BTN_FOCUSED_ICON,
	ID_MUSIC_REPEAT_ALL_BTN_PRESSED_ICON,
			
	ID_MUSIC_REPEAT_ONE_BTN_UNFOCUSED_ICON,	
	ID_MUSIC_REPEAT_ONE_BTN_FOCUSED_ICON,
	ID_MUSIC_REPEAT_ONE_BTN_PRESSED_ICON,
	
	ID_MUSIC_EQ_MODE_BTN_UNFOCUSED_ICON,
	ID_MUSIC_EQ_MODE_BTN_FOCUSED_ICON,			
	ID_MUSIC_EQ_MODE_BTN_PRESSED_ICON,
	
	ID_MUSIC_BL_TIME_BTN_UNFOCUSED_CION,
	ID_MUSIC_BL_TIME_BTN_FOCUSED_CION,	
	ID_MUSIC_BL_TIME_BTN_PRESSED_CION,
	
	ID_MUSIC_PLAY_PRE_BTN_UNFOCUSED_ICON,
	ID_MUSIC_PLAY_PRE_BTN_FOCUSED_ICON,	
	ID_MUSIC_PLAY_PRE_BTN_PRESSED_ICON,
	
	ID_MUSIC_PLAY_NEXT_BTN_UNFOCUSED_ICON,
	ID_MUSIC_PLAY_NEXT_BTN_FOCUSED_ICON,		
	ID_MUSIC_PLAY_NEXT_BTN_PRESSED_ICON,	
	
	ID_MUSIC_PLAY_PAUSE_BTN_UNFOCUSED_ICON,
	ID_MUSIC_PLAY_PAUSE_BTN_FOCUSED_ICON,		
	ID_MUSIC_PLAY_PAUSE_BTN_PRESSED_ICON,	
	
	ID_MUSIC_PLAY_BACKWARD_BTN_UNFOCUSED_ICON,
	ID_MUSIC_PLAY_BACKWARD_BTN_FOCUSED_ICON,		
	ID_MUSIC_PLAY_BACKWARD_BTN_PRESSED_ICON,	
	
	ID_MUSIC_PLAY_FORWARD_BTN_UNFOCUSED_ICON,	
	ID_MUSIC_PLAY_FORWARD_BTN_FOCUSED_ICON,	
	ID_MUSIC_PLAY_FORWARD_BTN_PRESSED_ICON,	

	ID_MUSIC_PALYING_STATUS_ICON,														
	ID_MUSIC_PAUSE_STATUS_ICON,															
	ID_MUSIC_PREVIOUS_STATUS_ICON,													
	ID_MUSIC_NEXT_STATUS_ICON,															
															
	ID_MUSIC_PROCESS_BAR_HEAD_ICON,													
	ID_MUSIC_PROCESS_BAR_BODY_ICON,
	ID_MUSIC_PROCESS_BAR_TAIL_ICON,													
	ID_MUSIC_PROCESS_BAR_POINT_ICON,												
	ID_MUSIC_PERFORMER_ICON,
	ID_MUSIC_FILE_SIZE_ICON,
	ID_MUSIC_ALBUM_ICON,
	ID_MUSIC_BIT_RATE_ICON,	

	ID_MUSIC_PARTICLE,
	ID_MUSIC_BOUY,

//scroll bar for explorer
	ID_MUSIC_LIST_SCROLL_BG,
	ID_MUSIC_LIST_SCROLL_HEAD,
	ID_MUSIC_LIST_SCROLL_BODY,
	ID_MUSIC_LIST_SCROLL_TAIL,

	ID_INIT_SM_0,
	ID_INIT_SM_1,
	ID_INIT_SM_2,
	ID_INIT_SM_3,
	ID_INIT_SM_4,
	ID_INIT_SM_5,
	ID_INIT_SM_6,
	ID_INIT_SM_7,
	ID_INIT_SM_8,
	ID_INIT_SM_9,

   MUSIC_ICON_MAX, 
}music_list_icon_id_e;							//e:����enum,  s:����struct

typedef struct music_icon_data_s
{
    __u32 res_id;
    HTHEME res_handle;
    void *res_buffer;
}music_icon_data_t;									//icon data struct

typedef struct music_layer_ui_s
{
    __pixel_rgbfmt_t fb_fmt;        //ͼ���ʽ
    SIZE fb_size;                   //framebuffer��С
    RECT src_rect;                  //srcwin����	
    RECT scn_rect;                  //scnwin����
    __u32 reserved[32];             //Ԥ��
}music_layer_ui_t;

typedef struct music_list_ui_s
{
		RECT listbar_rect;									//list display area in list mode 
    RECT item_rect;											//list item  UI ����       
    
    RECT focus_icon_rect;  						//select border
    RECT file_icon_rect;              //�ļ�ͼ��             		
		
		RECT text_rect; 
}music_list_ui_t;

typedef struct music_scrollbar_ui_s
{
    RECT BG_rect;						//��������
    RECT scroll_rect;				//������
    __u32 head_height;
    __u32 body_height;
    __s32 tail_height;
        
}music_scrollbar_ui_t;



//Headbar״̬��ui����
typedef struct music_headbar_ui_s
{
    RECT BG_rect;							//��������

	RECT sd_rect;							//SDͼ��
	RECT usb_rect;							//usbͼ��
	RECT cur_rect;							//usbͼ��
    RECT play_mode_rect;					//����ģʽ��ť
    RECT EQ_mode_rect;						//EQģʽͼ��
    RECT BLight_rect;						//����ر�ʱ��ͼ��
    RECT volume_icon_rect;					//����ͼ��
    RECT volume_text_rect;					//����ֵ��ʾ����
    RECT battery_rect;						//���ͼ��
    RECT sys_time_rect;    						//ʱ����ʾ
}music_headbar_ui_t;


//���������沿��
typedef struct music_palyer_ui_s
{
		RECT contrl_BG_rect;		//���ư�ť������ʾ������Ƶ�ײ��ֹ���һ����ʾ����
		RECT btn_hint_rect;			//������ʾ������
		RECT play_mode_btn_rect;	//����ģʽ��ť
		RECT EQ_mode_btn_rect;		//EQģʽ��ť
		RECT BLight_btn_rect;			//����ʱ����ư�ť
		
		RECT play_pre_btn_rect;		//��һ�װ�ť
		RECT play_next_btn_rect;	//��һ�װ�ť			    
		RECT play_pause_btn_rect;	//������ͣ��ť
		RECT backward_btn_rect;		//���˰�ť
		RECT forward_btn_rect;		//�����ť
		
		RECT play_status_rect;		//����״̬ͼ��(��ͣ�򲥷�)
		RECT play_time;						//��ǰ����ʱ��
		RECT total_time;					//��ʱ����ʾ����
		
		RECT process_bar_head_rect;		//������ͷ��		
		RECT process_bar_body_rect;		//������body	
		RECT process_bar_point_rect;	//��ǰ���ȵ�
		RECT process_bar_tail_rect;		//������β��
		
		
		RECT performer_icon_rect;	//�ݳ���ͼ��
		RECT performer_name_rect; //�ݳ�������
		RECT file_size_icon_rect;	//�ļ���Сͼ��
		RECT file_size_text_rect;			//�ļ���С
		RECT album_icon_rect;		//ר������ͼ��
		RECT album_text_rect;		//ר�������ı�
		RECT bit_rate_icon_rect;	//BitRateͼ��
		RECT bit_rate_text_rect;	//�ı���ʾ
}music_palyer_ui_t;

//����������ʾ����
typedef struct music_volume_bar_ui_s
{		
		RECT BG_rect;						//����������������
		RECT left_speaker_rect;	//�������ͼ��
		RECT vol_bar_rect;			//����������ͼ��
		RECT right_speaker_rect;//�ұ�����ͼ��						
}music_volume_bar_ui_t;

typedef struct music_spectrum_ui_s
{
		RECT BG_rect;		
}music_spectrum_ui_t;
	
typedef struct music_ui_s
{
	music_layer_ui_t 			layer;				//����������ͼ���������
	
	music_list_ui_t				list_ctrl;		//�����list
	music_scrollbar_ui_t	scrollbar;		//�����������
	
	music_headbar_ui_t		headbar;			//����״̬��
	music_palyer_ui_t     player;				//����������
	music_volume_bar_ui_t volume_bar;		//����������

	music_spectrum_ui_t     spectrum;
	music_icon_data_t 		icon_table[MUSIC_ICON_MAX+1];
}music_ui_t;

//��ʾ������
#define		C_LAYER_CACHE_AREA_X 		239+50	//210
#define		C_LAYER_CACHE_AREA_Y		120		//40
#define		C_LAYER_CACHE_AREA_W		180
#define		C_LAYER_CACHE_AREA_H		140

extern music_ui_t* music_get_ui_param(void);					//���UI����

//���Explorer ����layer size
extern __s32 music_get_screen_rect(RECT *screen_rect);

extern void* music_get_icon_res(__u32 index);
extern __s32 music_free_icon_res(void);


//������������UI����
extern music_list_ui_t *music_get_list_ui_param(void);

//��ö���HeadbarUI����
extern music_headbar_ui_t *music_get_headbar_ui_param(void);

//��ö���playerUI����
extern music_palyer_ui_t *music_get_player_ui_param(void);

//���volume barUI����
extern music_volume_bar_ui_t *music_get_volume_bar_ui_param(void);

//���listbarÿҳ������ʾ��Ŀ��
extern __u32 music_count_listbar_pagenum(void);
#endif



