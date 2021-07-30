struct EFI_OPEN_PROTOCOL
	Revision                     UINT32
	ParentHandle                 EFI_HANDLE
	SystemTable                  VOID_PTR
	DeviceHandle                 EFI_HANDLE
	FilePath                     VOID_PTR
	Reserved                     VOID_PTR
	LoadOptionsSize              UINT32
	LoadOptions                  VOID_PTR
	ImageBase                    VOID_PTR
	ImageSize                    UINT64
	ImageCodeType                INTN
	ImageDataType                INTN
	Unload                       VOID_PTR
ends


AllHandles		:= 0
ByRegisterNotify	:= 1
ByProtocol		:= 2

EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL	:= 0x01
EFI_OPEN_PROTOCOL_GET_PROTOCOL		:= 0x02
EFI_OPEN_PROTOCOL_TEST_PROTOCOL		:= 0x04
EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER	:= 0x08
EFI_OPEN_PROTOCOL_BY_DRIVER		:= 0x10
EFI_OPEN_PROTOCOL_EXCLUSIVE		:= 0x20

