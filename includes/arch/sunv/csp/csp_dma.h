/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                   CSP DMA
*
*                                    (c) Copyright 2011-2014, Jackie China
*                                             All Rights Reserved
*
* File    : csp_dma.h
* By      : Jackie
* Version : v1.0
* Date    : 2011-11-21
* Descript: DMA header file.
* Update  : date                auther      ver     notes
*           2021-11-21 15:07    Jackie      1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __CSP_DMA_H__
#define __CSP_DMA_H__

#define  CSP_DMA_HDLER_TYPE_CNT                     2
#define  CSP_DMAC_DMATYPE_NORMAL         			0
#define  CSP_DMAC_DMATYPE_DEDICATED      			1


#define  CSP_DMA_TRANSFER_HALF_INT                  0
#define  CSP_DMA_TRANSFER_END_INT                   1

#define  CSP_DMA_TRANSFER_UNLOOP_MODE               0
#define  CSP_DMA_TRANSFER_LOOP_MODE                 1


//============================================================
//====================    DMA config   =======================
//============================================================

// DMA continuous mode  
#define CSP_DMAC_CFG_CONTINUOUS_ENABLE              (0x01)	
#define CSP_DMAC_CFG_CONTINUOUS_DISABLE             (0x00)	

/// DMA wait clock                                        	
#define	CSP_DMAC_CFG_WAIT_0_DMA_CLOCK				(0x00)	
#define	CSP_DMAC_CFG_WAIT_1_DMA_CLOCK				(0x01)	
#define	CSP_DMAC_CFG_WAIT_2_DMA_CLOCK				(0x02)	
#define	CSP_DMAC_CFG_WAIT_4_DMA_CLOCK				(0x03)	
#define	CSP_DMAC_CFG_WAIT_8_DMA_CLOCK				(0x04)	
#define	CSP_DMAC_CFG_WAIT_16_DMA_CLOCK				(0x05)	
#define	CSP_DMAC_CFG_WAIT_32_DMA_CLOCK				(0x06)	
#define	CSP_DMAC_CFG_WAIT_64_DMA_CLOCK				(0x07)	

// ===============   DMA destnation config  ==============                                 	
// Data width                                  	
#define	CSP_DMAC_CFG_DEST_DATA_WIDTH_8BIT			(0x00)	
#define	CSP_DMAC_CFG_DEST_DATA_WIDTH_16BIT			(0x01)	
#define	CSP_DMAC_CFG_DEST_DATA_WIDTH_32BIT			(0x02)	

// Burst length                              	
#define	CSP_DMAC_CFG_DEST_1_BURST       			(0x00)	
#define	CSP_DMAC_CFG_DEST_4_BURST		    		(0x01)	
#define	CSP_DMAC_CFG_DEST_8_BURST		    		(0x02)	

// Address type                              	
#define	CSP_DMAC_CFG_DEST_ADDR_TYPE_LINEAR_MODE		(0x00)	
#define	CSP_DMAC_CFG_DEST_ADDR_TYPE_IO_MODE 		(0x01)	
#define	CSP_DMAC_CFG_DEST_ADDR_TYPE_HPAGE_MODE 		(0x02)	
#define	CSP_DMAC_CFG_DEST_ADDR_TYPE_VPAGE_MODE 		(0x03)	

                                                          	
// ===============    DMA source config    ===============                                  	
// Data width                                    	
#define	CSP_DMAC_CFG_SRC_DATA_WIDTH_8BIT			(0x00)	
#define	CSP_DMAC_CFG_SRC_DATA_WIDTH_16BIT			(0x01)	
#define	CSP_DMAC_CFG_SRC_DATA_WIDTH_32BIT			(0x02)	

// Burst length                                	
#define	CSP_DMAC_CFG_SRC_1_BURST       				(0x00)	
#define	CSP_DMAC_CFG_SRC_4_BURST		    		(0x01)	
#define	CSP_DMAC_CFG_SRC_8_BURST		    		(0x02)

// Address type                                	
#define	CSP_DMAC_CFG_SRC_ADDR_TYPE_LINEAR_MODE		(0x00)	
#define	CSP_DMAC_CFG_SRC_ADDR_TYPE_IO_MODE 			(0x01)	
#define	CSP_DMAC_CFG_SRC_ADDR_TYPE_HPAGE_MODE 		(0x02)	
#define	CSP_DMAC_CFG_SRC_ADDR_TYPE_VPAGE_MODE 		(0x03)	
                                                          	
                                                          	
                           	
// Normal DMA destnation DRQ type                       	
#define	CSP_DMAC_CFG_DEST_TYPE_IR0					(0x00)	
#define	CSP_DMAC_CFG_DEST_TYPE_IR1		    	    (0x01)	
#define	CSP_DMAC_CFG_DEST_TYPE_SPDIF			    (0x02)	
#define	CSP_DMAC_CFG_DEST_TYPE_IIS			    	(0x03)	
	
#define	CSP_DMAC_CFG_DEST_TYPE_AC97				    (0x05)	
	
#define	CSP_DMAC_CFG_DEST_TYPE_UART0				(0x08)	
#define	CSP_DMAC_CFG_DEST_TYPE_UART1				(0x09)	
#define	CSP_DMAC_CFG_DEST_TYPE_UART2				(0x0a)
#define	CSP_DMAC_CFG_DEST_TYPE_UART3				(0x0b)	
#define	CSP_DMAC_CFG_DEST_TYPE_UART4				(0x0c)
#define	CSP_DMAC_CFG_DEST_TYPE_UART5				(0x0d)	
#define	CSP_DMAC_CFG_DEST_TYPE_UART6				(0x0e)
#define	CSP_DMAC_CFG_DEST_TYPE_UART7				(0x0f)
#define	CSP_DMAC_CFG_DEST_TYPE_HDMIDDC				(0x10)

#define	CSP_DMAC_CFG_DEST_TYPE_AUDIO_DA				(0x13)	
	
#define	CSP_DMAC_CFG_DEST_TYPE_N_SRAM 				(0x15)	
#define	CSP_DMAC_CFG_DEST_TYPE_N_SDRAM				(0x16)

#define	CSP_DMAC_CFG_DEST_TYPE_N_SPI0				(0x18)	
#define	CSP_DMAC_CFG_DEST_TYPE_N_SPI1				(0x19)	
#define	CSP_DMAC_CFG_DEST_TYPE_N_SPI2				(0x1a)	
#define	CSP_DMAC_CFG_DEST_TYPE_N_SPI3				(0x1b)	
                                                          	
// Dedicated DMA destnation DRQ type                       	
#define	CSP_DMAC_CFG_DEST_TYPE_D_SRAM 				(0x00)
#define	CSP_DMAC_CFG_DEST_TYPE_D_SDRAM				(0x01)
#define	CSP_DMAC_CFG_DEST_TYPE_PATA				    (0x02)
#define	CSP_DMAC_CFG_DEST_TYPE_NFC  		    	(0x03)
#define	CSP_DMAC_CFG_DEST_TYPE_USB0			    	(0x04)
#define	CSP_DMAC_CFG_DEST_TYPE_ETHERNET_MAC	    	(0x06)
#define	CSP_DMAC_CFG_DEST_TYPE_D_SPI1 				(0x08)
#define	CSP_DMAC_CFG_DEST_TYPE_SECURITY_SYS			(0x0a)
#define	CSP_DMAC_CFG_DEST_TYPE_TCON0 				(0x0e)
#define	CSP_DMAC_CFG_DEST_TYPE_TCON1 				(0x0f)
#define	CSP_DMAC_CFG_DEST_TYPE_MSC			    	(0x17)
#define	CSP_DMAC_CFG_DEST_TYPE_HDMI_AUDIO	    	(0x18)                                                          
#define	CSP_DMAC_CFG_DEST_TYPE_D_SPI0			    (0x1a)
#define	CSP_DMAC_CFG_DEST_TYPE_D_SPI2			    (0x1c)
#define	CSP_DMAC_CFG_DEST_TYPE_D_SPI3			    (0x1e)


// Normal DMA source DRQ type                        
#define	CSP_DMAC_CFG_SRC_TYPE_IR0					(0x00)
#define	CSP_DMAC_CFG_SRC_TYPE_IR1		    	   	(0x01)
#define	CSP_DMAC_CFG_SRC_TYPE_SPDIF			    	(0x02)
#define	CSP_DMAC_CFG_SRC_TYPE_IIS			    	(0x03)

#define	CSP_DMAC_CFG_SRC_TYPE_AC97				    (0x05)

#define	CSP_DMAC_CFG_SRC_TYPE_UART0				    (0x08)
#define	CSP_DMAC_CFG_SRC_TYPE_UART1				    (0x09)
#define	CSP_DMAC_CFG_SRC_TYPE_UART2				    (0x0a)
#define	CSP_DMAC_CFG_SRC_TYPE_UART3				    (0x0b)
#define	CSP_DMAC_CFG_SRC_TYPE_UART4				    (0x0c)
#define	CSP_DMAC_CFG_SRC_TYPE_UART5				    (0x0d)
#define	CSP_DMAC_CFG_SRC_TYPE_UART6				    (0x0e)
#define	CSP_DMAC_CFG_SRC_TYPE_UART7				    (0x0f)
#define	CSP_DMAC_CFG_SRC_TYPE_HDMIDDC			    (0x10)
                                                        
#define	CSP_DMAC_CFG_SRC_TYPE_AUDIO_AD 				(0x13)

#define	CSP_DMAC_CFG_SRC_TYPE_N_SRAM 				(0x15)
#define	CSP_DMAC_CFG_SRC_TYPE_N_SDRAM				(0x16)
#define	CSP_DMAC_CFG_SRC_TYPE_TP				    (0x17)
#define	CSP_DMAC_CFG_SRC_TYPE_N_SPI0				(0x18)
#define	CSP_DMAC_CFG_SRC_TYPE_N_SPI1				(0x19)
#define	CSP_DMAC_CFG_SRC_TYPE_N_SPI2				(0x1a)
//#define	CSP_DMAC_CFG_SRC_TYPE_N_SPI3				(0x1b)
#define	CSP_DMAC_CFG_SRC_TYPE_N_USB_EP1				(0x1b)
#define	CSP_DMAC_CFG_SRC_TYPE_N_USB_EP2				(0x1c)
#define	CSP_DMAC_CFG_SRC_TYPE_N_USB_EP3				(0x1d)
#define	CSP_DMAC_CFG_SRC_TYPE_N_USB_EP4				(0x1e)
#define	CSP_DMAC_CFG_SRC_TYPE_N_USB_EP5				(0x1f)
                                                          
// Dedicated DMA source DRQ type                          
#define	CSP_DMAC_CFG_SRC_TYPE_D_SRAM 				(0x00)
#define	CSP_DMAC_CFG_SRC_TYPE_D_SDRAM				(0x01)
#define	CSP_DMAC_CFG_SRC_TYPE_PATA				    (0x02)
#define	CSP_DMAC_CFG_SRC_TYPE_NFC  		    	   	(0x03)
#define	CSP_DMAC_CFG_SRC_TYPE_USB0			    	(0x04)

#define	CSP_DMAC_CFG_SRC_TYPE_ETHERNET_MAC	    	(0x07)
#define	CSP_DMAC_CFG_SRC_TYPE_D_SPI1 				(0x09)
#define	CSP_DMAC_CFG_SRC_TYPE_SECURITY_SYS		    (0x0b)

#define	CSP_DMAC_CFG_SRC_TYPE_MSC 				    (0x17)
#define	CSP_DMAC_CFG_SRC_TYPE_D_SPI0   				(0x1b)
#define	CSP_DMAC_CFG_SRC_TYPE_D_SPI2			    (0x1d)
#define	CSP_DMAC_CFG_SRC_TYPE_D_SPI3			    (0x1f)




typedef struct  __CSP_DMA_CONFIG
{
    unsigned int      src_drq_type     ; //Source DRQ type
    unsigned int      src_addr_type    ; //Source Address type
    unsigned int      src_burst_length ; //Source Burst length
    unsigned int      src_data_width   ; //Source Data width
    unsigned int      dst_drq_type     ; //Destnation DRQ type
    unsigned int      dst_addr_type    ; //Destnation Address type  
    unsigned int      dst_burst_length ; //Destnation Burst length 
    unsigned int      dst_data_width   ; //Destnation Data width    
    unsigned int      wait_state       ; //Wait clock
    unsigned int      continuous_mode  ; //Continuous mode
    
    unsigned int      cmt_blk_cnt	   ; //DMA comity counter
}__csp_dma_config_t;


/*
*********************************************************************************************************
*                                           DMA INIT
*
* Description: Initialize DMA.
*
* Parameters : NONE
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_Init(void);

/*
*********************************************************************************************************
*                                           DMA EXIT
*
* Description: Initialize DMA.
*
* Parameters : NONE
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_Exit(void);

/*
*********************************************************************************************************
*                                           DMA REQUEST
*
* Description: Request a DMA channel.
*
* Parameters : dmatype  : dma type. 
                          CSP_DMAC_DMATYPE_NORMAL   :normal DMA. 
                          CSP_DMAC_DMATYPE_DEDICATED:dedicated DMA.
*
* Returns    : DMA handle if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/

__hdle CSP_DMA_Request(__u32 dmatype);

/*
*********************************************************************************************************
*                                           DMA RELEASE
*
* Description: Release a DMA channel.
*
* Parameters : hDMA : DMA handle to release.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_Release(__hdle hDMA);

/*
*********************************************************************************************************
*                                           DMA CONFIG
*
* Description: Config a DMA channel.
*
* Parameters : hDMA : DMA handle to config.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_Config(__hdle hDMA, __csp_dma_config_t *p_config);

/*
*********************************************************************************************************
*                                           DMA START
*
* Description: Start DMA transfer.
*
* Parameters : hDMA     : DMA handle.
               src_addr : source address for data transfer.
               src_addr : destnation address for data transfer.
               bytes    : count of bytes for transfer.
                    
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_Start(__hdle hDMA, __u32 src_addr, __u32 dst_addr, __u32 bytes);

/*
*********************************************************************************************************
*                                           DMA STOP
*
* Description: Stop DMA transfer.
*
* Parameters : hDMA : DMA handle.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_Stop(__hdle hDMA);

/*
*********************************************************************************************************
*                                           DMA RESTART
*
* Description: Restart DMA transfer.
*
* Parameters : hDMA : DMA handle.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_Restart(__hdle hDMA);

/*
*********************************************************************************************************
*                                           DMA QUERY STATUS
*
* Description: Query DMA status.
*
* Parameters : hDMA : DMA handle.
*
* Returns    : DMA channel status. 0:idle; 1:running.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_QueryStatus(__hdle hDMA);

/*
*********************************************************************************************************
*                                           DMA QUETY SOURCE
*
* Description: Query DMA source address.
*
* Parameters : hDMA : DMA handle.
*
* Returns    : source address.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_QuerySrc(__hdle hDMA);

/*
*********************************************************************************************************
*                                           DMA QURER DESTNATION
*
* Description: Query DMA destnation address.
*
* Parameters : hDMA : DMA handle.
*
* Returns    : destnation address.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_QueryDst(__hdle hDMA);

/*
*********************************************************************************************************
*                                           DMA QUERY BYTE COUNT
*
* Description: Query byte count for current DMA transfer.
*
* Parameters : hDMA : DMA handle.
*
* Returns    : Byte count for current DMA transfer.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_QueryByteCnt(__hdle hDMA);

/*
*********************************************************************************************************
*                                           DMA QUERY REST BYTE COUNT
*
* Description: Query rest byte count for current DMA transfer.
*
* Parameters : hDMA : DMA handle.
*
* Returns    : Rest byte count for current DMA transfer.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_QueryRestByteCnt(__hdle hDMA);

/*
*********************************************************************************************************
*                                           DMA SET MODE
*
* Description: Set DMA mode.
*
* Parameters : hDMA : DMA handle.
               mode : 
                      CSP_DMA_TRANSFER_LOOP_MODE    : continuous mode enable.
                      CSP_DMA_TRANSFER_UNLOOP_MODE  : continuous mode disable.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_SetMode(__hdle hDMA, __u32 mode);

/*
*********************************************************************************************************
*                                           DMA QUERY INTERRUPT
*
* Description: Query whether interrupt enable or not.
*
* Parameters : hDMA : DMA handle.
               type : 
                      CSP_DMA_TRANSFER_HALF_INT : half transfer.
                      CSP_DMA_TRANSFER_END_INT  : full transfer.
*
* Returns    : EPDK_TRUE if interrupt enable, EPDK_FALSE if interrupt disable.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_QueryInt(__hdle hDMA, __s32 type);

/*
*********************************************************************************************************
*                                           DMA CLEAR INTERRUPT PENDING
*
* Description: Clear interrupt pending.
*
* Parameters : hDMA : DMA handle.
               type : 
                      CSP_DMA_TRANSFER_HALF_INT : half transfer.
                      CSP_DMA_TRANSFER_END_INT  : full transfer.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_ClearIntPending(__hdle hDMA, __s32 type);

/*
*********************************************************************************************************
*                                           DMA ENABLE INTERRUPT
*
* Description: Enable interrupt.
*
* Parameters : hDMA : DMA handle.
               type : 
                      CSP_DMA_TRANSFER_HALF_INT : half transfer.
                      CSP_DMA_TRANSFER_END_INT  : full transfer.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_EnableInt(__hdle hDMA, __s32 type);

/*
*********************************************************************************************************
*                                           DMA DISABLE INTERRUPT
*
* Description: Disable interrupt.
*
* Parameters : hDMA : DMA handle.
               type : 
                      CSP_DMA_TRANSFER_HALF_INT : half transfer.
                      CSP_DMA_TRANSFER_END_INT  : full transfer.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_DisableInt(__hdle hDMA, __s32 type);

/*
*********************************************************************************************************
*                                           DMA GET IRQ NUM
*
* Description: Get irq number.
*
* Parameters : NONE
*
* Returns    : number of irq.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_GetIrqNum(void);

/*
*********************************************************************************************************
*                                           DMA GET VIRTUAL BASE ADDRESS
*
* Description: Get DMA virtual base address.
*
* Parameters : NONE
*
* Returns    : DMA virtual base address.
*
* Note       :
*********************************************************************************************************
*/

__s32 CSP_DMA_GetVirtualBaseAddr(void);


#endif
