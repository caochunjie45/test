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
#ifndef _SUBTITLE_PLYBK_H_
#define _SUBTITLE_PLYBK_H_

//#include "vdec_2_vply.h"
//0��ʾ����ʹ���ж�Ҫ֡����, 1��ʾ����ʹ���߳�����Ҫ֡�����DEDRV��֧�֣���ʹ��
//�ж�Ҫ֡���ƣ��ο�__VPLY_WORK_MODE.ĿǰĬ��ʹ���ж�Ҫ֡���ƣ���Ϊvply_task()��
//video decode task()�ڲ�����ʾ֡����ʱҪ���⣬��ԭ�ȵ�vdrv_lib�Ǹ����ж�Ҫ֡��Ƶ�,
//û�����̼߳以��Ĵ���.���Ҫ��ʽ�����߳�����Ҫ֡�������޸ĸ�vdrv_lib��decode()
//����������decode()�����ǲ���ȫ����sema����ġ���Ϊ�����˵�VEFINISH�жϵĹ��̣�
//����1080p���룬��ʱ20��ms,�����vply_task()�޷����ܡ����ڵ���������vply�߳�Ҫ֡ʱ���̵߳����Խ��������

//#define VPLY_WORK_MODE_VALUE    (1) 
//enum __VPLY_WORK_MODE
//{
//    VPLY_WORK_MODE_ISR = 0, //  �ж�Ҫ֡��֡ģʽ
//    VPLY_WORK_MODE_TASK_ACTIVE = 1, //task����Ҫ֡��֡ģʽ
//};
enum
{
    CEDAR_SPLY_CMD_START=0x00,          /* start                                */
    CEDAR_SPLY_CMD_STOP,                /* stop                                 */
    CEDAR_SPLY_CMD_PAUSE,               /* pause                                */
    CEDAR_SPLY_CMD_CONTI,               /* continue playing                     */
    CEDAR_SPLY_CMD_FF,                  /* fast forward                         */
    CEDAR_SPLY_CMD_RR,                  /* fast reverse                         */
    CEDAR_SPLY_CMD_CLEAR_FFRR,          /* clear fast forward/reverse           */
    CEDAR_SPLY_CMD_JUMP,                /* jump                         */
    CEDAR_SPLY_CMD_CLEAR_JUMP,          /* clear jump           */
    CEDAR_SPLY_CMD_ENABLE_AB,           //֪ͨaplyģ�飬ab���ſ�ʼ����������£�һ������Ƶ�ļ���jump��ʽ��ab����,aplyֻ����play״̬�²Ż���b���Ƿ��ѵ�
    CEDAR_SPLY_CMD_CLEAR_AB,            //֪ͨaplyģ�飬ab���Ž����������ټ��B����
    CEDAR_SPLY_CMD_SET_AB_B,                //����AB����ʱ��B���PTS, aux = pts, unit:ms
    CEDAR_SPLY_CMD_CLEAR_NOTIFYBDONEFLAG,   //AB����ʱ����֪ͨB�㵽��ı��λ��0.

    CEDAR_SPLY_CMD_OPEN = 0x20,     //������Դ
    CEDAR_SPLY_CMD_CLOSE,           //�ͷ���Դ

    
    CEDAR_SPLY_CMD_SET_AVSHDL,     /* set avsync module handle             */
    CEDAR_SPLY_CMD_SET_LDECHDL,     /* set subtitle decode handle             */



    
    CEDAR_SPLY_CMD_SET_SUBSKTIN,        /* picture buf from subtitle decoder       */
    //CEDAR_SPLY_CMD_SET_VIDLAYERHDL,     /* set video layer handle               */
    //CEDAR_SPLY_CMD_SET_VIDWINDOW,       /* set video layer window information,������ģʽ   */
    CEDAR_SPLY_CMD_SWITCH_SUB_SHOW,     /* switch subtitle show                  */
    //CEDAR_SPLY_CMD_INVALID_VIDEOLAYER,       /* ��ֹʹ��DE LAYER HANDLE, aux = 1, ��ֹʹ��; aux = 0,�ָ�ʹ��*/
    CEDAR_SPLY_CMD_GET_START_REQ_FRAME_LOOP_FLG,       /* ��ѯ�Ƿ����Ҫ֡ѭ��, ret = 1��ʾ�Ѿ����� */
    CEDAR_SPLY_CMD_SET_WORK_MODE,           /* aux = enum __SPLY_WORK_MODE, ��mopen()�Ժ󣬵�һ�����Ĳ������������IO�ӿڣ����ù���ģʽ! Ȼ����mclose()֮ǰ�������ٸ� */
    //CEDAR_SPLY_CMD_SET_MSG_QUEUE,           //����ͨ��cedar����ģ��ķ�����Ϣͨ��,ab����ʱ����⵽B���ѵ���ͨ����Ϣͨ��֪ͨCEDAR����ģ��ȥ����, pbuffer = __krnl_event_t  *CmdQ
    CEDAR_SPLY_CMD_SET_CB_MSGQPOST,           //����ͨ��cedar����ģ��ķ�����Ϣͨ��,ab����ʱ����⵽B���ѵ���ͨ����Ϣͨ��֪ͨCEDAR����ģ��ȥ����, pbuffer = __krnl_event_t  *CmdQ
    CEDAR_SPLY_CMD_SET_CB_SHOWDRAW,           //����Ļ   
    CEDAR_SPLY_CMD_SET_FILE_SWITCH_SPLY_MODE,   //aux = CEDAR_FILE_SWITCH_SPLY_MODE_GAP��
    CEDAR_SPLY_CMD_SET_CB_SETSRCSCNWINPARA, //����callback����,���ھ���srcwin��scnwin����ʾ����. pbuffer = __pCBK_t.
    CEDAR_SPLY_CMD_CHECK_PIC_FRAME_REDUNDANT, //��ѯSPLY��֡�����Ƿ��ж����֡,EPDK_TRUE:��ʾSPLY֡������֡����������dedrv��Ҫ���֡��, EPDK_FALSE:С�ڵ���dedrv��Ҫ���֡��(dedrv����Ҫ������2֡)

    //CEDAR_SPLY_CMD_CAPTURE_PIC = 0x30,  /* ����, pbuffer = __cedar_pic_info_t     */
    //CEDAR_SPLY_CMD_ASK_PIC_BUFSIZE,     /* ��CEDAR��ѯ��Ҫ����buffer�Ա�APP���䣬pbuffer = __cedar_pic_info_t, ret = size, -1����ʧ��*/
    //CEDAR_SPLY_CMD_SCALE_FRAME = 0x30,           /* ��ͼ, ͬ������. �ѵ�ǰ������ʾ��frame��һ��scale, pbuffer = FB*, FB�Ѿ�������ڴ棬����ȫ����ã�����ֱ����*/
    //CEDAR_SPLY_CMD_DISABLE_ALPHA_BLENDING,
    //CEDAR_SPLY_CMD_ENABLE_ALPHA_BLENDING,

   
    CEDAR_SPLY_CMD_SET_NEW_VIDEO_PTS = 0x40,           //aux = ms.�ڵ��ӹ㲥��,��pts��ͷ��,SPLY��Ҫ��������vclock���µ�ʱ��,���ʱ���Ǿ���ǰ���ģ�鴦�����ʱ��. vclock����Ϊstream clock������.SPLY���ڱ���Ǹ�ʱ�����֡������,������ʱ����.��ʵ�൱��ƽ���ĸ���ʱ������.
    CEDAR_SPLY_CMD_SET_DTV_DELAY_TIME,                  //�ڵ��ӹ㲥��,������cedar�����������õĻ���ʱ��.������ǰ��DMX/TSC�Ļ���ʱ��.��λms,����PTS��ͷʱʹ��
    CEDAR_SPLY_CMD_SET_AV_INTERVAL,                 //�ڵ��ӹ㲥��,��һ���յ�����Ƶpts����Ƶpts�Ĳ�ֵ,videoPts - audioPts, Ϊ����ʾvideo����, audio��Ҫ����׷video.
	
    CEDAR_SPLY_CMD_
};
//extern __cedar_vply_t   *cedar_vply;

typedef enum tag_SplyRetVal
{
    SPLY_OK             = EPDK_OK,
    SPLY_FAIL           = EPDK_FAIL,
    SPLY_ERR_TASK_STOP  = 1,    //�߳�׼��ֹͣ
    SPLY_ERR_,
} SPLYRetVal;

typedef struct __TAG_subtitle_inf
{
	__subtitle_profile_t    *sub_list;
//	__maple_isdb_item_t     *cur_isdb_item;
//    __maple_isdb_item_t     *last_isdb_item;
    CSubtitleDrawItem     *cur_isdb_item;   //�����ڴ涼���ڳ�ʼ��ʱmalloc������.
    CSubtitleDrawItem     *last_isdb_item;
}__sub_inf;


//extern __s32   sply_MInit(void);
//extern __s32   sply_MExit(void);
//extern __mp    *sply_MOpen(__u32 mid, __u32 mode);
//extern __s32   sply_MClose(__mp *mp);
//extern __u32   sply_MRead(void *pdata, __u32 size, __u32 n, __mp *mp);
//extern __u32   sply_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp);
//extern __s32   sply_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer);

#endif /*_SUBTITLE_PLYBK_H_*/

