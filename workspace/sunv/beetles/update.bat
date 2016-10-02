::/*
::*********************************************************************************************************
::*                                                    MELIS
::*                                    the Easy Portable/Player Develop Kits
::*                                               Update system files
::*
::*                                    (c) Copyright 2011-2014, Sunny China
::*                                             All Rights Reserved
::*
::* File    : update.bat
::* By      : Sunny
::* Version : v1.0
::* Date    : 2011-4-19
::* Descript: update system drivers and modules.
::* Update  : date                auther      ver     notes
::*           2011-4-19 13:05:05  Sunny       1.0     Create this file.
::*********************************************************************************************************
::*/

    @ECHO OFF
    ::----------------------------------------------------------------
    ::      boot0°¢boot1£¨–¥»ÎSPINOR
    ::----------------------------------------------------------------
    @copy   ..\eGon\storage_media\spinor\boot0.bin     boot0.fex    
    @copy   ..\eGon\storage_media\spinor\boot1.bin                  boot1.fex  
        
    ::----------------------------------------------------------------
    ::update apps in ramfs
    ::----------------------------------------------------------------
    @copy .\..\ramfs\pwm_cfg.ini        .\ramfs\pwm_cfg.ini
    @copy .\..\ramfs\drv\twi.drv        .\ramfs\drv\twi.drv
    @copy .\..\ramfs\drv\rtc.drv        .\ramfs\drv\rtc.drv
    @copy .\..\ramfs\drv\power.drv      .\ramfs\drv\power.drv
    @copy .\..\ramfs\drv\spinor.drv     .\ramfs\drv\spinor.drv
    @copy .\..\ramfs\drv\spi.drv        .\ramfs\drv\spi.drv
    @copy .\..\ramfs\drv\key.drv        .\ramfs\drv\key.drv
    @copy .\..\ramfs\drv\monitor.drv    .\ramfs\drv\monitor.drv
    @copy .\..\ramfs\drv\power_cfg.ini  .\ramfs\drv\power_cfg.ini
    @copy .\..\ramfs\mod\slib.mod       .\ramfs\mod\slib.mod
    @copy .\..\ramfs\drv\ir.drv        .\ramfs\drv\ir.drv
    @copy .\ramfs.ini                  .\ramfs\ramfs_ini.tmp

    ::----------------------------------------------------------------
    :: update rootfs
    ::----------------------------------------------------------------
    @copy .\..\rootfs\standby.bin       .\rootfs\standby.bin
    @copy .\..\rootfs\epos.img          .\rootfs\epos.img
    @copy .\..\rootfs\drv\audio.drv     .\rootfs\drv\audio.drv
    @copy .\..\rootfs\drv\display.drv   .\rootfs\drv\display.drv
    ::@copy .\..\rootfs\drv\g2d.drv       .\rootfs\drv\g2d.drv
    @copy .\..\rootfs\drv\fm.drv        .\rootfs\drv\fm.drv
    @copy .\..\rootfs\drv\sdmmc.drv     .\rootfs\drv\sdmmc.drv
    @copy .\..\rootfs\drv\uart.drv      .\rootfs\drv\uart.drv
    @copy .\..\rootfs\drv\usbd_msc.drv  .\rootfs\drv\usbd_msc.drv
    ::@copy .\..\rootfs\drv\usb_host0.drv .\rootfs\drv\usb_host0.drv
    @copy .\..\rootfs\drv\usb_host1.drv .\rootfs\drv\usb_host1.drv
    ::@copy .\..\rootfs\drv\usb_host2.drv .\rootfs\drv\usb_host2.drv
    ::@copy .\..\rootfs\drv\orchid.drv    .\rootfs\drv\orchid.drv
    ::@copy .\..\rootfs\drv\touchpanel.drv .\rootfs\drv\touchpanel.drv
    ::@copy .\..\rootfs\drv\csi.drv    		.\rootfs\drv\csi.drv
    @copy .\..\rootfs\drv\mcu.drv 	.\rootfs\drv\mcu.drv
    ::@copy .\..\rootfs\drv\atbm.drv 	.\rootfs\drv\atbm.drv
    ::@copy .\..\rootfs\drv\mxl603.drv 	.\rootfs\drv\mxl603.drv
        @copy .\rootfs.ini                  .\rootfs\rootfs_ini.tmp

    
    ::----------------------------------------------------------------
    :: update rootfs modules
    ::----------------------------------------------------------------
    @copy .\..\rootfs\mod\orange.mod    .\rootfs\mod\orange.mod
    @copy .\..\rootfs\mod\cedar.mod     .\rootfs\mod\cedar.mod
    @copy .\..\rootfs\mod\charset.mod   .\rootfs\mod\charset.mod
    @copy .\..\rootfs\mod\charset.bin   .\rootfs\mod\charset.bin
    @copy .\..\rootfs\mod\ginkgo.mod 	.\rootfs\mod\ginkgo.mod
    @copy .\..\rootfs\mod\update.mod 	.\rootfs\mod\update.mod
    @copy .\..\rootfs\mod\maple.mod     .\rootfs\mod\maple.mod

    ::@xcopy .\..\rootfs\mod\lotus	.\rootfs\mod\lotus  /s/i/y
    ::----------------------------------------------------------------
    :: update cedar plugs
    ::----------------------------------------------------------------
    ::update cedar modules
    @xcopy .\..\rootfs\mod\cedar        .\rootfs\mod\cedar  /s/i/y
    
    ::----------------------------------------------------------------
    :: update willow plugs
    ::----------------------------------------------------------------
    ::update willow modules
    @xcopy .\..\rootfs\mod\willow       .\rootfs\mod\willow /s/i/y

