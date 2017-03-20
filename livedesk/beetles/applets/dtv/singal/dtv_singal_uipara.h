
#ifndef __dtv_singal_PARA_H_
#define  __dtv_singal_PARA_H_
#include "dtv_singal_i.h"

#define  MAX_CHARGE_LEVEL  6
#define  MAX_SINGAL_LEVEL  6
#define  MAX_VOL_PIC       3


typedef struct dtv_singal_uipara
{
     //�������ź�����
	RECT dtv_singal_scene;        //ͼ���С
	
	RECT dtv_title;               //��Ŀ����
	
	RECT dtv_singal_quality;      //�ź���������
	
	RECT dtv_singal_strength;     //�ź�ǿ������
	
    RECT dtv_singal_strength_bmp; //�ź�ǿ��
        
    RECT dtv_volume;
    
    RECT dtv_volume_txt;
    
    RECT dtv_charge;
    
    RECT   cur_next_prog[2];      //��ǰ����һ�����Ž�Ŀ
    
	//��ĿԤ�������ź�����
	RECT dtv_no_signal;
	
    RECT dtv_no_signal_text;

    __s32 dtv_volume_state;
    
    __s32 dtv_volume_mute;
    
    __s32 dtv_singal_bmp[MAX_SINGAL_LEVEL]; //�ź�ǿ��ͼƬ
    
    __s32 id_charge[MAX_CHARGE_LEVEL];
    
    __s32 id_battery[MAX_CHARGE_LEVEL];
    	
	__s32 id_list_bar;
	
}dtv_singal_uipara_t;

dtv_singal_uipara_t* dtv_singal_get_uipara(void);

#endif

