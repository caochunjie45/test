#include "drv_display_i.h"

__disp_drv_t    g_disp_drv;

__s32 DRV_lcd_open(__u32 sel)
{    
    __u32 i = 0;
    __lcd_flow_t *flow;

	__msg("DRV_lcd_open\n");
    BSP_disp_lcd_open_before(sel);

    flow = BSP_disp_lcd_get_open_flow(sel);
    for(i=0; i<flow->func_num; i++)
    {
        flow->func[i].func(sel);
        esKRNL_TimeDly(flow->func[i].delay / 10);
    }
	
    BSP_disp_lcd_open_after(sel);
    
    return EPDK_OK;
}

__s32 DRV_lcd_close(__u32 sel)
{    
    __u32 i = 0;
    __lcd_flow_t *flow;
    
    BSP_disp_lcd_close_befor(sel);

    flow = BSP_disp_lcd_get_close_flow(sel);
    for(i=0; i<flow->func_num; i++)
    {
        flow->func[i].func(sel);
        esKRNL_TimeDly(flow->func[i].delay / 10);
    }

    BSP_disp_lcd_close_after(sel);

    return EPDK_OK;
}

__s32 DRV_scaler_begin(__u32 sel)
{
    __u8 err;

    esKRNL_SemPend(g_disp_drv.scaler_finished_sem[sel],0x30,&err);

	if(err == OS_NO_ERR)
	{
    	return EPDK_OK;
   	}
   	else
   	{
   		__wrn("writeback error\n");
   		return EPDK_FAIL;
   	}
}

void DRV_scaler_finish(__u32 sel)
{
    esKRNL_SemPost(g_disp_drv.scaler_finished_sem[sel]);
}


void DRV_tve_interrup(__u32 sel)
{
}

__s32 DRV_hdmi_set_mode(__disp_tv_mode_t mode)
{
	ES_FILE * p_hdmi;
	__s32 ret;
	
    p_hdmi = eLIBs_fopen("b:\\BUS\\HDMI", "r+");
    if(!p_hdmi)
    {
        __err("open hdmi device fail!\n");
    }
	ret  = eLIBs_fioctrl(p_hdmi, HDMI_CMD_SET_VIDEO_MOD, mode, 0);
	if(ret != EPDK_OK)
	{
		__wrn("set hdmi mode fail!\n");
		eLIBs_fclose(p_hdmi);
		return EPDK_FAIL;
	}
	eLIBs_fclose(p_hdmi);

	return EPDK_OK;
}

__s32 DRV_hdmi_open(void)
{
    
    ES_FILE * p_hdmi;
    __s32 ret;
    
    p_hdmi = eLIBs_fopen("b:\\BUS\\HDMI", "r+");
    if(!p_hdmi)
    {
        __err("open hdmi device fail!\n");
    }
    ret  = eLIBs_fioctrl(p_hdmi, HDMI_CMD_OPEN, 0, 0);
    if(ret != EPDK_OK)
    {
        __wrn("set hdmi mode fail!\n");
        eLIBs_fclose(p_hdmi);
        return EPDK_FAIL;
    }
    eLIBs_fclose(p_hdmi);
	
    return EPDK_OK;
}

__s32 DRV_hdmi_close(void)
{
    
    ES_FILE * p_hdmi;
    __s32 ret;
    
    p_hdmi = eLIBs_fopen("b:\\BUS\\HDMI", "r+");
    if(!p_hdmi)
    {
        __err("open hdmi device fail!\n");
    }
    ret  = eLIBs_fioctrl(p_hdmi, HDMI_CMD_CLOSE, 0, 0);
    if(ret != EPDK_OK)
    {
        __wrn("set hdmi mode fail!\n");
        eLIBs_fclose(p_hdmi);
        return EPDK_FAIL;
    }
    eLIBs_fclose(p_hdmi);
	
    return EPDK_OK;
}

__s32 DRV_hdmi_mode_support(__u8 mode)
{
    
    ES_FILE * p_hdmi;
    __s32 ret;
    
    p_hdmi = eLIBs_fopen("b:\\BUS\\HDMI", "r+");
    if(!p_hdmi)
    {
        __err("open hdmi device fail!\n");
    }
    ret  = eLIBs_fioctrl(p_hdmi, HDMI_CMD_MOD_SUPPORT, mode, 0);
    eLIBs_fclose(p_hdmi);
	
    return ret;
}
__s32 DRV_hdmi_get_HPD_status(void)
{
    
    ES_FILE * p_hdmi;
    __s32 ret;
    
    p_hdmi = eLIBs_fopen("b:\\BUS\\HDMI", "r+");
    if(!p_hdmi)
    {
        __err("open hdmi device fail!\n");
    }
    ret  = eLIBs_fioctrl(p_hdmi, HDMI_CMD_GET_HPD_STATUS, 0, 0);
    eLIBs_fclose(p_hdmi);
	
    return ret;
}

__s32 DRV_hdmi_set_pll(__u32 pll, __u32 clk)
{
    
    ES_FILE * p_hdmi;
    __s32 ret;
    
    p_hdmi = eLIBs_fopen("b:\\BUS\\HDMI", "r+");
    if(!p_hdmi)
    {
        __err("open hdmi device fail!\n");
    }
    ret  = eLIBs_fioctrl(p_hdmi, HDMI_CMD_SET_PLL, pll, (void*)clk);
    if(ret != EPDK_OK)
    {
        __wrn("set hdmi pll fail!\n");
        eLIBs_fclose(p_hdmi);
        return EPDK_FAIL;
    }
    eLIBs_fclose(p_hdmi);
	
    return EPDK_OK;
}


__mp * DRV_DISP_MOpen(__u32 mid, __u32 mode)
{
    g_disp_drv.mid = mid;
    g_disp_drv.used = 1;
    __msg("DRV_DISP_MOpen\n");
    return (__mp *)&g_disp_drv;
}
 
__s32 DRV_DISP_MClose(__mp * pDrv)
{
    return EPDK_OK;
}
__u32 DRV_DISP_MRead(void *pBuffer, __u32 size, __u32 count, __mp * pdev)
{
    return EPDK_OK;
}
__u32 DRV_DISP_MWrite(const void *pBuffer, __u32 size, __u32 count, __mp * pdev)
{
    return EPDK_OK;
}

__s32 DRV_DISP_MIoctrl(__mp* pDev, __u32 cmd, __s32 aux, void *pbuffer)
{ 
    __msg("cmd=%d DRV_DISP_MIoctrl\n",cmd);
    return   EPDK_OK;;
}

__s32 DRV_DISP_MInit(void)
{
    __disp_bsp_init_para para;
    __hdle hReg;
    __u32 i = 0;

	__msg("DRV_DISP_MInit...\n");
	eLIBs_memset(&para, 0, sizeof(__disp_bsp_init_para));
    para.base_image0    = 0xf1e60000;
    para.base_image1    = 0xf1e40000;
    para.base_scaler0   = 0xf1e00000;
    para.base_scaler1   = 0xf1e20000;
    para.base_lcdc0     = 0xf1c0c000;
    para.base_lcdc1     = 0xf1c0d000;
    para.base_tvec0     = 0xf1c0a000;
    para.base_tvec1     = 0xf1c1b000;
    para.base_ccmu      = 0xf1c20000;
    para.base_sdram     = 0xf1c01000;
    para.base_pioc      = 0xf1c20800;
	para.base_pwm       = 0xf1c20c00;
	para.base_iep 		= 0xf1e70000;
		
    para.scaler_begin   		= DRV_scaler_begin;
    para.scaler_finish  		= DRV_scaler_finish;
    para.tve_interrup   		= DRV_tve_interrup;
	para.hdmi_set_mode  		= DRV_hdmi_set_mode;
	para.Hdmi_open  			= DRV_hdmi_open;
	para.Hdmi_close  			= DRV_hdmi_close;
	para.hdmi_mode_support		= DRV_hdmi_mode_support;
	para.hdmi_get_HPD_status	= DRV_hdmi_get_HPD_status;
	para.hdmi_set_pll			= DRV_hdmi_set_pll;
	para.disp_int_process       = disp_int_process;
	
	eLIBs_memset(&g_disp_drv, 0, sizeof(__disp_drv_t));
    g_disp_drv.scaler_finished_sem[0] = esKRNL_SemCreate(0);
    if(!g_disp_drv.scaler_finished_sem[0])
    {
        __wrn("create scaler_finished_sem[0] fail!\n");
        return EPDK_FAIL;
    }  
    g_disp_drv.scaler_finished_sem[1] = esKRNL_SemCreate(0);
    if(!g_disp_drv.scaler_finished_sem[1])
    {
        __wrn("create scaler_finished_sem[1] fail!\n");
        return EPDK_FAIL;
    }    

    for(i = 0; i<MAX_EVENT_SEM; i++)
    {
    	g_disp_drv.event_sem[0][i] = NULL;
     	g_disp_drv.event_sem[1][i] = NULL;
    }

    BSP_disp_init(&para);
    BSP_disp_open();

	g_disp_drv.reg_operate_vaild = 1;
	
   	hReg = esDEV_DevReg("DISP", "DISPLAY", &dispdev_ops, 0);
    if(!hReg)
    {
        __inf("esDEV_DevReg() fail!\n");
    	return EPDK_FAIL;
    }
    return EPDK_OK;        
}

__s32 DRV_DISP_MExit(void)
{        
	__u8 err;

    BSP_disp_close();
    BSP_disp_exit(g_disp_drv.exit_mode);

	esKRNL_SemDel(g_disp_drv.scaler_finished_sem[0], 0, &err);
	esKRNL_SemDel(g_disp_drv.scaler_finished_sem[1], 0, &err);

    return EPDK_OK;
} 

 
