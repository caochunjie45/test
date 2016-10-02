/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                               Desktop Module
*
*                                    (c) Copyright 2006-2010, kevin.z China
*                                             All Rights Reserved
*
* File    : mod_cat.h
* By      : kinier
* Version : v1.0
* Date    : 2010-9-14 19:13
* Descript:
* Update  : date                auther      ver     notes
*********************************************************************************************************
*/

#ifndef __MOD_CAT_H__
#define __MOD_CAT_H__

#include "epdk.h"

#define CAT_MAX_FULL_PATH_LEN  1024

typedef __u32      HEXPLR;
typedef __u32      HCAT;
typedef __u32      HDRI;
typedef __u32      HORC;
typedef __u32      HMDB;
typedef char *     ENTRY;


#define CAT_USB_DISK    "ExternalDisk1"
#define CAT_TF_CARD     "ExternalDisk0"
#define CAT_MEDIALIB    "media_lib_root"
#define CAT_LOCAL_DISK  "LocalDisk"
#define CAT_CDROM       "ExternalDisk2"

/*!
  * \brief
  *     AUDIO文件格式
*/
typedef enum
{
    CAT_AUDIO_FORMAT_AAC = 0x00,
    CAT_AUDIO_FORMAT_AC3,
    CAT_AUDIO_FORMAT_AMR,
    CAT_AUDIO_FORMAT_APE,
    CAT_AUDIO_FORMAT_ATRC,
    CAT_AUDIO_FORMAT_DTS,
    CAT_AUDIO_FORMAT_FLAC,
    CAT_AUDIO_FORMAT_MID,    
    CAT_AUDIO_FORMAT_MP3, 
    CAT_AUDIO_FORMAT_OGG,   
    CAT_AUDIO_FORMAT_RA,
    CAT_AUDIO_FORMAT_WAV,
    CAT_AUDIO_FORMAT_WMA,    
    CAT_AUDIO_FORMAT_UNKNOWN,
}cat_audio_format_t;


/*!
  * \brief
  *     VIDEO文件格式
*/
typedef enum
{
    CAT_VIDEO_FORMAT_3GP = 0x20,
    CAT_VIDEO_FORMAT_ASF,
    CAT_VIDEO_FORMAT_AVI,
    CAT_VIDEO_FORMAT_DAT,
    CAT_VIDEO_FORMAT_FLV,
    CAT_VIDEO_FORMAT_MKV,
    CAT_VIDEO_FORMAT_MOV,
    CAT_VIDEO_FORMAT_MP4,
    CAT_VIDEO_FORMAT_MPG,
    CAT_VIDEO_FORMAT_PMP,
    CAT_VIDEO_FORMAT_PSR,
    CAT_VIDEO_FORMAT_RM,
    CAT_VIDEO_FORMAT_RMVB,
    CAT_VIDEO_FORMAT_TP,
    CAT_VIDEO_FORMAT_TS,    
    CAT_VIDEO_FORMAT_VOB,    
    CAT_VIDEO_FORMAT_WMV,    
    CAT_VIDEO_FORMAT_UNKNOWN
}cat_video_format_t;


/*!
  * \brief
  *     图片文件格式
*/
typedef enum
{
    CAT_PIC_FORMAT_BMP = 0x40,
    CAT_PIC_FORMAT_GIF,
    CAT_PIC_FORMAT_JPG,
    CAT_PIC_FORMAT_PNG,	
    CAT_PIC_FORMAT_UNKNOWN
}cat_pic_format_t;


/*!
  * \brief
  *     EBOOK文件格式
*/
typedef enum
{
    CAT_EBOOK_FORMAT_TXT = 0x50,
    CAT_EBOOK_FORMAT_C,
    CAT_EBOOK_FORMAT_H,
    CAT_EBOOK_FORMAT_INI,
    CAT_EBOOK_FORMAT_LRC,
    CAT_EBOOK_FORMAT_SRT,
    CAT_EBOOK_FORMAT_PDF,
    CAT_EBOOK_FORMAT_EPUB,
    CAT_EBOOK_FORMAT_FB2,
    CAT_EBOOK_FORMAT_PDB,
    CAT_EBOOK_FORMAT_RTF,
    CAT_EBOOK_FORMAT_HTML,
    CAT_EBOOK_FORMAT_CHM,
    CAT_EBOOK_FORMAT_HTM,
    
    CAT_EBOOK_FORMAT_UNKNOWN
}cat_ebook_format_t;


/*!
  * \brief
  *     GAME文件格式
*/
typedef enum
{
    CAT_GAME_FORMAT_EPOS = 0x70,
    CAT_GAME_FORMAT_UNKNOWN
}cat_game_format_t;


typedef enum
{
    CAT_FLASH_FORMAT_SWF = 0x80,
    CAT_FLASH_FORMAT_UNKNOWN
}cat_flash_format_t;


typedef enum
{
    CAT_FIRMWARE_FORMAT_IMG = 0x90,
    CAT_FIRMWARE_FORMAT_UNKNOWN	
}cat_firmware_format_t;


/*!
  * \brief
  *     浏览扫描的类型
*/
typedef enum
{
    CAT_SCAN_ALL_DEVICE,    //根扫描，扫描结果有CAT_USB_DISK，CAT_TF_CARD，CAT_MEDIALIB，CAT_LOCAL_DISK,CAT_CDROM
    CAT_SCAN_ALL_PATI,      //根扫描，但以分区为根，即返回CAT_MEDIALIB，e,f...等等
    CAT_SCAN_PARTI,         //分区扫描，只扫描某个设备的分区
    CAT_SCAN_DIR,           //普通目录扫描，只扫描当前路径，不递归
    CAT_SCAN_MEIDA_ALL,     //目录全扫描（指定路径下所有文件扫描，包括子目录下的文件）
}cat_scan_type_t;


/*!
  * \brief
  *     浏览的媒体类型
*/
typedef enum
{
    CAT_MEDIA_TYPE_ALL,         //所有媒体类型
    CAT_MEDIA_TYPE_PIC,         //所有图片文件
    CAT_MEDIA_TYPE_AUDIO,       //所有音频文件
    CAT_MEDIA_TYPE_VIDEO,       //所有视频文件
    CAT_MEDIA_TYPE_EBOOK,       //所有电子书文件
    CAT_MEDIA_TYPE_GAME,        //所有游戏文件
    CAT_MEDIA_TYPE_FLASH,       //所有SWF文件
    CAT_MEDIA_TYPE_FIRMWARE,    //所有固件文件
    CAT_MEDIA_TYPE_UNKNOWN,     //所有系统不支持的文件
}cat_media_type_t;


/*!
  * \brief
  *     浏览的结果中每一项的类型（文件或目录）
*/
typedef enum
{
    CAT_SW_FILE = 0x01,     //文件
    CAT_SW_DIR,             //目录
    CAT_SW_PARTI,           //分区
    CAT_SW_DEVICE,          //根
    CAT_SW_EMPTY            //无法识别
}cat_item_type_t;



/*!
  * \brief
  *     排序的方式
*/
typedef enum
{
    CAT_UNSORT,     //无序
    CAT_ASCEND,     //升序
    CAT_DESCEND     //降序
}cat_sort_mode_t;


/*!
  * \brief
  *     排序的类型
*/
typedef enum
{
    CAT_SORT_NAME,      //针对名字进行排序
    CAT_SORT_TIME,      //针对时间进行排序
    CAT_SORT_SIZE,      //针对大小进行排序
    CAT_SORT_FORMAT     //针对格式进行排序
}cat_sort_type_t;


/*!
  * \brief
  *     排序参数
*/
typedef struct
{
    cat_sort_mode_t     mode;       //排序方式
    cat_sort_type_t     type;       //排序的类型
}cat_sort_para_t;


/*!
  * \brief
  *     缩略图输入参数
*/
typedef struct
{
    char file[CAT_MAX_FULL_PATH_LEN];   //文件全路径
    __u8 format;                        //数据格式
    __u32 width;                        //宽度
    __u32 height;                       //高度
    __u32 mode;                         //缩放模式，0非拉伸，1拉伸
}cat_miniature_para_t;


/*!
  * \brief
  *     返回的缩略图数据参数
*/
typedef struct
{
    __u32 len;      //数据长度
    char *buf;      //数据
}cat_miniature_t;


/*!
  * \brief
  *     创建浏览数据时的传入参数
*/
typedef struct
{
    char                path[CAT_MAX_FULL_PATH_LEN];    // 待浏览的路径
    cat_scan_type_t     scan_type;                      //浏览扫描的类型
    cat_media_type_t    media_type;                     //浏览的媒体类型
}cat_create_param_t;


/*!
  * \brief
  *     创建浏览信息后的cat状态
*/
typedef enum
{
    CAT_CREATING = 0x01,    //创建中
    CAT_CREATE_OVER,        //创建完毕
    CAT_CREATE_UNKONWN,
}cat_create_status_t;


/*!
  * \brief
  *     分区对应的设备
*/
typedef enum
{
    CAT_UNKNOWN,        //未知
    CAT_LOCAL,          //本地磁盘
    CAT_TF,             //TF卡
    CAT_USB,            //USB设备
    CAT_CD,             //CD-ROM
    CAT_MEDIA           //媒体库
}cat_root_t;


/*!
  * \brief
  *     分区属性
*/
typedef enum
{
    CAT_READONLY = 0x01,    //只读
    CAT_READWRITE           //可读写
}cat_pati_attr_t;


/*!
  * \brief
  *     浏览项的基本信息
*/
typedef struct
{
    char                        fpath[CAT_MAX_FULL_PATH_LEN];           //该item的路径
    cat_item_type_t             type;                                   //该项的类型
    char                        name[CAT_MAX_FULL_PATH_LEN];            //该项的名字
    __epdk_charset_enm_e        char_encoding;                          //编码格式
    union
    {
        struct
        {
            cat_pati_attr_t     pati_attr;                          //分区属性，只读或者可读写
            cat_root_t          root_type;                          //根类型，本地盘或者TF卡或者其他
        }pati;
        
        struct
        {
            ES_TIMESTAMP        m_time;                             //创建时间
        }dir;
        
        struct
        {
            __s64               size;
            ES_TIMESTAMP        m_time;                             //创建时间
            cat_media_type_t    media_type;                         //媒体类型，音乐、视频、图片等等
            __u8                format;                             //格式，MP3\WAV等
        }file;
    }item_pri_info;    
}cat_item_base_info_t;


/*!
  * \brief
  *     浏览项的额外信息
*/
typedef struct
{
    union
    {
        struct
        {
            __s64   size;
            __s64   space_left;     //分区的剩余空间
        }pati;
        
        struct
        {
            __s64   size;
            __u32   item_cnt;       //包含的相应媒体类型的子项个数
        }dir;
        
        struct
        {
            cat_miniature_t miniature;  //缩略图信息
            union
            {
                struct
                {
                    __u32   bitrate;        //比特率
                    __u32   sample_rate;    //采样率
                    __u32   channel;        //声道数
                    __u32   duration;       //总时间
                    __u32   bits;           //采样精度
                }audio;
                
                struct
                {
                    __u32   width;          //宽
                    __u32   height;         //高
                }pic;
                
                struct
                {
                    __u32               width;      //宽
                    __u32               height;     //高
                    __u32               frame_rate; //帧率
                    __cedar_video_fmt_t video_fmt;  //视频格式
                    __cedar_audio_fmt_t audio_fmt;  //音频格式
                    __u32               duration;   //总时间 
                }video;
                
                struct
                {
                    char    author[64];     //作者
                    char    theme[256];     //主题
                }ebook;
            }file_info;
        }file;
    }extra_info;
}cat_item_extra_info_t;


/*!
  * \brief
  *     创建浏览后，每一条目的信息
*/
typedef struct __cat_item_t
{
    cat_item_base_info_t    base_info;      //该项的基本信息
    cat_item_extra_info_t   extra_info;     //该项的额外信息
}__cat_item_t;


/*!
  * \brief
  *     删除拷贝信息
*/
typedef struct
{
    cat_item_type_t     type;               //条目类型，文件或者目录
    __u64               total_size;         //需要拷贝的总容量;
    __u64               finish_size;        //已拷贝的总容量;
    __u32               total_num;          //目录时，需拷贝/删除的文件总数
    __u32               finish_num;         //目录时，已完成拷贝/删除的文件数    
    __u32               percent;            //已拷贝/删除数据的百分比，公式=已拷贝数据/需拷贝数据总量
}cat_file_op_info_t;


/*!
  * \brief
  *     缩略图需要cache时的参数
*/
typedef struct
{
    __u32 cache_num;    //需要cache的个数
    __u32 buf_size;     //cache时每个buffer的大小
}cat_cache_para_t;


#define MAX_TITLE_SIZE               63
#define MAX_ARTIST_SIZE              63
#define MAX_ALBUM_SIZE               63
#define MAX_GENRE_SIZE               63
#define MAX_YEAR_SIZE                15

typedef struct db_pic_info_t
{
	__u32               length;      // 0 means no picture. The length of the picture info in the file.
	__u32               offset;      // The offset of the picture info in the file.
	__sw_pic_format_e   format;      // jpg, bmp, and so on
}m_pic_info_t;

typedef struct db_audio_info_t
{
	char                    title[MAX_TITLE_SIZE+1];        // empty string : title is unknown.
	__epdk_charset_enm_e    title_charset;
	char                    artist[MAX_ARTIST_SIZE+1];      // empty string : artist is unknown.
	__epdk_charset_enm_e    artist_charset;
	char                    album[MAX_ALBUM_SIZE+1];        // empty string : album is unknown.
	__epdk_charset_enm_e    album_charset;
	char                    genre[MAX_GENRE_SIZE+1];        // empty string : genre is unknown.
	__epdk_charset_enm_e    genre_charset;
	char                    year[MAX_YEAR_SIZE+1];          // empty string : year is unknown.
	__epdk_charset_enm_e    year_charset;
	__u32                   duration;                           // 0 : the duration is unknown.
	__u32                   bps;
	__u32                   sample_rate;
	__sw_audio_format_e     format;
	m_pic_info_t            album_cover_info;
}m_audio_info_t;

#define  MAX_DB_PATH            50

#define  MEDIA_CLASS_ARTIST    (1<<0)
#define  MEDIA_CLASS_ALBUM     (1<<1)
#define  MEDIA_CLASS_GEBRE     (1<<2)
#define  MEDIA_CLASS_STAR      (1<<3)
#define  MEDIA_CLASS_YEAR      (1<<4)
#define  MEDIA_CLASS_ALLMUSIC  (1<<5)
#define  MEDIA_CLASS_PLAYLIST  (1<<6)
#define  MEDIA_CLASS_MYFAVOR   (1<<7)
#define  MEDIA_CLASS_TOP       (1<<8) 

#define  MEDIA_ALL_CLASS       (MEDIA_CLASS_ARTIST | MEDIA_CLASS_ALBUM | MEDIA_CLASS_GEBRE | MEDIA_CLASS_STAR \
							   | MEDIA_CLASS_YEAR | MEDIA_CLASS_ALLMUSIC | MEDIA_CLASS_PLAYLIST | MEDIA_CLASS_MYFAVOR)

typedef enum MEDIA_DB_SORT
{
	SORT_ASCEND,
	SORT_DESCEND,
		
}media_db_sort_e;

typedef struct QUERY_ATTR_PARA_T
{
	__u8             is_filter;   //是否进行关键字过滤
	char             filter_flag; //正过滤还是反过滤标志: 0正 1反
	char             keyword[50]; //过滤的关键字
	
 	media_db_sort_e  sort;        //排序方式
}query_attr_para_t;


typedef struct CAT_API_ENTRY
{		

	HORC (*ptr_cat_playlist_open)														    (void);

	HNPL (*ptr_cat_npl_creat)								  	  (HORC horc, orc_arg_npl_t *arg );

	__s32 (*ptr_cat_npl_delete)											    (HORC horc, HNPL hnpl);

	__s32 (*ptr_cat_npl_add_item)					 	  (HORC horc, HNPL hnpl, const char *file);

	__s32 (*ptr_cat_npl_del_item)				    	  (HORC horc, HNPL hnpl, const char *file);

	__s32 (*ptr_cat_npl_clear)												(HORC horc, HNPL hnpl);

	__s32 (*ptr_cat_npl_get_next)											(HORC horc, HNPL hnpl);

	__s32 (*ptr_cat_npl_file2index)				  	       (HORC horc, HNPL hnpl,const char *file);

	__s32 (*ptr_cat_npl_index2file)		           (HORC horc, HNPL hnpl, __s32 index, void *file);

	__s32 (*ptr_cat_npl_invalidate_item)				     	(HORC horc, HNPL hnpl,__u32 index);

	__s32 (*ptr_cat_npl_item_is_valid)						    (HORC horc, HNPL hnpl,__u32 index);

	__s32 (*ptr_cat_npl_get_total_count) 						     		(HORC horc, HNPL hnpl);

	__s32 (*ptr_cat_npl_get_valid_count)							     	(HORC horc, HNPL hnpl);

	__s32 (*ptr_cat_npl_is_dirty)									   	   (HORC horc ,HNPL hnpl );

	__s32 (*ptr_cat_npl_clear_dirty_flag)							   	   (HORC horc ,HNPL hnpl );

	__s32 (*ptr_cat_npl_set_cur) 								       (HORC horc,  void *pbuffer);

	__s32 (*ptr_cat_npl_get_cur)									       (HORC horc , HNPL hnpl);

	__s32 (*ptr_cat_npl_set_play_mode)					   (HORC horc, HNPL hnpl, __s32 play_mode);

	orc_play_mode_e (*ptr_cat_npl_get_play_mode)						   (HORC horc, HNPL hnpl );

	__s32 (*ptr_cat_playlist_close) 												  (HORC horc );



	//__s32 (*ptr_db_table_all_query)				(HMDB hmdb, const char *table, const char *columns);
	
	__s32 (*ptr_cat_medlib_db_creat)										  (const char *db_path);
	
	__s32 (*ptr_cat_medlib_db_delete)										  (const char *db_path);
	
	HMDB  (*ptr_cat_medlib_db_connect)										  (const char *db_path);
	
	__s32 (*ptr_cat_medlib_db_disconnet)												(HMDB hmdb);
	
	__s32 (*ptr_cat_medlib_db_get_audio_info)			 (const char *file, m_audio_info_t *info_p);
	
	__s32 (*ptr_cat_medlib_db_classify_add_file)		 (HMDB hmdb, const char *filePath, __u32 update_class);
	
	__s32 (*ptr_cat_medlib_db_classify_del_file)		 (HMDB hmdb, const char *filePath, __u32 update_class);
	
	__s32 (*ptr_cat_medlib_db_add_file_to_entry)		 (HMDB hmdb, ENTRY entry, const char *filePath);
	
	__s32 (*ptr_cat_medlib_db_del_file_from_entry)		 (HMDB hmdb, ENTRY entry, const char *filePath);
	
	__s32 (*ptr_cat_medlib_db_set_lrc_path)				 (HMDB hmdb, ENTRY entry, const char *filePath, const char *lrcPath);
	
	ENTRY (*ptr_cat_medlib_db_spl_creat)				 (HMDB hmdb, const char *spl_name);
	
	__s32 (*ptr_cat_medlib_db_spl_add_item)			 (HMDB hmdb, ENTRY entry, const char *filePath);
	
	__s32 (*ptr_cat_medlib_db_spl_del_item)          (HMDB hmdb, ENTRY entry, const char *filePath);
	
	__s32 (*ptr_cat_medlib_db_spl_delete)							       (HMDB hmdb, ENTRY entry);
	
	__s32 (*ptr_cat_medlib_db_save_npl)								(HMDB hmdb, HORC horc, HNPL hnpl, const char *spl_name);
	
	__s32 (*ptr_cat_medlib_db_load_spl_to_npl)	 (HMDB hmdb, ENTRY entry, HORC horc, HNPL hnpl, query_attr_para_t *attr_para);
	
	__u32 (*ptr_cat_medlib_db_is_vaild)										  (const char *db_path);
	
	ENTRY (*ptr_cat_medlib_db_get_class_entry)						(HMDB hmdb,  __u32 browse_class);
	
	__s32 (*ptr_cat_medlib_db_release_entry)                               (ENTRY entry);
	 
	__u32 (*ptr_cat_medlib_db_entry_get_rows)							   (HMDB hmdb, ENTRY entry);
	
	HDBCursor (*ptr_cat_medlib_db_entry_begin_browse)			(HMDB hmdb, ENTRY entry, __u32 step, query_attr_para_t* attr_para);
	
	__s32 (*ptr_cat_medlib_db_entry_move_to_next_item)						(HDBCursor hCursor);
	
	__s32 (*ptr_cat_medlib_db_entry_move_to_pre_item)						(HDBCursor hCursor);
	
	ENTRY (*ptr_cat_medlib_db_entry_item2pentry)						(HDBCursor hCursor);
	
	ENTRY (*ptr_cat_medlib_db_entry_item2nentry)					   	(HDBCursor hCursor);
	
	__s32 (*ptr_cat_medlib_db_entry_item2name)						(HDBCursor hCursor, char *name);
	
	__s32 (*ptr_cat_medlib_db_entry_item2filepath)					(HDBCursor hCursor, char *file_path);
	
	__s32 (*ptr_cat_medlib_db_entry_item2lrcpath)					(HDBCursor hCursor, char *lrc_path);
	
	__s32 (*ptr_cat_medlib_db_entry_end_browse)								(HDBCursor hCursor);
	
	__s32 (*ptr_cat_medlib_db_set_star_to_file)		(HMDB hmdb, const char *filePath, __u32 starID);
	
	__s32 (*ptr_cat_medlib_db_get_file_star)					  (HMDB hmdb, const char *filePath);
	
	__s32 (*ptr_cat_medlib_db_update)				 (HMDB hmdb, const char *dirPath, __u32 update_class, __u32 new_update_class);



	HCAT   (*ptr_cat_core_open)                                                         	(void);
	
	HEXPLR (*ptr_cat_create_explr)              (HCAT cat_handle, cat_create_param_t *create_para);
	
	cat_create_status_t (*ptr_cat_get_create_status)              (HCAT cat_handle, HEXPLR hexplr);
	
	__s32 (*ptr_cat_update_root)        (HCAT cat_handle, HEXPLR hexplr, __s32 plug_in, char disk);
	
	__s32 (*ptr_cat_destroy_explr)     							  (HCAT cat_handle, HEXPLR hexplr);
	
	__s32 (*ptr_cat_get_explr_num) 								  (HCAT cat_handle, HEXPLR hexplr);
	
	__s32 (*ptr_cat_get_explr_item) (HCAT cat_handle, HEXPLR hexplr, __u32 id, __cat_item_t *cat_item);
	
	__s32 (*ptr_cat_sort_explr)         (HCAT cat_handle, HEXPLR hexplr, cat_sort_para_t sort_para);
	
	__s32 (*ptr_cat_get_explr_para) (HCAT cat_handle, HEXPLR hexplr, cat_create_param_t *create_para);
	
	__s32 (*ptr_cat_open_extra_dec) 				        (HCAT cat_handle, cat_media_type_t type);
	
	__s32 (*ptr_cat_close_extra_dec) 											   (HCAT cat_handle);
	
	__s32 (*ptr_cat_cache_create)      (HCAT cat_handle, HEXPLR hexplr, cat_cache_para_t cache_para);
	
	__s32 (*ptr_cat_cache_destroy) 									(HCAT cat_handle, HEXPLR hexplr);
	
	__s32 (*ptr_cat_get_explr_extra) (HCAT cat_handle, HEXPLR hexplr, cat_miniature_para_t *para, __u32 id, __cat_item_t *cat_item);
	
	__s32 (*ptr_cat_del_file) 							(HCAT cat_handle, HEXPLR hexplr, __u32 index);
	
	__s32 (*ptr_cat_del_dir) 							(HCAT cat_handle, HEXPLR hexplr, __u32 index);
	
	__s32 (*ptr_cat_cancel_del) 									 (HCAT cat_handle, HEXPLR hexplr);
	
	__s32 (*ptr_cat_copy_file) 							(HCAT cat_handle, HEXPLR hexplr, __u32 index);
	
	__s32 (*ptr_cat_paste_file) 			   (HCAT cat_handle, HEXPLR hexplr, char *src, char *dst);
	
	__s32 (*ptr_cat_copy_dir) 						    (HCAT cat_handle, HEXPLR hexplr, __u32 index);
	
	__s32 (*ptr_cat_paste_dir)  			   (HCAT cat_handle, HEXPLR hexplr, char *src, char *dst);
	
	__s32 (*ptr_cat_get_file_operation_info) 			  (HCAT cat_handle, cat_file_op_info_t *info);
	
	__s32 (*ptr_cat_cancel_paste)									 (HCAT cat_handle, HEXPLR hexplr);
	
	__s32 (*ptr_cat_set_file_for_play) (HCAT cat_handle, HEXPLR hexplr, cat_media_type_t *media_type, __u32 cur_id);
	
	__s32 (*ptr_cat_get_cur_playfile) (HCAT cat_handle, HEXPLR hexplr, cat_media_type_t media_type, __u32 *cur_id);
	
	HEXPLR (*ptr_cat_search) 						  (HCAT cat_handle, HEXPLR hexplr, char* keyword);
	
	__s32 (*ptr_cat_core_close) 													(HCAT cat_handle);
	
	
} cat_api_entry_t;


#ifndef USED_BY_CAT     //not used by desktop module itsef

#define CAT_ENTRY_TBL   (0xe1e00000)   //this address is depended on the link address of mod_desktop
#define CAT_FUNC_ENTRY  ((cat_api_entry_t *)CAT_ENTRY_TBL)


#define 				cat_playlist_open		    (CAT_FUNC_ENTRY->ptr_cat_playlist_open)

#define 				cat_npl_creat				(CAT_FUNC_ENTRY->ptr_cat_npl_creat)

#define  				cat_npl_delete				(CAT_FUNC_ENTRY->ptr_cat_npl_delete)

#define	 				cat_npl_add_item			(CAT_FUNC_ENTRY->ptr_cat_npl_add_item)

#define	 				cat_npl_del_item			(CAT_FUNC_ENTRY->ptr_cat_npl_del_item)

#define	 				cat_npl_clear				(CAT_FUNC_ENTRY->ptr_cat_npl_clear)

#define	 				cat_npl_get_next			(CAT_FUNC_ENTRY->ptr_cat_npl_get_next)

#define 				cat_npl_file2index			(CAT_FUNC_ENTRY->ptr_cat_npl_file2index)

#define	 				cat_npl_index2file			(CAT_FUNC_ENTRY->ptr_cat_npl_index2file)

#define 				cat_npl_invalidate_item		(CAT_FUNC_ENTRY->ptr_cat_npl_invalidate_item)

#define 				cat_npl_item_is_valid		(CAT_FUNC_ENTRY->ptr_cat_npl_item_is_valid)

#define 				cat_npl_get_total_count		(CAT_FUNC_ENTRY->ptr_cat_npl_get_total_count)

#define 				cat_npl_get_valid_count		(CAT_FUNC_ENTRY->ptr_cat_npl_get_valid_count)

#define 				cat_npl_is_dirty			(CAT_FUNC_ENTRY->ptr_cat_npl_is_dirty)

#define 				cat_npl_clear_dirty_flag	(CAT_FUNC_ENTRY->ptr_cat_npl_clear_dirty_flag)

#define 				cat_npl_set_cur				(CAT_FUNC_ENTRY->ptr_cat_npl_set_cur)

#define 				cat_npl_get_cur				(CAT_FUNC_ENTRY->ptr_cat_npl_get_cur)

#define 				cat_npl_set_play_mode		(CAT_FUNC_ENTRY->ptr_cat_npl_set_play_mode)

#define 				cat_npl_get_play_mode		(CAT_FUNC_ENTRY->ptr_cat_npl_get_play_mode)

#define  				cat_playlist_close          (CAT_FUNC_ENTRY->ptr_cat_playlist_close)




//#define	                db_table_all_query						(CAT_FUNC_ENTRY->ptr_db_table_all_query)

#define					cat_medlib_db_creat 					(CAT_FUNC_ENTRY->ptr_cat_medlib_db_creat)

#define					cat_medlib_db_delete					(CAT_FUNC_ENTRY->ptr_cat_medlib_db_delete)

#define					cat_medlib_db_connect					(CAT_FUNC_ENTRY->ptr_cat_medlib_db_connect)

#define					cat_medlib_db_disconnet				(CAT_FUNC_ENTRY->ptr_cat_medlib_db_disconnet)

#define					cat_medlib_db_get_audio_info			(CAT_FUNC_ENTRY->ptr_cat_medlib_db_get_audio_info)
	
#define					cat_medlib_db_classify_add_file		(CAT_FUNC_ENTRY->ptr_cat_medlib_db_classify_add_file)

#define					cat_medlib_db_classify_del_file		(CAT_FUNC_ENTRY->ptr_cat_medlib_db_classify_del_file)

#define					cat_medlib_db_add_file_to_entry		(CAT_FUNC_ENTRY->ptr_cat_medlib_db_add_file_to_entry)

#define					cat_medlib_db_del_file_from_entry		(CAT_FUNC_ENTRY->ptr_cat_medlib_db_del_file_from_entry)

#define					cat_medlib_db_set_lrc_path			(CAT_FUNC_ENTRY->ptr_cat_medlib_db_set_lrc_path)

#define					cat_medlib_db_spl_creat				(CAT_FUNC_ENTRY->ptr_cat_medlib_db_spl_creat)

#define					cat_medlib_db_spl_add_item				(CAT_FUNC_ENTRY->ptr_cat_medlib_db_spl_add_item)

#define					cat_medlib_db_spl_del_item				(CAT_FUNC_ENTRY->ptr_cat_medlib_db_spl_del_item)

#define					cat_medlib_db_spl_delete				(CAT_FUNC_ENTRY->ptr_cat_medlib_db_spl_delete)

#define					cat_medlib_db_save_npl					(CAT_FUNC_ENTRY->ptr_cat_medlib_db_save_npl)

#define					cat_medlib_db_load_spl_to_npl			(CAT_FUNC_ENTRY->ptr_cat_medlib_db_load_spl_to_npl)

#define					cat_medlib_db_is_vaild					(CAT_FUNC_ENTRY->ptr_cat_medlib_db_is_vaild)

#define					cat_medlib_db_get_class_entry			(CAT_FUNC_ENTRY->ptr_cat_medlib_db_get_class_entry)

#define					cat_medlib_db_release_entry			    (CAT_FUNC_ENTRY->ptr_cat_medlib_db_release_entry)

#define					ptr_cat_medlib_db_entry_get_rows	    (CAT_FUNC_ENTRY->ptr_cat_medlib_db_entry_get_rows)

#define					cat_medlib_db_entry_begin_browse		(CAT_FUNC_ENTRY->ptr_cat_medlib_db_entry_begin_browse)

#define					cat_medlib_db_entry_move_to_next_item	(CAT_FUNC_ENTRY->ptr_cat_medlib_db_entry_move_to_next_item)
	
#define					cat_medlib_db_entry_move_to_pre_item	(CAT_FUNC_ENTRY->ptr_cat_medlib_db_entry_move_to_pre_item)
	
#define					cat_medlib_db_entry_item2pentry		(CAT_FUNC_ENTRY->ptr_cat_medlib_db_entry_item2pentry)
	
#define					cat_medlib_db_entry_item2nentry		(CAT_FUNC_ENTRY->ptr_cat_medlib_db_entry_item2nentry)
	
#define					cat_medlib_db_entry_item2name			(CAT_FUNC_ENTRY->ptr_cat_medlib_db_entry_item2name)
			
#define					cat_medlib_db_entry_item2filepath		(CAT_FUNC_ENTRY->ptr_cat_medlib_db_entry_item2filepath)
	
#define					cat_medlib_db_entry_item2lrcpath		(CAT_FUNC_ENTRY->ptr_cat_medlib_db_entry_item2lrcpath)
	
#define					cat_medlib_db_entry_end_browse		(CAT_FUNC_ENTRY->ptr_cat_medlib_db_entry_end_browse)
	
#define					cat_medlib_db_set_star_to_file			(CAT_FUNC_ENTRY->ptr_cat_medlib_db_set_star_to_file)
	
#define					cat_medlib_db_get_file_star			(CAT_FUNC_ENTRY->ptr_cat_medlib_db_get_file_star)
	
#define					cat_medlib_db_update				(CAT_FUNC_ENTRY->ptr_cat_medlib_db_update)
															


#define				    cat_core_open               (CAT_FUNC_ENTRY->ptr_cat_core_open)
				
#define					cat_create_explr            (CAT_FUNC_ENTRY->ptr_cat_create_explr)
				
#define					cat_get_create_status       (CAT_FUNC_ENTRY->ptr_cat_get_create_status)
				
#define					cat_update_root             (CAT_FUNC_ENTRY->ptr_cat_update_root)
				
#define					cat_destroy_explr           (CAT_FUNC_ENTRY->ptr_cat_destroy_explr)
				
#define					cat_get_explr_num           (CAT_FUNC_ENTRY->ptr_cat_get_explr_num)
				
#define					cat_get_explr_item          (CAT_FUNC_ENTRY->ptr_cat_get_explr_item)
				
#define					cat_sort_explr              (CAT_FUNC_ENTRY->ptr_cat_sort_explr)
				
#define					cat_get_explr_para          (CAT_FUNC_ENTRY->ptr_cat_get_explr_para)
				
#define					cat_open_extra_dec          (CAT_FUNC_ENTRY->ptr_cat_open_extra_dec)
				
#define					cat_close_extra_dec         (CAT_FUNC_ENTRY->ptr_cat_close_extra_dec)
				
#define					cat_cache_create            (CAT_FUNC_ENTRY->ptr_cat_cache_create)
				
#define					cat_cache_destroy           (CAT_FUNC_ENTRY->ptr_cat_cache_destroy)
				
#define					cat_get_explr_extra         (CAT_FUNC_ENTRY->ptr_cat_get_explr_extra)
				
#define					cat_del_file                (CAT_FUNC_ENTRY->ptr_cat_del_file)
				
#define					cat_del_dir                 (CAT_FUNC_ENTRY->ptr_cat_del_dir)
				
#define					cat_cancel_del              (CAT_FUNC_ENTRY->ptr_cat_cancel_del)
				
#define					cat_copy_file               (CAT_FUNC_ENTRY->ptr_cat_copy_file)
				
#define					cat_paste_file              (CAT_FUNC_ENTRY->ptr_cat_paste_file)
				
#define					cat_copy_dir                (CAT_FUNC_ENTRY->ptr_cat_copy_dir)
				
#define					cat_paste_dir               (CAT_FUNC_ENTRY->ptr_cat_paste_dir)
				
#define					cat_get_file_operation_info (CAT_FUNC_ENTRY->ptr_cat_get_file_operation_info)
				
#define					cat_cancel_paste            (CAT_FUNC_ENTRY->ptr_cat_cancel_paste)
				
#define					cat_set_file_for_play       (CAT_FUNC_ENTRY->ptr_cat_set_file_for_play)
				
#define					cat_get_cur_playfile        (CAT_FUNC_ENTRY->ptr_cat_get_cur_playfile)
				
#define					cat_search                  (CAT_FUNC_ENTRY->ptr_cat_search)
				
#define					cat_core_close              (CAT_FUNC_ENTRY->ptr_cat_core_close)
				

                          
#endif  //USED_BY_CAT

#endif  //__MOD_CAT_H__



