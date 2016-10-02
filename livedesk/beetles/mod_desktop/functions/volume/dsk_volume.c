/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: dsk_volume.c
* By      	: Andy.zhang
* Func		: voice control
* Version	: v1.0
* ============================================================================================================
* 2009-7-8 10:02:17  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "..\\functions_i.h"

#define AUDIO_DRV_PATH "b:\\AUDIO\\PLAY"
#define VOLUME_MAPPING		0

static __bool dsk_volume_gate_on = EPDK_TRUE;
static __s32 dsk_volume = EPDK_TRUE;
static __bool dsk_speak_gate_on = EPDK_FALSE;////EPDK_FALSE - speakoff,EPDK_TRUE - speakon
static __s32  volumemap = 27;

static __hdle shdn_hdl = NULL;

static __bool rec_is_play = EPDK_TRUE;
static __s32 fsm_statu   = 1;
static __bool is_phone_in = 0;
static __bool is_mic_in = 0;

static __s32 _set_volume( __s32 volume );
static __s32 _get_volume( void );


/*
****************************************************************************************************
*Name        : dsk_volume_set_volume
*Prototype   : __s32 dsk_volume_set_volume(__s32 volume)
*Arguments   : volume     input. volume value, ranges AUDIO_DEVICE_VOLUME_MIN~~AUDIO_DEVICE_VOLUME_MAX.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the volume.
*Other       :
****************************************************************************************************
*/
__s32 dsk_volume_set(__s32 volume)	// 设置当前音量
{
	__s32        ret;
	
	if( !dsk_volume_gate_on )
	{
	    return EPDK_FAIL;
	}
	   
	ret = _set_volume( volume );
	if( ret != EPDK_FAIL )
	{
	    dsk_volume = volume;	

        {
            reg_system_para_t* para;
            para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
            if(para)
            {
                para->volume = volume;
            }
        }   
	}
	
	return ret;	
}



/*
****************************************************************************************************
*Name        : dsk_volume_get_volume
*Prototype   : __s32 dsk_volume_get_volume(void)
*Arguments   : void
*Return      : the current volume value. -1 means fail.
*Description : get the curret volume value.
*Other       :
****************************************************************************************************
*/
__s32 dsk_volume_get(void)		// 当前音量大小
{
	__here__;
	if( !dsk_volume_gate_on )
	{
	    return 0;
	}
	__here__;

    //return dsk_volume;
	return _get_volume();
}



/*
****************************************************************************************************
*Name        : dsk_volume_inc_volume
*Prototype   : __s32 dsk_volume_inc_volume(void);
*Arguments   : void
*Return      : the current volume value. -1 means fail.
*Description : increase the volume.
*Other       :
****************************************************************************************************
*/
__s32 dsk_volume_inc(void)		// 增大音量
{
	__s32		volume;
	__s32       ret;

	if( !dsk_volume_gate_on )
	{
	    return -1;
	}
	
	volume = _get_volume();
	if( volume == -1 )
	{
	    return -1;
	}
	
	if( volume < AUDIO_DEVICE_VOLUME_MAX )
	{
		volume++;	
		ret = _set_volume( volume );
    	if( ret != EPDK_FAIL )
    	{
    	    dsk_volume = volume;

            {
                reg_system_para_t* para;
                para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
                if(para)
                {
                    para->volume = volume;
                }
            }   
    	    return volume;	
    	}
    	else
    	{
    	    return -1;
    	}
	}
	else
	{
	    return volume;
	}
}

/*
****************************************************************************************************
*Name        : dsk_volume_dec_volume
*Prototype   : __s32 dsk_volume_dec_volume(void);
*Arguments   : void
*Return      : the current volume value. -1 means fail.
*Description : increase the volume.
*Other       :
****************************************************************************************************
*/
__s32 dsk_volume_dec(void)		// 减小音量
{
	__s32		volume;
	__s32       ret;
	
	if( !dsk_volume_gate_on )
	{
	    return -1;
	}
	
	volume = dsk_volume_get();
	if( volume == -1 )
	{
	    return -1;
	}
	
	if( volume > AUDIO_DEVICE_VOLUME_MIN )
	{
		volume--;	
		ret = _set_volume( volume );

    	if( ret != EPDK_FAIL )
    	{
    	    dsk_volume = volume;

            {
                reg_system_para_t* para;
                para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
                if(para)
                {
                    para->volume = volume;
                }
            }       
    	    return volume;	
    	}
    	else
    	{
    	    return -1;
    	}
	}
	else
	{
	    return volume;
	}
}



/*
****************************************************************************************************
*Name        : dsk_volume_on
*Prototype   : __s32 dsk_volume_on(void);
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : make the voice on.
*Other       :
****************************************************************************************************
*/
__s32 dsk_volume_on( void )
{
	if( dsk_volume_gate_on )
	{
	    return EPDK_OK;
    }
    
    _set_volume( dsk_volume );
	
	dsk_volume_gate_on = EPDK_TRUE;
	
	return EPDK_OK;	
}


/*
****************************************************************************************************
*Name        : dsk_volume_off
*Prototype   : __s32 dsk_volume_off(void);
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : make the voice off(mute).
*Other       :
****************************************************************************************************
*/
__s32 dsk_volume_off( void )	         // close voice
{
	if( !dsk_volume_gate_on )
	{
	    return EPDK_OK;
    }
    
    _set_volume( 0 );              // mute
	
	dsk_volume_gate_on = EPDK_FALSE;
	
	return EPDK_OK;	
}



/*
****************************************************************************************************
*Name        : dsk_volume_is_on
*Prototype   : __bool dsk_volume_is_on( void )
*Arguments   : void
*Return      : the status of the voice, on or off(mute).
*Description : get the status of the voice, on or off(mute).
*Other       : 
****************************************************************************************************
*/
__bool dsk_volume_is_on( void )
{
    return 	dsk_volume_gate_on;
}



static __s32 _set_volume( __s32 volume )
{
	ES_FILE		*p_dac;	
	__s32        ret;

    if(volume < AUDIO_DEVICE_VOLUME_MIN
        || volume > AUDIO_DEVICE_VOLUME_MAX)
    {
        return EPDK_FAIL;
    }
    
	p_dac = eLIBs_fopen(AUDIO_DRV_PATH, "r+");
	if(!p_dac)
	{
		__err("open dac device fail!\n");
		return EPDK_FAIL;
	}
	
	////////////
#if VOLUME_MAPPING	
	if((27 == volume) || (28 == volume))
	{
		volumemap = volume;
		volume = 27;
	}
	else if(volume >= 29)
	{
		volumemap = volume;
		volume = 28;
	}
	else
	{
		volumemap = volume;
	}
#endif	
	//////////////
	//__msg(">>>>>>>>_set_volume>>>>volume = %d\n",volume);
	ret = eLIBs_fioctrl(p_dac, AUDIO_DEV_CMD_SET_VOLUME, volume, 0);
		
	eLIBs_fclose(p_dac);   
	
	return ret;
}

static __s32 _get_volume( void )
{
	ES_FILE		*p_dac;	
	__s32        volume;
	
	p_dac = eLIBs_fopen(AUDIO_DRV_PATH, "r+");
	if(!p_dac)
	{
		__err("open dac device fail!\n");
		return -1;
	}
	
	volume = eLIBs_fioctrl(p_dac, AUDIO_DEV_CMD_GET_VOLUME, 0, 0);	


#if VOLUME_MAPPING	
	if(volume >= 27)
	{
		volume = volumemap;
	}
#endif


	eLIBs_fclose(p_dac);		

	
	return volume;
}


void  dsk_speaker_onoff(__u32 onoff)
{	
	user_gpio_set_t  gpio_set[1];
	__s32 ret;

	if(!shdn_hdl)
	{
		eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
		ret = esCFG_GetKeyValue("audio_para", "PA_SHDN", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
		if (!ret)
		{
			shdn_hdl = esPINS_PinGrpReq((__user_pin_set_t*)gpio_set, 1);
			if (!shdn_hdl)
			{
				__wrn("request input_row1 pin failed\n");
				return;
			}
		}
		else
		{
			__wrn("fetch para from script failed\n");
			return;
		}
	}

	ret = esPINS_WritePinData(shdn_hdl, onoff, NULL);
	if (ret)
	{
		__msg("pull output_col failed\n");
		return;
	}
	else
	{
		
		if(BEETLES_SPEAK_ON == onoff)
			dsk_speak_gate_on = EPDK_TRUE;
		else
			dsk_speak_gate_on = EPDK_FALSE;
		
		__msg("esPINS_SetPinPull success\n");
	}

	//esPINS_PinGrpRel(shdn_hdl, 0);
	
	return;
}

__bool dsk_speaker_is_on( void )
{
    return 	dsk_speak_gate_on;
}

void set_record_is_play(__bool val)
{
	rec_is_play = val;
}

__bool	dsk_record_is_play(void)
{
	return rec_is_play;
}

void dsk_set_fsm_statu(__s32 sta)
{
	fsm_statu = sta;

	return;
}

__s32 dsk_get_fsm_statu(void)
{
	return fsm_statu;
}

__s32 dsk_set_is_phone_in(__bool val)
{
	is_phone_in = val;
}

__bool dsk_get_is_phone_in(void)
{
	return is_phone_in;
}

__s32 dsk_set_is_mic_in(__bool val)
{
	is_mic_in = val;
}

__bool dsk_get_is_mic_in(void)
{
	return is_mic_in;
}

