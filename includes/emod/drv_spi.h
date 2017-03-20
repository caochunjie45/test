/*
*********************************************************************************************************
*											        ePOS
*						           the Easy Portable/Player Operation System
*									           time sub-system
*
*                                    (c) Copyright 2007-2008, Jerry China
*                                          All Rights Reserved
*
*                                     module management headfile
*
* File   : drv_spi.h
* Version: V2.0
* By     : victor
*********************************************************************************************************
*/
#ifndef  _DRV_SPI_H_
#define  _DRV_SPI_H_


/* ˵��������master �� slaveģʽ
 * ֧��masterģʽ��
 * ��ʱ��֧��slaveģʽ,�û�ͨ��ioctl���ü��ͷš�    
*/



/**************************************************
*����work_mode����
* spi work mode    Leading Edge        Trailing Edge
*
*       0   :      Rising, Sample      Falling, Setup
*
*       1   :      Rising, Setup       Falling, Sample
*
*       2   :      Falling, Sample     Rising, Setup
*
*       3   :      Falling, Setup      Rising, Sample
*
****************************************************/
#define  DRV_SPI_WORK_MODE0       0
#define  DRV_SPI_WORK_MODE1       1
#define  DRV_SPI_WORK_MODE2       2
#define  DRV_SPI_WORK_MODE3       3


/* ����work_slot������ѡ���ض����豸,�û���֪���豸������һ��spi��� */
#define  DRV_SPI_SLOT_0   (0x01) /* cs0 */
#define  DRV_SPI_SLOT_1   (0x02) /* cs1 */
#define  DRV_SPI_SLOT_2   (0x04) /* cs2 */
#define  DRV_SPI_SLOT_3   (0x08) /* cs3 */


//������ʱ���û����������ݽṹ
typedef  struct  __SPI_DEV_SET_XFER
{
    void   *tx_addr;        /*���͵��׵�ַ,���û����NULL*/
    __u32   tx_count;       /*���������ֽ���*/
    
    void   *rx_addr;        /*���յ��׵�ַ,���û����NULL*/
    __u32   rx_count;       /*���������ֽ���*/     
    
    __u32   work_mode;      /*���ֹ���ģʽ֮һ��pha��pol����ϣ�����master��slaveģʽ��*/
    __u32   work_clk;       /*��λ��HZ������master����Ч��slaveģʽ�²���Ҫ�������������*/
    __u32   work_slot;      /* spi�豸����һ��cs��� */
    void    *reserved;      /*������slaveģʽ�¿����ص���*/

}__spi_dev_set_xfer_t;

typedef enum __SPI_DEV_CMD_SET
{
    SPI_DEV_CMD_NONE                = 0x01,   //������
    SPI_DEV_CMD_MASTER_RW                 ,   /*masterģʽ�µĶ�д�������� */
    SPI_DEV_CMD_SLAVE_READ                ,   /*slaveģʽ�£������������� */
    SPI_DEV_CMD_SLAVE_WRITE               ,   /*slaveģʽ�£������������� */
    SPI_DEV_CMD_REQUEST_SLAVE             ,   /* master->slaveģʽ���������� */
    SPI_DEV_CMD_RELEASE_SLAE              ,   /* slave->masterģʽ���������� */
    SPI_DEV_CMD_RESERVED,
}__spi_dev_cmd_set_t;




/*SPI��Ϊ�����豸ʱ����÷�˵��*/
/************************************************************************************
*
*   ����spi��Ϊmaster�豸��ʹ��:
*
*   1.  Ҫʹ��spi�豸������������һ���豸�ļ��������豸�ļ������hspi����ô
*              hspi = eLIBs_fopen("b:\\BUS\\SPI0", "r+");          
*       ������Ҫ�жϷ��سɹ�����ʧ��,�û���Ҫ֪���豸��������һ��spi�����ϡ�
*
*   2.  �õ��ļ��������Ҫ�����Լ�ʹ�õ���ز���������SPI����ģʽ������Ƶ�ʣ�Ƭѡ��ۣ�
*         ��Щ�����ݽṹ__spi_dev_set_xfer_t�и�ֵ��Ȼ��ʹ��SPI_DEV_CMD_MASTER_RW����������
*          ���������һ��__spi_dev_set_xfer_t���͵����ݽṹΪspi_xfer�����ù���Ƶ��
*       Ϊ10MHz������ģʽ��3��ʹ��Ƭѡ0�� ��ô
*                spi_xfer.work_slot = DRV_SPI_SLOT_0; //Ƭѡ0,�û���֪���豸������һ��Ƭѡ��
*                spi_xfer.work_mode = DRV_SPI_WORK_MODE3; //pha = 1��pol=1��
*                spi_xfer.work_clk  = 10000000;//10MHz��ÿ�ζ�д��������
*       ������Ҫ����д��Ϣ�������������
*             (1)���Ϊ����
*               __u32 user_buf[10]={0};
*               spi_xfer.rx_addr = user_buf;
*               spi_xfer.rx_count = 10; 
*               spi_xfer.tx_addr = NULL;
*               spi_xfer.tx_count = 0; 
*             (2)���Ϊд��
*               __u32 user_buf[10]={1};
*               spi_xfer.rx_addr = NULL;
*               spi_xfer.rx_count = 0;                 
*               spi_xfer.tx_addr = user_buf;
*               spi_xfer.tx_count = 10; 
*             (3)�����д�����(һ�����ڿ����豸ʱ��д����ٽ�������)
*               __u32 rx_buf[10]={0};
*               __u32 tx_buf[10]={1};
*               spi_xfer.rx_addr = rx_buf;
*               spi_xfer.rx_count = 10;                 
*               spi_xfer.tx_addr = tx_buf;
*               spi_xfer.tx_count = 10;               
*          ʹ������ĺ�����������
*                eLIBs_fioctrl(hspi, SPI_DEV_CMD_MASTER_RW, 0, (void *)&spi_xfer);
*
*    4.    ��Ҫ�ͷ�������豸�ļ����
*                eLIBs_fclose(hspi);
*
*    ������һ��������SPI�豸ʹ�ù���
*
**************************************************************************************/



#endif  /*_DRV_SPI_H_*/
