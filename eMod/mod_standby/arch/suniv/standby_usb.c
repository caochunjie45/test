/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                            power manager sub-system
*
*                                     (c) Copyright 2008-2009, kevin.z China
*                                              All Rights Reserved
*
* File   : standby_usb.c
* Version: V1.0
* By     : kevin.z
* Date   : 2009-6-16 17:45
*********************************************************************************************************
*/
#include "standby_usb.h"

#if(ALLOW_USB_WAKEUP_STANDBY)


//==============================================================================
// USB CHECK FOR WAKEUP SYSTEM FROM STANDBY
//==============================================================================
//usb base address
//just use usb0 for wakeup now
//by sunny at 2012-2-22 14:34:34
static __hdle standby_usb_port_base[1] = 
{
	STANDBY_USB0_REG_BASE
};


/* vbus, id, dpdm�仯λ��д1����, ��������ڲ�������bit��ʱ�������Щλ */
static __u32 __Standby_WakeUpClearChangeDetect(__u32 reg_val)
{
    __u32 temp = reg_val;

	temp &= ~(1 << USBC_BP_ISCR_VBUS_CHANGE_DETECT);
	temp &= ~(1 << USBC_BP_ISCR_ID_CHANGE_DETECT);
	temp &= ~(1 << USBC_BP_ISCR_DPDM_CHANGE_DETECT);

	return temp;
}

/*
*********************************************************************************************************
*                                     INIT USB FOR STANDBY
*
* Description: init usb for standby.
*
* Arguments  : none;
*
* Returns    : none;
*********************************************************************************************************
*/
void standby_usb_init(void)
{
    __u32 i 		= 0;
	__u32 reg_val 	= 0;

	for(i = 0; i < USBC_MAX_CTL_NUM; i++){
		//enable wakeup irq & dpdm
	    reg_val = USBC_Readl(USBC_REG_ISCR(standby_usb_port_base[i]));
	    reg_val |= 1 << USBC_BP_ISCR_IRQ_ENABLE;
		reg_val |= 1 << USBC_BP_ISCR_DPDM_CHANGE_DETECT_EN;
		reg_val = __Standby_WakeUpClearChangeDetect(reg_val);
		USBC_Writel(reg_val, USBC_REG_ISCR(standby_usb_port_base[i]));
	}

    return;
}


/*
*********************************************************************************************************
*                                     EXIT USB FOR STANDBY
*
* Description: exit usb for standby.
*
* Arguments  : none;
*
* Returns    : none;
*********************************************************************************************************
*/
void standby_usb_exit(void)
{
	/* ����Ͳ��ùر�USB��Դ��, ��Ϊ���ʱ��USB�����ڹ��� */
}

/*
*********************************************************************************************************
*                                     standby_is_usb_status_change
*
* Description:
*	ͨ��dpdm, id, vbus�ı仯��ȷ��USB�Ƿ�仯.
*
* Arguments  :
* 	port_no  :  input. usb�˿ں�
*
* Returns    :
*   ����USB�˿��Ƿ�仯
*
*********************************************************************************************************
*/
__s32 standby_is_usb_status_change(__u32 port_no)
{
    __u32 dpdm 		= 0;
	__u32 id 		= 0;
	__u32 vbus		= 0;
	__u32 reg_val 	= 0;
#if 0
    //some usb event happened, need not clear usb event, leave it for usb monitor
    //dpdm
    reg_val = USBC_Readl(USBC_REG_ISCR(standby_usb_port_base[port_no]));
	dpdm = reg_val & (1 << USBC_BP_ISCR_DPDM_CHANGE_DETECT);

	//id
	id = 0;

	//vbus
	vbus = 0;
#else   //20140419 hp modify
	/*ʹ��id/vbus ģʽ��ֻ���USB0*/
	__u32 *pg_data;
	__u32 data;

	pg_data =(__u32 *) 0xF1C208E8;	
	data = *pg_data;
	vbus = (data &(1 << 0x1));

	id   = 0;/*USB ���ֻ��Device*/
		
#endif
    return (dpdm | id | vbus);
}

/*
*********************************************************************************************************
*                                     QUERY USB EVENT
*
* Description: query usb event for wakeup system from standby.
*
* Arguments  : none;
*
* Returns    : result;
*               EPDK_TRUE,  some usb event happenned;
*               EPDK_FALSE, none usb event;
*********************************************************************************************************
*/
__s32 standby_query_usb_event(void)
{
    __u32 i = 0;

    //ֻҪ��һ��USB�˿��б仯�������Ի���ϵͳ
	for(i = 0; i < USBC_MAX_CTL_NUM; i++){
		if(standby_is_usb_status_change(i)){
			return EPDK_TRUE;
	}
	}

    return EPDK_FALSE;
}


#endif  //ALLOW_USB_WAKEUP_STANDBY

