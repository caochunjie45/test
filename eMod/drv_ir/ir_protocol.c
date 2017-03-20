/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                  DRV IR KEY
*
*                                    (c) Copyright 2011-2014, Jackie China
*                                             All Rights Reserved
*
* File    : ir_protocol.c
* By      : Jackie
* Version : v1.0
* Date    : 2012-01-09
* Descript: ir protocol source file(support nec protocol currently).
* Update  : date                auther      ver     notes
*           2012-01-09 15:07    Jackie      1.0     Create this file.
*********************************************************************************************************
*/

#include "drv_ir_key_i.h"
#include "ir_protocol.h"


/*
****************************************************************************************************
*
*  FunctionName:           ir_nec_code_valid
*
*  Description:
*                �жϽ��յ������Ƿ�Ϸ����жϱ�׼:1.�û�����ߵ�ַ���Ƿ�֧�֣�2.�����Ƿ����
*             
*  Parameters:
*       code   :  �������ݺ����õ���
*     user_code:  ҪУ����û�����ַ��
*
*  Return value:
*            �ɹ�����EPDK_OK��ʧ�ܷ���EPDK_FAIL
*  Notes:
*   
****************************************************************************************************
*/ 
__s32 IR_PROTOCOL_NecCodeValid(__u32 code, __u32 user_code)
{
    __u32 tmp1, tmp2;
    
    //ignore user_code check if user_code is ZERO!
    if(user_code != IR_NOT_CHECK_USER_CODE)
    {
        /* Check Address or User Code */
        if((code&0xffff) != (user_code&0xffff))
        {
           return EPDK_FALSE; /* Address Error */
        }
    }

    tmp1 = (code & 0x00ff0000)>>16;
    tmp2 = (code & 0xff000000)>>24;

    if(tmp1^tmp2 == 0xff) /*Check User Code*/
    {
        return EPDK_TRUE;
    }
    else
    {
        return EPDK_FALSE; 
    }

}

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
__u32 IR_PROTOCOL_NecDecode(const __u8 *buf, const __u32 size)
{
    __u32 len    = 0;//low or high level time duration
    __u32 code   = 0;//decode result
    __s32 bitCnt = 0;// 32bit long  16bit user code+ 8bit data + 8bit invert data 
    __u32 i      = 0;//counter for size
    __u8  val    = 0;//store sample data
    __u8  last   = 0;//low or high level flag
		
    if(buf == NULL)
    {
        __wrn("buf is NULL! please check it.\n");
        return IR_ERROR_CODE;
    }
    if(size == 0)
    {
        __wrn("size is zero!\n");
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
            	__here__
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
                	__here__
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
                	__here__
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
    __msg("decode result = %x\n", code);
    
    return code;
}

