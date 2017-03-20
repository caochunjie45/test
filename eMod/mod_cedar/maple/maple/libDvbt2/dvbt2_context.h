
#ifndef DVB_CONTEXT_H
#define DVB_CONTEXT_H

#include "maple_common.h"
#include "dvbt2_interface.h"
#include "dlist.h"

#define  ckydebug    0

#define MAX_SERVNAME_SIZE MAPLE_MAX_NAME_LEN //SDT��servicename����󳤶�,����\0
#define MAX_STREAMNAME_SIZE MAPLE_MAX_NAME_LEN  //һ����Ƶ�������ֵ���󳤶�, pmt��ʹ��

#define TS_MAX_SERVICE_NUM  (128)   //һ��ts�����128��service
#define MAX_SERVICE_NUM  (64)   //��isdb��oneseg��������8��service��ֲ������DVB���ݶ�Ϊ64


#define MAX_STREAM_NUM_IN_SERVICE  (8)       //һ��service��������stream������



#define DVBT2_SECTION_BUF_SIZE   (4096)

#define PAT_BUF_IDX   0
#define PMT_BUF_IDX   0
#define SDT_BUF_IDX   0
#define EIT_BUF_IDX   0
#define TDT_BUF_IDX   1
#define NIT_BUF_IDX   0
#define SECTION_BUFIDX_CNT   8 //5


//**********PID value List***********************//
#define PAT_PID             0x0000
#define PMT_PID_FOR_ONESEG  0x1fc8
#define CAT_PID             0x0001
#define TSDT_PID            0x0002
#define NIT_PID             0x0010
#define SDT_BAT_PID         0x0011
#define EIT_PID             0x0012
#define RST_PID             0x0013
#define TDT_TOT_PID         0x0014
#define DIT_PID             0x001E
#define SIT_PID             0x001F
//***********************************************//
#define STREAM_TYPE_VIDEO_MPEG1             0x01
#define STREAM_TYPE_VIDEO_MPEG2             0x02
#define STREAM_TYPE_AUDIO_MPEG1             0x03
#define STREAM_TYPE_AUDIO_MPEG2             0x04
#define STREAM_TYPE_PRIVATE_SECTION         0x05
#define STREAM_TYPE_PRIVATE_DATA            0x06
#define STREAM_TYPE_AUDIO_AAC               0x0f
#define STREAM_TYPE_VIDEO_MPEG4             0x10
#define STREAM_TYPE_AUDIO_MPEG4             0x11    //* LATM AAC AUDIO
#define STREAM_TYPE_VIDEO_H264              0x1b
#define STREAM_TYPE_VIDEO_VC1               0xea

#define STREAM_TYPE_PCM_BLURAY              0x80    //* add for blue ray
#define STREAM_TYPE_AUDIO_AC3               0x81
#define STREAM_TYPE_AUDIO_HDMV_DTS          0x82
#define STREAM_TYPE_AUDIO_AC3_TRUEHD        0x83
#define STREAM_TYPE_AUDIO_EAC3              0x84
#define STREAM_TYPE_AUDIO_DTS_HRA           0x85    //* add for blue ray
#define STREAM_TYPE_AUDIO_DTS_MA            0x86    //* add for blue ray
#define STREAM_TYPE_AUDIO_DTS               0x8a
#define STREAM_TYPE_AUDIO_AC3_              0xa1
#define STREAM_TYPE_AUDIO_DTS_              0xa2

#define STREAM_TYPE_AUDIO_HDMV_PGS_SUBTITLE 0x90
#define STREAM_TYPE_SUBTITLE_DVB            0x100
#define STREAM_TYPE_SUBTITLE_ARIB           0x200   //�Զ���,������˫�ֽ�
#define STREAM_TYPE_SUBTITLE_TELETEXT	 0x300
#define CABLE_DELIVERY_SYSTEM 1		     //for SATELLITE SYSTEM uncomment this line
//#define SATELLITE_DELIVERY_SYSTEM 1    //for SATELLITE SYSTEM uncomment this line
//#define TERRISTRIAL_DELOVERY_SYSATEM 1 //for TERRISTRIAL  SYSTEM uncomment this line

//***********************************************//
// * Descriptor Tag Value 
// * 
//************************************************//
#define VIDEO_STREAM_DESCRIPTOR_TAG                0x02
#define AUDIO_STREAM_DESCRIPTOR_TAG					0x03
#define REGISTRATION_DESCRIPTOR_TAG                0x05

//***********************************************//
// * Descriptor Tag Value 
// * defined by ETSI EN 300 468 Table 12
//************************************************//
#define ISO_639_LANGUAGE_DESCRIPTOR_TAG            0x0A
#define NETWORK_NAME_DESCRIPTOR_TAG                0x40
#define SERVICE_LIST_DESCRIPTOR_TAG                0x41
#define STUFFING_DESCRIPTOR_TAG                    0x42
#define SATELLITE_DESCRIPTOR_TAG                   0x43
#define CABLE_DESCRIPTOR_TAG                       0x44
#define VBI_DATA_DESCRIPTOR_TAG                    0x45
#define VBI_TELETEXT_DESCRIPTOR_TAG                0x46
#define BOUQUET_NAME_DESCRIPTOR_TAG                0x47
#define SERVICE_DESCRIPTOR_TAG                     0x48
#define COUNTRY_AVAILABILITY_DESCRIPTOR_TAG        0x49
#define LINKAGE_DESCRIPTOR_TAG                     0x4A 
#define NVOD_REFERENCE_DESCRIPTOR_TAG              0x4B
#define TIME_SHIFTED_SERVICE_DESCRIPTOR_TAG        0x4C
#define SHORT_EVENT_DESCRIPTOR_TAG                 0x4D
#define EXTENDED_EVENT_DESCRIPTOR_TAG              0x4E
#define TIME_SHIFTED_EVENT_DESCRIPTOR_TAG          0x4F
#define COMPONENT_DESCRIPTOR_TAG                   0x50
#define MOSAIC_DESCRIPTOR_TAG                      0x51
#define STREAM_IDENTIFIER_DESCRIPTOR_TAG           0x52
#define CA_IDENTIFIER_DESCRIPTOR_TAG               0x53 
#define CONTENT_DESCRIPTOR_TAG                     0x54 
#define PARENTAL_RATING_DESCRIPTOR_TAG             0x55
#define TELETEXT_DESCRIPTOR_TAG                    0x56
#define TELEPHONE_DESCRIPTOR_TAG                   0x57
#define LOCAL_TIME_OFFSET_DESCRIPTOR_TAG           0x58
#define SUBTITLING_DESCRIPTOR_TAG                  0x59 
#define TERRESTRIAL_DESCRIPTOR_TAG			       0x5A
#define MULTILINGUAL_NETWORK_NAME_DESCRIPTOR_TAG   0x5B
#define MULTILINGUAL_BOUQUET_NAME_DESCRIPTOR_TAG   0x5C
#define MULTILINGUAL_SERVICE_NAME_DESCRIPTOR_TAG   0x5D
#define MULTILINGUAL_COMPONENT_DESCRIPTOR_TAG      0x5E
#define PRIVATE_DATA_SPECIFIER_DESCRIPTOR_TAG      0x5F
#define SERVICE_MOVE_DESCRIPTOR_TAG                0x60 
#define SHORT_SMOOTHING_BUFFER_DESCRIPTOR_TAG      0x61
#define FREQUENCY_LIST_DESCRIPTOR_TAG              0x62
#define PARTIAL_TRANSPORT_STREAM_DESCRIPTOR_TAG    0x63
#define DATA_BROADCAST_MDESCRIPTOR_TAG             0x64 
#define DATA_BROADCAST_ID_DESCRIPTOR_TAG           0x66
#define TRANSPORT_STREAM_DESCRIPTOR_TAG            0x67	
#define DSNG_DESCRIPTOR_TAG                        0x68
#define PDC_DESCRIPTOR_TAG                         0x69 
#define AC3_DESCRIPTOR_TAG                         0x6A
#define ANCILLARY_DATA_DESCRIPTOR_TAG              0x6B
#define CELL_LIST_DESCRIPTOR_TAG                   0x6C
#define CELL_FREQUENCY_LINK_DESCRIPTOR_TAG         0x6D
#define ANNOUNCEMENT_SUPPORT_DESCRIPTOR_TAG        0x6E
#define APPLICATION_SIGNALLING_DESCRIPTOR_TAG      0x6F
#define ADAPTATION_FIELD_DATA_DESCRIPTOR_TAG       0x70
#define SERVICE_IDENTIFIER_DESCRIPTOR_TAG          0x71
#define SERVICE_AVAILABILITY_DESCRIPTOR_TAG        0x72


//****************************************************
//�������������û��Զ���
//****************************************************
#define ENHANCED_AC3_DESCRIPTOR_TAG                0x7A
#define DTS_DESCRIPTOR_TAG                         0x7B
#define AAC_DESCRIPTOR_TAG                         0x7C

#define  LOGICAL_CHANNEL_DESCRIPTOR_TAG					0x83  //ĳЩ����DVB SI NIT���е�LCN�����������������
#define  LOGICAL_CHANNEL_DESCRIPTOR_V2_TAG			0x87  //ĳЩ����DVB SI NIT���е�LCN������
#define  EACEM_STREAM_IDENTIFIER_DESCRIPTOR_TAG  0x86 

#define TS_INFORMATION_DESCRIPTOR_TAG              0xCD
#define LOGO_TRANSMISSION_DESCRIPTOR               0xCF
#define PARTIAL_RECEPTION_DESCRIPTOR_TAG           0xFB
#define DATA_COMPONENT_DESCRIPTOR_TAG              0xFD //ISDB SI spec�����������


static __inline __u16 SI_MERGE_U16(__u8 *ptr)
{
	return (__u16)((ptr[0]<< 8)|ptr[1]);
}

/*******************************************************************************
Function name: tag_PATInfo
Description:
    ����Ϊsearch�����Ĵ洢����PAT��Ϣ��struct.ֻ��Ŀǰ��Ҫ����Ϣ.�Ժ��������.
    ��¼����PAT���������Ϣ.����section���һ��PAT��.
    �����ǲ���.
Members: 
    
*******************************************************************************/
typedef struct tag_PATServInfo
{
    __u16   nServId;
    __u16   pmtpid;
}PATServInfo;
typedef struct tag_PATInfo
{
    __u16   tsId;
    __s32   nServNum;
    PATServInfo servInfoArray[TS_MAX_SERVICE_NUM];
}PATInfo;
extern __s32 PATInfo_Init(PATInfo *thiz);
extern __s32 PATInfo_Exit(PATInfo *thiz);
extern PATInfo* newPATInfo(void);
extern void deletePATInfo(PATInfo* thiz);

typedef struct tag_PMTStreamInfo
{
    __u16   nStreamType;    //STREAM_TYPE_VIDEO_MPEG1��
    __u16   nStreamPid;
    __s32   nStreamNameLen;   //������\0.
    __u8    StreamName[MAX_STREAMNAME_SIZE];
}PMTStreamInfo;

typedef struct tag_PMTInfo
{
    __u16   nServId;
    __u16   pcrPid;                         //* the pid of ts packets in which the pcr exits.
    __s32   nStreamNum;
    PMTStreamInfo   streamInfoArray[MAX_STREAM_NUM_IN_SERVICE];
}PMTInfo;
extern __s32 PMTInfo_Init(PMTInfo *thiz);
extern __s32 PMTInfo_Exit(PMTInfo *thiz);
extern PMTInfo* newPMTInfo(void);
extern void deletePMTInfo(PMTInfo* thiz);

typedef struct tag_SDTServInfo
{
    __u16           nServId;
    __u8            nFreeCAMode;    //0:free, 1:CA
    __u16           nServType;  //dvb��si spec, 
    __s32           nServNameLen;   //������\0.
    __u8            servName[MAX_SERVNAME_SIZE];
}SDTServInfo;
extern SDTServInfo* newSDTServInfo(void);
extern void deleteSDTServInfo(SDTServInfo* thiz);

/*******************************************************************************
Function name: tag_SDTInfo
Description:
    ����Ϊsearch�����Ĵ洢����SDT��Ϣ��struct.ֻ��Ŀǰ��Ҫ����Ϣ.�Ժ��������.
    ��¼����SDT���������Ϣ.����section���һ��PAT��.
    �����ǲ���.
Members: 
    
*******************************************************************************/
typedef struct tag_SDTInfo
{
    __u16   tsId;
    __u16   orgNetworkId;
    DList   *pList_SDTServInfo; //������SDTServInfo
}SDTInfo;
extern __s32 SDTInfo_Init(SDTInfo *thiz);
extern __s32 SDTInfo_Exit(SDTInfo *thiz);
extern SDTInfo* newSDTInfo(void);
extern void deleteSDTInfo(SDTInfo* thiz);

typedef struct tag_NITTSInfo
{
#if(ckydebug == 1)
	__u16 u16TSId;
    __u16 u16ONId;
    __u16 u16ServiceId;
    __u8  u8LinkageType;
    __u8  u8HandOverType : 4; //hand-over_type. Only vilid if u8LinkageType == 0x08.
    __u8  u8Reserved : 3; //Only vilid if u8LinkageType == 0x08.
    __u8  u8OriginType : 1; //origin_type. Only vilid if u8LinkageType == 0x08.
    __u16 u16NetworkId;      //Only vilid if u8LinkageType == 0x08 && u8OriginType != 0.
    __u16 u16InitServiceId;  //Only vilid if u8LinkageType == 0x08 && u8OriginType == 0.
    __u8  u8PrivDataLeng;
    __u8  *pu8PrivData;
	
#endif
    __u16   ts_id;
    __u16   orig_ts_id;
    __u16   nRemoteControlKeyId[TS_MAX_SERVICE_NUM];    //����ŷ��ĳЩ����dvb SI, һ��ts��ֻ��һ��remote control key id.�������¼����.
    __s32   nServNum;			//��remote control key id�Ľ�ĿNUM
	__u8    nVisible_Service_Flag; //��Ŀ�Ƿ�ɼ�����ʾ��service�Ƿ����ʾ��ֻ���TV,Teletext���͵ģ�Radio���Ͳ��ɼ���
    __u16   ServIdArray[TS_MAX_SERVICE_NUM];  //������Tv����㴫���service
}NITTSInfo;
extern NITTSInfo* newNITTSInfo(void);
extern void deleteNITTSInfo(NITTSInfo* thiz);

/*******************************************************************************
Function name: tag_NITInfo
Description:
    ����Ϊsearch�����Ĵ洢����NIT��Ϣ��struct.ֻ��Ŀǰ��Ҫ����Ϣ.�Ժ��������.
    ��¼����NIT���������Ϣ.����section���һ��NIT��.
Members: 
    
*******************************************************************************/
typedef struct tag_NITInfo
{
    __u16   networkId;
    DList   *pList_NITTSInfo; //������NITTSInfo
}NITInfo;
extern __s32 NITInfo_Init(NITInfo *thiz);
extern __s32 NITInfo_Exit(NITInfo *thiz);
extern NITInfo* newNITInfo(void);
extern void deleteNITInfo(NITInfo* thiz);

typedef struct DVBT2_CONTEXT
{
    maple_serv_list_t       localServList;
                            
    __u32                   frequency;  //�ڵ�Ƶ������ǰ,�洢Ƶ����Ϣ
    __u32                   bandwidth;

    NITInfo                 *pNitInfoForSearch; //��Ե�Ƶ���NITInfo
    PATInfo                 *pPatInfoForSearch; 
    PMTInfo                 *pPmtInfoForSearch;
    SDTInfo                 *pSdtInfoForSearch;
    
                            
    __u32                   slSearchTaskId;//��̨���߳�id. isdb_scan_task()
    __u32                   slSearchStopFlag; //��ʾ��̨�����Ƿ��Ѿ�ֹͣ.ֹͣ�����1.����Ҫ����ֹ�ı��λ.
                            
    __u32                   sepgSearchTaskId; //�ѷ���,����sepg,table_id = 0x4e,0x4f
    __u32                   sepgSearchStopFlag;//�ѷ���
    __u32                   serviceIdxSepg;	//�ѷ���
                            
    __u32                   schdlSearchTaskId; //�ѷ���. ��ʾ����schedule epg���߳�,����table_id = 0x50~0x5f, table_id = 0x60~0x6f��eit��,
    __u32                   schdlSearchStopFlag; //�ѷ���.��ʾ�����Ƿ��Ѿ�ֹͣ.
    __u32                   serviceIdxSchdl;	   //�ѷ���. ��ʾҪ���ص�EPG�����ڵ�service.
                            
    __u32                   timeMonitorTaskId;
    __u32                   timeMonitorStopFlag;
    
    __u32                   epgMonitorTaskId;		//��������epg���߳�id.
    __u32                   epgMonitorStopFlag;	//��ʾ���������Ѿ�ֹͣ.
    __krnl_event_t*         epgMutex;
                            
    maple_time_t            curStreamTime;
    
    __epdk_charset_enm_e    charCodeType;
    
    __u8*                   dvbt2MallocBuf;
    __u8*                   dvbt2SectionBuf[SECTION_BUFIDX_CNT];  //�±�ź�PAT_BUF_IDX�ȶ�Ӧ,EIT����
    __u8                    isBufFull[SECTION_BUFIDX_CNT];   //is buf full, ��isdbSectionBuf[]��Ӧ
   __u32                    curPatSecNum;      // current pat section number,��ǰ��Ҫ��section��number.����serviceʱ��.
   __u32			     	curPmtSecNum;   //��̨��
   __u32                    curSdtSecNum;	//��̨ʱ��ʾҪ�ѵڼ���section,��0��ʼ
   __u32                    curNitSecNum;
   __s32                    endPmtSection;
   __s32                    endPatSection;     // end pat table,��pat table������section����ȫ�Ժ�,��1
   __s32                    endSdtSection;	 //sdt�������section��parse�����1
   __s32                    endNitSection;

   __u8*                    eucStringBuf;   //���JIS���뷽ʽ,��Ҫ��תΪEUCJP.Ϊ����Ƶ�����ڴ�malloc,���øñ���.
   __u32                    eucStringBufSize;
}dvbt2_ctx_t;

extern dvbt2_ctx_t* g_dvbt2Ctx;

extern dvbt2_ctx_t* newDvbt2Ctx(void);
extern void deleteDvbt2Ctx(dvbt2_ctx_t *thiz);

void dvbt2_scan_task(void* arg);
__s32 dvbt2_scan_by_serviceId(maple_serv_item_t* pServItem);

void dvbt2_schedule_scan_task(void* arg);
void dvbt2_sepg_scan_task(void* arg);
void dvbt2_time_monitor_task(void* arg);
void dvbt2_epg_monitor_task(void* arg);

__s32 dvbt2_get_mutex(__krnl_event_t* mutex);
void  dvbt2_release_mutex(__krnl_event_t* mutex);
__s32 dvbt2_enble_remotecontrlkey(__u8 aux);
__s32 dvbt2_pat_request_buffer(void *p_arg);
__s32 dvbt2_pmt_request_buffer(void *p_arg);
__s32 dvbt2_sdt_request_buffer(void *p_arg);
__s32 dvbt2_nit_request_buffer(void *p_arg);
__s32 dvbt2_tdt_request_buffer(void *p_arg);
__s32 dvbt2_eit_request_buffer(void *p_arg);

__s32 dvbt2_pat_update_data(void *p_arg);      
__s32 dvbt2_pmt_update_data(void *p_arg);    
__s32 dvbt2_sdt_update_data(void *p_arg);
__s32 dvbt2_nit_update_data(void *p_arg);
__s32 dvbt2_tdt_update_data(void *p_arg);      
__s32 dvbt2_eit_update_data(void *p_arg);         
 


#endif

