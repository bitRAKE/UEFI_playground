include 'UEFI64_app.inc'

section '.text' executable readable

entry $
; RCX : EFI_HANDLE ImageHandle, supporting protocols:
;		EFI_LOADED_IMAGE_PROTOCOL
;		EFI_LOADED_IMAGE_DEVICE_PATH_PROTOCOL
;		(... and probably more ...)
;		?EFI_HII_PACKAGE_LIST_PROTOCOL
; RDX : EFI_SYSTEM_TABLE *SystemTable, additional tables, handles, and interfaces
;
	; align stack and create shadow space for local calls
	enter 32,0

	; protocol functions are called with RCX pointing to the interface
	; structure, (as the first parameter in the Win64 ABI convension).

	; EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL interface pointer
	mov rcx,[rdx + EFI_SYSTEM_TABLE.ConOut]

	; RIP relative addressing to reduce relocations, "mov rdx,_hello"
	; would be more code bytes and relocation data.
	lea rdx,[_hello]

	; the interface structure can contain functions and data
	call qword [rcx + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.OutputString]

	leave	; restore stack
	retn	; return to caller, which will call EFI_BOOT_SERVICES.Exit for us

; UEFI only supports UCS2 strings, DU converts text UTF-8 text encoding into UCS2:
_hello du 'Hello, World!',13,10,0
