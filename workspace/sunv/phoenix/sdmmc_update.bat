::----------------------------------------------------------------
:: create by james at 2011-09-01
::----------------------------------------------------------------

@echo off

::----------------------------------------------------------------
:: update ramfs
::----------------------------------------------------------------
@copy .\..\ramfs\pwm_cfg.ini            .\ramfs\pwm_cfg.ini
@copy .\..\ramfs\drv\twi.drv            .\ramfs\drv\twi.drv
@copy .\..\ramfs\drv\rtc.drv            .\ramfs\drv\rtc.drv
@copy .\..\ramfs\drv\power2.drv          .\ramfs\drv\power.drv
@copy .\..\ramfs\drv\spi.drv            .\ramfs\drv\spi.drv
@copy .\..\ramfs\drv\spinor.drv            .\ramfs\drv\spinor.drv
@copy .\..\ramfs\drv\matrixkey.drv      .\ramfs\drv\key.drv
@copy .\..\ramfs\drv\monitor.drv        .\ramfs\drv\monitor.drv
@copy .\..\ramfs\drv\power_cfg.ini      .\ramfs\drv\power_cfg.ini
@copy .\..\ramfs\mod\slib.mod           .\ramfs\mod\slib.mod
@copy .\..\rootfs\drv\sdmmc.drv         .\ramfs\drv\sdmmc.drv
@copy .\..\rootfs\drv\display.drv       .\rootfs\drv\display.drv
::以下注释卡量产时需要屏蔽；卡启动时需要打开
@copy .\..\beetles\ramfs\shell.zgj       .\ramfs\shell.zgj   

::----------------------------------------------------------------
:: update rootfs
::----------------------------------------------------------------
@copy .\..\rootfs\epos.img              .\rootfs\epos.img
@copy .\..\rootfs\drv\uart.drv          .\rootfs\drv\uart.drv
@copy .\..\rootfs\drv\audio.drv         .\rootfs\drv\audio.drv
    @copy .\..\rootfs\mod\orange.mod    .\rootfs\mod\orange.mod
@copy .\..\rootfs\mod\charset.mod       .\rootfs\mod\charset.mod
@copy .\..\rootfs\mod\charset.bin       .\rootfs\mod\charset.bin
::@copy .\..\rootfs\mod\lotus\core\venc_mjp.drv       .\rootfs\mod\venc_mjp.drv
@copy .\..\rootfs\mod\update.mod 	.\rootfs\mod\update.mod

@copy .\..\rootfs\drv\usb_host0.drv       .\ramfs\drv\usb_host0.drv
@copy .\..\rootfs\drv\usbd_msc.drv       .\ramfs\drv\usbd_msc.drv

@xcopy ..\beetles\rootfs\apps             .\rootfs\apps    /s/i/y