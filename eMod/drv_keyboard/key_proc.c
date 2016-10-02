/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                   DRV KEY
*
*                                    (c) Copyright 2011-2014, Jackie China
*                                             All Rights Reserved
*
* File    : key_proc.c
* By      : Jackie
* Version : v1.0
* Date    : 2012-01-16
* Descript: key driver data process source file.
* Update  : date                auther      ver     notes
*           2012-01-16 15:07    Jackie      1.0     Create this file.
*********************************************************************************************************
*/
#include "drv_key_i.h"

#define KEY_DEFAULT_VALUE    (0x3f)

typedef struct KEY_CTRL
{  
	__hdle			h_lradc;
	
    __u32           pre_value;          //previous key value
    __u32           crt_value;          //current key value
    __u32           effective_value;    //send keydown msg,not use now
                                     
    __u32           keydown_cnt;        //continuous key press
                                        
    __u32           debounce_time;      //filter para,set by USER.   
    __u32           first_key_time;     //sensitive
    __u32           second_key_time;    
    __u32           repeat_key_time;    
    __u32           last_tick;          //last time tick when lradc key interrupt coming
}__key_ctrl_t;


static __key_ctrl_t key_ctrl;
__scancode2key_t scancode2key[]={
/*  { Min,  Max,    Keycode         } */ 
	{0, 	7,	    KPAD_MUTE},
	{8, 	14,  	KPAD_ENTER},
	{15,	21, 	KPAD_MENU},	
	{22,	27, 	KPAD_TV_RIGHT},	
	{28, 	33, 	KPAD_TV_LEFT},
	{34, 	39, 	KPAD_VOICEUP},	
	{40, 	49, 	KPAD_VOICEDOWN},	
	{50, 	63, 	KEY_MAX       }
};

static void __SendKeyDown(__u32 key_value)
{
    __key_msg_t msg;
    msg.key_value = key_value;
    msg.flag = 1;
	if(key_value != KEY_MAX)
    	DEV_KEY_Ioctrl(0, DRV_KEY_CMD_PUTKEY, 0, &msg);
}

static void __SendKeyup(__u32 key_value)
{
    __key_msg_t msg;
    msg.key_value = key_value;
    msg.flag = 0;
	if(key_value != KEY_MAX)
        DEV_KEY_Ioctrl(0, DRV_KEY_CMD_PUTKEY, 0, &msg);
}

static __u32 __GetKeyValue(__u32 scancode)
{
    __u32 i = 0;
    __u32 counter = sizeof(scancode2key)/sizeof(scancode2key[0]);

    eDbug("scancode is %d \n",scancode);
    if(scancode <= SCANCODE_MAX)
    {
        for(i = 0; i < counter; ++i)
        {
            if( (scancode >= scancode2key[i].keyMin) 
                    && (scancode <= scancode2key[i].keyMax) )
            {
                return scancode2key[i].keyCode;
            }
        }
    }

    return 0;//error    
}

static void __KeyDownProc(void *pkey)
{
    __key_ctrl_t *pKey = (__key_ctrl_t *)pkey;

    /* ignore if things not happen */
    if((pKey->effective_value != pKey->crt_value)
            && (pKey->effective_value != KEY_DEFAULT_VALUE) )
    {
        __SendKeyup(pKey->effective_value); //key up msg to upper layer   
        pKey->keydown_cnt = 0;          
       
    }

    __SendKeyDown(pKey->crt_value); //key down msg to upper layer  
        
    pKey->effective_value = pKey->crt_value;
    pKey->last_tick = 0;
    ++pKey->keydown_cnt;     

    if(pKey->keydown_cnt == 1)
    {
        pKey->debounce_time = pKey->second_key_time;// first_delay > first_time.
    }
    else if(pKey->keydown_cnt > 1)
    {
        pKey->debounce_time = pKey->repeat_key_time;//repeat_time < first_delay.
    }
    
    return;
}

void __KeyUpProc(void *pkey)
{
    __key_ctrl_t *pKey = (__key_ctrl_t *)pkey;
    
    if(pKey->keydown_cnt)
    {
        __SendKeyup(pKey->effective_value); //key up msg to upper layer   
    }
    pKey->pre_value   = KEY_DEFAULT_VALUE;
    pKey->keydown_cnt = 0;
    pKey->last_tick   = 0;
      
    return;
}

void __KeyProcess(void *pkey)
{
    __key_ctrl_t *pKey = (__key_ctrl_t *)pkey;
    __u32 scancode = 0;
   
    scancode = BSP_LRADC_ReadData(pKey->h_lradc);
    __msg("---------  scancode = %d  ---------\n",scancode);     
    pKey->crt_value = __GetKeyValue(scancode);    

    if((pKey->pre_value == pKey->crt_value) && (++pKey->last_tick < pKey->debounce_time) )
    {
       //__msg("last_tick = %d \n", pKey->last_tick);  
       return;//debounce filter
    }
   
    if( pKey->pre_value == pKey->crt_value)
    {
    	//eDbug("keydown: key_value is = %x !\n\n", pKey->crt_value);
        __KeyDownProc(pKey); //key press down        
    } 
   
    pKey->pre_value = pKey->crt_value;

    return;    
}

__s32 __IrqHandler(void *pArg)
{
    __key_ctrl_t *pKey = (__key_ctrl_t *)pArg;
    __u32 irq_no = 0;   
    
    irq_no = BSP_LRADC_QueryInt(pKey->h_lradc);    
	
    if(irq_no & LRADC_KEYDOWN_INT)
    {
        __inf("----------key down---------\n");              
        BSP_LRADC_ClearIntPending(pKey->h_lradc, LRADC_KEYDOWN_INT);   
        pKey->pre_value    = KEY_DEFAULT_VALUE;
        pKey->effective_value = KEY_DEFAULT_VALUE;
        pKey->keydown_cnt  = 0;
        pKey->last_tick    = 0; 
        pKey->debounce_time = pKey->first_key_time;
        
        return EPDK_OK;
    }	
    if(irq_no & LRADC_DATA_INT)
    {        
    	__inf("----------key data---------\n");       
        BSP_LRADC_ClearIntPending(pKey->h_lradc, LRADC_DATA_INT); 
        BSP_LRADC_DisableInt(pKey->h_lradc, LRADC_DATA_INT);                   
        __KeyProcess(pKey);//key process
        BSP_LRADC_EnableInt(pKey->h_lradc, LRADC_DATA_INT);    
        return EPDK_OK;
    }    
    if(irq_no & LRADC_KEYUP_INT)
    {         
        __inf("----------key up---------\n");                     
        BSP_LRADC_ClearIntPending(pKey->h_lradc, LRADC_KEYUP_INT);
        BSP_LRADC_DisableInt(pKey->h_lradc, LRADC_KEYUP_INT);        
        __KeyUpProc(pKey);
        BSP_LRADC_EnableInt(pKey->h_lradc, LRADC_KEYUP_INT);  
        return EPDK_OK;
   	}
    return EPDK_FAIL;   
}

__s32 KEY_Init(void)
{
    eLIBs_memset(&key_ctrl, 0, sizeof(__key_ctrl_t));
    key_ctrl.first_key_time = 2; // 32ms*2 = 64ms
    key_ctrl.second_key_time= 5; // 32ms*5 = 160ms
    key_ctrl.repeat_key_time= 3; // 32ms*3 = 96ms 
    esINT_InsISR(INTC_IRQNO_LRADC, (__pISR_t)__IrqHandler, &key_ctrl); 
    esINT_EnableINT(INTC_IRQNO_LRADC);
    
    key_ctrl.h_lradc = BSP_LRADC_Request(LRADC_CHANNEL_0);    
    BSP_LRADC_SetSampleRate(key_ctrl.h_lradc, LRADC_SAMPLE_RATE_63HZ);
    BSP_LRADC_KeyConfig(key_ctrl.h_lradc, LRADC_KEY_MODE_NORMAL, 0, LRADC_LEVELB_1_9V, 0);
    BSP_LRADC_ClearIntPending(key_ctrl.h_lradc, LRADC_ALL_INT);
    BSP_LRADC_EnableInt(key_ctrl.h_lradc, LRADC_ALL_INT);

	return EPDK_OK;
}

__s32 KEY_Exit(void)
{
    esINT_UniISR(INTC_IRQNO_LRADC, (__pISR_t)__IrqHandler);  
    esINT_DisableINT(INTC_IRQNO_LRADC);
    BSP_LRADC_Release(key_ctrl.h_lradc);

	return EPDK_OK;
}

