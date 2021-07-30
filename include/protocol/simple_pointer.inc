; a handle supporting this protocol also supports EFI_DEVICE_PATH_PROTOCOL

define EFI_SIMPLE_POINTER_PROTOCOL_GUID { 0x31878c87,0xb75,0x11d5, { 0x9a,0x4f,0x00,0x90,0x27,0x3f,0xc1,0x4d } }
struct EFI_SIMPLE_POINTER_PROTOCOL
	Reset		PTR ? ; EFI_STATUS (*this,BOOLEAN)
	GetState	PTR ? ; EFI_STATUS (*this,*EFI_SIMPLE_POINTER_STATE)
	WaitForInput	PTR ? ; EFI_EVENT
	Mode		PTR ? ; EFI_SIMPLE_POINTER_MODE
ends

struct EFI_SIMPLE_POINTER_STATE
	RelativeMovementX	dd ? ; INT32
	RelativeMovementY	dd ? ; INT32
	RelativeMovementZ	dd ? ; INT32
	LeftButton		dd ? ; BOOLEAN
	RightButton		dd ? ; BOOLEAN
ends

struct EFI_SIMPLE_POINTER_MODE
	ResolutionX	dq ? ; UINT64
	ResolutionY	dq ? ; UINT64
	ResolutionZ	dq ? ; UINT64
	LeftButton	dd ? ; BOOLEAN
	RightButton	dd ? ; BOOLEAN
ends
