/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                               hello world sample
*
*                                (c) Copyright 2010-2012, Steven.ZGJ China
*                                            All    Rights Reserved
*
* File    : drv_twi.h
* By      : Jerry & victor
* Version : V2.00
*********************************************************************************************************
*/
#ifndef  _DRV_TWI_H_
#define  _DRV_TWI_H_

#include "typedef.h"
#include "cfgs.h"

#define      TWI_SLAVE_ADDRESS_07BIT           (0)
#define      TWI_SLAVE_ADDRESS_10BIT           (1)

typedef  enum __TWI_CMD_SET
{
	TWI_WRITE_SPEC_RS  = 0,  //д�淶��֧�ֱ�׼��twiд����restart
	TWI_READ_SPEC_RS      ,  //���淶��֧�ֱ�׼��twi������restart
	TWI_READ_EX_NO_RS      ,  //�µ�twi�����������restart
	TWI_READ_EX_STP_RS    ,  //�µ�twi�������stop֮����restart
	
    TWI_SET_SCL_CLOCK     ,  //����twi��sclʱ�ӣ����͵�Ϊ100khz��400khz����100000 ��400000 ��λΪHZ
    
}__twi_cmd_set_t;


typedef struct __TWI_DEV_PARA_EX
{
    __u16    slave_addr;             //���豸��ַ
    __u16    slave_addr_flag;        //�����õ�ַ���  0��7bit�� 1��10bit
	__u8    *byte_addr;              //��Ҫ��д�������ڴ��豸�еĵ�ַ,���ֽڴ�ŵ͵�ַ�����ֽڴ�Ÿߵ�ַ
	__u16    byte_addr_width;        //�����õ�ַ���  0��1��8 bit; ���������ִ����ֽ���
	__u16    byte_count;             //һ�ζ�дҪ��ɵ��ֽ���
	__u32    reserved;               //����λ
	__u8    *data;                   //���ݵĵ�ַ
}
__twi_dev_para_ex_t;

/**********************************************************
*   TWI ʹ��˵����    ���¼����°汾Ӳ��
*
*   ES_FILE   *ftwi;
*   __twi_dev_para_t        stwi;    // ��Ҫ���Ĳ���
*   __u8                 data[5];    // ���ݵ�buffer
*   __u8            byte_addr[2];    // ���豸�ļĴ�����ַ�����ֽڴ�ŵ͵�ַ�����ֽڴ�Ÿߵ�ַ
*
*
*   1.  ��twi�豸��
*	AW1620����3·TWI���û�Ӧ��֪���豸������һ·TWI������,��û�м����TWI·�������
*       ftwi = eLIBs_fopen("b:\\BUS\\TWI0", "r");//��0·twi
*	ftwi = eLIBs_fopen("b:\\BUS\\TWI1", "r");//��1·twi
*	ftwi = eLIBs_fopen("b:\\BUS\\TWI2", "r");//��2·twi
*	
*   2.  ������ݽṹstwi
*       �磺  stwi.slave_addr       =  0x10;     // ���豸�еĵ�ַ
*             stwi.slave_addr_flag  =  0   ;     // 7 bit address  
*             stwi.byte_addr        = byte_addr; //���豸�ļĴ�����ַ
*             stwi.byte_addr_width  =  2;        // ������һ�����������
*             stwi.byte_count      =   5;        //Ҫ���ͻ���յ����ݴ�С
*             stwi.data             = data;      // ����buffer�ĵ�ַ
*
*   3.  ͨ��iic��ȡ����
*           eLIBs_fioctrl(ftwi, TWI_READ_SPEC_RS,  0, (void *)&stwi);
*   4.  ͨ��iicд������
*           eLIBs_fioctrl(ftwi, TWI_WRITE_SPEC_RS, 0, (void *)&stwi);
*
*
*   5.  ������������twi���豸����ָ����Ҫָ���������豸�еļĴ�����ַ�ģ�����Ҫ���ݽṹ��byte_addr��ֵ��
*       ��byte_addr�ĵ�ַ��ֵΪ0x0,  ��stwi.byte_addr = 0x0�� ��byte_addr_widthҲ����Ϊ0�������Ĳ���Ҫ�仯��
*       ����Ӧ��: �ܶ�iic�豸��ֱ�Ӷ�ȡ��ʱ����Բ�����ַ,�������ʳ����ľ������һ���趨�ĵ�ַ������,
*                 ��Ҫ���ʹ̶�����ĵ�ַ��ʱ����ã����������ķ���Ҳ�����ô��Ĵ����ķ�ʽ����ֻ�Ƕ෢�˵�ַ�ֽڶ��ѡ�
*                 ����һ�㽨��д�ϵ�ַ�ֽڡ�  
*
*   6.  ʹ����Ϻ���Ҫ�ر� twi�豸��
*           eLIBs_fclose(ftwi);
*
*       ����˵��:
*           stwi.byte_count = �ֽ������� �豸�У��������ڵĵ�ַ���ֽڸ�����
*           ͨ��һ���豸�еĵ�ַ����0��0xff֮�䣬�������豸�У���ַ��������0��0xffffff֮��
*           ��ʱ������Ҫָ���ֽ�����
*               ����� 0x65431,�� stwi.baddr_count = 3;  __u8 tmp[3];tmp[0] = 0x31;tmp[1]=0x54;tmp[2]=0x06
*               ����� 0x4321, �� stwi.baddr_count = 2;  __u8 tmp[2];tmp[0] = 0x21;tmp[1]=0x43;
*               ����� 0x21,   �� stwi.baddr_count = 1;  __u8 tmp; tmp =0x21;
* 
*
***********************************************************/

#endif   /* _DRV_TWI_H_ */
