/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                             	Standby system
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : standby_int.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-2-22
* Descript: interrupt operations for standby.
* Update  : date                auther      ver     notes
*           2012-2-22 13:15:34	Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "standby_i.h"

static __u32 IrqEnReg[3];
static __u32 IrqMaskReg[3];
static __u32 IrqSelReg[3];
struct standby_int_reg_t *IntcReg;

/*
*********************************************************************************************************
*                                       STANDBY INTERRUPT INITIALISE
*
* Description: standby interrupt initialise.
*
* Arguments  : none.
*
* Returns    : 0/-1;
*********************************************************************************************************
*/
__s32 standby_int_init(void)
{
    IntcReg = (struct standby_int_reg_t *)STANDBY_INT_REG_BASE;

    //save interrupt controller registers
    IrqEnReg[0] 	= IntcReg->IrqEn[0];
    IrqEnReg[1] 	= IntcReg->IrqEn[1];
    IrqEnReg[2] 	= IntcReg->IrqEn[2];
    IrqMaskReg[0] 	= IntcReg->IrqMask[0];
    IrqMaskReg[1] 	= IntcReg->IrqMask[1];
    IrqMaskReg[2] 	= IntcReg->IrqMask[2];
    IrqSelReg[0] 	= IntcReg->TypeSel[0];
    IrqSelReg[1] 	= IntcReg->TypeSel[1];
    IrqSelReg[2] 	= IntcReg->TypeSel[2];

    //initialise interrupt enable and mask for standby
    IntcReg->IrqEn[0] 	= 0;
    IntcReg->IrqEn[1] 	= 0;
    IntcReg->IrqEn[2] 	= 0;
    IntcReg->IrqMask[0] = 0xffffffff;
    IntcReg->IrqMask[1] = 0xffffffff;
    IntcReg->IrqMask[2] = 0xffffffff;
    IntcReg->TypeSel[0] = 0;
    IntcReg->TypeSel[1] = 0;
    IntcReg->TypeSel[2] = 0;

    //clear external irq pending
   // IntcReg->IrqPend[0] = 1;  //1625 read only
   // IntcReg->IrqPend[1] = 1;
   // IntcReg->IrqPend[2] = 1;


    return 0;
}


/*
*********************************************************************************************************
*                                       STANDBY INTERRUPT INITIALISE
*
* Description: standby interrupt initialise.
*
* Arguments  : none.
*
* Returns    : 0/-1;
*********************************************************************************************************
*/
__s32 standby_int_exit(void)
{
    //restore interrupt registers
    IntcReg->IrqEn[0] 	= IrqEnReg[0];
    IntcReg->IrqEn[1] 	= IrqEnReg[1];
    IntcReg->IrqEn[2] 	= IrqEnReg[2];
    IntcReg->IrqMask[0] = IrqMaskReg[0];
    IntcReg->IrqMask[1] = IrqMaskReg[1];
    IntcReg->IrqMask[2] = IrqMaskReg[2];
    IntcReg->TypeSel[0] = IrqSelReg[0];
    IntcReg->TypeSel[1] = IrqSelReg[1];
    IntcReg->TypeSel[2] = IrqSelReg[2];

    return 0;
}


/*
*********************************************************************************************************
*                                       QUERY INTERRUPT
*
* Description: query interrupt.
*
* Arguments  : src  interrupt source number.
*
* Returns    : 0/-1;
*********************************************************************************************************
*/
__s32 standby_enable_int(enum interrupt_source_e src)
{
    __u32   tmpGrp = (__u32)src >> 5;
    __u32   tmpSrc = (__u32)src & 0x1f;

    //enable interrupt source
    IntcReg->IrqEn[tmpGrp] 	 |=  (1<<tmpSrc);
    IntcReg->IrqMask[tmpGrp] &= ~(1<<tmpSrc);
    IntcReg->IrqPend[tmpGrp] |=  (1<<tmpSrc);
    return 0;
}


/*
*********************************************************************************************************
*                                       disable INTERRUPT
*
* Description: disable interrupt.
*
* Arguments  : src  interrupt source number.
*
* Returns    : 0/-1;
*********************************************************************************************************
*/
__s32 standby_disable_int(enum interrupt_source_e src)
{
    __u32   tmpGrp = (__u32)src >> 5;
    __u32   tmpSrc = (__u32)src & 0x1f;

    //enable interrupt source
    IntcReg->IrqEn[tmpGrp] 	 &=  ~(1<<tmpSrc);
    IntcReg->IrqMask[tmpGrp] |= (1<<tmpSrc);
    IntcReg->IrqPend[tmpGrp] &=  ~(1<<tmpSrc);
    return 0;
}



/*
*********************************************************************************************************
*                                       QUERY INTERRUPT
*
* Description: query interrupt.
*
* Arguments  : src  interrupt source number.
*
* Returns    : 0/-1;
*********************************************************************************************************
*/
__s32 standby_query_int(enum interrupt_source_e src)
{
    __s32   result = 0;
    __u32   tmpGrp = (__u32)src >> 5;
    __u32   tmpSrc = (__u32)src & 0x1f;

    result = IntcReg->IrqPend[tmpGrp] & (1<<tmpSrc);

    return result? EPDK_TRUE:EPDK_FALSE;
}

