/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: msg_emit.c
* By      	: Andy.zhang
* Func		: system msg emit
* Version	: v1.0
* ============================================================================================================
* 2009-7-24 10:56:38 andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "mod_desktop_i.h"

#define 	SYS_MSGQ_SIZE			128

/**************************************************************************************************************/
typedef struct _msrv_msg_t
{
	__u32 type;	
	__u32 id;
	__u32 data;		
	__u32 reserved;
}__msrv_msg_t;

typedef struct _memit_ctr_t
{	
	__krnl_event_t	*p_array_sem;					// 缓冲区互斥
	__krnl_event_t	*p_hook_sem;
	__krnl_event_t  *psys_msg_queue;				// msg 消息队列
	int 			sys_msg_counter;
	__msrv_msg_t	sys_msg_array[SYS_MSGQ_SIZE];	// msg 消息队列缓冲	
	__u8 			ksrv_th_id;
	
	cb_key_hook 	key_hook;	
	cb_init_hook 	init_hook;

	__hdle  		h_keyGraber;
}__memit_ctr_t;

static __memit_ctr_t emit_ctr;
static __u8			 msg_srv_tid;

/**************************************************************************************************************/
static __msrv_msg_t  	*get_msg_buf(__memit_ctr_t *emit);
static __s32 			key_msg_cb(void *msg);
static void 			ksrv_msg_thread(void *arg);
/**************************************************************************************************************/
/**
 *  初始化 touch panel 消息传输通道
 */
__s32 msg_srv_init_tp_channel(__memit_ctr_t *emit)
{		
	 /* 创建 desktop_msg_queue */		
	emit->sys_msg_counter = 0;	
	emit->psys_msg_queue  = esKRNL_QCreate(SYS_MSGQ_SIZE);	
	if(!emit->psys_msg_queue)
	{
		__err(" create sys_msg_queue error \n");
		return EPDK_FAIL;
	}
		
	/* 创建 p_array_sem */	
	emit->p_array_sem = esKRNL_SemCreate(1);	
	if( !emit->p_array_sem )
	{
		__err(" create p_array_sem error \n");
		return EPDK_FAIL;
	}
	emit->p_hook_sem = esKRNL_SemCreate(1);
	if( !emit->p_hook_sem )
	{
		__err(" create p_hook_sem error \n");
		return EPDK_FAIL;
	}	
		
	return EPDK_OK;
}

__s32 msg_srv_init_key_channel(__memit_ctr_t *emit)
{		
    __s32  LdevID;
    
	/* grab logical keyboard, binding event callback to input_subsystem */
    emit_ctr.h_keyGraber = esINPUT_LdevGrab(INPUT_LKEYBOARD_DEV_NAME, (__pCBK_t)key_msg_cb, 0, 0);
    if (emit_ctr.h_keyGraber == NULL)
    {
        __msg("grab logical keyboard failed\n");
        return EPDK_FAIL;
    }

    __msg("emit_ctr.h_keyGraber=%d\n", emit_ctr.h_keyGraber);
    
     //get logical key device id
    LdevID = esINPUT_GetLdevID(emit_ctr.h_keyGraber);
    if (LdevID == -1)
    {
        eLIBs_printf("get logical id failed\n");
        return EPDK_FAIL;
    }
    __msg("LdevID=%d\n", LdevID);
    
    //设置时间间隔单位为1ms，
    if (esINPUT_LdevCtl(LdevID, INPUT_SET_REP_PERIOD,0x00280, NULL) != EPDK_OK)
    {
        eLIBs_printf("logical device ioctl failed\n");
        return EPDK_FAIL;
    }

    __here__;
	return EPDK_OK;
}

__s32 msg_srv_deinit_key_channel(__memit_ctr_t *emit)
{		
	esINPUT_LdevRelease(emit_ctr.h_keyGraber);
	
	return EPDK_OK;
}

/**
 *  释放 touch panel 消息传输通道
 */
__s32 msg_srv_deinit_tp_channel(__memit_ctr_t *emit)
{	
	__u8 err;	
	
	/* 删除 psys_msg_queue */	
	esKRNL_QDel(emit->psys_msg_queue, 0, &err);
	emit->psys_msg_queue = NULL;
	
	/* 删除 p_array_sem */	
	esKRNL_SemDel(emit->p_array_sem, 0, &err);
	esKRNL_SemDel(emit->p_hook_sem, 0, &err);
	emit->p_array_sem = NULL;
	emit->p_hook_sem = NULL;
	
	emit->sys_msg_counter = 0;
			
	return EPDK_OK;
}

/**
 * 初始化系统消息队列消息传输通道
 */
__s32 msg_srv_init_ksrv_channel(__memit_ctr_t *emit)
{		
	/* create ksrv msg thread  */
#ifdef SIM_PC_WIN
	__SAVE_MOD_EVN__
#endif
	emit->ksrv_th_id = esKRNL_TCreate(ksrv_msg_thread, (void *)emit, 0x400, KRNL_priolevel3);	
#ifdef SIM_PC_WIN
	__RESTORE_MOD_EVN__
#endif
	if( emit->ksrv_th_id == 0 )
	{
		__err(" ksrv_msg_thread create error \n");
		return EPDK_FAIL;
	}
	
	return EPDK_OK;	
}

/**
 * 释放系统消息队列消息传输通道
 */
__s32 msg_srv_deinit_ksrv_channel(__memit_ctr_t *emit)
{	
	while( esKRNL_TDelReq(emit->ksrv_th_id) != OS_TASK_NOT_EXIST )
	{
		esKRNL_TimeDly(1);
	}
	
	return EPDK_OK;
}

static __msrv_msg_t  *get_msg_buf(__memit_ctr_t *emit)
{
	//__msg("sys_msg_counter = %d \n", sys_msg_counter);	
	emit->sys_msg_counter = (emit->sys_msg_counter+1) &(SYS_MSGQ_SIZE-1);
	return &(emit->sys_msg_array[emit->sys_msg_counter]);
}

static __s32 key_msg_cb(void *msg)
{	
	__msrv_msg_t	*pmsg;
	__u8 error;
	
	__u32 i;
	__input_event_packet_t *pPacket;
    __input_event_t        *pEventFrame;				
	
    if (msg == NULL)
    {
        __msg("invalid argument for call back\n");
        return EPDK_FAIL;
    }

    
    //dump packet events
    pPacket = (__input_event_packet_t *)msg;
    if (pPacket->event_cnt > INPUT_MAX_EVENT_NR)
    {
        __msg("invalid event count number\n");
        return EPDK_FAIL;
    }
	for (i = 0; i < pPacket->event_cnt; i++)
	{
		pEventFrame = &(pPacket->events[i]);
		//__msg("Event %d: type = %d, code = %d, value = %d\n", i + 1,
              //                                                   pEventFrame->type,
              //                                                   pEventFrame->code,
              //                                                   pEventFrame->value
              //                                                   );
		if(pEventFrame->type == EV_KEY) 
		{            
			esKRNL_SemPend(emit_ctr.p_array_sem, 0, &error);
			pmsg = get_msg_buf(&emit_ctr);			
			esKRNL_SemPost(emit_ctr.p_array_sem);	
			
			
			switch(pEventFrame->code)
			{
				case KPAD_MENU:
				{
					pmsg->type 	= GUI_MSG_KEY;
					pmsg->id 	= GUI_MSG_KEY_MENU;
					__here__;
                   			 break;
				}
				case KPAD_UP:
			 	{
					pmsg->type 	= GUI_MSG_KEY;
			 		pmsg->id 	= GUI_MSG_KEY_UP;
					__here__;
					break;
				}
				case KPAD_DOWN:
			 	{
					pmsg->type 	= GUI_MSG_KEY;
			 		pmsg->id 	= GUI_MSG_KEY_DOWN;
					__here__;
					break;
				}
				case KPAD_LEFT:
			 	{
					pmsg->type 	= GUI_MSG_KEY;
			 		pmsg->id 	= GUI_MSG_KEY_LEFT;
					__here__;
					break;
				}
				case KPAD_RIGHT:
			 	{
					pmsg->type 	= GUI_MSG_KEY;
			 		pmsg->id 	= GUI_MSG_KEY_RIGHT;
					__here__;
					break;
				}					
				case KPAD_VOICEUP:
				{
					pmsg->type 	= GUI_MSG_KEY;
					pmsg->id 	= GUI_MSG_KEY_VADD;
					__here__;
					break;
				}
				case KPAD_VOICEDOWN:
				{
					pmsg->type 	= GUI_MSG_KEY;
					pmsg->id 	= GUI_MSG_KEY_VDEC;
					__here__;
					break;
				}
				case KPAD_ENTER:
			 	{
					pmsg->type 	= GUI_MSG_KEY;
			 		pmsg->id 	= GUI_MSG_KEY_ENTER;
					__here__;
					break;
				}
				case KPAD_RETURN:
			 	{
					pmsg->type 	= GUI_MSG_KEY;
			 		pmsg->id 	= GUI_MSG_KEY_ESCAPE;
					__here__;
					break;
				}
				case KPAD_POWER:
				{			
					pmsg->type 	= GUI_MSG_KEY;
					pmsg->id 	= GUI_MSG_KEY_POWER;
					__here__;
					break;
				}
				case KPAD_PHOTO:
				{
					pmsg->type 	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_PICTURE;
					__here__;
					break;
				}
				case KPAD_MUSIC:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_MUSIC;
					__here__;
					break;
				}
				case KPAD_MOVIE:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_MOVIE;
					__here__;
					break;
				}
				case KPAD_SETTING:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_SETTING;
					__here__;
					break;
				}
				case KPAD_FM:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_FM;
					__here__;
					break;
				}
				case KPAD_RECORD:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_RECORD;
					__here__;
					break;
				}
				case KPAD_MUTE:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_MUTE;
					__here__;
					break;
				}
				case KPAD_EQ:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_EQ;
					__here__;
					break;
				}
				case KPAD_POWEROFF:
				{
					pmsg->type 	= DSK_MSG_POWER_OFF;
					__here__;
					break;
				}
				case KPAD_MODE:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_MODE;
					__here__;
					break;
				}
				case KPAD_RR:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_RR;
					__here__;
					break;
				}
				case KPAD_FF:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_FF;
					__here__;
					break;
				}
				
				case KPAD_TV:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_TV;
					__here__;
					break;
				}

                case KPAD_EPG:
                {
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_EPG;
					__here__;
					break;
                }
                case KPAD_SCAN:
                {
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_SCAN;
					__here__;
					break;
                }
                case KPAD_LIST:
                {
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_LIST;
					__here__;
					break;
                }
                case KPAD_SUBTITLE:
                {
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_SUBTITLE;
					__here__;
					break;
                }
                
                
				case KPAD_DTVREC:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_DTVREC;
					__here__;
					break;
				}

                case KPAD_TVOUT:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_TVOUT;
					__here__;
					break;
				}
                    
 				case KPAD_AUDIO:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_AUDIO_CH;
					__here__;
					break;
				}

                case KPAD_CHANNEL_DEC:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_CH_DOWN;
					__here__;
					break;
				}

                case KPAD_CHANNEL_ADD:
                {
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_CH_UP;
					__here__;
					break;
				}

                case KPAD_TV_RIGHT:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_TV_RIGHT;
					__here__;
					break;
				}

                case KPAD_TV_LEFT:
                {
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_TV_LEFT;
					__here__;
					break;
				}
                
                case KPAD_INFO:
                {
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_INFO;
					__here__;
					break;
				}

				case KPAD_NUM0:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_NUM0;
					__here__;
					break;
				}
				case KPAD_NUM1:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_NUM1;
					__here__;
					break;
				}
				case KPAD_NUM2:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_NUM2;
					__here__;
					break;
				}
				case KPAD_NUM3:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_NUM3;
					__here__;
					break;
				}
				case KPAD_NUM4:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_NUM4;
					__here__;
					break;
				}
				case KPAD_NUM5:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_NUM5;
					__here__;
					break;
				}
				case KPAD_NUM6:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_NUM6;
					__here__;
					break;
				}
				case KPAD_NUM7:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_NUM7;
					__here__;
					break;
				}
				case KPAD_NUM8:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_NUM8;
					__here__;
					break;
				}
				case KPAD_NUM9:
				{
					pmsg->type	= GUI_MSG_KEY;
					pmsg->id	= GUI_MSG_KEY_NUM9;
					__here__;
					break;
				}
				
				default:
					break;
			}			
			pmsg->data 	= pEventFrame->value;
			//__msg("czp is add......power_off_id = %d\n", DSK_MSG_POWER_OFF);
			//__msg("GUI_MSG_KEY_ val=%d, pmsg->type=%d\n", pmsg->id, pmsg->type);

			esKRNL_QPost(emit_ctr.psys_msg_queue, pmsg);
		}		
	}	
	
	return EPDK_TRUE;
}

/** 
 * 从系统消息队列取消息
 */
static void ksrv_msg_thread(void *arg)
{
	__memit_ctr_t *emit = (__memit_ctr_t *)arg;
	__u8  error;
	
	/* 清空按键消息队列 */
	while(1)
	{
		__u32           usrmsg;		
		
		//usrmsg = esKSRV_GetLowMsg();
		usrmsg = esKSRV_GetMsg();
		if( usrmsg == 0 )
			break;
	}	
	
	while(1)
    {
        __u32           usrmsg;        
        __msrv_msg_t	*pmsg;   
        __u32 			tmp;     
  		 		  		
        while(1)
        {  	
           	if( esKRNL_TDelReq(OS_PRIO_SELF) == OS_TASK_DEL_REQ )
			{
				esKRNL_TDel(OS_PRIO_SELF);			
			}
           	
           	usrmsg = esKSRV_GetMsg();				// 系统消息队列
           	if(usrmsg)
            	break;
           	esKRNL_TimeDly(2);
        }
                
		esKRNL_SemPend(emit->p_array_sem, 0, &error);
		pmsg = get_msg_buf(emit);
		//eLIBs_memset(pmsg, 0, sizeof(__msrv_msg_t));
		esKRNL_SemPost(emit->p_array_sem);
		
		if( (usrmsg & 0xffff0000) == KMSG_USR_CLS_KEY) 	// key 按键消息
		{
			tmp = usrmsg & 0x0000ffff;
			__msg(" usrmsg = %d \n", tmp);
			switch (usrmsg) 
			{
					    
			#if( EPDK_CASE == EPDK_LIVE_BOX )
				case KMSG_USR_KEY_POWER:				/* standby	*/	
					 pmsg->type = DSK_MSG_STANDBY;
					 break;			
					 					                 	
	            case KMSG_USR_KEY_REPEATE:				/* 切换到色差输出 */
	            	pmsg->type = DSK_MSG_SWITCH_YPBPR;	            	
	            	break;            
	            
				case KMSG_USR_KEY_CLEAR:				/* 切换到cvbs输出 */
					pmsg->type = DSK_MSG_SWITCH_CVBS;
					break;					
					
				case KMSG_USR_KEY_DISPLAY:				/* 切换到hdmi输出 */
					pmsg->type = DSK_MSG_SWITCH_HDMI;
					break;
				
				case KMSG_USR_KEY_SHIFT:
					 pmsg->type = GUI_MSG_KEY;
					 pmsg->id	= GUI_MSG_KEY_SHIFT;					
					break;
				
				case KMSG_USR_KEY_SEL:	
					 pmsg->type = GUI_MSG_KEY;
					 pmsg->id	= GUI_MSG_KEY_SEL;		
					break;			
				
				case KMSG_USR_KEY_NUM4:
					pmsg->type = DSK_MSG_SWITCH_AUDIO_IF;
					pmsg->data = AUDIO_DEV_IF_CODEC;		
					break;
					 
				case KMSG_USR_KEY_NUM5:
					pmsg->type = DSK_MSG_SWITCH_AUDIO_IF;
					pmsg->data = AUDIO_DEV_IF_IIS;		
					break;
					 
				case KMSG_USR_KEY_NUM6:
					pmsg->type = DSK_MSG_SWITCH_AUDIO_IF;
					pmsg->data = AUDIO_DEV_IF_SPDIF;		
					break;					
					 
			#endif	
							
			#if( EPDK_CASE == EPDK_LIVE_TOUCH )
				case KMSG_USR_KEY_NUM8:					/* hold 按键 */
					 pmsg->type = DSK_MSG_HOLD;	    
	                 break; 
	            
	            case KMSG_USR_KEY_NUM7:								// 禁音功能
					pmsg->type = DSK_MSG_BAN_VOLUME;
					pmsg->data = 0;
					break;
	            
	        #endif 
	            	            
				case KMSG_USR_KEY_GOTO:
					pmsg->type = DSK_MSG_APP_EXIT;		/* 一键回主界面 */
					break;
			 
	            case KMSG_USR_KEY_POWEROFF:				// 关机消息			                 
	                 pmsg->type = DSK_MSG_POWER_OFF;
				     break;
				     	
				case KMSG_USR_KEY_NUM9:
					 pmsg->type = GUI_MSG_KEY;
					 pmsg->id	= GUI_MSG_KEY_ESCAPE;	
					 break;
				
			    case KMSG_USR_KEY_VOICE_UP:
					 pmsg->type = GUI_MSG_KEY;
					 pmsg->id	= GUI_MSG_KEY_VADD;				
				     break;
				     
			    case KMSG_USR_KEY_VOICE_DOWN:
					 pmsg->type    = GUI_MSG_KEY;
					 pmsg->id	   = GUI_MSG_KEY_VDEC;
	        	     break;
					
	            case KMSG_USR_KEY_UP:
					 pmsg->type    = GUI_MSG_KEY;
					 pmsg->id	   = GUI_MSG_KEY_UP;
	                 break;
					 
	        	case KMSG_USR_KEY_DOWN  :
					 pmsg->type    = GUI_MSG_KEY;
					 pmsg->id	   = GUI_MSG_KEY_DOWN;
	                 break;
	                 
	            case KMSG_USR_KEY_LEFT  : 
					 pmsg->type    	= GUI_MSG_KEY;
	                 pmsg->id	    = GUI_MSG_KEY_LEFT;
	                 break;
					 
	            case KMSG_USR_KEY_RIGHT :
					 pmsg->type    	= GUI_MSG_KEY;
					 pmsg->id	    = GUI_MSG_KEY_RIGHT;
					 break;
	            
				case KMSG_USR_KEY_ENTER:
					 pmsg->type    	= GUI_MSG_KEY;
	                 pmsg->id	    = GUI_MSG_KEY_ENTER;
	                break;                               		
	
			    case KMSG_USR_KEY_RISE:
					pmsg->type    	= GUI_MSG_KEY;
				    pmsg->id	    = GUI_MSG_KEY_RISE;
				    break;
				
				default:
		        	continue;
			}
		}
		else	/* system 消息 */
		{
			if( (usrmsg & 0x0000ffff) == KMSG_USR_SYSTEM_FS_PLUGIN )
			{
				pmsg->type = DSK_MSG_FS_PART_PLUGIN;
				pmsg->data = (usrmsg & KMSG_USR_SYSTEM_FS_PARA_MASK)>>16;				
			}
			else if( (usrmsg & 0x0000ffff) == KMSG_USR_SYSTEM_FS_PLUGOUT )
			{
				pmsg->type = DSK_MSG_FS_PART_PLUGOUT;
				pmsg->data = (usrmsg & KMSG_USR_SYSTEM_FS_PARA_MASK)>>16;
			}
			else
			{
				//__msg("*************usrmsg = %d***************\n", usrmsg);
				switch (usrmsg) 
        		{   
					case KMSG_USR_SYSTEM_USBD_PLUGIN:		// usb device plug in						 
					     __msg("case KMSG_USR_SYSTEM_USBD_PLUGIN\n");
						 pmsg->type = DSK_MSG_USBD_PLUG_IN;				
		                 break;
		                 
		            case KMSG_USR_SYSTEM_USBD_PLUGOUT:    	// usb device plug out		            	 
		    		     pmsg->type = DSK_MSG_USBD_PLUG_OUT;    		    
                         __msg("case KMSG_USR_SYSTEM_USBD_PLUGOUT\n");
		                 break; 
						 	    		
			        case KMSG_USR_SYSTEM_SDMMC_PLUGOUT:	
			        	 pmsg->type = DSK_MSG_APP_EXIT;
			        	 break;
		
					case KMSG_USR_SYSTEM_USBH_PLUGOUT:		// usb host 设备拔出
			             pmsg->type = DSK_MSG_APP_EXIT;	             
		                 break;    		 
					
					case KMSG_USR_SYSTEM_TVDAC_PLUGIN: 						 
                        __msg("**********KMSG_USR_SYSTEM_TVDAC_PLUGIN***********\n");
		    		     pmsg->type = DSK_MSG_TVDAC_PLUGIN;
		                 break;  
		
					case KMSG_USR_SYSTEM_TVDAC_PLUGOUT:	
                        __msg("***********KMSG_USR_SYSTEM_TVDAC_PLUGOUT***************\n");
		    		     pmsg->type = DSK_MSG_TVDAC_PLUGOUT;
		                 break; 
		
					case KMSG_USR_SYSTEM_HDMI_PLUGIN: 
                        __msg("**********KMSG_USR_SYSTEM_HDMI_PLUGIN***********\n");
		    		     pmsg->type = DSK_MSG_HDMI_PLUGIN;
		                 break;  
					
					case KMSG_USR_SYSTEM_HDMI_PLUGOUT:
                        __msg("***********KMSG_USR_SYSTEM_HDMI_PLUGOUT***************\n");
		    		     pmsg->type = DSK_MSG_HDMI_PLUGOUT;
		                 break; 
					
					#if( EPDK_CASE == EPDK_LIVE_TOUCH )
					case KMSG_USR_SYSTEM_WAKEUP:
	            		 __here__
	            	 	pmsg->type = DSK_MSG_STANDBY_WAKE_UP;
	            	 break;
					#endif
					
		        	default:
		        	    continue;
        		}
			}				
		}
        	
		esKRNL_QPost(emit->psys_msg_queue, pmsg);	
    }	 
}

/**
 * 
 */
__s32 msg_srv_get_message(__memit_ctr_t *emit, __msrv_msg_t *p_msg)
{
	__u8 error;
	__msrv_msg_t *tmp;
		
	tmp = (__msrv_msg_t *)esKRNL_QAccept( emit->psys_msg_queue, &error);
	
	if( tmp!= NULL)
	{
		memcpy( p_msg, tmp, sizeof(__msrv_msg_t) );	
		return EPDK_OK;	
	}
	else
	{
		return EPDK_FAIL;
	}
	
}

/**
 * 发送消息到桌面系统消息队列
 */
__s32 msg_srv_dispatch_message(__memit_ctr_t *emit, __msrv_msg_t *p_msg)
{
	__gui_msg_t     msg;
	__u8			err;
		
	if( p_msg->type == GUI_MSG_KEY )
	{		
		msg.id 			= p_msg->type;
		msg.h_srcwin 	= NULL;	
		msg.h_deswin	= GUI_GetActiveManWin();
		msg.dwAddData1	= p_msg->id;
		msg.dwAddData2	= p_msg->data;
		msg.p_arg		= NULL;
		
		esKRNL_SemPend(emit->p_hook_sem, 0, &err);			
		if( emit->key_hook )		
		{			
			(emit->key_hook)(&msg);
		}			
		esKRNL_SemPost(emit->p_hook_sem);			
	}
	else if ( (p_msg->type >= DSK_MSG_START)&&( p_msg->type <= DSK_MSG_LAST))
	{		
		msg.id 			= p_msg->type; 
		msg.h_srcwin 	= NULL;
		msg.h_deswin 	= GUI_GetActiveManWin();
		msg.dwAddData1	= 0;
		msg.dwAddData2	= p_msg->data;
		msg.p_arg		= NULL;		
		
		esKRNL_SemPend(emit->p_hook_sem, 0, &err);	
		if( emit->init_hook )
		{			
			(emit->init_hook)(&msg);
		}	
		esKRNL_SemPost(emit->p_hook_sem);			
	}
	else
	{
		__wrn(" msg.id = %d is error \n", p_msg->type);
		return EPDK_FAIL;
	}
		
	return EPDK_OK;
}


/**
 * msg srv thread
 */
void msg_srv_thread(void *arg)
{
	__msrv_msg_t msg;	
	
	__inf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
	__inf("&            message server started !!!   	           &\n");
	__inf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
	
	/* 初始化消息通道 */	
	memset(&emit_ctr, 0, sizeof(__memit_ctr_t));
		
	msg_srv_init_tp_channel(&emit_ctr);	
    //__wait__;
	msg_srv_init_key_channel(&emit_ctr);
	msg_srv_init_ksrv_channel(&emit_ctr);		
	
	/* 消息分发 */
	while(1)
	{
		if( esKRNL_TDelReq(OS_PRIO_SELF) == OS_TASK_DEL_REQ )
		{
			/* 释放消息通道 */	
			msg_srv_deinit_key_channel(&emit_ctr);
			msg_srv_deinit_tp_channel(&emit_ctr);			
			msg_srv_deinit_ksrv_channel(&emit_ctr);
			
				
			__inf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
			__inf("&            message server exited !!!                  &\n");
			__inf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
			
			esKRNL_TDel(OS_PRIO_SELF);			
			return;
		}
		
		memset( &msg, 0, sizeof(__msrv_msg_t) );		
				
		if( EPDK_OK ==  msg_srv_get_message(&emit_ctr, &msg) )	
		{		
			msg_srv_dispatch_message(&emit_ctr, &msg);
		}
		else	
		{
			esKRNL_TimeDly(1);
		}
	}	
}


__s32 msg_emit_init(void)
{
	/* 创建消息收集线程 */	
#ifdef SIM_PC_WIN
	__SAVE_MOD_EVN__
#endif
	msg_srv_tid = esKRNL_TCreate( msg_srv_thread, NULL, 0x800, KRNL_priolevel3 );
#ifdef SIM_PC_WIN
	__RESTORE_MOD_EVN__
#endif
	if( msg_srv_tid!= 0 )
		return EPDK_OK;
	else
		return EPDK_FAIL;
	
}

__s32 msg_emit_deinit(void)
{		
	/* 删除消息采集线程 */
	while( esKRNL_TDelReq(msg_srv_tid) != OS_TASK_NOT_EXIST )
	{		
		esKRNL_TimeDly(1);		
	}
	
	msg_srv_tid = 0;
	
	return EPDK_OK;
}

__s32 msg_emit_register_hook(__memit_hook_t *hook)
{
	__u8 err;
	
	esKRNL_SemPend(emit_ctr.p_hook_sem, 0, &err);
	
	emit_ctr.key_hook = hook->key_hook;
	emit_ctr.init_hook  = hook->init_hook;
	
	esKRNL_SemPost(emit_ctr.p_hook_sem);
	
	return EPDK_OK;
}

