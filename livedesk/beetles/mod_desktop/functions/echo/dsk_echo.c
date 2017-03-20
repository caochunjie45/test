
#include "..\\functions_i.h"

#define BEETLES_SOFTWARE_ECHO   1

static ES_FILE* g_fp_rec = NULL;


//mic ֱͨ���ܿ���mic  �����ź�(���ܾ���΢С�Ŵ�)ֱ��ͨ���������
//�����ֺ���Ƶ���ػ���������⣬�����ṩ
//�����ֺ���Ƶʱ�����ȵ���dsk_com_mic_close,�ٵ��ô˺���

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

//MIC ֱͨ
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
//auto_clean_buf:0�������Զ����buffer(¼��Ӧ��)��Ϊ1�����Զ����buffer(��¼����Ӧ��)
//��ϸ���÷����μ�SDK��app_root_scene.c��record_view_doc.c
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
    
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_SET_SAMPCNT, 0, 0);//��adc buffer������;

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

//��ϸ���÷����μ�SDK��app_root_scene.c��record_view_doc.c
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

//�ú���һ�㲻Ҫ���ã�����Ĭ�ϵľ�����
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

//�ú���һ�㲻Ҫ���ã�����Ĭ�ϵľ�����
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


//�ýӿ���ʱ���ã�CPU�ܲ���������mic������-32��32�����Ե���mic����
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

//�ýӿ���ʱ���ã�CPU�ܲ�������Сmic���� -32��32�����Ե���mic����
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

//�ú��������ã�������
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

/*aux: 0<= aux <= 10 ���û�����ʱ������Ĭ��Ϊ3*/
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

/*aux:10<= aux <= 3000,Ĭ��Ϊ100���Ժ���Ϊ��λ*/
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


/*�Ƿ�����MICХ�У�aux: 0:disable ec, 1: enable ec*/
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


/*�ýӿ���ʱ�����ã�CPU�ܲ��������ñ��ģʽ:
mode: 0xff:�û�ģʽ��1:��Ů�� 2:������ 3:��ͯ��
level:���mode=0xff,��ôlevelΪ����ȼ�-8<=level<=8, ���mode!=0xff����ôlevel��Ч*/
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

/*�Ƿ�򿪻��������aux: 0:disable , 1: enable */
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


