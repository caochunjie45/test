::----------------------------------------------------------------
:: create by james at 2011-09-01
::----------------------------------------------------------------

@ECHO OFF

echo make image begin...

set BUILDTOOLPATH=..\..\..\..\eStudio\Softwares\mod_update
set DRAGONTOOLPATH=..\..\..\..\eStudio\Softwares\eDragonEx200
set UPDATETOOLPATH=.\..\..\..\tools\build_tools

::----------------------------------------------------------------
:: ����ϵͳ�ļ�
::----------------------------------------------------------------
call update.bat > update.txt

::----------------------------------------------------------------
:: �������ýű�
::----------------------------------------------------------------
if exist ..\eFex\sys_config.bin del ..\eFex\sys_config.bin
%BUILDTOOLPATH%\script.exe ..\eFex\sys_config.fex

::----------------------------------------------------------------
:: ����BOOT0��BOOT1�ļ�ͷ��Ϣ
::----------------------------------------------------------------
copy ..\eGon\storage_media\sdcard\boot0.bin boot0.fex
copy ..\eGon\storage_media\sdcard\boot1.bin boot1.fex
    ::----------------------------------------------------------------
    ::      ����boot0_sdcard_sun�ļ�ͷ
    ::---------------------------------------------------------------- 
    %UPDATETOOLPATH%\update_boot0.exe boot0.fex ..\eFex\sys_config.bin SDMMC_CARD
    %UPDATETOOLPATH%\update_uboot.exe boot1.fex  ..\eFex\sys_config.bin 

::----------------------------------------------------------------
:: ����MBR
::----------------------------------------------------------------
%BUILDTOOLPATH%\script_old.exe part_config.fex
%BUILDTOOLPATH%\update_mbr.exe part_config.bin mbr.fex

::----------------------------------------------------------------
:: ��������ļ�
::----------------------------------------------------------------
call fsbuild.bat

::----------------------------------------------------------------
:: ������д�ļ�У���
::----------------------------------------------------------------
%DRAGONTOOLPATH%\FileAddSum.exe rootfs.fex ..\eFex\verify.fex

::----------------------------------------------------------------
:: ������д������ļ�
::----------------------------------------------------------------
if exist PHOENIXv100.img del PHOENIXv100.img
%DRAGONTOOLPATH%\compile -o image.bin image.cfg
%DRAGONTOOLPATH%\dragon image.cfg > image.txt

::----------------------------------------------------------------
:: ������ʱ�ļ�
::----------------------------------------------------------------
if exist rootfs.fex  del rootfs.fex  
if exist dlinfo.fex  del dlinfo.fex
if exist mbr.fex     del mbr.fex
if exist boot0.fex   del boot0.fex
if exist boot1.fex   del boot1.fex
if exist ..\eFex\sys_config.bin    del ..\eFex\sys_config.bin
if exist sys_config.bin del sys_config.bin
if exist part_config.bin del part_config.bin
if exist rootfs.iso del rootfs.iso
if exist image.txt del image.txt

echo make image finished...

PAUSE
