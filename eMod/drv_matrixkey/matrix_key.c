/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                           Matrix Key Driver Module
*
*                                    (c) Copyright 2006-2010, All winners Co,Ld.
*                                             All Rights Reserved
*
* File    : matrix_key.c
* By      : james deng <csjamesdeng@gmail.com>
* Version : 1.0.0
* Date    : 2011-04-25
* Descript:
* Update  : date              author         ver       notes
*           2011-04-25        james deng     1.0.0     build the file.
*********************************************************************************************************
*/
 
#include "drv_matrix_key_i.h"

#include "bsp_rk.h"

typedef enum
{
	KEY_NULL = 0,
	KEY_X1,
	KEY_Y1,
	KEY_X2,
	KEY_Y2,
	
}key_down;	




static __u8 matrix_key_tid = 0;

static __u32 g_key_down_tbl[KEY_CNT] = {0};


static __s32 get_adc_valume(void)
{
    __u32 ret;
    __s32 data[32];
    __s32 average;
    __s32 i;
    __s32 cnt;
        
    // 读取数据
    cnt = 0;
    while(1)
    {      
        ret = RK_QueryIntPending();
        if(ret & RK_DATA_PENDING)
        {
            RK_GetData(data, sizeof(data)/sizeof(data[0]));
            RK_ClearIntPending(ret);
            RK_ClearFifo();     
            break;
        }
        if(cnt++ > 2)
        {
            RK_ClearIntPending(ret);
            RK_ClearFifo();     
            break;
        }
        esKRNL_TimeDly(1);
    }

    if(!(ret & RK_DATA_PENDING))
    {
        return 0x7ff;
    }

    average = 0;
    for(i = sizeof(data)/sizeof(data[0])-1 ; i >= 0 ; i--)
    {
        average += data[i];
       // __msg("data[i]=%d \n", data[i]);
    }

    average /= sizeof(data)/sizeof(data[0]);

    return average;    
}

static void matrix_key_down(__u32 key_value)
{
    __key_msg_t msg;
    
    msg.key_value = key_value;
    msg.flag = 1;
    
    dev_matrix_key_ioctrl(0, DRV_KEY_CMD_PUTKEY, 0, &msg);

    g_key_down_tbl[key_value]++;
}

static void matrix_key_up(__u32 key_value)
{
    __key_msg_t msg;

    if(g_key_down_tbl[key_value] > 0)
    {
        g_key_down_tbl[key_value]--;
    }
    else
    {
        return ;
    }
    
	
    msg.key_value = key_value;
    msg.flag = 0;
    
    dev_matrix_key_ioctrl(0, DRV_KEY_CMD_PUTKEY, 0, &msg);
}




static __u32 adc_key_scan_X1()
{
	__s32 adc_valume;
	__u32 key = 0;
	

	esKRNL_TimeDly(1);
	adc_valume = get_adc_valume();

	if(adc_valume <= 1800)
	{
		
		if(adc_valume>=100&&adc_valume<400)
		{
			key = KPAD_MOVIE;
		}
		else if(adc_valume>=400&&adc_valume<700)
		{
			key = KPAD_MUSIC;
		}
		else if(adc_valume>=700&&adc_valume<1000)
		{
			key = KPAD_TV;
		}
		else if(adc_valume>=1000&&adc_valume<1300)
		{
			key = KPAD_MENU;
		}
		else if(adc_valume>=1300&&adc_valume<1500)
		{
			key = KPAD_TV_RIGHT;
		}			
		else if(adc_valume>=1500&&adc_valume<1700)
		{
			key = KPAD_TV_LEFT;
		}			

	}

	
	
	return key;
}

static __u32 adc_key_scan_X2()
{
	__s32 adc_valume = 0;
	__u32 key = 0;
	

	esKRNL_TimeDly(1);
	adc_valume = get_adc_valume();

	if(adc_valume <= 1900)
	{
		__msg("&&&&&&&&&&&&&& adc_valume_x2 = %d &&&&&&&&&&&&&\n",adc_valume);
		
		if(adc_valume>=100&&adc_valume<400)
		{
			key = KPAD_NUM0;
		}
		else if(adc_valume>=400&&adc_valume<700)
		{
			key = KPAD_NUM1;
		}
		else if(adc_valume>=700&&adc_valume<1000)
		{
			key = KPAD_NUM2;
		}
		else if(adc_valume>=1000&&adc_valume<1300)
		{
			key = KPAD_NUM3;
		}
		else if(adc_valume>=1300&&adc_valume<1450)
		{
			key = KPAD_NUM4;
		}			
		else if(adc_valume>=1450&&adc_valume<1600)
		{
			key = KPAD_NUM5;
		}			
		else if(adc_valume>=1600&&adc_valume<1900)
		{
			key = KPAD_NUM6;
		}			

	}
	
	
	return key;
}

static __u32 adc_key_scan_Y2()
{
	__s32 adc_valume;
	__u32 key = 0;

	
	esKRNL_TimeDly(1);
	adc_valume = get_adc_valume();
	if(adc_valume <= 2000)
	{
		if(adc_valume>=100&&adc_valume<400)
		{
			key = KPAD_NUM0;
		}
		else if(adc_valume>=400&&adc_valume<700)
		{
			key = KPAD_NUM7;
		}
		else if(adc_valume>=700&&adc_valume<1000)
		{
			key = KPAD_NUM9;
		}
		else if(adc_valume>=1000&&adc_valume<1300)
		{
			key = KPAD_NUM8;
		}
		else if(adc_valume>=1300&&adc_valume<1600)
		{
			key = KPAD_NUM6;
		}

	}
	
	return key;
}

static __u32 adc_key_scan_Y1()
{
	__s32 adc_valume = 0;
	__u32 key = 0;

	esKRNL_TimeDly(1);
	
	adc_valume = get_adc_valume();

	if(adc_valume <= 1700)
	{
		__msg("&&&&&&&&&&&&&& adc_valume_y1 = %d &&&&&&&&&&&&&\n",adc_valume);
		
		if(adc_valume>=100&&adc_valume<400)
		{
			key = KPAD_NUM7;
		}
		else if(adc_valume>=400&&adc_valume<700)
		{
			key = KPAD_NUM8;
		}
		else if(adc_valume>=700&&adc_valume<1000)
		{
			key = KPAD_NUM9;
		}
		else if(adc_valume>=1000&&adc_valume<1300)
		{
			key = KPAD_ENTER;
		}
		else if(adc_valume>=1300&&adc_valume<1500)
		{
			key = KPAD_RETURN;
		}
		else if(adc_valume>=1500&&adc_valume<1700)
		{
			key = KPAD_FM;
		}			

	}

	return key;
}




static void matrix_match_key_down(void)
{
    __s32 i;
    __s32 key_cnt;

    key_cnt = sizeof(g_key_down_tbl)/sizeof(g_key_down_tbl[0]);

    for(i = 0 ; i < key_cnt ; i++)
    {
        while(g_key_down_tbl[i] > 0)
        {
            matrix_key_up(i);
        }
    }
}

__s32 adc_key_init()
{
	RK_Init();
	RK_SetFifoDepth(RK_FIFO_DEPTH_08);
	RK_SetSampleRate(RK_SAMPLE_RATE_16K);
	RK_EnableInt(RK_DATA_PENDING);

	RK_SelectChannel(RK_CHANNEL_X1);
	RK_ClearFifo();
	RK_SelectChannel(RK_CHANNEL_X2);
	RK_ClearFifo();
	RK_SelectChannel(RK_CHANNEL_Y1);
	RK_ClearFifo();
	
	
	return EPDK_OK;
}

static void matrix_key_thread(void *p_arg)
{	 
	__u32 old_key = 0, new_key = 0, first_key = 0,key = 0,key_down = KEY_NULL;


	while (1)
	{
		if (esKRNL_TDelReq(OS_PRIO_SELF) == OS_TASK_DEL_REQ)
		{
	   		//杀线程;
			eDbug(" ===== kill the key thread ====== \n");
	   		esKRNL_TDel(OS_PRIO_SELF);
		} 

		
		if(old_key == 0)
		{
			RK_SelectChannel(RK_CHANNEL_X1);
			RK_ClearFifo();
			old_key = adc_key_scan_X1();
			key_down = KEY_X1;
		}
		
		
		if(old_key==0)
		{
			RK_SelectChannel(RK_CHANNEL_Y1);
			RK_ClearFifo();
			old_key = adc_key_scan_Y1();
			key_down = KEY_Y1;
		}
		
		
		if(old_key==0)
		{
			RK_SelectChannel(RK_CHANNEL_X2);
			RK_ClearFifo();
			old_key = adc_key_scan_X2();
			key_down = KEY_X2;
		}
		
	
		key = 0;
		new_key = 0;
		first_key = 1;
		

		while (old_key)
		{
			if(first_key == 1)
			{
				first_key = 0;
				esKRNL_TimeDly(2);
			}
			else
			{
				esKRNL_TimeDly(2);
			}
			

			if(key_down==KEY_X1)
			{
				RK_SelectChannel(RK_CHANNEL_X1);
				RK_ClearFifo();
				new_key = adc_key_scan_X1();
			}
			
			
			if(key_down==KEY_Y1)
			{
				RK_SelectChannel(RK_CHANNEL_Y1);
				RK_ClearFifo();
				new_key = adc_key_scan_Y1();
			}
			
			
			if(key_down==KEY_X2)
			{
				RK_SelectChannel(RK_CHANNEL_X2);
				RK_ClearFifo();
				new_key = adc_key_scan_X2();
			}
			
			
		
			if (new_key) /* 读到一个有效值 */
			{
				__here__;
				/* 前后两次读到的值不同，并且前面读到的值有效 */
				if(new_key != old_key)
				{
					
					old_key = 0;
					matrix_match_key_down();

					break;
				}
								
				else
				{
					__here__;
					/* 发送消息前检查是否延迟是否达到事先设定的阈值 */
					key = new_key;
					new_key = 0;
					matrix_key_down(key);
				}
			}
			
			else if (key) /* 按键弹起 */
			{
				__here__;
				old_key = 0;
				matrix_key_up(key);
				
				break;
			}
			else /* 读到一个无效值 */
			{
				//__here__;
				old_key = 0;
				matrix_match_key_down();
				
				break;
			}
		}
	}
}

__s32 matrix_key_init(void)
{

	__u8 Value = 0xff;


	adc_key_init();
	
	esKRNL_TimeDly(5);
	
	matrix_key_tid = esKRNL_TCreate(matrix_key_thread, 0, 0x800, KRNL_priolevel1);

	eDbug("matrix key init finish\n");

	return EPDK_OK;
}

__s32 matrix_key_exit(void)
{
	esKRNL_TDel(matrix_key_tid);
	
	RK_Exit();
	
	__msg("matrix key exit finish\n");

	return EPDK_OK;
}

