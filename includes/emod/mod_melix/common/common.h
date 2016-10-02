/*
**************************************************************************************************************************
*													        eMOD
*						            		the Easy Portable/Player Develop Kits
*									   					melix module 
*
*                             		 Copyright(C), 2011-2012, Softwinner Microelectronic Co., Ltd.
*									               All Rights Reserved
*
* File name		:	common.h
* Author		: 	Derek
* Version 		: 	V1.00
* Date    		: 	2012-02-07
* Description	:
* Others 		:
* History 		:
* <Author>           <time>          <version>      <description>
* Derek Lai		    2012-02-07         1.0.0          Create File
*
**************************************************************************************************************************
*/
#ifndef __COMMON_H__
#define __COMMON_H__

/*
************************************************************************************************
*	public data and interface of gui block mode
************************************************************************************************
*/
typedef __hdle HBLOCK;

/*Rotate angle flags*/
#define  ROTATE_ANGLE_NONE            (0)
#define  ROTATE_ANGLE_90              (1)
#define  ROTATE_ANGLE_180             (2)
#define  ROTATE_ANGLE_270             (3)
//#define  ROTATE_HFLIP				  (4)
//#define  ROTATE_VFLIP				  (5)

/*Blit flags*/
#define   BLITFLAG_COLORSET           0x00000000  /*copy*/
#define   BLITFLAG_COLORALPHA         0x00000001  /*point alpha*/
#define   BLITFLAG_ALPHACHANNEL       0x00000002  /*global alpha*/
#define   BLITFLAG_BOTHALPHA          0x00000004  /*point and global alpha*/

typedef struct GUI_BLOCK_VALUE
{
	__u8 src_globle_value;
}__gui_block_value_t;


extern HBLOCK 	 GUI_BlockCreate			(__u32 width,__u32 height, __u8 byte_seq);            //创建block，内存由该函数分配
extern HBLOCK    GUI_BlockCreateFrom        (RECT *block_rect, __u32 fb_width,__u32 fb_height,void *addr,__u8 byte_seq);            //创建block，内存由外面指定
extern __s32     GUI_BlockDelete            (HBLOCK hblock);                                     //删除block，并释放由block分配的内存
//extern __s32     GUI_BlockSel               (HBLOCK hblock);                                      //选择该block绘制
extern __s32	 GUI_GetBlockWidth			(HBLOCK hblock);
extern __s32	 GUI_GetBlockHeight			(HBLOCK hblock);
extern __s32 	 GUI_GetBlockAddr			(HBLOCK hblock);
extern __s32	 GUI_GetBlockFBWidth		(HBLOCK hblock);
extern __s32	 GUI_GetBlockFBHeight		(HBLOCK hblock);
extern __s32 	 GUI_BlockLock				(HBLOCK hblock);
extern __s32 	 GUI_BlockUnLock			(HBLOCK hblock);
extern __s32 	 GUI_BlockFill				(HBLOCK hblock, __u32 colorvalue);
extern __s32 	 GUI_BlockBitBlit			(HBLOCK hdstblock,__s32 dx,__s32 dy,HBLOCK hsrcblock,RECT *srcrect,__u32 flags, void *value);
//extern __s32 	 GUI_BlockLayerBlit			(H_LYR  hdstlayer,__s32 dx,__s32 dy,HBLOCK hsrcblock, RECT *srcrect,__u32 flags, void * value);
//extern __s32 	 GUI_BlockGetLayerMem		(H_LYR  hsrclayer,RECT *srcrect,HBLOCK hdstblock,__s32 dx,__s32 dy,__u32 flags, void * value);
extern __s32 	 GUI_BlockRotate			(__u32  srcblock_addr, __u32 dstblock_addr, __u32 srcblock_fb_width, __u32 srcblock_fb_height, __u32 angle_flags);

#endif	/*ifndef __COMMON_H__*/

