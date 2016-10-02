
/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_mmp sub-system
*                           ldec_txt decoder module
*
*          (c) Copyright 2009-2010, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : ldec_itxt_i.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2009-6-24
********************************************************************************
*/


#ifndef _LDEC_SUBDEC_H_
#define _LDEC_SUBDEC_H_

#include "ldec_idvb_i.h"

#define DVBSUB_FRAME_MAX_DURATION (60000) // 单位:ms. dvb字幕项不一样给出持续时间, 如果没给,就认为持续时间无限长,这个无限长定为60秒

#define LDEC_DATA_THRESHOLD        0x59

#define LDEC_REGION_NUM            16
#define LDEC_OBJECT_NUM            16
#define LDEC_CLUTENTRY_NUM        256
#define LDEC_CLUT_NUM             16
#define LDEC_MAX_PALTTE_NUM       16

#define LDEC_PAGE_REGION_INDEX     0
#define LDEC_REGION_OBJECT_INDEX   1
#define LDEC_CLUT_INDEX            2
#define LDEC_REGION_INDEX          3
#define LDEC_PAGE_INDEX            4


#define LDEC_PAGE_COMPOSITION_SEGMENT   0x10
#define LDEC_REGION_COMPOSITION_SEGMENT 0x11
#define LDEC_CLUT_DEFINITION_SEGMENT    0x12
#define LDEC_OBJECT_DATA_SEGMENT        0x13
#define LDEC_END_DISPLAY_SEGMENT        0x80
#define LDEC_END_PES_DATA_MARKER        0xFF


typedef enum  __SUBTITLE_DEC_STATUS
{
	LDEC_CHECK_DATA_VALID = 0,
	LDEC_FIND_STREAM_ID,
	LDEC_PARSE_SEGMENT,
	LDEC_IDLE
}__ldec_status_t;

typedef enum  __SUBTITLE_DEC_TYPE
{
	LDEC_CODE_PIXEL = 0,
    LDEC_CODE_STRING, 
}__ldec_type_t;

/* *********************Page composition segment (ETSI EN 300 743 section 7.2.1) */
typedef enum __SUBTITLE_PAGE_STATUS
{
    LDEC_PAGE_NORMAL_STATUS = 0,
	LDEC_PAGE_ACQUISITON_POINT,
	LDEC_PAGE_MODE_CHANGE,
	LDEC_PAGE_ERROR
} __ldec_page_status_t;

typedef struct _SUBTITLE_PAGE_REGION
{   
	__u8 hasUsedFlag;
    __u8  regionId;          
    __u16 regionHorAddr;
    __u16 regionVerAddr;
} __ldec_page_region_t;


typedef struct  SUBDEC_PAGE
{   
	__u8  newPageFlag;
    __u32  pageTimeOut;
    __u8  pageVersion;
	__u32 pageRegionNum;
	__u32 ptsVal;
	__u8 hasValidPts;
	__ldec_page_status_t pageState;
    __ldec_page_region_t pageRegion[LDEC_REGION_NUM];
} __ldec_page_t;

//*****************************************************//
typedef struct __SUBTITLE_REGION_OBJECT
{
    __u8    hasUsedFlag;
	__u16   objectId;
	__u8    objectType;
	__u16   objectHorPos;
	__u16   objectVerPos;
	__u8    forPIxelCode;
	__u8    backPixelCode;
}__ldec_region_object_t;

typedef struct __SUBTITLE_REGION
{  
	__u8  hasUsedFlag;
	__u8  regionId;
	__u8  regionVersion;
	__u16 regionWidth;
	__u16 regionHeight;
	__u8  regionLevel;
	__u8  regionDepth;
	__s8  regionClutId;
	__u32 regionObjectNum;
    __ldec_region_object_t regionObject[LDEC_OBJECT_NUM];
}__ldec_region_t;

//*************************************************************//
typedef struct __SUBTITLE_CLUT_ENTRY
{   
	__u8 hasUsedFlag;
	__u8 entryId;
	__u8 twoEntryFlag;
	__u8 fourEntryFlag;
	__u8 eightEntryFlag;
	__u8 fullRangeFlag;
	__u8 yValue;
	__u8 crValue;
	__u8 cbValue;
	__u8 tValue;
}__ldec_clut_entry_t;

typedef struct __SUBTITLE_CLUT
{   
	__u8 hasUsedFlag;
    __u8 clutId;
	__u8 clutVerion;
	__u32 clutEntryNum;
    __ldec_clut_entry_t clutEntry[LDEC_CLUTENTRY_NUM];
}__ldec_clut_t;

//***************************************************************//
typedef struct __SUBTITLE_OBJECT
{ 
  __u32   ptsVal;
  __u8    ptsValid;
  __u8    objectVersion;
  __u8    objectCodeType;
  __u8    nonModifyFlag;
  __u8    pageRegionIdx;
  __u8    regionIdx;
  __u8    regionObjectIdx;
  __s8    clutIdx;
  __u16   subLen;
  __u16   objectId;
  __u16   objectStartX;
  __u16   objectStartY;
  __u16   objectWidth;
  __u16   objectHeight;
  __u16   realSubWidth;
}__ldec_object_t;

//****************************************************************//

typedef struct __SUBTITLE_INFO
{   
    __u8            findStartCode;
  	__u8            nextStep;
	__s16           lastClutIdx;
	__u32           palette[LDEC_MAX_PALTTE_NUM];
	__u8            *anciDataBuf;
    __ldec_page_t   *subdecPage;
	__ldec_region_t *subdecRegion;
	__ldec_clut_t   *subdecClut;
    __ldec_object_t *subdecObject;
}__ldec_info_t;


__u32 subdec_2bit_pixel_code_string(__u8 *newDataBuf,__u8 *oldDataBuf, __u16 *realSubWidth);
__u32 subdec_4bit_pixel_code_string(__u8 *newDataBuf,__u8 *oldDataBuf,__u16 *realSubWidth);
__u32 subdec_8bit_pixel_code_string(__u8 *newDataBuf,__u8 *oldDataBuf,__u16 *realSubWidth);

__s8 dvbsub_parse_segment(__ldec_idvbsub_t *dvbsubDec, __u8 *findEndFlag);
__s8 subdec_find_startcode(__ldec_lbs_buf_t *subdecBuf, __u8 fstVal, __u8 secVal, __u8 checkSecFlag);

void subdec_clear_used_flag(__ldec_idvbsub_t *dvbsubDec);
void subdec_vbv_update_read_pointer(__ldec_lbs_buf_t *subdecBuf, __u32 uread_byte);
void subdec_pixel_to_rgbVal(__ldec_info_t *subdecInfo,__ldec_object_t *pSubObject);
void subdec_pixel_data_sub_parse(__u8 *newDataBuf,__u8 *oldDataBuf, __u16 dataLen, __u16 widthLen, __u16 heightLen, __u16 *realSubWidth);
__s8 subdec_process_object_data(__ldec_idvbsub_t *dvbsubDec);
void ldec_update_read_pointer(__ldec_lbs_buf_t *ldecLbsBuf, __u32 uread_byte);
void adjust_read_pointer_case1(__ldec_lbs_buf_t *ldecLbsBuf, __u32 uread_byte);
void adjust_read_pointer_case2(__ldec_lbs_buf_t *ldecLbsBuf, __u32 uread_byte);
void adjust_read_pointer_case3(__ldec_lbs_buf_t *ldecLbsBuf, __u32 uread_byte);

__s32 Impl_DvbsubDec_Open(__ldec_idvbsub_t *thiz);
__s32 Impl_DvbsubDec_Close(__ldec_idvbsub_t *thiz);
#endif

