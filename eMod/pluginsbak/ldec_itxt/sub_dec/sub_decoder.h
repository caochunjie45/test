
#ifndef _SUB_DECODER_H_
#define _SUB_DECODER_H_

//#include "vdec_type.h"
//#include "ldec_itxt_i.h"



typedef struct SP_DCSQT
{
   __u16 fstTop;
   __u16 fstBottom;
   __u16 spuSize;
   __u16 spStartaddr;
   __u16 hasDecnum;
   __u16 maxDecLine[2];
   __u16 minDecLine[2];
   __bool nonZreoValFlag;
   __bool nonEnoughData;
   __u8   lastVal;
   __u8   *decBufAddr;
} __ldec_sp_dcqt_t;

__s8 convert_data_to_bmp(__cedar_subtitle_item_t *curSuItem, __u8 *dataBuf);
__s8 run_length_decode(__cedar_subtitle_item_t *curSuItem, __u8 *start_addr, __ldec_sp_dcqt_t *spSqtInfo, __u8 *dataBuf, __u8 fieldType);
__s32 process_sp_dcsqt(ItxtDec *pdec, __cedar_subtitle_item_t *curSuItem, __u8 *start_addr, __ldec_sp_dcqt_t *spSqtInfo);        

typedef struct tag_IsubDec
{
    ItxtDec base;
}IsubDec;

extern __s32 IsubDec_Init(IsubDec *thiz);
extern __s32 IsubDec_Exit(IsubDec *thiz);
extern IsubDec *newIsubDec(void);
extern void deleteIsubDec(IsubDec *thiz);

#endif

