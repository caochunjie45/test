/*
************************************************************************************************************************
*                                                        ORCHID
*                                    the ORCHID Media Information Management Midware
*
*                             Copyright(C), 2006-2008, Softwinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name : drv_orchid.h
*
* Author : Gary.Wang
*
* Version : 1.1.0
*
* Date :  2008.07.17
*
* Description : 
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2008.01.15       1.1.0        build the file
*
* Gary.Wang      2008.07.17       1.1.0        many modification
*
************************************************************************************************************************
*/
#ifndef  __drv_orchid_h
#define  __drv_orchid_h


#define ORC_CMD_BASE_PLAYLIST           0X1000
#define ORC_CMD_BASE_NPL                0X2000
#define ORC_CMD_BASE_MLIB               0X3000
#define ORC_CMD_BASE_ATTACH             0X4000
#define ORC_CMD_BASE_GLOBAL             0X5000

/* ORCHID io control command list */
typedef enum _orc_cmd_e
{
	/* now playlist */
	ORC_CMD_CONSTRUCT_NOW_PLAYLIST = ORC_CMD_BASE_NPL,
	ORC_CMD_DESTROY_NOW_PLAYLIST      ,
	ORC_CMD_ADD_ITEM_TO_NPL           ,
	ORC_CMD_DELETE_ITEM_FROM_NPL      ,
	ORC_CMD_FILE_TO_INDEX_IN_NPL      ,
	ORC_CMD_INDEX_TO_FILE_IN_NPL      ,
	ORC_CMD_CLEAR_NPL                 ,
	ORC_CMD_GET_TOTAL_ITEM_COUNT_OF_NPL,
	ORC_CMD_GET_VALID_ITEM_COUNT_OF_NPL,
	ORC_CMD_SET_PLAY_MODE             ,
	ORC_CMD_GET_PLAY_MODE             ,
	ORC_CMD_PLAYING_SEEK              ,
	ORC_CMD_PLAYING_GET_CUR           ,
	ORC_CMD_PLAYING_AUTO_GET_NEXT     ,
	ORC_CMD_IS_VALID_ITEM_IN_NPL      ,
	ORC_CMD_INVALIDATE_ITEM_IN_NPL    ,
	ORC_CMD_NPL_IS_DIRTY              ,
	ORC_CMD_NPL_CLEAR_DIRTY_FLAG      ,
	ORC_CMD_SAVE_NPL                  ,
	ORC_CMD_LOAD_LAST_NPL             ,
	/* attachment */
	ORC_CMD_SET_ATTACH         = ORC_CMD_BASE_ATTACH,
	ORC_CMD_DELETE_ATTACH             ,
	ORC_CMD_OPEN_ATTACH               ,
	ORC_CMD_CLOSE_ATTACH              ,
	ORC_CMD_GET_ATTACH_SIZE           ,
	ORC_CMD_GET_ATTACH_CONTENT        ,
	/* global */
	ORC_CMD_ADD_FILE_TO_ORCHID = ORC_CMD_BASE_GLOBAL,
	ORC_CMD_DELETE_FILE_FROM_ORCHID   ,
	ORC_CMD_SET_STAR                  ,
	ORC_CMD_SET_RATE                  ,
	ORC_CMD_GET_MEDIA_INFO            ,
	ORC_CMD_FILE_TO_UID               ,
	ORC_CMD_GET_ERROR                 ,
	ORC_CMD_CLR_ERROR                 ,
	ORC_CMD_UPDATE_ORCHID             ,
	ORC_CMD_BEGIN_BROWSE_ALL_ITEM     ,
	ORC_CMD_END_BROWSE_ALL_ITEM       ,
	ORC_CMD_GET_TOTAL_NUMBER          ,
	ORC_CMD_GET_NEXT_ITEM             ,
	ORC_CMD_ORCHID_IS_VALID           ,
	ORC_CMD_REBUILD_ORCHID            ,
	/* browse media lib and playlist module */
	ORC_CMD_OPEN_DIR           = ORC_CMD_BASE_MLIB,
	ORC_CMD_CLOSE_DIR                 ,
	ORC_CMD_REWIND_DIR                ,
	ORC_CMD_GET_ENTRY_COUNT           ,
	ORC_CMD_GET_NEXT_ENTRY            ,
	ORC_CMD_ENTRY_TO_ATTR             ,
	ORC_CMD_ENTRY_TO_NAME             ,
	ORC_CMD_ENTRY_TO_ENCODING         ,
	/* playlist */
	ORC_CMD_CREATE_PLAYLIST    = ORC_CMD_BASE_PLAYLIST,
	ORC_CMD_DELETE_PLAYLIST           ,
	ORC_CMD_ADD_ITEM_TO_PLAYLIST      ,
	ORC_CMD_DELETE_ITEM_FROM_PLAYLIST ,
	
	ORC_CMD_                            // always the last one
}orc_cmd_e;


/* global error code */
typedef enum _orc_error_e
{
	ORC_OK                   = 0,	
	ORC_ERR_TITLE_USED          ,
	ORC_ERR_LIST_END            ,
	ORC_ERR_INVALID_TITLE       ,
	ORC_ERR_INVALID_UID         ,
	ORC_ERR_INVALID_INPUT       ,
	ORC_ERR_INVALID_MEDIA_TYPE  ,
	ORC_ERR_INVALID_SORT_MODE   ,
	ORC_ERR_INVALID_PLAY_MODE   ,
	ORC_ERR_INVALID_STAR        ,
	ORC_ERR_INVALID_IOCTRL_CMD  ,	
	ORC_ERR_INVALID_ITEM        ,
	ORC_ERR_FULL                ,
	ORC_ERR_NOW_PLAYLIST_EMPTY  ,
	ORC_ERR_NO_ITEM_TO_PLAY     ,
	ORC_ERR_SLOP_OVER           ,
	ORC_ERR_EXTRACT_INFO_FAIL   ,
	ORC_ERR_NO_ITEM_BEING_PLAYED,
	ORC_ERR_FILE_OP_FAIL        ,
	ORC_ERR_MALLOC_FAIL         ,
	ORC_ERR_FATAL_ERR           ,
	ORC_ERR_NPL_HANDLE_DIRTY    ,
	ORC_ERR_NPL_WORK_MODE_EXIST ,
	ORC_ERR_NPL_IN_MANAGE_MODE  ,
	ORC_ERR_NPL_ILLEGAL_OP      ,
	
	ORC_ERR                     = -1             // always the last one.
}orc_error_e;

              
/* media type */ 
typedef enum _orc_media_type_e
{                                 
	ORC_MEDIA_TYPE_AUDIO = 0,
	ORC_MEDIA_TYPE_VIDEO    ,
	ORC_MEDIA_TYPE_PIC	    ,
	ORC_MEDIA_TYPE_EBOOK	,
	ORC_MEDIA_TYPE_FLASH	,
	
	ORC_MEDIA_TYPE_UNKNOWN = -1
}orc_media_type_e;
                   
                           	  
/* play mode */                  
typedef enum _orc_play_mode_e
{
	ORC_PLAY_MODE_ONLY_ONCE = 0, 
	ORC_PLAY_MODE_ROTATE_ONE   ,
	ORC_PLAY_MODE_ROTATE_ALL   ,
	ORC_PLAY_MODE_SEQUENCE     ,
	ORC_PLAY_MODE_RANDOM       ,

	ORC_PLAY_MODE_UNKNOWN   = -1                  // always the last one
}orc_play_mode_e;


/* npl work mode */
typedef enum
{
	ORC_NPL_WORK_MODE_MANAGE = 0,       // ����ģʽ�����Խ������в�����һ���о�����ڴ�ģʽ��������ͬý�����͵ľ���Ĳ�������ʧ�ܡ�ÿ��ý�����ͣ����ֻ����һ��������ڴ�ģʽ��
	ORC_NPL_WORK_MODE_USE       ,       // ʹ��ģʽ�����Զ������Բ���playing pointer. ÿ��ý�����ͣ����ֻ����һ��������ڴ�ģʽ��
	ORC_NPL_WORK_MODE_QUERY     ,       // ��ѯģʽ��ֻ��������ͬʱ�Դ�ģʽ�򿪶�������

    ORC_NPL_WORK_MODE_UNKNOWN = -1
}orc_npl_work_mode_e;

                               	  
#define ORC_MEDIA_LIB_ROOT                        "media_lib_root"
#define ORC_PLAYLIST_ROOT                         "playlist_root"

#define AUDIO_TYPE_STR                            "audio"
#define VIDEO_TYPE_STR                            "video"
#define PIC_TYPE_STR                              "pic"
#define EBOOK_TYPE_STR							  "ebook"
#define GAME_TYPE_STR							  "game"

/* category attribute */
#define ARTIST                                    "artist" 
#define ALBUM                                     "album" 
#define GENRE                                     "genre" 
#define YEAR                                      "year" 
#define STAR                                      "star"
#define DATE                                      "date"
#define ORC_ALL                                   "all"
#define UNKNOWN_VARIETY                           "Unknown"

/* variety name */
#define ORC_VARIETY_NAME_0STAR                    "0star"
#define ORC_VARIETY_NAME_1STAR                    "1star"
#define ORC_VARIETY_NAME_2STAR                    "2star"
#define ORC_VARIETY_NAME_3STAR                    "3star"
#define ORC_VARIETY_NAME_4STAR                    "4star"
#define ORC_VARIETY_NAME_5STAR                    "5star"

/* orchid entry attribute */                  
typedef enum _orc_entry_attr_e
{
	ORC_ENTRY_ATTR_MLIB_ROOT   = 0,
	ORC_ENTRY_ATTR_MLIB_MEDIA_TYPE,
	ORC_ENTRY_ATTR_MLIB_CATE_ATTR ,
	ORC_ENTRY_ATTR_MLIB_VARIETY   ,
	ORC_ENTRY_ATTR_ITEM           ,
	ORC_ENTRY_ATTR_PL_ROOT        ,
	ORC_ENTRY_ATTR_PL_PLAYLIST    ,
	ORC_ENTRY_ATTR_PL_ITEM        ,
	
	ORC_ENTRY_ATTR_UNKNOWN    = -1
}orc_entry_attr_e;


/* origin mode */                               	  
#define ORC_NPL_SEEK_SET                    	  0
#define ORC_NPL_SEEK_CUR                    	  1
#define ORC_NPL_SEEK_END                    	  2
                            
                               
#define ORC_MEDIA_FILE_NAME_SIZE          	    ( 255 + 512 ) // add by Gary 2010-6-9 11:33:41
#define ORC_TITLE_SIZE                    	     47
#define ORC_ARTIST_SIZE                   	     47
#define ORC_ALBUM_SIZE                    	     47
#define ORC_GENRE_SIZE                    	     31
#define ORC_FILE_FORMAT_SIZE              	      7
#define ATTACH_TITLE_MAX_SIZE                     7

#define ORC_STR_MAX_SIZE                         47


#define ORC_ATTACH_TITLE_THUMB                    "THUMB"


typedef struct _orc_date_t
{
	__u8   day;
	__u8   month;
	__u16  year;
}orc_date_t;


typedef struct _orc_id3_pic_info_t
{
	__u32               length;      // 0 means no picture. The length of the picture info in the file.
	__u32               offset;      // The offset of the picture info in the file.
	__sw_pic_format_e   format;      // jpg, bmp, and so on
}orc_id3_pic_info_t;


typedef struct _audio_info_t
{
	__s32  media_type;
	__u32  UID;
	char   media_file[ORC_MEDIA_FILE_NAME_SIZE+1];
	char   title[ORC_TITLE_SIZE+1];
	__s32  title_encoding;
	char   artist[ORC_ARTIST_SIZE+1];                   	// null string : artist is unknown.
	__s32  artist_encoding;
	char   album[ORC_ALBUM_SIZE+1];                     	// null string : album is unknown.
	__s32  album_encoding;
	char   genre[ORC_GENRE_SIZE+1];                     	// null string : genre is unknown.
	__s32  genre_encoding;
	__s32  file_format;
	__u32  rate;                                           	// 0 : rate is unknown; unit is Kbps.
	__u32  star;
	__u32  year;
	__u32  duration;                                	    // 0 : the duration is unknown.
	__u32  sample_rate;
	orc_id3_pic_info_t  album_cover_info;
}audio_info_t;


typedef struct _video_info_t
{
	__s32  media_type;
	__u32  UID;
	char   media_file[ORC_MEDIA_FILE_NAME_SIZE+1];
	char   title[ORC_TITLE_SIZE+1];
	__s32  title_encoding;
	char   artist[ORC_ARTIST_SIZE+1];                   	// null string : artist is unknown.
	__s32  artist_encoding;
	__s32  file_format;
	__u32  rate;                                           	// 0 : rate is unknown; unit is Kbps.
	__u32  star;
	__u32  year;
	__u32  duration;                                	    // 0 : the duration is unknown.
}video_info_t;


typedef struct _pic_info_t
{
	__s32  media_type;
	__u32  UID;
	char   media_file[ORC_MEDIA_FILE_NAME_SIZE+1];
	char   title[ORC_TITLE_SIZE+1];
	__s32  title_encoding;
	__s32  file_format;
	__u32  star;
	orc_date_t date;                                        // 0 : the date is unknown
}pic_info_t;


typedef union _media_info_t
{
	audio_info_t  audio_info;
	video_info_t  video_info;
	pic_info_t    pic_info;
}media_info_t;


typedef __u32      HDIR;
typedef __u32      HENT;
typedef __u32      HATH;
typedef __u32      HNPL;
typedef __u32      HAIS;



/* ORCHID io control argument type list */

typedef struct _orc_arg_seek_t
{
	HNPL   hnpl;              // ���롣now playlist����ľ��
	__s32  offset;            // ���롣ƫ����
	__s32  origin;            // ���롣���λ�ã���ѡֵ�����֣�
	                              // ORC_NPL_SEEK_SET���ӿ�ʼ����
	                              // ORC_NPL_SEEK_CUR���ӵ�ǰλ�ã�
	                              // ORC_NPL_SEEK_END����ĩβ����
}orc_arg_seek_t;

typedef struct _orc_arg_set_attach_t
{
	__u32      UID;           // ���롣��Ŀ��UID
	const char *title;        // ���롣�����ı���
	const void *buf;          // ���롣����������
	__u32      size;          // ���롣�����ĳߴ�
}orc_arg_set_attach_t;

typedef struct _orc_arg_get_attach_t
{
	HATH    hath;             // ���롣��������ľ��
	void    *buf;             // �����������Ŷ�ȡ���ĸ������ݵĻ�����
	__u32   size;             // ���롣ϣ����ȡ�ĳߴ�ֵ�������ֵС�ڵ��ڸ��������ĳߴ�ֵ��������
	                              // size ��С�ĸ������ݣ����򣬱���
}orc_arg_get_attach_t;

typedef struct _orc_arg_index_to_file_t
{
	HNPL    hnpl;             // ���롣now playlist����ľ��
	__u32   index;            // ���롣��Ŀ��now playlist�е������š�
	char   *file;             // �������Ŀ���ļ���
}orc_arg_index_to_file_t;



typedef struct _orc_arg_npl_t
{
	orc_media_type_e     media_type;    // ���롣ý������
	orc_npl_work_mode_e  work_mode;     // ���롣����ģʽ��
}orc_arg_npl_t;
	

extern __s32 DRV_OrchidInit(void);
extern __s32 DRV_OrchidExit(void);
extern __mp *DRV_OrchidOpen(__u32 id, __u32 mode);
extern __s32 DRV_OrchidClose(__mp *mp);
extern __u32 DRV_OrchidRead(void *pdata, __u32 size, __u32 n, __mp *mp);
extern __u32 DRV_OrchidWrite(const void *pdata, __u32 size, __u32 n, __mp *mp);
extern __s32 DRV_OrchidIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer);
/*--------------------------------------------------------------------------------------------------
*                             construct_now_playlist
* mp          :  handle
* cmd         :  ORC_CMD_CONSTRUCT_NOW_PLAYLIST
* aux         :  void.     
* pbuffer     :  orc_arg_npl_t *arg;    // ���롣
* return type :  HNPL
*   NULL         ����ʧ��
*   !NULL        �����ɹ�������ֵΪ�������now playlist�ľ��
* description :  ����һ�� now playlist ����
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             destroy_now_playlist
* mp          :  handle
* cmd         :  ORC_CMD_DESTROY_NOW_PLAYLIST
* aux         :  none
* pbuffer     :  HNPL hnpl;             // ���롣now playlist����ľ��
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ����һ�� now playlist ����
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             add_item_to_npl
* mp          :  handle
* cmd         :  ORC_CMD_ADD_ITEM_TO_NPL
* aux         :  HNPL  hnpl;             // ���롣now playlist����ľ��
* pbuffer     :  const char *file;       // ���롣�ļ�����
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ��ĳ��Ŀ��ӵ�now playlist�С�
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             delete_item_from_npl
* mp          :  handle
* cmd         :  ORC_CMD_DELETE_ITEM_FROM_NPL
* aux         :  HNPL  hnpl;             // ���롣now playlist����ľ��
* pbuffer     :  const char *file;       // ���롣�ļ�����
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ��now_playlist��ɾ��ĳ����Ŀ��
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             file_to_index_in_npl
* mp          :  handle
* cmd         :  ORC_CMD_FILE_TO_INDEX_IN_NPL
* aux         :  HNPL  hnpl;             // ���롣now playlist����ľ��
* pbuffer     :  const char *file;       // ���롣�ļ�����
* return type :  __u32
*  0xFFFFFFFF    ����ʧ��
*  other value   �����ɹ���������Ŀ��now playlist�е�������
* description :  ��ȡ��Ŀ��now playlist�е�������
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             index_to_file_in_npl
* mp          :  handle
* cmd         :  ORC_CMD_INDEX_TO_FILE_IN_NPL
* aux         :  none
* pbuffer     :  orc_arg_index_to_file_t  *arg;   // ����
* return type :  __u32
*  0xFFFFFFFF    ����ʧ��
*  other value   �����ɹ���������Ŀ��UID��
* description :  ������Ŀ��now playlist�е������Ż�ȡ��Ŀ��UID��
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             clear_npl
* mp          :  handle
* cmd         :  ORC_CMD_CLEAR_NPL
* aux         :  none
* pbuffer     :  HNPL hnpl;             // ���롣now playlist����ľ��
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ��� now playlist.
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             get_total_item_count_of_npl
* mp          :  handle
* cmd         :  ORC_CMD_GET_TOTAL_ITEM_COUNT_OF_NPL
* aux         :  none
* pbuffer     :  HNPL hnpl;             // ���롣now playlist����ľ��
* return type :  __u32
*	0xFFFFFFFF   ����ʧ��
*   ����ֵ       �����ɹ���now playlist����Ŀ������Ŀ
* description :  ��ȡ now playlist ����Ŀ������Ŀ
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             get_valid_item_count_of_npl
* mp          :  handle
* cmd         :  ORC_CMD_GET_VALID_ITEM_COUNT_OF_NPL
* aux         :  none
* pbuffer     :  HNPL hnpl;             // ���롣now playlist����ľ��
* return type :  __u32
*	0xFFFFFFFF   ����ʧ��
*   ����ֵ       �����ɹ���now playlist����Ч��Ŀ������Ŀ
* description :  ��ȡ now playlist ����Ч��Ŀ������Ŀ
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             set_play_mode
* mp          :  handle
* cmd         :  ORC_CMD_SET_PLAY_MODE
* aux         :  HNPL hnpl;             // ���롣now playlist����ľ��
* pbuffer     :  __s32 play_mode        // ���롣����ģʽ
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ���ò���ģʽ
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             get_play_mode
* mp          :  handle
* cmd         :  ORC_CMD_GET_PLAY_MODE
* aux         :  HNPL hnpl;             // ���롣now playlist����ľ��
* pbuffer     :  __s32 *play_mode_p     // ���������ģʽ
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ���ò���ģʽ
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             playing_seek
* mp          :  handle
* cmd         :  ORC_CMD_PLAYING_SEEK
* aux         :  none
* pbuffer     :  orc_arg_seek_t  *arg;   // ���롣
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ���ò���ָ��(playing pointer)��λ��
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             playing_get_cur
* mp          :  handle
* cmd         :  ORC_CMD_PLAYING_GET_CUR
* aux         :  HNPL   hnpl;         // ���롣now playlist����ľ��
* pbuffer     :  __u32  *index_p;     // ���������playing pointer��ָ����Ŀ��now_playlist�е�������
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ��ȡ����ָ��(playing pointer)��ǰ��ָ����Ŀ��now_playlist�е������š�
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             playing_auto_get_next
* mp          :  handle
* cmd         :  ORC_CMD_PLAYING_AUTO_GET_NEXT
* aux         :  HNPL   hnpl;         // ���롣now playlist����ľ��
* pbuffer     :  __u32  *index_p;     // ������洢��Ŀ��now_playlist�е�������
* return type :  __s32
*	ORC_OK                       �����ɹ�
*   ORC_ERR_NO_ITEM_TO_PLAY      �ڵ�ǰģʽ�£��Ҳ����ܲ��ŵ���Ŀ
*   ORC_ERR_NOW_PLAYLIST_EMPTY   now playlist is empty
*   ORC_ERR                      ����ʧ��
* description :  ���Զ�����ģʽ�£��������õĲ���ģʽ(play mode)�����һ�������ŵ���Ŀ��now_playlist
*                �е������š�
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             invalidate_item_in_npl
* mp          :  handle
* cmd         :  ORC_CMD_INVALIDATE_ITEM_IN_NPL
* aux         :  HNPL hnpl;             // ���롣now playlist����ľ��
* pbuffer     :  __u32 index;           // ���롣��Ŀ������ֵ
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ��ĳ��Ŀ����Ϊ����Ч��״̬
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             is_valid_item_in_npl
* mp          :  handle
* cmd         :  ORC_CMD_IS_VALID_ITEM_IN_NPL
* aux         :  HNPL hnpl;             // ���롣now playlist����ľ��
* pbuffer     :  __u32 index;           // ���롣��Ŀ������ֵ
* return type :  __bool
*                �������Ŀ��Ч�����ء��桰�����򷵻ء��١�
* description :  �쿴ĳ��Ŀ�Ƿ���Ч
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             npl_clear_dirty_flag
* mp          :  handle
* cmd         :  ORC_CMD_NPL_CLEAR_DIRTY_FLAG
* aux         :  none
* pbuffer     :  HNPL hnpl;             // ���롣now playlist����ľ��
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ����˾����dirty��־
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             npl_is_dirty
* mp          :  handle
* cmd         :  ORC_CMD_NPL_IS_DIRTY
* aux         :  none
* pbuffer     :  HNPL hnpl;             // ���롣now playlist����ľ��
* return type :  __bool
* description :  �鿴��now playlist�Ƿ�Ϊ"dirty"�����Ϊdirty����Ҫ����ORC_CMD_IS_VALID_ITEM_IN_NPL
*                ���"dirty"��־��Ȼ���ٽ�������������
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             save_npl
* mp          :  handle
* cmd         :  ORC_CMD_SAVE_NPL
* aux         :  none
* pbuffer     :  HNPL hnpl;             // ���롣now playlist����ľ��
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ���� now playlist.
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             load_last_npl
* mp          :  handle
* cmd         :  ORC_CMD_LOAD_LAST_NPL
* aux         :  none
* pbuffer     :  HNPL hnpl;             // ���롣now playlist����ľ��
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ���ϴεĲ����б���ص�npl��
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             set_attach
* mp          :  handle
* cmd         :  ORC_CMD_SET_ATTACH
* aux         :  none
* pbuffer     :  orc_arg_set_attach_t *
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ������Ŀĳ������������
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             delete_attach
* mp          :  handle
* cmd         :  ORC_CMD_DELETE_ATTACH
* aux         :  __u32      UID;        // ���롣��Ŀ��UID  
* pbuffer     :  const char *title;     // ���롣�����ı��� 
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ɾ����Ŀ��ĳ������
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             open_attach
* mp          :  handle
* cmd         :  ORC_CMD_OPEN_ATTACH
* aux         :  __u32      UID;        // ���롣��Ŀ��UID  
* pbuffer     :  const char *title;     // ���롣�����ı��� 
* return type :  HATH
*   NULL         ����ʧ��
*   !NULL        �����ɹ�������HATH���͵ľ��
* description :  ����Ŀ��ĳ���������Ա��ȡ��������Ϣ
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             close_attach
* mp          :  handle
* cmd         :  ORC_CMD_CLOSE_ATTACH
* aux         :  none
* pbuffer     :  HATH   hath;           // ���롣��������ľ��
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  �رո�����
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             get_attach_size
* mp          :  handle
* cmd         :  ORC_CMD_GET_ATTACH_SIZE
* aux         :  none
* pbuffer     :  HATH   hath;           // ���롣��������ľ��
* return type :  __u32
*   0xFFFFFFFF   ����ʧ��
*   ����ֵ       �����ɹ�������ֵ���Ǹ����ĳߴ�ֵ
* description :  ��ȡ�����ĳߴ�ֵ��
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             get_attach_content
* mp          :  handle
* cmd         :  ORC_CMD_GET_ATTACH_CONTENT
* aux         :  none
* pbuffer     :  orc_arg_get_attach_t *
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ��ȡ����������
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             add_file_to_orchid
* mp          :  handle
* cmd         :  ORC_CMD_ADD_FILE_TO_ORCHID
* aux         :  const char *file;      // ���롣�ļ�������·����
* pbuffer     :  __u32   *UID_p;        // ��������ļ���UID���˲�������Ϊ��
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ���ļ���ӵ�ORCHIDģ�顣
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             delete_file_from_orchid
* mp          :  handle
* cmd         :  ORC_CMD_DELETE_FILE_FROM_ORCHID
* aux         :  none
* pbuffer     :  const char *file;      // ���롣�ļ�������·����
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ��orchidģ����ɾ���ļ�
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             set_star
* mp          :  handle
* cmd         :  ORC_CMD_SET_STAR
* aux         :  __u32      UID;        // ���롣��Ŀ��UID
* pbuffer     :  __s32      star;       // ���롣�Ǽ�����ѡֵ: 0--5
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ����ĳ��Ŀ���Ǽ�
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             set_rate
* mp          :  handle
* cmd         :  ORC_CMD_SET_RATE
* aux         :  __u32      UID;        // ���롣��Ŀ��UID
* pbuffer     :  __s32      rate;       // ���롣���ʣ���λΪ��Kbps     
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ����ĳ��Ŀ������
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             get_media_info
* mp          :  handle
* cmd         :  ORC_CMD_GET_MEDIA_INFO
* aux         :  __u32      UID;        // ���롣��Ŀ��UID
* pbuffer     :  media_info_t  *info_p; // �������Ż�ȡ����Ϣ
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ��ȡ��Ŀ��ý����Ϣ��
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             file_to_UID
* mp          :  handle
* cmd         :  ORC_CMD_FILE_TO_UID
* aux         :  none
* pbuffer     :  const char *file;      // ���롣�ļ�������·����
* return type :  __u32
*   0xFFFFFFFF   ����ʧ��
*   ����ֵ       �����ɹ�������ֵ�����ļ���Ӧ��UID
* description :  ��ȡ�ļ���Ӧ��UID
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             get_error
* mp          :  handle
* cmd         :  ORC_CMD_GET_ERROR
* aux         :  none
* pbuffer     :  none
* return type :  __s32        ������
* description :  ��ȡ������
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             clr_error
* mp          :  handle
* cmd         :  ORC_CMD_CLR_ERROR
* aux         :  none
* pbuffer     :  none
* return type :  void
* description :  ��մ�����
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             update_orchid
* mp          :  handle
* cmd         :  ORC_CMD_UPDATE_ORCHID
* aux         :  none
* pbuffer     :  none
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ����orchidģ��
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             add_portable_disks
* mp          :  handle
* cmd         :  ORC_CMD_ADD_PORTABLE_DISKS
* aux         :  none
* pbuffer     :  none
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ���п��ƶ��洢�豸���뵽ϵͳ��ʱ�����ô˺�������orchidģ��
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             remove_portable_disks
* mp          :  handle
* cmd         :  ORC_CMD_REMOVE_PORTABLE_DISKS
* aux         :  none
* pbuffer     :  none
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  �����ƶ��洢�豸��ϵͳ���Ƴ�ʱ�����ô˺�������orchidģ��
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             begin_browse_all_item
* mp          :  handle
* cmd         :  ORC_CMD_BEGIN_BROWSE_ALL_ITEM
* aux         :  none
* pbuffer     :  __s32 media_type;           // ���롣ý������
* return type :  HAIS
*	NULL         ����ʧ��
*   ����ֵ       �����ɹ�������һ���������
* description :  ��ȡ���ĳ����ý������item�ľ��
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             end_browse_all_item
* mp          :  handle
* cmd         :  ORC_CMD_END_BROWSE_ALL_ITEM
* aux         :  none
* pbuffer     :  HAIS  hais;           // ���롣���ĳ����ý������item�ľ��
* return type :  __s32
*	ORC_ERR      ����ʧ��
*   ORC_OK       �����ɹ�
* description :  �ر����ĳ����ý������item�ľ��
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             get_total_number
* mp          :  handle
* cmd         :  ORC_CMD_GET_TOTAL_NUMBER
* aux         :  none
* pbuffer     :  HAIS  hais;           // ���롣���ĳ����ý������item�ľ��
* return type :  __u32
*	0xFFFFFFFF   ����ʧ��
*   ����ֵ       �����ɹ���������Ŀ������Ŀ
* description :  ��ȡĳ����ý��������Ŀ������Ŀ
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             get_next_item
* mp          :  handle
* cmd         :  ORC_CMD_GET_NEXT_ITEM
* aux         :  HAIS  hais;           // ���롣���ĳ����ý������item�ľ��
* pbuffer     :  uint32 *UID_p;        // ������洢��Ŀ��UID
* return type :  __s32
*	ORC_ERR            ����ʧ��
*   ORC_OK             �����ɹ�
*   ORC_ERR_LIST_END   �����б�ĩβ
* description :  ��ȡĳ����ý����һ����Ŀ��UID
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             rebuild_orchid
* mp          :  handle
* cmd         :  ORC_CMD_REBUILD_ORCHID
* aux         :  none
* pbuffer     :  none
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ��orchid��Ч������£��ؽ�orchidģ��
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             orchid_is_valid
* mp          :  handle
* cmd         :  ORC_CMD_ORCHID_IS_VALID
* aux         :  none
* pbuffer     :  none
* return type :  __bool
*	EPDK_TRUE    orchid��Ч
*   EPDK_FALSE   orchid��Ч
* description :  �쿴orchid��ǰ�Ƿ���Ч������Ч���������rebuild_orchid�ӿ��ؽ�orchid����������ʹ��
*                orchid.
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             open_dir
* mp          :  handle
* cmd         :  ORC_CMD_OPEN_DIR
* aux         :  none
* pbuffer     :  const char *path;      // ���롣Ŀ¼������·��
* return type :  HDIR
*   NULL         ����ʧ��
*   !NULL        �����ɹ�������HDIR���͵ľ��
* description :  ��Ŀ¼������
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             close_dir
* mp          :  handle
* cmd         :  ORC_CMD_CLOSE_DIR
* aux         :  none
* pbuffer     :  HDIR   hdir;           // ���롣Ŀ¼���
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  �ر�Ŀ¼������
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             rewind_dir
* mp          :  handle
* cmd         :  ORC_CMD_REWIND_DIR
* aux         :  none
* pbuffer     :  HDIR   hdir;           // ���롣Ŀ¼���
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  �����ָ��������λ����ʼ�㣬�Ա��ͷ��ʼ���ζ�ȡ���е�entry.
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             get_entry_count
* mp          :  handle
* cmd         :  ORC_CMD_GET_ENTRY_COUNT
* aux         :  none
* pbuffer     :  HDIR   hdir;           // ���롣Ŀ¼���
* return type :  __u32
*   0xFFFFFFFF   ����ʧ��
*   ����ֵ       �����ɹ�������ֵ����Ŀ¼��entry������Ŀ
* description :  ��ȡĿ¼��entry������Ŀ
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             get_next_entry
* mp          :  handle
* cmd         :  ORC_CMD_GET_NEXT_ENTRY
* aux         :  none
* pbuffer     :  HDIR   hdir;           // ���롣Ŀ¼���
* return type :  HENT
*   NULL         ����ʧ�ܻ���NO more entry. ��ʱ��ѯ�����룬��������ֵΪORC_ERR_LIST_END����˵��NO 
*                    more entry; ������˵���д�������
*   !NULL        �����ɹ�������HENT���͵ľ��
* description :  ��ȡ��һ��entry��
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             entry_to_attr
* mp          :  handle
* cmd         :  ORC_CMD_ENTRY_TO_ATTR
* aux         :  none
* pbuffer     :  HENT   hent;           // ���롣entry���
* return type :  __s32
*	ORC_ENTRY_ATTR_UNKNOWN  ����ʧ��
*   ����ֵ                  �����ɹ�������ֵ����entry������ֵ
* description :  ��ȡentry������ֵ
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             entry_to_name
* mp          :  handle
* cmd         :  ORC_CMD_ENTRY_TO_NAME
* aux         :  HENT   hent;           // ���롣entry���
* pbuffer     :  void   *buf;           // ��������entry��������ORC_ENTRY_ATTR_ITEM, ��ôvoid *buf
*                                          // ����ʵ����Ϊuint32 *UID_p, ���������Ŀ��UID�����
*                                          // entry�����Բ���ORC_ENTRY_ATTR_ITEM����ôvoid *buf����
*                                          // ʵ����Ϊchar *name, �������entry��name����ʱname�ĳ�
*                                          // ��ӦΪORC_STR_MAX_SIZE+1��
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ��ȡentry��name��UID
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             entry_to_encoding
* mp          :  handle
* cmd         :  ORC_CMD_ENTRY_TO_ENCODING
* aux         :  none
* pbuffer     :  HENT   hent;           // ���롣entry���
* return type :  __s32
*	ORC_ENTRY_ENCODING_UNKNOWN  ����ʧ��
*   ����ֵ                      �����ɹ�������ֵ����entry������ֵ
* description :  ��ȡentry���ַ���������
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             create_playlist
* mp          :  handle
* cmd         :  ORC_CMD_CREATE_PLAYLIST
* aux         :  const char *title;       // ���롣�����б�ı��⣬����ĳ��ȱ���С��ORC_TITLE_SIZE.
* pbuffer     :  __s32 media_type;        // ���롣ý�����͡�
* return type :  __s32
*	ORC_OK              �ɹ�����һ���µĲ����б�playlist
*   ORC_ERR             ����ʧ��
*   ORC_ERR_TITLE_USED  ���д˱���title�Ĳ����б��Ѿ�����
* description :  ����һ���µĲ����б�
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             delete_playlist
* mp          :  handle
* cmd         :  ORC_CMD_DELETE_PLAYLIST
* aux         :  none
* pbuffer     :  const char *title;       // ���롣�����б�ı��⣬����ĳ��ȱ���С��ORC_TITLE_SIZE.
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ɾ��һ�������б�
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             add_item_to_playlist
* mp          :  handle
* cmd         :  ORC_CMD_ADD_ITEM_TO_PLAYLIST
* aux         :  const char *file;      // ����
* pbuffer     :  const char *title;     // ���롣�����б�ı��⣬����ĳ��ȱ���С��ORC_TITLE_SIZE
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ��ĳ����Ŀ��ӵ�ĳ�����б��С�
--------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------
*                             delete_item_from_playlist
* mp          :  handle
* cmd         :  ORC_CMD_DELETE_ITEM_FROM_PLAYLIST
* aux         :  const char *file;      // ����
* pbuffer     :  const char *title;     // ���롣�����б�ı��⣬����ĳ��ȱ���С��ORC_TITLE_SIZE
* return type :  __s32
*	ORC_OK       �����ɹ�
*   ORC_ERR      ����ʧ��
* description :  ��ĳ��Ŀ��ĳ�������б���ɾ����
--------------------------------------------------------------------------------------------------*/






#endif     //  ifndef __drv_orchid_h

/* end of drv_orchid.h  */
