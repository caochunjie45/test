/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									          shell application
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : Esh_shell.c
* By      : sunny
* Version : V2.0                                                            2009.3.2
*********************************************************************************************************
*/

#include    "Esh_shell_private.h"



__hdle logo_layer = NULL;
__bool pass_flag = EPDK_FALSE;
__u32  keycode1 = 0x00;
__u32  keycode2 = 0x00;

Picture_t * picture = NULL;



static __s32 __show_logo(void)
{
	ES_FILE      * p_disp;
	__s32		width, height;
	__disp_layer_info_t layer_para;
	__u32 arg[3];
	__s32 ret;

	//parse logo bmp	
	picture = (Picture_t *)eLIBs_malloc(sizeof(Picture_t));
	if(picture == NULL)
	{
		__wrn("malloc logo bmp buffer failed!\n");
		return EPDK_FAIL;
	}
	eLIBs_memset(picture, 0, sizeof(Picture_t));

	ret = Parse_Pic_BMP_ByPath(BEETLES_APP_ROOT"res\\boot_ui\\logo.bmp", picture, NULL);
	if(-1 == ret)
	{
		return -1;
	}
	
	/*open display driver handle*/
	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		Esh_Wrn("open display device fail!\n");
		return EPDK_FAIL;
	}	

	width  = eLIBs_fioctrl(p_disp, DISP_CMD_SCN_GET_WIDTH, 0, 0);
	height = eLIBs_fioctrl(p_disp, DISP_CMD_SCN_GET_HEIGHT, 0, 0);
	//__msg("width = %d, height = %d\n", width, height);

	////logo
	arg[0] = DISP_LAYER_WORK_MODE_NORMAL;
	arg[1] = 0;
	arg[2] = 0;
	logo_layer = eLIBs_fioctrl(p_disp, DISP_CMD_LAYER_REQUEST, 0, (void*)arg);

	eLIBs_memset(&layer_para,0,sizeof(__disp_layer_info_t));//防止以后SDK结构休有扩展时未清0,add by hot.lee
	layer_para.mode = DISP_LAYER_WORK_MODE_NORMAL; 
	layer_para.fb.addr[0]       = (__u32)picture->Buffer; 
	layer_para.fb.size.width    =  picture->Width;
	layer_para.fb.size.height    = picture->Height;
	layer_para.fb.mode          = DISP_MOD_INTERLEAVED;
	layer_para.fb.format        = DISP_FORMAT_ARGB8888;
	layer_para.fb.br_swap       = 0;
	layer_para.fb.seq           = DISP_SEQ_ARGB;
	layer_para.ck_enable        = 0;
	layer_para.alpha_en         = 1; 
	layer_para.alpha_val        = 0xff;
	layer_para.pipe             = 0; 
	layer_para.src_win.x        = 0;
	layer_para.src_win.y        = 0;
	layer_para.src_win.width    = picture->Width;
	layer_para.src_win.height   = picture->Height;
	layer_para.scn_win.x        = (width-picture->Width)>>1;
	layer_para.scn_win.y        = (height-picture->Height)>>1;
	layer_para.scn_win.width    = picture->Width;
	layer_para.scn_win.height   = picture->Height;

	arg[0] = logo_layer;
	arg[1] = (__u32)&layer_para;
	arg[2] = 0;
	eLIBs_fioctrl(p_disp,DISP_CMD_LAYER_SET_PARA,0,(void*)arg);

	arg[0] = logo_layer;
	arg[1] = 0;
	arg[2] = 0;
	eLIBs_fioctrl(p_disp,DISP_CMD_LAYER_OPEN,0,(void*)arg);

	eLIBs_fioctrl(p_disp, DISP_CMD_LCD_ON, 0, 0);
	

	eLIBs_fclose(p_disp);

	return EPDK_OK;
}

__s32 test_check_mcu()
{
	__s32 result=0;
	ES_FILE* fp = NULL;
	
	fp=eLIBs_fopen("b:\\USER\\MCU", "r+");
	if(fp==NULL)
	{
		__msg("open mcu driver is error\n");
		return EPDK_FAIL;
	}

	result = eLIBs_fioctrl(fp, DRV_MCU_CMD_CHECK_HXN, 0, 0);
	
	esKRNL_TimeDly(20);
	
	if(result == EPDK_FAIL)
		result = eLIBs_fioctrl(fp, DRV_MCU_CMD_CHECK, 0, 0);

	eLIBs_fclose(fp);	

	return result;
}


__s32 shellmain(void *p_arg)
{
	 __s32 result=0,i=0;
	__target_para_t target;


	esCFG_Init_Ex("d:\\sys_config.fex");
	esKSRV_GetTargetPara(&target);
	
	if(target.Debugenable == EPDK_TRUE)
	{
	    __here__;
	    esDEV_Plugin("drv\\sdmmc.drv", 0, 0, 1);  //112350 如果是调试，则加载sdmmc
	}

	esDEV_Plugin("\\drv\\display.drv", 0, 0, 1);

	__show_logo();
	
	

#if 1

	esDEV_Plugin("\\drv\\mcu.drv", 0, 0, 1);
	for(i = 0; i < 12; i++)
	{
		esKRNL_TimeDly(20);
		result = test_check_mcu();
		if(EPDK_OK == result)
			break;
	}
	esDEV_Plugout("\\drv\\mcu.drv", 0);

	if(result == EPDK_FAIL)
	{
	    esKRNL_TimeDly(1000);
	}

#endif


	
	esDEV_Plugin("\\drv\\ir.drv",0,0,1);
	esKRNL_TimeDly(50);
	//esDEV_Plugin("\\drv\\key.drv",0,0,1);
	

    /* Eshell initialize  */
    if ( ESH_FAILED == Esh_init() ) {
        Esh_Error("Eshell initialize failed\n");
        return ESH_FAILED;
    }
    /* execute setup first */
    Esh_StartUp();


    /* Eshell exit */
    Esh_exit();

	/* kill shell self */
    esEXEC_PDel(EXEC_pidself);

    /* del shell main thread */
    Esh_Msg("shell main thread: Bye Bye!\n");
    esKRNL_TDel(OS_PRIO_SELF);
    esKRNL_TimeDly(1000);

    return ESH_SUCCEED;
}
