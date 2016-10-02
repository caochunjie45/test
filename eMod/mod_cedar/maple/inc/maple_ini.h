/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_cedar sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : maple_ini.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2012-3-2
* Description:
    记录maple中间件配置的数据结构.
********************************************************************************
*/
#ifndef _MAPLE_INI_H_
#define _MAPLE_INI_H_

/*******************************************************************************
maple.ini文件的配置项说明:
Members: 
    AUDIO_RUN_ONLY_BUF_SIZE: audio驱动的bufsize.单位kbytes, 128代表128k
    AUDIO_WITH_VIDEO_BUF_SIZE:同上
    AACSBR:是否打开aac解码驱动的sbr功能, 0关闭;1打开
*******************************************************************************/
typedef struct tag_CMapleINI
{
    CFEConfig   FEConfig;           //demod模块和tsc驱动所认可的配置数据结构
    CedarINI    CedarINIInstance;   //cedar插件群传统的配置数据结构
}CMapleINI;

#endif  /* _MAPLE_INI_H_ */

