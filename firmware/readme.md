These firmwares come from various places. As the need for different execution environments grows


\OVMF_amd64

Extracted from Gerd Hoffmann’s latest OVMF builds:
https://github.com/tianocore/tianocore.github.io/wiki/OVMF


OVMF_IA32.fd
QEMU_EFI_AA64.fd
QEMU_EFI_ARM.fd

These are currently just used to test EBC executables, they are from Pete Batard's work on creating EBC support for fasmg:
https://github.com/pbatard/fasmg-ebc


Imagine a RISC-V firmware will show up here in time, and probably more Secure-Boot support for other architectures.