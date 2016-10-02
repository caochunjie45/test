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
*                判断接收到的码是否合法。判断标准:1.用户码或者地址码是否支持；2.数据是否错误。
*             
*  Parameters:
*       code   :  接收数据后解码得到的
*     user_code:  要校验的用户码或地址码
*
*  Return value:
*            成功返回EPDK_OK；失败返回EPDK_FAIL
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
*            根据接收到的数据帧解码，主要依据NEC的接收波形进行解码，解码过程如下:
*               解码前提:接收波形需要反转。
*               1.以8.7ms区别NEC前导；
*               2.以3.4ms区别normal code还是repeat code
*               3.0.56ms为高电平,注意脉冲不能超过或少于这个值  
*               4.低电平判断0或者1的标准为1.1275ms。
*             
*  Parameters:
*       buf  :  接收到的数据buffer首地址
*       size :  buffer的长度
*
*  Return value:
*           出错返回IR_ERROR_CODE，成功返回code值，为0为repeat code 。
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
	
    /*step 1-区分9ms*/
    len = 0;
    for( i = 0; i < size; i++)
    {
        val = buf[i];
        if(val & 0x80)//找9ms，高电平
        {
            len += val & 0x7f;//累加
        }
        else //低电平开始2.25ms或者4.5ms
        {
            if(len > NEC_IR_FORMAT_LEADING)//为NEC格式
            {
                break;
            }               
            else
            {
            	__here__
                return IR_ERROR_CODE;//非NEC格式，直接返回
            }
        }
    }   

    /*step 2- 区分4.5ms或者2.25ms */
    len = 0;	
    for(; i < size; i++)
    {    	
        val = buf[i];       
        if(val & 0x80)//开始找到数据高电平
        {
            if(len > NEC_IR_NORMAL_OR_REPEAT_CODE)
            {
                break;//normal code 处理
            }
            len = 0;
        }
        else //找4.5ms或2.25ms ，低电平
        {
            len += val & 0x7f;//累加
        }       
    }

    /*step3 -对用户码/地址码和数据解码*/
    code   = 0;  /*0 for Repeat Code*/
    bitCnt = 0;
    last   = 1;
    len    = 0;		
    for(; i < size; i++)
    {    	
        val = buf[i];       
        if(last)//统计高电平0.56ms，
        {
            if(val & 0x80)
            {
                len += val & 0x7f;
            }
            else //低电平出现
            {
                if(len > NEC_IR_PMAX) /*Error Pulse*/
                {
                	__here__
                    return IR_ERROR_CODE;
                }
                last = 0;//转入判断0,1
                len = val & 0x7f;//开始对低电平累加
            }
        }
        else //统计低电平识别0-1；
        {
            if(val & 0x80)//遇到高电平后
            {
                if(len > NEC_IR_DMAX) /*Error Distant*///必须保证低电平的持续时间不超过4个
                {
                	__here__
                    return IR_ERROR_CODE;
                }
                else//低电平 小于4个0.56ms
                {
                    if(len > NEC_IR_ONE_OR_ZERO_BIT)  //数据1，默认数据0
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
            else //统计低电平持续时间。
            {
                len += val & 0x7f;
            }
        }
    }
    __msg("decode result = %x\n", code);
    
    return code;
}

