; a handle supporting this protocol also supports EFI_DEVICE_PATH_PROTOCOL

define EFI_ABSOLUTE_POINTER_PROTOCOL_GUID { 0x8D59D32B, 0xC655, 0x4AE9, { 0x9B, 0x15, 0xF2, 0x59, 0x04, 0x99, 0x2A, 0x43 } }
struct EFI_ABSOLUTE_POINTER_PROTOCOL
	Reset		PTR ? ; EFI_STATUS (*this,BOOLEAN)
	GetState	PTR ? ; EFI_STATUS (*this,*EFI_ABSOLUTE_POINTER_STATE)
	WaitForInput	PTR ? ; EFI_EVENT
	Mode		PTR ? ; EFI_SIMPLE_POINTER_MODE
ends


define EFI_ABSP_TouchActive	0x00000001
define EFI_ABS_AltActive	0x00000002

struct EFI_ABSOLUTE_POINTER_STATE
	CurrentX	dq ? ; UINT64
	CurrentY	dq ? ; UINT64
	CurrentZ	dq ? ; UINT64
	ActiveButtons	dd ? ; UINT32
			dd ? ; pad
ends


define EFI_ABSP_SupportsAltActive	0x00000001
define EFI_ABSP_SupportsPressureAsZ	0x00000002

struct EFI_ABSOLUTE_POINTER_MODE
	AbsoluteMinX	dq ? ; UINT64
	AbsoluteMinY	dq ? ; UINT64
	AbsoluteMinZ	dq ? ; UINT64
	AbsoluteMaxX	dq ? ; UINT64
	AbsoluteMaxY	dq ? ; UINT64
	AbsoluteMaxZ	dq ? ; UINT64
	Attributes	dd ? ; UINT32
	dd ? ; pad
ends
