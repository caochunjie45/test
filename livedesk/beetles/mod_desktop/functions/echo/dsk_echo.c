
#include "..\\functions_i.h"

#define BEETLES_SOFTWARE_ECHO   1

static ES_FILE* g_fp_rec = NULL;


//mic 直通功能开，mic  采样信号(可能经过微小放大)直接通过功放输出
//进音乐和视频加载混响会有问题，所以提供
//进音乐和视频时必须先调用dsk_com_mic_close,再调用此函数

static void dsk_audio_direct_output_onoff(__u8 flag)
{
#if BEETLES_SOFTWARE_ECHO
    
	__u32 val;

	if(0 == flag)
	{
		val = MIC_DAC_CTL | (0x200019C0);
		MIC_DAC_CTL = val;

	    val = MIC_ADC_CTL & ~(1U<<4);
		MIC_ADC_CTL = val;		
	}
	else
	{
		val = MIC_DAC_CTL | (0x200019C0);
		MIC_DAC_CTL = val;
		
		val = MIC_ADC_CTL | (1U<<4);
		MIC_ADC_CTL = val;
	}
#endif	    
    
}

//MIC 直通
static __s32 __app_mic_start(__s32 mic_no)
{    
#if BEETLES_SOFTWARE_ECHO

    {
	    if(mic_no == 1)
	    {
		    SET_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_LMIC1_TO_LMIX_BIT);
		    SET_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_RMIC1_TO_RMIX_BIT);    
	    }
	    else if(mic_no == 2)
	    {
		    SET_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_LMIC2_TO_LMIX_BIT);
		    SET_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_RMIC2_TO_RMIX_BIT);    
	    }    

		dsk_audio_direct_output_onoff(1);
		

    }
#endif	    

    return EPDK_OK;
}

static __s32 __app_mic_stop(__s32 mic_no,__s32 direct_off)
{    
#if BEETLES_SOFTWARE_ECHO

    {
	    if(mic_no == 1)
	    {
		    CLEAR_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_LMIC1_TO_LMIX_BIT);
		    CLEAR_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_RMIC1_TO_RMIX_BIT);    
	    }
	    else if(mic_no == 2)
	    {
		    CLEAR_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_LMIC2_TO_LMIX_BIT);
		    CLEAR_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_RMIC2_TO_RMIX_BIT);    
	    }   

	if(direct_off)
	    dsk_audio_direct_output_onoff(0);	
	    

    }
#endif	    

    return EPDK_OK;
}

//type:AUDIO_REC_USR_MIC/AUDIO_REC_USR_FM....
//auto_clean_buf:0代表不会自动清空buffer(录音应用)，为1代表自动清空buffer(非录音的应用)
//详细调用方法参见SDK的app_root_scene.c和record_view_doc.c
__s32 dsk_audio_echo_open(__audio_rec_user_t type, __s32 auto_clean_buf)
{

#if BEETLES_SOFTWARE_ECHO
    __s32 aux;
    __audio_dev_para_t adev_para;
    
    if(g_fp_rec)
    {
        __wrn("rec hdl alreadly open...\n");
        return EPDK_FAIL;
    }
    
    g_fp_rec = eLIBs_fopen("b:\\AUDIO\\REC", "r+");    

    if(NULL == g_fp_rec)
    {
        __wrn("open rec hdl fail...\n");
        return EPDK_FAIL;
    }

    
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_REG_USERMODE, type, 0);

    adev_para.chn = 2;
    adev_para.bps = 16;
    adev_para.fs = 44100;
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_SET_PARA, 0, (void *)&adev_para);

    //eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_REG_USERMODE, type, 0);
    
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_SET_SAMPCNT, 0, 0);//清adc buffer计数器;

    aux = 1;
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_ENABLE_ECHO, aux, 0);

    aux = auto_clean_buf;
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_BUF_MODE, aux, 0);

    
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_START, 0, 0);
	
    dsk_audio_echo_set_mic_pre_gain(0);
    
    dsk_audio_echo_set_mic_gain(7);
    
	__app_mic_start(1);

	


#endif     
    return EPDK_OK;
}

//详细调用方法参见SDK的app_root_scene.c和record_view_doc.c
__s32 dsk_audio_echo_close(__s32 direct_off)
{
#if BEETLES_SOFTWARE_ECHO
    __s32 aux;
    
    if(NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }        

    aux = 0;
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_ENABLE_ECHO, aux, 0);

    aux = 0;
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_BUF_MODE, aux, 0);

    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_STOP, 0, 0);
    
    if(g_fp_rec)
    {
        eLIBs_fclose(g_fp_rec);
        g_fp_rec = NULL;
    }
	__here__;
	__app_mic_stop(1,direct_off);


#endif    
    return EPDK_OK;
}

//该函数一般不要调用，按照默认的就行了
//0<= pre_gain <= 3
__s32 dsk_audio_echo_set_mic_pre_gain(__s32 pre_gain)
{
#if BEETLES_SOFTWARE_ECHO
    pre_gain &= 0x03;
    
    *(volatile unsigned long*)(0xf1c22c28) &= ~(0x03<<25);
    *(volatile unsigned long*)(0xf1c22c28) |= (pre_gain<<25);
#endif
    return EPDK_OK;
}

//该函数一般不要调用，按照默认的就行了
//0<= gain <= 7
__s32 dsk_audio_echo_set_mic_gain(__s32 gain)
{
#if BEETLES_SOFTWARE_ECHO
    gain &= 0x07;
    
    *(volatile unsigned long*)(0xf1c22c28) &= ~(0x07<<20);
    *(volatile unsigned long*)(0xf1c22c28) |= (gain<<20);
#endif	
    return EPDK_OK;   
}


//该接口暂时不用，CPU跑不够，增加mic音量，-32到32，可以调节mic音量
__s32 dsk_audio_echo_gain_incr(void)
{
#if BEETLES_SOFTWARE_ECHO
    __s32 aux;

    __wrn("dsk_audio_echo_gain_incr...\n");
    
    if(NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }        

    aux = eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_GET_ECHO_REC_GAIN, 0, 0);
    if(aux < 32)
    {
        aux++;
    }

    __wrn("dsk_audio_echo_gain_incr, aux=%d\n", aux);

    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_REC_GAIN, aux, 0);
 #endif   
    return EPDK_OK;
}

//该接口暂时不用，CPU跑不够，减小mic音量 -32到32，可以调节mic音量
__s32 dsk_audio_echo_gain_decr(void)
{
#if BEETLES_SOFTWARE_ECHO
    __s32 aux;    
    
    if(NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }        

    aux = eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_GET_ECHO_REC_GAIN, 0, 0);
    if(aux > -32)
    {
        aux--;
    }

    __wrn("dsk_audio_echo_gain_decr, aux=%d\n", aux);
    
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_REC_GAIN, aux, 0);
#endif    
    return EPDK_OK;
}

//该函数不调用，调试用
__s32 dsk_audio_echo_pt_regs(__u32 start, __u32 end)
{
#if BEETLES_SOFTWARE_ECHO
    __u32 i;
    
    eLIBs_printf("*****dsk_audio_echo_pt_regs begin******\n");

    for(i = start ; i <= end ; i+= 4)
    {
        eLIBs_printf("0x%8x:0x%8x\n", i, *(volatile unsigned long*)i);
    }
    
    eLIBs_printf("*****dsk_audio_echo_pt_regs end******\n");
#endif
    return EPDK_OK;
}

/*aux: 0<= aux <= 10 设置混响延时次数，默认为3*/
__s32 dsk_audio_echo_set_delay_num(__s32 aux)
{
#if BEETLES_SOFTWARE_ECHO
    __wrn("dsk_audio_echo_set_delay_num...\n");
    
    if(NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }        

    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_DELAY_NUM, aux, 0);
#endif    
    return EPDK_OK;
}

/*aux:10<= aux <= 3000,默认为100，以毫秒为单位*/
__s32 dsk_audio_echo_set_delay_time(__s32 aux)
{
#if BEETLES_SOFTWARE_ECHO
    __wrn("dsk_audio_echo_set_delay_num...\n");
    
    if(NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }        

    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_DELAY_TIME, aux, 0);
#endif    
    return EPDK_OK;
}

__s32 dsk_audio_echo_is_open(void)
{
#if BEETLES_SOFTWARE_ECHO
    if(g_fp_rec)
    {
        return 1;
    }
    else
    {
        return 0;
    }
#endif	
        return 0;
}


/*是否抑制MIC啸叫，aux: 0:disable ec, 1: enable ec*/
__s32 dsk_audio_echo_enable_ec(__s32 aux)
{
#if BEETLES_SOFTWARE_ECHO
    __wrn("dsk_audio_echo_enable_ec, aux=%d...\n", aux);
    
    if(NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }        

    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_ENABLE_ECHO_CANCEL, aux, 0);
#endif	    
    return EPDK_OK;
}


/*该接口暂时不可用，CPU跑不够，设置变调模式:
mode: 0xff:用户模式，1:变女声 2:变男声 3:变童声
level:如果mode=0xff,那么level为变调等级-8<=level<=8, 如果mode!=0xff，那么level无效*/
__s32 dsk_audio_echo_set_pitch_mode(__s32 mode, __s32 level)
{
#if BEETLES_SOFTWARE_ECHO
    __wrn("dsk_audio_echo_set_pitch_mode, mode=%d, level=%d...\n", mode, level);
    
    if(NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }        

    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_ECHO_SET_PITCH_MODE, mode, (void*)level);
#endif    
    return EPDK_OK;
}

/*是否打开混响输出，aux: 0:disable , 1: enable */
__s32 dsk_audio_echo_enable_output(__s32 aux)
{
#if 0
    __wrn("dsk_audio_echo_enable_ec, aux=%d...\n", aux);
    
    if(NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }        

    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_ENABLE_ECHO_OUTPUT, aux, 0);
    
    if(0 == aux)
  	{
		__app_mic_stop(1,1);
  	}
  	else
	{  				
		__app_mic_start(1);
	}
    
#endif    
    return EPDK_OK;
}


