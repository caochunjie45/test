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
  *     AUDIO�ļ���ʽ
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
  *     VIDEO�ļ���ʽ
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
  *     ͼƬ�ļ���ʽ
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
  *     EBOOK�ļ���ʽ
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
  *     GAME�ļ���ʽ
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
  *     ���ɨ�������
*/
typedef enum
{
    CAT_SCAN_ALL_DEVICE,    //��ɨ�裬ɨ������CAT_USB_DISK��CAT_TF_CARD��CAT_MEDIALIB��CAT_LOCAL_DISK,CAT_CDROM
    CAT_SCAN_ALL_PATI,      //��ɨ�裬���Է���Ϊ����������CAT_MEDIALIB��e,f...�ȵ�
    CAT_SCAN_PARTI,         //����ɨ�裬ֻɨ��ĳ���豸�ķ���
    CAT_SCAN_DIR,           //��ͨĿ¼ɨ�裬ֻɨ�赱ǰ·�������ݹ�
    CAT_SCAN_MEIDA_ALL,     //Ŀ¼ȫɨ�裨ָ��·���������ļ�ɨ�裬������Ŀ¼�µ��ļ���
}cat_scan_type_t;


/*!
  * \brief
  *     �����ý������
*/
typedef enum
{
    CAT_MEDIA_TYPE_ALL,         //����ý������
    CAT_MEDIA_TYPE_PIC,         //����ͼƬ�ļ�
    CAT_MEDIA_TYPE_AUDIO,       //������Ƶ�ļ�
    CAT_MEDIA_TYPE_VIDEO,       //������Ƶ�ļ�
    CAT_MEDIA_TYPE_EBOOK,       //���е������ļ�
    CAT_MEDIA_TYPE_GAME,        //������Ϸ�ļ�
    CAT_MEDIA_TYPE_FLASH,       //����SWF�ļ�
    CAT_MEDIA_TYPE_FIRMWARE,    //���й̼��ļ�
    CAT_MEDIA_TYPE_UNKNOWN,     //����ϵͳ��֧�ֵ��ļ�
}cat_media_type_t;


/*!
  * \brief
  *     ����Ľ����ÿһ������ͣ��ļ���Ŀ¼��
*/
typedef enum
{
    CAT_SW_FILE = 0x01,     //�ļ�
    CAT_SW_DIR,             //Ŀ¼
    CAT_SW_PARTI,           //����
    CAT_SW_DEVICE,          //��
    CAT_SW_EMPTY            //�޷�ʶ��
}cat_item_type_t;



/*!
  * \brief
  *     ����ķ�ʽ
*/
typedef enum
{
    CAT_UNSORT,     //����
    CAT_ASCEND,     //����
    CAT_DESCEND     //����
}cat_sort_mode_t;


/*!
  * \brief
  *     ���������
*/
typedef enum
{
    CAT_SORT_NAME,      //������ֽ�������
    CAT_SORT_TIME,      //���ʱ���������
    CAT_SORT_SIZE,      //��Դ�С��������
    CAT_SORT_FORMAT     //��Ը�ʽ��������
}cat_sort_type_t;


/*!
  * \brief
  *     �������
*/
typedef struct
{
    cat_sort_mode_t     mode;       //����ʽ
    cat_sort_type_t     type;       //���������
}cat_sort_para_t;


/*!
  * \brief
  *     ����ͼ�������
*/
typedef struct
{
    char file[CAT_MAX_FULL_PATH_LEN];   //�ļ�ȫ·��
    __u8 format;                        //���ݸ�ʽ
    __u32 width;                        //���
    __u32 height;                       //�߶�
    __u32 mode;                         //����ģʽ��0�����죬1����
}cat_miniature_para_t;


/*!
  * \brief
  *     ���ص�����ͼ���ݲ���
*/
typedef struct
{
    __u32 len;      //���ݳ���
    char *buf;      //����
}cat_miniature_t;


/*!
  * \brief
  *     �����������ʱ�Ĵ������
*/
typedef struct
{
    char                path[CAT_MAX_FULL_PATH_LEN];    // �������·��
    cat_scan_type_t     scan_type;                      //���ɨ�������
    cat_media_type_t    media_type;                     //�����ý������
}cat_create_param_t;


/*!
  * \brief
  *     ���������Ϣ���cat״̬
*/
typedef enum
{
    CAT_CREATING = 0x01,    //������
    CAT_CREATE_OVER,        //�������
    CAT_CREATE_UNKONWN,
}cat_create_status_t;


/*!
  * \brief
  *     ������Ӧ���豸
*/
typedef enum
{
    CAT_UNKNOWN,        //δ֪
    CAT_LOCAL,          //���ش���
    CAT_TF,             //TF��
    CAT_USB,            //USB�豸
    CAT_CD,             //CD-ROM
    CAT_MEDIA           //ý���
}cat_root_t;


/*!
  * \brief
  *     ��������
*/
typedef enum
{
    CAT_READONLY = 0x01,    //ֻ��
    CAT_READWRITE           //�ɶ�д
}cat_pati_attr_t;


/*!
  * \brief
  *     �����Ļ�����Ϣ
*/
typedef struct
{
    char                        fpath[CAT_MAX_FULL_PATH_LEN];           //��item��·��
    cat_item_type_t             type;                                   //���������
    char                        name[CAT_MAX_FULL_PATH_LEN];            //���������
    __epdk_charset_enm_e        char_encoding;                          //�����ʽ
    union
    {
        struct
        {
            cat_pati_attr_t     pati_attr;                          //�������ԣ�ֻ�����߿ɶ�д
            cat_root_t          root_type;                          //�����ͣ������̻���TF����������
        }pati;
        
        struct
        {
            ES_TIMESTAMP        m_time;                             //����ʱ��
        }dir;
        
        struct
        {
            __s64               size;
            ES_TIMESTAMP        m_time;                             //����ʱ��
            cat_media_type_t    media_type;                         //ý�����ͣ����֡���Ƶ��ͼƬ�ȵ�
            __u8                format;                             //��ʽ��MP3\WAV��
        }file;
    }item_pri_info;    
}cat_item_base_info_t;


/*!
  * \brief
  *     �����Ķ�����Ϣ
*/
typedef struct
{
    union
    {
        struct
        {
            __s64   size;
            __s64   space_left;     //������ʣ��ռ�
        }pati;
        
        struct
        {
            __s64   size;
            __u32   item_cnt;       //��������Ӧý�����͵��������
        }dir;
        
        struct
        {
            cat_miniature_t miniature;  //����ͼ��Ϣ
            union
            {
                struct
                {
                    __u32   bitrate;        //������
                    __u32   sample_rate;    //������
                    __u32   channel;        //������
                    __u32   duration;       //��ʱ��
                    __u32   bits;           //��������
                }audio;
                
                struct
                {
                    __u32   width;          //��
                    __u32   height;         //��
                }pic;
                
                struct
                {
                    __u32               width;      //��
                    __u32               height;     //��
                    __u32               frame_rate; //֡��
                    __cedar_video_fmt_t video_fmt;  //��Ƶ��ʽ
                    __cedar_audio_fmt_t audio_fmt;  //��Ƶ��ʽ
                    __u32               duration;   //��ʱ�� 
                }video;
                
                struct
                {
                    char    author[64];     //����
                    char    theme[256];     //����
                }ebook;
            }file_info;
        }file;
    }extra_info;
}cat_item_extra_info_t;


/*!
  * \brief
  *     ���������ÿһ��Ŀ����Ϣ
*/
typedef struct __cat_item_t
{
    cat_item_base_info_t    base_info;      //����Ļ�����Ϣ
    cat_item_extra_info_t   extra_info;     //����Ķ�����Ϣ
}__cat_item_t;


/*!
  * \brief
  *     ɾ��������Ϣ
*/
typedef struct
{
    cat_item_type_t     type;               //��Ŀ���ͣ��ļ�����Ŀ¼
    __u64               total_size;         //��Ҫ������������;
    __u64               finish_size;        //�ѿ�����������;
    __u32               total_num;          //Ŀ¼ʱ���追��/ɾ�����ļ�����
    __u32               finish_num;         //Ŀ¼ʱ������ɿ���/ɾ�����ļ���    
    __u32               percent;            //�ѿ���/ɾ�����ݵİٷֱȣ���ʽ=�ѿ�������/�追����������
}cat_file_op_info_t;


/*!
  * \brief
  *     ����ͼ��Ҫcacheʱ�Ĳ���
*/
typedef struct
{
    __u32 cache_num;    //��Ҫcache�ĸ���
    __u32 buf_size;     //cacheʱÿ��buffer�Ĵ�С
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
	__u8             is_filter;   //�Ƿ���йؼ��ֹ���
	char             filter_flag; //�����˻��Ƿ����˱�־: 0�� 1��
	char             keyword[50]; //���˵Ĺؼ���
	
 	media_db_sort_e  sort;        //����ʽ
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



