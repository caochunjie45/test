/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                 G2D Driver
*
*                                    (c) Copyright 2012-2016, Yupu.Tang China
*                                             All Rights Reserved
*
* File    : drv_g2d.h
* By      : Yupu.Tang
* Version : v1.0
* Date    : 2012-2-16
* Descript:
* Update  : date                auther      ver     notes
*           2012-2-16 15:37:13  Yupu.Tang   2.0     build the file.
*********************************************************************************************************
*/
#ifndef __DRIVER_G2D_H__
#define __DRIVER_G2D_H__

/* mixer data format */
typedef enum {
	/* share data format */
	G2D_FMT_ARGB8888		= (0x0),
	G2D_FMT_BGRA8888		= (0x1),
	G2D_FMT_ABGR8888		= (0x2),
	G2D_FMT_RGBA8888		= (0x3),
	
	G2D_FMT_XRGB8888		= (0x4),
	G2D_FMT_BGRX8888		= (0x5),
	G2D_FMT_XBGR8888		= (0x6),
	G2D_FMT_RGBX8888		= (0x7),
	
	G2D_FMT_ARGB4444		= (0x8),
	G2D_FMT_ABGR4444		= (0x9),
	G2D_FMT_RGBA4444		= (0xA),
	G2D_FMT_BGRA4444		= (0xB),

	G2D_FMT_ARGB1555		= (0xC),
	G2D_FMT_ABGR1555		= (0xD),
	G2D_FMT_RGBA5551		= (0xE),
	G2D_FMT_BGRA5551		= (0xF),

	G2D_FMT_RGB565			= (0x10),
	G2D_FMT_BGR565			= (0x11),

	G2D_FMT_IYUV422			= (0x12),

	G2D_FMT_8BPP_MONO		= (0x13),
	G2D_FMT_4BPP_MONO		= (0x14),
	G2D_FMT_2BPP_MONO		= (0x15),
	G2D_FMT_1BPP_MONO		= (0x16),

	G2D_FMT_PYUV422UVC		= (0x17),
	G2D_FMT_PYUV420UVC		= (0x18),
	G2D_FMT_PYUV411UVC		= (0x19),
	
	/* just for output format */
	G2D_FMT_PYUV422			= (0x1A),
	G2D_FMT_PYUV420			= (0x1B),
	G2D_FMT_PYUV411			= (0x1C),
	
	/* just for input format */	
	G2D_FMT_8BPP_PALETTE	= (0x1D),
	G2D_FMT_4BPP_PALETTE	= (0x1E),
	G2D_FMT_2BPP_PALETTE	= (0x1F),
	G2D_FMT_1BPP_PALETTE	= (0x20),
			
}g2d_data_fmt;	

/* pixel sequence in double word */
typedef enum {
	G2D_SEQ_NORMAL = 0x0,
	
	/* for interleaved yuv422 */
    G2D_SEQ_VYUY   = 0x1,				/* pixel 0在低16位 */
    G2D_SEQ_YVYU   = 0x2,				/* pixel 1在低16位 */
    
	/* for uv_combined yuv420 */
    G2D_SEQ_VUVU   = 0x3,  
    
	/* for 16bpp rgb */
    G2D_SEQ_P10    = 0x4,				/* pixel 0在低16位 */
    G2D_SEQ_P01    = 0x5,				/* pixel 1在低16位 */
    
	/* planar format or 8bpp rgb */
    G2D_SEQ_P3210  = 0x6,				/* pixel 0在低8位 */
    G2D_SEQ_P0123  = 0x7,				/* pixel 3在低8位 */
    
	/* for 4bpp rgb */
    G2D_SEQ_P76543210  = 0x8,			/* 7,6,5,4,3,2,1,0 */
    G2D_SEQ_P67452301  = 0x9,			/* 6,7,4,5,2,3,0,1 */
    G2D_SEQ_P10325476  = 0xA,			/* 1,0,3,2,5,4,7,6 */
    G2D_SEQ_P01234567  = 0xB,			/* 0,1,2,3,4,5,6,7 */
    
	/* for 2bpp rgb */
    G2D_SEQ_2BPP_BIG_BIG       = 0xC,	/* 15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0 */
    G2D_SEQ_2BPP_BIG_LITTER    = 0xD,	/* 12,13,14,15,8,9,10,11,4,5,6,7,0,1,2,3 */
    G2D_SEQ_2BPP_LITTER_BIG    = 0xE,	/* 3,2,1,0,7,6,5,4,11,10,9,8,15,14,13,12 */
    G2D_SEQ_2BPP_LITTER_LITTER = 0xF,	/* 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 */
    
	/* for 1bpp rgb */
    G2D_SEQ_1BPP_BIG_BIG       = 0x10,	/* 31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0 */
    G2D_SEQ_1BPP_BIG_LITTER    = 0x11,	/* 24,25,26,27,28,29,30,31,16,17,18,19,20,21,22,23,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7 */
    G2D_SEQ_1BPP_LITTER_BIG    = 0x12,	/* 7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8,23,22,21,20,19,18,17,16,31,30,29,28,27,26,25,24 */
    G2D_SEQ_1BPP_LITTER_LITTER = 0x13,	/* 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 */
}g2d_pixel_seq;


typedef enum {  
	G2D_FIL_NONE			= 0x00000000,
	G2D_FIL_PIXEL_ALPHA		= 0x00000001,
	G2D_FIL_PLANE_ALPHA		= 0x00000002,
	G2D_FIL_MULTI_ALPHA		= 0x00000004,
}g2d_fillrect_flags;

typedef enum {
	G2D_BLT_NONE			= 0x00000000,
	G2D_BLT_PIXEL_ALPHA		= 0x00000001,
	G2D_BLT_PLANE_ALPHA		= 0x00000002,
	G2D_BLT_MULTI_ALPHA		= 0x00000004,
	G2D_BLT_SRC_COLORKEY	= 0x00000008,
	G2D_BLT_DST_COLORKEY	= 0x00000010,
	G2D_BLT_FLIP_HORIZONTAL	= 0x00000020,
	G2D_BLT_FLIP_VERTICAL	= 0x00000040,
	G2D_BLT_ROTATE90		= 0x00000080,
	G2D_BLT_ROTATE180		= 0x00000100,
	G2D_BLT_ROTATE270		= 0x00000200,
	G2D_BLT_MIRROR45		= 0x00000400,
	G2D_BLT_MIRROR135		= 0x00000800,
}g2d_blt_flags;

typedef enum {
	G2D_MAK_NORMAL			= 0xAAAA,	/* no op(remain the dst.Value)										*/
	G2D_MAK_SRCCOPY			= 0xCCCC,	/* dst.Value = src.Value											*/
	G2D_MAK_SRCPAINT		= 0xEEEE,	/* dst.Value = src.Value | dst.Value								*/
	G2D_MAK_SRCAND			= 0x8888,	/* dst.Value = src.Value & dst.Value								*/
	G2D_MAK_SRCINVERT		= 0x6666,	/* dst.Value = src.Value ^ dst.Value								*/
	G2D_MAK_SRCERASE		= 0x4444,	/* dst.Value = src.Value & ~dst.Value								*/
	G2D_MAK_NOTSRCCOPY		= 0x3333,	/* dst.Value = ~src.Value											*/
	G2D_MAK_NOTSRCERASE		= 0x1111,	/* dst.Value = ~(src.Value | dst.Value)								*/
	G2D_MAK_MERGECOPY		= 0xC0C0,	/* dst.Value = src.Value & brush.Value								*/
	G2D_MAK_MERGEPAINT		= 0xBBBB,	/* dst.Value = ~src.Value | dst.Value								*/
	G2D_MAK_PATCOPY			= 0xF0F0,	/* dst.Value = brush.Value											*/
	G2D_MAK_PATPAINT		= 0xFBFB,	/* dst.Value = brush.Value | ~src.Value | dst.Value					*/
	G2D_MAK_PATINVERT		= 0x5A5A,	/* dst.Value = brush.Value ^ dst.Value								*/
	G2D_MAK_DSTINVERT		= 0x5555,	/* dst.Value = ~dst.Value											*/
	G2D_MAK_BLACKNESS		= 0x0000,	/* dst.Value = 0													*/
	G2D_MAK_WHITENESS		= 0xFFFF,	/* dst.Value = 0xFFFFFFFF											*/
	G2D_MAK_FILL			= 0xAAF0,	/* dst.Value = brush.Value where mask is set						*/
	G2D_MAK_2222			= 0x2222,	/* dst.Value = (~src.Value) & dst.Value								*/
	G2D_MAK_ACAC			= 0xACAC,	/* dst.Value = ((src.Value ^ dst.Value) & brush.Value) ^ src.Value?	*/
	G2D_MAK_B8B8			= 0xB8B8,	/* dst.Value = (brush.Value & ~src.Value) | (src.Value & dst.Value?	*/
	G2D_MAK_E2E2			= 0xE2E2,	/* dst.Value = (dst.Value & ~src.Value) | (brush.Value & src.Value?	*/
}g2d_maskblt_flags;

/* flip rectangle struct */
typedef struct {
	__s32		x;			/* left top point coordinate x */
	__s32		y;			/* left top point coordinate y */
	__u32		w;			/* rectangle width */
	__u32		h;			/* rectangle height */
}g2d_rect;

/* image struct */
typedef struct {
	__u32			 addr[3];	/* base addr of image frame buffer in byte */
	__u32			 w;			/* width of image frame buffer in pixel */
	__u32			 h;			/* height of image frame buffer in pixel */
	g2d_data_fmt	 format;	/* pixel format of image frame buffer */
	g2d_pixel_seq	 pixel_seq;	/* pixel sequence of image frame buffer */
}g2d_image;
	
typedef struct {
	g2d_fillrect_flags	 flag;
	g2d_image			 dst_image;
	g2d_rect			 dst_rect;

	__u32				 color;		/* fill color */
	__u32				 alpha;		/* plane alpha value */

}g2d_fillrect;

typedef struct {
	g2d_blt_flags		 flag;
	g2d_image			 src_image; 
	g2d_rect			 src_rect;
	
	g2d_image			 dst_image;
	__s32				 dst_x;		/* left top point coordinate x of dst rect */
	__s32				 dst_y;		/* left top point coordinate y of dst rect */
	
	__u32				 color;		/* colorkey color */
	__u32				 alpha;		/* plane alpha value */

}g2d_blt;

typedef struct {
	g2d_blt_flags		 flag;
	g2d_image			 src_image;
	g2d_rect			 src_rect;
	
	g2d_image			 dst_image;
	g2d_rect			 dst_rect;
	
	__u32				 color;		/* colorkey color */	
	__u32				 alpha;		/* plane alpha value */
	
}g2d_stretchblt;

typedef struct {
	__u32				flag;		/* dwrop */
	g2d_image	 		dst_image;
	__s32		 		dst_x;
	__s32		 		dst_y;
			
	g2d_image	 		src_image;
	g2d_rect	 		src_rect;
	
	g2d_image	 		mask_image;
	__s32		 		mask_x;
	__s32		 		mask_y;
	__u32				color;		/* brush color */
	
}g2d_maskblt;

typedef struct {
	__u8 r;
	__u8 g;
	__u8 b;
	__u8 alpha;
} g2d_swcolor_t;

typedef struct {
	__u32		*pbuffer;
	__u32		 size;
	
}g2d_palette;

typedef enum
{
    G2D_CMD_NONE     		=	0x0,
	G2D_CMD_BITBLT			=	0x1,
	G2D_CMD_FILLRECT		=	0x2,
	G2D_CMD_STRETCHBLT		=	0x3,
	G2D_CMD_PALETTE_TBL		=	0x4,
	G2D_CMD_MASKBLT			=	0x5,
}g2d_cmd_t;

#endif	// __DRIVER_G2D_H__
