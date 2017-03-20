/*
************************************************************************************************************************
*                                                        robin
*
*                             Copyright(C), 2009-2010, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : robin_lyric.c
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
#ifndef  __robin_lyric_c
#define  __robin_lyric_c


#include "..\\robin_i.h"




/***************************************************************************************************
*Name        : robin_lyric_exist
*Prototype   : __bool robin_lyric_exist( void )
*Arguments   : void
*Return      : EPDK_TRUE      there are lyrics corresponding to the current playing file.
*              EPDK_FALSE     there is NO lyric corresponding to the current playing file.
*Description : check whether there are lyrics corresponding to the current playing file or not.
*Other       :
***************************************************************************************************/
__bool robin_lyric_exist( void )
{
	__s32  lyric_format;
	__u8   err;

    if(EPDK_FALSE == robin_is_open())
    {
        return EPDK_FALSE;
    }
	
	g_pend_mutex( robin_cedar_mutex, &err );
	robin_wait_no_file( );

	lyric_format = esMODS_MIoctrl( robin_hced, CEDAR_CMD_GET_LBSTYPE, 0, NULL );
	if( lyric_format == CEDAR_LRC_TYPE_NONE || lyric_format == CEDAR_LRC_TYPE_NOTSPT )
	{
		g_post_mutex( robin_cedar_mutex );
		return EPDK_FALSE;
	}
	else
	{
		g_post_mutex( robin_cedar_mutex );
		return EPDK_TRUE;
	}
}



/***************************************************************************************************
*Name        : robin_get_lyric_format
*Prototype   : __cedar_lyric_fmt_t  robin_get_lyric_format( void )
*Arguments   : void
*Return      : the lyric format of the current playing file. the formats are defined in mod_cedar.h,
*              see __cedar_lyric_fmt_t.
*Description : get the lyric format of the current playing file.
*Other       :
***************************************************************************************************/
__cedar_lyric_fmt_t  robin_get_lyric_format( void )
{
	__cedar_lyric_fmt_t  format;	
	__u8   err;
	
	g_pend_mutex( robin_cedar_mutex, &err );
	robin_wait_no_file( );

	format = (__cedar_lyric_fmt_t)esMODS_MIoctrl( robin_hced, CEDAR_CMD_GET_LBSTYPE, 0, NULL );
	
	g_post_mutex( robin_cedar_mutex );
	
	return format;
}



/***************************************************************************************************
*Name        : robin_get_lyric_item_count
*Prototype   : __s32  robin_get_lyric_item_count( void )
*Arguments   : void
*Return      : the item count of the lyric corresponding to the current playing file. if return -1, 
*              that means no lyric or lyric information has NOT been parsed completely.
*Description : get the item count of the lyric corresponding to the current playing file.
*Other       :
***************************************************************************************************/
__s32  robin_get_lyric_item_count( void )
{
	__cedar_buf_inf_t  *lyric_desc_p = NULL;	
	__u8   err;

    if(EPDK_FALSE == robin_is_open())
    {
        return 0;
    }
	
	g_pend_mutex( robin_cedar_mutex, &err );
	robin_wait_no_file( );

	lyric_desc_p = (__cedar_buf_inf_t *)esMODS_MIoctrl( robin_hced, CEDAR_CMD_GET_SUB_INFO, 0, 
	                                                    (void *)CEDAR_GET_SUB_INF_ALL );
	if( lyric_desc_p == NULL || lyric_desc_p->size <= 0 )
	{
		__wrn("Fail in getting lyric info.\n");
		g_post_mutex( robin_cedar_mutex );
		return -1;
	}
	
	g_post_mutex( robin_cedar_mutex );
	return lyric_desc_p->size;
}



/***************************************************************************************************
*Name        : robin_get_lyric_item_list
*Prototype   : __s32  robin_get_lyric_item_list( __cedar_subtitle_item_t  *lyric_item_list, __u32 item_count )
*Arguments   : lyric_item_list    output. an array of lyric item.
*              item_count         input. the item count of the array.
*Return      : the real count of items in the array. if return -1, that means no lyric or lyric
*              information has NOT been parsed completely.
*Description : get lyric item list.
*Other       :
***************************************************************************************************/
__s32  robin_get_lyric_item_list( __cedar_subtitle_item_t  *lyric_item_list, __u32 item_count )
{
	__cedar_buf_inf_t        *lyric_desc_p = NULL;
	__cedar_subtitle_item_t  *lyric_item_list_p = NULL;
	__u32                     count;
	__u8   err;

    if(EPDK_FALSE == robin_is_open())
    {
        return 0;
    }
		
	if( lyric_item_list == NULL )
	{
		return 0;
	}
		
	g_pend_mutex( robin_cedar_mutex, &err );
	robin_wait_no_file( );

	lyric_desc_p = (__cedar_buf_inf_t *)esMODS_MIoctrl( robin_hced, CEDAR_CMD_GET_SUB_INFO, 0, 
	                                                    (void *)CEDAR_GET_SUB_INF_ALL );
	if( lyric_desc_p == NULL )
	{
		__wrn("Fail in getting lyric item list.\n");
		g_post_mutex( robin_cedar_mutex );
		return 0;
	}

	lyric_item_list_p = (__cedar_subtitle_item_t *)(lyric_desc_p->buf);
	count = min( lyric_desc_p->size, item_count );
	g_memcpy( lyric_item_list, lyric_item_list_p, sizeof(__cedar_subtitle_item_t) * count );
	
	g_post_mutex( robin_cedar_mutex );
	return count;
}



#endif     //  ifndef __robin_lyric_c

/* end of robin_lyric.c */
