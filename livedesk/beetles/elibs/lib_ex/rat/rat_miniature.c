#include "epdk.h"
#include "apps.h"

#include "rat_common.h"
#include "rat_miniature.h"

typedef struct
{
    __u8    willow_libs;
    __mp    *willow_handle;
}rat_willow_t;

static rat_willow_t* rat_willow_hdl = NULL;
/*
**********************************************************************************************************************
*                                               _convert_2_argb
*
* author:          terry.zeng 
*
* date:             2009-11-15
*
* Description:       ������bgra��ʽת��Ϊargb 
*
* parameters:       data������  len�����ݳ���
*
* return:           void
* modify history: 
**********************************************************************************************************************
*/
static void _convert_2_argb(__u32 *data, __u32 len)
{
    __u32 tmp = 0;
    __u32 cnt = 0;
    
    for(cnt = 0; cnt < len; cnt++)
    {
        tmp = *(data + cnt);
        *(data + cnt) = ( ((tmp | 0xff) << 24)
                        | ((tmp & 0xff00) << 8)
                        | ((tmp & 0xff0000) >> 8)
                        | ((tmp & 0xff000000) >> 24) );
    }
}


/*
**********************************************************************************************************************
*                                               wait_willow_ready
*
* author:          terry.zeng 
*
* date:             2009-11-15
*
* Description:       ȷ��rat_willow_hdl�м�����ڷ�WILLOW_STATUS_BUSY״̬
*
* parameters:      
*
* return:           void
* modify history: 
**********************************************************************************************************************
*/
void wait_willow_ready(__mp *willow_handle)
{
    __u8 status = 0;
    
    if(willow_handle == NULL)
    {
        return;
    }

    /*check rat_willow_hdl status*/
    while(1)
    {
        status = esMODS_MIoctrl(willow_handle, WILLOW_CMD_QUERY_STATUS, 0, 0);
        if (status != WILLOW_STATUS_BUSY)
        {
            break;
        }
        esKRNL_TimeDly(1);
    }
}

/*
**********************************************************************************************************************
*                                               wait_willow_finish
*
* author:          terry.zeng 
*
* date:             2009-11-15
*
* Description:       ȷ��rat_willow_hdl�м������WILLOW_STATUS_FINISH״̬
*
* parameters:      
*
* return:           void
* modify history: 
**********************************************************************************************************************
*/
void wait_willow_finish(__mp *willow_handle)
{
    __u8 status = 0;
    
    if(willow_handle == NULL)
    {
        return;
    }

    /*check rat_willow_hdl status*/
    while(1)
    {   
        status = esMODS_MIoctrl(willow_handle, WILLOW_CMD_QUERY_STATUS, 0, 0);
        if (status == WILLOW_STATUS_FINISH)
        {
            break;
        }
        esKRNL_TimeDly(1);        
    }
}


/*
**********************************************************************************************************************
*                                               AWILLOW_Open
*
* author:           terry.zeng
*
* date:             2010-10-11
*
* Description:      ��ͼƬ��������
*
* parameters:		
*
* return:           if success return rat_willow_hdl handle
*                   if fail return NULL
* modify history:
**********************************************************************************************************************
*/
rat_willow_t *AWILLOW_Open(void)
{
    rat_willow_t *rat_willow_hdl = NULL;

    __here__;
    rat_willow_hdl = (rat_willow_t *)esMEMS_Malloc(0, sizeof(rat_willow_t));
    __here__;
    if(rat_willow_hdl == NULL)
    {
        __msg("esMEMS_Malloc fail...\n");
        return NULL;
    }
    else
    {
        eLIBs_memset(rat_willow_hdl, 0, sizeof(rat_willow_t));
    }
    
    rat_willow_hdl->willow_libs = esMODS_MInstall(BEETLES_APP_ROOT"mod\\willow\\willow.mod", 0);     
    if(rat_willow_hdl->willow_libs == NULL)
    {
        __msg("install rat_willow_hdl fail!\n");
        esMEMS_Mfree(0, rat_willow_hdl);
        rat_willow_hdl = NULL;
        return NULL;
    }

    __here__;
    rat_willow_hdl->willow_handle = esMODS_MOpen(rat_willow_hdl->willow_libs, 0);    
    if(rat_willow_hdl->willow_handle == NULL)
    {
        __msg("open rat_willow_hdl fail!\n");
        esMODS_MUninstall(rat_willow_hdl->willow_libs);
        rat_willow_hdl->willow_libs = NULL;
        esMEMS_Mfree(0, rat_willow_hdl);
        rat_willow_hdl = NULL;
        return NULL;
    }
    __msg("AWILLOW_Open success...\n");
    
    return rat_willow_hdl;
}


/*
**********************************************************************************************************************
*                                               AWILLOW_Close
*
* author:          terry.zeng 
*
* date:             2010-10-11
*
* Description:       �ر�rat_willow_hdl�м�� 
*
* parameters:      
*
* return:           success: EPDK_OK;   failed: EPDK_FAIL
* modify history: 
**********************************************************************************************************************
*/
__s32 AWILLOW_Close(rat_willow_t *handle)
{
    rat_willow_t *rat_willow_hdl = NULL;
    
    if(handle == NULL)
    {
        return EPDK_FAIL;
    }
    else
    {
        rat_willow_hdl = (rat_willow_t *)handle;
    }
    
    wait_willow_ready(rat_willow_hdl->willow_handle);
        
    if(rat_willow_hdl->willow_handle != NULL)
    {        
        esMODS_MClose(rat_willow_hdl->willow_handle);
        rat_willow_hdl->willow_handle = NULL;
    }
        
    if(rat_willow_hdl->willow_libs != NULL)
    {
        esMODS_MUninstall(rat_willow_hdl->willow_libs);
        rat_willow_hdl->willow_libs = NULL;
    }
    
    esMEMS_Mfree(0, rat_willow_hdl);
    rat_willow_hdl = NULL;
    
    return EPDK_OK;
}


/*
**********************************************************************************************************************
*                                               AWILLOW_GetThumbs
*
* author:          terry.zeng 
*
* date:             2009-11-15
*
* Description:       ���м����ȡ����ͼ
*
* parameters:      
*
* return:           success: EPDK_OK;   failed: EPDK_FAIL
* modify history: 
**********************************************************************************************************************
*/
__s32 AWILLOW_GetThumbs(__mp *willow_handle, __willow_get_thumbs_param_t *param, __u32 mode, __willow_img_info_t *img_info)
{
    __s32 result = -1;
        
    if((willow_handle == NULL) || (param == NULL) || (img_info == NULL))
    {
        return EPDK_FAIL;
    }
    
    wait_willow_finish(willow_handle);

    result = esMODS_MIoctrl(willow_handle, WILLOW_CMD_GET_THUMBS, 0, param);
    if(result == -1)
    {
        return EPDK_FAIL;
    }
        
    wait_willow_finish(willow_handle);
    
    if(param->filename)
    {
        if(esMODS_MIoctrl(willow_handle, WILLOW_CMD_CHECK_IMG, 0, param) == EPDK_FAIL)
        {
            return EPDK_FAIL;
        }
        
        wait_willow_ready(willow_handle);
        result = esMODS_MIoctrl(willow_handle, WILLOW_CMD_GET_IMG_INFO, 0, img_info);
        if(result == -1)
        {
            return EPDK_FAIL;
        }
        
        result = esMODS_MIoctrl(willow_handle, WILLOW_CMD_START_DEC, 0, 0);
        if(result == -1)
        {
            return EPDK_FAIL;
        }
        
        result = esMODS_MIoctrl(willow_handle, WILLOW_CMD_START_SHOW_EXT, 0, (void *)mode);
        if(result == -1)
        {
            return EPDK_FAIL;
        }
        wait_willow_finish(willow_handle);
        
        return EPDK_OK;
    }
    else
    {    
        return EPDK_FAIL;
    }
}


/*
**********************************************************************************************************************
*                                               AWILLOW_GetAlbumPic
*
* author:          terry.zeng 
*
* date:             2009-11-15
*
* Description:       ���м����ȡ����ͼ
*
* parameters:      
*
* return:           success: EPDK_OK;   failed: EPDK_FAIL
* modify history: 
**********************************************************************************************************************
*/
__s32 AWILLOW_GetAlbumPic(__mp *willow_handle, __willow_get_albumart_param_t *para, __u32 mode)
{
    __s32                       result = -1;
    __willow_show_file_param_t  check_para;
    
    if((willow_handle == NULL) || (para == NULL))
    {
        return EPDK_FAIL;
    }
    
    eLIBs_memset(&check_para, 0, sizeof(__willow_show_file_param_t));
    
    wait_willow_finish(willow_handle);
    
    result = esMODS_MIoctrl(willow_handle, WILLOW_CMD_SET_ALBUM_ART, 0, para);
    if(result == -1)
    {
        return EPDK_FAIL;
    }
    
    wait_willow_finish(willow_handle);
    
    if(para->thumbs.filename)
    {
        check_para.filename = para->thumbs.filename;
        check_para.img_no = 0;
        result = esMODS_MIoctrl(willow_handle, WILLOW_CMD_CHECK_IMG, 0, &check_para);
        if(result == -1)
        {
            return EPDK_FAIL;
        }
        
        wait_willow_ready(willow_handle);
        result = esMODS_MIoctrl(willow_handle, WILLOW_CMD_START_DEC, 0, 0);
        if(result == -1)
        {
            return EPDK_FAIL;
        }
        
        result = esMODS_MIoctrl(willow_handle, WILLOW_CMD_START_SHOW_EXT, 0, (void *)mode);
        if(result == -1)
        {
            return EPDK_FAIL;
        }
        
        wait_willow_finish(willow_handle);
    
        return EPDK_OK;
    }
    else
    {    
        return EPDK_FAIL;
    }
}

__s32 rat_wait_willow_finish(void)
{
    if(rat_willow_hdl == NULL)
    {
        return EPDK_FAIL;
    }
    
    wait_willow_finish(rat_willow_hdl->willow_handle);
    
    return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                          
*
* author:           
*
* date:             
*
* Description:       ��������ͼ���루��װrat_willow_hdl.mod������ÿ�ζ���װ�����ó�פ�ڴ棩
*
* parameters:      
*
* return:           success 0;	failed:-1
* modify history: 
**********************************************************************************************************************
*/
__s32 rat_start_miniature_decode(void)
{
    //eLIBs_printf("test: do not rat_start_miniature_decode\n");
    
   // return EPDK_FAIL;
    
	if (NULL == rat_willow_hdl)
	{
		rat_willow_hdl = AWILLOW_Open();
	}
	
	if (NULL == rat_willow_hdl)
	{
        __msg("rat_start_miniature_decode fail...\n");
		return EPDK_FAIL;
	}

    __msg("rat_start_miniature_decode success...\n");

	return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                          
*
* author:           
*
* date:             
*
* Description:       ֹͣ����ͼ���루ж��rat_willow_hdl.mod��
*
* parameters:      
*
* return:           success 0;	failed:-1
* modify history: 
**********************************************************************************************************************
*/
__s32 rat_stop_miniature_decode(void)
{
	if (rat_willow_hdl)
	{
		AWILLOW_Close(rat_willow_hdl);
		rat_willow_hdl = NULL;
	}	

	return EPDK_OK;
}

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
__s32 rat_get_pic_info(rat_miniature_para_t *para,rat_pic_info_t *pic_info)
{
    __willow_get_thumbs_param_t     thumbs;
    __willow_img_info_t             img_info;    
    __s32                           result = 0;
    
    if((rat_willow_hdl == NULL) || (para == NULL) || (pic_info == NULL))
    {
        return EPDK_FAIL;
    }
    
    eLIBs_memset(&thumbs, 0, sizeof(__willow_get_thumbs_param_t)); 
    eLIBs_memset(&img_info, 0, sizeof(__willow_img_info_t)); 
    
    thumbs.filename = para->file;
    thumbs.format = para->format;
    thumbs.size.width = para->width;
    thumbs.size.height = para->height;
    thumbs.buf = (__u8 *)pic_info->miniature.buf;
    //__msg("file=%s, format=%d, width=%d, height=%d, buf=%x\n",thumbs.filename, thumbs.format, thumbs.size.width, thumbs.size.height, thumbs.buf);
    result = AWILLOW_GetThumbs(rat_willow_hdl->willow_handle, &thumbs, para->mode, &img_info);
    if(result != EPDK_OK)
    {
        return EPDK_FAIL;
    }
    
    pic_info->miniature.len = para->width * para->height * 4;
    _convert_2_argb((__u32 *)pic_info->miniature.buf, para->width * para->height);
    //__msg("width=%d, height=%d\n",img_info.size.width, img_info.size.height);
    pic_info->width = img_info.size.width;
    pic_info->height = img_info.size.height;
    pic_info->format = (rat_pic_format_t)img_info.format;
    
    return EPDK_OK;
}

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
__s32 rat_get_album_info(rat_miniature_para_t *para,rat_audio_info_t *audio_info)
{
	__willow_get_albumart_param_t   album_para;
    __audio_file_info_t             file_info;
    __willow_img_info_t             img_info;    
    char                            info_buf[1024];    
    __s32                           result = 0;
    
    if((rat_willow_hdl == NULL) || (para == NULL) || (audio_info == NULL))
    {
        return EPDK_FAIL;
    }
    
    if((para->file == NULL) || (audio_info->album.buf == NULL))
    {
        return EPDK_FAIL;
    }
    
    eLIBs_memset(&album_para, 0, sizeof(__willow_get_albumart_param_t));
    eLIBs_memset(&file_info, 0, sizeof(__audio_file_info_t));
    eLIBs_memset(&img_info, 0, sizeof(__willow_img_info_t));
    eLIBs_memset(info_buf, 0, 1024);
    
    file_info.InforBufLeftLength = 1024;
    file_info.InforBuf = info_buf;
    if(!eLIBs_GetAudioInfo(para->file, &file_info)) // fail in getting information
    {
        __wrn("Fail in extracting audio info of file %s.\n", para->file);
        return EPDK_FAIL;
    }
    
    audio_info->bitrate = file_info.ulBitRate;
    audio_info->sample_rate = file_info.ulSampleRate;
    audio_info->channel = file_info.ulChannels;
    audio_info->duration = file_info.ulDuration;
    audio_info->bits = file_info.ulBitsSample;
    if(file_info.ulAPic_sz == 0)
    {
        __wrn("no album info in  %s. \n", para->file);
        return EPDK_FAIL;
    }
    
    album_para.album_art_info.enable = 1;
    if(file_info.ulAPic->img_format == ID3_IMG_FORMAT_BMP)
    {
        album_para.album_art_info.img_type = IMG_FORMAT_BMP;
    }
    else if(file_info.ulAPic->img_format == ID3_IMG_FORMAT_GIF)
    {
        album_para.album_art_info.img_type = IMG_FORMAT_GIF;
    }
    else if(file_info.ulAPic->img_format == ID3_IMG_FORMAT_PNG)
    {
        album_para.album_art_info.img_type = IMG_FORMAT_PNG;
    }
    else if(file_info.ulAPic->img_format == ID3_IMG_FORMAT_JPG)
    {
        album_para.album_art_info.img_type = IMG_FORMAT_JPG;
    }
    else if(file_info.ulAPic->img_format == ID3_IMG_FORMAT_UNSUPPORT)
    {
        __wrn("image format not support\n");
        return EPDK_FAIL;
    }
    else
    {
        return EPDK_FAIL;
    }
    
    album_para.album_art_info.offset = file_info.ulAPic->FileLocation;
    album_para.album_art_info.length = file_info.ulAPic->length;
    album_para.thumbs.format = para->format;
    album_para.thumbs.filename = para->file;
    album_para.thumbs.size.width = para->width;
    album_para.thumbs.size.height = para->height;
    album_para.thumbs.buf = (__u8 *)audio_info->album.buf;
    result = AWILLOW_GetAlbumPic(rat_willow_hdl->willow_handle, &album_para, para->mode);
    if(result != EPDK_OK)
    {
        return EPDK_FAIL;
    }
    audio_info->album.len = para->width * para->height * 4;
    _convert_2_argb((__u32 *)audio_info->album.buf, para->width * para->height);
    
    return EPDK_OK;
}
