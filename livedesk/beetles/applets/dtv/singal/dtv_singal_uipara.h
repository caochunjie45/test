
#ifndef __dtv_singal_PARA_H_
#define  __dtv_singal_PARA_H_
#include "dtv_singal_i.h"

#define  MAX_CHARGE_LEVEL  6
#define  MAX_SINGAL_LEVEL  6
#define  MAX_VOL_PIC       3


typedef struct dtv_singal_uipara
{
     //顶部的信号坐标
	RECT dtv_singal_scene;        //图层大小
	
	RECT dtv_title;               //节目名称
	
	RECT dtv_singal_quality;      //信号质量文字
	
	RECT dtv_singal_strength;     //信号强度文字
	
    RECT dtv_singal_strength_bmp; //信号强度
        
    RECT dtv_volume;
    
    RECT dtv_volume_txt;
    
    RECT dtv_charge;
    
    RECT   cur_next_prog[2];      //当前、下一个播放节目
    
	//节目预览的弱信号坐标
	RECT dtv_no_signal;
	
    RECT dtv_no_signal_text;

    __s32 dtv_volume_state;
    
    __s32 dtv_volume_mute;
    
    __s32 dtv_singal_bmp[MAX_SINGAL_LEVEL]; //信号强度图片
    
    __s32 id_charge[MAX_CHARGE_LEVEL];
    
    __s32 id_battery[MAX_CHARGE_LEVEL];
    	
	__s32 id_list_bar;
	
}dtv_singal_uipara_t;

dtv_singal_uipara_t* dtv_singal_get_uipara(void);

#endif

