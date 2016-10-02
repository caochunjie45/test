/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                            power manager sub-system
*
*                                     (c) Copyright 2008-2009, kevin.z China
*                                              All Rights Reserved
*
* File   : standby.c
* Version: V1.0
* By     : kevin.z
* Date   : 2009-6-6 16:09
*********************************************************************************************************
*/

#include "standby_i.h"

#define KEY_REG_BASE 		0xf1c25000
/* register  offset */
#define TP_CTRL_REG0          		(KEY_REG_BASE+0x00) //tp control register 0
#define TP_CTRL_REG1          		(KEY_REG_BASE+0x04) //tp control register 1
#define TP_CTRL_REG2          		(KEY_REG_BASE+0x08) //tp pressure measurement and sensitive control register 
#define TP_CTRL_REG3          		(KEY_REG_BASE+0x0c) //mediam filter control register 3
#define TP_INT_FIFO_CTRL_REG       	(KEY_REG_BASE+0x10) //tp interrupt  fifo control register
#define TP_INT_FIFO_STAT_REG       	(KEY_REG_BASE+0x14) //tp interrupt  fifo status register
#define TP_TEMP_PERIOD_REG			(KEY_REG_BASE+0x18) //tp temperature period register
#define TEMP_DATA_REG         		(KEY_REG_BASE+0x20) //emperaure data register
#define TP_DATA_REG         		(KEY_REG_BASE+0x24) //tp data register

#define TEMP_IRQ_EN     (0x01<<18) 
#define TPADCKEY_DAT_IRQ_EN     (0x01<<16)  
#define TPADCKEY_DOWN_EN     	(0x01<<0)  
#define TPADCKEY_UP_EN       	(0x01<<1)
#define TP_DATA_DRQ_EN       	(0x01<<7)
#define TP_DATA_XY_CHANGE_EN       	(0x01<<13)
#define TP_CLEAR_FIFO       	(0x01<<4)


#define TPADCKEY_IRQ_EN    		(TP_CLEAR_FIFO|TEMP_IRQ_EN|TPADCKEY_DAT_IRQ_EN|TPADCKEY_DOWN_EN|TPADCKEY_UP_EN|TP_DATA_DRQ_EN|TP_DATA_XY_CHANGE_EN)

#define TPADCKEY_DAT_IRQ_PENDING     	(0x01<<16)  
#define TPADCKEY_DOWN_PENDING     		(0x01<<0)  
#define TPADCKEY_UP_PENDING      		(0x01<<1)  
#define TPADCKEY_IRQ_PENDING    		(TPADCKEY_DAT_IRQ_PENDING|TPADCKEY_DOWN_PENDING|TPADCKEY_UP_PENDING)





#define stabdby_clk_set_onoff	1

//extern void wfi_func(void);

static __s32 standby_exit(void);
static __s32 standby_init(void);
static __s32 standby_wait_event(void);

static __u32 dcdc2Vol;
static __u32 dcdc3Vol;
static struct pll_factor_t orig_pll;
static struct pll_factor_t local_pll;
static struct sun4i_clk_div_t  clk_div;
static struct sun4i_clk_div_t  tmp_clk_div;

__u32  sd_flag;
__u32  uarttx_flag;
__u32  uartrx_flag;



/*
*********************************************************************************************************
*                                     SYSTEM PWM ENTER STANDBY MODE
*
* Description: enter standby mode.
*
* Arguments  : none
*
* Returns    : none;
*********************************************************************************************************
*/
extern  __sys_pwm_para_t StandbyPar;

#ifdef DBG_TEST
void test_init()
{
    __hdle      p_disp;
    /*open display driver handle*/
    p_disp = esFSYS_fopen("b:\\DISP\\DISPLAY", "r+");
    StandbyPar.IrMask = 0;
    StandbyPar.IrPowerVal = 0;
    
    //clear event flag
    StandbyPar.EventFlag = 0;
    StandbyPar.PowerOffTime = 2000;
    if(!p_disp)
    {
        standby_puts("open display drv err!\n");
        return ;
    }   

    esFSYS_fioctrl(p_disp, DISP_CMD_LCD_OFF, 0, 0);
    esFSYS_fclose(p_disp);

    return ;
}
void test_exit()
{
    __hdle      p_disp;
    /*open display driver handle*/
    p_disp = esFSYS_fopen("b:\\DISP\\DISPLAY", "r+");
    if(!p_disp)
    {
        standby_puts("open display drv err!\n");
        return ;
    }   

    esFSYS_fioctrl(p_disp, DISP_CMD_LCD_ON, 0, 0);
    esFSYS_fclose(p_disp);
    return ;
}
#endif


void enter_standby(void)
{
#ifdef DBG_TEST
    test_init();
#endif
    
    //enable dram selfrefresh
    standby_dram_enable_selfrefresh();

    //initialise standby modules
    standby_init();
   
    //wait for wakeup events
    standby_wait_event();
	
    //exit standby modules
    standby_exit();
    
    //disable dram selfrefresh
    standby_dram_disable_selfrefresh();
	standby_puts("standby_exit finish\n");
#ifdef DBG_TEST
    test_exit();
#endif
    return;
}




/*
*********************************************************************************************************
*                                       INITIALIZE STANDBY MODULES
*
* Description: 
*
* Arguments  : 
*
* Returns    :
*
* Note       :
*********************************************************************************************************
*/
static __s32 standby_init(void)
{

	char  string[128];

	
	STANDBY_WREG(0xf1c20c94,STANDBY_RREG(0xf1c20c94) &~(0x3)); //disable watchdog

	//standby_put_uint(20);standby_puts("\n");

	//initialize clock module
	#if (stabdby_clk_set_onoff)
	standby_clk_init();
	standby_clk_apbinit();
	#endif
	//standby_put_uint(21);
	//standby_puts("\n");

	//initialise interrupt module
	standby_int_init();
	
	//initialise timer module
	standby_tmr_init();

	if(pStandbyPar->PowerOffTime != 0)
	{
		standby_tmr_set(pStandbyPar->PowerOffTime);
		standby_enable_int(INT_SOURCE_TIMER0);
	}

	//standby_put_uint(22);standby_puts("\n");

	//initialise key module
#if (ALLOW_TPADCKEY_WAKEUP_STANDBY)
	standby_key_init();
	standby_enable_int(INT_SOURCE_TOUCHPNL);
#endif

	//standby_put_uint(223);standby_puts("\n");

	//initialise ir module
#if (ALLOW_IR_WAKEUP_STANDBY)
	standby_ir_init();
	//standby_put_uint(224);standby_puts("\n");
	standby_enable_int(INT_SOURCE_IR0);
	//standby_enable_int(INT_SOURCE_IR1);
#else
	STANDBY_WREG(IR_REG_CTRL,STANDBY_RREG(IR_REG_CTRL)&~(0x1<<0));
	standby_disable_int(INT_SOURCE_IR0);
#endif
	//standby_put_uint(33);standby_puts("\n");
	//initialise power module
	standby_power_init();
	standby_enable_int(INT_SOURCE_EXTNMI);

	//initialise usb module
#if (ALLOW_USB_WAKEUP_STANDBY)
	standby_usb_init();
	standby_enable_int(INT_SOURCE_USB0);
#endif


#if (ALLOW_SD_WAKEUP_STANDBY)
	standby_sdmmc_init();    
        standby_enable_int(INT_SOURCE_SDMMC0);
#endif
	//standby_put_uint(44);standby_puts("\n");

	/* gating off dram clock */
        standby_clk_dramgating(0);
    
	//standby_put_uint(23);



#if 1
	/* backup cpu freq */
	standby_clk_get_pll_factor(&orig_pll);
	
	/*lower freq from 1008M to 384M*/
	local_pll.FactorN = 16;
	local_pll.FactorK = 0;
	local_pll.FactorM = 0;
	local_pll.FactorP = 0;
	standby_clk_set_pll_factor(&local_pll);

#endif
        //switch cpu clock to HOSC
        standby_clk_core2hosc();
    
        //disable plls
        standby_clk_plldisable();


	//standby_put_uint(24);

    // adjust voltages
    //if(pStandbyPar->PowerCfg.power_exist)
    {
    	//backup voltages
    	dcdc2Vol = standby_get_voltage(STANDBY_POWER_VOL_DCDC2);
    	dcdc3Vol = standby_get_voltage(STANDBY_POWER_VOL_DCDC3);
    	//adjust voltage
    	standby_set_voltage(STANDBY_POWER_VOL_DCDC3, STANDBY_DCDC3_VOL);
    	standby_set_voltage(STANDBY_POWER_VOL_DCDC2, STANDBY_DCDC2_VOL);
		standby_set_voltage(STANDBY_POWER_VOL_LDO3, STANDBY_LDO3_VOL);
		
    }

#if (1)

    /* set clock division cpu:axi:ahb:apb = 2:2:2:1 */
    standby_clk_getdiv(&clk_div);
    tmp_clk_div.axi_div = 0;
    tmp_clk_div.ahb_div = 0;
    tmp_clk_div.apb_div = 0;
	tmp_clk_div.ahb_src = 0;
    standby_clk_setdiv(&tmp_clk_div);
    /* swtich apb1 to losc */
#if  (ALLOW_IR_WAKEUP_STANDBY == 0) //ir无法在32KHZ的情况下正常工作
    standby_clk_apb2losc();
#else
	standby_clk_apb2hosc();

#endif
    ///
    standby_mdelay(10);
	/* switch cpu to 32k */
#if  (ALLOW_IR_WAKEUP_STANDBY == 0) //ir无法在32KHZ的情况下正常工作
	standby_clk_core2losc();
#else
	standby_clk_core2hosc();
#endif

	//standby_put_uint(25);

#if(ALLOW_DISABLE_HOSC)
    // disable HOSC, and disable LDO
    standby_clk_hoscdisable();
	standby_clk_ldodisable();
#endif

#endif

	
	//wfi_func();
    //standby initialize finished
    return EPDK_OK;
}

/*
*********************************************************************************************************
*                                       EXIT STANDBY MODULES
*
* Description: 
*
* Arguments  : 
*
* Returns    :
*
* Note       :
*********************************************************************************************************
*/
static __s32 standby_exit(void)
{
#if (1)
	__u32 reg_val;

    #if(ALLOW_DISABLE_HOSC)
    /* enable LDO, enable HOSC */
    standby_clk_ldoenable();
    /* delay 1ms for power be stable */
    standby_delay(1);
    standby_clk_hoscenable();
    standby_delay(1);
    #endif
	//standby_puts("standby_exit 1\n");
    /* swtich apb1 to hosc */
    standby_clk_apb2hosc();
	//standby_puts("standby_exit 2\n");
    /* switch clock to hosc */
    standby_clk_core2hosc();
	//standby_puts("standby_exit 3\n");

    /* restore clock division */
    standby_clk_setdiv(&clk_div);  

//standby_puts("standby_exit 4\n");

	
#endif

    //restore voltage for exit standby
    if(pStandbyPar->PowerCfg.power_exist)
    {
        //restore voltage for exit standby
        //standby_puts("standby_exit 5\n");
    	standby_set_voltage(STANDBY_POWER_VOL_DCDC2, dcdc2Vol);
    	standby_set_voltage(STANDBY_POWER_VOL_DCDC3, dcdc3Vol);
		//standby_puts("standby_exit 6\n");
    	standby_mdelay(10);
    }
    #if (stabdby_clk_set_onoff)
    //enable plls
    //standby_puts("standby_exit 7\n");
    standby_clk_pllenable();
	//standby_puts("standby_exit 8\n");
    standby_mdelay(10);
    
    //switch cpu clock to core pll
    standby_clk_core2pll();
	//standby_puts("standby_exit 9\n");
    standby_mdelay(10);
#if 1
	standby_clk_set_pll_factor(&orig_pll);
	standby_mdelay(5);
#endif
	//standby_puts("standby_exit 10\n");

	
	//gating on dram clock
    standby_clk_dramgating(1);
	//standby_puts("standby_exit 11\n");
	#endif
	
    //exit usb module
    #if (ALLOW_USB_WAKEUP_STANDBY)
    standby_usb_exit();
    #endif
	//exit sd module
    #if (ALLOW_SD_WAKEUP_STANDBY)
	standby_sdmmc_exit();
	#endif
    
    //exit power module
   standby_power_exit();
    //standby_puts("standby_exit 12\n");
    //exit ir module
    #if (ALLOW_IR_WAKEUP_STANDBY)
    standby_ir_exit();
#else

	/* 把pending位清掉 */
	reg_val = STANDBY_RREG(IR_REG_RXSTA);
	reg_val |= IR_RXSTA_ALL_MASK;
	STANDBY_WREG(IR_REG_RXSTA, reg_val);
	
	standby_enable_int(INT_SOURCE_IR0);
	STANDBY_WREG(IR_REG_CTRL,STANDBY_RREG(IR_REG_CTRL)|0x1);

    #endif
    
    //exit key module
    #if (ALLOW_TPADCKEY_WAKEUP_STANDBY)
    standby_key_exit();
    #endif
    //standby_puts("standby_exit 13\n");
    //exit tmr module
    standby_tmr_exit();
    //standby_puts("standby_exit 14\n");
    //exit interrupt module
    standby_int_exit();
	//standby_puts("standby_exit 15\n");

	#if (stabdby_clk_set_onoff)
	
    standby_clk_apbexit();
	//standby_puts("standby_exit 16\n");
    //exit clock module
    standby_clk_exit();
	#endif
    
    //standby exit finished
    return EPDK_OK;    
}

/*
*********************************************************************************************************
*                                       WAIT FOR WAKEUP EVENTS
*
* Description: 
*
* Arguments  : 
*
* Returns    :
*
* Note       :
*********************************************************************************************************
*/


__s32 SelectChannel(__s32 channel)
{
    __u32       reg_value;

    switch(channel)
    {
        case 1:
        {
        	reg_value = STANDBY_RREG(0xf1c25004);
        	reg_value = reg_value & ~(0x07);
        	reg_value = reg_value | (0x00 << 0);        // select x1 channel
        	STANDBY_WREG(0xf1c25004, reg_value);
            
            break;
        }
        case 2:
        {
        	reg_value = STANDBY_RREG(0xf1c25004);
        	reg_value = reg_value & ~(0x07);
        	reg_value = reg_value | (0x01 << 0);        // select x2 channel
        	STANDBY_WREG(0xf1c25004, reg_value);
            
            break;
        }

		case 3:
        {
        	reg_value = STANDBY_RREG(0xf1c25004);
        	reg_value = reg_value & ~(0x07);
        	reg_value = reg_value | (0x02 << 0);        // select y1 channel
        	STANDBY_WREG(0xf1c25004, reg_value);
            
            break;
        }
        case 4:
        {
        	reg_value = STANDBY_RREG(0xf1c25004);
        	reg_value = reg_value & ~(0x07);
        	reg_value = reg_value | (0x03 << 0);        // select y2 channel
        	STANDBY_WREG(0xf1c25004, reg_value);
            
            break;
        }

		case 5:
		{
        	reg_value = STANDBY_RREG(0xf1c25004);
        	reg_value = reg_value & ~(0x07);
        	reg_value = reg_value | (0x04 << 0);        // select round channel
        	STANDBY_WREG(0xf1c25004, reg_value);
            
            break;
        }	
        default:
        {
            break;
        }
    }
    
    return EPDK_OK;
}



/*
*********************************************************************************************************
*                                     QUERY KEY FOR WAKEUP STANDBY
*
* Description: query key for wakeup standby.
*
* Arguments  : none
*
* Returns    : result;
*               EPDK_TRUE,      get a key;
*               EPDK_FALSE,     no key;
*********************************************************************************************************
*/
__s32 standby_get_adc_value(void)
{


	__u32 reg_val,ret;
    __s32 data[8];
	
	__s32 average;
    __s32 i;
	__s32 j;
    __s32 cnt;

	cnt = 0;
	while(1)
	{
		ret = STANDBY_RREG(TP_INT_FIFO_STAT_REG);
		if(ret & TPADCKEY_DAT_IRQ_PENDING)
		{
			for (i = 0; i < 8; i++){
				data[i] = STANDBY_RREG(0xf1c25024) & 0xfff;		
			}
			
			STANDBY_WREG(TP_INT_FIFO_STAT_REG, ret);

			reg_val = STANDBY_RREG(TP_INT_FIFO_CTRL_REG);
			reg_val |= (0x1 << 4);
			STANDBY_WREG(TP_INT_FIFO_CTRL_REG, reg_val);

			break;
			
		}
		if(cnt++ > 2)
        {
          	STANDBY_WREG(TP_INT_FIFO_STAT_REG, ret);

			
			reg_val = STANDBY_RREG(TP_INT_FIFO_CTRL_REG);
			reg_val |= (0x1 << 4);
			STANDBY_WREG(TP_INT_FIFO_CTRL_REG, reg_val);  
            break;
        }
		for(i=0;i<300;i++)
		{
			for(j=0;j<500;j++);
		}
    }

		
   if(!(ret & TPADCKEY_DAT_IRQ_PENDING))
    {
       for(i = sizeof(data)/sizeof(data[0])-1 ; i >= 0 ; i--)
       {
         data[i] = 0xfff;
       }
    }

    average = 0;
    for(i = sizeof(data)/sizeof(data[0])-1 ; i >= 0 ; i--)
    {
        average += data[i];
    }

    average /= sizeof(data)/sizeof(data[0]);

    return average;


}

static __s32 ClearFifo(void)
{
	__u32 reg_val;
	
	reg_val = STANDBY_RREG(0xf1c25010);
	reg_val |= (0x1 << 4);
	STANDBY_WREG(0xf1c25010, reg_val);
}



static __s32 standby_wait_event(void)
{

	__s32 data;
	__u32 i = 0;
	__s32 adc_val;

	standby_puts("-------------standby_wait_event\n");
	standby_put_hex(STANDBY_RREG(0xf1c2005c));
	standby_put_hex(STANDBY_RREG(0xf1c20060));
	standby_put_hex(STANDBY_RREG(0xf1c20064));
	standby_put_hex(STANDBY_RREG(0xf1c20068));
	standby_put_hex(STANDBY_RREG(0xf1c2006c));
    //wait for valid wakeup event coming
   while (1)
    {
        //enter WFI(wait for interrupt) instruction
        //__asm{mcr p15, 0, opdata, c7, c0, 4}
        //__asm{wfi}
        //wfi_func();
        //------------------------------------------------------------
        //
        //      wakeup interrupt coming, 
        //      check the wakeup interrupt source.
        //
        //------------------------------------------------------------
  


	
        // check power-off timer
        if(standby_tmr_query(STANDBY_TMR_TICK) == EPDK_TRUE)
        {
			pStandbyPar->WakeUpMode = STANDBY_WAKEUPMODE_OTHER;
           	 break;
        }


#if (ALLOW_TPADCKEY_WAKEUP_STANDBY)
	SelectChannel(1);
	ClearFifo();

	for(i=0;i<3;i++)
	{
		data = STANDBY_RREG(0xf1c25024) & 0xfff;
		if(data < 1800)
		{
			standby_puts("-------------return 0\n");
			return EPDK_OK;
		}
			
	}
///////////
	SelectChannel(2);
	ClearFifo();

	for(i=0;i<3;i++)
	{
		data = STANDBY_RREG(0xf1c25024) & 0xfff;
		if(data < 1800)
		{
			standby_puts("-------------return 0\n");
			return EPDK_OK;
		}
			
	}
	
////////////
	SelectChannel(3);
	ClearFifo();

	for(i=0;i<3;i++)
	{
		data = STANDBY_RREG(0xf1c25024) & 0xfff;
		if(data < 1800)
		{
			standby_puts("-------------return 0\n");
			return EPDK_OK;
		}

	}
///////////
	SelectChannel(4);
	ClearFifo();

	for(i=0;i<3;i++)
	{
		data = STANDBY_RREG(0xf1c25024) & 0xfff;
		if(data < 1800)
		{
			standby_puts("-------------return 0\n");
			return EPDK_OK;
		}
			
	}

#else
	 if(standby_query_int(INT_SOURCE_TOUCHPNL) == EPDK_TRUE)
	 {
		standby_puts("irq tp\n");
		standby_mdelay(1000);
	 }

#endif

	
		
#if 0
		#if (ALLOW_TPADCKEY_WAKEUP_STANDBY)
		{	
			
			SelectChannel(1);
			ClearFifo();

			for(i=0;i<100;i++)
			{
				data = STANDBY_RREG(0xf1c25024) & 0xfff;	
				pStandbyPar->debug_ad[i] = data;
			}


			pStandbyPar->debug_ad[98] = STANDBY_RREG(0xf1c25000);
			pStandbyPar->debug_ad[99] = STANDBY_RREG(0xf1c25014);
					
			for(i=0;i<100;i++)
			{
				if(pStandbyPar->debug_ad[i]<4000)
					return EPDK_OK;
			}
			
			
									
		
		}
		#endif
#endif		

        // check if alarm comming
        #if (ALLOW_ALARM_WAKEUP_STANDBY)
        if(standby_tmr_query(STANDBY_TMR_ALARM) == EPDK_TRUE)
        {
			pStandbyPar->WakeUpMode = STANDBY_WAKEUPMODE_OTHER;
            // alarm comming
            break;
        }
        #endif
        
        //check if external nmi event coming
        #if (ALLOW_NMI_WAKEUP_STANDBY)
        if(standby_query_int(INT_SOURCE_EXTNMI) == EPDK_TRUE)
        {
            if (standby_power_query(STANDBY_POWER_KEY_SHORT))
		    {
			/*短按普通唤醒*/	
			pStandbyPar->WakeUpMode = STANDBY_WAKEUPMODE_NUMI;
			break;
		    }	
            else if(standby_power_query(STANDBY_POWER_KEY_LONG))
            {
            	/*长按power off*/
				pStandbyPar->WakeUpMode = STANDBY_WAKEUPMODE_NUMI;
                pStandbyPar->EventFlag |= STANDBY_EVENT_POWEROFF;
                break;
            }
            if (standby_power_query(STANDBY_POWER_LOW_POWER))
            {
            	pStandbyPar->WakeUpMode = STANDBY_WAKEUPMODE_NUMI;
                pStandbyPar->EventFlag |= STANDBY_EVENT_LOWPOWER;
                break;
            }
        }
        #endif
		
        //check if ir event coming
        #if (ALLOW_IR_WAKEUP_STANDBY)
        	{
        		static __u8  quit=0;
			if(standby_query_int(INT_SOURCE_IR0) == EPDK_TRUE)
			{
					//standby_puts("-------------INT_SOURCE_IR0\n");
				if(standby_ir_verify() == EPDK_OK)
				{
					//standby_puts("-------------INT_SOURCE_IR0 0k\n");
					pStandbyPar->WakeUpMode = STANDBY_WAKEUPMODE_IR;
					//break;
					quit = 1;
				}
			}
			else
			{
				if(quit == 1)
				{
					quit =0;
					break;
				}				
			}

	}

        #endif




#if 0
		//check if key event coming
        #if (ALLOW_TPADCKEY_WAKEUP_STANDBY)
        if(1)//standby_query_int(INT_SOURCE_TOUCHPNL) == EPDK_TRUE)
        {


			
			ClearFifo();
			adc_val = standby_get_adc_value();
		
            if(adc_val < 4000){
				pStandbyPar->WakeUpMode = 10;
				return EPDK_OK;
			}

			

			SelectChannel(2);
			ClearFifo();
			adc_val = standby_get_adc_value();
		
            if(adc_val < 4000){
				pStandbyPar->WakeUpMode = 11;
				return EPDK_OK;
			}

	#if 0
			SelectChannel(3);
			ClearFifo();
			adc_val = standby_get_adc_value();
		
            if(adc_val < 4000){
				pStandbyPar->WakeUpMode = 12;
				return EPDK_OK;
			}


			SelectChannel(4);
			ClearFifo();
			adc_val = standby_get_adc_value();
		
            if(adc_val < 4000){
				pStandbyPar->WakeUpMode = 13;
				return EPDK_OK;
			}
	#endif
		
        }
        #endif
#endif

		#if(ALLOW_SD_WAKEUP_STANDBY) 
		if(standby_query_int(INT_SOURCE_SDMMC0) == EPDK_TRUE)
		{
			if(standby_query_sd_event() == EPDK_TRUE)
			{
				pStandbyPar->WakeUpMode = STANDBY_WAKEUPMODE_SDMMC;
				break;
			}
		}
		#endif	

	
        
    	//check if usb event coming
	    #if (ALLOW_USB_WAKEUP_STANDBY)
	    if(standby_query_usb_event() == EPDK_TRUE)
	    {
	       pStandbyPar->WakeUpMode = STANDBY_WAKEUPMODE_USB;
	       break;
	    }
	    #endif
        
        //invalid interrupt event coming, 
        //just ignore this type interrupt event,
        //wait for valid event continuly.
    }

    //valid wakeup event coming.
    return EPDK_OK;
}

