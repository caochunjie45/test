/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                   DEV KEY
*
*                                    (c) Copyright 2011-2014, Jackie China
*                                             All Rights Reserved
*
* File    : dev_key.c
* By      : Jackie
* Version : v1.0
* Date    : 2011-11-28
* Descript: key device source file.
* Update  : date                auther      ver     notes
*           2011-11-28 15:07    Jackie      1.0     Create this file.
*********************************************************************************************************
*/

#include "drv_key_i.h"

extern __key_drv_t keydrv;

/*
*********************************************************************************************************
*                            DEVICE MODULE OPEN
*
*Description: key device module open.
*
*Arguments  : None.
*
*Return     : Module exit result;
*               == 0    module exit successful;
*               != 0    module exit failed, maybe the module is used by other module.
*********************************************************************************************************
*/

__hdle DEV_KEY_Open(void * open_arg, __u32 mode)
{
	__s32           cpu_sr;
    __key_dev_t  *pKeycDev = (__key_dev_t *)open_arg;
    
    ENTER_CRITICAL(cpu_sr);
    if(pKeycDev->used)
    {
        EXIT_CRITICAL(cpu_sr);
        __wrn("keyboard dev open fail, used by someone else!\n");
    	return NULL;
    }
  	pKeycDev->used = 1;
    EXIT_CRITICAL(cpu_sr);
    
   	return (__hdle)pKeycDev;
}

/*
*********************************************************************************************************
*                          DEVICE MODULE EXIT
*
*Description: key device module close.
*
*Arguments  : None.
*
*Return     : Module exit result;
*               == 0    module exit successful;
*               != 0    module exit failed, maybe the module is used by other module.
*********************************************************************************************************
*/

__s32 DEV_KEY_Close(__hdle hKey)
{
	
    __s32           cpu_sr;
    __key_dev_t  *pKeycDev = (__key_dev_t *)hKey;
    
    ENTER_CRITICAL(cpu_sr);
    if(pKeycDev->used == 0)
    {
        EXIT_CRITICAL(cpu_sr);
    	return EPDK_FAIL;
    }
  	pKeycDev->used = 0;
    EXIT_CRITICAL(cpu_sr);
    
   	return EPDK_OK;		
}

/*
*********************************************************************************************************
*                           DEVICE MODULE READ
*
*Description: key device module read, do nothing.
*
*Arguments  :
*
*Return     :
*
*Note       : 
*********************************************************************************************************
*/

__u32 DEV_KEY_Read(void *pdata, __u32 size, __u32 n, __hdle hKey)
{
    __wrn("you can't read keyboard device\n");
    return 0;
}

/*
*********************************************************************************************************
*                           DEVICE MODULE WRITE
*
*Description: key device module write, do nothing.
*
*Arguments  :
*
*Return     :
*
*Note       : 
*********************************************************************************************************
*/

__u32 DEV_KEY_Write(const void *pdata, __u32 size, __u32 n, __hdle hKey)
{
    __wrn("you can't write keyboard device\n");
    return 0;
}

/*
*********************************************************************************************************
*                         DEVICE MODULE IO CONTROL
*
*Description: key device module IO control.
*
*Arguments  : mp        the handle of the key device module;
*             cmd       the command send to key device;
*             aux       user define;
*             pbuffer   user define;
*
*Return     :
*********************************************************************************************************
*/

__s32 DEV_KEY_Ioctrl(__hdle hKey, __u32 cmd, __s32 aux, void *pbuffer)
{
     __s32       ret = EPDK_FAIL;
    

    switch(cmd)
    {
		case DRV_KEY_CMD_PUTKEY:
		{   
		    if(aux != 255)
		    {
    		    __key_msg_t *msg = (__key_msg_t *)pbuffer;
    		    if(msg == NULL)
    		    {
                    return ret;
    		    }		    
    		    
    		    /* send event to input system */
    		    //__inf("keyborad send event to input system, "
    		    //      "[type:%d, code:%d, value:%d]\n",
    		    //      EV_KEY, msg->key_value, msg->flag);
    		          
    		    esINPUT_SendEvent(keydrv.pKeyc_dev,
    		                      EV_KEY,
    		                      msg->key_value,
    		                      msg->flag
    		                      );
    		    esINPUT_SendEvent(keydrv.pKeyc_dev,
        	                      EV_SYN,
        	                      SYN_REPORT,
        	                      0
        	                      );
        	 }
        	 else
        	 {
                 __u32 key_value = (__u32)pbuffer;          
                 
                 /* send event to input system */
                 //__inf("virtual keyboard send event to input system, "
                 //      "[type:%d, code:%d, value:%d]\n",
                 //      EV_KEY, key_value, 1);
                       
                 esINPUT_SendEvent(keydrv.pKeyc_dev,
                                   EV_KEY,
                                   key_value,
                                   1 /*key down, no key up*/
                                   );
                 esINPUT_SendEvent(keydrv.pKeyc_dev,
                                   EV_SYN,
                                   SYN_REPORT,
                                   0
                                   );
        	 }        	 
             break;
		}

		case DRV_KEY_CMD_SET_FIRST_DEBOUNCE_TIME:
    	{    	   
    		break;
    	}

    	case DRV_KEY_CMD_SET_FIRST_RPT_TIME:
    	{    	    
    		break;
    	}

    	case DRV_KEY_CMD_SET_SBSEQ_RPT_TIME:
    	{    	    
    		break;
    	}

    	case DRV_KEY_CMD_GET_FIRST_DEBOUNCE_TIME:
    	{
            
     	    break;
    	}

    	case DRV_KEY_CMD_GET_FIRST_RPT_TIME:
    	{
            
    		break;
    	}

    	case DRV_KEY_CMD_GET_SBSEQ_RPT_TIME:
    	{
            
    	    break;
    	} 
		default:
       {
		   ret = EPDK_FAIL;
		   break;
	   }
    }//end-of-switch
    
	return ret;
}

__dev_devop_t keyc_dev_ops =
{
    DEV_KEY_Open,
    DEV_KEY_Close,
    DEV_KEY_Read,
    DEV_KEY_Write,
    DEV_KEY_Ioctrl
};
