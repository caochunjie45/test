/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                    (c) Copyright 2008-2009, Kevin.Z China
*                                              All Rights Reserved
*
* File   : adec.h
* Version: V1.0
* By     : Kevin.Z
*********************************************************************************************************
*/
#ifndef _ADEC_H_
#define _ADEC_H_

typedef enum __AUDIO_DECODER_COMMAND
{
    CEDAR_ADEC_CMD_START=0x00,          /* ��������                             */
    CEDAR_ADEC_CMD_STOP,                /* ֹͣ����                             */
    CEDAR_ADEC_CMD_PAUS,                /* ֹͣ����                             */
    CEDAR_ADEC_CMD_CONT,                /* ֹͣ����                             */
    CEDAR_ADEC_CMD_FF,                  /* ���ÿ��ģʽ,        pause->ff                         */
    CEDAR_ADEC_CMD_RR,                  /* ���ÿ���ģʽ,        pause->rr                         */
    CEDAR_ADEC_CMD_CLEAR_FFRR,          /* ����������ģʽ,    pause->play                     */
    CEDAR_ADEC_CMD_GET_STATUS,           /* ��ȡadec��ǰ״̬,Ŀǰ����������              */
    CEDAR_ADEC_CMD_JUMP,                /* ����ģʽ, aux = des_tm,ms; pbuffer = 1����Ҫ��ȷ����, play->pause                         */
    CEDAR_ADEC_CMD_CLEAR_JUMP,          /* �������״̬,        pause->play                     */
    
    CEDAR_ADEC_CMD_PREPARE_FF,      //���ǰ��׼��, PLAY->pause, ��Ƶ�ļ���adec����Ƶ�ļ���adec����ʽ��ͬ
    CEDAR_ADEC_CMD_PREPARE_RR,      // play->pause
    CEDAR_ADEC_CMD_PREPARE_CLEAR_FFRR,  //�ӿ�����������ŵ�׼��, FFRR->pause,

    CEDAR_ADEC_CMD_SET_ABSSKTIN=0x20,   /* ����audio bit stream ����            */
    CEDAR_ADEC_CMD_GET_PCMSKTOUT,       /* ��ȡ�����pcm double buffer ��ַ     */
    CEDAR_ADEC_CMD_GET_RDRTYPE,
    CEDAR_ADEC_CMD_CHKSPT,              //aux = CEDAR_ABS_TYPE_MPEG12_LAYER1, pbuffer = nRawDataFlag,ֻ�Ǳ�ʾ�ܷ�֧��,������ǰ�Ѿ�������adrv���߼��ص�adrv���ǵ�ǰ��ѯ��adrv
    CEDAR_ADEC_CMD_SET_SPEED,           /* ���ò����ٶ�,speed=0:PLAY,<0:RR,>0:FF*/
    CEDAR_ADEC_CMD_SET_AVSHDL,
    CEDAR_ADEC_CMD_GET_ADRVHDL,         /* get audio driver handle              */
    CEDAR_ADEC_CMD_SET_WITH_VIDEO,      /* PLAY WITH VIDEO,�������Ƶpsr,������Ϊ1, �����ļ��Ƿ������Ƶ����                      */
    CEDAR_ADEC_CMD_IS_RAWDATA,          /* ��鱾ģ���Ƿ���RAW_DATA decode��ģ��, EPDK_TRUE��ʾ��RAW_data, EPDK_FALSE��ʾ����RAW_data*/
    CEDAR_ADEC_CMD_QUERY_ABSBUFFER_USAGE, //��ѯabs buffer��ʹ���ʣ�����ǰװ�ص����ݺ���buffer�İٷֱȣ�ret = 0~100, 100����100%

    CEDAR_ADEC_CMD_GET_TIME=0x40,       /* ��ȡ��ǰ����ʱ��                     */
    CEDAR_ADEC_CMD_GET_SPEED,           /* ��ȡ��ǰ�����ٶ�                     */
    CEDAR_ADEC_CMD_GET_BITRATE,         /* ��ȡ��ǰ����bit rate                 */
    CEDAR_ADEC_CMD_GET_BITSPERSAMPLE,   /* ��ȡbits per second                  */
    CEDAR_ADEC_CMD_GET_FS,              /* ��ȡ����Ƶ��                         */
    CEDAR_ADEC_CMD_GET_CHN,             /* ��ȡ������                           */
    CEDAR_ADEC_CMD_SET_CHN,             /* ����������                           */

    CEDAR_ADEC_CMD_SET_AB_A,            /* ����A-B����ģʽ��A��, aux = ms,����Ϊ��Ƶ�ļ���fseek��ʽ��ab����׼���Ľӿں���                 */
    CEDAR_ADEC_CMD_SET_AB_B,            /* ����A-B����ģʽ��B��, aux = BPointPts, ms                 */
    CEDAR_ADEC_CMD_SET_AB_LOOPCNT,      /* ����A-B���ŵ�ѭ������                */
    CEDAR_ADEC_CMD_CLEAR_AB,            /* ���A-B����ģʽ                      */
    CEDAR_ADEC_CMD_ENABLE_AB,           /* ����AB����                           */
    //CEDAR_ADEC_CMD_SET_AB_MODE,         /* ����A-B���ŵ�ʵ�ַ�ʽ,aux = ADRV_AB_MODE_JUMP��  */ 

    
    CEDAR_ADEC_CMD_GET_TAG,             /* ��ȡ��Ƶ���ŵı�ǩ��Ϣ               */
    CEDAR_ADEC_CMD_SET_FRSPEED,         /* ���ÿ���������ٶ�                   */
    CEDAR_ADEC_CMD_GET_FRSPEED,         /* ��ȡ����������ٶ�                   */

    
    CEDAR_ADEC_CMD_OPEN = 0x60,                //������Դ, aux, pbuffer����û��������. aux = CEDAR_ABS_TYPE_MPEG12_LAYER1, pbuffer = 1��ʾ��rawdata
    CEDAR_ADEC_CMD_CLOSE,               //�ͷ���Դ
    //CEDAR_ADEC_CMD_SET_ABSTYPE,         //�ýӿڲ�֪����,��ɾ��. ����abstype��aux =CEDAR_ABS_TYPE_MPEG12_LAYER1,  һ���ڽ������ڵ�����²���Ч��ret = EPDK_OK��ʾ����������
    CEDAR_ADEC_CMD_UPDATE_ADRV,         //֪ͨadecģ����¼���Ҫ���adrv, aux = CEDAR_ABS_TYPE_MPEG12_LAYER1, pbuffer = nRawDataFlag
    CEDAR_ADEC_CMD_GET_ADRV_STATUS,     /* �õ�adrvģ���״̬, aux = 1:��ʾһ��Ҫ�����ȶ�״̬; aux=0��ʾ��������.   */
    CEDAR_ADEC_CMD_SET_CEDARINI,        //����cedarini, Ҫ����IOOpen֮������
    

    CEDAR_ADEC_CMD_

} __audio_decoder_command_t;

//typedef enum __ADEC_INTERNAL_COMMAND
//{
//    CEDAR_ADEC_INTERNAL_CMD_IS_RAWDATA = 0x00,      /* ����Ƿ���raw data mode��adec plugin, ��:ret=EPDK_TRUE, ��:ret=EPDK_FALSE */
//    CEDAR_ADEC_INTERNAL_CMD_
//
//} __adec_internal_command_t;

#endif /*_ADEC_H_*/
