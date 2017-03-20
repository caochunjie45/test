/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, Kevin.Z China
*                                              All Rights Reserved
*
* File   : video_plybk.h
* Version: V1.0
* By     : Kevin.Z
    ����API
*********************************************************************************************************
*/
#ifndef _VIDEO_PLYBK_H_
#define _VIDEO_PLYBK_H_

//#include "vdec_2_vply.h"
//0��ʾ����ʹ���ж�Ҫ֡����, 1��ʾ����ʹ���߳�����Ҫ֡�����DEDRV��֧�֣���ʹ��
//�ж�Ҫ֡���ƣ��ο�__VPLY_WORK_MODE.ĿǰĬ��ʹ���ж�Ҫ֡���ƣ���Ϊvply_task()��
//video decode task()�ڲ�����ʾ֡����ʱҪ���⣬��ԭ�ȵ�vdrv_lib�Ǹ����ж�Ҫ֡��Ƶ�,
//û�����̼߳以��Ĵ���.���Ҫ��ʽ�����߳�����Ҫ֡�������޸ĸ�vdrv_lib��decode()
//����������decode()�����ǲ���ȫ����sema����ġ���Ϊ�����˵�VEFINISH�жϵĹ��̣�
//����1080p���룬��ʱ20��ms,�����vply_task()�޷����ܡ����ڵ���������vply�߳�Ҫ֡ʱ���̵߳����Խ��������

#define VPLY_WORK_MODE_VALUE    (1) 
enum __VPLY_WORK_MODE
{
    VPLY_WORK_MODE_ISR = 0, //  �ж�Ҫ֡��֡ģʽ
    VPLY_WORK_MODE_TASK_ACTIVE = 1, //task����Ҫ֡��֡ģʽ
};
enum
{
    CEDAR_VPLY_CMD_START=0x00,          /* start                                */
    CEDAR_VPLY_CMD_STOP,                /* stop                                 */
    CEDAR_VPLY_CMD_PAUSE,               /* pause                                */
    CEDAR_VPLY_CMD_CONTI,               /* continue playing                     */
    CEDAR_VPLY_CMD_FF,                  /* fast forward                         */
    CEDAR_VPLY_CMD_RR,                  /* fast reverse                         */
    CEDAR_VPLY_CMD_CLEAR_FFRR,          /* clear fast forward/reverse           */
    CEDAR_VPLY_CMD_JUMP,                /* jump                         */
    CEDAR_VPLY_CMD_CLEAR_JUMP,          /* clear jump           */
    CEDAR_VPLY_CMD_ENABLE_AB,           //֪ͨaplyģ�飬ab���ſ�ʼ����������£�һ������Ƶ�ļ���jump��ʽ��ab����,aplyֻ����play״̬�²Ż���b���Ƿ��ѵ�
    CEDAR_VPLY_CMD_CLEAR_AB,            //֪ͨaplyģ�飬ab���Ž����������ټ��B����
    CEDAR_VPLY_CMD_SET_AB_B,                //����AB����ʱ��B���PTS, aux = pts, unit:ms
    CEDAR_VPLY_CMD_CLEAR_NOTIFYBDONEFLAG,   //AB����ʱ����֪ͨB�㵽��ı��λ��0.

    CEDAR_VPLY_CMD_OPEN = 0x20,     //������Դ
    CEDAR_VPLY_CMD_CLOSE,           //�ͷ���Դ
    CEDAR_VPLY_CMD_SET_AVSHDL,     /* set avsync module handle             */
    CEDAR_VPLY_CMD_SET_PICSKTIN,        /* picture buf from video decoder       */
    CEDAR_VPLY_CMD_SET_VIDLAYERHDL,     /* set video layer handle               */
    CEDAR_VPLY_CMD_SET_VIDWINDOW,       /* set video layer window information,������ģʽ   */
    CEDAR_VPLY_CMD_SWITCH_PIC_SHOW,     /* switch picture show                  */
    CEDAR_VPLY_CMD_INVALID_VIDEOLAYER,       /* ��ֹʹ��DE LAYER HANDLE, aux = 1, ��ֹʹ��; aux = 0,�ָ�ʹ��*/
    CEDAR_VPLY_CMD_GET_START_REQ_FRAME_LOOP_FLG,       /* ��ѯ�Ƿ����Ҫ֡ѭ��, ret = 1��ʾ�Ѿ����� */
    CEDAR_VPLY_CMD_SET_WORK_MODE,           /* aux = enum __VPLY_WORK_MODE, ��mopen()�Ժ󣬵�һ�����Ĳ������������IO�ӿڣ����ù���ģʽ! Ȼ����mclose()֮ǰ�������ٸ� */
    //CEDAR_VPLY_CMD_SET_MSG_QUEUE,           //����ͨ��cedar����ģ��ķ�����Ϣͨ��,ab����ʱ����⵽B���ѵ���ͨ����Ϣͨ��֪ͨCEDAR����ģ��ȥ����, pbuffer = __krnl_event_t  *CmdQ
    CEDAR_VPLY_CMD_SET_CB_MSGQPOST,           //����ͨ��cedar����ģ��ķ�����Ϣͨ��,ab����ʱ����⵽B���ѵ���ͨ����Ϣͨ��֪ͨCEDAR����ģ��ȥ����, pbuffer = __krnl_event_t  *CmdQ
    CEDAR_VPLY_CMD_SET_FILE_SWITCH_VPLY_MODE,   //aux = CEDAR_FILE_SWITCH_VPLY_MODE_GAP��
    CEDAR_VPLY_CMD_SET_CB_SETSRCSCNWINPARA, //����callback����,���ھ���srcwin��scnwin����ʾ����. pbuffer = __pCBK_t.
    CEDAR_VPLY_CMD_CHECK_PIC_FRAME_REDUNDANT, //��ѯvply��֡�����Ƿ��ж����֡,EPDK_TRUE:��ʾvply֡������֡����������dedrv��Ҫ���֡��, EPDK_FALSE:С�ڵ���dedrv��Ҫ���֡��(dedrv����Ҫ������2֡)
    CEDAR_VPLY_CMD_SET_DISPLAY_MODE_OUT,      //������Ƶ����ģʽ 2D/3D  
    CEDAR_VPLY_CMD_SET_ROTATE,                  //������ת�Ƕ�,aux = pCedar->rotate_mode, 
    CEDAR_VPLY_CMD_SET_DEINTERLACE_LEVEL,       //��ΪĿǰvideo_start�ӿ�����˱���.�ڽӿ���ֱ������Ҳ����,�����ܱ�֤video_start�Ѿ����ù�.��¼deinterlace�ĵȼ�. aux = 2:leve2��deinterlace, ���������Ὺʼ��������,de������ѡ��mafģʽ,����ռ�����;1:level1��deinterlace,de����Ӧ�û�ѡ��maf_bobģʽ,����ռ���Դ�һЩ. 0:�ر�.Ĭ�Ϲر�.
    //CEDAR_VPLY_CMD_CAPTURE_PIC = 0x30,  /* ����, pbuffer = __cedar_pic_info_t     */
    //CEDAR_VPLY_CMD_ASK_PIC_BUFSIZE,     /* ��CEDAR��ѯ��Ҫ����buffer�Ա�APP���䣬pbuffer = __cedar_pic_info_t, ret = size, -1����ʧ��*/
    CEDAR_VPLY_CMD_SCALE_FRAME = 0x40,           /* ��ͼ, ͬ������. �ѵ�ǰ������ʾ��frame��һ��scale, pbuffer = FB*, FB�Ѿ�������ڴ棬����ȫ����ã�����ֱ����*/
    CEDAR_VPLY_CMD_DISABLE_ALPHA_BLENDING,
    CEDAR_VPLY_CMD_ENABLE_ALPHA_BLENDING,
    CEDAR_VPLY_CMD_CONFIG_VIDEO_LYR_CK_ENABLE,  // �������ø�maple����Ƶͼ���ck_enable������ֵ, aux = 1:enable; aux = 0:disable, 
    CEDAR_VPLY_CMD_CHECK_PICTURE_OUT,      //ret = EPDK_TRUE,��ʾ.��������Ƶʱ����ѯ��һ֡ͼ���Ƿ��Ѿ���ʾ����;�������Ƶ,�򷵻�ֵʼ��ΪEPDK_FALSE.


    CEDAR_VPLY_CMD_SET_NEW_VIDEO_PTS = 0x50,           //aux = ms.�ڵ��ӹ㲥��,��pts��ͷ��,vply��Ҫ��������vclock���µ�ʱ��,���ʱ���Ǿ���ǰ���ģ�鴦�����ʱ��. vclock����Ϊstream clock������.vply���ڱ���Ǹ�ʱ�����֡������,������ʱ����.��ʵ�൱��ƽ���ĸ���ʱ������.
    CEDAR_VPLY_CMD_SET_DTV_DELAY_TIME,                  //�ڵ��ӹ㲥��,������cedar�����������õĻ���ʱ��.������ǰ��DMX/TSC�Ļ���ʱ��.��λms,����PTS��ͷʱʹ��
    CEDAR_VPLY_CMD_SET_AV_INTERVAL,                 //�ڵ��ӹ㲥��,��һ���յ�����Ƶpts����Ƶpts�Ĳ�ֵ,videoPts - audioPts, Ϊ����ʾvideo����, audio��Ҫ����׷video.
	
    CEDAR_VPLY_CMD_
};
//extern __cedar_vply_t   *cedar_vply;

typedef enum tag_VplyRetVal
{
    VPLY_OK             = EPDK_OK,
    VPLY_FAIL           = EPDK_FAIL,
    VPLY_ERR_TASK_STOP  = 1,    //�߳�׼��ֹͣ
    VPLY_ERR_,
} VplyRetVal;

extern __s32   vply_MInit(void);
extern __s32   vply_MExit(void);
extern __mp    *vply_MOpen(__u32 mid, __u32 mode);
extern __s32   vply_MClose(__mp *mp);
extern __u32 vply_MRead(void *pdata, __u32 size, __u32 n, __mp *mp);
extern __u32 vply_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp);
extern __s32   vply_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer);

#endif /*_VIDEO_PLYBK_H_*/

