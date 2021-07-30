@echo on
"C:\Program Files\qemu\qemu-system-x86_64w.exe" ^
 -nodefaults ^
 -cpu qemu64 -vga std -machine q35,smm=on ^
 -device qemu-xhci -device usb-mouse -device usb-kbd ^
 -L . -drive file=fat:rw:image,index=1,media=disk,if=virtio,format=raw ^
 -drive file=firmware\OVMF_amd64\OVMF_CODE_4M.ms.fd,if=pflash,format=raw,unit=0,readonly=on ^
 -drive file=firmware\OVMF_amd64\OVMF_VARS_4M.ms.fd,if=pflash,format=raw,unit=1