/*
************************************************************************************************************************
*                                                         MAPLE
*                                                   the DTV middleware
*
*                             Copyright(C), 2010-2011, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : maple.h
*
* Author      : LiuXiaochuan
*
* Version     : 1.0
*
* Date        : 2010.03.01
*
* Description :
*
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* LiuXiaochuan   2010.03.01        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef _MAPLE_H_
#define _MAPLE_H_
#include "mod_cedar.h"

#define MAPLE_MAX_NAME_LEN	          64    //* maximum bytes length of a name string.
#define MAPLE_MAX_SERV_NUM            128    //* maximum service number.
#define MAPLE_MAX_SCHDL_NUM           1024  //* maxinum schedule event number
#define MAPLE_MAX_SCHDL_NUM_PER_SERV  672   //* maxinum schedule event number for one service
                                                  //* (96 event per day * 7 days)
#define MAPLE_MAX_VD_STREAM_NUM       1     //* maximum video stream number of a service.
#define MAPLE_MAX_AD_STREAM_NUM       4     //* maximum audio stream number of a service.
#define MAPLE_MAX_ST_STREAM_NUM       4     //* maximum subtitle stream number of a service.
#define MAPLE_MAX_TS_ITEMS            5

#define MAX_LTO_REGION_NUM  20 // max local time offset region number
//***********************************************************************//
//******************** define IO command return type ********************//
//***********************************************************************//
typedef enum MAPLE_RETURN_TYPE
{
    MAPLE_ERR  = -1,
    MAPLE_OK   = 0,
}maple_ret_e; 


//***********************************************************************//
//***************** define time prototype used in TV ********************//
//***********************************************************************//
typedef enum MAPLE_WEEKDAY
{
    Sunday,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
}maple_weekday_t;

typedef struct
{
    __u8                      eCountry[4]; 
    __u8                      u8RegionId : 6;
    __u8                      u8Reserved : 1;
    __u8                      u8LTOPolarity : 1; // local_time_offset_polarity �á�0��ʱ������Ϊ����˵������ʱ������UTCʱ�䣨ͨ���ڸ��������Զ������á�1��ʱ������Ϊ����˵������ʱ������UTCʱ�䡣
    __u16                     u16LTO;           // Local_time_offset ��ΧΪ-12Сʱ��+13Сʱ��16���غ���4��4λBCD�룬˳��ΪСʱ��ʮλ��Сʱ�ĸ�λ���ֵ�ʮλ���ֵĸ�λ��
    __u8                      au8TimeOfChange[5];
    __u16                     u16NextTimeOffset;
} maple_lto_info;

typedef struct 
{
    __u16   year;
    __u8    month;
    __u8    day;
    __u8    weekDay;
    __u8    hour;
    __u8    min;
	__u8    sec;
	
	__u16   MJD;
}maple_utc_time_t;

typedef struct
{
    __u8             u8NumofRegion;
    maple_lto_info   aLTOInfo[MAX_LTO_REGION_NUM]; // Local time offset info.
} maple_local_time_offset;

typedef struct MAPLE_TIME
{
    __u16   year;
    __u8    month;
    __u8    day;
    __u8    weekDay;
    __u8    hour;
    __u8    min;
	__u8    sec;
	
	__u16   MJD;
    
    maple_local_time_offset          stTotLto;
} maple_time_t;
//***********************************************************************//
//************** define some types used by service list *****************//
//***********************************************************************//
#define MAPLE_TV_DVB_T          0
#define MAPLE_TV_ISDB_T         1
#define MAPLE_TV_CMMB           2
#define MAPLE_TV_DTMB           3
#define MAPLE_TV_ATSC        	4
#define MAPLE_TV_DVB_T2       	5

//* cmmb driver
#define MAPLE_TV_CMMB_SIANO     0
#define MAPLE_TV_CMMB_IF				1
#define MAPLE_TV_CMMB_TELE			2

//TV subtitle
#define PLAY_DVBT_DVBSUBTITLE   1
#define PLAY_DVBT_DVBTELETEXT		0
#define PLAY_ISDBT_CLOSECAPTION 0
//* define service type
typedef enum MAPLE_SERVICE_TYPE
{
	MAPLE_SERV_TYPE_TV       = 1,
	MAPLE_SERV_TYPE_RADIO    = 2,
	MAPLE_SERV_TYPE_TELETEXT = 3,
	
	MAPLE_SERV_TYPE_UNKNOWN  = -1
}maple_serv_type_e;


//* define service group type
typedef enum MAPLE_SERVICE_GROUP_TYPE
{
    MAPLE_SERV_GROUP_TYPE_ALL,
    
    MAPLE_SERV_GROUP_TYPE_TV,
    MAPLE_SERV_GROUP_TYPE_RADIO,
    MAPLE_SERV_GROUP_TYPE_TELETEXT,
    
    MAPLE_SERV_GROUP_TYPE_NEWS,
    MAPLE_SERV_GROUP_TYPE_SCIENCE,
    MAPLE_SERV_GROUP_TYPE_SPORT,
    MAPLE_SERV_GROUP_TYPE_MOVIE,
    MAPLE_SERV_GROUP_TYPE_CHILDREN,
    MAPLE_SERV_GROUP_TYPE_MUSIC,
    MAPLE_SERV_GROUP_TYPE_FATION,
    
    MAPLE_SERV_GROUP_TYPE_FAVOURATE,
    MAPLE_SERV_GROUP_TYPE_ALL_CHANNEL,
    
    MAPLE_SERV_GROUP_TYPE_NUM
}maple_serv_group_type_e;


//* define video codec types, as I know, only mpeg2 and h264 are used in dvb.
typedef enum MAPLE_VIDEO_CODEC_TYPE //SI��,ʵ��תΪmod_cedar.h���������CEDAR_ABS_TYPE_AC3��
{
    MAPLE_VID_TYPE_MPEG2      = 1,
    MAPLE_VID_TYPE_H264       = 2,
    MAPLE_VID_TYPE_AVS        = 3,
    
	MAPLE_VID_TYPE_UNKNOWN	  = -1
}maple_vd_codec_e;


//* define audio codec types, as I know, only the 4 codecs below are used in dvb.
typedef enum MAPLE_AUDIO_CODEC_TYPE
{
    MAPLE_AUD_TYPE_MPEG       = 1,
    MAPLE_AUD_TYPE_AC3        = 2,
    MAPLE_AUD_TYPE_AAC        = 3,
    MAPLE_AUD_TYPE_DTS        = 4,   
    MAPLE_AUD_TYPE_DRA        = 5,              
    
	MAPLE_AUD_TYPE_UNKNOWN	  = -1
}maple_ad_codec_e;


//* define subtitle codec types.
typedef enum MAPLE_SUBTITLE_CODEC_TYPE
{
    MAPLE_SUBT_TYPE_DVBSUB = CEDAR_LRC_TYPE_DVBSUB,
    MAPLE_SUBT_TYPE_TTXT =  CEDAR_LRC_TYPE_TTXT,
    MAPLE_SUBT_TYPE_ARIB = CEDAR_LRC_TYPE_ISDB,
	MAPLE_SUBT_TYPE_UNKNOWN	  = -1
}maple_subt_codec_e;


//* video stream information of a service.
typedef struct MAPLE_VIDEO_INFO
{
	__u16			  pid;
	maple_vd_codec_e  vdCodeType;
	__u16             frameRate;
	__u16             height;
	__u16             width;
}maple_vd_info_t;


//* audio stream information of a service.
typedef struct MAPLE_AUDIO_INFO
{
	__u16			   pid;
	maple_ad_codec_e   adCodeType;
	__u16              sampleRate;
	__u16              codeRate;
	__u8			   adStreamName[MAPLE_MAX_NAME_LEN];
}maple_ad_info_t;


//* subtitle stream information of a service
typedef struct MAPLE_SUBTITLE_INFO
{
	__u16			    subtPid;
	maple_subt_codec_e	subtCodeType;
	__u8			    subtName[MAPLE_MAX_NAME_LEN];
}maple_subt_info_t;


//* demodulation information of a service
typedef struct MAPLE_DEMOD_INFO
{
    __u32   frequency;  //unit:kHz
    __u32   bandwidth;  //unit:kHz
//#if(MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
   	__u32		plpindex;		//DVBT2ר��
 	__u32		plppid;			//DVBT2ר��
 	__u32		plpnumber;  //DVBT2ר��
//#endif
 	__u32		t_t2_flag;
}maple_demod_info_t;


//* service item contains all information of one service.
typedef struct MAPLE_SERVICE_ITEM   //748bytes
{   
	__u8                recordInfo;
	__u8                isFavourate;                    //* searchʱĬ��0.indicate wheter this service is a favourate one, 
	__u8                isLocked;                       //* searchʱĬ��0.whether the service is locked
	__s32               key;                            //* searchʱĬ��0,��isLocked����.enter this key if you want to receive a locked service
	
	__u32               channelId;                      //* logical program id,������spec��ĺ���,�����ڲ��Զ����id��, �������ظ�.
	//Ŀǰ�������Ӹ�serviceʱ��service������Ϊid��.maple_serv_list_t->pServItems.��������ʱ,�������û�и�ֵ.
	//��Ϊ��������ʱ����ȷ���Ƿ����������service����maple�Լ���silib��service�б���.�����ڼ���maple��service�б�ʱ�Żḳֵ.isdb_add_serv_item()
                        
	__u16               pmtId;                          //* program map table pid of the service,����pat��ʱ��ֵ.
	                                                    //* for cmmb, this field is used as the subframe id of the service.
    __u16               servId;                         //* service id of the service,
    __u16               tsId;                           //* transport stream id to which the service belong
                                                        //* for cmmb, this field is used as the time slot start index of the multiplex frame which containing the service.
    __u16               networkId;                      //* network id to which the service belong
                                                        //* for cmmb, the low byte of this field is used as the time slot count of the multiplex frame which containning 
                                                        //* the service, and the high byte of is used to store the original character coding type of its esg information.
		__u8								demodArg;												//for demod if206 driver use it to open chan filter.
		
		__u8								byteInterMode;									//only for cmmb tp3213 
		__u8								ldpcRate;												//only for cmmb tp3213
		__u8								modulateMode;										//only for cmmb tp3213
		__u8								muxfrmId;												//only for cmmb tp3213
		__u8								rsRate;													//only for cmmb tp3213
		__u8								scrambleMode;										//only for cmmb tp3213
													
		maple_serv_type_e   servType;                       //* service type
    __u8                isCAFree;                       //* indicate whether the service is encrypted, 1:free; 0:encrypt
    __u8                servName[MAPLE_MAX_NAME_LEN];   //* name of the service
    
    maple_demod_info_t  demod_pm;                       //* tune and demodulation information of the service
            		
    //* stream information of the service
    __u8                hasPcr;                         //* whether this service has program clock reference.
    __u16               pcrPid;                         //* the pid of ts packets in which the pcr exits.
            		
    //* video stream information
    __u8                vdStreamNum;                    //* how many video streams exit in the service
    maple_vd_info_t     vdInfo[MAPLE_MAX_VD_STREAM_NUM];      //* video stream information of each video stream
           
    //* audio stream information
    __u8                adStreamNum;                    //* how many audio streams exit in the service
    maple_ad_info_t     adInfo[MAPLE_MAX_AD_STREAM_NUM];      //* audio stream information of each audio stream     
        
    //* subtitle stream information
    __u8                subtitleStreamNum;              //* how many subtitle streams exit in the service
    maple_subt_info_t	subtInfo[MAPLE_MAX_ST_STREAM_NUM];    //* subtitle stream information of each subtitle stream

    __s32               nOneSegFlag;        //��ǰ�˵���ģʽ��isdbʱ,ָʾ��service��oneseg����fullseg�����.��������ģʽ��dvb,atsc,������������.��0����.  Ŀǰ��pmt���������ڸ�ֵ.
    __u16               nRemoteControlKeyId; //��service���ڵ�ts����remote control key id.���û��,����Ϊ0xff. isdb��SI����.
    __s32               uReserved[16];
}maple_serv_item_t;

//* service list
typedef struct MAPLE_SERVICE_LIST
{
    __s32                servNum;           //* how many services totally in the list, <= nServItemsCount
	__s32           	 curServIdx;        //* service index last played
    maple_serv_item_t   *pServItems;        //* list
    __s32                nServItemsCount;   //* pServItems�ܵ�����. MAPLE_MAX_SERV_NUM
}maple_serv_list_t;




//***********************************************************************//
//************************* define simple EPG ***************************//
//***********************************************************************//

//* simple EPG information
typedef struct MAPLE_SIMPLE_EPG
{
    __u8           hasCurEventInfo;                //* whether current event(program) information exit
    __u8           hasNextEventInfo;               //* whether the next event(program) information exit
    maple_time_t   curEventstartTime;              //* current event start time
    maple_time_t   curEventEndTime;                //* current event end time
    maple_time_t   nextEventstartTime;             //* next event start time
    maple_time_t   nextEventEndTime;               //* next event end time
    __u8           curEventName[MAPLE_MAX_NAME_LEN];     //* name of current event
    __u8           nextEventName[MAPLE_MAX_NAME_LEN];    //* name of the following event
    __u8           *curEventContent;  //event���������,�ڴ���maple����.
    __u32          curEventContentLen;   //content�ĳ���, ������\0.��һ����󶼻����\0
    __u8           *nextEventContent;  //event���������,�ڴ���maple����.
    __u32          nextEventContentLen;   //content�ĳ���, ������\0.��һ����󶼻����\0

    __u32          nParentalRating; //event�ĵȼ�, ȡֵ0x1~0xf. 0��ʾδ����,�޵ȼ�.0x10�Ժ���broadcaster�Լ�����ĵȼ�.
    __u32          nFollowingParentalRating;    //��һ��event�ĵȼ�.
    __u32          nReserved[4];
}maple_sepg_t;



//***********************************************************************//
//********************** define of EPG Schedule *************************//
//***********************************************************************//

//* information of one schedule event
typedef struct MAPLE_SCHEDULE_EVENT
{   
    //* date at which the event will be broadcast
    maple_time_t   startTime;
    maple_time_t   endTime;

    //* name of the event
    __u8           name[MAPLE_MAX_NAME_LEN];
    __u8    *eventContent;  //event���������,�ڴ���maple����.
    __u32   eventContentLen;   //content�ĳ���, ������\0.��һ����󶼻����\0
    
    __u32   reserved[4];        //Ԥ��
}maple_schdl_event_t;


//* schedule list
typedef struct MAPLE_SCHEDULE_LIST
{
    __s32                 eventNum;       //* how many events totally in the list
    maple_schdl_event_t  *pSchdlEvents;   //* schedule event list
}maple_schdl_list_t;
/*******************************************************************
*define of DATA SERVICE  //only for CMMB
*
*****************************************************************************/
//*information of data service
typedef struct MAPLE_DATA_SERVICE
{
    __s32 data_update;
	__u32 SvcHdl;
}maple_data_service_t;


//* subtitle 
//
//
//typedef enum MAPLE_SUBTITLE_COLOR_MODE
//{
//    MAPLE_SUB_COLOR_BLACK       = 0x80,    
//    MAPLE_SUB_COLOR_RED         = 0x81,  
//    MAPLE_SUB_COLOR_GREEN       = 0x82,  
//    MAPLE_SUB_COLOR_YELLOW      = 0x83,  
//    MAPLE_SUB_COLOR_BLUE        = 0x84,  
//    MAPLE_SUB_COLOR_MAGENTA     = 0x85,  
//    MAPLE_SUB_COLOR_CYAN        = 0x86,  
//    MAPLE_SUB_COLOR_WHITE       = 0x87,  
//
//    MAPLE_SUB_COLOR_
//}maple_subtitle_color_mode_t;
//
//typedef enum MAPLE_SUBTITLE_STYLE_SIZE_MODE
//{
//    MAPLE_SUB_STYLE_SMALL_SIZE        = 0x88,  
//    MAPLE_SUB_STYLE_MIDDLE_SIZE       = 0x89,    
//    MAPLE_SUB_STYLE_NORMAL_SIZE       = 0x8a,    
//    
//    MAPLE_SUB_STYLE_SIZE
//}maple_subtitle_style_size_t;
//
//
//typedef enum MAPLE_SUBTITLE_DISPLAY_MODE
//{
//    MAPLE_SUB_DISPLAY_MODE_HD_SIDE            = 0x01,    
//    MAPLE_SUB_DISPLAY_MODE_SD_SIDE            = 0x02,    
//    MAPLE_SUB_DISPLAY_MODE_SD_WIDE_SIDE       = 0x03,    
//    MAPLE_SUB_DISPLAY_MODE_MOBILE             = 0x04,   
//
//    MAPLE_SUB_DISPLAY_MODE_UNKNOW,  
//    MAPLE_SUB_DISPLAY_MODE_
//}maple_subtitle_display_mode_t;

/*
//typedef struct __MAPLE_ISDB_ITEM __maple_isdb_item_t;


typedef struct __MAPLE_ISDB_ITEM
{
    __u8        isFull;   
    __s8        subNum;
    
    __s8        position_flags;       //
    __s8        Clear_screen_flags;   // 1: clear screen��   0: showdraw;  
    
    __u32       uPts;           // time stamp of the subtitle item, (ms)
    __u32       uDuration;      // duration of the item, (ms)
    __u16       uDataType;      // subtitle data type, text,bitmap,encode type,��Ļ������                            
                        
    maple_subtitle_display_mode_t       displayMode;        //HD; SD; SD Wide; Mobile;
    maple_subtitle_color_mode_t         subColor[5];        //color;   
    maple_subtitle_style_size_t         subStyleSize[5];    //style size;
        
    __u16               subSize;
    __u16               subWidth;                           
    __u16               subHeight;
    __u16               startX;    
    __u16               startY;

    __s16               line[5];
    __s16               forward[5];
    __u16               videoWidth;
    __u16               videoHeight;
    
    __s32               subDataLen[5];
    __u8*               subData[5];
    RECT                subClearScreen[5];    //subtitle clear screen;

    __s32                  uSubSize;       // size of the subtitle data
    __u8                   *pSubtitle;
    __u32                  reserve[32];

} __maple_isdb_item_t;

*/

typedef enum tag_SubtitleDrawMode
{
    SUBTITLE_DRAWMODE_ISDB = 0, //isdb�Ļ���Ļ��ʽ
    SUBTITLE_DRAWMODE_BITMAP,   //sub��pgs��ͼ����Ļ�Ļ���Ļ��ʽ
    SUBTITLE_DRAWMODE_,
}SubtitleDrawMode;


typedef struct tag_CSubtitleDrawModeISDB
{
    __u8        isFull;   //1:������; 0:��֡
    __s8        subNum; //��Ļ������, <=10
    
    __s8        position_flags;       //1:��λ����Ϣ; 0:�������.
    __s8        Clear_screen_flags;   //1: clear screen��   0: showdraw;  ����Ļʱ����app�廹�ǻ�.
    
    __u32       uPts;           // time stamp of the subtitle item, (ms)
    __u32       uDuration;      // duration of the item, (ms)
    __u16       uDataType;      // subtitle data type, text,bitmap,encode type,��Ļ������,__cedar_subtitle_encode_t, CEDAR_SUB_ENCODE_UTF8

    maple_subtitle_display_mode_t       displayMode;        //HD; SD; SD Wide; Mobile;
    maple_subtitle_color_mode_t         subColor[ISDBDEC_MAX_LINE_NUM];        //color;   
    maple_subtitle_style_size_t         subStyleSize[ISDBDEC_MAX_LINE_NUM];    //style size;
        
    __s16               subSize;        //һ���ֵĿ��, 16����16*16; 36����36*36. Ĭ����36������
    __s16               subWidth;       //����Ļ��������Ŀ�, ISDB����ó�
    __s16               subHeight;      //����Ļ��������ĸ�
    __s16               startX;         //����Ļ���������λ��,�������Ļ��ʽ�Լ��ٶ�����Ƶͼ�����������Ͻ�. videoWidth, videoHeight, �����Ļ����������
    __s16               startY;

    //��Ļ�����򻮷�Ϊ������,�и���maple_subtitle_style_size_t����,large = 60������.
    __s16               line[ISDBDEC_MAX_LINE_NUM];       //��Ļ��ÿ��������Ļ������ĵڼ��п�ʼ,��Ļ�б�Ŵ�0��ʼ��
    __s16               forward[ISDBDEC_MAX_LINE_NUM];    //��Ļ��ÿ���֣�ǰ��ռ�����,0��ʾ����,1��ʾ��1����.Ĭ�ϴ��ֵĿ�Ϊ40������,С��20������.
    __s16               videoWidth;     // ISDB��Ļ��ʽ�ٶ�����Ƶͼ��Ŀ��, ��maple_subtitle_display_mode_t����HD:960*540; SD:720*480;
    __s16               videoHeight;
    
    __s32               subDataLen[ISDBDEC_MAX_LINE_NUM]; //
    __u8*               subData[ISDBDEC_MAX_LINE_NUM];    //��Ļ��һ��,���ַ�������pSubtitle��ʼλ��
    RECT                subClearScreen[ISDBDEC_MAX_LINE_NUM];    //subtitle clear screen;

    RECT                subImageRect[DVBSUBDEC_MAX_LINE_NUM];    //DVB-T Image display restangle

    __s32                  uSubSize;       // size of the subtitle data
    __u8                   *pSubtitle;
}CSubtitleDrawModeISDB;

typedef struct tag_CSubtitleDrawItem
{
    SubtitleDrawMode   nDrawMode;
    union
    {
        CSubtitleDrawModeISDB   isdb;
        __s32                   nReserved[1024]; //�������������������unionDrawMode��4096�ֽ�,�������������չ����drawmode�Ŀռ䶼������
    }unionDrawMode;

} CSubtitleDrawItem;  //__maple_isdb_item_t


typedef enum tag_SortServListMode
{
    SORTMODE_REMOTE_CONTROL_KEY_ID = 0, //��remote_control_key_idΪ�ο���������, isdbʹ��.
    SORTMODE_,
}SortServListMode;

//maple status������cedar��ֵ����һ��
typedef enum __MAPLE_STATUS             /* the status os cedar                  */
{
    MAPLE_STAT_PLAY=0,                  /* play                                 */
    MAPLE_STAT_PAUSE,                   /* pause                                */
    MAPLE_STAT_STOP,                    /* stop                                 */
    //MAPLE_STAT_FF,                      /* fast forward                         */
    //MAPLE_STAT_RR,                      /* fast backward                        */
    //MAPLE_STAT_JUMP,                    /* jump                                 */

    MAPLE_STAT_RECORD = 0x20,   //¼�ƽ�����,
    MAPLE_STAT_
} __maple_status_t;

typedef enum MAPLE_RECORD_CB_STATUS
{
    MAPLE_REC_STAT_OK = 0,
    MAPLE_REC_STAT_NO_DISK,                     /*û�д���                                */
    MAPLE_REC_STAT_DISK_SPACE_NOT_ENOUGH,       /*����ʣ��ռ�̫С                        */
    MAPLE_REC_STAT_DISK_FULL,                   /*�����Ѿ���                              */
    MAPLE_REC_STAT_ABOVE_MAX,                   /*¼�Ƶ��ļ��ﵽ���趨�����ֵ            */
    MAPLE_REC_STAT_OPEN_FAIL,                   /*����¼���ļ�ʧ��                        */
    MAPLE_REC_STAT_WRITE_FAIL,                  /*д¼������ʧ��                          */
    MAPLE_REC_STAT_
}maple_recCb_stat_t;

//***********************************************************************//
//************ define IO interface of TV MiddleWare module **************//
//***********************************************************************//

//* all the IO command interface of maple return EPDK_OK or EPDK_FAIL;
//* EPDK_FAIL means the implementattion fail.
//* EPDK_OK   means the implementation success and the return result can be used.

typedef enum MAPLE_CMD
{
    //* demod/tuner implement
        //* tune to a specific frequency
        MAPLE_CMD_TUNE                = 0x0,    //* aux = bandwidth_in_10KHz<<22 | frequency_in_KHz, pbuffer = NULL;
        
        //* get current signal status
        MAPLE_CMD_GET_SS,                       //* aux = , pbuffer = maple_demod_ss_t*;
        
        MAPLE_CMD_GET_LOCK_STATUS,              // ��ѯ��ǰ�Ƿ�������Ƶ��, ret = !0:lock; ret=0:unlock;
        
    //* service list implement
        //* open file to read service list
        MAPLE_CMD_SL_OPEN             = 0x10,   //* aux = , pbuffer = list_file_path;����·����service�б��ļ�,����maple�Լ����б�.Ȼ��ر��ļ����˳�.

        //* save the current service list to file
        MAPLE_CMD_SL_SAVE,                     //* aux = , pbuffer = list_file_path;maple���Լ����б�д���ƶ����ļ���.

        //* get service list of the TV middleware
        MAPLE_CMD_SL_GET,                      //* aux = maple_serv_group_type_e, pbuffer = pointer to maple_serv_list_t;��maple��service�б��л�ȡservice

        //* add service list to the TV middleware
        MAPLE_CMD_SL_ADD,                      //* aux = , pbuffer = pointer to maple_serv_list_t;����maple�Լ���service�б�.

        //* delete the service list of the TV middleware
        MAPLE_CMD_SL_DELETE,                   //* aux = , pbuffer = ;maple����Լ���service�б�.
        
        //* add service item
        MAPLE_CMD_SERV_ITEM_ADD,               //* aux = , pbuffer = pointer to service item, maple_serv_item_t*;
        
        //* delete service item,
        MAPLE_CMD_SERV_ITEM_DELETE,           //* aux = item channel id, pbuffer = ;
        
        //* set service item to be favourate
        MAPLE_CMD_SERV_ITEM_SET_FAVOURATE,    //* aux = item channel id, pbuffer = 0 or 1 to clear or set favourate flag;
        
        //* set service item to be lock
        MAPLE_CMD_SERV_ITEM_SET_LOCK,         //* aux = item channel id, pbuffer = key, (key == -1 means unlock the specified service);
        
        MAPLE_CMD_SL_GET_SERV_NUM,            //* aux = 0, pbuffer = NULL, this command return the service number of the service list.
        
        MAPLE_CMD_SL_GET_CUR_SERV_CHAN_ID,    //* aux = 0, pbuffer = NULL, this command return the current service's channel id.

        MAPLE_CMD_SORT_SERV_LIST,          //* ��MAPLE���ݿ��е�servicelist����������, aux = ����ʽ SortServListMode. 0:ʹ��remote control key id.����isdb��SI�����һ�ַ�ʽ.

    //* callback function implement
        //* set a callback function
        MAPLE_CMD_CB_SET              = 0x20,  //* aux = callback type, maple_cb_type_e, pbuffer = callback function, __pCBK_t;
        //* set parameter to be passed when callback
        MAPLE_CMD_CB_SET_PARAM,                //* aux = callback type, maple_cb_type_e, pbuffer = callback param;
        
    //* set default charactor coding type
        MAPLE_CMD_SET_DEFAULT_CHAR_CODING_TYPE = 0x28,  //* aux = value of __epdk_charset_enm_e, such as EPDK_CHARSET_ENM_GB2312,
                                                        //* pbuffer = NULL.

    //* service searching
        //* start searching service
        MAPLE_CMD_SRCH_SERV           = 0x30,  //* aux = bandwidth_in_10KHz<<22 | frequency_in_KHz, 
                                               //* pbuffer = pointer to maple_serv_list_t;
        //* break service searching
        MAPLE_CMD_SRCH_SERV_STOP,              //* 

    //* simple EPG searching
        //* start simple EPG searching
        MAPLE_CMD_SRCH_SEPG           = 0x40,  //* aux = service channel id, pbuffer = pointer to maple_sepg_t;����
        //* break simple EPG searching
        MAPLE_CMD_SRCH_SEPG_STOP,               // ����

    //* schedule searching
        //* start schedule searching
        MAPLE_CMD_SRCH_SCHDL          = 0x50,  //* ����, ��MAPLE_CMD_EPG_MONITOR_START�����aux = service channel id, pbuffer = pointer to maple_schdl_list_t
        //* break schedule searching
        MAPLE_CMD_SRCH_SCHDL_STOP,             //* ����.
        
    //* tv epg monitor
        //* start epg monitor
        MAPLE_CMD_EPG_MONITOR_START   = 0x58,  //* it will create a task to receive EIT sections, aux = 0, pbuffer = NULL.
        //* stop epg monitor
        MAPLE_CMD_EPG_MONITOR_STOP,            //* it will break the EIT monitor task, aux = 0, pbuffer = NULL.
        //* get simple epg of specified service
        MAPLE_CMD_GET_SEPG,                    //* aux = service channel id, pbuffer = pointer to maple_sepg_t.
        //* get schedule list
        MAPLE_CMD_GET_SCHEDULE,                //* aux = service channel id, pbuffer = pointer to maple_schdl_list_t.

    //* tv time monitor
        //* start tv time monitor
        MAPLE_CMD_TIME_MONITOR_START  = 0x60,  //* it will create a task to receive TDT from stream.
        //* stop tv time monitor
        MAPLE_CMD_TIME_MONITOR_STOP,           //* it will break the TDT monitor task.
        //* get the current stream time
        MAPLE_CMD_TIME_GET,                    //* aux = , pbuffer = pointer to maple_time_t;

    //* service play implement
        MAPLE_CMD_PLAY                = 0x70,  //* aux = service index in sl, channelId, pbuffer = audioIndex;���������,ָʾһ�����ŵ�����.
        MAPLE_CMD_STOP,
        MAPLE_CMD_SET_PREVIEW_MODE,            //* ����. aux = 1 or 0, 0 - close preview mode, 1 - open preview mode. 
        MAPLE_CMD_GET_STATUS,                   //ret = __maple_status_t,��ȡ���ŵ�״̬.

        MAPLE_CMD_SET_RECORD_MEDIAFILE,    // * ����¼�Ƶ�ý���ļ�·��, pbuffer = CedarHerbMediaFileInf*; ����:e:\\dtv_service0.ts, �����߷����ڴ�.
        MAPLE_CMD_RECORD,       //aux = channelId
        MAPLE_CMD_STOP_RECORD,
        MAPLE_CMD_GET_RECORD_STATUS,        //��ȡ¼�Ƶ�״̬,ret = __maple_status_t, MAPLE_STAT_RECORD, MAPLE_STAT_PAUSE, MAPLE_STAT_STOP 3��.
        MAPLE_CMD_SET_RECORD_CB,//�����ռ����ص�����
        
    //* graphic implement
        //* set a graphic layer to play video
        MAPLE_CMD_SET_VID_LAYER       = 0x80,   // pbuffer = (__s32) video_layer_hdl.
        //* set video window
        MAPLE_CMD_SET_VID_WINDOW,
        //* get the current video window,
        MAPLE_CMD_GET_VID_WINDOW,       //ret = __rect_t*
        //* set video picture height and width ratio
        //* ������Ƶ���Ż������, ������Ƶͼ����ʾ��ģʽ��aux = maple_vd_win_ratio_mode_e, return = ���ú�ĵ�ǰ��ʾģʽ, 
        //* ��� aux = MAPLE_VID_WINDOW_USERDEF, pbuffer = __pCBK_t, ����������SrcScnWinPara, �ο�[mod_cedar.h]
        MAPLE_CMD_SET_VID_SHOW_MODE,
        //* get video picture height and width ratio
        MAPLE_CMD_GET_VID_SHOW_MODE,
        //* set video alpha blending value, this value is from 0 to 255
        MAPLE_CMD_SET_VIDEO_LYR_ALPHA_VALUE,
        //* open video graphic layer alpha blending
        MAPLE_CMD_SET_VIDEO_LYR_ALPHA_ON,
        //* close video graphic layer alpha blending
        MAPLE_CMD_SET_VIDEO_LYR_ALPHA_OFF,

        MAPLE_CMD_CONFIG_VIDEO_LYR_CK_ENABLE,   //�������ø�maple����Ƶͼ���ck_enable������ֵ, aux = 1:enable; aux = 0:disable

        MAPLE_CMD_ENABLE_DEINTERLACE,                   //�����Ƿ��deinterlace����. aux = 2:enable level2,Ч�����,����ռ����� ;aux = 1: enable level1,Ч��һ��,����ռ���Դ�; aux = 0: disable, Ĭ��disable, ��play֮ǰ����.��maple�����ڼ�,һֱ��Ч.

        MAPLE_CMD_GET_FRAMESIZE,            /* frame size       return = (width << 16) + height             */

				MAPLE_CMD_GET_VE_STATUS,   				//��ȡ����������״̬���Ƿ�ɹ��ı�ǵȣ����ص���һ���ṹ��MAPLE_VE_STATUS���׵�ַ    =  *(MAPLE_VE_STATUS *)result

				MAPLE_CMD_SET_ROTATE,               /* ���ý���ͼ����ת�ĽǶ�.aux = 0:Ĭ��Ϊԭͼ,1:˳ʱ��90��,2:180��,3:270��,4:ˮƽ����,5:��ֱ����,ע������:ֻ�������ļ�����֮ǰ���ã����Ź�����(�����������)������Ч*/
				
    		MAPLE_CMD_CHECK_PICTURE_OUT,       //ret = EPDK_TRUE,��ʾ.��������Ƶʱ����ѯ��һ֡ͼ���Ƿ��Ѿ���ʾ����;�������Ƶ,�򷵻�ֵʼ��ΪEPDK_FALSE.

    //* CA implement
        MAPLE_CMD_CA_GET_SN = 0x90,                    //* aux = buffer size, pbuffer = buffer to store ca serial number. aux������. pbuffer = maple_cas_param_t

        //*set call back function for sply              
        MAPLE_CMD_SET_SPLY_DRAW_CB = 0xa0,   //*set call back function for drawing subtitle;
        MAPLE_CMD_GET_SUBTITLE_CNT,         // aux = service index in sl, channelId. ��ȡָ����service����Ļ����������ret = ����,ret=-1�������
        MAPLE_CMD_SET_SUBTITLE_LANG,         //aux = service index in sl, channelId. pbuffer = current subtitle index
        MAPLE_CMD_GET_SUBTITLE_LANG,         //aux = service index in sl, channelId.

        MAPLE_CMD_SET_SWGAIN = 0xc0,        //audio���,���������������,��ǿ����, -20~20db֮��.
        MAPLE_CMD_GET_SWGAIN,
	 
    		MAPLE_CMD_ENABLE_REMOTE_CONTRL_KEY,//�Ƿ����Ƶ�����ڲ���remote_contrl_key,Ĭ��1:�򿪣�remotecontrlkeyID������256��̨��0������Զ���Ŵ�350��ʼС��350��̨

				MAPLE_CMD_SET_RESERVED_MEM_SIZE,    //����Ԥ�����ڴ�����,�����߽��������õ��ڴ�����ʱ,���ⲿ���ڴ�Ԥ������.
    		MAPLE_CMD_SET_TV_MODE,							//����1:TV MODE 0:����ý�岥��

				MAPLE_CMD_GET_SERVICE_INFO=0xd0,  //��ȡ�Ƿ��޽�Ŀ���������źŵ��޽�Ŀʱ��Ҫ���Բ��Ź���Ƶ���������ź�������ֿ����������һ��play���ܿ�ʼ��ȡ������ֵ0:δ֪״̬��1:������Ƶ���ݣ�2:������Ƶ���ݣ�3:ֻ����Ƶ����, 4:ֻ����Ƶ���ݵ���ʽδ֪��5:ֻ����Ƶ���ݣ�6:ֻ����Ƶ���ݵ���ʽδ֪

				MAPLE_CMD_GET_SERVICE_STATUS,  //��ȡ��Ȩ��ʶ������0: ������1:δ��Ȩ��2:����Ȩ

				MAPLE_CMD_UPDATE_SERVICE_INFO, //����1��ʾ�б仯������0�ޱ仯
				
				MAPLE_CMD_SEL_AUDSTREAM,
				
				MAPLE_CMD_GET_AUDSTREAM,

				MAPLE_CMD_SET_SWITCH_VPLY_MODE,

				MAPLE_CMD_SET_STOP_MODE,
		
				MAPLE_CMD_

}maple_cmd_e;


//* video show mode definition
typedef enum MAPLE_VIDEO_WINDOW_RATIO_MODE
{
    MAPLE_VID_WINDOW_NOTCARE = 0x00,       //* ������ͼƬ��ʾ�������Ե�ǰ���õı���
    MAPLE_VID_WINDOW_ORIGINAL,             //* ��ͼƬԭʼ��С�ڴ�������ʾ�������������
    MAPLE_VID_WINDOW_BESTSHOW,             //* ��ͼƬ����ı�����������������ʾ��ͼƬ������
    MAPLE_VID_WINDOW_FULLSCN,              //* �Դ��ڵı�������ͼƬ����������ʾ�����ܻ����
    MAPLE_VID_WINDOW_4R3MODE,              //* ǿ����4:3��ģʽ������ʾ��Ƶͼ��ͼ������
    MAPLE_VID_WINDOW_16R9MODE,             //* ǿ����16:9��ģʽ������ʾ��Ƶͼ��ͼ������
    MAPLE_VID_WINDOW_CUTEDGE,               /* ͼƬ�����кڱߣ���õ�                       */
    MAPLE_VID_WINDOW_USERDEF,               //�û��Զ���,�Լ�ȷ��srcwin, scnwin�Ĳ���,��Ҫcallback����

    MAPLE_VID_WINDOW_
}maple_vd_win_ratio_mode_e;


//* When we tune or search service(using the MAPLE_CMD_SRCH_SERV or MAPLE_CMD_TUNE io command), 
//* application should tell the TV MiddleWare module the frequency and bandwidth.
//* TV MiddleWare recognizes frequency and bandwidth through the 'aux' parameter passed at
//* MAPLE_SRCH_SERV IO command or MAPLE_CMD_TUNE io command, and the format of 'aux' is
//* (bandwidth_in_10KHz<<22 | frequency_in_KHz), the low 22 bits for frequency and the high 10 bits for bandwidth.
//* The frequency is unit of KHz and the bandwidth is in unit of 10KHz.
//* For example, when we tune to frequency point 722000000Hz(722000KHz) width bandwidth value equals
//* to 8000000Hz(8MHz), we should set the aux parameter to be ((MAPLE_TV_BANDWIDTH_8M/10)<<22 | 722000).
//* 
typedef enum MAPLE_TV_BANDWIDTH
{
    MAPLE_TV_BANDWIDTH_8M       = 8000,     //* 8000KHz, 8MHz
    MAPLE_TV_BANDWIDTH_7M       = 7000,     //* 7000KHz, 7MHz
    MAPLE_TV_BANDWIDTH_6M       = 6000,     //* 6000KHz, 6MHz
    MAPLE_TV_BANDWIDTH_5M       = 5000,     //* 5000KHz, 5MHz
    MAPLE_TV_BANDWIDTH_2M       = 2000,     //* 2000KHz, 2MHz
    MAPLE_TV_BANDWIDTH_1_5M     = 1500,     //* 1500KHz, 1.5MHz
    MAPLE_TV_BANDWIDTH_1_SEG    = 430,      //* 430KHz,  0.43MHz
    MAPLE_TV_BANDWIDTH_3_SEG    = 1290,     //* 1290KHz, 1.29MHz
    MAPLE_TV_BANDWIDTH_FULL_SEG = 5590      //* 5590KHz, 5.59MHz
}maple_tv_bandwidth_e;


//* Application may need notification when some event happens, this is implemented by
//* callback functions. Application set callback functions for different type of 
//* events. When one event happens and the relative callback function is specified,
//* TV MiddleWare will call the callback function to notify application.
//* When setting callback function, application should tell the callback type for
//* different type of events defined below.
typedef enum MAPLE_CALLBACK_TYPE
{
    MAPLE_CB_SRCH_SL_EVENT    = 0,       //* callback when one service item is searched.
    MAPLE_CB_SRCH_SL_FINISH,             //* callback when all service items are searched.

    MAPLE_CB_SRCH_SEPG_FINISH = 0x10,    //* callback when simple EPG search is finish.

    MAPLE_CB_SRCH_SCHDL_EVENT = 0x20,    //* callback when one schedule.
    MAPLE_CB_SRCH_SCHDL_FINISH,          //* callback when all schedule is search.
    
    MAPLE_CB_SIGNAL_LOCK_EVENT = 0x30,   //* callback function of signal locking status.
  
    MAPLE_CB_CMMB_DATASVC_BEGIN =0x40,   //only for cmmb 
    MAPLE_CB_CMMB_DATASVC_END,					////only for cmmb 

}maple_cb_type_e;


//* different callback type use different type of parameter defined below
//* service search callback param type.
typedef struct MAPLE_CB_PARAM_SL 
{
    void                 *pOpaque;
    maple_serv_item_t     serv_item;
}maple_sl_cb_param_t;

//* sepg search callback param type.
typedef struct MAPLE_CB_PARAM_SEPG
{
    void                 *pOpaque;
    maple_sepg_t          sepg;
}maple_sepg_cb_param_t;

//* schedule search callback param type.
typedef struct MAPLE_CB_PARAM_SCHEDULE
{
    void                 *pOpaque;
    maple_schdl_event_t   event;
}maple_schdl_cb_param_t;


typedef struct MAPLE_DEMOD_SIGNAL_STATUS
{
    __u8 strength;
    __u8 quality;
}maple_demod_ss_t;


typedef struct MAPLE_DEMOD_LOCK_CB_PARAM
{
    void*               pOpaque;
    __u8                lockOk;
    maple_demod_ss_t    signalStatus;
}maple_lock_param_t;

typedef struct MAPLE_DEMOD_CAS_PARAM
{
    __u8                casFlag;/*1-- have card, 0--no card*/
    __u8*               casNum; //app malloc memory
    __s32               ncasNumSize;    //casNum size
    __s32               ncasNumValidSize; //mapleʹ�õĳ���, maple������д, <= ncasNumSize
}maple_cas_param_t;

typedef struct SmsDataSvcFatUpdateInfo_S
{
	__u32 FatMinorVersion;         // Minor version of the FAT taken from the FAT XML.
	__u32 FatMajorVersion;			// Major version of the FAT taken from the FAT XML.
	__u32 FatUpdateNum;			// Update number (0-31) of the FAT. 
	__u32 NumFiles;				// Number of files in this update
	const char** pFilePathsArr;		// Array of strings containing the paths of the files
									// included in this update
	__u32 Reserved[4];				// Reserved for future use
} SmsDataSvcFatUpdateInfo_ST;		 

typedef struct MAPLE_CB_PARAM_CMMB_DATASVC 
{
    void                 *pOpaque;
    SmsDataSvcFatUpdateInfo_ST     FatUpdateInfo;
    
}maple_cmmb_datasvc_cb_param_t;

//#endif
	typedef struct MAPLE_VE_STATUS
	{
		u8 ve_errcnt; //���ve ��������Ϊ0��Ϊ5ʱ�ļ���
		u8 ve_succnt; //���ve ��������Ϊ0�Ҳ�Ϊ5ʱ�ļ���
		u8 ve_errflg; // ���ve ��������Ϊ0������50��Ϊ5ʱ��1
		u8 ve_tmpcnt; //���VE RELEASE BUFFER ״̬�ļ���

	}maple_ve_status_e;

#define HIGHCHARACTER  1
#define XINWU 2
#define CHESHIJIE 3
#define XINWU_CMMB_OLD 4
#define LEAISHI 5

#define CUSTOMER  HIGHCHARACTER


#endif

