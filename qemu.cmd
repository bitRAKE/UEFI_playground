@ECHO OFF
REM Some default options:

set QEMU_PATH=C:\Program Files\qemu\
set QEMU_OPTS=-net none -monitor none -parallel none
set HDA=-drive file=fat:rw:image,index=1,media=disk,if=virtio,format=raw
set QEMU_EXTRA=

set QEMU_EXE=%QEMU_PATH%\qemu-system-x86_64w.exe
set FIRMWARE_NAME=OVMF_amd64\OVMF.fd

:LOOP
if [%1]==[] goto NEXT
if [%1]==[x64] (
  REM default options set
) else if [%1]==[ia32] (
  set QEMU_EXE=%QEMU_PATH%\qemu-system-i386w.exe
  set FIRMWARE_NAME=OVMF_IA32.fd
) else if [%1]==[arm] (
  set QEMU_EXE=%QEMU_PATH%\qemu-system-armw.exe
  set QEMU_OPTS=-M virt -cpu cortex-a15 %QEMU_OPTS%
  set FIRMWARE_NAME=QEMU_EFI_ARM.fd
) else if [%1]==[aa64] (
  set QEMU_EXE=%QEMU_PATH%\qemu-system-aarch64w.exe
  set QEMU_OPTS=-M virt -cpu cortex-a57 %QEMU_OPTS%
  set FIRMWARE_NAME=QEMU_EFI_AA64.fd
) else (
  echo %1 not a valid option
  goto end
)
shift
goto loop

:NEXT

@echo on
"%QEMU_EXE%" %QEMU_OPTS% -L . -bios firmware\%FIRMWARE_NAME% %HDA% %QEMU_EXTRA%
@echo off
:end
@echo on
