/*
************************************************************************************************************************
*                                                        robin
*
*                             Copyright(C), 2009-2010, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : robin_zoom.h
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2009.09.27
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
* Gary.Wang      2009.09.27       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  __robin_zoom_h
#define  __robin_zoom_h


typedef enum
{
	ROBIN_ZOOM_FULL_SCREEN_VIDEO_RATIO = 0,    // ȫ��������Ƶ����
	ROBIN_ZOOM_FULL_SCREEN_SCREEN_RATIO   ,    // ȫ��������Ļ����
	ROBIN_ZOOM_ORIGIN_SIZE                ,    // ԭʼ�ߴ�
	ROBIN_ZOOM_4R3                        ,    // ǿ��4��3���������ܻᵼ�±���
	ROBIN_ZOOM_16R9                       ,    // ǿ��16��9���������ܻᵼ�±���
	ROBIN_ZOOM_FIT_VIEW                   ,    // �ʺϿɼ���ֻӦ����TV��ʾ��
	
	ROBIN_ZOOM_UNKNOWN = -1
}robin_zoom_e;

	
/***************************************************************************************************
*Name        : robin_set_zoom
*Prototype   : __s32  robin_set_zoom( robin_zoom_e zoom )
*Arguments   : zoom      input. zoom mode.
*Return      : ==0       succeed
*              !=0       fail
*Description : set video zoom mode.
*Other       :
***************************************************************************************************/
extern __s32  robin_set_zoom( robin_zoom_e zoom );

/***************************************************************************************************
*Name        : robin_get_zoom
*Prototype   : robin_zoom_e  robin_get_zoom( void )
*Arguments   : void
*Return      : the current zoom mode.
*Description : get the current zoom mode.
*Other       :
***************************************************************************************************/
extern robin_zoom_e  robin_get_zoom( void );


#endif     //  ifndef __robin_zoom_h

/* end of robin_zoom.h */
