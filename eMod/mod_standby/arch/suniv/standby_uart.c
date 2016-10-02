/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                             	Standby system
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : standby_uart.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-2-22
* Descript: uart operations for standby.
* Update  : date                auther      ver     notes
*           2012-2-22 15:24:27	Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "standby_i.h"
#include "standby_uart_i.h"

//use uart1
static standby_uart_t *uart_port = (standby_uart_t *)STANDBY_UART0_REGS_BASE;

/*
*********************************************************************************************************
*                                       STANDBY PUT CHAR
*
* Description: standby put char.
*
* Arguments  : none.
*
* Returns    : none;
*********************************************************************************************************
*/
void standby_putc(char c)
{
	//check fifo not full
	while ((uart_port->usr & TXFIFO_FULL) == 0)
	{
		//fifo is full, wait always
		;
	}
	
	//put a charset
	uart_port->thr = c;
}

/*
*********************************************************************************************************
*                                       STANDBY PUT STRING
*
* Description: standby put string.
*
* Arguments  : none.
*
* Returns    : none;
*********************************************************************************************************
*/
void standby_puts(const char *str)
{
	while(*str != '\0')
	{
		if(*str == '\n')
		{
			// if current character is '\n', insert and output '\r'
			standby_putc('\r');
		}
		standby_putc(*str++);
	}
}

void standby_put_uint(__u32 input)
{
    char stack[11] ;
    int i ;
    int j ;
    __u32 tmpIn = input;
    
    
    if( tmpIn == 0 )
    {
        standby_putc('0');
        return ;
    }
    
    for( i = 0; tmpIn > 0; ++i )
    {
        stack[i] = tmpIn%10 + '0';       // characters in reverse order are put in 'stack' .
        tmpIn /= 10;
    }                                    // at the end of 'for' loop, 'i' is the number of characters.

    for( --i, j = 0; i >= 0; --i, ++j )
    {
        standby_putc(stack[i]);
    }
}

void standby_put_hex(__u32 input)
{
    char tmpList[20] = "0123456789ABCDEF";
    char stack[11] ;
    int i ;
    int j ;
    __u32 tmpIn = input;
    
    
    standby_puts("0x");
    if( tmpIn == 0 )
    {
        standby_puts("00\n");
        return ;
    }
    
    for( i = 0; tmpIn > 0; ++i )
    {
        stack[i] = tmpList[tmpIn%16];       // characters in reverse order are put in 'stack' .
        tmpIn /= 16;
    }                                    // at the end of 'for' loop, 'i' is the number of characters.

    for( --i, j = 0; i >= 0; --i, ++j )
    {
        standby_putc(stack[i]);
    }
    standby_puts("\n");
}

