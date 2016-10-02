/*
****************************************************************************************************
*                                               MELIS
*                               the Easy Portable/Player Develop Kits
*                                           UART Module
*
*                           (c) Copyright 2011-2014, All winners Co,Ld.
*                                       All Rights Reserved
*
* File    : csp_uart.h
* By      : james.deng
* Version : 1.0.0
* Date    : 2011-11-18
* Descript: the header of uart protocal.
* Update  : <date>          <author>            <version>           <notes>
*           2011-11-18      james.deng          1.0.0               build the file.
****************************************************************************************************
*/

#ifndef __CSP_UART_H__
#define __CSP_UART_H__

#include "csp_pin.h"


#define UART_PORT_COUNT                 8

// Time out is infinity
#define CSP_UART_INFINITY               0

// The Parameter of uart
typedef struct __UART_DBG_PARA
{
    __u32 nParityEnable;
    __u32 nStopBit;
    __u32 nDataLen;
} __uart_dbg_para_t;

// Uart FIFO type
typedef enum __UART_DBG_FIFO_TYPE
{
    UART_DBG_FIFO_TYPE_RX,
    UART_DBG_FIFO_TYPE_TX
} __uart_dbg_fifo_type_t;

/*
****************************************************************************************************
*                                       CSP_UART_DbgInit
*              
* Description: Init uart module, called before other uart functions were called.
*              
* Arguments  : 
*              
* Return     : EPDK_OK
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_UART_DbgInit(void);

/*
****************************************************************************************************
*                                       CSP_UART_DbgExit
*              
* Description: Exit uart module.
*              
* Arguments  : 
*              
* Return     : EPDK_OK
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_UART_DbgExit(void);

/*
****************************************************************************************************
*                                       CSP_UART_DbgRequest
*              
* Description: Request a uart port.
*              
* Arguments  : nPort        : port number.
*              pPinSet      : the pin of uart port used.
*              nPinCnt      : the count of pin.
*              
* Return     : __hdle       : the handle of uart, NULL indicate fail to request a uart port.
*              
* Notes      : Request a uart port beforn clock and pin have been config.
*              
****************************************************************************************************
*/
__hdle CSP_UART_DbgRequest(__u32 nPort, __user_pin_set_t *pPinSet, __u32 nPinCnt);

/*
****************************************************************************************************
*                                       CSP_UART_DbgRelease
*              
* Description: Release a uart port.
*              
* Arguments  : hUart        : the handle of uart.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_UART_DbgRelease(__hdle hUart);

/*
****************************************************************************************************
*                                       CSP_UART_DbgLock
*              
* Description: Lock the port to disable transmit.
*              
* Arguments  : hUart        : the handle of uart.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_UART_DbgLock(__hdle hUart);

/*
****************************************************************************************************
*                                       CSP_UART_DbgUnlock
*              
* Description: Unlock the port to enable transmit.
*              
* Arguments  : hUart        : the handle of uart.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_UART_DbgUnlock(__hdle hUart);

/*
****************************************************************************************************
*                                       CSP_UART_DbgSetPara
*              
* Description: Set the parameter of uart.
*              
* Arguments  : hUart        : the handle of uart.
*              pUartPara    : the parameter of uart.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_UART_DbgSetPara(__hdle hUart, const __uart_dbg_para_t *pUartPara);

/*
****************************************************************************************************
*                                       CSP_UART_DbgSetBaudrate
*              
* Description: Set the baudrate of uart.
*              
* Arguments  : hUart        : the handle of uart.
*              nApbClk      : the clock frequence of APB.
*              nBaudRate    : the baudrate of uart.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_UART_DbgSetBaudrate(__hdle hUart, __u32 nBaudrate);

/*
****************************************************************************************************
*                                       CSP_UART_DbgGetChar
*              
* Description: Read a char data from uart.
*              
* Arguments  : hUart        : the handle of uart.
*              pChar        : the pointer of receive data, allocate by caller.
*              nTimeOut     : time out when waiting read state ready, CSP_UART_INFINITY indicates no time out.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_UART_DbgGetChar(__hdle hUart, __u8 *pChar, __u32 nTimeOut);

/*
****************************************************************************************************
*                                       CSP_UART_GetString
*              
* Description: Get a string from uart.
*              
* Arguments  : hUart        : the handle of uart.
*              pStr         : the pointer of receive string buffer, allocate by caller.
*              nSize        : the size of string buffer.
*              nTimeOut     : time out when waiting read state ready of every char,
*                             CSP_UART_INFINITY indicates no time out.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_UART_DbgGetString(__hdle hUart, __u8 *pStr, __u32 nSize, __u32 nTimeOut);

/*
****************************************************************************************************
*                                       CSP_UART_DbgPutChar
*              
* Description: Put a char to uart.
*              
* Arguments  : hUart        : the handle of uart.
*              ch           : the char.
*              nTimeOut     : time out when waiting write state ready, CSP_UART_INFINITY indicates no time out.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_UART_DbgPutChar(__hdle hUart, char ch, __u32 nTimeOut);

/*
****************************************************************************************************
*                                       CSP_UART_DbgPutString
*              
* Description: Put a string to uart.
*              
* Arguments  : hUart        : the handle of uart.
*              str          : the string.
*              nTimeOut     : time out when waiting write state ready, CSP_UART_INFINITY indicates no time out.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_UART_DbgPutString(__hdle hUart, const char *str, __u32 nTimeOut);

/*
****************************************************************************************************
*                                       CSP_UART_DbgClearFIFO
*              
* Description: Clear the port fifo.
*              
* Arguments  : hUart        : the handle of uart.
*              type         : UART_FIFO_TYPE_RX or UART_FIFO_TYPE_TX.
*   
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_UART_DbgClearFIFO(__hdle hUart, __uart_dbg_fifo_type_t type);

/*
****************************************************************************************************
*                                       CSP_UART_DbgEnableINT
*              
* Description: Enable uart interrupt.
*              
* Arguments  : hUart        : the handle of uart.
*              nIrqSrc      : the interrupt source of uart.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_UART_DegEnableINT(__hdle hUart, __u32 nIrqSrc);

/*
****************************************************************************************************
*                                       CSP_UART_DbgDisableINT
*              
* Description: Disable uart interrupt.
*              
* Arguments  : hUart        : the handle of uart.
*              nIrqSrc      : the interrupt source of uart.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_UART_DbgDisableINT(__hdle hUart, __u32 nIrqSrc);

/*
****************************************************************************************************
*                                       CSP_UART_DbgQueryINT
*              
* Description: Query uart interrupt state.
*              
* Arguments  : hUart        : the handle of uart.
*              pStatus      : the status of interrupt. bit[31:24] main interrupt number. bit[8:0] interrupt source.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_UART_DbgQueryINT(__hdle hUart, __u32 *pStatus);

#endif // __CSP_UART_H__
