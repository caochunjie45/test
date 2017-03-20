

#ifndef __DTV_I_H__
#define __DTV_I_H__


#include      "beetles_app.h"
#include      "epdk.h"
#include      "mod_maple.h"
#include      "apps.h"


//以下为CMMB/ISDBT需要配置的信息
#define      MAPLE_TV_STANDAR                MAPLE_TV_ISDB_T



typedef enum
{
    TV_SPEAKER_OFF,
    
    TV_SPEAKER_ON,
    
}tv_speaker_state;



typedef enum
{
    NOT_CLOSE_VIDEO_LAYER,
    
    CLOSE_VIDEO_LAYER,   
    
}tv_video_layer_state;


#endif

