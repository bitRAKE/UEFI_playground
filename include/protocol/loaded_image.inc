define EFI_LOADED_IMAGE_PROTOCOL_GUID \
{0x5B1B31A1,0x9562,0x11D2,{0x8E,0x3F,0x00,0xA0,0xC9,0x69,0x72,0x3B}}
struct EFI_LOADED_IMAGE_PROTOCOL
	Revision	dd ?		; UINT32
	ParentHandle	EFI_HANDLE ?
	SystemTable	PTR ?		EFI_SYSTEM_TABLE

; source location of image:
	DeviceHandle	EFI_HANDLE ?
	FilePath	PTR ?		; EFI_DEVICE_PATH_PROTOCOL
	Reserved	PTR ?		; Reserved. DO NOT USE.

; image load options:
	LoadOptionsSize	dd ?	; UINT32
	LoadOptions	PTR ?	;

; location of where image was loaded:
	ImageBase	PTR ?			; base address
	ImageSize	UINT64 ?		; bytes of the loaded image
	ImageCodeType	EFI_MEMORY_TYPE
	ImageDataType	EFI_MEMORY_TYPE
	Unload		EFI_IMAGE_UNLOAD	;
ends

; EFI_SYSTEM_TABLE & EFI_IMAGE_UNLOAD are defined in EfiApi.h
EFI_LOADED_IMAGE_PROTOCOL_REVISION := 0x1000
EFI_LOADED_IMAGE_INFORMATION_REVISION := EFI_LOADED_IMAGE_PROTOCOL_REVISION


; is a type of EFI_DEVICE_PATH_PROTOCOL/EFI_DEVICE_PATH_UTILITIES_PROTOCOL ?
define EFI_LOADED_IMAGE_DEVICE_PATH_PROTOCOL_GUID \
{0xBC62157E,0x3E33,0x4FEC,{0x99,0x20,0x2D,0x3B,0x36,0xD7,0x50,0xDF}}
