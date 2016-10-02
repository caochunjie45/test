
#ifndef CMMB_PARSER_H
#define CMMB_PARSER_H

//#include "cmmbDemux_interface.h"

__s32 CmmbParseMultiplexBlock(__u8* pBuf, __u32 bufSize, cmmb_mpx_blk_hdr_t* pOutHeader);

__s32 CmmbParseDataSectionHeader(__u8* pBuf, __u32 bufSize, cmmb_section_hdr_t* pOutHeader);

__s32 CmmbParseVideoSectionHeader(__u8* pBuf, __u32 bufSize, cmmb_section_hdr_t* pOutHeader);

__s32 CmmbParseAudioSectionHeader(__u8* pBuf, __u32 bufSize, cmmb_section_hdr_t* pOutHeader);

__s32 CmmbParseSubFrameHeader(__u8* pBuf, __u32 bufSize, cmmb_subfrm_hdr_t* pOutHeader);

__s32 cmmbParseFrameHeader(__u8* pBuf, __u32 bufSize, cmmb_multiplex_frm_hdr_t* pOutFrameHeader);

__s32 cmmbParseXpe(__u8* pBuf, __u32 bufSize, cmmb_xpe_pkt_t* pOutXpeHeader);

__s32 cmmbParseXpeFec(__u8* pBuf, __u32 bufSize, cmmb_xpefec_pkt_t* pOutXpeFecHeader);

#endif

