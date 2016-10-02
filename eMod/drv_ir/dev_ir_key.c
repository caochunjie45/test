/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                  DRV IR KEY
*
*                                    (c) Copyright 2011-2014, Jackie China
*                                             All Rights Reserved
*
* File    : dev_ir_key.c
* By      : Jackie
* Version : v1.0
* Date    : 2012-01-09
* Descript: ir key device source file.
* Update  : date                auther      ver     notes
*           2012-01-09 15:07    Jackie      1.0     Create this file.
*********************************************************************************************************
*/

#include "drv_ir_key_i.h"

extern __ir_key_drv_t ir_key_drv;

/*
*********************************************************************************************************
*                           DEV IR KEY OPEN
*
* Description: DEV IR KEY open.
*
* Parameters : NONE.               
*
* Returns    : EPDK_OK if success,  EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/
__hdle DEV_IR_KEY_Open(void * open_arg, __u32 mode)
{
    __s32           cpu_sr;
    __ir_key_dev_t  *pIrkeyDev = (__ir_key_dev_t *)open_arg;
    
    ENTER_CRITICAL(cpu_sr);
    if(pIrkeyDev->used)
    {
        EXIT_CRITICAL(cpu_sr);
        __wrn("keyboard dev open fail, used by someone else!\n");
    	return NULL;
    }
  	pIrkeyDev->used = 1;
    EXIT_CRITICAL(cpu_sr);
    
   	return (__hdle)pIrkeyDev;
}
/*
*********************************************************************************************************
*                           DEV IR KEY CLOSE
*
* Description: DEV IR KEY close.
*
* Parameters : NONE.               
*
* Returns    : EPDK_OK if success,  EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/
__s32 DEV_IR_KEY_Close(__hdle hIrKey)
{
    __s32           cpu_sr;
    __ir_key_dev_t  *pIrkeyDev = (__ir_key_dev_t *)hIrKey;
    
    ENTER_CRITICAL(cpu_sr);
    if(pIrkeyDev->used == 0)
    {
        EXIT_CRITICAL(cpu_sr);
    	return EPDK_FAIL;
    }
  	pIrkeyDev->used = 0;
    EXIT_CRITICAL(cpu_sr);
    
   	return EPDK_OK;	
}
/*
*********************************************************************************************************
*                           DEV IR KEY READ
*
* Description: DEV IR KEY read.
*
* Parameters : NONE.               
*
* Returns    : EPDK_OK if success,  EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/
__u32 DEV_IR_KEY_Read(void *pdata, __u32 size, __u32 n, __hdle hIrKey)
{
    __wrn("you can't read ir keyboard device\n");
    return 0;
}

/*
*********************************************************************************************************
*                           DEV IR KEY WRITE
*
* Description: DEV IR KEY write.
*
* Parameters : NONE.               
*
* Returns    : EPDK_OK if success,  EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/
__u32 DEV_IR_KEY_Write(const void *pdata, __u32 size, __u32 n, __hdle hIrKey)
{
    __wrn("you can't write ir keyboard device\n");
    return 0;
}

/*
*********************************************************************************************************
*                           DEV IR KEY IOCTRL
*
* Description: DEV IR KEY ioctrl.
*
* Parameters : NONE.               
*
* Returns    : EPDK_OK if success,  EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/
__s32 DEV_IR_KEY_Ioctrl(__hdle hIR_KEY, __u32 cmd, __s32 aux, void *pbuffer)
{
    __s32       ret = EPDK_FAIL;
    
    switch(cmd)
    {
		case DRV_IRKEY_CMD_PUTKEY:
		{   

		    __ir_key_msg_t *msg = (__ir_key_msg_t *)pbuffer;
		    if(msg == NULL)
		    {
                return ret;
		    }		    
		    
		    /* send event to input system */
		   // __inf("ir keyborad send event to input system, "
		   //       "[type:%d, code:%d, value:%d]\n",
		    //      EV_KEY, msg->key_value, msg->flag);
		          
		    ret = esINPUT_SendEvent(ir_key_drv.input_dev,
		                      EV_KEY,
		                      msg->key_value,
		                      msg->flag
		                      );
		    ret = esINPUT_SendEvent(ir_key_drv.input_dev,
    	                      EV_SYN,
    	                      SYN_REPORT,
    	                      0
    	                      );
            break;
        }
#if 0
    	case DRV_IRKEY_CMD_SET_SBSEQ_RPT_TIME:
    	{
    	    __u32 rp_keytime = (__u32)pbuffer;             
            BSP_IR_key_set_rp_keytime(rp_keytime);
    		break;
    	}

    	case DRV_IRKEY_CMD_GET_SBSEQ_RPT_TIME:
    	{    	    
            ret = BSP_IR_key_get_rp_keytime();
    		break;
    	}

    	case DRV_IRKEY_CMD_DISPLAY_SCANCODE:
    	{
    	    __u32 on_off = (__u32)pbuffer;
    	    ret = BSP_IR_key_display_scancode(on_off);
            break;
    	}
#endif		
        case DRV_KEY_CMD_GET_IRPOWERVALUE:
        {
            ret = 21;
            break;
        }
		
        case DRV_KEY_CMD_GET_IRMASK:
        {
            ret = IR_key_get_code();
            break;
        }

        default:
            break;
    }
    
    return ret;
}

__dev_devop_t ir_key_dev_ops =
{
    DEV_IR_KEY_Open,
    DEV_IR_KEY_Close,
    DEV_IR_KEY_Read,
    DEV_IR_KEY_Write,
    DEV_IR_KEY_Ioctrl
};

