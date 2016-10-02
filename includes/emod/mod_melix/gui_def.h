/*
**************************************************************************************************************************
*													        eMOD
*						            		the Easy Portable/Player Develop Kits
*									   				melix interface module 
*
*                             		 Copyright(C), 2011-2012, Softwinner Microelectronic Co., Ltd.
*									               All Rights Reserved
*
* File name		:	gui_def.h
* Author		: 	Derek
* Version 		: 	V1.00
* Date    		: 	2011-12-27
* Description	:
* Others 		:
* History 		:
* <Author>           <time>          <version>      <description>
* Derek Lai		    2011-12-27         1.0.0          Create File
*
**************************************************************************************************************************
*/

#ifndef __GUI_DEF_H__
#define __GUI_DEF_H__

#define LOBYTE(w)               ((__u8)(w))
#define HIBYTE(w)               ((__u8)(((__u16)(w) >> 8) & 0xFF))
#define MAKEWORD(low, high)     ((__u16)(((__u8)(low)) | (((__u16)((__u8)(high))) << 8)))
#define LOWORD(l)               ((__u16)(__u32)(l))
#define HIWORD(l)               ((__u16)((((__u32)(l)) >> 16) & 0xFFFF))
#define LOSWORD(l)              ((__s16)(__u32)(l))
#define HISWORD(l)              ((__s16)((((__u32)(l)) >> 16) & 0xFFFF))
#define MAKELONG(low, high)     ((__u32)(((__u16)(low)) | (((__u32)((__u16)(high))) << 16)))


typedef unsigned char       GuiAlpha;
typedef unsigned int        GuiColor;
typedef unsigned int        GuiUint32;
typedef unsigned short      GuiUint16;
typedef unsigned char       GuiUint8;
typedef signed int          GuiSint32;
typedef signed long long    GuiSint64;
typedef unsigned long long  GuiUint64;
typedef signed short        GuiSint16;
typedef signed char         GuiSint8;
typedef unsigned int        GuiPixel;
typedef unsigned char       GuiBool;

typedef GuiUint32           HMEM;
typedef GuiUint32           HDC;
typedef GuiUint32           HWIN;
typedef GuiUint32           HLAYER;
typedef GuiUint32           HBITMAP;
typedef GuiUint32           HString;
typedef GuiUint32           HFONT;
typedef GuiUint32           HPALETTE;
typedef GuiUint32           HLOOPER;

typedef void*    			HXFB;


typedef ES_FILE*            GuiFile;
typedef GuiUint32           GuiHandle;
typedef GuiUint32           HTHEME;
typedef GuiUint32           HLANGUAGE;

/*HVIEW is view_data_t's handle*/
typedef void*   Hview;

typedef struct {__s32 x, y;} GUI_POINT;

typedef char			Byte;


#if(GUI_WIN_OS == GUI_OS_EPOS)
	#define   GUI_TRUE		                EPDK_TRUE
	#define   GUI_FALSE		                EPDK_FALSE
	#define   GUI_WRN                       __wrn
	#define   GUI_INF                       __inf
	#define   GUI_ERR                       __err
	#define   GUI_MSG                       __msg
#endif
/*
 * @internal
 *
 * Encodes format constants in the following way (bit 31 - 0):
 *
 * lkjj:hhgg | gfff:eeed | cccc:bbbb | baaa:aaaa
 *
 * a) pixelformat index<br>
 * b) effective color (or index) bits per pixel of format<br>
 * c) effective alpha bits per pixel of format<br>
 * d) alpha channel present<br>
 * e) bytes per "pixel in a row" (1/8 fragment, i.e. bits)<br>
 * f) bytes per "pixel in a row" (decimal part, i.e. bytes)<br>
 * g) smallest number of pixels aligned to byte boundary (minus one)<br>
 * h) multiplier for planes minus one (1/4 fragment)<br>
 * j) multiplier for planes minus one (decimal part)<br>
 * k) color and/or alpha lookup table present<br>
 * l) alpha channel is inverted
 */
#define GUI_PIXELFORMAT( index, color_bits, alpha_bits, has_alpha,     \
                                 row_bits, row_bytes, align, mul_f, mul_d,     \
                                 has_lut, inv_alpha )                          \
     ( (((index     ) & 0x7F)      ) |                                         \
       (((color_bits) & 0x1F) <<  7) |                                         \
       (((alpha_bits) & 0x0F) << 12) |                                         \
       (((has_alpha ) ? 1 :0) << 16) |                                         \
       (((row_bits  ) & 0x07) << 17) |                                         \
       (((row_bytes ) & 0x07) << 20) |                                         \
       (((align     ) & 0x07) << 23) |                                         \
       (((mul_f     ) & 0x03) << 26) |                                         \
       (((mul_d     ) & 0x03) << 28) |                                         \
       (((has_lut   ) ? 1 :0) << 30) |                                         \
       (((inv_alpha ) ? 1 :0) << 31) )

/*
 * Pixel format of a surface.
 */
typedef enum 
{
     GUIPF_UNKNOWN   = 0x00000000,  /* unknown or unspecified format */

     GUIPF_MONO1BPP  = GUI_PIXELFORMAT(  0, 1, 0, 0, 1, 0, 7, 0, 0, 0, 0 ),

     GUIPF_LUT1BPP   = GUI_PIXELFORMAT(  1, 1, 1, 0, 1, 0, 7, 0, 0, 1, 0 ),

     GUIPF_MONO2BPP  = GUI_PIXELFORMAT(  2, 2, 0, 0, 1, 0, 3, 0, 0, 0, 0 ),

     GUIPF_LUT2BPP   = GUI_PIXELFORMAT(  3, 2, 1, 0, 1, 0, 3, 0, 0, 1, 0 ),

     GUIPF_MONO4BPP  = GUI_PIXELFORMAT(  4, 4, 0, 0, 1, 0, 1, 0, 0, 0, 0 ),

     GUIPF_LUT4BPP   = GUI_PIXELFORMAT(  5, 4, 1, 0, 1, 0, 1, 0, 0, 1, 0 ),

     GUIPF_MONO8BPP  = GUI_PIXELFORMAT(  6, 8, 0, 0, 1, 0, 0, 0, 0, 0, 0 ),

     GUIPF_LUT8BPP   = GUI_PIXELFORMAT(  7, 8, 1, 1, 0, 1, 0, 0, 0, 1, 0 ),

     /* 16 bit   BGR (2 byte, nothing @15, blue 5@10, green 5@5, red 5@0) */
     GUIPF_RGB565    = GUI_PIXELFORMAT(  8, 16, 0, 0, 0, 2, 0, 0, 0, 0, 0 ),

     /* 16 bit   BGR (2 byte, nothing @15, blue 5@10, green 5@5, red 5@0) */
     GUIPF_BGR565    = GUI_PIXELFORMAT(  9, 16, 0, 0, 0, 2, 0, 0, 0, 0, 0 ),

     /* 16 bit   BGR (2 byte, nothing @15, blue 5@10, green 5@5, red 5@0) */
     GUIPF_RGB556    = GUI_PIXELFORMAT( 10, 16, 0, 0, 0, 2, 0, 0, 0, 0, 0 ),

     /* 16 bit   BGR (2 byte, nothing @15, blue 5@10, green 5@5, red 5@0) */
     GUIPF_RGB655    = GUI_PIXELFORMAT( 11, 16, 0, 0, 0, 2, 0, 0, 0, 0, 0 ),
     
     /* 16 bit  ARGB (2 byte, alpha 1@15, red 5@10, green 5@5, blue 5@0) */
     GUIPF_ARGB1555  = GUI_PIXELFORMAT( 12, 15, 1, 1, 0, 2, 0, 0, 0, 0, 0 ),

     /* 16 bit   BGR (2 byte, nothing @15, blue 5@10, green 5@5, red 5@0) */
     GUIPF_BGRA5551  = GUI_PIXELFORMAT( 13, 16, 1, 1, 0, 2, 0, 0, 0, 0, 0 ),

     /* 16 bit  ARGB (2 byte, alpha 4@12, red 4@8, green 4@4, blue 4@0) */
     GUIPF_ARGB4444  = GUI_PIXELFORMAT( 14, 12, 4, 1, 0, 2, 0, 0, 0, 0, 0 ),

     /* 16 bit  RGBA (2 byte, red 4@12, green 4@8, blue 4@4, alpha 4@0) */
     GUIPF_RGBA4444  = GUI_PIXELFORMAT( 15, 12, 4, 1, 0, 2, 0, 0, 0, 0, 0 ),

     /* 24 bit   RGB (3 byte, red 8@16, green 8@8, blue 8@0) */
     GUIPF_RGB888    = GUI_PIXELFORMAT( 16, 24, 0, 0, 0, 3, 0, 0, 0, 0, 0 ),

     /* 24 bit   RGB (4 byte, nothing@24, red 8@16, green 8@8, blue 8@0) */
     GUIPF_RGB8888   = GUI_PIXELFORMAT( 17, 24, 0, 0, 0, 4, 0, 0, 0, 0, 0 ),

     /* 32 bit  ARGB (4 byte, alpha 8@24, red 8@16, green 8@8, blue 8@0) */
     GUIPF_ARGB8888  = GUI_PIXELFORMAT( 18, 24, 8, 1, 0, 4, 0, 0, 0, 0, 0 ),

     /* 16 bit   BGR (2 byte, nothing @15, blue 5@10, green 5@5, red 5@0) */
     GUIPF_BGRA8888  = GUI_PIXELFORMAT( 19, 32, 8, 1, 0, 4, 0, 0, 0, 0, 0 )
} GuiPixelFmt;
/* Number of pixelformats defined */
#define GUI_NUM_PIXELFORMATS            20

/* These macros extract information about the pixel format. */
#define GUI_PIXELFORMAT_INDEX(fmt)      (((fmt) & 0x0000007F)      )
#define GUI_COLOR_BITS_PER_PIXEL(fmt)   (((fmt) & 0x00000F80) >>  7)
#define GUI_ALPHA_BITS_PER_PIXEL(fmt)   (((fmt) & 0x0000F000) >> 12)
#define GUI_PIXELFORMAT_HAS_ALPHA(fmt)  (((fmt) & 0x00010000) !=  0)
#define GUI_BITS_PER_PIXEL(fmt)         (((fmt) & 0x007E0000) >> 17)
#define GUI_BYTES_PER_PIXEL(fmt)        (((fmt) & 0x00700000) >> 20)
#define GUI_BYTES_PER_LINE(fmt,width)   (((((fmt) & 0x007E0000) >> 17) * (width) + 7) >> 3)
#define GUI_PIXELFORMAT_ALIGNMENT(fmt)  (((fmt) & 0x03800000) >> 23)
#define GUI_PLANE_MULTIPLY(fmt,height)  ((((((fmt) & 0x3C000000) >> 26) + 4) * (height)) >> 2)
#define GUI_PIXELFORMAT_IS_INDEXED(fmt) (((fmt) & 0x40000000) !=  0)
#define GUI_PLANAR_PIXELFORMAT(fmt)     (((fmt) & 0x3C000000) !=  0)
#define GUI_PIXELFORMAT_INV_ALPHA(fmt)  (((fmt) & 0x80000000) !=  0) 

typedef  enum
{
	GUI_CHARSET_GB2312  = 0,       // 简体中文
	GUI_CHARSET_UTF8       ,       // utf8
	GUI_CHARSET_UTF16BE    ,       // utf16be
	GUI_CHARSET_UTF16LE    ,       // utf16le
    GUI_CHARSET_BIG5       ,       // 繁体中文
    GUI_CHARSET_GBK        ,       // 中文
    GUI_CHARSET_SJIS       ,       // 日文
    GUI_CHARSET_EUC_JP     ,       // 日文
    GUI_CHARSET_EUC_KR     ,       // 韩文
    GUI_CHARSET_KIO8_R     ,       // 俄文
    GUI_CHARSET_ISO_8859_1 ,       // cp1252.西欧语言
    GUI_CHARSET_ISO_8859_2 ,       // 中欧语言
    GUI_CHARSET_ISO_8859_3 ,       // 南欧语言
    GUI_CHARSET_ISO_8859_4 ,       // 北欧语言
    GUI_CHARSET_ISO_8859_5 ,       // 西里尔字母
    GUI_CHARSET_ISO_8859_6 ,       // 阿拉伯语
    GUI_CHARSET_ISO_8859_7 ,       // 希腊语
    GUI_CHARSET_ISO_8859_8 ,       // 希伯来语
    GUI_CHARSET_ISO_8859_9 ,       // cp1254，土耳其语
    GUI_CHARSET_ISO_8859_10,       // 北欧斯堪的纳维亚语系
    GUI_CHARSET_ISO_8859_11,       // 泰语
    GUI_CHARSET_ISO_8859_12,       // 梵文
    GUI_CHARSET_ISO_8859_13,       // 波罗的海语系
    GUI_CHARSET_ISO_8859_14,       // 凯尔特人语系
    GUI_CHARSET_ISO_8859_15,       // 扩展了法语和芬兰语的西欧语系
    GUI_CHARSET_ISO_8859_16,       // 扩展的东南欧语系
    GUI_CHARSET_CP874      ,       // 泰文
    GUI_CHARSET_CP1250     ,       // 中欧
    GUI_CHARSET_CP1251     ,       // 西里尔文
    GUI_CHARSET_CP1253     ,       // 希腊文
    GUI_CHARSET_CP1255     ,       // 希伯来文
    GUI_CHARSET_CP1256     ,       // 阿拉伯文
    GUI_CHARSET_CP1257     ,       // 波罗的海文
    GUI_CHARSET_CP1258     ,       // 越南

    GUI_CHARSET_UNKNOWN = -1       // always the last one
}GuiCharset;

typedef struct tag_GUICOLORKEY
{
    GuiColor        min;
    GuiColor        max;
    GuiUint32       match;
}GuiColorkey;

/*矩形区域定义*/
typedef struct tag_GUIRECT
{
    GuiSint32       left;
    GuiSint32       top;
    GuiSint32       right;
    GuiSint32       bottom;
}GuiRect;

typedef struct tag_GUIRECTE
{
    GuiSint32       x;
    GuiSint32       y;
    GuiUint32       width;
    GuiUint32       height;
}GuiRectE;

/*坐标点定义*/
typedef struct tag_GUIPOS
{
    GuiSint32       x;
    GuiSint32       y;
}GuiPos;

typedef struct tag_GUISIZE
{
    GuiUint32       width;
    GuiUint32       height;
}GuiSize;


/*调色板定义*/
typedef struct tag_GUIPALETTE
{
    GuiUint32       ncolors;

    GuiColor       *colors;
}GuiPalette;


typedef struct tag_GUIFBFMT
{
    GuiPixelFmt         pixelfmt;           /* 像素的格式                                                   */
    GuiUint16			br_swap;            /* blue red color swap flag                                     */
    GuiUint16           pixseq;             /* 图像流的存储顺序                                             */
    GuiPalette         *palette;
}GuiFBFmt;

typedef struct tag_GUIWORDINFO
{
    GuiUint32           len;
    GuiUint8            delimiter;
    GuiUint32           nr_delimiters;
}GuiWordInfo;

/**
 * Arc structure.
 */
typedef struct tag_GUIARC
{
    GuiSint32       x;
    GuiSint32       y;
    GuiSint32       width;
    GuiSint32       height;
    GuiSint32       angle1;
    GuiSint32       angle2;
} GuiARC;

/*返回值定义*/
typedef enum GUI_RETVAL
{
	GUI_RET_TURE		= 1,
	GUI_RET_FALSE		= 0,
    GUI_RET_OK          = 0,
    GUI_RET_FAIL        = -1,
    GUI_RET_INPARAERR   = -2,
    GUI_RET_MALLOCERR   = -3,
    GUI_RET_LAYERERR    = -4,
}GuiRetVal; 

typedef enum
{
    GUI_DISPDIR_NORMAL,
    GUI_DISPDIR_ROTATE90,
    GUI_DISPDIR_ROTATE180,
    GUI_DISPDIR_ROTATE270
}GuiDisplayDirection;

typedef struct tag_HALDISPFB
{
    GuiSize                     size;               /* frame buffer的长宽                                           */
    void*                       addr[3];            /* frame buffer的内容地址，对于rgb类型，只有addr[0]有效         */
    GuiFBFmt                    fmt;
}GuiDispFB;

typedef enum 
{
     GUIDRAW_NOFX               = 0x00000000, /* uses none of the effects */
     GUIDRAW_SET                = 0x00000000,
     GUIDRAW_BLEND              = 0x00000001, /* uses alpha from color */
     GUIDRAW_DST_COLORKEY       = 0x00000002, /* write to destination only if the destination pixel
                                                matches the destination color key */
     GUIDRAW_SRC_PREMULTIPLY    = 0x00000004, /* multiplies the color's rgb channels by the alpha
                                                channel before drawing */
     GUIDRAW_DST_PREMULTIPLY    = 0x00000008, /* modulates the dest. color with the dest. alpha */
     GUIDRAW_DEMULTIPLY         = 0x00000010, /* divides the color by the alpha before writing the
                                                data to the destination */
     GUIDRAW_XOR                = 0x00000020  /* bitwise xor the destination pixels with the
                                                specified color after premultiplication */
} GUIDrawFlags;

/*blitting flags*/                                  
typedef enum                                        
{                                                   
     GUIBLIT_NOFX                    = 0x00000000,   /* uses none of the effects */
                                                    
     GUIBLIT_BLEND_ALPHACHANNEL      = 0x00000001,   /* enables blending and uses alphachannel from source */
     GUIBLIT_BLEND_COLORALPHA        = 0x00000002,   /* enables blending and uses alpha value from color */
     GUIBLIT_BLEND_BOTHALPHA         = 0x00000004,   /* enables blending and uses alpha value from both color and alphachannel*/
                                                    
     GUIBLIT_BLEND_OUTALPHA_SRC	    = 0x00000008,   
     GUIBLIT_BLEND_OUTALPHA_DST      = 0x00000010,   
     GUIBLIT_BLEND_OUTALPHA_MODULATED= 0x00000020,   
	                                                
     GUIBLIT_SRC_COLORKEY            = 0x00000100,   /* don't blit pixels matching the source color key */
     GUIBLIT_DST_COLORKEY            = 0x00000200,   /* write to destination only if the destination pixel m1tches the destination color key */

     GUIBLIT_ROTATE90                = 0x00001000,    /* rotate the image by 90 degree */
     GUIBLIT_ROTATE180               = 0x00002000,   /* rotate the image by 180 degree */
     GUIBLIT_ROTATE270               = 0x00004010,   /* rotate the image by 270 degree */
     GUIBLIT_FLIP_HORIZONTAL         = 0x00008000,   /* flip the image horizontally */
     GUIBLIT_FLIP_VERTICAL           = 0x00010000,
     
     GUIBLIT_XOR                     = 0x00020000,   /* bitwise xor the destination pixels with the source pixels after premultiplication */
} GuiBlittingFlags;

/*gdi define*/
#define GDI_BLUE          				0xffFF0000
#define GDI_GREEN         				0xff00FF00
#define GDI_RED           				0xff0000FF
#define GDI_CYAN          				0xffFFFF00
#define GDI_MAGENTA       				0xffFF00FF
#define GDI_YELLOW        				0xff00FFFF
#define GDI_LIGHTBLUE     				0xffFF8080
#define GDI_LIGHTGREEN    				0xff80FF80
#define GDI_LIGHTRED      				0xff8080FF
#define GDI_LIGHTCYAN     				0xffFFFF80
#define GDI_LIGHTMAGENTA  				0xffFF80FF
#define GDI_LIGHTYELLOW   				0xff80FFFF
#define GDI_DARKBLUE      				0xff800000
#define GDI_DARKGREEN     				0xff008000
#define GDI_DARKRED       				0xff000080
#define GDI_DARKCYAN      				0xff808000
#define GDI_DARKMAGENTA   				0xff800080
#define GDI_DARKYELLOW    				0xff008080
#define GDI_WHITE         				0xffFFFFFF
#define GDI_LIGHTGRAY     				0xffD3D3D3
#define GDI_GRAY          				0xff808080
#define GDI_DARKGRAY      				0xff404040
#define GDI_BLACK         				0xff000000
#define GDI_BROWN                       0xff2A2AA5

/*gdi绘制模式*/
#define GDI_DRAWMODE_NORMAL 			(0)
#define GDI_DRAWMODE_XOR    			(1<<0)
#define GDI_DRAWMODE_TRANS  			(1<<1)
#define GDI_DRAWMODE_REV    			(1<<2)
#define GDI_DRAWMODE_SHADOW    			(1<<3)
#define GDI_DRAWMODE_BLEND              (1<<4)  /* uses alpha from color */
#define GDI_DRAWMODE_SRC_PREMULTIPLY    (1<<5)
#define GDI_DRAWMODE_DST_PREMULTIPLY    (1<<6)

#define GDI_FONTMODE_8BPP32             (0)
#define GDI_FONTMODE_8BPP256            (1<<0)
#define GDI_FONTMODE_8BPP128_1         	(1<<1)    // font color 1
#define GDI_FONTMODE_8BPP128_2         	(1<<2)    // font color 2
#define GDI_FONTMODE_32BPPSHADOW   		(1<<3)   // 32bpp font shadow
#define GDI_FONTMODE_8BPP32_FRAME   	(1<<4)  // 8bpp32 font frame mode 

#define GDI_TEXTMODE_NORMAL  			GDI_DRAWMODE_NORMAL
#define GDI_TEXTMODE_XOR     			GDI_DRAWMODE_XOR
#define GDI_TEXTMODE_TRANS   			GDI_DRAWMODE_TRANS
#define GDI_TEXTMODE_REV     			GDI_DRAWMODE_REV
#define GDI_TM_NORMAL        			GDI_DRAWMODE_NORMAL
#define GDI_TM_XOR           			GDI_DRAWMODE_XOR
#define GDI_TM_TRANS         			GDI_DRAWMODE_TRANS
#define GDI_TM_REV           			GDI_DRAWMODE_REV

/*Text Style*/
#define GDI_TS_NORMAL           		0x00000000
#define GDI_TS_UNDERLINE        		0x00000001
#define GDI_TS_STRIKETHRU       		0x00000002
#define GDI_TS_OVERLINE         		0x00000004
                                		
/* Pen Styles */                		
#define GDI_PS_NULL             		0x00000010
#define GDI_PS_SOLID            		0x00000011
#define GDI_PS_DASH             		0x00000012	/* -------  */
#define GDI_PS_DOT              		0x00000013	/* .......  */
#define GDI_PS_DASHDOT         			0x00000014	/* _._._._  */
#define GDI_PS_DASHDOTDOT				0x00000015	/* _.._.._  */
                                		
/*Pen shapes*/                  		
#define GDI_PS_ROUND        			0x00000000
#define GDI_PS_FLAT         			0x00000001
#define GDI_PS_SQUARE       			0x00000002
                                		
/* Brush Styles */              		
#define GDI_BS_NULL						0x00000020
#define GDI_BS_SOLID					0x00000021
#define GDI_BS_HATCHED					0x00000022
#define GDI_BS_AGAINST					0x00000023

typedef enum
{
  GDIPT_SOLID,
  GDIPT_ON_OFF_DASH,
  GDIPT_DOUBLE_DASH,
} GDIPenType;

typedef enum
{
  GDIBT_SOLID,
  GDIBT_TILED,
  GDIBT_STIPPLED,
  GDIBT_OPAQUE_STIPPLED
} GDIBrushType;

typedef enum
{
  GDIPT_CAP_BUTT,
  GDIPT_CAP_ROUND,
  GDIPT_CAP_PROJECTING
} GDIPTCapStyle;

typedef enum
{
  GDIPT_JOIN_MITER,
  GDIPT_JOIN_ROUND,
  GDIPT_JOIN_BEVEL
} GDIPTJoinStyle;

#define GDIROP_SET         0
#define GDIROP_AND         1
#define GDIROP_OR          2
#define GDIROP_XOR         3
                                		
                                		
/* Hatch Styles */              		
#define GDI_HS_HORIZONTAL       		0	/* ----- */
#define GDI_HS_VERTICAL         		1	/* ||||| */
#define GDI_HS_FDIAGONAL        		2	/* \\\\\ */
#define GDI_HS_BDIAGONAL        		3	/* ///// */
#define GDI_HS_CROSS            		4	/* +++++ */
#define GDI_HS_DIAGCROSS        		5	/* xxxxx */

//System Pre-defined GDI
#define WHITE_PEN						0x00000000
#define LTGRAY_PEN						0x00000001
#define GRAY_PEN						0x00000002
#define DKGRAY_PEN						0x00000003
#define BLACK_PEN						0x00000004
#define NULL_PEN						0x00000005
                            			
#define WHITE_BRUSH						0x00000006
#define LTGRAY_BRUSH					0x00000007
#define GRAY_BRUSH						0x00000008
#define DKGRAY_BRUSH					0x00000009
#define BLACK_BRUSH						0x0000000A
#define NULL_BRUSH						0x0000000B

#endif

