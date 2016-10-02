/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           willow app sample
*
*						        (c) Copyright 2006-2007, RICHARD, China
*											All	Rights Reserved
*
* File    : breath.h
* By      : worldchip by James
* Version : V1.00
*********************************************************************************************************
*/
#ifndef  _BREATH_H_
#define  _BREATH_H_

#include "..\\mainmenu_private.h"


//#define ENABLE_BREATH_EFFECT


/*breath light info struct*/
typedef struct
{
	volatile __bool 			exit;			/* exit breath flag 		*/

	__u8					mtsk;			/* the thread run breath    */
	H_LYR					layer;			/* gui layer				*/
	__pos_t 					site;			/* breath poisition in layer*/

	SIZE					core_size;		/* core img size    		*/
	SIZE					breath_size;     /* breath region			*/
	__u8					*buf;			/* breath data buffer		*/
	FB						fb;				/* breath area frame buffer */
	__krnl_event_t			*sem;			/* breath control semphore  */
}__sw_breath_info_t;

__s32 BREATH_Init(void);
void BREATH_Exit(void);
void BREATH_ReStart(void);
void BREATH_Start(H_LYR layer,__pos_t *site,SIZE *core_size, void *core_buf,SIZE *breath_size);
void BREATH_Stop(__u8 *core_data,SIZE *core_size);

#endif

