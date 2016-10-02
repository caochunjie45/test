/****************************************************************************
*
*  Front-end device driver for Fujitsu MB86A22 ISDB-T frontend.
*
*  Copyright (c) 2010 Fujitsu Semiconductor 
*  All rights reserved. 
*
*  Created 12/14/2010
*
****************************************************************************/
#ifndef _MB86A22_USER_DEFINE_H_
#define _MB86A22_USER_DEFINE_H_


typedef enum {
	I2C_OK = 0,
	I2C_NG = 1
} I2C_ACCCESS ;

void   MB86A2x_wait( SINT32 ) ;
UINT32 MB86A2x_write( UINT8, UINT8, UINT8 ) ;
UINT32 MB86A2x_read( UINT8, UINT8, UINT8 * ) ;
#endif

