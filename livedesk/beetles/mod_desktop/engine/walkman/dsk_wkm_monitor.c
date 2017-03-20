/*
************************************************************************************************************************
*                                                        dsk_walkman
*
*                             Copyright(C), 2009-2010, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : dsk_wkm_monitor.c
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2009.11.19
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
* Gary.Wang      2009.11.19       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  __dsk_wkm_monitor_c
#define  __dsk_wkm_monitor_c


#include "dsk_walkman_i.h"


#define MNT_FILE_LEN_MAX                511


static volatile __s32  monitor_id  = 0; 
static volatile __bool forbid_autoplay = EPDK_TRUE;
static __bool monitor_has_run = EPDK_FALSE;


static void   monitor( void *arg_p );
static __bool _is_filter_file( const char *file );
static __s32 _request_monitor_res( void );
static void  _release_monitor_res( void );



__s32  dsk_wkm_monitor_create( __u8 prio )
{
	/* create monitor thread */
	if( monitor_id == 0 )
	{
		__u32 i;
		
		monitor_has_run = EPDK_FALSE;
		
		monitor_id = esKRNL_TCreate( monitor, NULL, 0x2000, ( EPOS_curid<<8 ) | prio );
		if( monitor_id == 0 )
		{
			__err("Error in creating \"dsk_walkman monitor\" thread.\n");
			return -1;
		}

		/* wait monitor to run */
		for( i = 0; i < 100; i++ )
	    {
	    	if( monitor_has_run )
				break;
			
			g_delay(10);
	    }
		
		forbid_autoplay = EPDK_TRUE;
	}
	
	return 0;
}



void   dsk_wkm_monitor_delete( void )
{
	/* delete monitor thread */
	if( monitor_id != 0 )
	{
		while( esKRNL_TDelReq( monitor_id ) != OS_TASK_NOT_EXIST )
	    {
			g_delay(10);
	    }
		monitor_id = 0;
		__msg("\"dsk_walkman monitor\" thread has been deleted.\n");
	}
}



__s32  __dsk_wkm_monitor_enable( void )
{
    forbid_autoplay = EPDK_FALSE;

    return 0;
}



__s32  __dsk_wkm_monitor_disable( void )
{
    forbid_autoplay = EPDK_TRUE;

    return 0;
}



static void monitor( void *arg_p )
{
	__u8   err;
	
	/* request resouce */
	if( _request_monitor_res() )
	{
	    goto exit;
	}

	monitor_has_run = EPDK_TRUE;
	
    __dsk_wkm_set_open_state( EPDK_TRUE );
		
	while( 1 )
	{
        g_pend_mutex( dsk_wkm_res_mutex, &err );
        
        if( !forbid_autoplay )
        {
            if( __dsk_wkm_get_pause_state() == EPDK_FALSE
                &&__dsk_wkm_get_fsm_state() == CEDAR_STAT_STOP )
            {
                __s32  index;
                __here__
                if(__dsk_wkm_npl_is_dirty() || __dsk_wkm_get_npl_dirty_flag())
            	{                       
                    __dsk_wkm_npl_clear_dirty_flag();
                    __dsk_wkm_set_npl_dirty_flag(EPDK_FALSE); 
                    __here__
                    __dsk_wkm_monitor_disable();
                    __here__
                    g_post_mutex( dsk_wkm_res_mutex );
                    continue;
            	}
                /* auto play the next */                
                index = __dsk_wkm_npl_get_next();
                if( index != -1 )
                {
                    char  file[FS_FULL_PATH_LEN_MAX];
                    
                    /* get the file path */
                    __dsk_wkm_npl_index2file( index, file );
                    
                    if( !_is_filter_file( file ) )
                    {
                        /* call hook */
                        if( cb_auto_play_next != NULL )
                        {
                            esKRNL_CallBack( cb_auto_play_next, cb_ctx_auto_play_next );
                        }
                        /* play */
                        __dsk_wkm_syn_op_play_file( file );

						eLIBs_strcpy(now_playing_file, file);
                    }
                }
                else
                {
                    if( __dsk_wkm_npl_get_total_count() > 0 && __dsk_wkm_npl_get_cur() != -1 )      // no item to play
                    {
                        /* state change to "NOT playing" */
                        forbid_autoplay = EPDK_TRUE;
                        __dsk_wkm_set_playing_state( EPDK_FALSE );
                        __dsk_wkm_set_pause_state( EPDK_FALSE );
                        /* call hook */
                        if( cb_no_item_to_play != NULL )
                        {
                            esKRNL_CallBack( cb_no_item_to_play, cb_ctx_no_item_to_play );
                        }
                    }
                }
            }
        }

    	g_post_mutex( dsk_wkm_res_mutex );

    	if( esKRNL_TDelReq(OS_PRIO_SELF) == OS_TASK_DEL_REQ )
    	{
    		goto exit;
    	}
    	
    	g_delay( 20 );
	}
	
exit:
    _release_monitor_res();
    __dsk_wkm_set_open_state( EPDK_FALSE );
    esKRNL_TDel(OS_PRIO_SELF);
}



static __bool _is_filter_file( const char *file )
{
    __sw_audio_format_e audio_format;
    
    eLIBs_GetAudioFormat( file, (__s32 *)&audio_format );
    if(    audio_format == SW_AUDIO_FORMAT_APE 
        || audio_format == SW_AUDIO_FORMAT_FLAC 
        || audio_format == SW_AUDIO_FORMAT_OGG )
    {
        return EPDK_TRUE;
    }
    else
    {
        return EPDK_FALSE;
    }
}



static __s32  _request_monitor_res( void )
{
	if( __dsk_wkm_feedbackQ_create() )
	    goto error;
	    
	if( __dsk_wkm_cedar_open( ) )
		goto error;
		
	if( __dsk_wkm_get_feedback_msgQ( ) )
		goto error;

    return 0;
    
error:
    _release_monitor_res();
    return -1;
}



static void  _release_monitor_res( void )
{
	__dsk_wkm_cedar_close( );
    dsk_wkm_cedar_msgQ = NULL;
    __dsk_wkm_feedbackQ_delete();
}




#endif     //  ifndef __dsk_wkm_monitor_c

/* end of dsk_wkm_monitor.c */
