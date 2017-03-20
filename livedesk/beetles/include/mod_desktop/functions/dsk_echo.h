#ifndef __COM_ECHO_H__
#define __COM_ECHO_H__

#include "apps.h"

//type:AUDIO_REC_USR_MIC/AUDIO_REC_USR_FM....
//auto_clean_buf:0�������Զ����buffer(¼��Ӧ��)��Ϊ1�����Զ����buffer(��¼����Ӧ��)
//��ϸ���÷����μ�SDK��app_root_scene.c��record_view_doc.c
__s32 dsk_audio_echo_open(__audio_rec_user_t type, __s32 auto_clean_buf);

//��ϸ���÷����μ�SDK��app_root_scene.c��record_view_doc.c
__s32 dsk_audio_echo_close(__s32 direct_off);

//�ú���һ�㲻Ҫ���ã�����Ĭ�ϵľ�����
//0<= pre_gain <= 3
__s32 dsk_audio_echo_set_mic_pre_gain(__s32 pre_gain);

//�ú���һ�㲻Ҫ���ã�����Ĭ�ϵľ�����
//0<= gain <= 7
__s32 dsk_audio_echo_set_mic_gain(__s32 gain);

//�ýӿ���ʱ���ã�CPU�ܲ���������mic������-32��32�����Ե���mic����
__s32 dsk_audio_echo_gain_incr(void);

//�ýӿ���ʱ���ã�CPU�ܲ�������Сmic���� -32��32�����Ե���mic����
__s32 dsk_audio_echo_gain_decr(void);

//�ú��������ã�������
__s32 dsk_audio_echo_pt_regs(__u32 start, __u32 end);

/*aux: 0<= aux <= 10 ���û�����ʱ������Ĭ��Ϊ3*/
__s32 dsk_audio_echo_set_delay_num(__s32 aux);

/*aux:10<= aux <= 3000,Ĭ��Ϊ100���Ժ���Ϊ��λ*/
__s32 dsk_audio_echo_set_delay_time(__s32 aux);

/*�Ƿ�����MICХ�У�aux: 0:disable ec, 1: enable ec*/
__s32 dsk_audio_echo_enable_ec(__s32 aux);
/*�ýӿ���ʱ�����ã�CPU�ܲ��������ñ��ģʽ:
mode: 0xff:�û�ģʽ��1:��Ů�� 2:������ 3:��ͯ��
level:���mode=0xff,��ôlevelΪ����ȼ�-8<=level<=8, ���mode!=0xff����ôlevel��Ч*/
__s32 dsk_audio_echo_set_pitch_mode(__s32 mode, __s32 level);

__s32 dsk_audio_echo_is_open(void);

/*�Ƿ�򿪻��������aux: 0:disable , 1: enable */
__s32 dsk_audio_echo_enable_output(__s32 aux);



#endif//__COM_ECHO_H__


