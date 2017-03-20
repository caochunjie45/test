/*
**************************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           LARK app sample
*
*						        	(c) Copyright 2006-2007, TERRY, China
*											 All Rights Reserved
*
* File    : dsk_radio.c
* By      : terry
* Version : V1.00
* time    : 2009-12-01
**************************************************************************************************************
*/

#include "..\\dsk_radio_i.h"
#include "list_file.h"

__pCBK_t volatile  cb_search_success  = NULL;
void   * volatile  cb_ctx_search_success  = NULL;

__pCBK_t volatile  cb_search_fail  = NULL;
void   * volatile  cb_ctx_search_fail  = NULL;

__pCBK_t volatile  cb_search_over  = NULL;
void   * volatile  cb_ctx_search_over  = NULL;



#define MAX_FM1_3_AM1_2			5
#define MAX_FM1_3_AM1_2_FREQ	6

typedef struct
{
	//ES_FILE 			*fm_drv;
	__u8 				thread_id;
	__u32				cur_freq;
	__u32         			rec_search_mode;
	__u32				manual_search_way;										//add by Kingvan
	__u32				auto_maual_mode;
	__u32				search_flag;/*1,searching,2,search over*/	
	__u32 				start_freq;
	__u32				manual_start_freq;
	__u32				manual_cur_channum;
	__play_list_type_e		play_list_type;	
	dsk_radio_ch_list_t 	search_result;
	__u32 				FM1_3_AM1_2_freq[MAX_FM1_3_AM1_2][MAX_FM1_3_AM1_2_FREQ];//add by Kingvan
	__u16				curFM1_3AM1_2_id;										//add by Kingvan
	dsk_radio_ch_list_t 	favorite;
	__u32				manual_seach_cur_freq;	//by cky  �ֶ��������ĵ�ǰƵ��	
}dsk_radio_rcv_t;

static dsk_radio_rcv_t *dsk_radio_rcv = NULL;

void __autosearch_thread(void *p_arg)
{
	dsk_radio_rcv_t *dsk_radio_rcv;	
	__s32 result;	
	__s32 channel_no=0;
	__u32 radio_freq;
	
	dsk_radio_rcv = (dsk_radio_rcv_t *)p_arg;
	
	while(1)
	{
		if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
        {     
            esKRNL_TDel(EXEC_prioself);
            break;
        }
        if(dsk_radio_rcv->search_flag == 0x01)
        {
        	dsk_radio_rcv->cur_freq = dsk_radio_rcv->start_freq;		
		
		if(dsk_radio_rcv->auto_maual_mode == DSK_RADIO_SEARCH_MANUAL)
		{
		        radio_freq = dsk_radio_rcv->start_freq+LARK_SEARCH_STEP_US;
				__wrn(" DSK:radio_freq = %d\n",radio_freq);
			if( dsk_radio_rcv->manual_search_way != 0 )//add by Kingvan  //���ķ�������
			{				
				__s32 curfreq = 0;
				curfreq = dsk_radio_rcv->start_freq-LARK_SEARCH_STEP_US;
				__wrn(" DEC: curfreq = %d\n",curfreq);
				__wrn(" DEC: dsk_radio_rcv->manual_cur_channum = %d\n",dsk_radio_rcv->manual_cur_channum);
				__wrn(" DEC: curfreq <= dsk_radio_rcv->manual_start_freq = %d\n",curfreq <= dsk_radio_rcv->manual_start_freq);
				if( (dsk_radio_rcv->manual_cur_channum&0xf0000000)
					&&(curfreq <= dsk_radio_rcv->manual_start_freq) )
				{
					__wrn(" DEC: (dsk_radio_rcv->manual_cur_channum * 100)|0xff000000 = %d\n",(dsk_radio_rcv->manual_cur_channum * 100)|0xff000000);
					dsk_radio_rcv->manual_search_way = 0;
					result = -1;					
				}
				else if(curfreq < h_radio->freq_range.fm_area_min_freq)
				{					
					curfreq = h_radio->freq_range.fm_area_max_freq;					
					dsk_radio_rcv->manual_cur_channum |= 0xf0000000;					
					result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_SEARCH, DRV_FM_MANUAL_SEARCH|DRV_FM_SEARCH_DOWN, (void *)(curfreq));					
					__wrn(" DSK:result = %d\n",result);
				}
				else
				{	
					result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_SEARCH, DRV_FM_MANUAL_SEARCH|DRV_FM_SEARCH_DOWN, (void *)(curfreq));					
					__wrn(" DSK:result = %d\n",result);
				}
			}
			else//��С�ķ�������
			{
				if((dsk_radio_rcv->manual_cur_channum&0xf0000000)
					&&(radio_freq >= dsk_radio_rcv->manual_start_freq))
				{
					__wrn(" ADD: (dsk_radio_rcv->manual_cur_channum* 100)|0xff000000 = %d\n",(dsk_radio_rcv->manual_cur_channum * 100)|0xff000000);
					result = -1;
				}
				else if(radio_freq > h_radio->freq_range.fm_area_max_freq)//FM5807
				{
					__here__;
					radio_freq = h_radio->freq_range.fm_area_min_freq;
					dsk_radio_rcv->manual_cur_channum |= 0xf0000000;
					result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_SEARCH, DRV_FM_MANUAL_SEARCH|DRV_FM_SEARCH_UP, (void *)(radio_freq));					
					__wrn(" DSK:result = %d\n",result);
				}
				else	//FM4702
				{
					__here__;
					result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_SEARCH, DRV_FM_MANUAL_SEARCH|DRV_FM_SEARCH_UP, (void *)(radio_freq));
					__wrn(" DSK:result = %d\n",result);
				}
			}
		}
		else		//dsk_radio_rcv->auto_maual_mode == DSK_RADIO_SEARCH_AUTO
		{	
		        radio_freq = dsk_radio_rcv->start_freq;
			__wrn(" DSK auto search:radio_freq = %d\n",radio_freq);
			result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_SEARCH, DRV_FM_AUTO_SEARCH|DRV_FM_SEARCH_UP, (void *)(radio_freq));
			__wrn(" DSK:result = %d\n",result);
		}
        	
	        if(result == -1)
	        {
				__here__;
	        	dsk_radio_rcv->search_flag = 0x02;
	        }
	        else if(result&0xff000000)//search fail~ֻ��FM5807ģ��,����ʧ�ܣ���������
	        {
			__here__;
			__wrn("result&0xff000000 = %d\n",result&0xff000000);
			dsk_radio_rcv->cur_freq = (result&0x00ffffff);
			__wrn("result&0x00ffffff = %d\n",result&0x00ffffff);
			
			dsk_radio_rcv->manual_seach_cur_freq = dsk_radio_rcv->start_freq;//by cky add
			
			if (dsk_radio_rcv->auto_maual_mode == DSK_RADIO_SEARCH_MANUAL)
			{
				dsk_radio_rcv->start_freq = dsk_radio_rcv->cur_freq;
			}
			else//DSK_RADIO_SEARCH_AUTO
			{	
				dsk_radio_rcv->start_freq = dsk_radio_rcv->cur_freq+100;
			}
			
			__wrn(" dsk_radio_rcv->cur_freq = %d\n",dsk_radio_rcv->cur_freq);
			if(cb_search_fail != NULL)
            		{
            			esKRNL_CallBack(cb_search_fail, cb_ctx_search_fail);
			}
	        }
		else  //result == cur_freq
	        {	        	
	        	__here__;
			if(dsk_radio_rcv->auto_maual_mode == DSK_RADIO_SEARCH_MANUAL)
			{
				
					__u32 i = 0;
					__u32 isvalidstop = 1;
					__wrn(" earch_result.valid_num = %d\n",dsk_radio_rcv->search_result.valid_num);
					for(i=0;i<dsk_radio_rcv->search_result.valid_num;i++)//���Ҹ�Ƶ�����б����Ƿ��Ѵ��ڣ�����������������
					{
						if(dsk_radio_rcv->search_result.freq_list[i].freq == result)
						{
						__wrn(" search_result.freq_list[i].freq  = %d\n",dsk_radio_rcv->search_result.freq_list[i].freq );
							isvalidstop = 0;
						}
					}
					dsk_radio_rcv->cur_freq = result;
					dsk_radio_rcv->manual_seach_cur_freq = result;//by cky add
					__wrn(" cur_freq = %d\n",dsk_radio_rcv->cur_freq);
					__wrn(" manual_seach_cur_freq = %d\n",dsk_radio_rcv->manual_seach_cur_freq);		
					//dsk_radio_rcv->start_freq = result;					
					//if(isvalidstop == 1)//�������Ч�ĵ�̨
					//{							
						//dsk_radio_rcv->search_result.freq_list[dsk_radio_rcv->manual_cur_channum&0x0fffffff-1].freq = result;																
					//}
					//else//��Ƶ�����źţ������б����Ѵ���
					//{
						if(cb_search_success != NULL)
						{
							esKRNL_CallBack(cb_search_success, cb_ctx_search_success);
						}				
					//}
					dsk_radio_rcv->search_flag = 0x02;	//by cky ������һ��Ƶ�������Ƿ�
			}
			else  //dsk_radio_rcv->auto_maual_mode == DSK_RADIO_SEARCH_AUTO
			{
		            	dsk_radio_rcv->search_result.freq_list[channel_no].freq = result;
		            	dsk_radio_rcv->cur_freq = result;
			       	channel_no++;
			        dsk_radio_rcv->search_result.valid_num++;			            
			        dsk_radio_rcv->start_freq = result+LARK_SEARCH_STEP_US;		
				__wrn(" DSK:SEARCH_AUTO:result  = %d\n",result );
			   	if(cb_search_success != NULL)
		            	{
		            		esKRNL_CallBack(cb_search_success, cb_ctx_search_success);
			    	}		                  

#if	BEETLES_FM_SEARCH_PREVIEW
					//��̨�ɹ���, Ԥ������; Ethan;
					{
						dsk_speaker_onoff(BEETLES_SPEAK_ON);
						result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_PLAY, 0, (void *)(dsk_radio_rcv->cur_freq));			
						esKRNL_TimeDly(200);
					}
#endif
					
	            		if(dsk_radio_rcv->search_result.valid_num >= MAX_CHANNEL_NUM )
	            		{
					__here__;
	            			dsk_radio_rcv->search_flag = 0x02;
	            		}
			}
	        }
        }
        if(dsk_radio_rcv->search_flag == 0x02)//search over
        {
			if(dsk_radio_rcv->auto_maual_mode == DSK_RADIO_SEARCH_MANUAL)//�ֶ�����
			{
				dsk_radio_rcv->manual_cur_channum &= 0x0fffffff;
				__wrn(" DSK:search over:dsk_radio_rcv->cur_freq  = %d\n",dsk_radio_rcv->cur_freq );
				result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_PLAY, 0, (void *)(dsk_radio_rcv->cur_freq));			
				__wrn(" DSK:search over:result  = %d\n",result );
			}
			else//�Զ�����
			{
	        		if(dsk_radio_rcv->search_result.valid_num>0)
			    	{
			    	__here__;
					__wrn(" DSK:search over:dsk_radio_rcv->search_result.valid_num = %d\n",dsk_radio_rcv->search_result.valid_num );
			        result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_PLAY, 0, (void *)(dsk_radio_rcv->search_result.freq_list[0].freq));			        
				__wrn(" DSK:search over:result  = %d\n",result );
				dsk_radio_rcv->cur_freq = dsk_radio_rcv->search_result.freq_list[0].freq;
				__wrn(" dsk_radio_rcv->cur_freq = %d\n",dsk_radio_rcv->cur_freq);
			        channel_no = 0;
			    	}
			    /*���򲥷���СƵ��*/
				else//�Զ�����û������̨
				 {
				    	__here__;
				        result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_PLAY, 0, (void *)dsk_radio_rcv->start_freq);			        
				        dsk_radio_rcv->cur_freq = dsk_radio_rcv->start_freq;
					__wrn(" dsk_radio_rcv->cur_freq = %d\n",dsk_radio_rcv->cur_freq);
				 }
	        	}		   
		    	dsk_radio_rcv->search_flag = 0x00;
			dsk_radio_rcv->auto_maual_mode = 0x00;
			__wrn(" cb_search_over555=0x%x\n", cb_search_over);
			if(cb_search_over != NULL)
			{
                		__here__;
				esKRNL_CallBack(cb_search_over, cb_ctx_search_over);
			}
        }
        esKRNL_TimeDly(4);
        //esKRNL_TimeDly(500);
	}
}

__s32 dsk_radio_rcv_open(void)
{	
	if(h_radio == NULL)
	{
		__msg(" radio not open!\n");
		return EPDK_FAIL;
	}
	
	if(dsk_radio_rcv)
	{		
		return EPDK_OK;
	}
	
	dsk_radio_rcv = (dsk_radio_rcv_t *)esMEMS_Malloc(0,sizeof(dsk_radio_rcv_t));
	if(dsk_radio_rcv == NULL)
	{
		__msg("dsk_radio_rcv malloc error!\n");
		return EPDK_FAIL;
	}
	eLIBs_memset(dsk_radio_rcv, 0, sizeof(dsk_radio_rcv_t));
	//h_radio->fm_drv = fm_drv;
	
	eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_INIT, 0, 0);
	
	//dsk_radio_rcv->thread_id = esKRNL_TCreate(__autosearch_thread, (void *)dsk_radio_rcv, 0x400,KRNL_priolevel3);

    
    //changed by libaiao, for auto search while display freq,   2011.10.28
    dsk_radio_rcv->thread_id = esKRNL_TCreate(__autosearch_thread, (void *)dsk_radio_rcv, 0x400,KRNL_priolevel3);
	return EPDK_OK;
}

__bool dsk_radio_rcv_is_open(void)
{
	if(dsk_radio_rcv)
	{
		return EPDK_TRUE;
	}
	else
	{
		return EPDK_FALSE;
	}
}

__s32 dsk_radio_rcv_set_freq_play(__s32 freq)
{
	__s32 result = 0;
	__drv_fm_area_freq_t freq_range = h_radio->freq_range;

    __wrn("freq=%d\n", freq);
    
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	if(dsk_radio_rcv->search_flag == 0x01 || dsk_radio_rcv->search_flag == 0x02)
	{
		return -1;
	}
	/*���������ڲ��ζ�Ӧ�����Ƶ��ʱ,Ƶ����Ϊ���ε����Ƶ��*/
    if(freq < freq_range.fm_area_min_freq )
    {
        freq = freq_range.fm_area_min_freq;
    }
    /*������С�ڲ��ζ�Ӧ����СƵ��ʱ,Ƶ����Ϊ���ε���СƵ��*/
    else if(freq > freq_range.fm_area_max_freq)
    {
        freq = freq_range.fm_area_max_freq;
    }
    result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_PLAY, 0, (void *)freq);
    __wrn("result=%d\n", result);
    if(result == 0)
    {
    	dsk_radio_rcv->cur_freq = freq;
    }

    __wrn("dsk_radio_rcv->cur_freq=%d\n", dsk_radio_rcv->cur_freq);
	return result;
}

__s32 dsk_radio_rcv_set_search_cb(__pCBK_t cb, void* ctx)
{
    __s32 ret;
    __u32 arg[3] = {0,0,0};
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
    arg[0] = (__u32)cb;
    arg[1] = (__u32)ctx;
    //__msg("\narg[0]=%x, arg[1] = %x\n", cb, ctx);
	ret = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_SET_SEARCH_CB,0,(void *)arg);

    return ret;
}


__s32 dsk_radio_rcv_auto_search(void)
{
	__s32 result;
	//dsk_radio_rcv_t *dsk_radio_rcv = (dsk_radio_rcv_t *)handle;
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	if(dsk_radio_rcv->rec_search_mode == DRV_FM_SEARCH_UP)
	{
		result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_SEARCH, 
					DRV_FM_AUTO_SEARCH|dsk_radio_rcv->rec_search_mode, (void *)(dsk_radio_rcv->cur_freq+LARK_SEARCH_STEP_US));
		__msg("MSG%d%s:result = %d\n",__LINE__,__FILE__,result);
	}
    else//dsk_radio_rcv->rec_search_mode == DRV_FM_SEARCH_DOWN
	{
		result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_SEARCH, 
						DRV_FM_AUTO_SEARCH|dsk_radio_rcv->rec_search_mode, (void *)(dsk_radio_rcv->cur_freq-LARK_SEARCH_STEP_US));
		__msg("MSG%d%s:result = %d\n",__LINE__,__FILE__,result);
	}

    if(result == -1)
    {
        __wrn(" Error:find a channel from fm driver failed \n ");
    }
    else
    {
    	dsk_radio_rcv->cur_freq = result;
    }
    //eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_MUTE, DRV_FM_VOICE_ON, 0);
    
    return result;
}

__s32 dsk_radio_rcv_get_cur_freq(__u32 *freq)
{
	__u32 *inner_freq;
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	inner_freq = freq;
	if(dsk_radio_rcv->auto_maual_mode == DSK_RADIO_SEARCH_MANUAL)
	{
		*inner_freq = dsk_radio_rcv->manual_seach_cur_freq;//by cky
		__wrn("dsk_radio_rcv->manual_seach_cur_freq = %d\n",dsk_radio_rcv->manual_seach_cur_freq);
	}
	else
	{
		*inner_freq = dsk_radio_rcv->cur_freq;
		__wrn("draw:dsk_radio_rcv->cur_freq = %d\n",dsk_radio_rcv->cur_freq);
	}
	return 0;
}

__s32 dsk_radio_rcv_set_search_mode(__u32 search_mode)
{
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	if(search_mode == DSK_RADIO_REC_SEARCH_UP)
    {
        dsk_radio_rcv->rec_search_mode = DRV_FM_SEARCH_UP;
    }
    else//search_mode == DSK_RADIO_REC_SEARCH_DOWN
    {
        dsk_radio_rcv->rec_search_mode = DRV_FM_SEARCH_DOWN;
    }
    
    return 0;	
}

__s32 dsk_radio_rcv_search_stop(void)										//add by Kingvan
{
	if( dsk_radio_rcv->search_flag == 0x01 )
		dsk_radio_rcv->search_flag = 0x02;
    return 0;
}

__u16 dek_radio_set_FM1_3_ID( __s16 fm1_3_id )								//add by Kingvan
{
	if( fm1_3_id > 0 )
	{
		dsk_radio_rcv->curFM1_3AM1_2_id = (dsk_radio_rcv->curFM1_3AM1_2_id+1)%3;
	}
	else if( fm1_3_id < 0 )
	{
		dsk_radio_rcv->curFM1_3AM1_2_id = (dsk_radio_rcv->curFM1_3AM1_2_id+3-1)%3;
	}


	return dsk_radio_rcv->curFM1_3AM1_2_id;
}

__s32 dek_radio_set_FM1_3( __u16 fm1_3_id, __u16 fm_index, __u32 freq )		//add by Kingvan
{
	if( freq < h_radio->freq_range.fm_area_min_freq || freq > h_radio->freq_range.fm_area_max_freq )
		return -1;
	dsk_radio_rcv->FM1_3_AM1_2_freq[fm1_3_id][fm_index] = freq;
	return 0;
}

__u32 dek_radio_get_FM1_3( __u16 fm1_3_id, __u16 fm_index )					//add by Kingvan
{
	__u32 freq = 0;

	freq = dsk_radio_rcv->FM1_3_AM1_2_freq[fm1_3_id][fm_index];

	return freq;
}

__s32 dsk_radio_get_search_flag(__u32 *search_flag)						//add by cky
{
	__u32 *inner_search_flag;
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	inner_search_flag = search_flag;
	*inner_search_flag = dsk_radio_rcv->search_flag ;
    return 0;
}

__s32 dsk_radio_set_search_flag(__u32 search_flag)						//add by cky
{
	
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}	
	search_flag = dsk_radio_rcv->search_flag ;
    return 0;
}

__s32 dsk_radio_set_manual_way(__u32 manual_way)						//add by Kingvan
{
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}

    dsk_radio_rcv->manual_search_way = manual_way;
    return 0;
}

__s32 dsk_radio_set_automanual_mode(__u32 search_mode)
{
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}	
	dsk_radio_rcv->auto_maual_mode = search_mode;	
	return 0;	
}


__s32 dsk_radio_rcv_search_all(__u32 freq_cur,__s32 channel_cur)
{
	__u32 i;
	__u32 start_freq = 0;

	if(dsk_radio_rcv->auto_maual_mode == DSK_RADIO_SEARCH_MANUAL)
	{
		//eLIBs_printf("DSK_search freq_cur = %d\n",freq_cur);
		start_freq = freq_cur;
	}
	else
	{
		start_freq = h_radio->freq_range.fm_area_min_freq;
	}
	
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	//__msg("DSK_radio_rcv->search_flag = %d\n",dsk_radio_rcv->search_flag);
	
	if(dsk_radio_rcv->search_flag)
		return 0;
	if(dsk_radio_rcv->auto_maual_mode == DSK_RADIO_SEARCH_MANUAL)
	{
		dsk_radio_rcv->manual_start_freq = start_freq;
		dsk_radio_rcv->manual_cur_channum = channel_cur;
		__msg("dsk_radio_rcv->manual_cur_channum = %d\n",dsk_radio_rcv->manual_cur_channum);
	}
	else
	{
		dsk_radio_rcv->search_result.valid_num = 0;
		for(i=0;i<MAX_CHANNEL_NUM;i++)
		{
			//eLIBs_memset(dsk_radio_rcv->search_result.freq_list[i].name, 0, CHANNEL_NAME_LEN);
			dsk_radio_rcv->search_result.freq_list[i].freq = 0;
		}
	}
	dsk_radio_rcv->start_freq = start_freq;
	dsk_radio_rcv->search_flag = 0x01;
	return 0;
}


__s32 dsk_radio_rcv_next_freq_play(void)
{
	
	__u32 freq;
	__s32 result;
	__s32 cur_band;
	__drv_fm_area_freq_t freq_range;
	
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	cur_band	= h_radio->cur_band;
	freq_range 	= h_radio->freq_range;
	
	if(dsk_radio_rcv->search_flag == 0x01 || dsk_radio_rcv->search_flag == 0x02)
	{
		return -1;
	}
	/*������Ϊ�ձ�����ʱ,Ƶ�ʵ�����Ϊ50KHZ*/
    if(cur_band == DSK_RADIO_BAND_JAPAN)
    {
        freq = dsk_radio_rcv->cur_freq + LARK_SEARCH_STEP_JAPAN;         
    }
    /*������Ϊ��������ʱ,Ƶ�ʵ�����Ϊ100KHZ*/
    else 
    {
        freq = dsk_radio_rcv->cur_freq + LARK_SEARCH_STEP_US;
    }
    /*����һƵ�ʵ�ֵ���ڲ��ζ�Ӧ�����Ƶ��ʱ,ѭ�����ص���СƵ�ʴ�*/
    if(freq > freq_range.fm_area_max_freq)
    {
        freq = freq_range.fm_area_min_freq;
    }

    dsk_radio_rcv->cur_freq = freq;
    result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_PLAY, 0, (void *)freq);
	return result;
}

__s32 dsk_radio_rcv_pre_freq_play(void)
{
	
	__u32 freq;
	__s32 result;
	__s32 cur_band;
	__drv_fm_area_freq_t freq_range;
	
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	cur_band	= h_radio->cur_band;
	freq_range 	= h_radio->freq_range;
	
	if(dsk_radio_rcv->search_flag == 0x01 || dsk_radio_rcv->search_flag == 0x02)
	{
		return -1;
	}
	/*������Ϊ�ձ�����ʱ,Ƶ�ʵ�����Ϊ50KHZ*/
    if(cur_band == DSK_RADIO_BAND_JAPAN)
    {
        freq = dsk_radio_rcv->cur_freq - LARK_SEARCH_STEP_JAPAN;         
    }
    /*������Ϊ��������ʱ,Ƶ�ʵ�����Ϊ100KHZ*/
    else 
    {
        freq = dsk_radio_rcv->cur_freq - LARK_SEARCH_STEP_US;
    }
    /*����һƵ�ʵ�ֵ���ڲ��ζ�Ӧ�����Ƶ��ʱ,ѭ�����ص���СƵ�ʴ�*/
    if(freq < freq_range.fm_area_min_freq)
    {
        freq = freq_range.fm_area_max_freq;
    }
    dsk_radio_rcv->cur_freq = freq;

    result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_PLAY, 0, (void *)freq);
	return result;
}

__s32 dsk_radio_rcv_set_play_list_type(__play_list_type_e play_list_type )
{
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}

	dsk_radio_rcv->play_list_type = play_list_type;	

	return 0;
}

__s32 dsk_radio_rcv_get_play_list_type(__s32 *play_list_type)
{
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}

	*play_list_type = dsk_radio_rcv->play_list_type;
	
	return 0;
}

__s32 dsk_radio_rcv_get_search_result_vn(void)
{
	__s32 valid_num;
	
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	__wrn("valid_num=%d\n", dsk_radio_rcv->search_result.valid_num);
	valid_num = dsk_radio_rcv->search_result.valid_num;
	
	return valid_num;
}

__s32 dsk_radio_rcv_get_favorite_vn(__s32 *valid_num)
{
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	*valid_num = dsk_radio_rcv->favorite.valid_num;
	
	return 0;
}

__s32 dsk_radio_rcv_get_search_result_chaninfo(__s32 id, dsk_radio_chan_t *chan_info)
{
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	//�ֶ���������Ƶ�������޸�Ϊ�������ˣ��������²���Ҫ��
	//if(dsk_radio_rcv->auto_maual_mode == DSK_RADIO_SEARCH_MANUAL)//by cky add 
	//{
	//	chan_info->freq = dsk_radio_rcv->manual_seach_cur_freq;
	//}
	//else
	//{
		chan_info->freq = dsk_radio_rcv->search_result.freq_list[id].freq;
		eLIBs_strcpy(chan_info->name,dsk_radio_rcv->search_result.freq_list[id].name);
	//}	
	__wrn("freq_list[%d].freq =%d\n", id,dsk_radio_rcv->search_result.freq_list[id].freq);
	__wrn("valid_num=%d\n", dsk_radio_rcv->search_result.valid_num);
	return 0;
}

__s32 dsk_radio_rcv_get_favorite_chaninfo(__s32 id, dsk_radio_chan_t *chan_info)
{
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	chan_info->freq = dsk_radio_rcv->favorite.freq_list[id].freq;
	eLIBs_strcpy(chan_info->name,dsk_radio_rcv->favorite.freq_list[id].name);
	
	return 0;
}

__s32 dsk_radio_rcv_get_favorite(char *file_name)
{
	__s32 result;
	__u32 i;
	
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	dsk_radio_rcv->favorite.valid_num = 0;
	for(i=0;i<MAX_CHANNEL_NUM;i++)
	{
		eLIBs_memset(dsk_radio_rcv->favorite.freq_list[i].name,0,CHANNEL_NAME_LEN);
		dsk_radio_rcv->favorite.freq_list[i].freq = 0;
	}

	result = get_channel_list_from_file(file_name,&dsk_radio_rcv->favorite);
	
	__msg("valid num read from file is %d============\n",dsk_radio_rcv->favorite.valid_num);
	
	return result;
}

__s32 dsk_radio_rcv_save_favorite(char *file_name)
{
	__s32 result;
	
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	result = write_channel_list_to_file(file_name,&dsk_radio_rcv->favorite);
	
	return result;
}

__s32 dsk_radio_rcv_get_search_result(reg_fm_para_t* para)
{
	__u32 i, j;
	__u32 fm[3][6] = {  {87500, 90100, 98100, 106100, 108000, 87500},
						{87500, 90100, 98100, 106100, 108000, 87500},
						{87500, 90100, 98100, 106100, 108000, 87500} };
	
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	dsk_radio_rcv->search_result.valid_num = 0;
	for(i=0;i<MAX_CHANNEL_NUM;i++)
	{
		eLIBs_memset(dsk_radio_rcv->search_result.freq_list[i].name,0,CHANNEL_NAME_LEN);
		dsk_radio_rcv->search_result.freq_list[i].freq = 0;
	}

	//result = get_channel_list_from_file(file_name,&dsk_radio_rcv->search_result);
	if(para->FM_channel[0]!=0)
	{
		for(i=0;i<MAX_CHANNEL_NUM;i++)
		{
			dsk_radio_rcv->search_result.freq_list[i].freq = para->FM_channel[i];
			if(0 != dsk_radio_rcv->search_result.freq_list[i].freq)
			{
				dsk_radio_rcv->search_result.valid_num++;
			}
		}
	}
	dsk_radio_rcv->curFM1_3AM1_2_id = para->curFM1_3AM1_2_id;
	for(i = 0; i < MAX_FM1_3_AM1_2; i++)
	{
		for(j = 0; j < MAX_FM1_3_AM1_2_FREQ; j++)
		{
			if( para->FM1_3_AM1_2_freq[i][j] == 0 )
				dsk_radio_rcv->FM1_3_AM1_2_freq[i][j] = fm[i][j];
			else
				dsk_radio_rcv->FM1_3_AM1_2_freq[i][j] = para->FM1_3_AM1_2_freq[i][j];
		}
	}
	
	return EPDK_OK;
}

__s32 dsk_radio_rcv_save_search_result(void)
{
	__u32 i, j;
	reg_fm_para_t* para;
	
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}

	para = (reg_fm_para_t*)dsk_reg_get_para_by_app(REG_APP_FM);
	para->mode = 1;//FM mode~
	for(i=0;i<MAX_CHANNEL_NUM;i++)
	{
		para->FM_channel[i] = 0;
		para->AM_channel[i] = 0;
	}
	//result = write_channel_list_to_file(file_name,&dsk_radio_rcv->search_result);
	for(i=0;i<MAX_CHANNEL_NUM;i++)
	{
		para->FM_channel[i] = dsk_radio_rcv->search_result.freq_list[i].freq;
        	//__wrn("i=%d, para->FM_channel[i]=%d\n", i, para->FM_channel[i]);
	}
	for(i = 0; i < MAX_FM1_3_AM1_2; i++)
	{
		for(j = 0; j < MAX_FM1_3_AM1_2_FREQ; j++)
		{
			para->FM1_3_AM1_2_freq[i][j] = dsk_radio_rcv->FM1_3_AM1_2_freq[i][j];
		}
	}
	para->curFM1_3AM1_2_id = dsk_radio_rcv->curFM1_3AM1_2_id;
	dsk_reg_flush();
	
	return EPDK_OK;
}

__s32 dsk_radio_rcv_save_cur_freq(void)
{
	__u32 i, j;
	reg_fm_para_t* para;
    	__s32 cur_index = 1;
	    	    
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}

    	para = (reg_fm_para_t*)dsk_reg_get_para_by_app(REG_APP_FM);
   	__wrn("dsk_radio_rcv->cur_freq=%d\n", dsk_radio_rcv->cur_freq);
	__wrn("dsk_radio_rcv->search_result.valid_num=%d\n", dsk_radio_rcv->search_result.valid_num);
	for(i=0;i<MAX_CHANNEL_NUM;i++)
	{
		para->FM_channel[i] = dsk_radio_rcv->search_result.freq_list[i].freq;
		//__wrn("i=%d, para->FM_channel[i]=%d\n", i, para->FM_channel[i]);	
		
		//��ǰƵ���Ƿ���Ƶ���б������ֵ���,ȡǰ��λ�Ƚ���Чby cky
		if (dsk_radio_rcv->cur_freq /100 == dsk_radio_rcv->search_result.freq_list[i].freq /100)
	         {	         		
			para->channel_id = i;	
			__wrn("dsk_radio_rcv->cur_freq /100=%d\n",  dsk_radio_rcv->cur_freq/100);
			__wrn("dsk_radio_rcv->search_result.freq_list[i].freq /100=%d\n",  dsk_radio_rcv->search_result.freq_list[i].freq /100);				
			dsk_radio_rcv_save_search_result();
    			para->reserverd[0] = dsk_radio_rcv->search_result.valid_num;
			return EPDK_OK;
		 }      		
	}
		
    	__here__;
    	if(dsk_radio_rcv->cur_freq >= FM_SEARCH_CHN_MIN_FREQ
        	&& dsk_radio_rcv->cur_freq < dsk_radio_rcv->search_result.freq_list[0].freq)
    	{
		__here__;
		for(i = MAX_CHANNEL_NUM-1 ; i > 0; i--)
		{
		    __here__;
		    dsk_radio_rcv->search_result.freq_list[i].freq
		        = dsk_radio_rcv->search_result.freq_list[i-1].freq;
		}       
		dsk_radio_rcv->search_result.freq_list[0].freq = dsk_radio_rcv->cur_freq;
		dsk_radio_rcv->search_result.valid_num++;
		cur_index = 0;
		__wrn("@@@@@dsk_radio_rcv->search_result.freq_list[i].freq=%d\n", dsk_radio_rcv->search_result.freq_list[i].freq);
		__wrn("dsk_radio_rcv->search_result.valid_num=%d\n", dsk_radio_rcv->search_result.valid_num);
    	}
    	else//��Ҫ��������
    	{
        	__bool bok;
        	__here__;
	        bok = 0;//�Ƿ�����־
	        for(i=0;i<MAX_CHANNEL_NUM-1;i++)
	    	{            
			if(0 == dsk_radio_rcv->search_result.freq_list[i].freq)
			{
				break;
			}
			if(dsk_radio_rcv->cur_freq > dsk_radio_rcv->search_result.freq_list[i].freq
			&& dsk_radio_rcv->cur_freq < dsk_radio_rcv->search_result.freq_list[i+1].freq)
			{
				__here__;
				bok = 1;//�����־

				for(j = MAX_CHANNEL_NUM-2 ; j >= i+1; j--)
				{                    
				    	dsk_radio_rcv->search_result.freq_list[j+1].freq
				        	= dsk_radio_rcv->search_result.freq_list[j].freq;
				}

				dsk_radio_rcv->search_result.freq_list[i+1].freq = dsk_radio_rcv->cur_freq;
				dsk_radio_rcv->search_result.valid_num++;
				cur_index = i+1;
					__wrn("dsk_radio_rcv->search_result.valid_num=%d\n", dsk_radio_rcv->search_result.valid_num);  
				break;
	            	}                	
	    	}

	        if(0 == bok)
	        {
		       if(i > 0)
		       {
		                __here__;
		                if(dsk_radio_rcv->cur_freq > dsk_radio_rcv->search_result.freq_list[i-1].freq)
		                {
					__here__;
					__wrn("dsk_radio_rcv->search_result.freq_list[i-1].freq =%d\n", dsk_radio_rcv->search_result.freq_list[i-1].freq);
					__wrn("dsk_radio_rcv->cur_freq =%d\n", dsk_radio_rcv->cur_freq);
					dsk_radio_rcv->search_result.freq_list[i].freq = dsk_radio_rcv->cur_freq;                           
					dsk_radio_rcv->search_result.valid_num++;
					cur_index = i;
					__wrn("dsk_radio_rcv->search_result.valid_num=%d\n", dsk_radio_rcv->search_result.valid_num);
			         }
		         }
            		else
	            	{
		                __here__;
		                dsk_radio_rcv->search_result.freq_list[0].freq = dsk_radio_rcv->cur_freq;
		                dsk_radio_rcv->search_result.valid_num++;
		                cur_index = 0;
				__wrn("dsk_radio_rcv->search_result.valid_num=%d\n", dsk_radio_rcv->search_result.valid_num);
	            	}
        	}
    	}    
	__wrn("dsk_radio_rcv->search_result.valid_num=%d\n", dsk_radio_rcv->search_result.valid_num);
	
	    dsk_radio_rcv_save_search_result();
	    para->reserverd[0] = dsk_radio_rcv->search_result.valid_num;
	    if(-1 != cur_index)
	    {
	        para->channel_id = cur_index;		
	    }
    	__wrn("para->channel_id=%d\n", para->channel_id);
	return EPDK_OK;
}


__s32 dsk_radio_rcv_add_chan_to_favorite(__u32 id,dsk_radio_chan_t *chan_info)
{
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	if(id >= MAX_CHANNEL_NUM)
	{
		__msg("id exceed the size of favorite!");
		return -1;
	}
	
	dsk_radio_rcv->favorite.freq_list[id].freq = chan_info->freq;
	eLIBs_strcpy(dsk_radio_rcv->favorite.freq_list[id].name,chan_info->name);
	dsk_radio_rcv->favorite.valid_num++;
	
	return 0;
}

__s32 dsk_radio_rcv_del_chan_from_favorite(__u32 id)
{	
	__u32 i = id;
	
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	if(id <= 0)
	{
		__msg("id is invalid!\n");
		return -1;
	}
	if(id >= MAX_CHANNEL_NUM)
	{
		__msg("id exceed the size of favorite!\n");
		return -1;
	}
	
	if(id < dsk_radio_rcv->favorite.valid_num - 1)
	{
		for(i = id; i < dsk_radio_rcv->favorite.valid_num - 1; i++)
		{
			dsk_radio_rcv->favorite.freq_list[i].freq = dsk_radio_rcv->favorite.freq_list[i+1].freq;
			eLIBs_strcpy(dsk_radio_rcv->favorite.freq_list[i].name, dsk_radio_rcv->favorite.freq_list[i+1].name);
		}
	}
	dsk_radio_rcv->favorite.freq_list[i].freq = 0;
	eLIBs_strcpy(dsk_radio_rcv->favorite.freq_list[i].name, 0);
	
	dsk_radio_rcv->favorite.valid_num--;
	
	//__inf("---------------------\n");
	//for(i = 0; i < dsk_radio_rcv->favorite.valid_num; i++)
	//{
	//	__inf("number %d in favorite is %d\n", i, dsk_radio_rcv->favorite.freq_list[i].freq);
	//}
	//__inf("---------------------\n");
	return 0;
}


__s32 dsk_radio_rcv_close(void)
{
	__s32 result = 0;
	
	if(dsk_radio_rcv == NULL)
	{
		__msg("radio receive not open!\n");
		return EPDK_FAIL;
	}	

	if(dsk_radio_rcv->thread_id)
	{
		while(1)
		{       
			if(esKRNL_TDelReq(dsk_radio_rcv->thread_id) == OS_TASK_NOT_EXIST)
			{
			    break;
			}
			esKRNL_TimeDly(3);        
		}
	}
	
	result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_EXIT, 0, 0);
	
	esMEMS_Mfree(0,dsk_radio_rcv);
	dsk_radio_rcv = NULL;
	return result;
}



