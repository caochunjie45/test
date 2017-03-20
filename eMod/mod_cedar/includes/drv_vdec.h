/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                    (c) Copyright 2008-2009, Kevin.Z China
*                                              All Rights Reserved
*
* File   : dev_vdec.h
* Version: V1.0
* By     : Kevin.Z
*********************************************************************************************************
*/
#ifndef _DRV_VDEC_H_
#define _DRV_VDEC_H_

typedef enum __VIDEO_DECODE_DRV_COMMAND
{                                           // command list for video decode driver
    DRV_VDEC_CMD_START=0x00,                // start video decoder��������DRV_VDEC_CMD_SET_VBSINF���������֮����ܵ��ñ�IO�ӿڣ���ΪҪ���ݲ�������Ӳ���Ĵ���������buffer�ȡ�
    DRV_VDEC_CMD_STOP,                      // stop video decoder
    DRV_VDEC_CMD_PAUSE,                     // û���õ�, pause video decoder, 
    DRV_VDEC_CMD_CONTI,                     // û���õ�, continue video decoder, 
    DRV_VDEC_CMD_FF,                        // fast forward,                                    pause->ff, 
    DRV_VDEC_CMD_RR,                        // fast reverse,                                    pause->rr
    DRV_VDEC_CMD_CLEAR_FFRR,                // clear fast forward/reverse,                      pause->play
    DRV_VDEC_CMD_GET_STATUS,                // get status of video decode driver, aux = 1��ʾ�ȵ�vdrv��״̬Ϊ�ȶ�״̬���ܷ���, aux = 0 ��ʾ��������
    DRV_VDEC_CMD_JUMP,                      // ����, aux = 1: jump forward; aux=0:jump backward,play->pause
    DRV_VDEC_CMD_CLEAR_JUMP,                // �������״̬,                                    pause->play

    DRV_VDEC_CMD_PREPARE_FF,                //play->pause, ��ν׼��,������vbs buffer,��ķ���������vdeclib�µ�״̬��Ȼ���һ��decode()�ӿ�., ret = 1��ʾvdrv�ܾ�����״̬ת��,��Ϊ�Ѿ�������dataend��.
    DRV_VDEC_CMD_PREPARE_RR,                //play->pause
    DRV_VDEC_CMD_PREPARE_CLEAR_FFRR,        //ffrr->pause

    DRV_VDEC_CMD_OPEN,                      //������Դ�� MOpen()ֻ�Ǹ������ʼ��malloc,��Դ��������.��������Ϊ��vdrvģ���IO�ӿڿ��Ե���.
    DRV_VDEC_CMD_CLOSE,                     //�ͷ���Դ
    DRV_VDEC_CMD_SET_CEDARINI,

    DRV_VDEC_CMD_SET_DATAINPUT=0x20,        // previewҲʹ��.   set data input
    DRV_VDEC_CMD_QUERY_VBSBUFFER,           // previewҲʹ��.   query free buffer
    DRV_VDEC_CMD_QUERY_FRAME,               // previewҲʹ��.   query frame to show
    DRV_VDEC_CMD_RELEASE_FRAME,             // previewҲʹ��.   release frame showing last time
    DRV_VDEC_CMD_DATA_END,                  // data end
    DRV_VDEC_CMD_QUERY_VBSBUFFER_USAGE,     //��ѯvbs buffer��ʹ���ʣ�����ǰװ�ص����ݺ���buffer�İٷֱȣ�ret = 0~100, 100����100%
    DRV_VDEC_CMD_SET_DISPLAY_REQUEST_MUTEX_MODE, //�ο�CEDAR_VDEC_CMD_SET_DISPLAY_REQUEST_MUTEX_MODE, aux = 0:�ж�Ҫ֡,����Ҫ����; aux = 1:�߳�Ҫ֡,��Ҫ����
    DRV_VDEC_CMD_ENABLE_AB,                 //֪ͨvdrv ׼��abplay, ret = EPDK_OK: vdrv����; EPDK_FAIL:vdrv��Ϊ�Ѿ�����data_end���˽����,�޷�״̬ת����,�ʾܾ�
    DRV_VDEC_CMD_CLEAR_AB,                  //���abplay, ret = EPDK_OK;

    DRV_VDEC_CMD_SET_CB_WAITVBS=0x40,       // set call back function, wait video bitstream data
    DRV_VDEC_CMD_SET_CB_SENDMSG,            // set call back function, send message frame
    DRV_VDEC_CMD_SET_CB_FREEVBSBUF,         // set call back function, free video bitstream buffer
    DRV_VDEC_CMD_SET_VBSINF,                // previewҲʹ��.   set video bitstream information for decode, �������DRV_VDEC_CMD_START֮ǰ��pbuffer = __vdec_mpeg4_vbs_inf_t*
    DRV_VDEC_CMD_SET_AVSHDL,
    DRV_VDEC_CMD_GET_TAG,                   // �õ������ֳ���Ϣ, pbuffer = __cedar_tag_inf_t*, �����߷����ڴ�
    DRV_VDEC_CMD_SET_TAG,                   // ���ý����ֳ���Ϣ, pbuffer = __cedar_tag_inf_t*, �����߷����ڴ�
    DRV_VDEC_CMD_SET_ROTATE,                // ���ý���ͼ����ת�ĽǶ�.aux = 0:Ĭ��Ϊԭͼ,1:˳ʱ��90��,2:180��,3:270��,4:ˮƽ����,5:��ֱ����
    DRV_VDEC_CMD_ENABLE_VIDEO_AUTO_SCALE,   //����vdrv�Զ���scale,�Լ��ٴ�������. aux = 1:enable; aux=0:disable.Ĭ��Ϊdisable.ֻ�����ڲ���ǰ���ã����Ź�����������Ч.
    DRV_VDEC_CMD_GET_FRMSIZE,               //CEDAR_VDEC_CMD_GET_FRMSIZE, ��ȡ��ʵͼ���С��return = (x<<16)|(y<<0)
    DRV_VDEC_CMD_SET_RESERVED_MEM_SIZE,     //����Ԥ�����ڴ�����,�����߽��������õ��ڴ�����ʱ,���ⲿ���ڴ�Ԥ������.
    DRV_VDEC_CMD_CHKSPT,                    //����Ƿ�֧�ֱ����ʽ, aux = CEDAR_VBS_TYPE_XVID, Ĭ����ģ��IO_Open()֮ǰ����
    DRV_VDEC_CMD_ENABLE_MAF,                //aux = 1:֧��maf
		DRV_VDEC_CMD_GET_VE_STATUS,						//��ȡ�����״̬�Ƿ��Ѿ�����ɹ������ص���һ���ṹ��				
    //Ϊ����ͼ���ӵ�ר�ýӿ�, ֱͨ�����.
    DRV_VDEC_CMD_OPEN_VDECLIB = 0x50,           //����open_drv,�ӽ�����open,������DRV_VDEC_CMD_START��.
    DRV_VDEC_CMD_CLOSE_VDECLIB,
    DRV_VDEC_CMD_SET_VDECLIB_PREVIEW_MODE,      //�ý�����״̬Ϊpreview.
    //DRV_VDEC_CMD_SET_VDECLIB_PLAY,            //�ý�����״̬ΪPLAY.������DRV_VDEC_CMD_START��.����Ҫ��
    DRV_VDEC_CMD_SET_VDECLIB_STOP,              //�ý�����״̬ΪSTOP.������DRV_VDEC_CMD_STOP��.
    DRV_VDEC_CMD_RESET_VDECLIB,                 //reset vdeclib
    DRV_VDEC_CMD_SET_VDECLIB_DATA_END,          //֪ͨ�����dataend
    DRV_VDEC_CMD_DECODE,                        //����һ�ν�����decode(). ret = VdrvReturnValue,��Ҫ��__vdrv_pic_dec_resultת��VdrvReturnValue.������һһ��Ӧ
		DRV_VDEC_CMD_SET_TV_MODE,									//1: TV mode, 0:media mode
    DRV_VDEC_CMD_

} __video_decode_drv_command_t;

//������Ƶ��������ģ��ķ���ֵ
typedef enum tag_VdrvReturnValue
{
    VDRV_OK             = EPDK_OK,        //no error
    VDRV_FAIL           = EPDK_FAIL,
    VDRV_ERR_VBS_FULL   = 6,   //vbs buffer�Ѿ�����,û�п��е�buffer.

    //��__vdrv_pic_dec_result(����������ඨ��ķ���ֵ)ת����,ֵ��֤һ��, VDRV_PIC_DEC_ERR_NONE
    VDRV_DEC_ERR_VE_BUSY         = -1,   // video engine is busy
    VDRV_DEC_ERR_VFMT_ERR        = -2,   // the format of the video is not supported, fatal error
    VDRV_DEC_ERR_COMMON          = -3,  
    VDRV_DEC_ERR_NONE            = EPDK_OK,    // decode one picture successed
    VDRV_DEC_ERR_FRMNOTDEC       = 1,    // decode one picture successed
    VDRV_DEC_ERR_VBS_UNDERFLOW   = 2,    // video bitstream underflow
    VDRV_DEC_ERR_NO_FRAMEBUFFER  = 3,    // there is no free frame buffer for decoder
    VDRV_DEC_ERR_DEC_VBSEND      = 4,    // decode all video bitstream end
    VDRV_DEC_I_FRAME             = 5,   // VC1 driver will return this decode result, һ������ʽ���ļ���mpg,vob,ts���ڿ������ʱ, psr��Ҫ�����Ϣ
    
    VDRV_ERR_
} VdrvReturnValue;

#endif /* _DRV_VDEC_H_ */

