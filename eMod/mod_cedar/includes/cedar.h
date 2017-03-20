/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_cedar sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : cedar.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-2-10
* Description:
    cedar����ģ��Ķ���ͷ�ļ�(��cedar�м���ķ�Χ��),Ҳ���Ե����Ǹ��������ķ�����Ϣ����
    ����ͨ��cedar�̵߳ķ�����Ϣ������
********************************************************************************
*/
#ifndef _CEDAR_H_
#define _CEDAR_H_
enum
{
    _CEDAR_CMD_PLAY=1,              /* play                                     */
    _CEDAR_CMD_PAUSE,               /* pause                                    */
    _CEDAR_CMD_STOP,                /* stop                                     */
    _CEDAR_CMD_SET_PLAYLIST,        /* dir or playlist/playlistfile             */
    _CEDAR_CMD_FF,                  /* fast forward                             */
    _CEDAR_CMD_RR,                  /* fast backward                            */
    _CEDAR_CMD_CLEAR_FFRR,          /* clear fast forward/backword              */

    _CEDAR_CMD_STOP_RECORD = 0x20,
    
    _CEDAR_CMD_PLYEND = 0x80,       /* current file play end                    */
    _CEDAR_CMD_STEP,
    _CEDAR_CMD_JUMP,                //�û�����jump
    //_CEDAR_CMD_JUMP_ABPLAY,         //ABPlay�������jump
    _CEDAR_CMD_SEL_AUDSTREAM,       /* change aud stream                        */
    _CEDAR_CMD_SEL_SUBTITLE,        //ѡ����Ļ��

//�����Ǹ�ģ��ķ�����Ϣ
    _CEDAR_CMD_APLYMSG_AB_B_DONE=0xa0,    //aplyģ����ab����ʱ��������B��
    _CEDAR_CMD_APLYMSG_STOP,

    _CEDAR_CMD_VPLYMSG_AB_B_DONE=0xb0,      //vplyģ����ab����ʱ��������B��
    _CEDAR_CMD_VPLYMSG_STOP,

    _CEDAR_CMD_DISK_FULL=0xd0,  //¼��ʱ,muxerģ�鷢�ִ�����,���͵���Ϣ

	_CEDAR_CMD_SET_PANEL_SELECT,	//˫����ʾ��ѡ��0,LCD0; 1,LCD1

    _CEDAR_CMD_NOP,                 /* empty command, weekup cedar main task    */

    

    _CEDAR_CMD_
};

//�����ݽṹ�Ǹ�cedar->CedarCmdQ����Ϣ�����ƶ�����Ϣ�����ݽṹ
typedef struct tag_CedarMsg
{
    __u32   nMsgType;   //_CEDAR_CMD_PLAY��,��ģ��ķ�����Ϣ���;���Ҫ����;����ģ���Լ�����Ϣ���Ϳ��Բ�ͬ,�������ܺͲ��Ⱥ�ķ�����Ϣ����ֵ��ͻ.

    //������һЩ��ͬ��������ҪЯ������Ϣ.Ŀǰ_CEDAR_CMD_JUMP������ҪЯ����Ϣ
    //_CEDAR_CMD_JUMP��ҪЯ������Ϣ
    __s32           accurate_jump_flag; //�Ƿ�Ҫ��ȷ����, 1:��ȷ����, 0:��Ҫ��ȷ����
    __u32           des_tm;             //����ʱ��Ŀ��ʱ��㣬��offset timeΪ��׼
    __s32           nJumpReason;        //����������ԭ��. 0:�û�����; 1:�ڲ�����ab�������������.

    //_CEDAR_CMD_VPLYMSG_STOP��_CEDAR_CMD_APLYMSG_STOP��ҪЯ������Ϣ
    __s32           fatal_error_flag;

    //_CEDAR_CMD_SEL_SUBTITLE��Я������Ϣ
    __s32           nSubtitleStreamIndex;   //Ҫ����������Ļ�����±��
}CedarMsg;

#define CEDAR_PLUGINS_DIRECTORY   "d:\\mod\\cedar\\"
#define GINKGO_PLUGINS_DIRECTORY  "d:\\mod\\cedar\\"


#endif  /* _CEDAR_H_ */

