/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                            TOUCHPANEL DRIVER
*
*                                    (c) Copyright 2011-2014, Jackie China
*                                             All Rights Reserved
*
* File    : drv_touchpanel.h
* By      : Jackie
* Version : v1.0
* Date    : 2011-12-26
* Descript: bsp touchpanel header file.
* Update  : date                auther      ver     notes
*           2011-12-26 15:07    Jackie      1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __DRV_TOUCHPANEL_H__
#define __DRV_TOUCHPANEL_H__

#include "typedef.h"
#include "cfgs.h"

#define    TP_DEFAULT_MSG_PERTIME           (3)
#define    TP_DEFAULT_OFFSET                (6)

#define    EV_TP_PRESS_START                0xf0    //���µ����
#define    EV_TP_PRESS_MOVE                 0xf1    //����ʱ���ڻ���
#define    EV_TP_PRESS_HOLD                 0xf2    //����ʱ�򲻶�, ����

#define    EV_TP_PINCH_IN                   0xf4   //  zoom in 
#define    EV_TP_PINCH_OUT                  0xf5   //  zoom out
#define    EV_TP_PINCH_HOLD                 0xf6   //  hold


/************************����������*************************
*
*����ͬ������msg_type�������У�Ĭ�ϳ���һ��̧����Ϣ
*
*************************************************************/
#define    EV_TP_ACTION_NULL				0x00	//�հ׶������޷�ʶ��Ķ���
#define    EV_TP_ACTION_CLICK				0x01	//����������
#define    EV_TP_ACTION_DOWN				0x02	//�������»�--used
#define    EV_TP_ACTION_UP				    0x12	//�������ϻ�--used
#define    EV_TP_ACTION_LEFT				0x03	//��������--used
#define    EV_TP_ACTION_RIGHT				0x13	//�������һ�--used
#define    EV_TP_ACTION_ANTICLOCKWISE		0x04	//��������ʱ�뻬��
#define    EV_TP_ACTION_CLOCKWISE			0x14	//������˳ʱ�뻬��
#define    EV_TP_ACTION_LD2RU				0x05	//������б�߻���,���µ�����
#define    EV_TP_ACTION_RU2LD      		    0x15	//������б�߻���,���ϵ�����
#define    EV_TP_ACTION_LU2RD	    		0x06	//��������б�߻���,���ϵ�����
#define    EV_TP_ACTION_RD2LU 			    0x16	//��������б�߻���,���µ�����

typedef struct __EV_TP_MSG
{
    __u8   msg_pattern;     //��Ϣ���࣬���ں������豸���ݣ��û���ʱ������,����
    __u8   msg_type;        //��Ϣ���ͣ������ǰ��µ���㣬���������²�����������̧��ʱ�������
    __u16  xpoint;          //��ǰ���X����ֵ
    __u16  ypoint;          //��ǰ���Y����ֵ
    __u16  speed_dir;       //̧��ʱ���ƶ��ķ���
    __u16  speed_val;       //̧��ʱ���ٶȵĴ�С, �ٶȷּ�:0-6

}__ev_tp_msg_t;

typedef struct __EV_TP_POS
{
    __u16  disp_xpos;       //��Ļ��Ӧ��x,y����ֵ
    __u16  disp_ypos;
}__ev_tp_pos_t;

/*
����TP����ĻУ׼

    Tp����ĻУ׼commandֻ��һ������DRV_TP_CMD_ADJUST��Ҫ��������������Ҫ����ʹ���Ĵ���������
    ��Ҫһ�����ݽṹ  __ev_tp_pos_t  my_pos;

    �������� eLIBs_fioctrl(hdle, DRV_TP_CMD_ADJUST, 0, (void *)&my_pos);

            �����ݽṹmy_pos��disp_xpos��disp_ypos��������Ļ����㣬�� aux ����дУ׼����
            ���ô�����󣬽��ȴ��û���step1-4����ʾ�ĵ���е����ֱ���û������ŷ���
            ����APP�Դ˵�Ϊ���ģ����һЩ��־��ͼ�꣬��ԲȦ��ʮ�ֵȣ�����ʾ�û��Ըõ���е����

    ���ϣ��Ƕ�һ�������У׼�ı�׼���衣ͨ������£���ĻУ׼Ӧ�����ĸ�У׼�㣬����Ӧ����
    �ʾ��ηֲ����ĸ��㡣�����������ĸ������ν��������У׼�����������Ϳ��Ի�ȡ���㹻�����ݣ�
    �����ȷ�ļ��㡣�����������ĸ�����Ҫ�󣬵�һ���͵ڶ������x������ͬ���ڶ����͵�������y������ͬ��
    �������͵��ĸ���x������ͬ�����ĸ��͵�������y������ͬ,�����ͳ���һ������״̬
            adjust_step��˳��һ��Ҫ��ȷ���������л���У׼����˳��������1��2��3��4������������ֵ�ı仯��Ӧ

    ��800��480��ĻУ׼�򵥽̳�
        step1:
            ֪ͨ����׼��У׼TP��������ʼ����Ӧ�Ĺ���
            eLIBs_fioctrl(hdle, DRV_TP_CMD_ADJUST, 0, 0);
            ִ����Ϻ���������
        step2:
            my_pos.disp_xpos = 100;
            my_pos.disp_ypos = 100;
            eLIBs_fioctrl(hdle, DRV_TP_CMD_ADJUST, 1, (void *)&my_pos);    //�ȴ��û����������󷵻�
            //�û�����󷵻أ����ﲻ�жϳɹ���ʧ��
        step3:
            my_pos.disp_xpos = 700;
            my_pos.disp_ypos = 100;
            eLIBs_fioctrl(hdle, DRV_TP_CMD_ADJUST, 2, (void *)&my_pos);    //�ȴ��û����������󷵻�
            //�û�����󷵻أ����ﲻ�жϳɹ���ʧ��
        step4:
            my_pos.disp_xpos = 700;
            my_pos.disp_ypos = 400;
            eLIBs_fioctrl(hdle, DRV_TP_CMD_ADJUST, 3, (void *)&my_pos);    //�ȴ��û����������󷵻�
            //�û�����󷵻أ����ﲻ�жϳɹ���ʧ��
        step5:
            my_pos.disp_xpos = 100;
            my_pos.disp_ypos = 400;
            eLIBs_fioctrl(hdle, DRV_TP_CMD_ADJUST, 4, (void *)&my_pos);    //�ȴ��û������У׼�󷵻�
            //�����Ѿ�ȡ���ĸ����ݣ����������ж�����ʱ����Ч���ɹ���ʼУ׼���û���Ҫ�жϷ���ֵ������ɹ���
            //���˳�У׼�����ʧ�ܣ�Ӧ�÷���step2���¿�ʼ��ȡ����

*/

/*
�µ�5��У׼
    5��У׼����ԭ��4��Ļ��������ĸĽ�����Ҫ��Ϊ�˽���������ڰ�װ��ʱ������ĳЩԭ��û�а�װ��ȷ��������
    ��б�ȵ����⡣

    Tp����ĻУ׼commandֻ��һ������DRV_TP_CMD_ADJUST��Ҫ��������������Ҫ����ʹ�������������
    ��Ҫһ�����ݽṹ  __ev_tp_pos_t  my_pos;

    �������� eLIBs_fioctrl(hdle, DRV_TP_CMD_ADJUST, 0, (void *)&my_pos);

            �����ݽṹmy_pos��disp_xpos��disp_ypos��������Ļ����㣬�� aux ����дУ׼����
            ���ô�����󣬽��ȴ��û���step1-4����ʾ�ĵ���е����ֱ���û������ŷ���
            ����APP�Դ˵�Ϊ���ģ����һЩ��־��ͼ�꣬��ԲȦ��ʮ�ֵȣ�����ʾ�û��Ըõ���е����

    ���ϣ��Ƕ�һ�������У׼�ı�׼���衣ͨ������£���ĻУ׼Ӧ�������У׼�㣬����Ӧ����
    �ʾ��ηֲ����ĸ��㣬�Լ��������ĵ�һ���㡣������������������ν��������У׼�����������Ϳ��Ի�ȡ���㹻�����ݣ�
    �����ȷ�ļ��㡣�����������������Ҫ�󣬵�һ���͵ڶ������x������ͬ���ڶ����͵�������y������ͬ��
    �������͵��ĸ���x������ͬ�����ĸ��͵�������y������ͬ,�����ͳ���һ������״̬����������������Ļ���м䡣
            adjust_step��˳��һ��Ҫ��ȷ���������л���У׼����˳��������1��2��3��4��5������������ֵ�ı仯��Ӧ


�ر�˵����
    ��У׼�����е�ʱ������ĳЩԭ��ϣ����������У׼�����Ե������µĺ�����֪ͨ����������������ʵ���˳�������
    У׼���֣��ָ������ĵ㴥����
    eLIBs_fioctrl(hdle, DRV_TP_CMD_ADJUST, 6, (void *)&my_pos);
    ���������ϵĺ����Ժ������Ҫ���½���У׼����Ҫȫ����������

    ��800��480��ĻУ׼�򵥽̳�
        step1:
            ֪ͨ����׼��У׼TP��������ʼ����Ӧ�Ĺ���
            eLIBs_fioctrl(hdle, DRV_TP_CMD_ADJUST, 0, 0);
            ִ����Ϻ���������
        step2:
            my_pos.disp_xpos = 100;
            my_pos.disp_ypos = 100;
            eLIBs_fioctrl(hdle, DRV_TP_CMD_ADJUST, 1, (void *)&my_pos);    //�ȴ��û����������󷵻�
            //�û�������������жϵ�ǰ�ĵ���Ƿ���ȷ�������ȷ���سɹ������򷵻�ʧ��
            //Ӧ�ó���Ӧ�����յ�ʧ�ܵ�ʱ�����·�����������Լ�ͬ���Ĳ���
        step3:
            my_pos.disp_xpos = 700;
            my_pos.disp_ypos = 100;
            eLIBs_fioctrl(hdle, DRV_TP_CMD_ADJUST, 2, (void *)&my_pos);    //�ȴ��û����������󷵻�
            //�û�������������жϵ�ǰ�ĵ���Ƿ���ȷ�������ȷ���سɹ������򷵻�ʧ��
            //Ӧ�ó���Ӧ�����յ�ʧ�ܵ�ʱ�����·�����������Լ�ͬ���Ĳ���
        step4:
            my_pos.disp_xpos = 700;
            my_pos.disp_ypos = 400;
            eLIBs_fioctrl(hdle, DRV_TP_CMD_ADJUST, 3, (void *)&my_pos);    //�ȴ��û����������󷵻�
            //�û�������������жϵ�ǰ�ĵ���Ƿ���ȷ�������ȷ���سɹ������򷵻�ʧ��
            //Ӧ�ó���Ӧ�����յ�ʧ�ܵ�ʱ�����·�����������Լ�ͬ���Ĳ���
        step5:
            my_pos.disp_xpos = 100;
            my_pos.disp_ypos = 400;
            eLIBs_fioctrl(hdle, DRV_TP_CMD_ADJUST, 4, (void *)&my_pos);    //�ȴ��û������У׼�󷵻�
            //�û�������������жϵ�ǰ�ĵ���Ƿ���ȷ�������ȷ���سɹ������򷵻�ʧ��
            //Ӧ�ó���Ӧ�����յ�ʧ�ܵ�ʱ�����·�����������Լ�ͬ���Ĳ���
        step5:
            my_pos.disp_xpos = 400;
            my_pos.disp_ypos = 250;
            eLIBs_fioctrl(hdle, DRV_TP_CMD_ADJUST, 4, (void *)&my_pos);    //�ȴ��û������У׼�󷵻�
            //�û�������������жϵ�ǰ�ĵ���Ƿ���ȷ�����ʧ�ܷ���ʧ��
            //Ӧ�ó���Ӧ�����յ�ʧ�ܵ�ʱ�����·�����������Լ�ͬ���Ĳ���
            //�����Ѿ�ȡ��������ݣ����������ж�����ʱ����Ч���ɹ���ʼУ׼�����򷵻�-2.�û���Ҫ�жϷ���ֵ������ɹ���
            //���˳�У׼�������-2��Ӧ�÷���step2���¿�ʼ��ȡ����

*/

/*
�ر�˵����
    ��У׼�����е�ʱ������ĳЩԭ��ϣ����������У׼�����Ե������µĺ�����֪ͨ����������������ʵ���˳�������
    У׼���֣��ָ������ĵ㴥����
    eLIBs_fioctrl(hdle, DRV_TP_CMD_ADJUST, 6, (void *)&my_pos);
    ���������ϵĺ����Ժ������Ҫ���½���У׼����Ҫȫ����������

*/
typedef enum __DRV_TOUCHPANEL_CMD
{
    DRV_TP_CMD_NULL       =0,
    DRV_TP_CMD_REG          ,          //���ػص����������ǰ����ڻص����������Զ��滻ǰһ���ص�����
    DRV_TP_CMD_UNREG        ,          //ж�ػص�������ж�غ����ǰ����ڻص����������Զ�ʹ��ǰһ���ص�����
    DRV_TP_CMD_ADJUST       ,          //��ĻУ׼����μ�����˵��
    DRV_TP_CMD_SET_OFFSET_INFO,        //��������ƫ�x����y���򳬹����ֵ��Ϊ��һ��move  aux = ƫ��ֵ��Ĭ����ǰ��ĺ� TP_DEFAULT_OFFSET
    DRV_TP_CMD_GET_OFFSET_INFO,        //��ȡ����ƫ��
    DRV_TP_CMD_SET_MSG_PERTIME,        //���÷��ͼ�����������ٴη���һ����Ϣ��aux = ���ͼ����Ĭ����ǰ��ĺ� TP_DEFAULT_MSG_PERTIME
    DRV_TP_CMD_GET_MSG_PERTIME,        //��ȡ���ͼ��
    DRV_TP_CMD_SET_WORKMODE   ,        //���ù���ģʽ��������ͨģʽ���Լ�����ģʽ aux = {_drv_tp_workmode_t}���μ��·����ݽṹ��˵��
    DRV_TP_CMD_

}__drv_touchpanel_cmd_t;


typedef enum _DRV_TP_WORKMODE
{
    DRV_TP_MODE_NORMAL          ,
    DRV_TP_MODE_FAST            ,
    DRV_TP_MODE_DUAL            ,
    DRV_TP_MODE_XXX             ,
}
_drv_tp_workmode_t;
/*
    ���ʽ���  ����ͨģʽ������ģʽ, ����ģʽ

    ʹ�������ĳ��ϣ���Ϊ�˽��Ӧ�ó����Ĳ�ͬ
    ����ģʽ:
    ��ͨģʽ�������û�ͨ���Ĳ�����
    ����ģʽ�����������뷨��ʶ��
    ����ģʽ:
    �����ڷŴ���С��Ӧ�ó���������ͼƬ�ķŴ����С����ͼ�ķŴ����С�ȡ�

*/

#endif  /*_DRV_TOUCHPANEL_H_*/
