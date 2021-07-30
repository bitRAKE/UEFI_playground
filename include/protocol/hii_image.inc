define EFI_HII_IMAGE_PROTOCOL_GUID {0x31A6406A,0x6BDF,0x4E46,{0xB2,0xA2,0xEB,0xAA,0x89,0xC4,0x09,0x20}}
struct EFI_HII_IMAGE_PROTOCOL
	NewImage	PTR ? ; EFI_STATUS (*this,EFI_HII_HANDLE,*EFI_IMAGE_ID,*EFI_IMAGE_INPUT)
	GetImage	PTR ? ; EFI_STATUS (*this,EFI_HII_HANDLE,EFI_IMAGE_ID,*EFI_IMAGE_INPUT)
	SetImage	PTR ? ; EFI_STATUS (*this,EFI_HII_HANDLE,EFI_IMAGE_ID,*EFI_IMAGE_INPUT)
	DrawImage	PTR ? ; EFI_STATUS (*this,EFI_HII_DRAW_FLAGS,*EFI_IMAGE_INPUT,**EFI_IMAGE_OUTPUT,UINTN,UINTN)
	DrawImageId	PTR ? ; EFI_STATUS (*this,EFI_HII_DRAW_FLAGS,EFI_HII_HANDLE,EFI_IMAGE_ID,**EFI_IMAGE_OUTPUT,UINTN,UINTN)
ends

EFI_IMAGE_TRANSPARENT		:= 0x00000001

struct _EFI_IMAGE_INPUT {
	Flags	dd ?	; EFI_IMAGE_*
	Width	dw ?
	Height	dw ?
	Bitmap	PTR ?	; EFI_GRAPHICS_OUTPUT_BLT_PIXEL
ends

EFI_HII_DRAW_FLAG_DEFAULT	:= 0x00000000
EFI_HII_DRAW_FLAG_CLIP		:= 0x00000001
EFI_HII_DRAW_FLAG_FORCE_TRANS	:= 0x00000010
EFI_HII_DRAW_FLAG_FORCE_OPAQUE	:= 0x00000020
EFI_HII_DRAW_FLAG_TRANSPARENT	:= 0x00000030
EFI_HII_DIRECT_TO_SCREEN	:= 0x00000080

struct EFI_IMAGE_OUTPUT
	Width	dw ?
	Height	dw ?
	union
		Bitmap	PTR ? ; EFI_GRAPHICS_OUTPUT_BLT_PIXEL
		Screen	PTR ? ; EFI_GRAPHICS_OUTPUT_PROTOCOL
	endu
ends


; introduced in UEFI Specification 2.6
; only extends with function GetImageInfo()
define EFI_HII_IMAGE_EX_PROTOCOL_GUID {0x1A1241E6,0x8F19,0x41A9,{0xBC,0x0E,0xE8,0xEF,0x39,0xE0,0x65,0x46}}
struct EFI_HII_IMAGE_EX_PROTOCOL
	NewImageEx	PTR ? ; EFI_STATUS (*this,EFI_HII_HANDLE,*EFI_IMAGE_ID,*EFI_IMAGE_INPUT)
	GetImageEx	PTR ? ; EFI_STATUS (*this,EFI_HII_HANDLE,EFI_IMAGE_ID,*EFI_IMAGE_INPUT)
	SetImageEx	PTR ? ; EFI_STATUS (*this,EFI_HII_HANDLE,EFI_IMAGE_ID,*EFI_IMAGE_INPUT)
	DrawImageEx	PTR ? ; EFI_STATUS (*this,EFI_HII_DRAW_FLAGS,*EFI_IMAGE_INPUT,**EFI_IMAGE_OUTPUT,UINTN,UINTN)
	DrawImageIdEx	PTR ? ; EFI_STATUS (*this,EFI_HII_DRAW_FLAGS,EFI_HII_HANDLE,EFI_IMAGE_ID,**EFI_IMAGE_OUTPUT,UINTN,UINTN)
	GetImageInfo	PTR ? ; EFI_STATUS (*this,EFI_HII_HANDLE,EFI_IMAGE_ID,*EFI_IMAGE_OUTPUT)
ends


define EFI_HII_IMAGE_DECODER_PROTOCOL_GUID {0x9E66F251,0x727C,0x418C,{0xBF,0xD6,0xC2,0xB4,0x25,0x28,0x18,0xEA}}
struct EFI_HII_IMAGE_DECODER_PROTOCOL
	GetImageDecoderName	PTR ? ; EFI_STATUS (*this,**EFI_GUID,*UINT16)
	GetImageInfo		PTR ? ; EFI_STATUS (*this,*,UINTN,**EFI_HII_IMAGE_DECODER_IMAGE_INFO_HEADER)
	DecodeImage		PTR ? ; EFI_STATUS (*this,*,UINTN,**EFI_IMAGE_OUTPUT,BOOLEAN)
ends

; EFI_HII_IMAGE_DECODER_COLOR_TYPE
EFI_HII_IMAGE_DECODER_COLOR_TYPE_RGB		:= 0x00
EFI_HII_IMAGE_DECODER_COLOR_TYPE_RGBA		:= 0x01
EFI_HII_IMAGE_DECODER_COLOR_TYPE_CMYK		:= 0x02
EFI_HII_IMAGE_DECODER_COLOR_TYPE_UNKNOWN	:= 0xFF

struct EFI_HII_IMAGE_DECODER_IMAGE_INFO_HEADER
	DecoderName		EFI_GUID
	ImageInfoSize		dw ?	; bytes of this header
	ImageWidth		dw ?	;
	ImageHeight		dw ?	;
	ColorType		db ?	; EFI_HII_IMAGE_DECODER_COLOR_TYPE
	ColorDepthInBits	db ?	;
ends

define EFI_HII_IMAGE_DECODER_NAME_JPEG_GUID {0xEFEFD093,0xD9B,0x46EB,{0xA8,0x56,0x48,0x35,0x07,0x00,0xC9,0x08}}
struct EFI_HII_IMAGE_DECODER_JPEG_INFO
	Header	EFI_HII_IMAGE_DECODER_IMAGE_INFO_HEADER
	ScanType	dw ?	;
			rw 3	; align
	Reserved	dq ?	;
ends
EFI_IMAGE_JPEG_SCANTYPE_PROGREESSIVE	:= 0x01
EFI_IMAGE_JPEG_SCANTYPE_INTERLACED	:= 0x02

define EFI_HII_IMAGE_DECODER_NAME_PNG_GUID {0xAF060190,0x5E3A,0x4025,{0xAF,0xBD,0xE1,0xF9,0x05,0xBF,0xAA,0x4C}}
struct EFI_HII_IMAGE_DECODER_PNG_INFO
	Header	EFI_HII_IMAGE_DECODER_IMAGE_INFO_HEADER
	Channels	dw ?	;
			rw 3	; align
	Reserved	dq ?	;
ends

struct EFI_HII_IMAGE_DECODER_OTHER_INFO
	Header	EFI_HII_IMAGE_DECODER_IMAGE_INFO_HEADER
	ImageExtenion	dw ? ; variable length name, CHAR16 string
ends