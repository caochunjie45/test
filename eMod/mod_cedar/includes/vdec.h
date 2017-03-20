/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, Kevin.z China
*                                              All Rights Reserved
*
* File   : vdec.h
* Version: V1.0
* By     : Kevin.Z
*********************************************************************************************************
*/
#ifndef _VDEC_H_
#define _VDEC_H_

typedef enum __VIDEO_DECODER_COMMAND
{
    CEDAR_VDEC_CMD_START=0x00,      /* ��������                                 */
    CEDAR_VDEC_CMD_STOP,            /* ֹͣ����                                 */
    CEDAR_VDEC_CMD_PAUS,            /* û���õ�, ��ͣ����, �����߲�����������pause�ӿڵ�,�������ӿڻ�����pause״̬ */
    CEDAR_VDEC_CMD_CONT,            /* û���õ�, �ָ�����, �����߲�����������continue�ӿ�,  */
    CEDAR_VDEC_CMD_FF,              /* ���ÿ��ģʽ,pause->ff                             */
    CEDAR_VDEC_CMD_RR,              /* ���ÿ���ģʽ, pause->rr                       */
    CEDAR_VDEC_CMD_CLEAR_FFRR,      /* ����������ģʽ��������pause->play                         */
    CEDAR_VDEC_CMD_GET_STATUS,  //aux = 1һ��Ҫ����ƽ��״̬;aux=0��������
    CEDAR_VDEC_CMD_JUMP,            /* ��������ģʽ,aux = Ŀ��ʱ���,play->pause            */
    CEDAR_VDEC_CMD_CLEAR_JUMP,      /* �������״̬��������pause->play                         */

    CEDAR_VDEC_CMD_PREPARE_FF,      //��playת��ff,rr,��Ҫ����vdec����׼��,׼���ú󣬻����pause״̬����.����jumpҲ�ǽ���pause״̬����. play->pause
    CEDAR_VDEC_CMD_PREPARE_RR,      //play->pause
    CEDAR_VDEC_CMD_PREPARE_CLEAR_FFRR,  //ffrr->pause
    

    CEDAR_VDEC_CMD_ENABLE_AB,   //֪ͨvdrv׼��ab����.���vdrv�Ѿ�������dataend�������,��ô����EPDK_FAIL;
    CEDAR_VDEC_CMD_CLEAR_AB,    //���abplay, ret = EPDK_OK;

    CEDAR_VDEC_CMD_CHKSPT=0x20,     //aux = CEDAR_VBS_TYPE_MPEG12_LAYER1, ֻ�Ǳ�ʾ�ܷ�֧��,������ǰ�Ѿ�������vdrv���߼��ص�vdrv���ǵ�ǰ��ѯ��vdrv type
    CEDAR_VDEC_CMD_SET_VBSSKTIN,    /* ����video bit stream ����                */
    CEDAR_VDEC_CMD_GET_PICSKTOUT,   /* ��ȡ����� picture data buffer ��ַ      */
    CEDAR_VDEC_CMD_GET_TIME,        /* ��ȡ��ǰ����ʱ��                         */
    CEDAR_VDEC_CMD_SET_AVSHDL,      /* ����AV Syncģ����                      */
    CEDAR_VDEC_CMD_GET_FRMSIZE,     /* ��ȡ֡��С��return = (x<<16)|(y<<0)      */
    CEDAR_VDEC_CMD_GET_FRMRATE,     /* ��ȡ��Ƶ֡��                             */
    CEDAR_VDEC_CMD_GET_TAG,         /* ��ȡ������ֳ���Ϣ, pbuffer = __cedar_tag_inf_t*, �����߷����ڴ� */
    CEDAR_VDEC_CMD_SET_TAG,         /* ���ûָ�������ֳ���Ϣ����decode��ʼǰ��, pbuffer = __cedar_tag_inf_t*�������߷����ڴ� */
    CEDAR_VDEC_CMD_SET_ROTATE,      /* ���ý���ͼ����ת�ĽǶ�.aux = 0:Ĭ��Ϊԭͼ,1:˳ʱ��90��,2:180��,3:270��,4:ˮƽ����,5:��ֱ����*/
    CEDAR_VDEC_CMD_QUERY_VBSBUFFER_USAGE,   //��ѯvbs buffer��ʹ���ʣ�����ǰװ�ص����ݺ���buffer�İٷֱȣ�ret = 0~100, 100����100%
    CEDAR_VDEC_CMD_SET_DISPLAY_REQUEST_MUTEX_MODE,  //Ϊvdrv����displayrequest�Ƿ���Ҫ��semaphore����.��Ӧ�ж�Ҫ֡���߳�Ҫ֡, aux = 0:����Ҫ; aux = 1:��Ҫ����
    CEDAR_VDEC_CMD_OPEN,            //vdec��ʼ������Դ��Ŀǰ��Ҫ��open vdrv.�������Դ��MOpen()ʱ�ͷ������, aux = CEDAR_VBS_TYPE_XVID��
    CEDAR_VDEC_CMD_CLOSE,           //vdec��ʼ�ͷ���Դ��close vdrv
    CEDAR_VDEC_SET_VBSTYPE,         //��֪����,��ɾ��.���ñ�������, aux = CEDAR_VBS_TYPE_XVID��, ret = EPDK_OK��ʾ�ɹ�,������������
    CEDAR_VDEC_CMD_ENABLE_VIDEO_AUTO_SCALE,  //����vdrv�Զ���scale,�Լ��ٴ�������. aux = 1:enable; aux=0:disable.Ĭ��Ϊdisable.ֻ�����ڲ���ǰ���ã����Ź�����������Ч.
    CEDAR_VDEC_CMD_SET_RESERVED_MEM_SIZE,   //����Ԥ�����ڴ�����,�����߽��������õ��ڴ�����ʱ,���ⲿ���ڴ�Ԥ������.
    CEDAR_VDEC_CMD_UPDATE_VDRV,     //���¼��ؽ�������, aux = CEDAR_VBS_TYPE_XVID��
    CEDAR_VDEC_CMD_ENABLE_MAF,      //�Ƿ��maf����, aux = 1,��, aux = 0,�ر�, Ĭ�ϴ�.
		CEDAR_VDEC_CMD_SET_TV_MODE,			// 1:TV MODE 0:MEDIA MODE
		CEDAR_VDEC_CMD_GET_VE_STATUS, //��ȡ����⹤��״̬���Ƿ����ɹ��ȣ����ص���һ��ָ��ָ��ǿ��ת����S32�Ľṹ��
    CEDAR_VDEC_CMD_SET_CEDARINI,    //����cedarini, Ҫ����IOOpen֮������

    CEDAR_VDEC_CMD_

} __video_decoder_command_t;


#endif /*_VDEC_H_*/
