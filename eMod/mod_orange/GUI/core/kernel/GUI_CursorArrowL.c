/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              eMOD Sub-System
*
*                                   (c) Copyright 2007-2009,  Miracle.MCL.China
*                                             All Rights Reserved
*
* Moudle  : lemon
* File    : GUI_CursorArrowL.C
* Purpose : Defines the arrow cursor, large
*
* By      : Miracle
* Version : v1.0
* Date    : 2009-2-19 9:53:53
**********************************************************************************************************************
*/


#include <stdlib.h>
#include "gui_core_kernel_private.h"

GUI_CONST_STORAGE GUI_BITMAP GUI_BitmapArrowL = {
 18,                  /* XSize */
 30,                  /* YSize */
 5,                   /* BytesPerLine */
 2,                   /* BitsPerPixel */
 GUI_Pixels_ArrowL,   /* Pointer to picture data (indices) */
 &GUI_CursorPal       /* Pointer to palette */
};

GUI_CONST_STORAGE GUI_CURSOR GUI_CursorArrowL = {
  &GUI_BitmapArrowL, 0, 0
};

/*************************** End of file ****************************/
