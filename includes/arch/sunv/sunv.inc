;/*
;*********************************************************************************************************
;*                                                    MELIS
;*                                    the Easy Portable/Player Develop Kits
;*                                            Sunv Registers Base
;*
;*                                    (c) Copyright 2011-2014, Sunny China
;*                                             All Rights Reserved
;*
;* File    : sunv.inc
;* By      : Sunny
;* Version : v1.0
;* Date    : 2011-11-24
;* Descript: the controller register address for sunv(1623).
;* Update  : date                auther      ver     notes
;*           2011-11-24 9:07:39  Sunny       1.0     Create this file.
;*********************************************************************************************************
;*/

    IF :LNOT::DEF:__SUNV_INC__
    GBLA  __SUNV_INC__


    ;//------------------------------------------------------------------------------
    ;// Controllers register phyical base address
    ;//------------------------------------------------------------------------------
SRAMC_REG_BASE 		 equ   (0x01c00000)
DRAMC_REG_BASE 		 equ   (0x01c01000)
DMA_REG_BASE 		 equ   (0x01c02000)
NFC_REG_BASE 		 equ   (0x01c03000)
TS_REG_BASE 		 equ   (0x01c04000)
SPI0_REG_BASE 		 equ   (0x01c05000)
SPI1_REG_BASE 		 equ   (0x01c06000)
MS_REG_BASE 		 equ   (0x01c07000)
TVD_REG_BASE 		 equ   (0x01c08000)
CSI0_REG_BASE 		 equ   (0x01c09000)
TVE0_REG_BASE 		 equ   (0x01c0a000)
EMAC_REG_BASE 		 equ   (0x01c0b000)
LCD0_REG_BASE 		 equ   (0x01c0c000)
LCD1_REG_BASE 		 equ   (0x01c0d000)
VE_REG_BASE 		 equ   (0x01c0e000)
SDMMC0_REG_BASE      equ   (0x01c0f000)
SDMMC1_REG_BASE      equ   (0x01c10000)
SDMMC2_REG_BASE      equ   (0x01c11000)
SDMMC3_REG_BASE      equ   (0x01c12000)
USB0_REG_BASE        equ   (0x01c13000)
USB1_REG_BASE        equ   (0x01c14000)
SS_REG_BASE          equ   (0x01c15000)
HDMI_REG_BASE        equ   (0x01c16000)
SPI2_REG_BASE        equ   (0x01c17000)
SATA_REG_BASE        equ   (0x01c18000)
PATA_REG_BASE        equ   (0x01c19000)
ACE_REG_BASE         equ   (0x01c1a000)
TVE1_REG_BASE        equ   (0x01c1b000)
USB2_REG_BASE        equ   (0x01c1c000)
CSI1_REG_BASE        equ   (0x01c1d000)
TZASC_REG_BASE       equ   (0x01c1e000)
SP3_REG_BASE         equ   (0x01c1f000)
CCM_REG_BASE         equ   (0x01c20000)
INTC_REG_BASE        equ   (0x01c20400)
PIN_REG_BASE         equ   (0x01c20800)
TIMER_REG_BASE       equ   (0x01c20c00)
SPDIF_REG_BASE       equ   (0x01c21000)
AC97_REG_BASE        equ   (0x01c21400)
IR0_REG_BASE         equ   (0x01c21800)
IR1_REG_BASE         equ   (0x01c21c00)
IIS_REG_BASE         equ   (0x01c22400)
LRADC_REG_BASE       equ   (0x01c22800)
ADDA_REG_BASE        equ   (0x01c22c00)
KEYPAD_REG_BASE      equ   (0x01c23000)
TZPC_REG_BASE        equ   (0x01c23400)
SID_REG_BASE         equ   (0x01c23800)
SJTAG_REG_BASE       equ   (0x01c23c00)
TP_REG_BASE          equ   (0x01c25000)
PMU_REG_BASE         equ   (0x01c25400)
UART0_REG_BASE       equ   (0x01c28000)
UART1_REG_BASE       equ   (0x01c28400)
UART2_REG_BASE       equ   (0x01c28800)
UART3_REG_BASE       equ   (0x01c28c00)
UART4_REG_BASE       equ   (0x01c29000)
UART5_REG_BASE       equ   (0x01c29400)
UART6_REG_BASE       equ   (0x01c29800)
UART7_REG_BASE       equ   (0x01c29c00)
PS20_REG_BASE        equ   (0x01c2a000)
PS21_REG_BASE        equ   (0x01c2a400)
TWI0_REG_BASE        equ   (0x01c2ac00)
TWI1_REG_BASE        equ   (0x01c2b000)
TWI2_REG_BASE        equ   (0x01c2b400)
CAN_REG_BASE         equ   (0x01c2bc00)
SCR_REG_BASE         equ   (0x01c2c400)
GPS_REG_BASE         equ   (0x01c30000)
MALI400_REG_BASE     equ   (0x01c40000)
SRAM_C_REG_BASE      equ   (0x01d00000)
DE_FE0_REG_BASE      equ   (0x01e00000)
DE_FE1_REG_BASE      equ   (0x01e20000)
DE_BE0_REG_BASE      equ   (0x01e60000)
DE_BE1_REG_BASE      equ   (0x01e40000)
MP_REG_BASE          equ   (0x01e80000)
AVG_REG_BASE         equ   (0x01ea0000)

;//------------------------------------------------------------------------------
;// DRAM physical base address
;//------------------------------------------------------------------------------
DRAM_pBASE          equ   (0x40000000)
    
    ENDIF     ;// IF :LNOT::DEF:__SUNV_INC__

    END       ;// end of sunv.inc
