/*
*********************************************************************************************************
*											        ePDK
*						           the Easy Portable/Player Development Kits
*									           image file format
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File   : fwimage.h
* Version: V1.0
* By     : steven.zgj
*********************************************************************************************************
*/
#ifndef FWIMAGE_H
#define FWIMAGE_H



#define EPDK_FFMT_KNL   'knl'           // *.knl       �ں�ӳ���ļ�
#define EPDK_FFMT_EXF   'exf'           // *.exf       ��ִ�г���
#define EPDK_FFMT_MOD   'mod'           // *.mod       ģ���ļ�������mod�ļ��ְ���������ģ�飬�м��ģ���
#define EPDK_FFMT_RES   'res'           // *.res       ��Դ�ļ�
#define EPDK_FFMT_FNT   'fnt'           // *.fnt       �ֿ��ļ�
#define EPDK_FFMT_BRM   'brm'           // *.brm       brom�ļ�,���ļ���������rom��    
#define EPDK_FFMT_BT0   'bt0'           // *.bt0       nf644.bt0, nf321.bt0, ..., ���ļ���������nand/nor flash�Ŀ�ʼ����
#define EPDK_FFMT_BT1   'bt1'           // *.bt1       nf644.bt1, nf321.bt1, ..., ���ļ���������nand/nor flash��boot0֮��
#define EPDK_FFMT_FEL   'fel'           // *.fel       eFex loader, ���ļ���������rom��
#define EPDK_FFMT_FES   'fes'           // *.fes       eFex server, pc��ͨ��fel�����ļ����ص��ڴ���
#define EPDK_FFMT_FET   'fet'           // *.fet       eFex task, pc��ͨ��fes����ͬ��eFex task���ص��ڴ���ִ��
#define EPDK_FFMT_BIO   'bio'           // *.bio       eBios�ļ�                          

#define EPDK_FFMT_KNL   'ePDK.knl'      // *.knl       �ں�ӳ���ļ�
#define EPDK_FFMT_EXF   'ePDK.exf'      // *.exf       ��ִ�г���
#define EPDK_FFMT_MOD   'ePDK.mod'      // *.mod       ģ���ļ�������mod�ļ��ְ���������ģ�飬�м��ģ���
#define EPDK_FFMT_RES   'ePDK.res'      // *.res       ��Դ�ļ�
#define EPDK_FFMT_FNT   'ePDK.fnt'      // *.fnt       �ֿ��ļ�
#define EPDK_FFMT_BRM   'ePDK.brm'      // *.brm       brom�ļ�,���ļ���������rom��    
#define EPDK_FFMT_BT0   'ePDK.bt0'      // *.bt0       nf644.bt0, nf321.bt0, ..., ���ļ���������nand/nor flash�Ŀ�ʼ����
#define EPDK_FFMT_BT1   'ePDK.bt1'      // *.bt1       nf644.bt1, nf321.bt1, ..., ���ļ���������nand/nor flash��boot0֮��
#define EPDK_FFMT_FEL   'ePDK.fel'      // *.fel       eFex loader, ���ļ���������rom��
#define EPDK_FFMT_FES   'ePDK.fes'      // *.fes       eFex server, pc��ͨ��fel�����ļ����ص��ڴ���
#define EPDK_FFMT_FET   'ePDK.fet'      // *.fet       eFex task, pc��ͨ��fes����ͬ��eFex task���ص��ڴ���ִ��
#define EPDK_FFMT_BIO   'ePDK.img'      // *.img       firmware image file
                        

typedef struct __FWIMGHEAD
{
    __u8  image[8];         // 'ePDK.emg'
    
    __u8  arch;             // arm:0, mips:1,... 
    __u8  vendor;
    __u32 solution;         // A1:0
    
    __u32 version;          // image file version
    __u32 pdkversion;       // pdk version
    
    __u32 headsize;         // head size
   
    __u32 dirnum;           // dir item number
    __u32 diritemschecksum; // dir item area checksum
    
    __u32 fwimgchecksum;    // total check sum(exept this head)
    
    __u32 reserved[];       // reserved
    __u32 headchecksum;     // head check sum
    
} __epdk_fwimghead_t;

typedef struct __FWIMGDIR
{
    __u8  filename[32];         // file name
    __u8  type[3];              // type
    __u32 offset;               // file's image offset        
    __u32 len;                  // file length
    __u32 filechecksum;         // file check sum       
    __u8  reserved[4];          
} __epdk_fwimgdir_t;

typedef struct __FWIMG
{
    __fwimg_head    head;
    __fwimg_dir     diritem[];
    __u32           area[];
} __epdk_fwimg_t;

#endif		//#define FWIMAGE_H




