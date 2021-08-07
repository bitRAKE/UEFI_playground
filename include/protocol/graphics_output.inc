?GUID equ EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID \
{0x9042A9DE,0x23DC,0x4A38,{0x96,0xFB,0x7A,0xDE,0xD0,0x80,0x51,0x6A}}
struct EFI_GRAPHICS_OUTPUT_PROTOCOL
	QueryMode	PTR ? ; EFI_STATUS (*this,UINT32,*UINTN,**EFI_GRAPHICS_OUTPUT_MODE_INFORMATION)
	SetMode		PTR ? ; EFI_STATUS (*this,UINT32)
	Blt		PTR ? ; EFI_STATUS (*this,?*EFI_GRAPHICS_OUTPUT_BLT_PIXEL,EFI_GRAPHICS_OUTPUT_BLT_OPERATION,UINTN,UINTN,UINTN,UINTN,UINTN,UINTN,?UINTN)

	Mode PTR ? ; EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE
ends

struct EFI_PIXEL_BITMASK
	RedMask		dd ?
	GreenMask	dd ?
	BlueMask	dd ?
	ReservedMask	dd ?
ends

iterate I,RedGreenBlueReserved8BitPerColor,BlueGreenRedReserved8BitPerColor,BitMask,BltOnly,FormatMax
	Pixel#I := % - 1 ; EFI_GRAPHICS_PIXEL_FORMAT
end iterate

struct EFI_GRAPHICS_OUTPUT_MODE_INFORMATION
	Version			dd ?
	HorizontalResolution	dd ?
	VerticalResolution	dd ?
	PixelFormat		dd ? ; EFI_GRAPHICS_PIXEL_FORMAT
	PixelInformation	EFI_PIXEL_BITMASK
	PixelsPerScanLine	dd ?
ends

struct EFI_GRAPHICS_OUTPUT_BLT_PIXEL
	Blue	db ?
	Green	db ?
	Red	db ?
	Reserved db ?
ends

iterate I,VideoFill,VideoToBltBuffer,BufferToVideo,VideoToVideo
	EfiBlt#I := % - 1 ; EFI_GRAPHICS_OUTPUT_BLT_OPERATION
end iterate

struct EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE
	MaxMode		dd ?
	Mode		dd ?
	Info		PTR ?	; EFI_GRAPHICS_OUTPUT_MODE_INFORMATION
	SizeOfInfo	UINTN ?
	FrameBufferBase	EFI_PHYSICAL_ADDRESS ?
	FrameBufferSize	UINTN ?	; PixelsPerScanLine x VerticalResolution x PixelElementSize
ends
