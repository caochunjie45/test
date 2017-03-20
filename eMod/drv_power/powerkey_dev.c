/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                           power key board driver
*
*                                    (c) Copyright 2010-2012, Sunny China
*                                             All Rights Reserved
*
* File    : powerkey_dev.c
* By      : Sunny
* Version : v1.0
* Date    : 2011-1-6 20:30:20
* Descript: ����Դ������Ϊһ�������豸ע�ᵽMELIS������ϵͳ.
* Update  : date                auther      ver     notes
*           2011-1-6 20:30:56   Sunny       1.0     ��Դ�������豸����.
*********************************************************************************************************
*/

#include    "powerkey_dev_i.h"

/* ��Դ���豸ע����豸���     */
__input_dev_t *pPowerKeyDev;

/* ��Դ��֧�ֵ��豸����         */
static __u32 PowerKeyDevCodes[] = 
{
	KPAD_POWER,
	KPAD_POWEROFF,
};

/*------------------------------------------------
 *
 *      ��Դ���豸�ڵ����������
 *
 *------------------------------------------------
 */
static __hdle PowerKey_DevOpen(void *openArg, __u32 Mode)
{
    __wrn("you can't open powerkey device\n");
   	return NULL;
}

static int PowerKey_DevClose(__hdle hDev) 
{
    __wrn("you can't close powerkey device\n");
   	return EPDK_FAIL;	
}

static __u32 PowerKey_DevRead(void *pBuffer, __u32 blk, __u32 n, __hdle hDev) 
{
    __wrn("you can't read powerkey device\n");
    return n;
}

static __u32 PowerKey_DevWrite(const void * pBuffer, __u32 blk, __u32 n, __hdle hDev) 
{
    __wrn("you can't write powerkey device\n");
    return 0;
}

static __s32 PowerKey_DevIoctrl(__hdle hDev, __u32 Cmd, __s32 Aux, void *pBuffer) 
{
    __wrn("you can't write powerkey device\n");
 	return EPDK_OK;
}

__dev_devop_t PowerKeyOps = 
{
    PowerKey_DevOpen,
    PowerKey_DevClose,
    PowerKey_DevRead,
    PowerKey_DevWrite,
    PowerKey_DevIoctrl
};

__s32 PowerKeyDevInit(void)
{
    /* �����Դ�������豸���������������ϵͳע��.*/
    __s32               i;
    __u32               KeyCodeNum;
    pPowerKeyDev = (__input_dev_t *)eLIBs_malloc(sizeof(__input_dev_t));
    if (pPowerKeyDev == NULL)
    {
    	__wrn("not enough memory for INPUT physical device handle\n");
    	return EPDK_FAIL;
    }
    eLIBs_memset(pPowerKeyDev, 0, sizeof(__input_dev_t));
    
    /* �豸��Դ���豸��Ϣ���������豸����ϵͳע��һ�������豸�ڵ� */
    pPowerKeyDev->classname = INPUT_CLASS_NAME;
    pPowerKeyDev->name      = "PowerKey";
    pPowerKeyDev->ops       = &PowerKeyOps;
    pPowerKeyDev->parg      = NULL;
    
    pPowerKeyDev->id.product = 0x0001;
    pPowerKeyDev->id.version = 0x0001;
    
    /* ���õ�Դ���豸���ͣ����ڰ������豸 */
    pPowerKeyDev->device_class = INPUT_KEYBOARD_CLASS;
    
    /* ���õ�Դ���豸֧�ֵ��¼����ͣ�ֻ֧�ְ����¼�,
     * ע���Դ���豸����֧���ظ�����
     */
    pPowerKeyDev->evbit[0] = BIT_MASK(EV_KEY);
    
    /* ���õ�Դ���豸֧�ֵİ����¼� */
    KeyCodeNum = sizeof(PowerKeyDevCodes) / sizeof(PowerKeyDevCodes[0]);
    for (i = 0; i < KeyCodeNum; i++)
    {
    	__set_bit(PowerKeyDevCodes[i], pPowerKeyDev->keybit);
    }
    
    /* �豸��Դ���豸���¼������ص�����ΪNULL,��֧���¼��Ļ��� */
    pPowerKeyDev->event = NULL;
    
    /* ��������ϵͳע���Դ�������豸 */
    if (esINPUT_RegDev(pPowerKeyDev) != EPDK_OK)
    {
        __wrn("register powerkey device to INPUT system failed\n");
        eLIBs_free(pPowerKeyDev);
    	return EPDK_FAIL;
    }
    
    __inf("PowerKey device initialize succeeded\n");
    return EPDK_OK;
}

void PowerKeyDevExit(void)
{
    if (pPowerKeyDev)
    {
        /* ע���豸���ͷ��ڴ� */
        esINPUT_UnregDev(pPowerKeyDev);
        eLIBs_free(pPowerKeyDev);
    }
}


__s32 PowerKeyDevSendEvent(__u32 KeyCode, __bool IsDown)
{
    if (IsDown)
    {
        /* ���Ͱ��������¼� */
        esINPUT_SendEvent(pPowerKeyDev,
	                      EV_KEY,
	                      KeyCode,
	                      KEY_DOWN_ACTION
	                      );
        /* ����ͬ���¼�     */
        esINPUT_SendEvent(pPowerKeyDev,
	                      EV_SYN,
	                      SYN_REPORT,
	                      0
	                      );
    }
    else
    {
        /* ���Ͱ���̧���¼� */
        esINPUT_SendEvent(pPowerKeyDev,
	                      EV_KEY,
	                      KeyCode,
	                      KEY_UP_ACTION
	                      );
        /* ����ͬ���¼�     */
        esINPUT_SendEvent(pPowerKeyDev,
	                      EV_SYN,
	                      SYN_REPORT,
	                      0
	                      );
    }
    return EPDK_OK;
}
