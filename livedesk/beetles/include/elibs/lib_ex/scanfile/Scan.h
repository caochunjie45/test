/*
*********************************************************************************************************
*                                                    ePDK
*                                   the Easy Portable/Player Development Kit
*                                       ebook information manager libary
*
*                                    (c) Copyright 2010-2012, Sunny China
*                                              All Rights Reserved
*
* File    : Scan.h
* By      : Sunny
* Version : V1.00
* Date    : 2010-4-21 21:11:47
*********************************************************************************************************
*/

#ifndef		__SCAN_H__
#define		__SCAN_H__

#include	"epdk.h"

/* ebook control scaned file format */
#define		EBOOK_SCAN_TXT	((__u64)1<<1)
#define		EBOOK_SCAN_H	((__u64)1<<2)
#define		EBOOK_SCAN_C	((__u64)1<<3)
#define 	EBOOK_SCAN_INI	((__u64)1<<4)
#define		EBOOK_SCAN_LRC	((__u64)1<<5)
#define		EBOOK_SCAN_SRT	((__u64)1<<6)
#define 	EBOOK_SCAN_SSA	((__u64)1<<7)
#define 	EBOOK_SCAN_ASS	((__u64)1<<8)

#define		EBOOK_SCAN_ALL	 (EBOOK_SCAN_TXT | EBOOK_SCAN_H | EBOOK_SCAN_C | EBOOK_SCAN_INI| EBOOK_SCAN_LRC | EBOOK_SCAN_SRT | EBOOK_SCAN_SSA | EBOOK_SCAN_ASS)/* all support ebook types */

/*movie*/
#define 	MOVIE_SCAN_AVI	((__u64)1<<15)
#define     MOVIE_SCAN_MP4	((__u64)1<<16)
#define     MOVIE_SCAN_MPG	((__u64)1<<17)
#define     MOVIE_SCAN_RM	((__u64)1<<18)
#define     MOVIE_SCAN_FLV	((__u64)1<<19)
#define     MOVIE_SCAN_PMP	((__u64)1<<20)
#define     MOVIE_SCAN_3GP	((__u64)1<<21)
#define     MOVIE_SCAN_VOB	((__u64)1<<22)
#define     MOVIE_SCAN_DAT	((__u64)1<<23)
#define     MOVIE_SCAN_RMVB	((__u64)1<<24)
#define     MOVIE_SCAN_MKV	((__u64)1<<25)
#define     MOVIE_SCAN_WMV	((__u64)1<<26)
#define     MOVIE_SCAN_ASF	((__u64)1<<27)
#define     MOVIE_SCAN_PSR	((__u64)1<<28)
#define     MOVIE_SCAN_TP 	((__u64)1<<29)
#define     MOVIE_SCAN_TS	((__u64)1<<30)
#define     MOVIE_SCAN_MOV	((__u64)1<<31)
#define     MOVIE_SCAN_M2TS	((__u64)1<<32)
#define 	MOVIE_SCAN_ALL  (MOVIE_SCAN_AVI | MOVIE_SCAN_MP4 | MOVIE_SCAN_MPG | MOVIE_SCAN_RM | MOVIE_SCAN_RM | MOVIE_SCAN_FLV | MOVIE_SCAN_PMP \
								| MOVIE_SCAN_3GP | MOVIE_SCAN_VOB | MOVIE_SCAN_DAT | MOVIE_SCAN_RMVB | MOVIE_SCAN_MKV | MOVIE_SCAN_WMV | MOVIE_SCAN_ASF \
								| MOVIE_SCAN_PSR | MOVIE_SCAN_TP | MOVIE_SCAN_TS | MOVIE_SCAN_MOV | MOVIE_SCAN_M2TS)

/*music*/
#define 	MUSIC_SCAN_MP3	((__u64)1<<33)
#define     MUSIC_SCAN_MP2	((__u64)1<<34)
#define     MUSIC_SCAN_MP1	((__u64)1<<35)
#define     MUSIC_SCAN_WMA	((__u64)1<<36)
#define     MUSIC_SCAN_OGG	((__u64)1<<37)
#define     MUSIC_SCAN_APE	((__u64)1<<38)
#define     MUSIC_SCAN_FLAC	((__u64)1<<39)
#define     MUSIC_SCAN_AC3	((__u64)1<<40)
#define     MUSIC_SCAN_DTS	((__u64)1<<41)
#define     MUSIC_SCAN_RA	((__u64)1<<42)
#define     MUSIC_SCAN_AAC	((__u64)1<<43)
#define     MUSIC_SCAN_AMR	((__u64)1<<44)
#define     MUSIC_SCAN_ATRC	((__u64)1<<45)
#define     MUSIC_SCAN_MID	((__u64)1<<46)
#define     MUSIC_SCAN_WAV 	((__u64)1<<47)
#define 	MUSIC_SCAN_ALL  (MUSIC_SCAN_MP3 | MUSIC_SCAN_MP2  | MUSIC_SCAN_MP1  | MUSIC_SCAN_WMA\
| MUSIC_SCAN_OGG | MUSIC_SCAN_APE | MUSIC_SCAN_FLAC	 | MUSIC_SCAN_AC3 | MUSIC_SCAN_DTS\
| MUSIC_SCAN_RA | MUSIC_SCAN_AAC | MUSIC_SCAN_AMR | MUSIC_SCAN_ATRC | MUSIC_SCAN_MID | MUSIC_SCAN_WAV)

/*photo*/
#define 	PHOTO_SCAN_JFIF	((__u64)1<<48)
#define     PHOTO_SCAN_JPEG	((__u64)1<<49)
#define     PHOTO_SCAN_PNG	((__u64)1<<50)
#define     PHOTO_SCAN_BMP	((__u64)1<<51)
#define     PHOTO_SCAN_JPG	((__u64)1<<52)
#define     PHOTO_SCAN_GIF	((__u64)1<<53)
#define 	PHOTO_SCAN_ALL  (PHOTO_SCAN_JFIF | PHOTO_SCAN_JPEG  | PHOTO_SCAN_PNG  | PHOTO_SCAN_BMP\
	| PHOTO_SCAN_JPG | PHOTO_SCAN_GIF)

#define     IMAGE_SCAN_IMG  ((__u64)1<<54)
/* 
 * to store scaned entry information.
 */
typedef	struct ScanedEntry {
	
	__u64  Format; 	/* format of  the ebook entry */
	char   *Path;	  	/* file full path 	  */
} ScanedEntry_t;

typedef	__hdle	HSCAN;

typedef __s32 (*prat_search_cb)(void* arg, __s32 cur_cnt);
extern prat_search_cb g_rat_search_cb;
extern void* g_rat_search_arg;


HSCAN  ScanFilesBegin(const char *pDir,
				   	  __u64 FileTypes,
				      int OnceCnt);
int    ScanMoveToFirst(HSCAN hScan);
int    ScanMoveToNext (HSCAN hScan);
int    ScanMoveToPrev (HSCAN hScan);
__bool ScanIsDone     (HSCAN hScan);
void   ScanFree       (HSCAN hScan);
ScanedEntry_t *ScanGetCurrent(HSCAN hScan);
int    ScanGetCurCnt  (HSCAN hScan);


extern __bool __ValidFileType(const char *file, __u64 Types, int *Format);

#endif		/* __SCAN_H__ */

