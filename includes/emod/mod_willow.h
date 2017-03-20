/*
**********************************************************************************************************************
*                                                   eMOD
*                                  the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                               (c) Copyright 2006-2007, RICHARD,  China
*                                           All Rights Reserved
*
* File   : mod_willow.h
* Version: V1.0
* By     : Richard.x
**********************************************************************************************************************
*/

#ifndef _MOD_WILLOW_H_
#define _MOD_WILLOW_H_

#include "typedef.h"
#include "cfgs.h"

/*����Ϊ��ȡ����ͼʱ�����ģʽ*/
#define WILLOW_SCALE_RATIO 0         /* ��������С��ָ���ߴ磬�������
                                      * ͼƬ�ߴ�С��ָ���ߴ�ʱ������ͼƬ�ߴ粻�� */
#define WILLOW_SCALE_STRETCH 1       /* �Դ��ڵı�������ͼƬ����������ʾ�����ܻ���� */
#define WILLOW_SCALE_RATIO_SCALE 2   /* ��������С��ָ���ߴ磬�������
                                      * ͼƬ�ߴ�С��ָ���ߴ�ʱ���Ŵ�ͼƬ����Ӧ�ߴ� */
#define WILLOW_SCALE_RATIO_CUTOFF 3  /* ��ͼƬ�ı�������ͼƬ����������ʾ��
                                      * ���ܻ��в���ͼƬ�������(���ɼ�)*/
#define WILLOW_SCALE_CLEAR_BK 0x100  /*����ͼδ�����ʱ�����ʣ�ಿ��Ϊ��ɫ*/
#define WILLOW_SCALE_CLEAR_NONE 0x0  /*����ͼδ�����ʱ������䱳��ɫ����Ӧ���Լ����*/

#define MAX_TRANSITION_FB_NUM 16

/*ͼƬ������Ŵ�ͼƬʱ����ز���*/
#define WILLOW_SCALER_BASE_RATIO 1000 /*�Ŵ�������*/
#define WILLOW_SCALER_MAX_RATIO 1000000 /*�Ŵ�������*/


/*ͼƬ��ʽ*/
typedef enum
{
    IMG_FORMAT_BMP,                    /*bmpͼƬ*/
    IMG_FORMAT_JPG,                    /*JPEGͼƬ*/
    IMG_FORMAT_GIF,                    /*GIF��ʽͼƬ*/
    IMG_FORMAT_PNG,                    /*PNG��ʽͼƬ*/
    IMG_FORMAT_3D,                     /*JPEGͼƬ*/
    IMG_FORMAT_UNSUPPORT               /*ͼƬ��ʽ��֧��*/
}__willow_img_format_t;

/*willow status*/
typedef enum
{
    WILLOW_STATUS_INIT,
    WILLOW_STATUS_BUSY,
    WILLOW_STATUS_READY,
    WILLOW_STATUS_FINISH
}__willow_img_status_t;


typedef enum
{
    WILLOW_IMAGE_SHOW_ORISIZE,         /* ��ͼƬԭʼ��С�ڴ�������ʾ�������������
                                        * ͼƬ�ߴ�С�ڴ���ʱ������ͼƬ�ߴ粻�� */
    WILLOW_IMAGE_SHOW_STRETCH,         /* �Դ��ڵı�������ͼƬ����������ʾ�����ܻ���� */
    WILLOW_IMAGE_SHOW_CUTOFF,          /* ��ͼƬ�ı�������ͼƬ����������ʾ��
                                        * ���ܻ��в���ͼƬ�������(���ɼ�)*/
    WILLOW_IMAGE_SHOW_AUTO,
    WILLOW_IMAGE_SHOW_RATIO,           /* ��ͼƬ�ı�������ͼƬ����������ʾ�������������
                                        * ͼƬ�ߴ�С�ڴ���ʱ����Ŵ������ʳߴ� */
    WILLOW_IMAGE_SHOW_USRDEF,          /* �û��Զ��壬ͼƬ��ʾ�ĳߴ���callback �趨*/

}__willow_image_show_mode_t;


/*pshow support switch method*/
typedef enum 
{ 
    DEFAULT_SWITCH  = 0x0,    /*Ĭ�����л�Ч��*/
    FADE_IN_OUT     = 0x1,    /*���뵭��*/
    PERSIAN_BLIND_H = 0x2,    /*ˮƽ��Ҷ��*/
    PERSIAN_BLIND_V = 0x3,    /*��ֱ��Ҷ��*/
    SLID_UP         = 0x4,    /*���ϻ���*/
    SLID_DOWN       = 0x5,    /*���»���*/
    SLID_LEFT       = 0x6,    /*���󻬶�*/
    SLID_RIGHT      = 0x7,    /*���һ���*/
    STRETCH_UP      = 0x8,    /*����չ��*/
    STRETCH_DOWN    = 0x9,    /*����չ��*/
    STRETCH_LEFT    = 0xA,    /*����չ��*/
    STRETCH_RIGHT   = 0xB,    /*����չ��*/
    MOSAIC          = 0xC,    /*������*/
    ROOM_IN         = 0xD,    /*��С*/
    ROOM_OUT        = 0xE,    /*�Ŵ�*/
    
    TOUCH_SLID_H    = 0x50,    /*touchˮƽ���򻬶�*/
    TOUCH_SLID_V    = 0x51,   /*touch��ֱ���򻬶�*/
    EXTENT_SWITCH1  = 0x100,
    EXTENT_SWITCH2,
    EXTENT_SWITCH3,
    EXTENT_SWITCH4,
    EXTENT_SWITCH5
}__willow_switch_mode_t;

/*pshow support splended effect*/
enum 
{ 
    EXTENT_EFFECT1,
    EXTENT_EFFECT2,
    EXTENT_EFFECT3,
    EXTENT_EFFECT4,
    EXTENT_EFFECT5
};

typedef enum
{
    WILLOW_LYR_TOP,               /*��willow�е�ͼ������Ϊtop*/
    WILLOW_LYR_BOTTOM,            /*��willow�е�ͼ������Ϊbottom*/
    WILLOW_LYR_PIPE_0,            /*��willow�е�ͼ������Ϊpipe0*/
    WILLOW_LYR_PIPE_1,            /*��willow�е�ͼ������Ϊpipe1*/
    WILLOW_LYR_HIDE,              /*��willow�е�ͼ������Ϊ���ɼ�*/
    WILLOW_LYR_SHOW,              /*��willow�е�ͼ������Ϊ�ɼ�*/
}__willow_lyr_cfg_t;


typedef enum
{
    PSHOW_ROTATE_NONE = 0,          /*����ת*/
    PSHOW_ROTATE_RIGHT = 1,         /*������ת90��*/
    PSHOW_ROTATE_180 = 2,           /*��ת180��*/
    PSHOW_ROTATE_LEFT = 3,          /*������ת90��*/
    PSHOW_ROTATE_UNSUPPORTED = 4,   /*======������ʱ��֧��======*/
    PSHOW_FLIP_HORIZONTAL = 4,      /*ˮƽ��ת*/
    PSHOW_FLIP_VERTICAL = 5,        /*��ֱ��ת*/
}__willow_rotate_mode_t;


/*willow support command*/
typedef enum                                        
{
    WILLOW_CMD_QUERY_STATUS          = 0x0,  /* ��ѯ״̬��
                                              * ����ֵΪ__willow_img_status_t
                                              */

    WILLOW_CMD_GET_THUMBS            = 0x1,  /* ���û�ȡ����ͼ����Ҫ����Ϣ
                                              * pbuffer = (__willow_get_thumbs_param_t *)
                                              */ 
                                              
    WILLOW_CMD_SHOW_IMG_FROM_FILE    = 0x2,  /* ������ʾͼƬ���ļ���Ϣ
                                              * pbuffer = (__willow_show_file_param_t *)
                                              */
                                              
    WILLOW_CMD_SHOW_IMG_FROM_BUFFER  = 0x3,  /* ������ʾ��argb����
                                              * pbuffer = (__willow_show_buffer_param_t *)
                                              * ע�⣬������Ҫ��buffer�е�����Ϊargb8888, 
                                              * �����ǽ���buffer�е�ͼƬ��
                                              * ����buffer�е�ͼƬʹ������WILLOW_CMD_CFG_INPUT
                                              */
    
    WILLOW_CMD_SET_SWITCH_MODE       = 0x4,  /* ����ͼƬ���ʱ����л�ģʽ
                                              * aux = (__willow_switch_mode_t)
                                              */
    
    
    WILLOW_CMD_GET_SHOW_PARAM        = 0x5,  /* ��ȡ��ǰ��ʾ����
                                              * pbuffer = (__willow_showing_img_info_t *)
                                              */
                                              
    WILLOW_CMD_GET_IMG_INFO          = 0x6,  /* ��ȡͼƬ��Ϣ
                                              * pbuffer = (__willow_img_info_t *) 
                                              */
    /*change current img show*/                    
    WILLOW_CMD_SCALE                 = 0x7,  /* ��С/�Ŵ�ͼƬ
                                              * aux = -3��-2��-1��0��1��2��3
                                              * �ֱ������С8/4/2����ԭʼ��С���Ŵ�2/4/8��
                                              */
                                              
    WILLOW_CMD_ROTATE                = 0x8,  /* ��תͼƬ
                                              * aux = -270, -180, -90, 0, 90, 180, 270
                                              * ������ʾ��ʱ����ת,������ʾ˳ʱ����ת
                                              */
                                              
    WILLOW_CMD_MOVE                  = 0x9,  /* �ƶ�ͼƬ����ͼƬ�Ŵ����Ч
                                              * pbuffer = (__pos_t *) Ϊ�ƶ���offset
                                              */
                                              
    WILLOW_CMD_COME_BACK             = 0xA,  /* ������С/�Ŵ�ͼƬ�󷵻�ԭʼ��С*/
    
    WILLOW_CMD_OPEN_SHOW             = 0xB,  /* ��ͼƬ���*/
    WILLOW_CMD_SHUT_SHOW             = 0xC,  /* �ر�ͼƬ���*/
        
    WILLOW_CMD_CHECK_IMG             = 0xD,  /* ���ͼƬ�Ƿ�֧��*/
    WILLOW_CMD_START_DEC             = 0xE,  /* ��ʼ����*/  
    WILLOW_CMD_START_SHOW            = 0xF,  /* ��ʾͼƬ����ȡ����ͼ*/
    WILLOW_CMD_STOP                  = 0x10, //
    
    WILLOW_CMD_SET_ALBUM_ART         = 0x11, /* ����ר��ͼƬ��Ϣ, 
                                              * pbuffer = (__willow_get_albumart_param_t *)
                                              */
    WILLOW_CMD_SET_SCN               = 0x12, /* ����slider show����ʾ����
                                              * pbuffer = __rect_t 
                                              */
    WILLOW_CMD_GET_VER               = 0x13, /* ��ȡwillow�汾�� 
                                              * (__u32 *)pbuffer = WILLOW_VERSION
                                              */
    WILLOW_CMD_START_SHOW_EXT        = 0x14, /* ��ȡ����ͼ,Ԥ���û�ָ����䷽ʽ,
                                              * pbuffer = (__u32 *)(fill_mode | clear_flag)
                                              * fill_mode����Ϊ: WILLOW_SCALE_RATIO,
                                              *                  WILLOW_SCALE_STRETCH
                                              *                  WILLOW_SCALE_RATIO_SCALE
                                              *                  WILLOW_SCALE_RATIO_CUTOFF
                                              * clear_flag����Ϊ WILLOW_SCALE_CLEAR_BK
                                              *                  WILLOW_SCALE_CLEAR_NONE
                                              * ���庬����ο��궨���е�ע�͡�
                                              */
    
    WILLOW_CMD_CATCH_SCREEN          = 0x15, /* ��ȡ��ǰ��ʾ��fb
                                              * pbuffer = (FB *)
                                              */
    
    WILLOW_CMD_AUTO_ROTATE           = 0x16, /* ���߶� > ���ʱ���Ƿ���Ҫ�Զ���ת��
                                              * aux = __willow_rotate_mode_t
                                              */
                                                                            
    WILLOW_CMD_GET_ERRNO             = 0x17, /* ��ȡ����ţ�Ŀǰδ�ṩ*/
    
    WILLOW_CMD_SET_SHOW_MODE         = 0x18, /* ������ʾģʽ
                                              * aux = __willow_image_show_mode_t��
                                              * ��aux = WILLOW_IMAGE_SHOW_USRDEF�ǣ�pbuffer = __pCBK_t��
                                              * ����Ϊ __willow_showmod_usr_para_t
                                              */
    
    WILLOW_CMD_CFG_LYR               = 0x19, /* ����willowͼ������, 
                                              * aux = __willow_lyr_cfg_t
                                              */
    
    WILLOW_CMD_ROT_LYR               = 0x20, /* ��ͼƬ��תһ���ǶȺ���ʾ, ע��ͼƬ������ʾ����ת
                                              * ����ΪWILLOW_CMD_ROT_LYR�� WILLOW_CMD_AUTO_ROTATE��
                                              * �߹�ͬ���õĽ����
                                              * aux = __willow_rotate_mode_t
                                              */
                                              
    WILLOW_CMD_CFG_OUTPUT            = 0x21, /* ���������ָ����fb�У�
                                              * pbuffer = (__willow_output_t *)
                                              */
                                              
    WILLOW_CMD_CFG_INPUT             = 0x22, /* ����ָ�������뷽ʽ��
                                              * aux = __willow_input_src_e
                                              * ��aux = WILLOW_INPUT_BUFFERʱ,pbuffer = (__willow_buf_info_t *)
                                              */
                                              
    WILLOW_CMD_SET_PLAYLIST          = 0x23, /* ����ͼƬԤ������Ϣ
                                              * pbuffer = (__willow_img_plst_info_t *)
                                              */
    
    WILLOW_CMD_MOV_TRACK             = 0x24, /* ������ק�˶��켣
                                              * pbuffer = (slider_step_para_t *)
                                              */
                                              
    WILLOW_CMD_GET_CUR_INDEX         = 0x25, /* ��ȡ��ק������ͼƬ�ڲ����б��е�index
                                              * ����index
                                              */
                                              
    WILLOW_CMD_SCALE_EXT             = 0x26, /* �޼�����ͼƬ,��������ʵ��С��WILLOW_SCALER_MAX_RATIO��������ͼƬ
                                              * aux = �Ŵ��� * WILLOW_SCALER_BASE_RATIO
                                              * aux = 2300��ʾ�Ŵ� 2.3��
                                              */
    WILLOW_CMD_CHECK_CUR             = 0x27,     /* ��ȡ��ק�������ȡ��ǰͼƬ�Ƿ�֧�� */

    WILLOW_CMD_SET_3D_MOD            = 0x100, /* ����3D ��ʾģʽ, Ŀǰ��֧��
                                               * aux = __willow_show_3d_mode_e
                                               */
}__willow_main_cmd_t;    

typedef enum
{
    WILLOW_INPUT_FILENAME = 0x0,            /*����Ϊ�����ļ����ļ���*/
    WILLOW_INPUT_BUFFER   = 0x1,            /*����Ϊbuffer�������ļ�Ԥ�ȶ��뵽�ڴ�*/
    WILLOW_INPUT_RAWDATA  = 0x2,            /*����Ϊ����������*/
    WILLOW_INPUT_ERR      = 0x3,
}__willow_input_src_e;


typedef struct __WILLOW_BUF_INFO
{
    void *buffer;
    __u32 buffersize;
}__willow_buf_info_t;

typedef struct
{
    __u8    format;
    __u8    seq;

    __u32    nimg;
    __u32     height;
    __u32     width;
    void     *data;
}__willow_thumbs_t;

typedef struct
{
    __s32    is_supported;          /*�Ƿ�֧�ָ�ͼƬ(ͼƬ�ߴ�̫��ʱ��֧�֣�flag = -1)*/
    __u32    reserve[10];           /*�����������Ժ���չʹ��*/
    FB       img_fb;                /*ͼ����������*/
}__willow_output_t;


/*�����ļ�ר��ͼƬ��Ϣ�����ڽ���ͼƬ�����ڱ����ļ�����ƫ�Ƶ����*/
typedef struct
{
    __u8 enable;                          /*���ݽṹ�Ƿ���Ч*/
    __willow_img_format_t img_type;       /*ͼƬ����*/
    __u32 offset;                         /*ͼƬ�������ļ��е�ƫ��*/
    __u32 length;                         /*ͼƬ���ݵĳ���*/
}__willow_AlbumArt_ctrl_t;



/*willow thumbs command param*/
typedef struct
{
    char             *filename;                  //img file name
    __u8            *buf;                        //thumbs data dest addr
    
    __u8            format;                      //thumbs format
    __rectsz_t         size;
    
}__willow_get_thumbs_param_t;


/*willow album art command param*/
typedef struct
{
    __willow_get_thumbs_param_t thumbs;          
    __willow_AlbumArt_ctrl_t album_art_info;
    
}__willow_get_albumart_param_t;

/*willow show buffer comamand param*/
typedef struct
{
    __rect_t    img_size;
    __u8        *img_buf;                          //img addr
    __u32         buf_size;                        //img size    
    __u8        img_format;                        //img format
    
}__willow_show_buffer_param_t;



/*willow show img file command param*/
typedef struct
{
    char        *filename;                                    //file name
    __u32        img_no;
}__willow_show_file_param_t;

typedef struct
{
    RECT        show_region;
    __hdle        layer_hdle;
    ES_FILE        *de;
}__willow_showing_img_info_t;

typedef struct
{
    char    *name;
    __u32    file_len;
    
    SIZE    size;
    __u8    format;
    __u8    color_depth;
    __u32    aperture;
    __u32    shutter;    
}__willow_img_info_t;

typedef struct
{
    __pCBK_t  get_imagename_by_index;
    __s32     total;
    FB        *decoding_fb[MAX_TRANSITION_FB_NUM];
    FB        *unsupported_fb;
    __s32     cycEnabled;
}__willow_img_plst_info_t;


typedef enum
{
    SLIDER_STEP_START,            /*��ʼ��ק*/
    SLIDER_STEP_MOVE,             /*�ƶ���Ŀ��λ��*/
    SLIDER_STEP_FINISH,           /*������ק*/
}slider_step_cmd_e;


typedef struct 
{
    slider_step_cmd_e    cmd;
    OFFSET               offset;
}slider_step_para_t;

typedef enum
{
    WILLOW_SHOW_2D     = 0x0,              /*2Dģʽ��ʾ3dͼƬ*/
    WILLOW_SHOW_3D     = 0x1,              /*3Dģʽ��ʾ3dͼƬ*/
}__willow_show_3d_mode_e;


typedef struct
{
    __rectsz_t  imgSize;        /* ԭʼͼ��Ĵ�С��callback������ */
    __rectsz_t  dstSize;        /* ���Ŵ����ȫͼ�Ĵ�С */
    __rect_t    srcRect;        /* Ҫ��ʾ�Ĳ�������ǰ����ԭʼͼ���е�����/λ�� */
    __rect_t    dstRect;        /* Ҫ��ʾ�Ĳ������ź���dstSizeͼ���е�����/λ�� */
}__willow_showmod_usr_para_t;
#endif  /* _MOD_WILLOW_H_ */

