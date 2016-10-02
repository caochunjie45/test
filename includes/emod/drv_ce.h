/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                            
*
*                              (c) Copyright 2011-2013, young.gq.yang China
*                                             All Rights Reserved
*
* File    : drv_ce.h
* By      : young.gq.yang
* Version : v1.00
* Date    : 2011-1-20 15:39
*********************************************************************************************************
*/
#ifndef _DRV_CE_H_
#define	_DRV_CE_H_
#include "typedef.h"

/*
to start the ce, first,we need configure the input buffer stream and output buffer.
when return CE_INPUT_DATA_REQ, the para we need to reconfigure is:in_offset,in_len ,first_in_flags,last_in_flags, 
others para can't be changed by sw developer;
when return CE_OUTPUT_DATA_FULL, we need to reconfigure the output buffer without configure input buffer stream.  
*/
typedef struct CE_STREAM_S {
    __u8      *in_start;           /* start of cycle buf: 1024 bytes align*/
    __u8      *in_end;             /* end of cycle buf: point to end of (n*1024) block, means in_end == 0x?????3ff*/
  	__u32     in_offset;           /* data offset: unit is byte: when restart, should: new offset == (old_addr[31:6] +(|old_addr[5:0]))[5:0]*/
  	__u32     in_len;              /* data length: unit is byte: less than 32M bytes*/
    __bool    first_in_flags;      /* means it's the first part of data for this process */
    __bool    last_in_flags;       /* means it's the last part of data for this process */
    __u32     total_in;            /* decompressed input bytes until now: refresh when input buffer data is exhausted.
                                                               * unit is byte.
                                                               */
                                
    __u8      *out_start;          /* next output byte should be put there : */
    __u32     out_len;             /* remaining free space at out_start : unit is byte , range is 256bytes - 256M bytes
                                                               *for png decoding, the output buffer len required to be enough to accomodate the decoded png pic data;
                                                               *the decoded png pic's width must be 4 bytes aligned. 
                                                                */ 
    __u32     total_out;           /* decompressed output bytes each time: refresh when output buffer is exhausted. 
                                    * unit is byte.
	                                */
                                   
    __u32   reserved;              /* reserved for future use */
} __ce_stream_t;

typedef enum{
    CE_PNG_GRAYSCALE,
    CE_PNG_TRUECOLOR = 2,
    CE_PNG_PALETTE,
    CE_PNG_GRAYSCALEWITHALPHA,
    CE_PNG_TRUECOLORWITHALPHA = 6
}COLORTYPE;

typedef struct PNG_FORMAT{
    __u32 height;               //height： png 图片的原始高度； 
    __u32 width;                //width: png图片的原始宽度；
    __u32 heightScale;          //heightScale： png height scale factor, output pic's height == 1/(heightScale), the allowed value is [1,2]
    __u32 widthScale;           //widthScale：  png width scale factor, output pic's width == 1/(widthScale), the allowed value is [1,2]
                                //the heightScale must equal widthScale;
    __u32 alphaFillValue;  
//	__u8 *plteTblAddr;          //pallete table start addr: not support right now.
    COLORTYPE colorType;             //only support CE_PNG_TRUECOLOR, CE_PNG_TRUECOLORWITHALPHA at present.
}__ce_png_format;

#define CE_WithPending         1         //request ce hardware resource with pending
#define CE_WithoutPending      0

/* Return codes for the decompression functions. Negative values
 * are errors, positive values are used for special but normal events.
 */
#define CE_OK                4      //if some progress has been made (more input processed or more output produced)
#define CE_INPUT_DATA_REQ    3      //input data is exhausted
#define CE_OUTPUT_DATA_FULL  2      //output buffer is exhausted
#define CE_STREAM_END        1      //the end of the compressed data has been reached and all uncompressed output has been produced
#define CE_NOT_AVAILABLE    (-1)    //ce hardware resource is not available
#define CE_STREAM_ERROR     (-2)    //if the stream structure was inconsistent (for example next_in or next_out was NULL)
#define CE_MEM_ERROR        (-4)    //if there was not enough memory
#define CE_BUF_ERROR        (-5)    //if no progress is possible,Inflate() can be called again with more input and more output space to
                                    //continue decompressing.
#define CE_FAIL             (-6)    //all other failure occur in ce module.

#ifdef USED_BY_DRVCE	//used by drvce module itsef

/*
*********************************************************************************************************
*                                       CE_Request
*
* Description: request ce resource within timout period
*
* Arguments  : 
*              if mode == CE_WithPending
*                     timeout
*                     is a period (in clock ticks).  
*                     If non-zero, your task will
*                     wait for the resource up to the amount of time specified by this argument.
*                     If you specify 0, however, your task will wait forever at the specified
*                     semaphore or, until the resource becomes available (or the event occurs).
*             if mode == CE_WithoutPending
*                     timeout is untrivial
*                     your task will check the resource either available or not
*                     if the resource is available, return CE_OK;
*                     if the resource is not available, return CE_NOT_AVAILABLE.
* Returns    :if succeed,return CE_OK;
*             if the hardware resource is not available within (timeout)ms, return CE_NOT_AVAILABLE.
*
* Note       :
*********************************************************************************************************
*/
__s32 CE_Request(__u16 timeout, __u32 mode);


/*
*********************************************************************************************************
*                                      CE_Release
*
* Description: release the resource get in request step
*
* Arguments  : 
*
* Returns    :
*
* Note       :
*********************************************************************************************************
*/
__s32 CE_Release(void);


/*
*********************************************************************************************************
*                                           CE_InflateInit
*
* Description: Initializes the internal stream state for decompression.
*
* Arguments  : 
*
* Returns    : CE_OK    :   initialize succeeded.
*              others   :   error code.
*
* Note       :
*********************************************************************************************************
*/
__s32 CE_InflateInit(void);

/*
*********************************************************************************************************
*                                        CE_Inflate
*
* Description: inflate decompresses as much data as possible, and stops when the input
*              buffer becomes empty or the output buffer becomes full.
* Arguments  : 
*
* Returns    :
*
* Note       :
*********************************************************************************************************
*/
__s32 CE_Inflate(__ce_stream_t * pStream);

/*
*********************************************************************************************************
*                                       CE_InflateEnd
*
* Description: All dynamically allocated data structures for this stream are freed.
*
* Arguments  : 
*
* Returns    :
*
* Note       :
*********************************************************************************************************
*/  
__s32 CE_InflateEnd(void);


//png related function
/*
*********************************************************************************************************
*                                       PNG_Request
*
* Description: request ce resource within timout period
*
* Arguments  : 
*              if mode == CE_WithPending
*                     timeout
*                     is a period (in clock ticks).  
*                     If non-zero, your task will
*                     wait for the resource up to the amount of time specified by this argument.
*                     If you specify 0, however, your task will wait forever at the specified
*                     semaphore or, until the resource becomes available (or the event occurs).
*             if mode == CE_WithoutPending
*                     timeout is untrivial
*                     your task will check the resource either available or not
*                     if the resource is available, return CE_OK;
*                     if the resource is not available, return CE_NOT_AVAILABLE.
* Returns    :if succeed,return CE_OK;
*             if the hardware resource is not available within (timeout)ms, return CE_NOT_AVAILABLE.
*
* Note       :
*********************************************************************************************************
*/
__s32 PNG_Request(__u16 timeout, __u32 mode);

/*
*********************************************************************************************************
*                                       PNG_Release
*
* Description: release the resource get in request step
*
* Arguments  : 
*
* Returns    :
*
* Note       :
*********************************************************************************************************
*/
__s32 PNG_Release(void);


/*
*********************************************************************************************************
*                                       PNG_InflateInit
*
* Description: Initializes the internal stream state for decompression.
*
* Arguments  : 
*
* Returns    :
*
* Note       :
*********************************************************************************************************
*/
__s32 PNG_InflateInit(const __ce_png_format* pPngFormat);


/*
*********************************************************************************************************
*                                       PNG_Inflate
*
* Description: inflate decompresses as much data as possible, and stops when the input
*              buffer becomes empty or the output buffer becomes full.
* Arguments  : 
*
* Returns    :
*
* Note       :
*********************************************************************************************************
*/
__s32 PNG_Inflate(__ce_stream_t * pStream);

/*
*********************************************************************************************************
*                                       PNG_InflateEnd
*
* Description: All dynamically allocated data structures for this stream are freed.
*
* Arguments  : 
*
* Returns    :
*
* Note       :
*********************************************************************************************************
*/  
__s32 PNG_InflateEnd(void);

#else
/*
*********************************************************************************************************
*                                       FOR EXPORTS SYMBOL TABLE
*
* Description: 
*
* Arguments  : 
*
* Returns    :
*
* Note       :
*********************************************************************************************************
*/
typedef struct DRVCE_API_ENTRY
{
	__s32(*ptr_CE_Request)(__u16 timeout, __u32 mode);
	__s32(*ptr_CE_Release)(void);
	__s32(*ptr_CE_InflateInit)(void);
	__s32(*ptr_CE_Inflate)(__ce_stream_t * pStream);
	__s32(*ptr_CE_InflateEnd)(void);
	__s32(*ptr_PNG_Request)(__u16 timeout, __u32 mode);
	__s32(*ptr_PNG_Release)(void);
	__s32(*ptr_PNG_InflateInit)(const __ce_png_format* pPngFormat);
	__s32(*ptr_PNG_Inflate)(__ce_stream_t * pStream);
	__s32(*ptr_PNG_InflateEnd)(void);
} drvce_api_entry_t;


#define DRVCE_ENTRY_TBL   (0xe0a00000)   //this address is depended on the link address of mod_drvce
#define DRVCE_FUNC_ENTRY  ((drvce_api_entry_t *)DRVCE_ENTRY_TBL)
#define CE_Request     	(DRVCE_FUNC_ENTRY->ptr_CE_Request	)
#define CE_Release     	(DRVCE_FUNC_ENTRY->ptr_CE_Release	)
#define CE_InflateInit 	(DRVCE_FUNC_ENTRY->ptr_CE_InflateInit	)
#define CE_Inflate     	(DRVCE_FUNC_ENTRY->ptr_CE_Inflate	)
#define CE_InflateEnd  	(DRVCE_FUNC_ENTRY->ptr_CE_InflateEnd	)
#define PNG_Request    	(DRVCE_FUNC_ENTRY->ptr_PNG_Request	)
#define PNG_Release    	(DRVCE_FUNC_ENTRY->ptr_PNG_Release	)
#define PNG_InflateInit	(DRVCE_FUNC_ENTRY->ptr_PNG_InflateInit	)
#define PNG_Inflate    	(DRVCE_FUNC_ENTRY->ptr_PNG_Inflate	)
#define PNG_InflateEnd 	(DRVCE_FUNC_ENTRY->ptr_PNG_InflateEnd	)

#endif /*USED_BY_DRVCE*/

#endif	/* _DRV_CE_H_ */
