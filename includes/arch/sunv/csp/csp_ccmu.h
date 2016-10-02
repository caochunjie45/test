/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                             Clock  Control Module
*
*                                    (c) Copyright 2011-2014, Sunny China
*                                             All Rights Reserved
*
* File    : csp_ccmu.h
* By      : Sunny
* Version : v1.0
* Date    : 2011-11-17
* Descript: Clock  Control Module interface header file.
* Update  : date                auther      ver     notes
*           2011-11-17 11:28:04 Sunny       1.0     Create this file.
*           2012-10-16 11:28:04 Kingvan     2.0     A13.
*********************************************************************************************************
*/

#ifndef __CSP_CCMU_H__
#define __CSP_CCMU_H__

/* define if need use pll6 to take the place of pll4,
   this definition is significative on C ver. only */
#define USE_PLL6M_REPLACE_PLL4      (1)


/* define clock error type      */
typedef enum __CCMU_ERR
{
    CCMU_ERR_NONE         = 0,
    CCMU_ERR_PARA_NULL    = -1,
    CCMU_ERR_PARA_INVALID = -1,

} __ccmu_err_e;


typedef enum __CCMU_CLK_ONOFF
{
    CCMU__CLK_OFF,
    CCMU__CLK_ON,

} __ccmu_clk_onff_e;

//------------------------------------------------------------------------------
//      Clock reset valid status.
//------------------------------------------------------------------------------
typedef enum __CCMU_CLK_RESET
{
    CCMU_CLK_RESET,
    CCMU_CLK_NRESET,
    
} __ccmu_clk_reset_e;

//------------------------------------------------------------------------------
//      System clocks.
//------------------------------------------------------------------------------
typedef enum __CCMU_SYS_CLK
{
    CCMU_SYS_CLK_NONE,       //invalid system clock number
                            
    CCMU_SYS_CLK_LOSC,       //LOSC, 32768HZ clock
    CCMU_SYS_CLK_HOSC,       //HOSC, 24MHZ clock
    
    CCMU_SYS_CLK_CORE_PLL,      //PLL1      : core pll
    CCMU_SYS_CLK_AUDIO_PLL,     //PLL2      : audio pll
    CCMU_SYS_CLK_AUDIO_PLL_X8,  //PLL2_X8   : audio pll_x8
    CCMU_SYS_CLK_VIDEO0_PLL,    //PLL3      : video0 pll
    CCMU_SYS_CLK_VIDEO0_PLL_X2, //PLL3_X2   : video0 pll_x2
    CCMU_SYS_CLK_VE_PLL,        //PLL4      : ve pll
    CCMU_SYS_CLK_SDRAM_PLL,    //9 //PLL5      : sdram pll
    CCMU_SYS_CLK_SDRAM_PLL_M,   //PLL5_M    : sdram pll_m
    CCMU_SYS_CLK_SDRAM_PLL_P,   //PLL5_P    : sdram pll_p
    CCMU_SYS_CLK_SATA_PLL,      //PLL6      : sata pll, as source of sata_pll_m 
                                //and sata_pll_2, users should not use this clock directly
    CCMU_SYS_CLK_SATA_PLL_M,    //PLL6_M    : sata_pll_m
    CCMU_SYS_CLK_SATA_PLL_2,    //PLL6_P    : sata_pll_2
    
    CCMU_SYS_CLK_VIDEO1_PLL,    //PLL7      : video1 pll1
    CCMU_SYS_CLK_VIDEO1_PLL_X2, //PLL7_X2   : video1 pll1_x2
    CCMU_SYS_CLK_200M,          //200m pll
    
    CCMU_SYS_CLK_CPU,           //cpu
    CCMU_SYS_CLK_AXI,           //axi
    CCMU_SYS_CLK_AHB,           //ahb
    CCMU_SYS_CLK_APB0,          //apb
    CCMU_SYS_CLK_APB1,          //apb1
    
    CCMU_SYS_CLK_CNT            //invalid id, for calc system clock count
    
} __ccu_sys_clk_e;


//------------------------------------------------------------------------------
//      Module clocks.
//------------------------------------------------------------------------------
typedef enum __CCMU_MOD_CLK
{
    CCMU_MOD_CLK_NONE,        //invalid module clock number
    
    CCMU_MOD_CLK_NFC,         //nfc
    CCMU_MOD_CLK_MSC,         //msc
    CCMU_MOD_CLK_SDC0,        //sdc0
    CCMU_MOD_CLK_SDC1,        //sdc1
    CCMU_MOD_CLK_SDC2, //5       //sdc2		
    CCMU_MOD_CLK_SDC3,        //sdc3
    CCMU_MOD_CLK_TS,   //7       //ts
    CCMU_MOD_CLK_SS,          //ss
    CCMU_MOD_CLK_SPI0,        //spi0
    CCMU_MOD_CLK_SPI1, //10       //spi1
    CCMU_MOD_CLK_SPI2,        //spi2
    CCMU_MOD_CLK_PATA,        //pata
    CCMU_MOD_CLK_IR0,         //ir0
    CCMU_MOD_CLK_IR1,         //ir1
    CCMU_MOD_CLK_I2S,  //15       //i2s
    CCMU_MOD_CLK_AC97,        //ac97
    CCMU_MOD_CLK_SPDIF,       //spdif
    CCMU_MOD_CLK_KEYPAD,      //key_pad
    CCMU_MOD_CLK_SATA,        //sata
    CCMU_MOD_CLK_USBPHY, //20     //usb_phy
    CCMU_MOD_CLK_USBPHY0,     //usb_phy0
    CCMU_MOD_CLK_USBPHY1,     //usb_phy1
    CCMU_MOD_CLK_USBPHY2,     //usb_phy2
    CCMU_MOD_CLK_USBOHCI0,    //usb_ohci0
    CCMU_MOD_CLK_USBOHCI1, //25   //usb_ohci1
    CCMU_MOD_CLK_GPS,         //com
    CCMU_MOD_CLK_SPI3,        //spi3
    CCMU_MOD_CLK_DEBE0,       //de_image0
    CCMU_MOD_CLK_DEBE1,       //de_image1
    CCMU_MOD_CLK_DEFE0,   //30    //de_scale0
    CCMU_MOD_CLK_DEFE1,       //de_scale1
    CCMU_MOD_CLK_DEMIX,       //de_mix
    CCMU_MOD_CLK_LCD0CH0,     //lcd0_ch0
    CCMU_MOD_CLK_LCD1CH0,     //lcd1_ch0
    CCMU_MOD_CLK_CSIISP,  //35    //csi_isp
    CCMU_MOD_CLK_TVD,         //tvd
    CCMU_MOD_CLK_LCD0CH1_S1,  //lcd0_ch1_s1
    CCMU_MOD_CLK_LCD0CH1_S2,  //lcd0_ch1_s2
    CCMU_MOD_CLK_LCD1CH1_S1,  //lcd1_ch1_s1
    CCMU_MOD_CLK_LCD1CH1_S2,//40  //lcd1_ch1_s2
    CCMU_MOD_CLK_CSI0,        //csi0
    CCMU_MOD_CLK_CSI1,        //csi1
    CCMU_MOD_CLK_VE,          //ve
    CCMU_MOD_CLK_ADDA,        //audio_codec
    CCMU_MOD_CLK_AVS,   //45      //avs
    CCMU_MOD_CLK_ACE,         //ace
    CCMU_MOD_CLK_LVDS,        //lvds
    CCMU_MOD_CLK_HDMI,        //hdmi
    CCMU_MOD_CLK_MALI,        //mali
    CCMU_MOD_CLK_TWI0,   //50     //twi0
    CCMU_MOD_CLK_TWI1,        //twi1
    CCMU_MOD_CLK_TWI2,        //twi2
    CCMU_MOD_CLK_CAN,         //can
    CCMU_MOD_CLK_SCR,         //scr
    CCMU_MOD_CLK_PS20,  //55      //ps0
    CCMU_MOD_CLK_PS21,        //ps1
    CCMU_MOD_CLK_UART0,       //uart0
    CCMU_MOD_CLK_UART1,       //uart1
    CCMU_MOD_CLK_UART2,       //uart2
    CCMU_MOD_CLK_UART3, //60      //uart3
    CCMU_MOD_CLK_UART4,       //uart4
    CCMU_MOD_CLK_UART5,       //uart5
    CCMU_MOD_CLK_UART6,       //uart6
    CCMU_MOD_CLK_UART7,       //uart7
    
    //clock gating for hang to AXI bus
    CCMU_MOD_CLK_AXI_DRAM,//65    //axi_dram
    
    //clock gating for hang to AHB bus
    CCMU_MOD_CLK_AHB_USB0,    //ahb_usb0
    CCMU_MOD_CLK_AHB_EHCI0,   //ahb_ehci0
    CCMU_MOD_CLK_AHB_OHCI0,   //ahb_ohci0
    CCMU_MOD_CLK_AHB_SS,      //ahb_ss
    CCMU_MOD_CLK_AHB_DMA,     //ahb_dma
    CCMU_MOD_CLK_AHB_BIST,    //ahb_bist
    CCMU_MOD_CLK_AHB_SDMMC0,  //ahb_sdc0
    CCMU_MOD_CLK_AHB_SDMMC1,  //ahb_sdc1
    CCMU_MOD_CLK_AHB_SDMMC2,  //ahb_sdc2
    CCMU_MOD_CLK_AHB_SDMMC3,  //ahb_sdc3
    CCMU_MOD_CLK_AHB_MS,      //ahb_msc
    CCMU_MOD_CLK_AHB_NAND,    //ahb_nfc
    CCMU_MOD_CLK_AHB_SDRAM,   //ahb_sdramc
    CCMU_MOD_CLK_AHB_ACE,     //ahb_ace
    CCMU_MOD_CLK_AHB_EMAC,    //ahb_emac
    CCMU_MOD_CLK_AHB_TS,      //ahb_ts
    CCMU_MOD_CLK_AHB_SPI0,    //ahb_spi0
    CCMU_MOD_CLK_AHB_SPI1,    //ahb_spi1
    CCMU_MOD_CLK_AHB_SPI2,    //ahb_spi2
    CCMU_MOD_CLK_AHB_SPI3,    //ahb_spi3
    CCMU_MOD_CLK_AHB_PATA,    //ahb_pata
    CCMU_MOD_CLK_AHB_SATA,    //ahb_sata
    CCMU_MOD_CLK_AHB_GPS,     //ahb_com
    CCMU_MOD_CLK_AHB_VE,      //ahb_ve
    CCMU_MOD_CLK_AHB_TVD,     //ahb_tvd
    CCMU_MOD_CLK_AHB_TVE0,    //ahb_tve0
    CCMU_MOD_CLK_AHB_TVE1,    //ahb_tve1
    CCMU_MOD_CLK_AHB_LCD0,    //ahb_lcd0
    CCMU_MOD_CLK_AHB_LCD1,    //ahb_lcd1
    CCMU_MOD_CLK_AHB_CSI0,    //ahb_csi0
    CCMU_MOD_CLK_AHB_CSI1,    //ahb_csi1
    CCMU_MOD_CLK_AHB_HDMI,    //ahb_hdmi
    CCMU_MOD_CLK_AHB_DEBE0,   //ahb_de_image0
    CCMU_MOD_CLK_AHB_DEBE1,   //ahb_de_image1
    CCMU_MOD_CLK_AHB_DEFE0,   //ahb_de_scale0
    CCMU_MOD_CLK_AHB_DEFE1,   //ahb_de_scale1
    CCMU_MOD_CLK_AHB_MP,      //ahb_de_mix
    CCMU_MOD_CLK_AHB_MALI,    //ahb_mali
    
    //clock gating for hang APB bus
    CCMU_MOD_CLK_APB_ADDA,    //apb_audio_codec
    CCMU_MOD_CLK_APB_SPDIF,   //apb_spdif
    CCMU_MOD_CLK_APB_AC97,    //apb_ac97
    CCMU_MOD_CLK_APB_I2S,     //apb_i2s
    CCMU_MOD_CLK_APB_PIO,     //apb_pio
    CCMU_MOD_CLK_APB_IR0,     //apb_ir0
    CCMU_MOD_CLK_APB_IR1,     //apb_ir1
    CCMU_MOD_CLK_APB_KEYPAD,  //apb_key_pad
    CCMU_MOD_CLK_APB_TWI0,    //apb_twi0
    CCMU_MOD_CLK_APB_TWI1,    //apb_twi1
    CCMU_MOD_CLK_APB_TWI2,    //apb_twi2
    CCMU_MOD_CLK_APB_CAN,     //apb_can
    CCMU_MOD_CLK_APB_SCR,     //apb_scr
    CCMU_MOD_CLK_APB_PS20,    //apb_ps0
    CCMU_MOD_CLK_APB_PS21,    //apb_ps1
    CCMU_MOD_CLK_APB_UART0,   //apb_uart0
    CCMU_MOD_CLK_APB_UART1,   //apb_uart1
    CCMU_MOD_CLK_APB_UART2,   //apb_uart2
    CCMU_MOD_CLK_APB_UART3,   //apb_uart3
    CCMU_MOD_CLK_APB_UART4,   //apb_uart4
    CCMU_MOD_CLK_APB_UART5,   //apb_uart5
    CCMU_MOD_CLK_APB_UART6,   //apb_uart6
    CCMU_MOD_CLK_APB_UART7,   //apb_uart7

    //clock gating for access dram
    CCMU_MOD_CLK_SDRAM_VE,    //sdram_ve
    CCMU_MOD_CLK_SDRAM_CSI0,  //sdram_csi0
    CCMU_MOD_CLK_SDRAM_CSI1,  //sdram_csi1
    CCMU_MOD_CLK_SDRAM_TS,    //sdram_ts
    CCMU_MOD_CLK_SDRAM_TVD,   //sdram_tvd
    CCMU_MOD_CLK_SDRAM_TVE0,  //sdram_tve0
    CCMU_MOD_CLK_SDRAM_TVE1,  //sdram_tve1
    CCMU_MOD_CLK_SDRAM_DEFE0, //sdram_de_scale0
    CCMU_MOD_CLK_SDRAM_DEFE1, //sdram_de_scale1
    CCMU_MOD_CLK_SDRAM_DEBE0, //sdram_de_image0
    CCMU_MOD_CLK_SDRAM_DEBE1, //sdram_de_image1
    CCMU_MOD_CLK_SDRAM_DEMP,  //sdram_de_mix
    CCMU_MOD_CLK_SDRAM_ACE,   //sdram_ace
    
    CCMU_MOD_CLK_AHB_EHCI1,   //ahb_ehci1 
    CCMU_MOD_CLK_AHB_OHCI1,   //ahb_ohci1 

    CCMU_MOD_CLK_IEP,      //iep
    CCMU_MOD_CLK_AHB_IEP,  //ahb_iep
    CCMU_MOD_CLK_SDRAM_IEP,//sdram_iep
    CCMU_MOD_CLK_CNT         //invalid module clock number, for calc count
    
} __ccu_mod_clk_e;


//------------------------------------------------------------------------------
//  Module clock parameters for set module clock frequency
//------------------------------------------------------------------------------
typedef struct __CCMU_MOD_CLK_PARA
{
    __u32   ClkNo;          //must be CCM_MOD_CLK_XXX
    __u32   SrcClkNo;       //must be CCM_SYS_CLK_XXX
    __u32   DivideRatio;    //devide ratio of module clock
    __bool  IsOn;           //module clock on or off
    __bool  ResetIsValid;   //should been reset or not
} __ccmu_mod_clk_para_t;

//------------------------------------------------------------------------------
//  System clock information
//------------------------------------------------------------------------------
typedef struct __CCMU_SYS_CLK_INFO
{
    __u32   ClkNo;          //the system clock number
    __u32   SrcClkNo;       //the source clock number, CCM_SYS_CLK_CNT if no source
    __u32   Freq;           //the frequency of system clock
    char   *pName;          //name of this system clock
} __ccmu_sys_clk_info_t;


//------------------------------------------------------------------------------
//  Module clock information
//------------------------------------------------------------------------------
typedef struct __CCMU_MOD_CLK_INFO
{
    __u32   ClkNo;          //the module clock number
    __u32   SrcClkNo;       //the source clock number
    __u32   DivideRatio;    //the divide ratio of module clock
    char   *pName;          //name of this module clock
} __ccmu_mod_clk_info_t;


//------------------------------------------------------------------------------
//  CCM module interfaces
//------------------------------------------------------------------------------
__s32   CCMU_Init(void);
__s32   CCMU_Exit(void);

__s32   CCMU_SetSysClkFreq(__u32 SysClkNo, __u64 Freq);
__s64   CCMU_GetSysClkFreq(__u32 SysClkNo);
__s32   CCMU_GetSysClkInfo(__u32 SysClkNo, __ccmu_sys_clk_info_t *pInfo);
__u32   CCMU_GetSysClkCnt(void);
__s32   CCMU_SetSysClkStatus(__u32 SysClkNo, __bool IsOn);
__bool  CCMU_GetSysClkStatus(__u32 SysClkNo);

__s32   CCMU_SetModClkFreq(__ccmu_mod_clk_para_t *pModClkPara);
__s64   CCMU_GetModClkFreq(__u32 ModClkNo);
__s32   CCMU_SetModClkStatus(__u32 ModClkNo, __bool IsOn);
__bool  CCMU_GetModClkStatus(__u32 ModClkNo);
__u32   CCMU_GetModClkCnt(void);
__s32   CCMU_GetModClkInfo(__u32 ModClkNo, __ccmu_mod_clk_info_t *pInfo);
__s32   CCMU_SetModClkResetControl(__u32 ModClkNo, __bool ResetValid);
__bool  CCMU_GetModClkResetControl(__u32 ModClkNo);

void    CCMU_DumpClkInfo(void);

#endif //__CSP_CCMU_H__

