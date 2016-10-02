/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                             	Standby system
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : standby_clock.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-2-22
* Descript: clock operations for standby.
* Update  : date                auther      ver     notes
*           2012-2-22 14:07:58	Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "standby_i.h"
#include "standby_clock_i.h"

static __ccmu_reg_list_t *CmuReg;;
static __u32 ccu_reg_back[7];
__u32	cpu_ms_loopcnt;


static  __u32 ccu_reg_gate_back[4];


//static __u32 ccu_reg_ir_clk;


/*
*********************************************************************************************************
*                           standby_clk_init
*
*Description: ccu init for platform standby
*
*Arguments  : none
*
*Return     : result,
*
*Notes      :
*
*********************************************************************************************************
*/
__s32 standby_clk_init(void)
{
    CmuReg = (__ccmu_reg_list_t *)STANDBY_CCU_REG_BASE;

    //backup pll registers
    ccu_reg_back[0] = *(volatile __u32 *)&CmuReg->Pll1Ctl;
    ccu_reg_back[1] = *(volatile __u32 *)&CmuReg->Pll2Ctl;
    ccu_reg_back[2] = *(volatile __u32 *)&CmuReg->Pll3Ctl;
    ccu_reg_back[3] = *(volatile __u32 *)&CmuReg->Pll4Ctl;
    ccu_reg_back[4] = *(volatile __u32 *)&CmuReg->Pll5Ctl;
    ccu_reg_back[5] = *(volatile __u32 *)&CmuReg->Pll6Ctl;
    ccu_reg_back[6] = *(volatile __u32 *)&CmuReg->Pll7Ctl;



	//ccu_reg_ir_clk = *(volatile __u32 *)&CmuReg->Ir0Clk;

	//CmuReg->Ir0Clk.SpecClkGate = 0;
	//CmuReg->Pll6Ctl.PLLEn = 0;
	//CmuReg->VeClk.SpecClkGate = 0;
    //cpu frequency is 60mhz now
    cpu_ms_loopcnt = 3000;
	
    return 0;
}

 __s32 gate_clock_init(void)
{
#if 0
	ccu_reg_gate_back[0] =  *(volatile __u32 *)&CmuReg->AhbGate0;
	ccu_reg_gate_back[0] =  *(volatile __u32 *)&CmuReg->AhbGate1;
	ccu_reg_gate_back[0] =  *(volatile __u32 *)&CmuReg->Apb0Gate;
	ccu_reg_gate_back[0] =  *(volatile __u32 *)&CmuReg->Apb1Gate;

	CmuReg->AhbGate0.SsGate = 0;
	CmuReg->AhbGate0.SdramGate = 0;
	CmuReg->AhbGate0.SdramDll = 0;
	CmuReg->AhbGate0.Spi0Gate = 0;

	CmuReg->AhbGate1.Tve0Gate = 0;
	CmuReg->AhbGate1.Lcd0Gate = 0;
	CmuReg->AhbGate1.Csi0Gate = 0;
	CmuReg->AhbGate1.DeBe0Gate = 0;
	CmuReg->AhbGate1.DeFe0Gate = 0;
	CmuReg->AhbGate1.IepGate = 0;

	//CmuReg->Apb0Gate.AddaGate = 0;
	//CmuReg->Apb0Gate.PioGate = 0;

	CmuReg->Apb1Gate.Twi1Gate = 0;
	//CmuReg->Apb1Gate.Uart3Gate = 0;
#endif

}

  __s32 gate_clock_exit(void)
{
	 //*(volatile __u32 *)&CmuReg->AhbGate0 = ccu_reg_gate_back[0]  ;
	// *(volatile __u32 *)&CmuReg->AhbGate1 = ccu_reg_gate_back[1];
	 //*(volatile __u32 *)&CmuReg->Apb0Gate = ccu_reg_gate_back[2];
	 //*(volatile __u32 *)&CmuReg->Apb1Gate = ccu_reg_gate_back[3];

	 //standby_mdelay(1000);
}


/*
*********************************************************************************************************
*                           standby_clk_exit
*
*Description: ccu exit for platform standby
*
*Arguments  : none
*
*Return     : result,
*
*Notes      :
*
*********************************************************************************************************
*/
__s32 standby_clk_exit(void)
{
    //restore pll registers
    *(volatile __u32 *)&CmuReg->Pll1Ctl = ccu_reg_back[0];
    *(volatile __u32 *)&CmuReg->Pll2Ctl = ccu_reg_back[1];
    *(volatile __u32 *)&CmuReg->Pll3Ctl = ccu_reg_back[2];
    *(volatile __u32 *)&CmuReg->Pll4Ctl = ccu_reg_back[3];
   // *(volatile __u32 *)&CmuReg->Pll5Ctl = ccu_reg_back[4];
    *(volatile __u32 *)&CmuReg->Pll6Ctl = ccu_reg_back[5];
    *(volatile __u32 *)&CmuReg->Pll7Ctl = ccu_reg_back[6];


	//CmuReg->Ir0Clk.SpecClkGate = 1;
    return 0;
}


/*
*********************************************************************************************************
*                                     standby_clk_core2losc
*
* Description: switch core clock to 32k low osc.
*
* Arguments  : none
*
* Returns    : 0;
*********************************************************************************************************
*/
__s32 standby_clk_core2losc(void)
{
    CmuReg->SysClkDiv.AC328ClkSrc = 0;
    
    //cpu frequency is 32k hz
    cpu_ms_loopcnt = 1;

    return 0;
}


/*
*********************************************************************************************************
*                                     standby_clk_core2hosc
*
* Description: switch core clock to 24M high osc.
*
* Arguments  : none
*
* Returns    : 0;
*********************************************************************************************************
*/
__s32 standby_clk_core2hosc(void)
{
    CmuReg->SysClkDiv.AC328ClkSrc = 1;
    CmuReg->AxiGate.SdramGate = 0;
    
    //cpu frequency is 24M hz
    cpu_ms_loopcnt = 600;

    return 0;
}


/*
*********************************************************************************************************
*                                     standby_clk_core2pll
*
* Description: switch core clock to core pll.
*
* Arguments  : none
*
* Returns    : 0;
*********************************************************************************************************
*/
__s32 standby_clk_core2pll(void)
{
    CmuReg->SysClkDiv.AC328ClkSrc = 2;
    
    //cpu frequency is 60M hz
    cpu_ms_loopcnt = 2000;

    return 0;
}


/*
*********************************************************************************************************
*                                     standby_clk_plldisable
*
* Description: disable dram pll.
*
* Arguments  : none
*
* Returns    : 0;
*********************************************************************************************************
*/
__s32 standby_clk_plldisable(void)
{
    CmuReg->Pll1Ctl.PLLEn = 0;
    CmuReg->Pll2Ctl.PLLEn = 0;
    CmuReg->Pll3Ctl.PLLEn = 0;
    CmuReg->Pll4Ctl.PLLEn = 0;
    CmuReg->Pll5Ctl.PLLEn = 0;
    CmuReg->Pll6Ctl.PLLEn = 0;
    CmuReg->Pll7Ctl.PLLEn = 0;

    return 0;
}


/*
*********************************************************************************************************
*                                     standby_clk_pllenable
*
* Description: enable dram pll.
*
* Arguments  : none
*
* Returns    : 0;
*********************************************************************************************************
*/
__s32 standby_clk_pllenable(void)
{
    CmuReg->Pll1Ctl.PLLEn = 1;
    CmuReg->Pll2Ctl.PLLEn = 1;
    CmuReg->Pll3Ctl.PLLEn = 1;
    CmuReg->Pll4Ctl.PLLEn = 1;
    CmuReg->Pll5Ctl.PLLEn = 1;
    CmuReg->Pll6Ctl.PLLEn = 1;
    CmuReg->Pll7Ctl.PLLEn = 1;

    return 0;
}


/*
*********************************************************************************************************
*                                     standby_clk_hoscdisable
*
* Description: disable HOSC.
*
* Arguments  : none
*
* Returns    : 0;
*********************************************************************************************************
*/
__s32 standby_clk_hoscdisable(void)
{
    CmuReg->HoscCtl.OSC24MEn = 0;
    return 0;
}


/*
*********************************************************************************************************
*                                     standby_clk_hoscenable
*
* Description: enable HOSC.
*
* Arguments  : none
*
* Returns    : 0;
*********************************************************************************************************
*/
__s32 standby_clk_hoscenable(void)
{
    CmuReg->HoscCtl.OSC24MEn = 1;
    return 0;
}


/*
*********************************************************************************************************
*                                     standby_clk_ldodisable
*
* Description: disable LDO.
*
* Arguments  : none
*
* Returns    : 0;
*********************************************************************************************************
*/
__s32 standby_clk_ldodisable(void)
{
    CmuReg->HoscCtl.KeyField = 0x538;
    CmuReg->HoscCtl.LDOEn = 0;
    CmuReg->Pll5Ctl.LDO2En = 0;
    CmuReg->HoscCtl.KeyField = 0x00;
    return 0;
}


/*
*********************************************************************************************************
*                                     standby_clk_ldoenable
*
* Description: enable LDO.
*
* Arguments  : none
*
* Returns    : 0;
*********************************************************************************************************
*/
__s32 standby_clk_ldoenable(void)
{
    CmuReg->HoscCtl.KeyField = 0x538;
    CmuReg->HoscCtl.LDOEn = 1;
    CmuReg->Pll5Ctl.LDO2En = 1;
    CmuReg->HoscCtl.KeyField = 0x00;
    return 0;
}


/*
*********************************************************************************************************
*                                     standby_clk_setdiv
*
* Description: switch core clock to 32k low osc.
*
* Arguments  : none
*
* Returns    : 0;
*********************************************************************************************************
*/
__s32 standby_clk_setdiv(struct sun4i_clk_div_t  *clk_div)
{
    if(!clk_div)
    {
        return -1;
    }

    CmuReg->SysClkDiv.AXIClkDiv = clk_div->axi_div;
    CmuReg->SysClkDiv.AHBClkDiv = clk_div->ahb_div;
    CmuReg->SysClkDiv.APB0ClkDiv = clk_div->apb_div;
    CmuReg->SysClkDiv.AHBClkSel  = clk_div->ahb_src;

    return 0;
}


/*
*********************************************************************************************************
*                                     standby_clk_getdiv
*
* Description: switch core clock to 32k low osc.
*
* Arguments  : none
*
* Returns    : 0;
*********************************************************************************************************
*/
__s32 standby_clk_getdiv(struct sun4i_clk_div_t  *clk_div)
{
    if(!clk_div)
    {
        return -1;
    }

    clk_div->axi_div = CmuReg->SysClkDiv.AXIClkDiv;
    clk_div->ahb_div = CmuReg->SysClkDiv.AHBClkDiv;
    clk_div->apb_div = CmuReg->SysClkDiv.APB0ClkDiv;
    clk_div->ahb_src = CmuReg->SysClkDiv.AHBClkSel;
    return 0;
}

/*
*********************************************************************************************************
*                                     standby_clk_set_pll_factor
*
* Description: set pll factor, target cpu freq is 384M hz
*
* Arguments  : none
*
* Returns    : 0;
*********************************************************************************************************
*/

__s32 standby_clk_set_pll_factor(struct pll_factor_t *pll_factor)
{
    if(!pll_factor)
    {
        return -1;
    }

	CmuReg->Pll1Ctl.FactorN = pll_factor->FactorN;
	CmuReg->Pll1Ctl.FactorK = pll_factor->FactorK;
	CmuReg->Pll1Ctl.FactorM = pll_factor->FactorM;
	CmuReg->Pll1Ctl.PLLDivP = pll_factor->FactorP;
	
	//busy_waiting();
	
	return 0;
}

/*
*********************************************************************************************************
*                                     standby_clk_get_pll_factor
*
* Description: 
*
* Arguments  : none
*
* Returns    : 0;
*********************************************************************************************************
*/

__s32 standby_clk_get_pll_factor(struct pll_factor_t *pll_factor)
{
    if(!pll_factor)
    {
        return -1;
    }

	pll_factor->FactorN = CmuReg->Pll1Ctl.FactorN;
	pll_factor->FactorK = CmuReg->Pll1Ctl.FactorK;
	pll_factor->FactorM = CmuReg->Pll1Ctl.FactorM;
	pll_factor->FactorP = CmuReg->Pll1Ctl.PLLDivP;
	
	//busy_waiting();
	
	return 0;
}

/*
*********************************************************************************************************
*                                     standby_clk_dramgating
*
* Description: gating dram clock.
*
* Arguments  : onoff    dram clock gating on or off;
*
* Returns    : 0;
*********************************************************************************************************
*/
void standby_clk_dramgating(int onoff)
{
    if(onoff) {
        CmuReg->Pll5Ctl.OutputEn = 1;
    }
    else {
        CmuReg->Pll5Ctl.OutputEn = 0;
    }
}


/*
*********************************************************************************************************
*                                     standby_clk_apb2losc
*
* Description: switch apb1 clock to 32k low osc.
*
* Arguments  : none
*
* Returns    : 0;
*********************************************************************************************************
*/
__s32 standby_clk_apb2losc(void)
{
    CmuReg->Apb1ClkDiv.ClkSrc = 2;
    return 0;
}


/*
*********************************************************************************************************
*                                     standby_clk_apb2hosc
*
* Description: switch apb1 clock to 24M hosc.
*
* Arguments  : none
*
* Returns    : 0;
*********************************************************************************************************
*/
__s32 standby_clk_apb2hosc(void)
{
    CmuReg->Apb1ClkDiv.ClkSrc = 0;
    return 0;
}


static __ccmu_apb1clk_ratio_reg0058_t  apbclkbak;

/*
*********************************************************************************************************
*                                     standby_clk_apb2hosc
*
* Description: switch apb1 clock to 24M hosc.
*
* Arguments  : none
*
* Returns    : 0;
*********************************************************************************************************
*/
__s32 standby_clk_apbinit(void)
{
    apbclkbak = CmuReg->Apb1ClkDiv;
    /* change apb1 clock to hosc */
    CmuReg->Apb1ClkDiv.ClkSrc = 0;
    CmuReg->Apb1ClkDiv.ClkDiv = 0;
    CmuReg->Apb1ClkDiv.PreDiv = 0;
    return 0;
}


/*
*********************************************************************************************************
*                                     standby_clk_apb2hosc
*
* Description: switch apb1 clock to 24M hosc.
*
* Arguments  : none
*
* Returns    : 0;
*********************************************************************************************************
*/
__s32 standby_clk_apbexit(void)
{
    /* restore clock division */
    CmuReg->Apb1ClkDiv.ClkDiv = apbclkbak.ClkDiv;
    CmuReg->Apb1ClkDiv.PreDiv = apbclkbak.PreDiv;
    /* restore clock source */
    CmuReg->Apb1ClkDiv.ClkSrc = apbclkbak.ClkSrc;
    return 0;
}

