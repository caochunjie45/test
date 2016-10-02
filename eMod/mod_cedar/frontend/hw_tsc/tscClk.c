
#include "tscClk.h"

static __krnl_event_t       *pSemClkAdjust;
static __hdle               hTscClk, hDramTscClk, hTscAhbClk;


#if 0
/*
*********************************************************************************************************
*                           CALL-BACK FUNCTION - CHANGE MACC SOURCE CLOCK
*
* Description: change mpeg macc source clock, call-back fuction;
*
* Arguments  : cmd      command for change clock,request change or change done;
*              aux      new source clock value;
*
* Returns    : EPDK_OK/FAIL
*********************************************************************************************************
*/
static __s32 cb_TSC_ClockChange(__u32 cmd, __s32 aux)
{
    switch(cmd)
    {
        case CLK_CMD_SCLKCHG_REQ:
       {
            __u8 err;

            //request semphore to check if it can adjust frequence now
            esKRNL_SemPend(pSemClkAdjust, 0, &err);
            return EPDK_OK;
        }

        case CLK_CMD_SCLKCHG_DONE:
        {
            //release semphore
            esKRNL_SemPost(pSemClkAdjust);
            return EPDK_OK;
        }

        default:
            return EPDK_FAIL;
    }
}
#endif

/*
*********************************************************************************************************
*                                       OPEN TSC CLOCK MODULE
*
* Description: This function open the macc clock module, register the macc module to clock management;
*
* Arguments  : none;
*
* Returns    : EPDK_OK/FAIL
*********************************************************************************************************
*/
__s32 TSC_OpenClk(void)
{
    //create semphore to sync adjust clock operation
    pSemClkAdjust = esKRNL_SemCreate(1);
    if(!pSemClkAdjust)
    {
        return EPDK_FAIL;
    }

    //* open tsc module clock
    //hTscClk = esCLK_OpenMclk(CSP_CCM_MOD_CLK_TS);
		hTscClk = esCLK_OpenMclk(CCMU_MOD_CLK_TS);
    if(!hTscClk)
    {
        return EPDK_FAIL;
    }
    
//    esCLK_MclkRegCb(CSP_CCM_MOD_CLK_TS, cb_TSC_ClockChange);

    //* open tsc ahb clock gate
    //hTscAhbClk = esCLK_OpenMclk(CSP_CCM_MOD_CLK_AHB_TS);
    hTscAhbClk = esCLK_OpenMclk(CCMU_MOD_CLK_AHB_TS);
    if(!hTscAhbClk)
    {
        return EPDK_FAIL;
    }

    //* open tsc dram clock gate
    //hDramTscClk = esCLK_OpenMclk(CSP_CCM_MOD_CLK_SDRAM_TS);
		hDramTscClk = esCLK_OpenMclk(CCMU_MOD_CLK_SDRAM_TS);
    if(!hDramTscClk)
    {
        return EPDK_FAIL;
    }
    
    //set tsc clock source
    if(esCLK_SetMclkSrc(hTscClk, TSC_CLOCK_SOURCE) != EPDK_OK)
    {
        TSC_CloseClk();
        return EPDK_FAIL;
    }
    
    if(esCLK_SetMclkDiv(hTscClk, TSC_CLOCK_DIVID_RATIO) != EPDK_OK)
    {
        TSC_CloseClk();
        return EPDK_FAIL;
    }
    
    esCLK_MclkOnOff(hTscClk, CLK_ON);
    esCLK_MclkOnOff(hTscAhbClk, CLK_ON);
    esCLK_MclkOnOff(hDramTscClk, CLK_ON);

    return EPDK_OK;
}

/*
*********************************************************************************************************
*                                       CLOSE TSC CLOCK MODULE
*
* Description: This function close the mpeg acc clock module, unregister the mpeg acc
*              module from clock management;
*
* Arguments  : none;
*
* Returns    : EPDK_OK/FAIL
*********************************************************************************************************
*/
__s32 TSC_CloseClk(void)
{
    __u8        err;

    //unregister ve module clock
    if(hTscClk)
    {
        esCLK_MclkOnOff(hTscClk, CLK_OFF);
        esCLK_CloseMclk(hTscClk);
        hTscClk = 0;
    }
    
    //unregister macc ahb clock
    if(hTscAhbClk)
    {
        esCLK_MclkOnOff(hTscAhbClk, CLK_OFF);
        esCLK_CloseMclk(hTscAhbClk);
        hTscAhbClk = 0;
    }
    
    //unregister macc module clock
    if(hDramTscClk)
    {
        esCLK_MclkOnOff(hDramTscClk, CLK_OFF);
        esCLK_CloseMclk(hDramTscClk);
        hDramTscClk = 0;
    }

    //delete semphore
    if(pSemClkAdjust)
    {
        esKRNL_SemDel(pSemClkAdjust, OS_DEL_ALWAYS, &err);
        pSemClkAdjust = 0;
    }

    return EPDK_OK;
}

