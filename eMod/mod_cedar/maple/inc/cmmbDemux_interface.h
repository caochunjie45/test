
#ifndef _CMMB_DEMUX_INTERFACE_H
#define _CMMB_DEMUX_INTERFACE_H

//#include "maple_common.h"

#define CMMB_DEMUX_PLUGIN_FILE_PATH  CEDAR_PLUGINS_DIRECTORY"demux.mod"

#define CMMB_MAX_DMX_CHAN_NUM           16
#define CMMB_DEMUX_ANY_SUBFRAME_INDEX   (0xff)

typedef enum CMMB_DEMUX_TYPE
{
    CMMB_DMX_TYPE_CONTROL_INFO,     //* channel for cmmb control frame.
    CMMB_DMX_TYPE_CONTROL_INFO_EXT, //* channel for cmmb control frame, process frame only when the control frame has updated information
    CMMB_DMX_TYPE_SERVICE_INFO,     //AV data siano1184 //* channel for cmmb subframe header, some service information, such as video code type, audio code type, maybe need.
    CMMB_DMX_TYPE_MEDIA,            //* channel for video and audio section processing.
    CMMB_DMX_TYPE_DATA,             //ESG	//* channel for cmmb subframe data section processing.   
    CMMB_DMX_TYPE_PROGRM,          //AV data   	//*IF206 for AV service
    CMMB_DMX_TYPE_NUM
    
}cmmb_dmx_type_t;

typedef struct CMMB_DMX_CHANNEL_PARAM
{
    cmmb_dmx_type_t type;
                    
    __u32           serviceId;              //* service id.
    __u32           subFrameId;             //* subframe id of the specified service.
                    
    __u32           startTimeSlot;          //* start time slot of the specified multiplex frame.
    __u32           timeSlotCnt;            //* time slot number of the multiplex frame.
    __u8            demodArg;             	//only for if206 
		__u8            muxfrmId;								//only for tp2313
		__u8            rsRate;									//only for tp2313
		__u8            byteInterMode;					//only for tp2313
		__u8            ldpcRate;								//only for tp2313
		__u8            modulateMode;						//only for tp2313
		__u8            scrambleMode;						//only for tp2313
    __pCBK_t        vd_request_buffer;      //* call by demux when the media pasing thread need buffer for video data.
    __pCBK_t        vd_update_data;         //* call by demux when the media pasing thread have fill some data in video buffer.
    __pCBK_t        ad_request_buffer;      //* call by demux when the media pasing thread need buffer for audio data.
    __pCBK_t        ad_update_data;         //* call by demux when the media pasing thread have fill some data in audio buffer.
    __pCBK_t        filterCallback;         //* call by demux when there is some data for outside program.
    void*           filterCallbackParam;    //* this param is set to the 'opaque' field of the cmmb_chan_cb_param and passed to
                                            //* outside program when 'filterCallback' is called, the demux do not care what it is.
}cmmb_dmx_chan_param_t;

typedef enum CMMB_MSG_TYPE
{
    CMMB_MSG_FRM_HDR_CB,            //* multiplex frame header decoded callback.
    CMMB_MSG_CTRL_TAB_CB,           //* control tables callback.
    CMMB_MSG_SUB_FRM_HDR_CB,        //* subframe header decoded callback.
    CMMB_MSG_SECTION_HDR_CB,        //* section header decoded callback.
    CMMB_MSG_UNIT_CB,               //* data unit callback.
    CMMB_MSG_MPX_BLK_CB,            //* multiplex block callback
    CMMB_MSG_DATA_XPE_CB,           //* xpe data unit callback.
    CMMB_MSG_DATA_XPEFEC_CB,        //* xpe fec data unit callback.
    CMMB_MSG_NUM
}cmmb_dmx_msg_type_t;

typedef struct CMMB_DMX_CHAN_CALLBACK_PARAM
{
    cmmb_dmx_msg_type_t type;
    __u32               serviceId;
    void*               opaque;
    void*               params[5];
}cmmb_dmx_chan_cb_param_t;


//解密相关数据结构
//libcmmb传递下来的ca table数据结构
typedef struct CMMBDMX_CAT
{
    __u8*               data;
    __u16               datalen;
   	__u16								servid;
		__u8								isFindCASConfigure;
}cmmbdmx_cat_t;
//libcmmb拿到解密servid后解析了相关参数然后设置打开解密功能
typedef struct CMMBDMX_CA_PARAM
{
   
   	__u16						servid;
		__u32           subFrameId;             //* subframe id of the specified service.                    
    __u32           startTimeSlot;          //* start time slot of the specified multiplex frame.
    __u32           timeSlotCnt;            //* time slot number of the multiplex frame.
    __u8            demodArg; 
		
}cmmbdmx_ca_param_t;

//* variable of this struct type cmmb_dmx_chan_cb_param_t is passed to outside program 
//* through filterCallback function set in cmmb_dmx_chan_param_t when opening a demux .
//* channel different message type has different params.

//* for type = CMMB_MSG_FRM_HDR_CB
//*     params[0] = (__u8*)buf                          (data address of a multiplex frame);
//*     params[1] = (__u32)bufSize                      (data size);
//*     params[2] = (cmmb_multiplex_frm_hdr_t*) pMfHdr  (pointer to a cmmb_multiplex_frm_hdr_t decoded);

//* for type = CMMB_MSG_CTRL_TAB_CB
//*     params[0] = (__u8*)buf                          (data address of a control table);
//*     params[1] = (__u32)bufSize                      (data size);
//*     params[2] = (__u32)subFrmIdx                    (subframe index of the control table);

//* for type = CMMB_MSG_SUB_FRM_HDR_CB
//*     params[0] = (__u8*)buf                          (data address of a subframe);
//*     params[1] = (__u32)bufSize                      (data size);
//*     params[2] = (__u32)subFrmIdx                    (subframe index of the subframe header);
//*     params[3] = (cmmb_subfrm_hdr_t*)pSubFrmHdr      (pointer to a cmmb_subfrm_hdr_t decoded);

//* for type = CMMB_MSG_SECTION_HDR_CB
//*     params[0] = (__u8*)buf                          (data address of a video/audio/data section);
//*     params[1] = (__u32)bufSize                      (data size);
//*     params[2] = (__u32)subFrmIdx                    (subframe index of the the section);
//*     params[3] = (cmmb_section_hdr_t*)pSecHdr        (pointer to a cmmb_section_hdr_t decoded);
//*     params[4] = (cmmb_section_type_t)type           (the section type);

//* for type = CMMB_MSG_UNIT_CB
//*     params[0] = (__u8*)buf                          (data address of a video/audio/data unit);
//*     params[1] = (__u32)bufSize                      (data size);
//*     params[2] = (__u32)subFrmIdx                    (subframe index of the the unit);
//*     params[3] = (cmmb_unit_params_t*)pUnitParams    (pointer to a cmmb_unit_params_t decoded);
//*     params[4] = (cmmb_section_type_t)type           (section type of the section which contains this unit);

//* for type = CMMB_MSG_MPX_BLK_CB
//*     params[0] = (__u8*)buf                          (data address of a video/audio/data unit);
//*     params[1] = (__u32)bufSize                      (data size);
//*     params[2] = (__u32)subFrmIdx                    (subframe index of the the unit);
//*     params[3] = (cmmb_mpx_blk_hdr_t*)pMpxBlkHdr     (pointer to a cmmb_mpx_blk_hdr_t decoded);

//* for type = CMMB_MSG_DATA_XPE_CB
//*     params[0] = (__u8*)buf                          (data address of a xpe packet);
//*     params[1] = (__u32)bufSize                      (data size);
//*     params[2] = (__u32)subFrmIdx                    (subframe index of the the packet);
//*     params[3] = (cmmb_xpe_pkt_t*)pXpePkt            (pointer to a cmmb_xpe_pkt_t decoded);

//* for type = CMMB_MSG_DATA_XPEFEC_CB
//*     params[0] = (__u8*)buf                          (data address of a xpe fec packet);
//*     params[1] = (__u32)bufSize                      (data size);
//*     params[2] = (__u32)subFrmIdx                    (subframe index of the the packet);
//*     params[3] = (cmmb_xpefec_pkt_t*)pXpePkt         (pointer to a cmmb_xpefec_pkt_t decoded);


typedef enum CMMB_DMX_CMD
{
    CMMB_DMX_CMD_OPEN_CHAN,         //* open one channel to get video data/audio data/epg data/service information.
                                    //* aux = 0, pbuffer = pointer to cmmb_dmx_chan_param_t;
                                    //* return value: channel id of the opened chan.
                                    
    CMMB_DMX_CMD_CLOSE_CHAN,        //* close channel.
                                    //* aux = channel id return by CMMB_DMX_CMD_OPEN_CHAN io command calling, pbuffer = NULL;
                                    //* return value: EPDK_OK or EPDK_FAIL.
                                    
    CMMB_DMX_CMD_SET_DRV_HDL,       //* set cmmb driver handle to the demux module.
                                    //* aux = 0, pbuffer = handle of cmmb driver.
		CMMB_DMX_CMD_SET_CA_TO_DRV,			//* for if206  set ca table number to open ca chan .
		
		CMMB_DMX_CMD_SEND_CA_PARAM_TO_DRV,//* for if206  set ca param  to open ca chan .(解密后的数据)

		CMMB_DMX_CMD_GET_SERVICE_STATUS,
		
    CMMB_DMX_CMD_NUM

}cmmb_dmx_cmd_t;


//*---------------------
//* #define some bits used in cmmb standar.
#define CMMB_BIT(a)		(1 << (a))

enum
{
	CMMB_SUB_FRAME_INDICATION_BIT_INIT_BCAST_TIME	= 7,
	CMMB_SUB_FRAME_INDICATION_BIT_VID_SECT_LEN		= 6,
	CMMB_SUB_FRAME_INDICATION_BIT_AUD_SECT_LEN		= 5,
	CMMB_SUB_FRAME_INDICATION_BIT_DATA_SECT_LEN		= 4,
	CMMB_SUB_FRAME_INDICATION_BIT_EXPANSION_AREA	= 3,
	CMMB_SUB_FRAME_INDICATION_BIT_SCRAMBLING0   	= 2,
	CMMB_SUB_FRAME_INDICATION_BIT_SCRAMBLING1   	= 1,
	CMMB_SUB_FRAME_INDICATION_BIT_PACK_MODE			= 0,

	CMMB_VID_EXP_INDICATION_CODE_RATE		= 3,
	CMMB_VID_EXP_INDICATION_PIC_DISP		= 2,
	CMMB_VID_EXP_INDICATION_RESOLUTION		= 1,
	CMMB_VID_EXP_INDICATION_FRAME_FREQ		= 0,

	CMMB_AUD_EXP_INDICATION_CODE_RATE		= 2,
	CMMB_AUD_EXP_INDICATION_SAMPLING_RATE	= 1,
	CMMB_AUD_EXP_INDICATION_STREAM_DESC		= 0,

	CMMB_VID_UNIT_INDICATION_FRAME_END		= 1,
	CMMB_VID_UNIT_INDICATION_REL_BCAST_TIME	= 0,
};

//*---------------------
//* Encryption modes
enum
{
	CMMB_ENCRYPTION_MODE_NOT_ENCRYPTED	= 0,
	CMMB_ENCRYPTION_MODE_DECRYPTED		= 1,
	CMMB_ENCRYPTION_MODE_ENCRYPTED		= 2,
	CMMB_ENCRYPTION_MODE_RESERVED		= 3,
};

//*---------------------
//* Data unit type
typedef enum CMMB_DATA_UNIT_TYPE
{
	CMMB_DATA_UNIT_TYPE_ESG     = 0,
	CMMB_DATA_UNIT_TYPE_XPE     = 160, 
	CMMB_DATA_UNIT_TYPE_XPE_FEC = 161, 
}cmmb_data_unit_type_t; 

enum
{
    CMMB_AUD_ALGORITHM_TYPE_DRA      = 0,
    CMMB_AUD_ALGORITHM_TYPE_AAC_PLUS = 1,
    CMMB_AUD_ALGORITHM_TYPE_AAC      = 2,
    
    CMMB_VID_ALGORITHM_TYPE_AVS      = 0,
    CMMB_VID_ALGORITHM_TYPE_H264     = 1
};


//*****************************************
// Multiplex frame header
typedef struct CMMB_MULTIPLEX_FRAME_HDR
{
    __u8    HeaderLengthField;              //* 8
    __u8    ProtocolVersion;                //* 5
    __u8    MinimumProtocolVersion;         //* 5
    __u8    MultiplexFrameID;               //* 6
    __u8    EmergencyBroadcastIndication;   //* 2
    __u8    NextFrameParameterIndication;   //* 1
    __u8    InitialLead;                    //* 2
    __u8    NIT_SeqNum;                     //* 4
    __u8    CMCT_SeqNum;                    //* 4
    __u8    CSCT_SeqNum;                    //* 4
    __u8    SMCT_SeqNum;                    //* 4
    __u8    SSCT_SeqNum;                    //* 4
    __u8    ESG_SeqNum;                     //* 4
    __u8    CA_SeqNum;                      //* 4 AKA "Update No. of Expansion Controlling Information"
    __u8    NumOfSubFrames;                 //* 4
    __u8    NextFrameParamHeaderLength;     //* 8
    __u8    NextFrameParamMsf1HeaderLength; //* 8
    __u32   SubFrameLength[16];             //* 24*n
    __u32   NextFrameParamMsf1Length;       //* 24
    __u32   TotalFrameLength;               //* Calculated field - not from header
    __u32   RESERVED1;
    __u32   RESERVED2;
}cmmb_multiplex_frm_hdr_t ;


//*****************************************
// Video expansion area in sub frame header
typedef struct CMMB_VID_EXPANSION
{
    __u8    VidType;
    __u8    VidIndications;
            
    __u8    PicDisplayAbscissa;
    __u8    PicDisplayOrdinate;
    __u8    PicDisplayPriority;
    __u8    FrameFreq;
            
    __u16   VideoCodeRate;
    __u16   VidHorizontalResolution;
    __u16   VidVerticalResolution;
    
    __u32   RESERVED1;
    __u32   RESERVED2;
}cmmb_vid_exp_t;

//*****************************************
// Audio expansion area in sub frame header
typedef struct CMMB_AUD_EXPANSION
{
    __u8    AudType;		// According to #SMS_CMMB_ALGORYTHM_TYPE_E
    __u8    Indications;
    __u8    SamplingRate;
    __u16   CodeRate;
    __u32   AudioDesc;
    __u32   RESERVED1;
    __u32   RESERVED2;
}cmmb_aud_exp_t;


#define CMMB_MAX_VID_STRM_NUM_IN_SUBFRM (8)     //* max video stream num in a subframe.
#define CMMB_MAX_AUD_STRM_NUM_IN_SUBFRM (8)     //* max audio stream num in a subframe.
//*****************************************
// Multiplex Sub frame header
typedef struct CMMB_SUBFRAME_HDR
{
    __u8            SubframeIndex;      //* Not from the header in the stream - set by demux
    __u8            HeaderLengthField;
    __u8            Indications;
    __u8            EncryptionMode;     //* According to SmsCmmbEncryptionIndication_EN
    __u8            IsEncapMode1;
    __u8            TotalVidStreams;
    __u8            TotalAudStreams;
    __u32           InitBcastTime;
    __u32           VidSectLen;
    __u32           AudSectLen;
    __u32           DataSectLen;
    __u32           RESERVED1;
    __u32           RESERVED2;

    cmmb_vid_exp_t  VidStreamsDesc[CMMB_MAX_VID_STRM_NUM_IN_SUBFRM];
    cmmb_aud_exp_t  AudStreamsDesc[CMMB_MAX_AUD_STRM_NUM_IN_SUBFRM];
    
}cmmb_subfrm_hdr_t;

//*-------------------------
//* Audio/Video/Data unit parameters
//* Important note - this struct is held 100 times on stack in an array
//* Its size must not be extended.
//* This structure holds the same fields for audio, video and data units,
//* but not all fields are relevant to each type.
//* These parameters are parsed from the section header and delivered
//* by the demux with each unit.
typedef struct CMMB_UNIT_PARAMS
{
    __u16   UnitLengthField;	// The length of the unit
    __u16   RelativeBcastTime;	// Relevant only to audio/video units. The relative time 
    							// for broadcasting this unit. 
    __u8    StreamNum;			// Relevant only to audio/video units. 
    __u8    VideoFrameType;		// Relevant only to video units
    __u8    VideoIndications;	// Relevant only to video units
    __u8    DataUnitType;		// Relevant only to data units
}cmmb_unit_params_t;


#define CMMB_MAX_UNIT_NUM_IN_SECTION    (100)
//*-------------------------
//* Section Header
typedef struct CMMB_SECTION_HEADER
{
    __u32            	NumUnits;
    __u32            	HeaderLengthIncludingCrc;
    __u32            	RESERVED1;
    __u32         		RESERVED2;
	cmmb_unit_params_t	UnitsParamsArr[CMMB_MAX_UNIT_NUM_IN_SECTION];
}cmmb_section_hdr_t;

//*-------------------------
//* Multiplex Block Header
typedef struct CMMB_MULTIPLEX_BLOCK
{
	__u16 FullLength;
	__u16 HeaderLength;
	__u8  BeginTag;
	__u8  EndTag;
	__u8  Type;
	__u8  DataUnitType;
	
	__u32 RESERVED1;
	__u32 RESERVED2;
}cmmb_mpx_blk_hdr_t;

typedef enum CMMB_SECTION_TYPE
{
    CMMB_SECTION_TYPE_VIDEO     = 0,
    CMMB_SECTION_TYPE_AUDIO     = 1,
    CMMB_SECTION_TYPE_DATA      = 2,
    CMMB_SECTION_TYPE_RESERVED  = 3,
}cmmb_section_type_t;


//*-----------------
//* XPE header information
typedef struct CMMB_XPE_PACKET
{
    __u8    HeaderLen; 
    __u8    IsStart;
    __u8    IsEnd; 
    __u8    IsErrorCorrection;
    __u8    IsCrc; 
    __u8    IsCrcError; 
    __u8    IsBusinessModeFileModel; 
    __u8    TransportSequence; 
    __u16   PayloadLen; 
    __u16   DataPacketTotalLen; 
}cmmb_xpe_pkt_t; 


//*-----------------
//* Generic XPE FEC parameter
typedef struct CMMB_XPEFEC_GENERIC_PARAM
{
    __u8 Len; 
    __u8 pBuf[256]; 
}cmmb_xpefec_gen_param_t; 

//*-----------------
//* XPE FEC parameter union
typedef struct CMMB_XPEFEC_PARAM
{
    __u16 Rs_255_207_NumRows; //Number of interleaver rows
    cmmb_xpefec_gen_param_t Generic; 
}cmmb_xpefec_param_t; 

//*-----------------
//* XPE header information
typedef struct CMMB_XPEFEC_PACKET
{
    __u8                HeaderLen; 
    __u8                IsStart;
    __u8                IsEnd; 
    __u8                IsCrc; 
    __u8                IsCrcError;
    __u8                TransportSequence; 
    __u8                FecAlgoTag;
    __u16               PayloadLen; 
    __u16               FecDataTotalLen; 
	cmmb_xpefec_param_t FecParameter; 
}cmmb_xpefec_pkt_t; 


/// Return Codes
typedef enum
{
	SMSHOSTLIB_ERR_OK										,
	SMSHOSTLIB_ERR_PENDING									,
	SMSHOSTLIB_ERR_UNDEFINED_ERR							,
	SMSHOSTLIB_ERR_NOT_IMPLEMENTED							,
	SMSHOSTLIB_ERR_NOT_SUPPORTED							,

	SMSHOSTLIB_ERR_LIB_NOT_INITIATED						,
	SMSHOSTLIB_ERR_LIB_ALREADY_INITIATED					,
	SMSHOSTLIB_ERR_DEVICE_NOT_INITIATED						,
	SMSHOSTLIB_ERR_COMM_NOT_VALID							,
	SMSHOSTLIB_ERR_TASK_CREATION_FAILED						,

	SMSHOSTLIB_ERR_INVALID_ARG								,
	SMSHOSTLIB_ERR_LIST_FULL								,
	SMSHOSTLIB_ERR_DAB_LIB_INIT_FAILED						,
	SMSHOSTLIB_ERR_MEM_ALLOC_FAILED							,
	SMSHOSTLIB_ERR_TIMEOUT									,
	SMSHOSTLIB_ERR_FS_ERROR									,
	SMSHOSTLIB_ERR_HANDINIT_ERROR							,
	SMSHOSTLIB_ERR_ALREADY_EXIST							,
	SMSHOSTLIB_ERR_SRV_IS_PLAYING							,
	SMSHOSTLIB_ERR_ONLY_ONE_SRV_ALLOWED						,
	SMSHOSTLIB_ERR_ONLY_DMB_SRV_ALLOWED						,
	SMSHOSTLIB_ERR_REQ_BEFORE_RESPONSE						,
	SMSHOSTLIB_ERR_NOT_SUPPORTED_BY_LITE					,

	SMSHOSTLIB_ERR_ENSEMBLE_NOT_EXIST						,
	SMSHOSTLIB_ERR_COMPONENT_NOT_EXIST						,
	SMSHOSTLIB_ERR_ALREADY_PLAYED							,
	SMSHOSTLIB_ERR_PLAYING_ANOTHER_ENSEMBLE					,
	SMSHOSTLIB_ERR_EXCEED_MAX_SERVICES						,
	SMSHOSTLIB_ERR_NOT_SUPPORTED_TRANSMIT_MODE				,
	SMSHOSTLIB_ERR_HANDLE_NOT_AVAIL							,
	SMSHOSTLIB_ERR_MODEM_START_FAILED						,

	SMSHOSTLIB_ERR_EXCEED_MAX_HANDLES						,
	SMSHOSTLIB_ERR_HANDLE_INVALID							,
	SMSHOSTLIB_ERR_MODEM_STOP_FAILED						,
	SMSHOSTLIB_ERR_CA_STOP_SERVICE_FAILED					,
	SMSHOSTLIB_ERR_CS_STOP_SERVICE_FAILED					,

  SMSHOSTLIB_ERR_EMM_START_SERVICE_NOT_SUPPORTED			,
	SMSHOSTLIB_ERR_ALREADY_ACTIVE							,
	SMSHOSTLIB_ERR_NOT_ACTIVE								,
	SMSHOSTLIB_ERR_SRVM_RESPONSE_TIMEOUT					,
	SMSHOSTLIB_TUNE_RESPONSE_TIMEOUT						,
	SMSHOSTLIB_ERR_MODEM_NOT_LOCKED							,
	SMSHOSTLIB_ERR_NO_SI_ACQ								,
	SMSHOSTLIB_ERR_SRVM_UNDEFINED_ERR						,
	SMSHOSTLIB_ERR_ENSEMBLE_IDX_EXCEED						,
	SMSHOSTLIB_ERR_SERVICE_ACTIVE							,
	SMSHOSTLIB_ERR_COMM_ERR									,
	SMSHOSTLIB_ERR_SCAN_ACTIVE								,
	SMSHOSTLIB_ERR_SCAN_NOT_ACTIVE							,
	SMSHOSTLIB_ERR_SERVICE_MAPPING_ACTIVE					,
	SMSHOSTLIB_ERR_SERVICE_MAPPING_ERROR					,
	SMSHOSTLIB_ERR_IP_SERVICE_IS_NOT_MAPPED					,
	SMSHOSTLIB_ERR_IP_SERVICE_DOES_NOT_EXIST				,
	SMSHOSTLIB_ERR_PID_FILTER_DOES_NOT_EXIST				,
	SMSHOSTLIB_ERR_MODEM_NO_TIMESLICING_IN_TPS				,
	SMSHOSTLIB_ERR_SCAN_CELL_ID_FILTER_ERROR				,
	SMSHOSTLIB_ERR_SCAN_STOPPED_BY_HOST						,
	SMSHOSTLIB_ERR_SERVICE_EXISTS_ON_OTHER_HANDLE			,

	SMSHOSTLIB_ERR_DEVICE_DISCONNECTED						,
	SMSHOSTLIB_ERR_DEVICE_DOES_NOT_EXIST					,

	SMSHOSTLIB_ERR_CA_FAILURE								,
	SMSHOSTLIB_ERR_DEVICE_NEED_REINIT   					,

	SMSHOSTLIB_ERR_NETWORK_DOES_NOT_EXIST					,
	SMSHOSTLIB_ERR_TIMESLOT_NOT_ACTIVE						,
	SMSHOSTLIB_ERR_CHANNEL_DOES_NOT_EXIST					,
	SMSHOSTLIB_ERR_EXCEED_MAX_CHANNELS						,
	SMSHOSTLIB_ERR_SERVICE_DOES_NOT_EXIST					,
	SMSHOSTLIB_ERR_FREQ_POINT_DOES_NOT_EXIST				,
	SMSHOSTLIB_ERR_PLAYING_ANOTHER_NETWORK					,
	SMSHOSTLIB_ERR_EXCEEDED_ALLOCATED_MEMORY				,
	SMSHOSTLIB_ERR_DATA_CORRUPTED							,
	SMSHOSTLIB_ERR_BOTH_SYNC_ASYNC							,
	SMSHOSTLIB_ERR_ILLEGAL_DATA								,
	SMSHOSTLIB_ERR_PLAYING_ANOTHER_SERVICE					,
	SMSHOSTLIB_ERR_ALREADY_SET								,
	SMSHOSTLIB_ERR_INJECT_TABLES_FAILED						,
	SMSHOSTLIB_ERR_PLAYING_ANOTHER_FREQUENCY				,
	SMSHOSTLIB_ERR_SERVICE_NOT_ACTIVE						,
	SMSHOSTLIB_ERR_EXCEED_MAX_CLIENTS						,
	SMSHOSTLIB_ERR_OUT_OF_BOUNDS							,
	SMSHOSTLIB_ERR_ILLEGAL_DATA2							,
	SMSHOSTLIB_ERR_ILLEGAL_DATA3							,
	SMSHOSTLIB_ERR_INCOMPLETE_FRAME							,
	SMSHOSTLIB_ERR_NOT_INITIALIZED							,
	SMSHOSTLIB_ERR_ALREADY_INITIALIZED						,
	SMSHOSTLIB_ERR_FAILED_CRC_CHECK							,
	SMSHOSTLIB_ERR_DATA_SCRAMBLED							,
	SMSHOSTLIB_ERR_NO_SIGNAL								,
	SMSHOSTLIB_ERR_DESCRAMBLING_FAILURE						,
	SMSHOSTLIB_ERR_PARSING_FAILURE							,

	SMSHOSTLIB_ERR_SRVM_GENERAL								,

	SMSHOSTLIB_ERR_SRVM_PAT_TABLE_READ_ERROR				,
	SMSHOSTLIB_ERR_SRVM_PAT_TABLE_READ_TIMEOUT				,

	SMSHOSTLIB_ERR_SRVM_NIT_TABLE_READ_ERROR				,
	SMSHOSTLIB_ERR_SRVM_NIT_TABLE_READ_TIMEOUT				,

	SMSHOSTLIB_ERR_SRVM_SDT_TABLE_READ_ERROR				,
	SMSHOSTLIB_ERR_SRVM_SDT_TABLE_READ_TIMEOUT				,

	SMSHOSTLIB_ERR_SRVM_INT_TABLE_READ_ERROR				,
	SMSHOSTLIB_ERR_SRVM_INT_TABLE_READ_TIMEOUT				,

	SMSHOSTLIB_ERR_SRVM_INT_PMT_TABLE_READ_ERROR			,
	SMSHOSTLIB_ERR_SRVM_INT_PMT_TABLE_READ_TIMEOUT			,

	SMSHOSTLIB_ERR_SRVM_MPE_PMT_TABLE_READ_ERROR			,
	SMSHOSTLIB_ERR_SRVM_MPE_PMT_TABLE_READ_TIMEOUT			,

	SMSHOSTLIB_ERR_SRVM_NO_LINKAGE_DESC_IN_NIT				,
	SMSHOSTLIB_ERR_SRVM_NO_PLATFORMS_IN_NIT					,
	SMSHOSTLIB_ERR_SRVM_NO_VALID_PLATFORMS_IN_NIT			,

	SMSHOSTLIB_ERR_SRVM_NO_VALID_INT_PID					,
	SMSHOSTLIB_ERR_SRVM_NO_VALID_IP_SERVICES				,

	SMSHOSTLIB_ERR_SRVM_NETWORK_ID_NOT_IN_FILTER			,
	SMSHOSTLIB_ERR_SRVM_PLATFORM_ID_NOT_IN_FILTER			,
	SMSHOSTLIB_ERR_SRVM_TS_VALIDATION_ERR					,

	SMSHOSTLIB_ERR_SRVM_MEMORY_ALLOCATION					,

	SMSHOSTLIB_ERR_REMOTE_HOST_NOT_FOUND					,
	SMSHOSTLIB_ERR_REMOTE_SOCKET_NOT_FOUND					,

	SMSHOSTLIB_ERR_UNSUPPORTED_CRYSTAL						,

	SMSHOSTLIB_ERR_COMM_NOT_INITIALIZED						,
	SMSHOSTLIB_ERR_DATA_LOSS_DEVICE							,

	SMSHOSTLIB_ERR_ESG_BAD_DATA     						,
	SMSHOSTLIB_ERR_ESG_IN_PROGRESS							,
	SMSHOSTLIB_ERR_ESG_NOT_EXIST      						,
	SMSHOSTLIB_ERR_ESG_STOPPED   							,
	SMSHOSTLIB_ERR_ESG_TABLE_NOT_FOUND						,
	

	SMSHOSTLIB_ERR_CA_MPX_PARSING_ERROR						,
	SMSHOSTLIB_ERR_CA_DATA_PARSING_ERROR					,
	SMSHOSTLIB_ERR_CA_DEVICE_ERROR							,
	SMSHOSTLIB_ERR_CA_CARD_DATA_ERROR						,
	SMSHOSTLIB_ERR_CA_CARD_MEM_ERROR						,
	SMSHOSTLIB_ERR_CA_CARD_NO_AUTH							,
	SMSHOSTLIB_ERR_CA_CARD_NOMATCH_CAS						,
	SMSHOSTLIB_ERR_CA_CARD_NO_ECM							,
	SMSHOSTLIB_ERR_CA_CARD_EMM_PROCESSING_ERROR				,
	SMSHOSTLIB_ERR_CA_CARD_AUTO_TO_EXPIRE					,
	SMSHOSTLIB_ERR_CA_CARD_FORBIDDEN						,
	SMSHOSTLIB_ERR_CA_NO_AUTH_TABLE				        	,
	SMSHOSTLIB_ERR_CA_MAX_CA_SERVICES				       	,
	SMSHOSTLIB_ERR_CA_NOT_INITIALIZED			        	,
	SMSHOSTLIB_ERR_CA_DEVICE_OPEN_FAILED		        	,
	SMSHOSTLIB_ERR_CA_SVC_NOT_OPENED			        	,
	SMSHOSTLIB_ERR_CA_NOT_SCRAMBLED			        		,
	SMSHOSTLIB_ERR_CA_AUTH_TABLE_TOO_BIG		        	,

	SMSHOSTLIB_ERR_RDS_GROUP_NOT_VALID			        	,	
	SMSHOSTLIB_ERR_RDS_WRONG_POINTER			        	,
	SMSHOSTLIB_ERR_TMC_WRONG_EVENT				        	,
	SMSHOSTLIB_ERR_BUFFER_OVERFLOW				        	,
	SMSHOSTLIB_ERR_NO_PARSING					        	,
	SMSHOSTLIB_ERR_WRONG_BLOCK_PARSING			        	,
	SMSHOSTLIB_ERR_FWDL_TIMEOUT								

} SMSHOSTLIB_ERR_CODES_E;

#endif

