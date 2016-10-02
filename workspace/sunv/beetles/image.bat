::/*
::*********************************************************************************************************
::*                                                    MELIS
::*                                    the Easy Portable/Player Develop Kits
::*                                                 make image
::*
::*                                    (c) Copyright 2011-2014, Sunny China
::*                                             All Rights Reserved
::*
::* File    : image.bat
::* By      : hot.lee
::* Version : v1.0
::* Date    : 2013-09-06
::* Descript: make a melis100 sdk image file.
::* Update  : date                auther      ver     notes
::*           2013-09-06 13:11:43  hot.lee       1.0     Create this file.
::*********************************************************************************************************
::*/

    @ECHO OFF
    
    echo make image begin......
    
    set BUILDTOOLPATH=.\..\..\..\tools\build_tools
    set DRAGONTOOLPATH=..\..\..\..\eStudio\Softwares\eDragonEx400

 
    ::----------------------------------------------------------------
    ::      更新系统文件
    ::----------------------------------------------------------------
    call update.bat >update.txt
    
    ::----------------------------------------------------------------
    ::      解析配置脚本，系统
    ::----------------------------------------------------------------
    %BUILDTOOLPATH%\script.exe .\..\eFex\sys_config.fex
    %BUILDTOOLPATH%\script.exe .\..\eFex\sys_partition.fex
   
    ::----------------------------------------------------------------
    ::      解析配置脚本，应用
    ::----------------------------------------------------------------
    %BUILDTOOLPATH%\script.exe .\rootfs\app_config.fex


    ::----------------------------------------------------------------
    ::      配置脚本
    ::----------------------------------------------------------------
    @copy .\..\eFex\sys_config.bin  sys_config.bin
    @copy .\..\eFex\sys_partition.bin  sys_partition.bin

    ::----------------------------------------------------------------
    ::      fes1、uboot，用于USB烧录
    ::----------------------------------------------------------------

    @copy ..\eFex\usb\fes1_sun5i.bin          fes1.fex   
    @copy ..\eFex\usb\u-boot-sun5i.bin        u-boot.fex 


    ::----------------------------------------------------------------
    ::      boot0_sdcard_sun，用于卡量产
    ::----------------------------------------------------------------     
    @copy ..\eFex\card\boot0_sdcard_sun5i.bin    boot0_sdcard.fex     
          
    ::----------------------------------------------------------------
    ::      更新boot0_sdcard_sun文件头
    ::---------------------------------------------------------------- 
    %BUILDTOOLPATH%\update_boot0.exe boot0_sdcard.fex sys_config.bin SDMMC_CARD

    ::----------------------------------------------------------------
    ::      更新boot0、boot1文件头
    ::---------------------------------------------------------------- 
    %BUILDTOOLPATH%\update_boot0.exe boot0.fex  sys_config.bin  SPINOR_FLASH

    %BUILDTOOLPATH%\update_uboot.exe boot1.fex  sys_config.bin

    ::----------------------------------------------------------------
    ::      更新fes1、uboot文件头
    ::---------------------------------------------------------------- 
    %BUILDTOOLPATH%\update_fes1.exe  fes1.fex  sys_config.bin

    %BUILDTOOLPATH%\update_uboot.exe u-boot.fex  sys_config.bin
      
   
    ::----------------------------------------------------------------
    ::      生成MBR文件 ,这里必须是4，因为usbtool.fex已经写死
    ::----------------------------------------------------------------
    %BUILDTOOLPATH%\update_mbr.exe sys_partition.bin  4


    ::----------------------------------------------------------------
    ::      生成ramfs、rootfs、udisk
    ::----------------------------------------------------------------
    call fsbuild.bat


    ::----------------------------------------------------------------
    ::      BOOT0和BOOT1文件大小填充对其
    ::----------------------------------------------------------------
    %BUILDTOOLPATH%\fix_file.exe boot0.fex  24
    ::%BUILDTOOLPATH%\fix_file.exe boot1.fex  120

    ::----------------------------------------------------------------
    ::      打包
    ::----------------------------------------------------------------
    %BUILDTOOLPATH%\merge_package.exe  full_img.fex boot0.fex   boot1.fex   sunxi_mbr.fex   sys_partition.bin

    ::----------------------------------------------------------------
    ::      生成烧写文件校验和
    ::----------------------------------------------------------------
    %DRAGONTOOLPATH%\FileAddSum.exe full_img.fex verify.fex

    %DRAGONTOOLPATH%\dragon.exe  image.cfg > image.txt



    ::----------------------------------------------------------------
    ::      清除临时文件
    ::----------------------------------------------------------------    
    if exist u-boot.fex   del  u-boot.fex
    if exist boot1.fex   del  boot1.fex
    if exist fes1.fex   del  fes1.fex
    if exist boot0.fex   del  boot0.fex
    if exist boot0_sdcard.fex   del  boot0_sdcard.fex
    if exist dlinfo.fex   del  dlinfo.fex
    if exist sunxi_mbr.fex   del  sunxi_mbr.fex
    if exist sys_config.bin   del  sys_config.bin
    if exist sys_partition.bin   del  sys_partition.bin
    ::if exist full_img.fex   del  full_img.fex

    if exist rootfs.iso   del  rootfs.iso
    if exist udisk.iso   del  udisk.iso
    
    if exist .\..\eFex\sys_config.bin  del  .\..\eFex\sys_config.bin
    if exist .\..\eFex\sys_partition.bin  del  .\..\eFex\sys_partition.bin
    if exist .\rootfs\app_config.bin  del  .\rootfs\app_config.bin

    if exist verify.fex  del  verify.fex



    echo make image finished....
    
    PAUSE
