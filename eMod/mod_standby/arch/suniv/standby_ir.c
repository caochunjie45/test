/*
**********************************************************************************************************************
*                                                    ePOS
*                                   the Easy Portable/Player Operation System
*                                              standby sub-system
*
*                                      (c) Copyright 2009-2010, jerr China
*                                              All Rights Reserved
*
* File    : standby_ir.c
* By      : jerry
* Version : V1.00
**********************************************************************************************************************
*/
#include  "standby_ir.h"

extern __u32    ir_mask;
extern __u32    ir_power;



#if(ALLOW_IR_WAKEUP_STANDBY)


/* data for store sample air */
#define IR_DMA_LEN     75

static  __irs_reserve_status_t   irs_status;
static  __u8      ir_data[100];

/*
****************************************************************************************************
*
*  FunctionName:           ir_nec_data_decode
*
*  Description:
*            ���ݽ��յ�������֡���룬��Ҫ����NEC�Ľ��ղ��ν��н��룬�����������:
*               ����ǰ��:���ղ�����Ҫ��ת��
*               1.��8.7ms����NECǰ����
*               2.��3.4ms����normal code����repeat code
*               3.0.56msΪ�ߵ�ƽ,ע�����岻�ܳ������������ֵ
*               4.�͵�ƽ�ж�0����1�ı�׼Ϊ1.1275ms��
*
*  Parameters:
*       buf  :  ���յ�������buffer�׵�ַ
*       size :  buffer�ĳ���
*
*  Return value:
*           ������IR_ERROR_CODE���ɹ�����codeֵ��Ϊ0Ϊrepeat code ��
*  Notes:
*
****************************************************************************************************
*/
static __u32 ir_nec_data_decode(const __u8 *buf, const __u32 size)
{
    __u32 len    = 0;//low or high level time duration
    __u32 code   = 0;//decode result
    __s32 bitCnt = 0;// 32bit long  16bit user code+ 8bit data + 8bit invert data
    __u32 i      = 0;//counter for size
    __u8  val    = 0;//store sample data
    __u8  last   = 0;//low or high level flag

    if(buf == NULL)
    {
        return IR_ERROR_CODE;
    }
    if(size == 0)
    {
        return IR_ERROR_CODE;
    }

    /*step 1-����9ms*/
    len = 0;
    for( i = 0; i < size; i++)
    {
        val = buf[i];
        if(val & 0x80)//��9ms���ߵ�ƽ
        {
            len += val & 0x7f;//�ۼ�
        }
        else //�͵�ƽ��ʼ2.25ms����4.5ms
        {
            if(len > NEC_IR_FORMAT_LEADING)//ΪNEC��ʽ
            {
                break;
            }
            else
            {
                return IR_ERROR_CODE;//��NEC��ʽ��ֱ�ӷ���
            }
        }
    }

    /*step 2- ����4.5ms����2.25ms */
    len = 0;
    for(; i < size; i++)
    {
        val = buf[i];
        if(val & 0x80)//��ʼ�ҵ����ݸߵ�ƽ
        {
            if(len > NEC_IR_NORMAL_OR_REPEAT_CODE)
            {
                break;//normal code ����
            }
            len = 0;
        }
        else //��4.5ms��2.25ms ���͵�ƽ
        {
            len += val & 0x7f;//�ۼ�
        }
    }

    /*step3 -���û���/��ַ������ݽ���*/
    code   = 0;  /*0 for Repeat Code*/
    bitCnt = 0;
    last   = 1;
    len    = 0;
    for(; i < size; i++)
    {
        val = buf[i];
        if(last)//ͳ�Ƹߵ�ƽ0.56ms��
        {
            if(val & 0x80)
            {
                len += val & 0x7f;
            }
            else //�͵�ƽ����
            {
                if(len > NEC_IR_PMAX) /*Error Pulse*/
                {
                    return IR_ERROR_CODE;
                }
                last = 0;//ת���ж�0,1
                len = val & 0x7f;//��ʼ�Ե͵�ƽ�ۼ�
            }
        }
        else //ͳ�Ƶ͵�ƽʶ��0-1��
        {
            if(val & 0x80)//�����ߵ�ƽ��
            {
                if(len > NEC_IR_DMAX) /*Error Distant*///���뱣֤�͵�ƽ�ĳ���ʱ�䲻����4��
                {
                    return IR_ERROR_CODE;
                }
                else//�͵�ƽ С��4��0.56ms
                {
                    if(len > NEC_IR_ONE_OR_ZERO_BIT)  //����1��Ĭ������0
                    {
                        /*data '1'*/
                        code |= 1<<bitCnt;
                    }
                    bitCnt ++;
                    if(bitCnt == 32)
                    {
                        break;  /*decode over*/
                    }
                }
                last = 1;
                len = val & 0x7f;
            }
            else //ͳ�Ƶ͵�ƽ����ʱ�䡣
            {
                len += val & 0x7f;
            }
        }
    }
    return code;
}

/*
*********************************************************************************************************
*                           INIT IR FOR STANDBY
*
*Description: init ir for standby;
*
*Arguments  : none
*
*Return     : result;
*               EPDK_OK,    init ir successed;
*               EPDK_FAIL,  init ir failed;
*********************************************************************************************************
*/
__s32  standby_ir_init(void)
{
    __s32   i;
    __u32   reg_val;
    __u32   saddr, daddr;
	//standby_puts("standby_ir_init 0\n");

    irs_status.ir_int = STANDBY_RREG(IR_REG_RXINT);/* ����ir�����жϼĴ��� */
	//standby_puts("standby_ir_init 1\n");

    /* ��ROI_EN,RPEI_EN,RISI_EN,CRCI_EN,RAI_EN,DRQ_EN ��0���ر������жϣ�������fifo���䲻�� */
    reg_val = STANDBY_RREG(IR_REG_RXINT);
    reg_val &= ~IR_RXINT_ALL_MASK;
    reg_val |= IR_RXINT_DRQ_EN;/* ����DRQ�ж� */
    reg_val |= IR_RXINT_RPEI_EN;/* �������հ�����ж� */
   // reg_val |= IR_RXINT_RAI_EN; /* ����fifo�������ж� */
    STANDBY_WREG(IR_REG_RXINT, reg_val);
   //standby_puts("standby_ir_init 2\n");

    /* ��pendingλ��� */
    reg_val = STANDBY_RREG(IR_REG_RXSTA);
    reg_val |= IR_RXSTA_ALL_MASK;
    STANDBY_WREG(IR_REG_RXSTA, reg_val);
	//standby_puts("standby_ir_init 3\n");

    saddr = (__u32)(0x0fffffff & IR_REG_RXFIFO);/* ���ý��ռĴ���ΪԴ��ַ */
    daddr = (__u32)((__u32)ir_data & 0x000fffff);

    STANDBY_WREG(DMAC_REG_N0_CFG, 0xC160020/*0x0C110020*/);/* ����DMA, ˫8�ֽڣ���burst  */
	//standby_puts("standby_ir_init 4\n");
    STANDBY_WREG(DMAC_REG_N0_SRC_ADDR, saddr);/* Դ�����ַ */
	//standby_puts("standby_ir_init 5\n");
    STANDBY_WREG(DMAC_REG_N0_DST_ADDR, daddr);/* Ŀ�������ַ */
	//standby_puts("standby_ir_init 6\n");

    reg_val = STANDBY_RREG(0xf1c0210c/*DMAC_REG_N0_BYTE_CNT*/);
    reg_val &= 0x3ffff;
    reg_val |= IR_DMA_LEN;/* ��ʱ����ȡ100���ֽ����ݣ����ֵ��ʵ�ʵ�һ֡IR�����ֽ���Ҫ��һ��  */
    STANDBY_WREG(DMAC_REG_N0_BYTE_CNT, reg_val);
	//standby_puts("standby_ir_init 7\n");

    while(IR_RXSTA_RAC_MASK & STANDBY_RREG(IR_REG_RXSTA))
    {
        daddr =  STANDBY_RREG(IR_REG_RXFIFO);/* �������õ����� */
    }
	//standby_puts("standby_ir_init 8\n");

    //delay 1 second for ir init
    for(i=0; i<1*1000*1000; i++);
    while(IR_RXSTA_RAC_MASK & STANDBY_RREG(IR_REG_RXSTA))
    {
        daddr = STANDBY_RREG(IR_REG_RXFIFO);/* �������õ����� */
    }
	//standby_puts("standby_ir_init 9\n");
    /* pending���� */
    reg_val = STANDBY_RREG(IR_REG_RXSTA);
    reg_val |= IR_RXSTA_ALL_MASK;
    STANDBY_WREG(IR_REG_RXSTA, reg_val);
	//standby_puts("standby_ir_init 10\n");
    /* ����DMA�������� */
    reg_val = STANDBY_RREG(DMAC_REG_N0_CFG);
    reg_val |= (__u32)1 << 31;
    STANDBY_WREG(DMAC_REG_N0_CFG, reg_val);
	//standby_puts("standby_ir_init 11\n");

    return EPDK_OK;
}


/*
*********************************************************************************************************
*                           EXIT IR FOR STANDBY
*
*Description: exit ir for standby;
*
*Arguments  : none;
*
*Return     : result.
*               EPDK_OK,    exit ir successed;
*               EPDK_FAIL,  exit ir failed;
*********************************************************************************************************
*/
__s32 standby_ir_exit(void)
{
    //stop dma
    STANDBY_WREG(0xf1c02100/*DMAC_REG_N0_CFG*/, 0);
    //restore ir controller int registe
    STANDBY_WREG(IR_REG_RXINT, irs_status.ir_int);

    return EPDK_OK;
}


/*
*********************************************************************************************************
*                           DETECT IR FOR STANDBY
*
*Description: detect ir for standby;
*
*Arguments  : none
*
*Return     : result;
*               EPDK_OK,    receive some signal;
*               EPDK_FAIL,  no signal;
*********************************************************************************************************
*/
__s32 standby_ir_detect(void)
{
    if(0x80000000&STANDBY_RREG(DMAC_REG_N0_CFG))
    {
        return EPDK_FAIL;/* BUSY */
    }

    return EPDK_OK;
}

/*
*********************************************************************************************************
*                           VERIFY IR SIGNAL FOR STANDBY
*
*Description: verify ir signal for standby;
*
*Arguments  : none
*
*Return     : result;
*               EPDK_OK,    valid ir signal;
*               EPDK_FAIL,  invalid ir signal;
*********************************************************************************************************
*/
__s32 standby_ir_verify(void)
{
    __u32 code,reg_val;
    __u32 saddr, daddr;

	//standby_clk_apb2hosc();
	  //  standby_mdelay(100);    
    	//standby_clk_core2hosc();
	//	standby_mdelay(205);    
	    /* ��pendingλ��� */
	    reg_val = STANDBY_RREG(IR_REG_RXSTA);
	    reg_val |= IR_RXSTA_ALL_MASK;
	    STANDBY_WREG(IR_REG_RXSTA, reg_val);


    code = ir_nec_data_decode(ir_data, IR_DMA_LEN);

	//standby_put_hex(code);

    	//standby_put_hex(pStandbyPar->TvdFlag);
	//standby_put_hex(pStandbyPar->IrMask);
	//standby_put_hex(pStandbyPar->IrPowerVal);
	
    if( IR_ERROR_CODE !=  code )
    {


    	//standby_put_hex(pStandbyPar->TvdFlag);
	//standby_put_hex(pStandbyPar->IrMask);
	//standby_put_hex(pStandbyPar->IrPowerVal);		
	//standby_puts("here \n");
/*
        if(pStandbyPar->TvdFlag)
        {
            if(((code&0xffff)  == pStandbyPar->IrMask)
                    && (((code>>16)&0xff) == pStandbyPar->IrPowerVal))
            {            	
            	standby_ir_exit();
                return EPDK_OK;
            }
        }
        else
*/
        {
            if((code&0xffff)  == pStandbyPar->IrMask)
            {                
            	standby_ir_exit();
                return EPDK_OK;
            }
        }

    }

	//standby_clk_apb2losc();
	//    standby_mdelay(100);    
    	//standby_clk_core2losc();
	//	standby_mdelay(250);    

	//standby_puts("decode fail \n");

    /* ��pendingλ��� */
    reg_val = STANDBY_RREG(IR_REG_RXSTA);
    reg_val |= IR_RXSTA_ALL_MASK;
    STANDBY_WREG(IR_REG_RXSTA, reg_val);

    saddr = (__u32)(0x0fffffff & IR_REG_RXFIFO);/* ���ý��ռĴ���ΪԴ��ַ */
    daddr = (__u32)((__u32)ir_data & 0x000fffff);

    STANDBY_WREG(DMAC_REG_N0_CFG, 0xC160020/*0x0C110020*/);/* ����DMA, ˫8�ֽڣ���burst  */
    STANDBY_WREG(DMAC_REG_N0_SRC_ADDR, saddr);/* Դ�����ַ */
    STANDBY_WREG(DMAC_REG_N0_DST_ADDR, daddr);/* Ŀ�������ַ */

    reg_val = STANDBY_RREG(DMAC_REG_N0_BYTE_CNT);
    reg_val &= 0x3ffff;
    reg_val |= IR_DMA_LEN;/* ��ʱ����ȡ100���ֽ����ݣ����ֵ��ʵ�ʵ�һ֡IR�����ֽ���Ҫ��һ��  */
    STANDBY_WREG(DMAC_REG_N0_BYTE_CNT, reg_val);

    while(IR_RXSTA_RAC_MASK & STANDBY_RREG(IR_REG_RXSTA))
    {
        daddr =  STANDBY_RREG(IR_REG_RXFIFO);/* �������õ����� */
    }

    /* ����DMA�������� */
    reg_val = STANDBY_RREG(DMAC_REG_N0_CFG);
    reg_val |= (__u32)1 << 31;
    STANDBY_WREG(DMAC_REG_N0_CFG, reg_val);

    return EPDK_FAIL;
}


#endif  //ALLOW_IR_WAKEUP_STANDBY

