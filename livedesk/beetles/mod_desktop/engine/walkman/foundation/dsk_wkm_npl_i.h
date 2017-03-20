/*
************************************************************************************************************************
*                                                       walkman
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : dsk_wkm_npl_i.h
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.10.06
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
* Gary.Wang      2010.10.06        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __DSK_WKM_NPL_I_H__
#define  __DSK_WKM_NPL_I_H__


/***************************************************************************************************
*Name        : __dsk_wkm_npl_add_item
*Prototype   : __s32  __dsk_wkm_npl_add_item( const char *file )
*Arguments   : file     input. full path.
*Return      : ==0       succeed
*              !=0       fail
*Description : add a file to now playlist
*Other       :
***************************************************************************************************/
extern __s32  __dsk_wkm_npl_add_item( const char *file );

/***************************************************************************************************
*Name        : __dsk_wkm_npl_delete_item
*Prototype   : __s32  __dsk_wkm_npl_delete_item( const char *file )
*Arguments   : file     input. full path.
*Return      : ==0       succeed
*              !=0       fail
*Description : delete a file from now playlist
*Other       :
***************************************************************************************************/
extern __s32  __dsk_wkm_npl_delete_item( const char *file );

/***************************************************************************************************
*Name        : __dsk_wkm_npl_clear
*Prototype   : __s32  __dsk_wkm_npl_clear( void )
*Arguments   : void
*Return      : ==0       succeed
*              !=0       fail
*Description : clear the now playlist
*Other       :
***************************************************************************************************/
extern __s32  __dsk_wkm_npl_clear( void );

/***************************************************************************************************
*Name        : __dsk_wkm_npl_playing_seek
*Prototype   : __s32  __dsk_wkm_npl_playing_seek( __s32 origin, __s32 offset )
*Arguments   : origin    input. origin position, as follows : ORC_NPL_SEEK_SET, ORC_NPL_SEEK_CUR ,
*                        and ORC_NPL_SEEK_END.
*              offset    input. offset.
*Return      : ==0       succeed
*              !=0       fail
*Description : seek the playing pointer. There is a playing pointer in the now play list, and it 
*              should be adjusted to always point to the current playing item.
*Other       :
***************************************************************************************************/
extern __s32  __dsk_wkm_npl_playing_seek( __s32 origin, __s32 offset );

/***************************************************************************************************
*Name        : __dsk_wkm_npl_get_next
*Prototype   : __s32  __dsk_wkm_npl_get_next( void )
*Arguments   : void
*Return      : the index of the next file to be played in the now play list. if return -1, that means
*              no item to play in current "play mode" or some error occured.
*Description : get the index of the next file to be played in the now play list according to the 
*              current "play mode" and the current item. this operation will adjust the playing
*              pointer to the next item.
*Other       : 
***************************************************************************************************/
extern __s32  __dsk_wkm_npl_get_next( void );

/***************************************************************************************************
*Name        : __dsk_wkm_npl_get_cur
*Prototype   : __s32  __dsk_wkm_npl_get_cur( void )
*Arguments   : void
*Return      : the index of the item that the playing ponter points to. if return -1, that means fail
*Description : get the index of the item that the playing ponter points to.
*Other       : 
***************************************************************************************************/
extern __s32  __dsk_wkm_npl_get_cur( void );

/***************************************************************************************************
*Name        : __dsk_wkm_npl_file2index
*Prototype   : __s32  __dsk_wkm_npl_file2index( const char *file )
*Arguments   : file     input. full path.
*Return      : the index value of the file in the now play list. If return -1. that means fail.
*Description : get the index value of the file in the now play list.
*Other       :
***************************************************************************************************/
extern __s32  __dsk_wkm_npl_file2index( const char *file );

/***************************************************************************************************
*Name        : __dsk_wkm_npl_index2file
*Prototype   : __s32  __dsk_wkm_npl_index2file( __u32 index, char *file )
*Arguments   : index     input. the item index in the now play list.
*              file      output. the file corresponding to the index.
*Return      : ==0       succeed
*              !=0       fail
*Description : get the file corresponding to its index in the now play list.
*Other       :
***************************************************************************************************/
extern __s32  __dsk_wkm_npl_index2file( __u32 index, char *file );

/***************************************************************************************************
*Name        : __dsk_wkm_npl_invalidate_item
*Prototype   : __s32  __dsk_wkm_npl_invalidate_item( __u32 index )
*Arguments   : index     input. the item index in the now play list.
*Return      : ==0       succeed
*              !=0       fail
*Description : set a item as invalid status.
*Other       :
***************************************************************************************************/
extern __s32  __dsk_wkm_npl_invalidate_item( __u32 index );

/***************************************************************************************************
*Name        : __dsk_wkm_npl_item_is_valid
*Prototype   : __bool __dsk_wkm_npl_item_is_valid( __u32 index )
*Arguments   : index     input. the item index in the now play list.
*Return      : If the item is valid, return "true"; else return "false".
*Description : check whether an item in now play list is valid or not.
*Other       : 
***************************************************************************************************/
extern __bool __dsk_wkm_npl_item_is_valid( __u32 index );

/***************************************************************************************************
*Name        : __dsk_wkm_npl_get_total_count
*Prototype   : __s32  __dsk_wkm_npl_get_total_count( void )
*Arguments   : void
*Return      : the total count of items in the now play list. If return -1. that means fail.
*Description : get the total count of items in the now play list.
*Other       :
***************************************************************************************************/
extern __s32  __dsk_wkm_npl_get_total_count( void );

/***************************************************************************************************
*Name        : __dsk_wkm_npl_get_valid_count
*Prototype   : __s32  __dsk_wkm_npl_get_valid_count( void )
*Arguments   : void
*Return      : the count of the valid items in the now play list. If return -1. that means fail.
*Description : get the count of the valid items in the now play list.
*Other       :
***************************************************************************************************/
extern __s32  __dsk_wkm_npl_get_valid_count( void );

/*
****************************************************************************************************
*Name        : __dsk_wkm_npl_save_list
*Prototype   : __s32  __dsk_wkm_npl_save_list( void )
*Arguments   : void
*Return      : ==0       succeed
*              !=0       fail
*Description : save the now play list to data disk
*Other       :
****************************************************************************************************
*/
extern __s32  __dsk_wkm_npl_save_list( void );

/*
****************************************************************************************************
*Name        : __dsk_wkm_npl_load_last_list
*Prototype   : __s32  __dsk_wkm_npl_load_last_list( void )
*Arguments   : void
*Return      : ==0       succeed
*              !=0       fail
*Description : load the last play list to the npl.
*Other       :
****************************************************************************************************
*/
extern __s32  __dsk_wkm_npl_load_last_list( void );

/*
****************************************************************************************************
*Name        : __dsk_wkm_npl_is_dirty
*Prototype   : __bool  __dsk_wkm_npl_is_dirty( void )
*Arguments   : void
*Return      : if npl is dirty, return EPDK_TRUE; else return EPDK_FALSE.
*Description : check wherther the npl is dirty or not.
*Other       : if the npl is dirty, all operations on npl will be invalid and then you should call
*              __dsk_wkm_npl_clear_dirty_flag() to clear the dirty flag.
****************************************************************************************************
*/
extern __bool  __dsk_wkm_npl_is_dirty( void );

/*
****************************************************************************************************
*Name        : __dsk_wkm_npl_clear_dirty_flag
*Prototype   : __s32  __dsk_wkm_npl_clear_dirty_flag( void )
*Arguments   : void
*Return      : ==0       succeed
*              !=0       fail
*Description : clear dirty flag of npl.
*Other       : if the npl is dirty, all operations on npl will be invalid and then you should call
*              __dsk_wkm_npl_clear_dirty_flag() to clear the dirty flag.
****************************************************************************************************
*/
extern __s32  __dsk_wkm_npl_clear_dirty_flag( void );



#endif     //  ifndef __DSK_WKM_NPL_I_H__

/* end of dsk_wkm_npl_i.h */
