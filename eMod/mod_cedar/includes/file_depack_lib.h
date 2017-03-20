/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_herb sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2009-2010, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : avi_depack_lib.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2010-8-12
* Description:
    [cedarlib]
********************************************************************************
*/
#ifndef _FILE_DEPACK_LIB_H_
#define _FILE_DEPACK_LIB_H_

//����ֵ����
#ifndef EPDK_OK
#define EPDK_OK (0) /* ����ɹ�                                                     */
#endif
#ifndef EPDK_FAIL
#define EPDK_FAIL (-1)  /* ����ʧ��                                                     */
#endif
#ifndef EPDK_TRUE
#define EPDK_TRUE (1)   /* ��                                                           */
#endif
#ifndef EPDK_FALSE
#define EPDK_FALSE (0)  /* ��                                                           */
#endif

//define when should file parser delay some time when ff/rr
#define FFRR_DELAY_PIC_FRM_CNT      (10)
#define FFRR_DELAY_AUD_FRM_CNT      (50)
/*******************************************************************************
    ��������: ��Ƶ�ļ���������ʽ����Ƶ�ļ���������ʽ��ȫ��ͬ����˷�Ϊ���ֱַ�
    ����
*******************************************************************************/
enum JUMP_PLAY_TYPE{
    NOT_SUPPORT_JUMP_PLAY   = -1,
    JUMP_PLAY_TYPE_VIDEO    = 0,
    JUMP_PLAY_TYPE_AUDIO    = 1,
};

//�����ļ�������ķ���ֵ
typedef enum __FILE_DEPACK_RESULT
{
    FILE_DEPACK_ERR_NONE       = 0,        //no error

    FILE_DEPACK_ERR_READ_END        = 1,        //read file or idx table end.�Զ�����������Ϊ��ʶ,Ҳ���Ա�ʾһ��chunk�����ݶ������ˡ�
    FILE_DEPACK_ERR_BUF_LACK        = 2,        //BUFFER is not enough to contain data.
    FILE_DEPACK_ERR_GET_NONE        = 3,        //read file success for (>0) bytes, but get nothing.
    FILE_DEPACK_ERR_ERROR_IGNORE    = 4,        //some exception happens, but can continue process.
    FILE_DEPACK_ERR_PAUSE_GET_DATA  = 5,        //pause get data from file, when in FF,RR mode,�Ѿ������ˡ�
    FILE_DEPACK_ERR_VIDEO_BUF_FULL = 6,     //��ʾPSR_***��Ҫ�����video bufferȥװ�����ݣ�Ŀǰ��PSR_TSʹ��
    FILE_DEPACK_ERR_AUDIO_BUF_FULL = 7,     //��ʾPSR_***��Ҫ�����audio bufferȥװ�����ݣ�Ŀǰ��PSR_TSʹ��

    FILE_DEPACK_ERR_PARA_ERR        = -1,       //some parameter error
    FILE_DEPACK_ERR_OPEN_FILE_FAIL  = -2,       //open media file failed
    FILE_DEPACK_ERR_READ_FILE_FAIL  = -3,       //read media file data failed
    FILE_DEPACK_ERR_FILE_FMT_ERR    = -4,       //media file format error
    FILE_DEPACK_ERR_NO_AV           = -5,       //neither audio nor video supported in media file
    FILE_DEPACK_ERR_END_OF_MOVI     = -6,       //movi data end
    FILE_DEPACK_ERR_REQMEM_FAIL     = -7,       //request memory failed
    FILE_DEPACK_ERR_EXCEPTION       = -8,       //some impossible case happens.
    FILE_DEPACK_ERR_

} __file_depack_result_t;

//����FileDepackIoctrl()�����,�������depack lib�����ڴ˻�������չ
typedef enum __FILE_DEPACK_COMMAND
{
    FILE_DEPACK_CMD_CHKSPT_JUMPTYPE = 0,     /* ����Ƿ��ܹ�֧�������Լ���������,ret= enum JUMP_PLAY_TYPE,    */
    FILE_DEPACK_CMD_GET_VIDEO_FORMAT,        /* ret = VIDEO_XVID��   */
    FILE_DEPACK_CMD_DISCARD_DATA,            /* ��ֹ����a/v���ݣ�aux = BITSTREAM_TYPE_AUDIO��, buffer = 1:��ֹ��buffer = 0: ���� */
    FILE_DEPACK_CMD_AUD_RESYNC,              /* �´��õ�audio ����ʱ��Ҫset avsyncģ�� */
    FILE_DEPACK_CMD_GET_CUR_ABSSUFFIX,       /* �õ���ǰʹ�õ���Ƶ��������±�ţ����û�У�Ҳ����0 */
    FILE_DEPACK_CMD_SET_ABSSUFFIX,           /* �����µ�������±��, �ڲ����Ժ���������ţ���GetNextChunkInfo()��һ�α������Ժ� */
    FILE_DEPACK_CMD_CHECK_NEW_ABS_UPDATED,   /* ����»��������Ƿ񱻽�����*/
    FILE_DEPACK_CMD_GET_FILE_SIZE,          /* �õ��ļ���С��pbuffer = *s64, ret = FILE_PARSER_RETURN_OK. ������ļ�ָ����psr���������ļ������磬����ODF�����ļ���depack_lib���ص������ڲ���װ���ļ���С������ODF�ļ�����Ĵ�С*/
    FILE_DEPACK_CMD_NOTIFY_ABORT,           /*  ֪ͨpsrģ��׼��, ret = EPDK_OK/EPDK_FAIL*/
    FILE_DEPACK_CMD_SET_CHUNK_BUF_SIZE,     /* ���ô洢���ݵ�chunk�Ĵ�С, aux = size*/
    FILE_DEPACK_CMD_SET_INITIAL_ABSSUFFIX,     /* ��FILE_DEPACK_CMD_SET_ABSSUFFIX��֮ͬ������,��GetNextChunkInfo()����֮ǰ����������ţ���ǰĬ����0 */
    

    //FILE_DEPACK_CMD_PROCESS_STUFDATA = 0x10,        /* ����, ����stuf data���������ʱ�䣬����audioʱ���� */
    FILE_DEPACK_CMD_,
} __file_depack_command_t;

typedef enum {
    CEDARLIB_AVS_VIDEO = 0, //AVS_VIDEO
    CEDARLIB_AVS_AUDIO = 1, //AVS_AUDIO
    CEDARLIB_AVS_AUDVID = 2,
}__cedarlib_clock_type_t;
typedef enum
{                                   // define parameter type for get and set time
    CEDARLIB_AVS_TIME_TOTAL=0,           // total time, base + offset
    CEDARLIB_AVS_TIME_BASE,              // base time, video time start value
    CEDARLIB_AVS_TIME_OFFSET,            // time offset, play time offset
    CEDARLIB_AVS_TIME_
} __cedarlib_time_type_t;   //__avs_time_type_t

typedef enum
{                                   
    CEDARLIB_AVS_ENABLE=1,          //AVS_ENABLE
    CEDARLIB_AVS_DISABLE = 0,       //AVS_DISABLE
} __cedarlib_avs_enable_t;

#define CEDARLIB_TAG_INF_SIZE (256)
typedef struct __CEDARLIB_TAG_INF
{
    unsigned char    tag_magic[4];            /* ý���ļ���ǩ�ı�־��'A'/'V','T','A','G' */

    int   file_offset;                /* ý���ļ��Ķϵ����ļ��ڵ�ƫ��         */
    int   time_offset;                /* ý���ļ��Ķϵ��ʱ��ƫ��             */
    unsigned char    decinf[CEDARLIB_TAG_INF_SIZE]; /* ���������ֳ��ָ���Ϣ                 */
}__cedarlib_tag_inf_t; //���ڽӿ�GetMediaTag()�ȵ�ptag.��ͬ��__cedar_tag_inf_t
typedef struct __JUMP_CONFIG
{
    int       ms;             //the jumped time point in ms
    short       flag;           //
    short       dir;            //search direction, >0 search forward, <=0 search backward
    short       mode;           //0--goto next key frame, 1--goto last key frame��
                                //�����last������ڲ��ҷ�����Եģ��ڲ��ҷ�����
                                //�ķ������һ��key frame,�����search forward��
                                //���ǻ��˵�һ��key frame�������search backward��������ǰ����һ��key frame

} __jump_cfg_t;
/*******************************************************************************
Function name: __audio_stream_info_t
Description:
    �����ݽṹ�������ں���__modify_aud_strhf()�����õ��ĺ���.
    ����ֻ�õ������ʡ��Ժ�����������������������
Members: 
    �����mp3 cbr:
    nBlockAlign: == strh->dwSampleSize

    �����mp3 vbr:
    nBlockAlign: ����Ƶ��������mp3_frame���ֽ���(��λ:�ֽ�).һ��avi chunk����
                �������mp3_frame.
*******************************************************************************/
typedef struct
{
    //define audio codec type
    //__u32       codec_type;         // audio codec type
    //__u32       codec_type_sub;     // audio codec sub type, the highest bit mark endian type
                                    // bit0-bit15, sub type of the audio codec
                                    // bit16,      endian flag: 0, little ending; 1, big ending;
                                    // other bits, reserved
    //define audio bitstream format
    //__u16       channels;           // channel count
    //__u16       bits_per_sample;    // bits per sample
    //__u32       sample_per_frame;       //ÿ��audio frame���е�sample��,������cbr����vbr��ÿ��audio frame���е�sample�����ǹ̶���
    //__u32       sample_per_second;  // sample count per second�����ǲ�����
    unsigned int       avg_bit_rate;       // average bit rate��������
} __audio_stream_info_t;

typedef struct _FILE_DEPACK_ FILE_DEPACK;
typedef struct _FILE_DEPACK_ //
{
    //��������
    //void   (*deleteFILE_DEPACK)(FILE_DEPACK *pFileDepack);
    //������19����Ա����
    int   (*OpenMediaFile)(FILE_DEPACK *pFileDepack, int nFmtType, char *pFileName);    //ret = __file_depack_result_t
    int   (*CloseMediaFile)(FILE_DEPACK *pFileDepack);
    int   (*GetAbsFormat)(FILE_DEPACK *pFileDepack);
    int   (*GetVbsFormat)(FILE_DEPACK *pFileDepack);
    int   (*GetLbsFormat)(FILE_DEPACK *pFileDepack);
    int   (*GetAbsInfForADec)(FILE_DEPACK *pFileDepack, void *pAbsInf, unsigned int *pAbsInfLen);//�÷�ͬGetVbsInfForVDec
    int   (*GetVbsInfForVDec)(FILE_DEPACK *pFileDepack, void *pVbsInf, unsigned int *pVbsInfLen);//����pVbsInf = __vdec_mpeg4_vbs_inf_t**, ָ���ָ��, pVbsInfLen��ʵ�ʳ��ȣ�һ��û���õ�
    int   (*GetLbsInfForLDec)(FILE_DEPACK *pFileDepack, void *pLbsInf, unsigned int *pLbsInfLen);//pLbsInf = __psr_2_ldec_msg_t*,
    int   (*SetProcMode)(FILE_DEPACK *pFileDepack, unsigned int  nMode, unsigned int uParam);
    int   (*GetNextChunkInfo)(FILE_DEPACK *pFileDepack, unsigned int *pChunkType);
    int   (*GetChunkData)(FILE_DEPACK *pFileDepack, unsigned char *pBuf, unsigned int nBufSize, unsigned int *pDataSize,unsigned char *pFrmInf, unsigned int *pFrmInfType);//��Ҫ����һЩ���ݽṹ��__psr_2_vdec_msg_t�����Կ����ǽ������Լ���������ݽṹ
    int   (*SkipChunkData)(FILE_DEPACK *pFileDepack);
    int   (*CheckIfSupportFFRR)(FILE_DEPACK *pFileDepack);
    int   (*SetFFRRSpeed)(FILE_DEPACK *pFileDepack, unsigned int nSpeed);
    int   (*SetFFRRPicShowTime)(FILE_DEPACK *pFileDepack, unsigned int nTime);
    int   (*GetMediaTag)(FILE_DEPACK *pFileDepack, void *ptag);
    int   (*SetMediaTag)(FILE_DEPACK *pFileDepack, void *ptag);
    int   (*GetMediaInfo)(FILE_DEPACK *pFileDepack, __media_file_info_internal_t *pMediaInfo);
    int   (*FileDepackIoctrl)(FILE_DEPACK *pFileDepack,int cmd,int aux, void *pbuffer);  // __file_depack_command_t

    // �ص�����,nMode = CEDARLIB_AVS_VIDEO��, nTimeType = CEDARLIB_AVS_TIME_TOTAL��, 
    // psr_lib�������������ֵ�video pts��audio pts. SetAudVidTime(pts(ms),CEDARLIB_AVS_VIDEO, CEDARLIB_AVS_TIME_BASE)
//    int   (*CbSetAudVidTime)(int nTime,int nMode, int nTimeType);   
//    int   (*CbGetAudVidTime)(int nMode, int nTimeType);
//    int   (*CbSetVideoClkSpeed)(unsigned int nSpeed);    //nSpeed�ĺ���ο�SetVideoClkSpeed(1);
//    int   (*CbSwitchAvSync)(int mode);    //����avsyncģ���do_sync_flagλ����ʾ�Ƿ�Ҫsunc; mode:AVS_DISABLE��, ����ԭ��SwitchAvSync()
//    int   (*CbGetAudioDataInfo)(int codec_type, unsigned char *pbuf, unsigned int length, __audio_stream_info_t *paudstrminfo);
//    int   (*CbBuildAbsPacketHdr)(__audio_codec_format_t *pAbsInf, int uPacketLen, unsigned char *pBuf, int *pHdrLen);
//    int   (*CbUpdateAbsPacketHdr)(__audio_codec_format_t *pAbsInf, int uPacketLen, unsigned char *pBuf, int uHdrLen);
} FILE_DEPACK;

extern FILE_DEPACK* newFILE_DEPACKChild(void);  //����ʵ��, __cedarlib_file_fmt_t, CEDARLIB_FILE_FMT_AVI, (__cedar_media_file_fmt_t, CEDAR_MEDIA_FILE_FMT_AVI)��
extern void deleteFILE_DEPACKChild(FILE_DEPACK* thiz);

//��Ҫ�ⲿʵ�ֵĺ���
extern int SysCb_SetAudVidTime(int nTime, int nMode, int nTimeType);
extern int SysCb_GetAudVidTime(int nMode, int nTimeType);
extern int SysCb_SetVideoClkSpeed(unsigned int nSpeed);  //nSpeed�ĺ���ο�SetVideoClkSpeed(1);
extern int SysCb_SwitchAvSync(int mode);        //mode: AVS_DISABLE, //����avsyncģ���do_sync_flagλ����ʾ�Ƿ�Ҫsunc; mode:AVS_DISABLE��, ����ԭ��SwitchAvSync()
extern int SysCb_SetAVEndFlag(__s32 nType);    //__cedarlib_clock_type_t
extern int SysCb_GetAudioDataInfo(int codec_type, unsigned char *pbuf, unsigned int length, __audio_stream_info_t *paudstrminfo);
extern int Cedarlib_BuildAbsPacketHdr(__audio_codec_format_t *pAbsInf, int uPacketLen, unsigned char *pBuf, int *pHdrLen);
extern int Cedarlib_UpdateAbsPacketHdr(__audio_codec_format_t *pAbsInf, int uPacketLen, unsigned char *pBuf, int uHdrLen);

#endif  /* _FILE_DEPACK_LIB_H_ */

