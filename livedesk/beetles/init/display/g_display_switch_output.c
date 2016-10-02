/*
************************************************************************************************************************
*                                                     close screen
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : g_display_switch_output.c
*
* Author      : Jackie.Chen
*
* Version     : 1.0
*
* Date        : 2010.10.28
*
* Description :
* 
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Jackie.Chen      2010.10.28        1.0         build the file
*
************************************************************************************************************************
*/

#include "..\\mod_init_i.h"


static __s32 set_hpCom_driver_mode(__bool isDcDriveMode)
{
    ES_FILE* pAuCtrl = eLIBs_fopen("b:\\AUDIO\\CTRL","r+");
    if(!pAuCtrl){
        __wrn("Fail to open Audio control dev.\n");
        return EPDK_FAIL;
    }
    eLIBs_fioctrl(pAuCtrl, AUDI_DEV_CMD_SET_HPCOM_DRIVE_MODE, isDcDriveMode, NULL);

    eLIBs_fclose(pAuCtrl);
    return EPDK_OK;
}

static __s32 __switch_output(__lion_disp_format_e mode)
{
	ES_FILE      		   	*p_disp;
	__u32                	epdkmode;
	__disp_output_type_t 	output;
	__u32					arg[3];
	//__u32					result;
	
	epdkmode = dsk_display_dskfmt2epdkfmt(mode, &output);	
	
	/*open display driver handle*/
	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		__err("open display device fail!\n");
	}
	//dsk_volume_off();
    if(output == DISP_OUTPUT_TYPE_LCD)
    {    	 
        set_hpCom_driver_mode(1);
		//dsk_set_audio_if(AUDIO_DEV_IF_CODEC);//112358
    }     
	else if(output == DISP_OUTPUT_TYPE_TV)
	{   		
		set_hpCom_driver_mode(0);
		arg[0] = epdkmode;
		arg[1] = 0;
		arg[2] = 0;
		eLIBs_fioctrl(p_disp, DISP_CMD_TV_SET_MODE, 0, (void *)arg);
		//dsk_set_audio_if(AUDIO_DEV_IF_CODEC);//112358
	}
	else if(output == DISP_OUTPUT_TYPE_VGA)
	{     
		arg[0] = epdkmode;
		arg[1] = 0;
		arg[2] = 0;
	    eLIBs_fioctrl(p_disp, DISP_CMD_VGA_SET_MODE, 0, (void *)arg);	    
	    dsk_set_audio_if(AUDIO_DEV_IF_CODEC);
	}
	else if(output == DISP_OUTPUT_TYPE_HDMI)
	{     
		arg[0] = epdkmode;
		arg[1] = 0;
		arg[2] = 0;
	    eLIBs_fioctrl(p_disp, DISP_CMD_HDMI_SET_MODE, 0, (void *)arg);		
	    dsk_set_audio_if(AUDIO_DEV_IF_IIS);
	}     
	eLIBs_fclose(p_disp);	
	//打开声音	
	dsk_volume_on();
	
	return  EPDK_OK;
}

__s32 g_display_switch_output(__lion_disp_format_e mode)
{	
	__disp_output_type_t 	output;

    {
        reg_system_para_t* para;
        para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
        if(para)
        {
            if(para->output == mode)
            {
                return EPDK_OK;
            }
            para->output = mode;
            //dsk_reg_flush();
        }
    }    
	
	dsk_display_dskfmt2epdkfmt(mode, &output);

	if(dsk_wkm_is_open())
	{
		dsk_wkm_pause();
	}
	//关闭当前显示
	dsk_display_off();
	
	//切屏	
	__switch_output(mode);	
	
	//通知init  切屏	
	SEND_MSG(DSK_MSG_SCREEN_SWITCH, 0, GUI_WinGetHandFromName("init"), mode, 0); 
	
	//toplevel  的activity  重新创建UI 界面
	//activity_suspend_top();
	//activity_resume_top();
	
	//打开显示
	dsk_display_on(output);	
	esKRNL_TimeDly(100);
	if(dsk_wkm_is_open())
	{
		dsk_wkm_resume_play();
	}
	
	return  EPDK_OK;
}

