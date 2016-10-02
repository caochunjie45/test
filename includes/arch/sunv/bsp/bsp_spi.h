/*
****************************************************************************************************
*                                               MELIS
*                               the Easy Portable/Player Develop Kits
*                                           SPI Module
*
*                           (c) Copyright 2011-2014, All winners Co,Ld.
*                                       All Rights Reserved
*
* File    : bsp_spi.h
* By      : james.deng
* Version : 1.0.0
* Date    : 2011-12-31
* Descript: the head of bsp spi module.
* Update  : <date>          <author>            <version>           <notes>
*           2011-12-31      james.deng          1.0.0               build the file.
****************************************************************************************************
*/

#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__


#define SPI_COUNT                       4

// SPI work mode
#define SPI_MODE_MASTER                 1
#define SPI_MODE_SLAVE                  0

// SPI chip select
#define SPI_SS0                         0
#define SPI_SS1                         1
#define SPI_SS2                         2
#define SPI_SS3                         3

// SPI work frequency, 10MHz
#define SPI_DEFAULT_FREQ                10000000

// SPI max transfer size
#define SPI_MAX_TRANSFER                0xffffff


typedef struct __SPI_PARA
{
    __u32 nMaster;                      // spi work mode, SPI_MODE_MASTER
    __u32 nChipSelect;                  // spi chip select
    __u32 nFreq;                        // spi work frequency, Hz
    __s32 bInt;                         // uses interrupt, 0/1
    __s32 bDMA;                         // uses dma, 0/1
} __spi_para_t;

typedef struct __SPI_TRANSFER
{
    void  *tx_addr;
    __u32 tx_count;
    void  *rx_addr;
    __u32 rx_count;
    
    __u32 nWorkMode;
    __u32 nFreq;
    __u32 nChipSelect;
    
} __spi_transfer_t;


/*
****************************************************************************************************
*                                       BSP_SPI_Init
*              
* Description: Initialize spi module.
*              
* Arguments  : 
*              
* Return     : EPDK_OK
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_SPI_Init(void);

/*
****************************************************************************************************
*                                       BSP_SPI_Exit
*              
* Description: Exit spi module.
*              
* Arguments  : 
*              
* Return     : EPDK_OK
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_SPI_Exit(void);

/*
****************************************************************************************************
*                                       BSP_SPI_Request
*              
* Description: Request spi by bus number.
*              
* Arguments  : nBus         : the bus number of spi.
*              
* Return     : __hdle       : the handle of spi bus.
*              
* Notes      : 
*              
****************************************************************************************************
*/
__hdle BSP_SPI_Request(__u32 nBus, const __spi_para_t *para);

/*
****************************************************************************************************
*                                       BSP_SPI_Release
*              
* Description: Release spi bus.
*              
* Arguments  : hSPI         : the handle of spi bus.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_SPI_Release(__hdle hSPI);

/*
****************************************************************************************************
*                                       BSP_SPI_SetFreq
*              
* Description: Set the frequency of spi.
*              
* Arguments  : hSPI         : the handle of spi.
*              nFreq        : the frequency of spi.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_SPI_SetFreq(__hdle hSPI, __u32 nFreq);

/*
****************************************************************************************************
*                                       BSP_SPI_SetChip
*              
* Description: Set the chip select of spi.
*              
* Arguments  : hSPI         : the handle of spi.
*              nChipSelect  : the chip select of spi.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_SPI_SetChip(__hdle hSPI, __u32 nChipSelect);

/*
****************************************************************************************************
*                                       BSP_SPI_SetSSMode
*              
* Description: Set SS signal output mode.
*              
* Arguments  : hSPI         : the handle of spi.
*              nMode        : the SS signal output mode of spi, 0 indicates automatic, 1 indicates manual.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_SPI_SetSSMode(__hdle hSPI, __u32 nMode);

/*
****************************************************************************************************
*                                       BSP_SPI_SetSSLevel
*              
* Description: Set the level of SS signal. Only available when SS signal output mode is manual.
*              
* Arguments  : hSPI         : the handle of spi.
*              nLevel       : the level of SS signal, 0 set SS to low, 1 set SS to high.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_SPI_SetSSLevel(__hdle hSPI, __u32 nLevel);

/*
****************************************************************************************************
*                                       BSP_SPI_MasterWaitClock
*              
* Description: Set wait clock counter in master mode.
*              
* Arguments  : hSPI         : the handle of spi.
*              nWaitCnt     : wait clock counter.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_SPI_MasterWaitClock(__hdle hSPI, __u32 nWaitCnt);

/*
****************************************************************************************************
*                                       BSP_SPI_MasterSampleDly
*              
* Description: Control spi sample delay in high speed mode. Only available in master mode.
*              
* Arguments  : hSPI         : the handle of spi.
*              nOnOff       : 0 indicates off, 1 indicates on.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_SPI_MasterSampleDly(__hdle hSPI, __u32 nOnOff);

/*
****************************************************************************************************
*                                       BSP_SPI_MasterRW
*              
* Description: Read and write in master mode.
*              
* Arguments  : hSPI         : the handle of spi.
*              pXfer        : the read/write parameter of spi.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_SPI_MasterRW(__hdle hSPI, __spi_transfer_t *pXfer);

__s32 BSP_SPI_Clk_ONOFF(__hdle hSPI, __s32 flag);

#endif // __BSP_SPI_H__
