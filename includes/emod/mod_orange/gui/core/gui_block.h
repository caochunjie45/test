/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									          shell application
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : GUI_BLOCK.h
* By      : Derek
* Version : V1.0
* Tiem	  : 2021-1-12 9:49:02
*********************************************************************************************************
*/
#ifndef __GUI_BLOCK_H__
#define __GUI_BLOCK_H__

typedef __hdle HBLOCK;

#define  ROTATE_ANGLE_NONE            (0)
#define  ROTATE_ANGLE_90              (1)
#define  ROTATE_ANGLE_180             (2)
#define  ROTATE_ANGLE_270             (3)

#define   BLITFLAG_COLORSET           0x00000000
#define   BLITFLAG_COLORALPHA         0x00000001
#define   BLITFLAG_ALPHACHANNEL       0x00000002
#define   BLITFLAG_BOTHALPHA          0x00000004
#define   BLITFLAG_COLORKEY           0x00000008
#define   BLITFLAG_ROTATE90           0x00000010
#define   BLITFLAG_ROTATE180          0x00000020
#define   BLITFLAG_ROTATE270          0x00000040
#define   BLITFLAG_HFLIP              0x00000080
#define   BLITFLAG_VFLIP              0x00000100
#define   BLITFLAG_AND                0x00000200
#define   BLITFLAG_OR                 0x00000400
#define   BLITFLAG_XOR                0x00000800

typedef struct GUI_BLOCK_VALUE
{
	__u8 src_globle_value;
	GUI_COLOR ck_value;
}__gui_block_value_t;


extern HBLOCK 	 GUI_BlockCreate			(__u32 width,__u32 height,__u32 format, __u8 byte_seq);            //����block���ڴ��ɸú�������
extern HBLOCK    GUI_BlockCreateFrom        (RECT *block_rect,__u32  format,__u32 fb_width, __u32 fb_height, void *addr,__u8 byte_seq);            //����block���ڴ�������ָ��
extern __s32     GUI_BlockDelete            (HBLOCK hblock);                                     //ɾ��block�����ͷ���block������ڴ�
extern HBLOCK    GUI_BlockRotate            (HBLOCK hblock, __u32 angle);                         //��תblock
extern HBLOCK    GUI_BlockHFlip             (HBLOCK hblock);                                     //ˮƽ��ת
extern HBLOCK    GUI_BlockVFlip             (HBLOCK hblock);
extern __s32     GUI_BlockSel               (HBLOCK hblock);                                      //ѡ���block����
extern __s32     GUI_BlockBitBlit           (HBLOCK hdstblock,__s32 dx,__s32 dy,HBLOCK hsrcblock,RECT *srcrect,__u32 flags, void *value);    //��Դblock�е�srcrect��������ݣ����ӵ�Ŀ��block��(dx,dy)��
extern __s32     GUI_BlockLayerBlit         (H_LYR  hdstlayer,RECT *clip_rect,__s32 dx,__s32 dy,
											 HBLOCK hsrcblock,RECT *srcrect,__u32 flags, void *value);    //��Դblock�е�srcrect��������ݣ����ӵ�Ŀ��layer��(dx,dy)��
extern __s32     GUI_BlockGetLayerMem       (H_LYR  hsrclayer,RECT *srcrect,HBLOCK hdstblock,__s32 dx,__s32 dy,__u32 flags, void *value);    //��Դlayer�е�srcrect��������ݣ����ӵ�Ŀ��block��(dx,dy)��
extern __s32	 GUI_SetBlockClip 			(HBLOCK hblock, RECT *cliprect);
extern RECT	 	 GUI_GetBlockClip 			(HBLOCK hblock);
extern __s32	 GUI_GetBlockWidth			(HBLOCK hblock);
extern __s32	 GUI_GetBlockHeight			(HBLOCK hblock);
extern __s32	 GUI_GetBlockFormat			(HBLOCK hblock);
extern __s32 	 GUI_GetBlockAddr			(HBLOCK hblock);
extern __s32	 GUI_GetBlockFBWidth		(HBLOCK hblock);
extern __s32	 GUI_GetBlockFBHeight		(HBLOCK hblock);
extern __s32 	 GUI_BlockLock				(HBLOCK hblock);
extern __s32 	 GUI_BlockUnLock			(HBLOCK hblock);
extern __s32 	 GUI_BlockFill				(HBLOCK hblock,GUI_COLOR colorvalue);

#endif
