/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                            Sunv Registers Base
*
*                                    (c) Copyright 2011-2014, Sunny China
*                                             All Rights Reserved
*
* File    : sunv.h
* By      : Sunny
* Version : v1.0
* Date    : 2011-11-14
* Descript: the controller register address for sunv(1623).
* Update  : date                auther      ver     notes
*           2011-11-14 13:30:24 Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __SUNV_H__
#define __SUNV_H__

//------------------------------------------------------------------------------
// Controllers register phyical base address
//------------------------------------------------------------------------------
#define SRAMC_REG_BASE 		    (0x01c00000)
#define DRAMC_REG_BASE 		    (0x01c01000)
#define DMA_REG_BASE 		    (0x01c02000)
#define NFC_REG_BASE 		    (0x01c03000)
#define TS_REG_BASE 		    (0x01c04000)
#define SPI0_REG_BASE 		    (0x01c05000)
#define SPI1_REG_BASE 		    (0x01c06000)
#define MS_REG_BASE 		    (0x01c07000)
#define TVD_REG_BASE 		    (0x01c08000)
#define CSI0_REG_BASE 		    (0x01c09000)
#define TVE0_REG_BASE 		    (0x01c0a000)
#define EMAC_REG_BASE 		    (0x01c0b000)
#define LCD0_REG_BASE 		    (0x01c0c000)
#define LCD1_REG_BASE 		    (0x01c0d000)
#define VE_REG_BASE 		    (0x01c0e000)
#define SDMMC0_REG_BASE         (0x01c0f000)
#define SDMMC1_REG_BASE         (0x01c10000)
#define SDMMC2_REG_BASE         (0x01c11000)
#define SDMMC3_REG_BASE         (0x01c12000)
#define USB0_REG_BASE           (0x01c13000)
#define USB1_REG_BASE           (0x01c14000)
#define SS_REG_BASE             (0x01c15000)
#define HDMI_REG_BASE           (0x01c16000)
#define SPI2_REG_BASE           (0x01c17000)
#define SATA_REG_BASE           (0x01c18000)
#define PATA_REG_BASE           (0x01c19000)
#define ACE_REG_BASE            (0x01c1a000)
#define TVE1_REG_BASE           (0x01c1b000)
#define USB2_REG_BASE           (0x01c1c000)
#define CSI1_REG_BASE           (0x01c1d000)
#define TZASC_REG_BASE          (0x01c1e000)
#define SPI3_REG_BASE           (0x01c1f000)
#define CCM_REG_BASE            (0x01c20000)
#define INTC_REG_BASE           (0x01c20400)
#define PIN_REG_BASE            (0x01c20800)
#define TIMER_REG_BASE          (0x01c20c00)
#define SPDIF_REG_BASE          (0x01c21000)
#define AC97_REG_BASE           (0x01c21400)
#define IR0_REG_BASE            (0x01c21800)
#define IR1_REG_BASE            (0x01c21c00)
#define IIS_REG_BASE            (0x01c22400)
#define LRADC_REG_BASE          (0x01c22800)
#define ADDA_REG_BASE           (0x01c22c00)
#define KEYPAD_REG_BASE         (0x01c23000)
#define TZPC_REG_BASE           (0x01c23400)
#define SID_REG_BASE            (0x01c23800)
#define SJTAG_REG_BASE          (0x01c23c00)

#define TP_REG_BASE             (0x01c25000)

#define PMU_REG_BASE            (0x01c25400)

#define UART0_REG_BASE          (0x01c28000)
#define UART1_REG_BASE          (0x01c28400)
#define UART2_REG_BASE          (0x01c28800)
#define UART3_REG_BASE          (0x01c28c00)
#define UART4_REG_BASE          (0x01c29000)
#define UART5_REG_BASE          (0x01c29400)
#define UART6_REG_BASE          (0x01c29800)
#define UART7_REG_BASE          (0x01c29c00)

#define PS20_REG_BASE           (0x01c2a000)
#define PS21_REG_BASE           (0x01c2a400)

#define TWI0_REG_BASE           (0x01c2ac00)
#define TWI1_REG_BASE           (0x01c2b000)
#define TWI2_REG_BASE           (0x01c2b400)

#define CAN_REG_BASE            (0x01c2bc00)

#define SCR_REG_BASE            (0x01c2c400)

#define GPS_REG_BASE            (0x01c30000)

#define MALI400_REG_BASE        (0x01c40000)

#define SRAM_C_REG_BASE         (0x01d00000)

#define DE_FE0_REG_BASE         (0x01e00000)
#define DE_FE1_REG_BASE         (0x01e20000)
#define DE_BE0_REG_BASE         (0x01e60000)
#define DE_BE1_REG_BASE         (0x01e40000)
#define MP_REG_BASE             (0x01e80000)
#define AVG_REG_BASE            (0x01ea0000)


//------------------------------------------------------------------------------
// DRAM physical base address
//------------------------------------------------------------------------------
#define DRAM_pBASE              (0x40000000)

//------------------------------------------------------------------------------
// Register read and write operations
//------------------------------------------------------------------------------
#define READ_REG8(reg)			(*(volatile unsigned char  *)(reg))
#define READ_REG16(reg)			(*(volatile unsigned short *)(reg))
#define READ_REG32(reg)			(*(volatile unsigned int   *)(reg))

#define WRITE_REG8(Value, reg)	(*(volatile unsigned char  *)(reg) = (Value))
#define WRITE_REG16(Value, reg)	(*(volatile unsigned short *)(reg) = (Value))
#define WRITE_REG32(Value, reg)	(*(volatile unsigned int   *)(reg) = (Value))

//------------------------------------------------------------------------------
// Basic bit operations
//------------------------------------------------------------------------------
#define SET_REG_BIT(val, pos)               ((val) |= 1U<<(pos))
#define CLEAR_REG_BIT(val, pos)             ((val) &= ~(1U<<(pos)))
#define TEST_REG_BIT(val, pos)              ((val) & (1U<<(pos)))
#define BITS_REG_MOD(len)                   ((1U<<(len)) - 1)
#define BITS_REG_MASK(len, pos)             (~(BITS_REG_MOD(len)<<(pos)))
#define CLEAR_REG_BITS(val, pos, len)       (val) &= BITS_REG_MASK(len, pos) 
#define SET_REG_BITS(val, pos, len, _val)   (val) = ((val) & BITS_REG_MASK(len, pos)) | (((_val) & BITS_REG_MOD(len))<<(pos))
#define TEST_REG_BITS(val, pos, len, _val)  ((val) & (~BITS_REG_MASK(len, pos))) == ((_val)<<(pos))
#define GET_REG_BITS_VAL(val, pos, len)     (((val)>>(pos)) & BITS_REG_MOD(len))

#define SRAM_pBASE      0x00000000

//#ifdef  USE_PHYSICAL_ADDRESS
//	#define SRAM_BASE               SRAM_pBASE
//	#define DRAM_BASE               DRAM_pBASE
//#else
	#define SRAM_BASE               SRAM_vBASE
	#define DRAM_BASE               DRAM_vBASE
//#endif    // #ifdef  USE_PHYSICAL_ADDRESS


#endif //__SUNV_H__
