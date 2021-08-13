define EFI_BOOT_SERVICES_SIGNATURE 0x56524553544f4f42
define EFI_BOOT_SERVICES_REVISION EFI_SPECIFICATION_VERSION

struct EFI_BOOT_SERVICES
	Hdr					EFI_TABLE_HEADER

	; EFI 1.0+

	; Task Priority Services
	RaiseTPL				PTR ?	; EFI_TPL (EFI_TPL)
	RestoreTPL				PTR ?	; (EFI_TPL)

	; Memory Services
	AllocatePages				PTR ?	; EFI_STATUS (EFI_ALLOCATE_TYPE,EFI_MEMORY_TYPE,UINTN,*EFI_PHYSICAL_ADDRESS)
	FreePages				PTR ?	; EFI_STATUS (EFI_PHYSICAL_ADDRESS,UINTN)
	GetMemoryMap				PTR ?	; EFI_STATUS (*UINTN,*EFI_MEMORY_DESCRIPTOR,*UINTN,*UINTN,*UINT32)
	AllocatePool				PTR ?	; EFI_STATUS (EFI_MEMORY_TYPE,UINTN,**)
	FreePool				PTR ?	; EFI_STATUS (*)

	; Event & Timer Services
	CreateEvent				PTR ?	; EFI_STATUS (UINT32,EFI_TPL,?EFI_EVENT_NOTIFY,?*,*EFI_EVENT)
	SetTimer				PTR ?	; EFI_STATUS (EFI_EVENT,EFI_TIMER_DELAY,UINT64)
	WaitForEvent				PTR ?	; EFI_STATUS ()
	SignalEvent				PTR ?	; EFI_STATUS ()
	CloseEvent				PTR ?	; EFI_STATUS ()
	CheckEvent				PTR ?	; EFI_STATUS ()

	; Protocol Services
	InstallProtocolInterface		PTR ?
	ReinstallProtocolInterface		PTR ?
	UninstallProtocolInterface		PTR ?
	HandleProtocol				PTR ?
	__reserved__				PTR ?	; reserved
	RegisterProtocolNotify			PTR ?
	LocateHandle				PTR ?
	LocateDevicePath			PTR ?
	InstallConfigurationTable		PTR ?

	; Image Services
	LoadImage				PTR ?
	StartImage				PTR ?
	Exit					PTR ?
	UnloadImage				PTR ?
	ExitBootServices			PTR ?

	; Miscellaneous Services
	GetNextMonoticCount			PTR ?
	Stall					PTR ?
	SetWatchdogTimer			PTR ?

	; EFI 1.1+

	; Driver Support Services
	ConnectController			PTR ?
	DisconnectController			PTR ?

	; Protocol Services
	OpenProtocol				PTR ?
	CloseProtocol				PTR ?
	OpenProtocolInformation			PTR ?

	; Miscellaneous Services
	ProcotocolsPerHandle			PTR ?
	LocateHandleBuffer			PTR ?
	LocateProtocol				PTR ?
	InstallMultipleProtocolInterfaces	PTR ?
	UninstallMultipleProtocolInterfaces	PTR ?
	CalculateCrc32				PTR ?
	CopyMem					PTR ?
	SetMem					PTR ?

	; UEFI 2.0+

	CreateEventEx				PTR ?
ends


define EFI_TPL UINTN
define TPL_APPLICATION 4
define TPL_CALLBACK 8
define TPL_NOTIFY 16
define TPL_HIGH_LEVEL 31


; global type abstractions:
;	+ need alignment
;	+ same size reguardless of execution model
iterate T, UINT64,EFI_LBA,EFI_PHYSICAL_ADDRESS,EFI_VIRTUAL_ADDRESS
	calminstruction T values&
		asmcmd =ALIGN 8
		asmcmd =DQ values
	end calminstruction
	calminstruction(name) T values&
		asmcmd =ALIGN 8
		asmcmd name =DQ values
	end calminstruction
end iterate




iterate I,AllocateAnyPages,AllocateMaxAddress,AllocateAddress
	I := % - 1 ; EFI_ALLOCATE_TYPE
end iterate
iterate I,EfiReservedMemoryType,EfiLoaderCode,EfiLoaderData,EfiBootServicesCode,EfiBootServicesData,EfiRuntimeServicesCode,EfiRuntimeServicesData,EfiConventionalMemory,EfiUnusableMemory,EfiACPIReclaimMemory,EfiACPIMemoryNVS,EfiMemoryMappedIO,EfiMemoryMappedIOPortSpace,EfiPalCode,EfiPersistentMemory,EfiUnacceptedMemoryType
	I := % - 1 ; EFI_MEMORY_TYPE
end iterate

define EFI_MEMORY_DESCRIPTOR_VERSION 1

; Memory Attributes:
define EFI_MEMORY_UC		0x0000000000000001
define EFI_MEMORY_WC		0x0000000000000002
define EFI_MEMORY_WT		0x0000000000000004
define EFI_MEMORY_WB		0x0000000000000008
define EFI_MEMORY_UCE		0x0000000000000010
define EFI_MEMORY_WP		0x0000000000001000
define EFI_MEMORY_RP		0x0000000000002000
define EFI_MEMORY_XP		0x0000000000004000
define EFI_MEMORY_NV		0x0000000000008000
define EFI_MEMORY_MORE_RELIABLE	0x0000000000010000
define EFI_MEMORY_RO		0x0000000000020000
define EFI_MEMORY_SP		0x0000000000040000
define EFI_MEMORY_CPU_CRYPTO	0x0000000000080000
define EFI_MEMORY_RUNTIME	0x8000000000000000

struct EFI_MEMORY_DESCRIPTOR
	Type		dd ?,?	; +alignment
	PhysicalStart	dq ?	; EFI_PHYSICAL_ADDRESS
	VirtualStart	dq ?	; EFI_VIRTUAL_ADDRESS
	NumberOfPages	dq ?
	Attribute	dq ?
ends

struct EFI_MEMORY_ATTRIBUTES_TABLE
	Version		dd ?	;
	NumberOfEntries	dd ?	;
	DescriptorSize	dd ?	;
	Reserved	dd ?	;
	?Entry		EFI_MEMORY_DESCRIPTOR ; array ...
ends

;define EFI_EVENT PTR

define EVT_NOTIFY_WAIT				0x00000100
define EVT_NOTIFY_SIGNAL			0x00000200
define EVT_RUNTIME_CONTEXT			0x20000000
define EVT_RUNTIME				0x40000000
define EVT_TIMER				0x80000000
define EVT_SIGNAL_EXIT_BOOT_SERVICES		0x00000201
define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE	0x60000202

define EFI_EVENT_GROUP_EXIT_BOOT_SERVICES { 0x27abf055, 0xb1b8, 0x4c26, { 0x80, 0x48, 0x74, 0x8f, 0x37, 0xba, 0xa2, 0xdf } }
define EFI_EVENT_GROUP_BEFORE_EXIT_BOOT_SERVICES { 0x8be0e274, 0x3970, 0x4b44, { 0x80, 0xc5, 0x1a, 0xb9, 0x50, 0x2f, 0x3b, 0xfc } }
define EFI_EVENT_GROUP_VIRTUAL_ADDRESS_CHANGE { 0x13fa7698, 0xc831, 0x49c7, { 0x87, 0xea, 0x8f, 0x43, 0xfc, 0xc2, 0x51, 0x96 } }
define EFI_EVENT_GROUP_MEMORY_MAP_CHANGE { 0x78bee926, 0x692f, 0x48fd, { 0x9e, 0xdb, 0x1, 0x42, 0x2e, 0xf0, 0xd7, 0xab } }
define EFI_EVENT_GROUP_READY_TO_BOOT { 0x7ce88fb3, 0x4bd7, 0x4679, { 0x87, 0xa8, 0xa8, 0xd8, 0xde, 0xe5,0xd, 0x2b } }
define EFI_EVENT_GROUP_AFTER_READY_TO_BOOT { 0x3a2a00ad, 0x98b9, 0x4cdf, { 0xa4, 0x78, 0x70, 0x27, 0x77, 0xf1, 0xc1, 0xb } }
define EFI_EVENT_GROUP_RESET_SYSTEM { 0x62da6a56, 0x13fb, 0x485a, { 0xa8, 0xda, 0xa3, 0xdd, 0x79, 0x12, 0xcb, 0x6b } }

iterate I,TimerCancel,TimerPeriodic,TimerRelative
	I := % - 1 ; EFI_TIMER_DELAY
end iterate



macro gBS line& ; compact coding helper
	match ->A B,line
		WIN64ABI B
		mov rax,[rbp + EFI_SYSTEM_TABLE.BootServices]
		call [rax + EFI_BOOT_SERVICES.A]
	else match ->A,line
		mov rax,[rbp + EFI_SYSTEM_TABLE.BootServices]
		call [rax + EFI_BOOT_SERVICES.A]
	else
		err
	end match
end macro
;	gBS->SetWatchdogTimer 0,0,0,0
;	gBS->OpenProtocol [.ImageHandle],ADDR EFI_LOADED_IMAGE_PROTOCOL_GUID,\
;		ADDR [.LoadedImage],[.ImageHandle],0,EFI_OPEN_PROTOCOL_GET_PROTOCOL




