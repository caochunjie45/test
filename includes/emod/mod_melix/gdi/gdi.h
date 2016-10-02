/*
**************************************************************************************************************************
*													        eMOD
*						            		the Easy Portable/Player Develop Kits
*									   				melix gdi module 
*
*                             		 Copyright(C), 2011-2012, Softwinner Microelectronic Co., Ltd.
*									               All Rights Reserved
*
* File name		:	gdi.h
* Author		: 	Derek
* Version 		: 	V1.00
* Date    		: 	2012-02-17
* Description	:
* Others 		:
* History 		:
* <Author>           <time>          <version>      <description>
* Derek Lai		    2012-02-17         1.0.0          Create File
*
**************************************************************************************************************************
*/
#ifndef __GDI_H__
#define __GDI_H__

/*
      *********************************
      *                               *
      * Text and drawing mode defines *
      *                               *
      *********************************

These defines come in two flavors: the long version (.._DRAWMODE_..)
and the short ones (.._DM_..). They are identical, feel free to use
which ever one you like best.

*/

#define GUI_DRAWMODE_NORMAL  (0)
#define GUI_DRAWMODE_XOR     (1<<0)
#define GUI_DRAWMODE_TRANS   (1<<1)
#define GUI_DRAWMODE_REV     (1<<2)
#define GUI_DRAWMODE_SHADOW     (1<<3)
#define GUI_DM_NORMAL        LCD_DRAWMODE_NORMAL
#define GUI_DM_XOR           LCD_DRAWMODE_XOR
#define GUI_DM_TRANS         LCD_DRAWMODE_TRANS
#define GUI_DM_REV           LCD_DRAWMODE_REV

// mcl add font mode 
#define GUI_FONTMODE_8BPP32       LCD_FONTMODE_8BPP32      
#define GUI_FONTMODE_8BPP256     LCD_FONTMODE_8BPP256
#define GUI_FONTMODE_8BPP128_1     LCD_FONTMODE_8BPP128_1     //font color 1
#define GUI_FONTMODE_8BPP128_2    LCD_FONTMODE_8BPP128_2     // font color 2
#define GUI_FONTMODE_32BPPSHADOW  LCD_FONTMODE_32BPPSHADOW
#define GUI_FONTMODE_8BPP32_FRAME      LCD_FONTMODE_8BPP32_FRAME
#define  GUI_FONTMODE_32BPPMAG2      LCD_FONTMODE_32BPPMAG2
#define  GUI_FONTMODE_8BPP32_FRAME_MAG  LCD_FONTMODE_8BPP32_FRAME_MAG

#define GUI_TEXTMODE_NORMAL  LCD_DRAWMODE_NORMAL
#define GUI_TEXTMODE_XOR     LCD_DRAWMODE_XOR
#define GUI_TEXTMODE_TRANS   LCD_DRAWMODE_TRANS
#define GUI_TEXTMODE_REV     LCD_DRAWMODE_REV
#define GUI_TM_NORMAL        LCD_DRAWMODE_NORMAL
#define GUI_TM_XOR           LCD_DRAWMODE_XOR
#define GUI_TM_TRANS         LCD_DRAWMODE_TRANS
#define GUI_TM_REV           LCD_DRAWMODE_REV

/* Text alignment flags, horizontal */
#define GUI_TA_HORIZONTAL  (3<<0)
#define GUI_TA_LEFT        (0<<0)
#define GUI_TA_RIGHT	     (1<<0)
#define GUI_TA_CENTER	     (2<<0)
#define GUI_TA_HCENTER	   GUI_TA_CENTER  /* easier to remember :-)  */

/* Text alignment flags, vertical */
#define GUI_TA_VERTICAL   (3<<2)
#define GUI_TA_TOP	      (0<<2)
#define GUI_TA_BOTTOM	  (1<<2)
#define GUI_TA_BASELINE   (2<<2)
#define GUI_TA_VCENTER    (3<<2)

typedef enum GUI_WRAP_MODE
{
	GUI_WRAP_MODE_NONE,
	GUI_WRAP_MODE_WORD,
	GUI_WRAP_MODE_CHAR
}gui_wrap_mode_t;

/*Gray Font draw mode, added by Derek*/
#define GUI_GRAYFONT_DRAWMODE_AA		0  /*AntiAlias mode*/
#define GUI_GRAYFONT_DRAWMODE_NAA		1  /*No AntiAlias mode*/

/*********************************************************************
*
*       FONT structures
*/

/* Translation list. Translates a character code into up to 2
   indices of images to display on top of each other;
   '? -> index('a'), index('?) */
typedef struct {
  __s16 c0;
  __s16 c1;
} GUI_FONT_TRANSLIST;

typedef struct {
  __u16 FirstChar;
  __u16 LastChar;
  const GUI_FONT_TRANSLIST * pList;
} GUI_FONT_TRANSINFO;

typedef struct {
  __u8 XSize;
  __u8 XDist;
  __u8 BytesPerLine;
  const unsigned char * pData;
} GUI_CHARINFO;

typedef struct {
  __u8 XSize;
  __u8 YSize;
  __s8 XPos;
  __s8 YPos;
  __u8 XDist;
  const unsigned char * pData;
} GUI_CHARINFO_EXT;

typedef struct GUI_FONT_PROP {
  __u16 First;                                              /* First character               */
  __u16 Last;                                               /* Last character                */
  const GUI_CHARINFO         paCharInfo;     /* Address of first character    */
  struct GUI_FONT_PROP *pNext;          /* Pointer to next               */
} GUI_FONT_PROP;

typedef struct GUI_FONT_PROP_EXT {
  __u16 First;                                              /* First character               */
  __u16 Last;                                               /* Last character                */
  const GUI_CHARINFO_EXT         paCharInfo; /* Address of first character    */
  struct GUI_FONT_PROP_EXT *pNext;      /* Pointer to next               */
} GUI_FONT_PROP_EXT;

typedef struct {
  const unsigned char * pData;
  const __u8 * pTransData;
  const GUI_FONT_TRANSINFO * pTrans;
  __u16 FirstChar;
  __u16 LastChar;
  __u8 	XSize;
  __u8 	XDist;
  __u8 	BytesPerLine;
} GUI_FONT_MONO;


/*********************************************************************
*
*       FONT structures
*
* This structure is used when retrieving information about a font.
* It is designed for future expansion without incompatibilities.
*/
typedef struct {
  __u16 Flags;
  __u8 	Baseline;
  __u8 	LHeight;     /* height of a small lower case character (a,x) */
  __u8 	CHeight;     /* height of a small upper case character (A,X) */
} GUI_FONTINFO;

#define GUI_FONTINFO_FLAG_PROP (1<<0)    /* Is proportional */
#define GUI_FONTINFO_FLAG_MONO (1<<1)    /* Is monospaced */
#define GUI_FONTINFO_FLAG_AA   (1<<2)    /* Is an antialiased font */
#define GUI_FONTINFO_FLAG_AA2  (1<<3)    /* Is an antialiased font, 2bpp */
#define GUI_FONTINFO_FLAG_AA4  (1<<4)    /* Is an antialiased font, 4bpp */

/*********************************************************************
*
*       UNICODE Encoding
*/
typedef __u16  	tGUI_GetCharCode(const char *s);
typedef int  	tGUI_GetCharSize(const char *s);
typedef int  	tGUI_CalcSizeOfChar(__u16 Char);
typedef int  	tGUI_Encode(char *s, __u16 Char);

typedef struct {
  tGUI_GetCharCode*            pfGetCharCode;
  tGUI_GetCharSize*            pfGetCharSize;
  tGUI_CalcSizeOfChar*         pfCalcSizeOfChar;
  tGUI_Encode*                 pfEncode;
} GUI_UC_ENC_APILIST;

/*********************************************************************
*
*       FONT Encoding
*/
typedef int  tGUI_GetLineDistX(HDC hdc, const char *s, int Len);
typedef int  tGUI_GetLineLen(HDC hdc, const char *s, int MaxLen);
typedef void tGL_DispLine(HDC hdc, const char *s, int Len);

typedef struct {
  tGUI_GetLineDistX*          pfGetLineDistX;
  tGUI_GetLineLen*            pfGetLineLen;
  tGL_DispLine*               pfDispLine;
} tGUI_ENC_APIList;

extern const tGUI_ENC_APIList GUI_ENC_APIList_SJIS;
extern const tGUI_ENC_APIList GUI_ENC_APIList_EXT;

/*********************************************************************
*
*       FONT methods
*/
typedef struct GUI_FONT GUI_FONT;

typedef void GUI_DISPCHAR    (HDC hdc, __u16 c);
typedef int  GUI_GETCHARDISTX(HDC hdc, __u16 c);
typedef void GUI_GETFONTINFO (const GUI_FONT * pFont, GUI_FONTINFO * pfi);
typedef char GUI_ISINFONT    (const GUI_FONT * pFont, __u16 c);
typedef int  GUI_GETCHARINFO (__u16 c, GUI_CHARINFO_EXT * pInfo);

#define DECLARE_FONT(Type)                                     \
void GUI##Type##_DispChar    (HDC hdc, __u16 c);                         \
int  GUI##Type##_GetCharDistX(HDC hdc, __u16 c);                         \
void GUI##Type##_GetFontInfo (const GUI_FONT * pFont, GUI_FONTINFO * pfi); \
char GUI##Type##_IsInFont    (const GUI_FONT * pFont, __u16 c); \
int  GUI##Type##_GetCharInfo (__u16 c, GUI_CHARINFO_EXT * pInfo)

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/* MONO: Monospaced fonts */
DECLARE_FONT(MONO);
#define GUI_FONTTYPE_MONO       \
  GUIMONO_DispChar,             \
  GUIMONO_GetCharDistX,         \
  GUIMONO_GetFontInfo,          \
  GUIMONO_IsInFont,             \
  (GUI_GETCHARINFO *)0,         \
  (tGUI_ENC_APIList*)0

#if defined(__cplusplus)
  }
#endif

typedef struct
{
  int	magic;
  __u32 faceHandle; 
  __u8	*pFrame8bpp32; //  pointer to font frame buffer 
}FONT_EXT;

struct GUI_FONT {
  GUI_DISPCHAR     * pfDispChar; 
  GUI_GETCHARDISTX * pfGetCharDistX; 
  GUI_GETFONTINFO  * pfGetFontInfo; 
  GUI_ISINFONT     * pfIsInFont;
  GUI_GETCHARINFO  * pfGetCharInfo;
  const tGUI_ENC_APIList* pafEncode;
  __u8 YSize;
  __u8 YDist;
  __u8 XMag;
  __u8 YMag;
  union {
    const void              * pFontData;
    GUI_FONT_MONO     		* pMono;
    const GUI_FONT_PROP     * pProp;
    const GUI_FONT_PROP_EXT * pPropExt;
  } p;
  __u8 Baseline;
  __u8 LHeight;     /* Height of a small lower case character (a,x) */
  __u8 CHeight;     /* Height of a small upper case character (A,X) */
  FONT_EXT  Font_EXT;
};

/*********************************************************************
*
*       TrueType support (TTF)
*/
//#if GUI_COMPILER_SUPPORTS_FP

typedef struct {
  const char  * filePath;        /* mcl */
  __u32 NumBytes;            /* Size of file in bytes */
} GUI_TTF_DATA;

typedef struct {
  GUI_TTF_DATA * pTTF;     /* Pointer to GUI_TTF_DATA structure which contains location and size of font file */
  __u32 aImageTypeBuffer[4]; /* Buffer for image type structure */
  int PixelHeight;         /* Pixel height of new font. It means the height of the surrounding rectangle
                            * between the glyphs 'g' anf 'f'. Please notice that it is not the distance
                            * between two lines of text. With other words the value returned byGUI_GetFontSizeY()
                            * is not identically with this value. */
  int FaceIndex;           /* Some font files can contain more than one font face. In case of more than one face
                            * this index specifies the zero based face index to be used to create the font. 
                            * Usually 0. */
} GUI_TTF_CS;

typedef struct  tag_GUI_FONT_PAL{
	int start_index;             // start index of palette(256 color)
	int num;                     // size of pal index used for font
}GUI_FONT_PAL;


typedef struct tag_GUI_TTF_ATTR{
	int height;                             // size of font matrix
	 char filepath[60];                      //  file path of *.ttf file
	GUI_FONT_PAL font_pal;
}GUI_TTF_ATTR;

/**********************************
*
*      Line styles
*
***********************************
*/

#define GUI_LS_SOLID        (0)
#define GUI_LS_DASH         (1)
#define GUI_LS_DOT          (2)
#define GUI_LS_DASHDOT      (3)
#define GUI_LS_DASHDOTDOT   (4)

#endif	/*__GDI_H__*/

