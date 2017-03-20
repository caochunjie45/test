#ifndef __RAT_MINIATURE_H__
#define __RAT_MINIATURE_H__

/*!
* \brief
*     ͼƬ�ļ���ʽ
*/
typedef enum
{
    RAT_PIC_FORMAT_BMP = 0x40,
	RAT_PIC_FORMAT_GIF,
	RAT_PIC_FORMAT_JPG,
	RAT_PIC_FORMAT_PNG,	
	RAT_PIC_FORMAT_UNKNOWN
}rat_pic_format_t;

/*!
  * \brief
  *     ����ͼ�������
*/
typedef struct
{
    char file[RAT_MAX_FULL_PATH_LEN];   //�ļ�ȫ·��
    __u8 format;                        //���ݸ�ʽ
    __u32 width;                        //���
    __u32 height;                       //�߶�
    __u32 mode;                         //����ģʽ��0�����죬1����
}rat_miniature_para_t;

/*!
* \brief
*     ���ص�����ͼ���ݲ���
*/
typedef struct
{
    __u32 len;      //���ݳ���
    char *buf;      //����
}rat_miniature_t;

/*!
* \brief
*     ͼƬ�ļ���ʽ
*/


typedef struct
{
    rat_pic_format_t    format;
    __u32               width;
    __u32               height;
    rat_miniature_t     miniature;
}rat_pic_info_t;

typedef struct
{
    rat_pic_format_t    format;
    __u32               bitrate;
    __u32               sample_rate;
    __u32               channel;
    __u32               duration;
    __u32               bits;
    rat_miniature_t     album;
}rat_audio_info_t;


/*
**********************************************************************************************************************
*                                          
*
* author:           
*
* date:             
*
* Description:       �ȴ�willow�߳̽���
*
* parameters:      
*
* return:           success 0;	failed:-1
* modify history: 
**********************************************************************************************************************
*/
__s32 rat_wait_willow_finish(void);

/*
**********************************************************************************************************************
*                                          
*
* author:           
*
* date:             
*
* Description:       ��������ͼ���루��װwillow.mod������ÿ�ζ���װ�����ó�פ�ڴ棩
*
* parameters:      
*
* return:           success 0;	failed:-1
* modify history: 
**********************************************************************************************************************
*/
__s32 rat_start_miniature_decode(void);

/*
**********************************************************************************************************************
*                                          
*
* author:           
*
* date:             
*
* Description:       ֹͣ����ͼ���루ж��willow.mod��
*
* parameters:      
*
* return:           success 0;	failed:-1
* modify history: 
**********************************************************************************************************************
*/
__s32 rat_stop_miniature_decode(void);

/*
**********************************************************************************************************************
*                                          
*
* author:           
*
* date:             
*
* Description:       ��ȡͼƬ����ͼ����Ϣ
*
* parameters:      
*
* return:           success 0;	failed:-1
* modify history: 
**********************************************************************************************************************
*/
__s32 rat_get_pic_info(rat_miniature_para_t *para,rat_pic_info_t *pic_info);

/*
**********************************************************************************************************************
*                                          
*
* author:           
*
* date:             
*
* Description:       ��ȡ����������Ϣ
*
* parameters:      
*
* return:           success 0;	failed:-1
* modify history: 
**********************************************************************************************************************
*/
__s32 rat_get_album_info(rat_miniature_para_t *para,rat_audio_info_t *audio_info);

#endif//__RAT_MINIATURE_H__
