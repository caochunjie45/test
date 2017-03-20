/*
**********************************************************************************************************************
*											        ePOS
*								the Easy Portable/Player Operation System
*									           Krnl sub-system
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File   : sys_dev.h
* Version: V1.0
* By     : steven.zgj
**********************************************************************************************************************
*/
#ifndef _SYS_DEV_H_
#define _SYS_DEV_H_

#define DEV_DEVTYPE_MAXNUM   32

/* maximum size of device class name */
#define MAX_DEV_CLASS_NAME_LEN    32

/* maximum size of device node name */
#define MAX_DEV_NODE_NAME_LEN   32

/* �豸������                                       */
#define DEV_CLASS_NOP           "NOP"
#define DEV_CLASS_DISK          "DISK"              /* �����豸��Ҳ���ǿ��豸                                       */
#define DEV_CLASS_USBD          "USBD"              /* u���豸                                                      */
#define DEV_CLASS_KEY           "KEY"               /* �����豸                                                     */
#define DEV_CLASS_MOUSE         "MOUSE"             /* �����豸                                                     */
#define DEV_CLASS_FS            "FSDRV"             /* �����豸                                                     */
#define DEV_CLASS_PD            "PDDRV"             /* �����豸                                                     */
#define DEV_CLASS_DMS			"DMS"				/* �豸�����豸									    */
#define DEV_CLASS_HWSC			"HWSC"				/* Ӳ��ɨ���豸��									    */

#define DEV_CLASS_USERDEF       "USERDEF"

/* �豸�ڵ����� */
#define DEV_NAME_RAMDISK        "RAMDISK"           /* RAMDISK�豸 */
#define DEV_NAME_SDCARD         "SDMMC-DISK"        /* SDcard?? */
#define DEV_NAME_SDCARD0        "SDMMC-DISK:0"      /* ��0��������SDcard */
#define DEV_NAME_SDCARD1        "SDMMC-DISK:1"      /* ��1��������SDcard */
#define DEV_NAME_SCSI_DISK_00   "SCSI_DISK_00"      /* USB host��һ���� */
#define DEV_NAME_USB_CDROM_00   "USB_CDROM_00"
#define DEV_NAME_ROOTFS         "ROOTFS"            /* ������ϵͳ����nand�����豸 */
#define DEV_NAME_PROVIDERFS     "PROVIDERFS"        /* �������û�����nand�����豸 */
#define DEV_NAME_SYSDATAFS      "SYSDATAFS"         /* ������ϵͳ��������nand�����豸 */
#define DEV_NAME_SYSBOOTFS      "SYSBOOTFS"         /* �������̼��޸����ݴ�ŵ�nand�����豸 */
#define DEV_NAME_BOOTFS         "BOOTFS"            /* eGON2.0������nand�����豸 */
#define DEV_NAME_USERDISK      	"USERDISK"         	/* �����û��Զ����nand������ǰ׺ */
#define DEV_NAME_HWSC           "hwsc"              /* ����ͬ��罻����Ӳ��ɨ���豸 */
#define DEV_NAME_PRIVATE        "private"           /* �������˽�����ݣ���ʽ������ʱ�ý����ݱ��ݺã�ȷ�����������ݲ���ʧ */
#define DEV_NAME_GAMEDISK       "GAMEDISK"

/* �豸���� */
#define DEV_NODE_ATTR_BLK       (1<<0)              /* ���豸   */
#define DEV_NODE_ATTR_CHR       (1<<1)              /* �ַ��豸 */
#define DEV_NODE_ATTR_CTL       (1<<2)              /* �����豸 */
#define DEV_NODE_ATTR_PART      (1<<3)              /* �豸���������� */
#define DEV_NODE_ATTR_FS        (1<<4)              /* �豸���ļ�ϵͳ���� */
#define DEV_NODE_ATTR_MOVABLE   (1<<5)              /* �豸�п��ƶ��豸���� */
#define DEV_NODE_ATTR_USBSLOT   (1<<6)              /* �豸�б��ύ��PC USB������� */
#define DEV_NODE_ATTR_RD        (1<<7)              /* �豸�пɶ����� */
#define DEV_NODE_ATTR_WR        (1<<8)              /* �豸�п�д���� */
#define DEV_NODE_ATTR_SYNMNT    (1<<9)              /* �豸ע��ʱͬ�������������� */

/* ���ݾ�dev ioctl */
#define DEV_CMD_GET_INFO            0
#define DEV_CMD_GET_INFO_AUX_SIZE   0
#define DEV_CMD_GET_STATUS          1


/* user defined device ioctrol cmd: 0x00000000~0x7FFFFFFF */
/* system defined device ioctrol cmd: 0x80000000~0xFFFFFFFF */
#define DEV_IOC_USR_BASE				0x00000000
#define DEV_IOC_SYS_BASE				0x80000000
#define IS_DEVIOCSYS(cmd)				(cmd>=DEV_IOC_SYS_BASE)
/* sys */
#define DEV_IOC_SYS_GET_DEVNAME		(DEV_IOC_SYS_BASE + 0)
#define DEV_IOC_SYS_GET_CLSNAME		(DEV_IOC_SYS_BASE + 1)
#define DEV_IOC_SYS_GET_ATTRIB		(DEV_IOC_SYS_BASE + 2)
#define DEV_IOC_SYS_GET_LETTER		(DEV_IOC_SYS_BASE + 3)
#define DEV_IOC_SYS_GET_OPENARGS    (DEV_IOC_SYS_BASE + 4)
/* dms */
#define DEV_IOC_USR_GET_DEVITEM		(DEV_IOC_USR_BASE + 102)
#define DEV_IOC_USR_GET_DEVROOT		(DEV_IOC_USR_BASE + 103)
/* blk */
#define DEV_IOC_USR_GET_GEO			(DEV_IOC_USR_BASE + 104)
#define DEV_IOC_USR_FLUSH_CACHE		(DEV_IOC_USR_BASE + 105)
#define DEV_IOC_USR_PHY_READ        (DEV_IOC_USR_BASE + 106)
#define DEV_IOC_USR_PHY_WRITE       (DEV_IOC_USR_BASE + 107)
#define DEV_IOC_USR_PHY_ERASE       (DEV_IOC_USR_BASE + 108)
#define DEV_IOC_USR_LOGIC_WRITE     (DEV_IOC_USR_BASE + 109)
#define DEV_IOC_USR_LOGIC_READ      (DEV_IOC_USR_BASE + 110)
#define DEV_IOC_USR_GET_HW_INFO     (DEV_IOC_USR_BASE + 111)
#define DEV_IOC_USR_BLK_ERASE       (DEV_IOC_USR_BASE + 112)
#define DEV_IOC_USR_LOAD_CMMB_BUF   (DEV_IOC_USR_BASE + 113)
#define DEV_IOC_USR_DEL_CMMB_BUF	(DEV_IOC_USR_BASE + 114)
#define DEV_IOC_USR_DEL_CMMB_USER_BUF (DEV_IOC_USR_BASE + 115)
#define DEV_IOC_USR_LOAD_USER_BUF (DEV_IOC_USR_BASE + 116)


/* cd-rom */
#define DEV_CDROM_LAST_WRITTEN		(DEV_IOC_USR_BASE + 120)  /* get last block written on disc */
#define DEV_CDROM_MULTISESSION		(DEV_IOC_USR_BASE + 121)  /* Obtain the start-of-last-session
 															   * address of multi session disks,
 															   * address type is logical block.*/

#define DRV_CMD_PLUGIN				0xffff0000
#define DRV_CMD_PLUGOUT				0xffff0001
#define DRV_CMD_GET_STATUS			0xffff0002

typedef enum __DRV_STA
{
    DRV_STA_FREE=0,
    DRV_STA_BUSY
}__drv_sta_t;


typedef enum __DEV_ERR
{
    DEV_NO_ERR = 0,
    DEV_ERR,

    DEV_HANDLE_ERR,                                 /* handle error, maybe handle equ. null, or handle not a opened */
                                                    /* handle                                                       */

    DEV_CLK_NOT_EN,                                 /* clock not enable                                             */

    DEV_CLK_SCLK_ERR,
    DEV_CLK_DIV_ERR,
    DEV_CLK_HANDLE_FREE,

}__dev_err_t;


/* �豸���������� */
typedef struct  __DEV_DEVOP
{
    __hdle  (*Open  )(void * open_arg, __u32 mode);
    __s32   (*Close )(__hdle hDev);
    __u32   (*Read  )(void * /*pBuffer*/,
                      __u32 /*chrdev: 1,        blkdev: sector pos*/,
                      __u32 /*chrdev: byte cnt, blkdev: sector cnt*/,
                      __hdle/*hDev*/);
    __u32   (*Write )(const void * /*pBuffer*/,
                      __u32 /*chrdev: 1,        blkdev: sector pos*/,
                      __u32 /*chrdev: byte cnt, blkdev: sector cnt*/,
                      __hdle/*hDev*/);
    __s32   (*Ioctl )(__hdle hDev, __u32 Cmd, __s32 Aux, void *pBuffer);
}__dev_devop_t;

#ifdef SIM_PC_WIN
#define __packed
#endif
typedef __packed struct __DEV_BLKINFO
{
    __u32   hiddennum;
    __u32   headnum;
    __u32   secnum;
    __u32   partsize;
	__u32	secsize;
}__dev_blkinfo_t;

/* disk info            */
typedef __packed struct __DEV_DSKINFO
{
    char                name[16];
    __u32               n;
    __dev_blkinfo_t  *  info;
}__dev_dskinfo_t;

typedef enum __DEV_HOTPLUG_MSG_TARGET
{
    DEV_MSG_TARGET_HOTPLUG_USBMONITOR,
}__dev_msgtarget_t;


typedef struct __BLK_DEV_RW_ATTR
{
    __u32       blk;        //���豸��д����ʼ���
    __s32       cnt;        //���豸��д�Ŀ���
    void        *buf;       //�洢���豸��д���ݵĻ�����
    __u32       reserved;   //����

} __blk_dev_rw_attr_t;


//************************************************
// ��������
/* system call table */
typedef struct
{
    __pSWI_t esDEV_Plugin               ;
    __pSWI_t esDEV_Plugout              ;

    __pSWI_t esDEV_DevReg               ;
    __pSWI_t esDEV_DevUnreg             ;

    __pSWI_t esDEV_Open                 ;
    __pSWI_t esDEV_Close                ;
    __pSWI_t esDEV_Read                 ;
    __pSWI_t esDEV_Write                ;
    __pSWI_t esDEV_Ioctl                ;

    __pSWI_t esDEV_Lock                 ;
    __pSWI_t esDEV_Unlock               ;

    __pSWI_t esDEV_Insmod               ;
    __pSWI_t esDEV_Unimod               ;
} SWIHandler_DEV_t;
#ifndef SIM_PC_WIN
#define SYSCALL_DEV(x,y) __swi(SWINO(SWINO_DEV, SWIHandler_DEV_t, y))x y
#else 
#define SYSCALL_DEV(x,y) x y
#endif

SYSCALL_DEV(__s32 , esDEV_Plugin     )(char * /* plgmsg */, __u32 /*devno*/, void * /*p_arg*/, __u8/*prio*/);

SYSCALL_DEV(__s32 , esDEV_Plugout    )(char * /*device*/, __u32 /*devno*/);

SYSCALL_DEV(__hdle, esDEV_DevReg     )(const char * /*class*/, const char * /*name*/ ,
                                       __dev_devop_t * /*drv*/  , void * /*p_arg*/);
SYSCALL_DEV(__s32 , esDEV_DevUnreg   )(__hdle/*dev*/);

SYSCALL_DEV(__hdle, esDEV_Open       )(__hdle /*hNode*/, __u32/*Mode*/);
SYSCALL_DEV(__s32 , esDEV_Close      )(__hdle /*hDev*/);
SYSCALL_DEV(__u32 , esDEV_Read       )(void * /*pdata*/,
                                       __u32 /*chrdev: 1,        blkdev: sector pos*/,
                                       __u32 /*chrdev: byte cnt, blkdev: sector cnt*/,
                                       __hdle/*hDev*/);
SYSCALL_DEV(__u32 , esDEV_Write      )(const void * /*pdata*/,
                                       __u32 /*chrdev: 1,        blkdev: sector pos*/,
                                       __u32 /*chrdev: byte cnt, blkdev: sector cnt*/,
                                       __hdle/*hDev*/);
SYSCALL_DEV(__s32 , esDEV_Ioctl      )(__hdle /*hDev*/,   __u32/*cmd*/, __s32/*aux*/, void * /*pbuffer*/);

SYSCALL_DEV(__s32,  esDEV_Lock       )(__hdle hNode);
SYSCALL_DEV(__s32,  esDEV_Unlock     )(__hdle hNode);

SYSCALL_DEV(__s32 , esDEV_Insmod     )(char * /*modfile*/, __u32 /*devno*/, void * /*p_arg*/);
SYSCALL_DEV(__s32 , esDEV_Unimod     )(char * /*modfile*/, __u32 /*devno*/);

//************************************************
// ��������
extern SWIHandler_DEV_t SWIHandler_DEV;
//************************************************
// ��������
__s32 DEV_Init (void);
__s32 DEV_Exit (void);
__s32 HWSC_Init(void);
__s32 HWSC_Exit(void);

#endif //#ifndef _SYS_DEV_H_
