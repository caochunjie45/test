/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                   BSP TWI
*
*                                    (c) Copyright 2011-2014, Jackie China
*                                             All Rights Reserved
*
* File    : bsp_twi.h
* By      : Jackie
* Version : v1.0
* Date    : 2011-11-16
* Descript: two wire interface header file.
* Update  : date                auther      ver     notes
*           2011-11-16 15:07    Jackie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __BSP_TWI_H__
#define __BSP_TWI_H__

//7 bits or 10 bits slave address 
#define TWI_SLAVE_ADDR_7BITS  0
#define TWI_SLAVE_ADDR_10BITS 1

//slave's data register address length
#define TWI_REG_ADDR_1_BYTE    1
#define TWI_REG_ADDR_2_BYTE    2
#define TWI_REG_ADDR_3_BYTE    3

//TWI write or read command
#define TWI_WR_FLAG  0     
#define TWI_RD_FLAG  1

//read or write format
//temporary support one types of write format and three types of read format
#define TWI_WRITE_DATA_SPEC_RS            (0) //spec write
#define TWI_READ_DATA_SPEC_RS             (1) //spec read
#define TWI_READ_DATA_EX_NORS             (2) //no restart3
#define TWI_READ_DATA_EX_STPRS            (3) //stop + restart 

//TWI select poll method or interrupt method
#define TWI_RW_POLLING_METHOD     1
#define TWI_RW_INTERRUPT_METHOD   0


#define TWI_SCLK_400KHZ    (400000) // 400kHz 
#define TWI_SCLK_100KHZ    (100000) //100kHz
#define TWI_SCLK_50KHZ    (50000) //50kHz



typedef struct __TWI_PARA_SET
{
    //parameters of read and write
    __u16 slave_addr;   //slave address
    __u8  slave_len;    //length of slave address

    __u32 data_addr;    //slave device data register address
    __u8  data_len;     //length of slave device data register address

    __u8  rw_flag;      //read or write flag£»
    __u8  rw_format;    //read or write format

}__twi_para_set_t;


typedef void (*__twi_cb)(__s32 ret);
//  the possible value of ret as follows:
//  0   : succeed
//  -1  : fail            
//  0xf8: On reset or stop the bus is idle            
//  0xd8: second slave address has been transmitted; NOT ACK has been received            
//  0x20: SLA+W has been transmitted; NOT ACK has been received   
//  0x30: Data byte in I2CDAT has been transmitted; NOT ACK has been received
//  0x48: SLA+R has been transmitted; NOT ACK has been received            
//  0x58: Data byte has been received; NOT ACK has been transmitted

typedef struct __TWI_SETTING
{
    //parameters when initialize iic
    __s32       bus_num;
    __u32       sclk_req;       //100kHz or 400kHz

    __u32       poll_flag;      //0:interrupt; 1:poll£¬must realize callback function, 
                                //the follow parameters is not necessary.   
    
    __u32       irq_no;
    __u32       irq_flags;
    __u32       irq_prio;    
    
    __twi_cb    wakup_callback; //callback, wakup the upper layer

}__twi_setting_t;


/*
*********************************************************************************************************
*                                           TWI INIT
*
* Description: Initialize twi.
*
* Parameters : twi:the structure include initialization settings.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 BSP_TWI_Init(__twi_setting_t *twi);


/*
*********************************************************************************************************
*                                           TWI EXIT
*
* Description: Exit twi.
*
* Parameters : twi:the structure include initialization settings.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 BSP_TWI_Exit(__twi_setting_t *twi);

/*
*********************************************************************************************************
*                                           TWI READ
*
* Description: Receive data.
*
* Parameters : bus_num  : twi bus number.
               para     : read parameter.
*              buffer   : buffer receive the data.
               len      : length of data to be received.
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 BSP_TWI_Read(__s32 bus_num, __twi_para_set_t *para, __u8 *buffer, __u32 len);

/*
*********************************************************************************************************
*                                           TWI WRITE
*
* Description: Transmit data.
*
* Parameters : bus_num  : twi bus number.
               para     : write parameter.
*              buffer   : pointer to the data buffer to be transmitted.
               len      : length of data to be transmitted.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 BSP_TWI_Write(__s32 bus_num, __twi_para_set_t *para, const __u8 *buffer, __u32 len);

/*
*********************************************************************************************************
*                                           TWI SET CLOCK
*
* Description: Set the clock frequency.
*
* Parameters : bus_num  : twi bus number.
               sclk_req : clock frequency.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 BSP_TWI_SetClock(__s32 bus_num, __u32 sclk_req);

/*
*********************************************************************************************************
*                                           TWI GET LINE STATUS
*
* Description: Get the status of SCL or SDA.
*
* Parameters : bus_num  : twi bus number.
               sda_scl  : select SDA or SCL. 0:SDA; 1:SCL.
*
* Returns    : the status of selected line. 0:low; 1:high.
*
* Note       :
*********************************************************************************************************
*/
__s32 BSP_TWI_GetLineStatus(__s32 bus_num, __u32 sda_scl);

/*
*********************************************************************************************************
*                                           TWI SET LINE STATUS
*
* Description: Set the status of SCL or SDA.
*
* Parameters : bus_num  : twi bus number.
               sda_scl  : select SDA or SCL. 0:SDA; 1:SCL.
*              hi_low   : set the selected line to low(0) or high(1).
    
* Returns    : the status of selected line. 0:low; 1:high.
*
* Note       : BSP_TWI_LineControl() should be called to set the selected line can be controled 
               before this function call.
*********************************************************************************************************
*/
__s32 BSP_TWI_SetLineStatus(__s32 bus_num, __u32 sda_scl, __u32 hi_low);

/*
*********************************************************************************************************
*                                           TWI SET LINE CONTROL
*
* Description: Set whether the SCL/SDA can be controled or not.
*
* Parameters : bus_num  : twi bus number.
               sda_scl  : select SDA or SCL. 0:SDA; 1:SCL.
*              ctl      : set the selected line can be controled or not.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 BSP_TWI_LineControl(__s32 bus_num, __u32 sda_scl, __u32 ctl);

#endif  //__BSP_TWI_H__
