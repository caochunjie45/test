
#ifndef DEMOD_INTERFACE_H
#define DEMOD_INTERFACE_H

//#include "maple_common.h"

#define DEMOD_DRV_FILE_PATH  CEDAR_PLUGINS_DIRECTORY"demod.drv"

typedef enum DEMOD_BANDWIDTH
{
    DEMOD_BANDWIDTH_8M       = 8000,    //* 8000KHz
    DEMOD_BANDWIDTH_7M       = 7000,    //* 7000KHz
    DEMOD_BANDWIDTH_6M       = 6000,    //* 6000KHz
    DEMOD_BANDWIDTH_5M       = 5000,    //* 5000KHz
    DEMOD_BANDWIDTH_2M       = 2000,    //* 2000KHz
    DEMOD_BANDWIDTH_1_5M     = 1500,    //* 1500KHz
    DEMOD_BANDWIDTH_1_SEG    = 430,     //* 430KHz
    DEMOD_BANDWIDTH_3_SEG    = 1290,    //* 1290KHz
    DEMOD_BANDWIDTH_FULL_SEG = 5590     //* 5590KHz
}demod_bandwidth_e;

typedef enum DEMOD_STATUS
{
    SLEEP,                  //* set the demodulator to sleep mode can save power.
    WORKING                 //* set the demodulator to working mode before it is used.
}demod_status_t;

typedef struct DEMOD_SIGNAL_STATUS
{
    __u8 strength;          //* the signal strength is from 0 to 100.
    __u8 quality;           //* the signal quality is from 0 to 100.
}demod_ss_t;

typedef struct DEMOD_LOCK_CB_PARAM
{
    void*           pOpaque;
    __u8            lockOk;         //* whether the demodulator can receive signal and ts data.
    demod_ss_t      signalStatus;   //* signal strength and signal quality.
}lock_cb_param_t;

typedef struct TUNE_PARAM
{
    __u32 	frequency;    //unit:kHz
    __u32 	bandwidth;    //unit:kHz
//#if (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
    __u32		plpindex;		//DVBT2ר��
 		__u32		plppid;			//DVBT2ר��
 		__u32		plpnumber;  //DVBT2ר��
			__s32	t_t2_flag;	
//#endif
}tune_param_t;

typedef struct DEMOD_DATA
{
	__u8* pTsData;                        //* start address of ts packets.��һ������,��Ϊ��ringbuffer,���Կ��ܻ��ͷ,�����ͷ�Ϊ��������.
	__u32 TsPktNum;                       //* packet number of the first part of ts packets.
	__u8* pRingTsData;                    //* start address of the second part of ts packets, �ڶ�������.
	                                      //*  use a loop buffer, so there may be two part of ts packets when buffer loop back.
	__u32 RingTsPktNum;                   //* packet number of the second part of ts packets.
	
}
demod_data_t;

typedef struct DEMOD_RDWT_DATA
{
	
	__u32 RdSize;                        //* TS����BUF�Ѿ���ȡ��TS��byteΪ��λ . ����ָʾRdPos��λ��.��0�ֽڼ���
	__u32 WtSize;                       //* TS����BUF�Ѿ�д���TS��byteΪ��λ .  ����ָʾwtPos��λ��.
}
demod_rdwt_data_t;


typedef enum DEMOD_CMD
{
    //* set the demodulator/tuner to sleep mode to save power
    DEMOD_SLEEP,                //* aux = 0, pbuffer = NULL;
    
    DEMOD_WAKEUP,               //* aux = 0, pbuffer = NULL;
    
    DEMOD_LOCK,                 //* create a task to check the signal status every one second.
                                //* the signal status is passed through the lock callback function you set.
                                //* aux = 0, pbuffer = NULL;
                                
    DEMOD_UNLOCK,               //* delete the locking task.
                                //* aux = 0, pbuffer = NULL;
    
    DEMOD_GET_STATUS,           //* get working status of the demodulator.
                                //* aux = 0, pbuffer = NULL, return value: SLEEP or WORKING;
                                
    DEMOD_GET_SIGNAL_STATUS,    //* get signal status.
                                //* aux = 0, pbuffer = a pointer to a variable of struct type demod_ss_t which is used to save signal status;
                                //* return value: EPDK_OK or EPDK_FAIL;

    DEMOD_GET_LOCK_STATUS,      //��ѯ��ǰ�Ƿ�������Ƶ��, ret = 1:lock; ret=0:unlock;
                                
    DEMOD_TUNE,                 //* tune tuner to a specified frequency
                                //* aux = 0, pbuffer = a pointer to a variable of struct type tune_param_t, which specify the frequency and bandwidth.
                                //* return value: EPDK_OK or EPDK_FAIL;
                                
    DEMOD_SET_LOCK_CB,          //* set the callback function and callback parameter, these will be use when a lock task is created.
                                //* aux = a pointer to a variable of struct type lock_cb_param_t;
                                //* pbuffer = pointer to the callback function;
                                
    DEMOD_GET_CUR_TUNE_PARAM,   //* get the current tuner parameters
                                //* aux = 0, pbuffer = a pointer to a variable of struct type tune_param_t which is used to save the current tuner parameters;
                                
    DEMOD_GET_CMMB_DRIVER_HDL,  //* get the handler of cmmb demod driver
                                //* aux = 0, pbuffer = a pointer to a variable to store the driver handle;
                                //* return value: 0 means ok, -1 means fail.
                                
    DEMOD_GET_CA_SN,            //* get the ca serial number
                                //* aux = buffer size, pbuffer = buffer to store ca serial number. result = 1:���ɹ�,��״̬�ı�; 0:���ɹ�,��״̬����;-1:���ʧ��,����Ҫ��ʼ��

    DEMOD_RESET_BCAS,           //���³�ʼ��bcas

//    DEMOD_SET_FECONFIG,         	// ��ǰ���������ø�DEMOD�� pbuffer = CFEConfig*
//
//    DEMOD_OPEN,                 		// demod ��

    DEMOD_REQUEST_TS_DATA,     //* pbuffer = a pointer to a variable of struct type demod_data_t;
    
    //* flush ts packets 
    DEMOD_FLUSH_TS_DATA,     	//* pbuffer = bytes to flush.
//#if (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)	
		DEMOD_TUNE_PLAY,					//DVBT2ר�ã��ڲ���ʱ�����Ƶ�㣬PLPPID�Ȳ���ѡ�����ȥ���š�
		DEMOD_TUNE_T2_SEARCH,
		DEMOD_GET_TUNE_PLPPID,
		DEMOD_GET_DEMOD_BER,
		DEMOD_TUNE_DVB_T_SEARCH,  			//FOR DEMOD support DVBT OR DVBT2 only TUNE DVB-T
//#endif    
    DEMOD_CMD_NUM
}demod_cmd_t;

#endif
