
#ifndef CMMBDRV_INTERFACE_H
#define CMMBDRV_INTERFACE_H

#include "maple_common.h"

#define CMMB_DRIVER_FILE_PATH              CEDAR_PLUGINS_DIRECTORY"cmmb.drv"

#define CMMB_MAX_FILTER_NUM             (16)
#define CMMB_MAX_FILTER_REFFERENCE_NUM  (16)
#define CMMB_MAX_CONTROL_FRAME_SIZE     8640
#define CMMB_MAX_DATA_FRAME_SIZE        (288*240)   //* set a bound of one max RS table (288 rows X 240 bytes per row), it is not set formally by the standard.
#define CMMB_MAX_PROGRM_FRAME_SIZE        (288*240)   //For if206 by antonio


typedef struct CMMB_SIGNAL_STATUS
{
    __u8 strength;                      //* the signal strength is from 0 to 100.
    __u8 quality;                       //* the signal quality is from 0 to 100.
    __u8 bLock;													//*for siano3130 
}cmmb_ss_t;

typedef struct CMMB_TUNE_PARAM
{
    __u32 frequency;
    __u32 bandwidth;
}cmmb_tune_param_t;


typedef enum CMMB_FILTER_TYPE
{
    CMMB_FILTER_TYPE_CONTROL_FRAME,         //* filter for time slot 0, the cmmb control frame.
    CMMB_FILTER_TYPE_CONTROL_UPDATE_FRAME,  //* filter for time slot 0, the cmmb control frame, frame is receive only when it has some update.
    //CMMB_FILTER_TYPE_DATA_FRAME,            //* filter for time slot 1~39 frmaes.
    CMMB_FILTER_TYPE_DATA_FRAME,          //filter for time slot 1~2 frmaes.   //For if206 by antonio
    CMMB_FILTER_TYPE_PROGM_FRAME,         //filter for time slot 3~39 frmaes.   //For if206 by antonio
    
    CMMB_FILTER_TYPE_NUM
}cmmb_filter_type_t;


typedef struct CMMB_FILTER_REGISTER
{
    cmmb_filter_type_t  type;           //* filter type.
    __u32               service_id;     //* one channel has one pid.
    __u32               timeslot_start; //* time slot start index of the multiplex frame which is containing the service.
    __u32               timeslot_cnt;   //* how many time slots the multiplex frame occupy.
    __u8                demod_arg;      //add by antonio for IF206 demod arg
    __u8                muxfrmId;				//only for tp2313
    __u8                rsRate;					//only for tp2313
    __u8                byteInterMode;	//only for tp2313
    __u8                ldpcRate;				//only for tp2313
    __u8                modulateMode;		//only for tp2313
    __u8                scrambleMode;		//only for tp2313
    __pCBK_t            callback;       //* callback function, when frame data arrived, the driver will call this function.
    void*               callbackparam;  //* parameter passed to the callback function.
}cmmb_filter_register_t;


typedef enum CMMB_DRV_CMD
{
    CMMB_DRV_CMD_TUNE,                  //* tune tuner to a specified frequency
                                        //* aux = 0, pbuffer = a pointer to a variable of struct type cmmb_tune_param_t, which specify the frequency and bandwidth.
                                        //* return value: EPDK_OK or EPDK_FAIL;
                                        
    CMMB_DRV_CMD_GET_SIGNAL_STATUS,     //* get signal status.
                                        //* aux = 0, pbuffer = a pointer to a variable of struct type cmmb_ss_t which is used to save signal status;
                                        //* return value: EPDK_OK or EPDK_FAIL;
                                        
    CMMB_DRV_CMD_GET_CUR_TUNE_PARAM,    //* get the current tuner parameters
                                        //* aux = 0, pbuffer = a pointer to a variable of struct type cmmb_tune_param_t which is used to save the current tuner parameters;
                                        
    CMMB_DRV_CMD_OPEN_CHAN,             //* open a cmmb channel to receive data.
                                        //* aux = 0, pbuffer = a pointer to a variable of struct type cmmb_filter_register_t;
                                        //* return value: EPDK_FAIL if fail, the channel id if success.
                                        
    CMMB_DRV_CMD_CLOSE_CHAN,            //* close a cmmb channel to stop receiving data.
                                        //* aux = the channel id returned by CMMB_DRV_CMD_OPEN_CHAN io command; pbuffer = NULL;
                                        
    CMMB_DRV_CMD_GET_CA_SN,             //* get the smd ca serial number.
                                        //* aux = the buffer size, pbuffer = buffer to store the ca serial number.
   	CMMB_DRV_CMD_SET_CA_TO_DRV,					//set ca table num to cmmbdrv for if 206 

		CMMB_DRV_CMD_SEND_CA_PARAM_TO_DRV	, //send ca param to if 206 for openning ca function

		CMMB_DRV_CMD_GET_SERVICE_STATUS,    //获取加密台授权状态
		
    CMMB_DRV_CMD_NUM
}cmmb_drv_cmd_t;


#endif

