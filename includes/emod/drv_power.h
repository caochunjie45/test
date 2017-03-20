/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              eBIOS Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : power bios module
* File    : drv_power.h
*
* By      : Jerry
* Version : v1.00
* Date    : 2008-8-19 18:38:16
**********************************************************************************************************************
*/
#ifndef _DRV_POWER_H_
#define _DRV_POWER_H_

#define POWER_BOARD1_DEV_ADDR_20    (0x68>>1)
#define POWER_BOARD1_DEV_ADDR_19    (0x68>>1)
#define POWER_BOARD1_DEV_ADDR_18    (0x2C>>1)

#define DRV_POWER_BAT_CAP           2200
#define POWER_RDC_COUNT             10
#define DISCHARGE_CURRENT_RATE      35          //�ŵ�ʱ�����ⲿ������MOSFET
#define EXT_PMOS                    115         //������ⲿPMOSFET���˴�����Ϊ100
#define CHG_RDC_RATE                100

#define MUSIC_CURRENT_LCD_OFF       50          //ʵ�ʲ��Թ���������ֲ��ŵ�����20100222 Add

#define CHECK_TP_VOL 0

typedef struct __DRV_POWER_PARA_T
{
    __u8    byte_addr;
    __u8    reserved1;
    __u16   reserved2;
    __u8    *data;

}__drv_power_para_t;


/*
    ��ѹ��ȡ
    ��ѹ����
            aux �� ��ѹͨ����
            ��Ҫ���õĵ�ѹʹ�ú�����λ��ʹ��(void *)V��V������Ҫ���õĵ�ѹ
            ��ȡ�ĵ�ѹ��ֱֵ�ӷ��أ��ɹ����ص�ѹ��ֵ��ʧ�ܷ���-1
    ��ѹ״̬
            aux == ��ѹͨ����
            ʹ��һ��32λ����״̬��Ϣ��ֻ�п�����Ϣ��ֱ�������λ��0��1��ʾ
                                      ����ģʽ��Ϣ�ģ���DCDC1���õ�0λ��ʾ���͹أ�
                                      ��һλ��ʾ   0���Զ�ģʽ��1����bit2����
                                      ��2λ��ʾ    0��PWMģʽ�� 1��PFMģʽ
                                      û�п�����Ϣ�ĵ�һλ�����ģ�����λ���岻��
*/

/*
    ��������״̬��ȡ
    ��������״̬����
            aux = ����������
            ���ò�������ʹ��(void *)para�ķ�ʽ��para����Ҫ���ݵĲ���
            ��ȡ��ʱ��ʹ�÷���ֵ�����ʧ�ܷ���-1
            ������0��1λ��ʾ���͹�       0���رգ�   1����
            ������2��3λ��ʾN����P       0��N�Ϳ��أ�1��P�Ϳ���
            ���õ�ʱ�������Ӧbit����2�����ʾ��λ������
*/

/*
****************************************************************************************
*                                   ���ڵ��״̬
*
*   ��״̬1�йص������� DRV_POWER_CMD_BATADC_GET, DRV_POWER_CMD_BATADC_SET
*
*   ����ṹ���������õ�ؼ������״̬���Լ���ȡ��ǰ��صĵ�ѹ�͵���
*   ʹ�õ�ʱ������ʹ������ DRV_POWER_CMD_BATADC_SET
*��ʹ��DRV_POWER_CMD_BATADC_GET��������Ч������ɸ���
*****************************************************************************************
*/
typedef struct __DRV_POWER_BATTERY_STATUS1
{
    __u16   battery_exist       :2;     //��ش���״̬       0:������,    1����ش���
    __u16   bat_voladc_enable   :2;     //��ص�ѹ���ʹ��   0:��ֹ���,  1��������
    __u16   bat_crntadc_enable  :2;     //��ص������ʹ��   0:��ֹ���,  1��������
    __u16   bat_status          :2;     //��ص�ǰ����״̬   0:���,      1���ŵ�
    __u16   reserved            :8;
    __u16   adc_freq;                   //��ؼ��Ƶ��
    __u16   bat_vol;                    //��ǰ��ص�ѹ����λmV
    __u16   bat_crnt;                   //��ǰ��ص�������λmA

}__drv_power_battery_status_t1;


typedef struct __DRV_POWER_BATTERY_STATUS2
{
    __u8    dcin_valid;                 //�ⲿ��Դ�Ƿ���ڣ�1-���ڣ�0-������
    __u8    charge_status;              //���״̬��1-���ڳ�磻0-���ڳ��
    __u8    battery_exist;              //����Ƿ���ڣ�1-���ڣ�0-������
    __u8    battery_status;             //0: ���  1���ŵ�
    __u16   rest_vol;                   //ʣ���ص������ٷֱ�
    __u16   rest_time;                  //ʣ����ʱ�����ʣ����ʱ��
    __u16   bat_vol;					//���ʵʱ��ѹֵ����λ-��
    __u16   bat_crnt;					//���ʵʱ��ŵ����ֵ����λ-����
    __u16   bat_cap;					//���ʵ����������λ-����ʱ
    __u16   bat_rdc;					//���ͨ·ֱ���迹-��ŷ
    __u8    dcin_not_enough;            //��ʾ DCIN ������������
    __u16   bat_vol_correct;			//���У�����ѹ����λ-��

}__drv_power_battery_status_t2;


typedef struct _DRV_POWER_CHARGE_STATUS
{
    __u8    acin_valid;
    __u8    vbus_valid;
    __u8    ext_power_valid;
    __u8    bat_current_direction;      //��ص�������,0-�ŵ�,1-���
    __u8    in_Short;                   //1-short
    __u8    int_over_temp;
    __u8    charge_indication;
    __u8    battery_exist;
    __u8    battery_active;
    __u8    low_charge_current;
    __u16   battery_current;
    __u16   battery_vol;
    __u16   acin_vol;
    __u16   vbus_vol;
    __u8    dcin_not_enough;
    __u16   Ichgset;
    __u16   Iendchg;

}__drv_power_charge_stat;


/*
****************************************************************************************
*                                   ���ڵ��״̬
*
*   ��״̬1�йص������� DRV_POWER_CMD_GET_DCIN
*   ֻ�ܻ�ȡ״̬����������
*
*****************************************************************************************
*/
typedef struct __DRV_POWER_BATTERY_STATUS3
{
    __u8    dcin_valid;                 //0:������,    1������
    __u8    charge_status;              //0:û�г��,  1�����
    __u8    acin_en;                    //0:û��,      1:��

}__drv_power_battery_status_t3;


typedef struct __DRV_POWER_FUELGUAGE    //��ӦPOWER���DRV_POWER_CMD_GET_FUELGUAGE
{
    __u32   charge_status;              //��ǰ״̬��bit2:DCIN�Ƿ���ڣ�  bit1:��ش��ڣ�  bit0: ���״̬��1Ϊ��磬0Ϊû�г��
                                        //����״̬�����屣��
    __u32   rest_vol;                   //ʣ��������ٷֱ�
    __u32   rest_time;                  //ʣ��ʱ�䣬������
    __u32   bat_cap;                    //�����������λ-����ʱ
    __u32   bat_power;                  //���˲̬���ʣ���λ-����
    __u32   rest_time_music;
    __u32   rest_time_video;
    __u32   rest_time_pic;              //ʣ��ʱ�䣬������
    __u32   rest_time_book;             //ʣ��ʱ�䣬������
    __u32   rest_time_standby;          //ʣ��ʱ�䣬������
    __s32   is_low_power;               //����Ƿ�͵� 
    __s32   is_full_charge;             //����Ƿ����

}__drv_power_fuelguage_t;

typedef struct __DRV_POWER_DCIN_STATUS
{
    __u16   dcin_exist          :2;//DCIN�Ƿ����
    __u16   dcin_voladc_enable  :2;//DCIN��ѹADC�Ƿ�ʹ�ܣ�
    __u16   dcin_crntadc_enable :2;//DCIN����ADC�Ƿ�ʹ�ܣ�
    __u16   reserved            :10;
    __u16   adc_freq;               //ADC����Ƶ�ʣ�
    __u16   dcin_vol;               //DCIN ʵʱ��ѹֵ����λ-��
    __u16   dcin_crnt;              //DCINʵʱ����ֵ����λ-����
}__drv_power_dcin_status_t;

/*
*********************************************************************************************
*                                   ��ѹ�������
*   �ͳ����ص�������������DRV_POWER_CMD_VOUT_GET, DRV_POWER_CMD_VOUT_SET
*
*   һ����׼�������ʽ��  eLIBs_fioctrl(hdle, DRV_POWER_CMD_VOUT_GET, aux, pbuffer);
*   hdle�Ǿ��
*   aux�ǲ�����ָ����Դ����,�����ɺ궨�����
*   pbuffer��һ��ָ��,ͨ�����ڴ���һ�����ݡ�
*   ����DRV_POWER_CMD_VOUT_GET���ʹ�÷���ֵ��ʾ��ѹ��mV��λ�����ʧ�ܽ�����-1��ʹ�õ�ʱ����Ҫע��
*
*   ����DRV_POWER_CMD_VOUT_SET
*   �����ʽ  eLIBs_fioctrl(hdle, DRV_POWER_CMD_VOUT_SET, aux, pbuffer);
*   hdle�Ǿ��
*   aux�ǲ�����ָ����Դ���ƣ����ɺ궨�����
*   pbuffer��һ��ָ�룬ͨ�����ڴ���һ������
*   ����DRV_POWER_CMD_VOUT_SET���ʹ��pbufferָ�봫��һ����ѹ���ݣ�mV��λ
*   ���óɹ�����0������ʧ�ܣ��磺��ѹ���ɴ��Դ���Ʋ����ڣ�����-1
*
**********************************************************************************************
*/

/*
*********************************************************************************************
*                                   ����������
*   �͵�ѹ������ص�������������DRV_POWER_CMD_CHARGE_GET, DRV_POWER_CMD_CHARGE_SET
*   ���Ҫ�������ã�ͨ���Ƚ��ж�ȡ������ĳЩ�����ĳ�ʼֵ���������Ӱ��
*
*   �������õ������½ṹ�������
**********************************************************************************************
*/
typedef struct __DRV_POWER_CHARGE_STATUS
{
    __u8    battery_status;                   //00: ��ز�����
                                              //01����ش��ڣ������磬���ڳ��
                                              //02����ش��ڣ������磬���ڳ��
                                              //03����ش��ڣ���ֹ���
    __u8    charge_end;                       //���Ŀ�����ã� 0��10������   1��15������
    __u8    charge_autolimit;                 //����Զ�����   0: ��ֹ       1: ʹ��
    __u8    charge_enable;                    //���ʹ��       0: ��ֹ���   1��ʹ�ܳ��
    __u16   charge_vtarget;                   //���Ŀ���ѹ, mV��λ
    __u16   charge_current;                   //����������, mA��λ
    __u16   charge_timer1;                    //����ʱ��1����
    __u16   charge_timer2;                    //����ʱ��2����

}__drv_power_charge_status_t;


/*
*********************************************************************************************
*                                   ��·����״̬��ȡ
*   �͵�ѹ������ص�����ֻ��һ����DRV_POWER_CMD_BASIC_STATUS
*
*   ���Ի�ȡ�������½ṹ�������
**********************************************************************************************
*/
typedef struct __DRV_POWER_BASIC_STATUS
{
    __u32   acin_input          :1;     //ACINEN״̬            0��û��AC adapter���룻1����AC adapter����

    __u32   charge_source       :2;     //00:û�г��  01��Բ�ڻ�ţ���  10��USB�߳��  11��USB��ţ���
    __u32   vout_exist          :1;     //ָʾ�ⲿ��Դ�Ƿ����  0���ⲿ��Դû�н��룻  1���ⲿ��Դ����
    __u32   vout_enable         :1;     //ָʾ�ⲿ��Դ�Ƿ����  0���ⲿ��Դ�����ã�    1���ⲿ��Դ����
    __u32   vout_choose         :1;     //DCIN����ʱ�Ƿ�ѡ��DCIN��Ϊϵͳ��Դ 0����ѡ��1��ѡ��

    __u32   bat_exist           :1;     //����Ƿ����          0��û�е�����ӵ�AXP180��1���е�����ӵ�AXP180
    __u32   bat_work_status     :1;     //��ص�������ָʾ��    0����س�磻1����ع���
    __u32   bat_charge_status   :1;     //���״ָ̬ʾ          0���ڳ�磻1�����ڳ��

}__drv_power_basic_status_t;


/*
****************************************************************************************
*                                   ���ڿ�����״̬�����Լ���ȡ
*
*   �Ϳ�����״̬�йص������� DRV_POWER_CMD_IPS_GET, DRV_POWER_CMD_IPS_SET
*
*   ����ṹ���������õ�ؼ������״̬���Լ���ȡ��ǰ��صĵ�ѹ�͵���
*   ʹ�õ�ʱ������ʹ������ DRV_POWER_CMD_IPS_SET
*��ʹ��DRV_POWER_CMD_IPS_GET��������Ч������ɸ���
*****************************************************************************************
*/
typedef struct __DRV_POWER_IPS_STATUS
{
    __u16   dcin_choose     :2;         //DCIN����ʱ�Ƿ�ѡ��DCIN��Ϊϵͳ��Դ, 0����ѡ��1��ѡ��
    __u16   vhold_mode      :2;         //��ѹģʽ����λ                      0������ѹ��1����ѹ
    __u16   reserved1       :4;         //
    __u16   vhold;                      //����VHold��ѹ
    __u16   vwarning;                   //����VWarning��ѹ
    __u16   voff;                       //����VOff��ѹ           ��3����ѹ�ĵ�λ����mV

}__drv_power_ips_status_t;


/*
****************************************************************************************
*                                   ���ڿ��ػ�ʱ�������Լ���ȡ
*
*   �Ϳ�����״̬�йص������� DRV_POWER_CMD_ONOFFTIME_GET, DRV_POWER_CMD_ONOFFTIME_SET
*
*   ����ṹ���������ÿ�����ʱ�䣬�ػ���ʱ��
*
*   ����ʱ�䣺�ϵ�����£����°���������һ��ʱ��󿪻������ʱ����ǿ���ʱ��
*   �ػ�ʱ�䣺ͨ������£����°���������һ��ʱ��󽫴����жϹػ������ʱ����ǹػ�ʱ��
*
*****************************************************************************************
*/
typedef struct  __DRV_POWER_ONOFFTIME
{
    __u16   on_time;                //����ʱ�䣬128ms�� 512ms�� 1000ms�� 2000ms
    __s16   on_status;              //��������״̬�����ʱ�����óɹ�������д0������-1
    __u16   off_time;               //�ػ�ʱ�䣬1000ms, 1500ms, 2000ms,  2500ms
    __s16   off_status;             //�ػ�����״̬�����ʱ�����óɹ�������д0������-1

}__drv_power_onofftime_t;

/*
****************************************************************************************
*                                   ����PWM����
*
*   �Ϳ�����״̬�йص�������  DRV_POWER_CMD_PWM_SET
*
*   ����ṹ����������PWMƵ�ʺ�ռ�ձȣ�Ƶ�ʵ�λΪHz��ռ�ձ�Ϊ1%
*
*   PWMƵ������ܵ�35Hz����ߵ�1MHz����1MHzʱ��ռ�ձ�ֻ��50%������
*  PWMƵ�ʵ���17.576KHz��ռ�ձȷֱ��ʲ��ܴﵽ1%����
*
*****************************************************************************************
*/
typedef struct  __DRV_POWER_PWM
{
    __u32   frequency;                //PWM��Ƶ�ʣ���λΪHz
    __u16   duty;                     //PWM��ռ�ձȵ�λΪ1%

}__drv_power_pwm_t;


#define POWER_EPT_1                 0x1f00  //AXP18x,��Ӧ PIN-41��DRV1-CDOUTPL��AXP19x����ӦGPIO0
#define POWER_EPT_2                 0x1f01	//AXP18x,��Ӧ PIN-42��DRV2-CDOUTNL��AXP19x����ӦGPIO1
#define POWER_EPT_3                 0x1f02	//AXP18x,��Ӧ PIN-45��DRV3-CDOUTPR��AXP19x����ӦGPIO2
#define POWER_EPT_4                 0x1f03	//AXP18x,��Ӧ PIN-46��DRV4-CDOUTNR��AXP19x����ӦGPIO3
#define POWER_EPT_5                 0x1f04	//��ӦAXP19x��GPIO4
#define POWER_EPT_6                 0x1f05	//��ӦAXP19x��GPIO5��N_RSTO��
#define POWER_EPT_7                 0x1f06	//��ӦAXP19x��GPIO6��PWREN��
#define POWER_EPT_8                 0x1f07	//��ӦAXP19x��GPIO7��SYSEN��

#define POWER_EPT_MOS_N             (0x00 << 2)//��ӦAXP18x IO���ͣ�AXP19x IO����
#define POWER_EPT_MOS_P             (0x01 << 2)//��ӦAXP18x IO���ߣ�AXP19x IO����
#define POWER_EPT_MOS_NOTCARE       (0x02 << 2)

#define POWER_EPT_GATE_OFF          (0x00 << 0)//����AXP18x���ر��������ߡ�����
#define POWER_EPT_GATE_ON           (0x01 << 0)//����AXP18x���򿪲������ߡ�����
#define POWER_EPT_GATE_NOTCARE      (0x02 << 0)

#define POWER_VOL_DCDC1             0x0f00
#define POWER_VOL_DCDC2             0x0f01
#define POWER_VOL_DCDC3             0x0f02
#define POWER_VOL_LDO1              0x0f03
#define POWER_VOL_LDO2              0x0f04
#define POWER_VOL_LDO3              0x0f05
#define POWER_VOL_LDO4              0x0f06
#define POWER_VOL_GPIO0             0x0f0a
#define POWER_VOL_EXTEN             0x0f0b

#define POWER_VOL_LDO5              0x0f07
#define POWER_VOL_SW1               0x0f08
#define POWER_VOL_SW2               0x0f09

#define POWER_VOL_ON                0x01				//�������ѹ
#define POWER_VOL_OFF               0x00        //�ر������ѹ

#define POWER_OPEN                	0x01        // GPIO1 ����ߵ�ƽ���ߴ��ⲿ��� ��ӦDRV_POWRE_CMD_BL_CTRL��DRV_POWER_CMD_EXTCHG_CTRL
#define POWER_CLOSE               	0x00        // GPIO1 ����͵�ƽ���߹ر��ⲿ��� ��ӦDRV_POWRE_CMD_BL_CTRL��DRV_POWER_CMD_EXTCHG_CTRL

#define POWER_VBUS_CUR_900          0x01        //VBUS ��������900mA
#define POWER_VBUS_CUR_500          0x02        //VBUS ��������500mA
#define POWER_VBUS_CUR_100          0x03        //VBUS ��������100mA
#define POWER_VBUS_CUR_NOT_LIMIT    0x00        //VBUS ��������

#define POWER_INT_VOUT_OP           (1<<7)      //IRQ1�� �ⲿ��Դ��ѹ�����λд1��DCIN��ѹ�ָ���������Χ�������λ
#define POWER_INT_VOUT_PLUGIN       (1<<6)      //IRQ2�� �ⲿ��Դ���룬���λд1��DCIN�ɸߵ�����ʧ�������λ
#define POWER_INT_VOUT_PLUGOUT      (1<<5)      //IRQ3�� �ⲿ��Դ�Ƴ������λд1��DCIN�ɵ͵��߳��ֻ������λ
#define POWER_INT_VOOUT_L_VHOLD     (1<<4)      //IRQ4�� �ⲿ��Դ��ѹ����VHOLD�����λд1��DCIN�ɸߵ�����ʧ�������λ
#define POWER_INT_BAT_TMP_HIGH      (1<<2)      //IRQ5�� ����¶ȹ��ߣ����λд1�����¶Ȼָ���������Χ�������λ
#define POWER_INT_BAT_TMP_LOW       (1<<1)      //IRQ6�� ����¶ȹ��ͣ����λд1�����¶Ȼָ���������Χ�������λ
#define POWER_INT_VDCDC1_LOW        (1<<16)     //IRQ7�� DC-DC1�����ѹ���ͣ����λд1�������ѹ�ָ���������Χ�������λ
#define POWER_INT_VDCDC2_LOW        (1<<15)     //IRQ8�� DC-DC2�����ѹ���ͣ����λд1�������ѹ�ָ���������Χ�������λ
#define POWER_INT_VDCDC3_LOW        (1<<14)     //IRQ9,  DC-DC3�����ѹ���ͣ����λд1�������ѹ�ָ���������Χ�������λ
#define POWER_INT_VLDO3_LOW         (1<<13)     //IRQ10��LDO3�����ѹ���ͣ����λд1�������ѹ�ָ���������Χ�������λ
#define POWER_INT_PEK_SHORT         (1<<12)     //IRQ11��PEK�̰������λд1�������λ
#define POWER_INT_PEK_LONG          (1<<11)     //IRQ12��PEK���������λд1�������λ
#define POWER_INT_BAT_IN            (1<<24)     //IRQ13����ؽ��룬���λд1���ر��γ��������λ
#define POWER_INT_BAT_OUT           (1<<23)     //IRQ14������Ƴ������λд1���ر�����������λ
#define POWER_INT_BAT_ACTIVE_IN     (1<<22)     //IRQ15, �����ؼ���ģʽ�����λд1���˳���ؼ���ģʽ�������λ
#define POWER_INT_BAT_ACTIVE_OUT    (1<<21)     //IRQ16���˳���ؼ���ģʽ�����λд1������ؼ���ģʽ�������λ
#define POWER_INT_CHARGE_START      (1<<20)     //IRQ17����ʼ��磬���λд1��ֹͣ���������λ
#define POWER_INT_CHARGE_END        (1<<19)     //IRQ18�������ɣ����λд1�������¿�ʼ�������λ
#define POWER_INT_LOWPWR_WARN       (1<<18)     //IRQ19���͵羯�棬���λд1��AIPS��ѹ�ָ���������Χ�������λ
#define POWERON_TIME_128MS          (0<<6)      //����PowerON��������ʱ��Ϊ128ms
#define POWERON_TIME_512MS          (1<<6)	    //����PowerON��������ʱ��Ϊ512ms����3s
#define POWERON_TIME_1S             (2<<6)	    //����PowerON��������ʱ��Ϊ1s
#define POWERON_TIME_2S             (3<<6)	    //����PowerON��������ʱ��Ϊ2s
#define LONGKEY_TIME_1000MS			(0<<4)      //���ó�����ʱ��Ϊ1s
#define LONGKEY_TIME_1500MS			(1<<4)	    //���ó�����ʱ��Ϊ1.5s
#define LONGKEY_TIME_2000MS			(2<<4)	    //���ó�����ʱ��Ϊ2s
#define LONGKEY_TIME_2500MS			(3<<4)	    //���ó�����ʱ��Ϊ2.5s
#define PWM1												0x01
#define PWM2												0x02

typedef enum __DRV_POWER_CMD_GROUP
{
    DRV_POWER_CMD_READ_REG     =      0xff00,   //������������Ҫͨ��
    DRV_POWER_CMD_WRITE_REG,                    //����д�������Ҫͨ��
    DRV_POWER_CMD_BASIC_STATUS,                 //P1����״̬����Ӧ__drv_power_basic_status_t
    DRV_POWER_CMD_POWER_ON,                     //�������յģ���Ч��
    DRV_POWER_CMD_POWER_OFF,                    //�ػ�
    DRV_POWER_CMD_INT_ENABLE,                   //�ж�ʹ��
    DRV_POWER_CMD_INT_DISABLE,                  //�жϽ�ֹ
    DRV_POWER_CMD_INT_QUERY,                    //��ѯ�ж�
    DRV_POWER_CMD_INT_CLEAR,                    //����ж�
    DRV_POWER_CMD_VOUT_GET,                     //�����ѹ��ȡ
    DRV_POWER_CMD_VOUT_SET,                     //�����ѹ����
    DRV_POWER_CMD_VSTS_GET,                     //�����ѹ״̬��ȡ
    DRV_POWER_CMD_VSTS_SET,                     //�����ѹ״̬����
    DRV_POWER_CMD_PWREPT_GET,                   //IO״̬��ȡ
    DRV_POWER_CMD_PWREPT_SET,                   //IO״̬����
    DRV_POWER_CMD_BATADC_GET,                   //��ز���״̬��ȡ����Ӧ__drv_power_battery_status_t1
    DRV_POWER_CMD_BATADC_SET,                   //��ز����������ã���Ӧ__drv_power_battery_status_t1
    DRV_POWER_CMD_DCINADC_GET,                  //DCIN����״̬��ȡ����Ӧ__drv_power_dcin_status_t
    DRV_POWER_CMD_DCINADC_SET,                  //DCIN�����������ã���Ӧ__drv_power_dcin_status_t
    DRV_POWER_CMD_CHARGE_GET,                   //���״̬��ȡ����Ӧ__drv_power_charge_status_t
    DRV_POWER_CMD_CHARGE_SET,                   //���״̬���ã���Ӧ__drv_power_charge_status_t
    DRV_POWER_CMD_IPS_GET,                      //IPS���ݶ�ȡ����Ӧ__drv_power_ips_status_t
    DRV_POWER_CMD_IPS_SET,                      //IPS״̬���ã���Ӧ__drv_power_ips_status_t
    DRV_POWER_CMD_BATSTATUS_TABLE_GET,          //״̬����ȡ��ص�ѹ����ŵ��������س�ŵ�ʣ��ʱ�䣬��Ӧ__drv_power_battery_status_t2
    DRV_POWER_CMD_GET_BAT_RDC,                  //��ȡ���ͨ·ֱ���迹����λ��ŷ
    DRV_POWER_CMD_GET_DCIN,                     //��ȡDCINͨ·�Ƿ���Ч���Ƿ�Enable�����״̬����Ӧ__drv_power_battery_status_t3
    DRV_POWER_CMD_GET_FUELGUAGE,                //��ȡ��ص�����ʱ�䣬״̬,�¼�cmd����Ӧ__drv_power_fuelguage_t
                                                //aux = 0, pbuffer = __drv_power_fuelguage_t��������ָ�룬�μ���������˵��
    DRV_POWER_CMD_SET_USB_CHARGE,               //�ṩ��USB���ó��״̬ʹ��
    DRV_POWER_CMD_SET_STANDBY_FLAG,             // standby = 1, ��standby ״̬�ָ���
    DRV_POWER_CMD_ONOFFTIME_GET,                //��ȡAXP19x Timer��Ϣ��
    DRV_POWER_CMD_ONOFFTIME_SET,                //����AXP19x Timer��Ϣ��
    DRV_POWER_CMD_CHGSTAT_GET,                  //��ó��״̬����Ӧ __drv_power_charge_stat �ṹ��
    DRV_POWER_CMD_GET_COULOMB,                  //��ÿ��ؼ�ֵ����λmAh
    DRV_POWER_CMD_GET_PMU_TYPE,
    DRV_POWER_CMD_SET_POWERON_TIME,             //���ÿ���ʱ�� 128ms/512ms/1s/2s,��aux����
    DRV_POWER_CMD_SET_LONGKEY_TIME,             //���ó�����ʱ�� 1s/1.5s/2s/2.5s,��aux����
    DRV_POWRE_CMD_BL_CTRL,                      //�ⲿGPIO��ΪPWM���Ʊ��⿪�ؿ���  aux =POWER_OPENΪ�� aux =POWER_CLOSE Ϊ�ر�
    DRV_POWER_CMD_EXTCHG_CTRL,					//�ⲿ��翪�ؿ���  aux =POWER_OPENΪ�� aux =POWER_CLOSE Ϊ�ر�
    DRV_POWER_CMD_SET_VBUS_CURR,				//����USB�������� --100mA��500mA��900mA�����������ù��ܽ�AXP20x��AXP19x��Щ������pbuffer�д��ݲ���
    DRV_POWER_CMD_SET_EXTCHG_CURR,				//�����ⲿ����������λmA,ֻ������300mA��400mA��500mA��600mA��700mA��800mA��900mA��1000mA����pbuffer�д��ݲ���
    DRV_POWER_CMD_PWM_SET,						//����PWM��ͨ����PWM��Ƶ���Լ�PWM��ռ�ձȣ�����
	
	DRV_POWER_CMD_SET_LOW_POWER,               //���õ͵��־
	DRV_POWER_CMD_SET_81H,						//for unlock protect 20140324 hp
}__drv_power_cmd_group_t;


#endif /*_DRV_POWER_H_*/

