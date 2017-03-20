/*
************************************************************************************************************************
*                                                         Slider

*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : slider.h
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2009.10.09
*
* Description :
*
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2009.10.09       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  __slider_h
#define  __slider_h

#include "ResAdapter.h"

/* control class name */
#define CTRL_SLIDER                     "CTRL_SLIDER"


/* slider style */
#define SDS_VERTICAL                    ( 1 << 16 )    //��ֱ
#define SDS_RESPOND_OUTDEV              ( 1 << 17 )
#define SDS_RESPOND_WITHIN_WIN          ( 1 << 18 )

typedef __u32  HBMP;

/* slider notify message */
typedef enum
{
	SDN_STARTDRAG = 0,
	SDN_STOPDRAG     ,
	SDN_POS_CHANGED  ,
	SDN_KEY          ,
	SDN_
}sdn_e;


/* attr for a slider */
typedef struct tag_slider_para_t
{

	DECLARE_BMP_RES_Pt(head_bmp_buf);
	
	DECLARE_BMP_RES_Pt(tail_bmp_buf);
	
	DECLARE_BMP_RES_Pt(cursor_bmp_buf);

    __pos_t     			 text_pos;
    
	GUI_FONT*                draw_font;
	    
    __u32			   		 txt_color;
    
	
}slider_para_t;


typedef struct slider_action
{
	__pGUI_WIN_CB                    left_action;
	
	__pGUI_WIN_CB                    right_action;
	
	__pGUI_WIN_CB                    enter_action;
	
	__pGUI_WIN_CB                    return_action;
	
	
}slider_action_t;



typedef struct slider_attribute
{
	slider_para_t               slider_paint;
	
	slider_action_t             slider_action;

	
}slider_attribute_t;


extern __bool RegisterSliderControl( void );

extern __s32  SLIDER_SetRange   ( H_WIN hwin, __s32 min, __s32 max );
extern __s32  SLIDER_SetValue   ( H_WIN hwin, __s32 value );
extern __s32  SLIDER_GetMin     ( H_WIN hwin );
extern __s32  SLIDER_GetMax     ( H_WIN hwin );
extern __s32  SLIDER_GetValue   ( H_WIN hwin );
extern __s32  SLIDER_SetLineSize( H_WIN hwin, __u32 line_size );
extern __s32  SLIDER_GetLineSize( H_WIN hwin );

extern HBMP  bmp_open( const void *bmp_buf );
extern __s32 bmp_close( HBMP hbmp );
extern __u32 bmp_get_bitcount( HBMP hbmp );
extern __u32 bmp_get_width   ( HBMP hbmp );
extern __u32 bmp_get_height  ( HBMP hbmp );
extern __u32 bmp_get_size    ( HBMP hbmp );
extern void  bmp_get_matrix( const void *bmp_buf, void *matrix );

/**************************************************************************************************/
/*                                            Line Size                                           */
/*  ��Slider���ڰ������뽹�㲢�Ҵ���SDS_RESPOND_OUTDEV���ʱ������UP����DOWN������ֱģʽ�������߰�*/
/*��RIGHT��LEFT����ˮƽģʽ�����α꽫�ƶ�Line Size��ʾ�ľ���                                      */
/**************************************************************************************************/



#endif     //  ifndef __slider_h

/* end of slider.h */
