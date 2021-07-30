define EFI_MP_SERVICES_PROTOCOL_GUID \
{0x3FDDA605,0xA76E,0x4F46,{0xAD,0x29,0x12,0xF4,0x53,0x1B,0x3D,0x08}}
struct EFI_MP_SERVICES_PROTOCOL
	GetNumberOfProcessors	PTR ? ; EFI_STATUS (*this,*UINTN,*UINTN)
	GetProcessorInfo	PTR ? ; EFI_STATUS (*this,UINTN,*EFI_PROCESSOR_INFORMATION)
	StartupAllAPs		PTR ? ; EFI_STATUS (*this,EFI_AP_PROCEDURE,BOOLEAN,?EFI_EVENT,UINTN,?*,?**UINTN)
	StartupThisAP		PTR ? ; EFI_STATUS (*this,EFI_AP_PROCEDURE,UINTN,?EFI_EVENT,UINTN,?*,?*BOOLEAN)
	SwitchBSP		PTR ? ; EFI_STATUS (*this,UINTN,BOOLEAN)
	EnableDisableAP		PTR ? ; EFI_STATUS (*this,UINTN,BOOLEAN,?*UINT32)
	WhoAmI			PTR ? ; EFI_STATUS (*this,*UINTN)
ends

; Note: older EFI might only implement EFI_FRAMEWORK_MP_SERVICES_PROTOCOL, but
; EdkCompatibilityPkg has an implementation of this protocol on the older.
; (see branch: https://github.com/tianocore/edk2/tree/UDK2018)

CPU_V2_EXTENDED_TOPOLOGY := BIT24

END_OF_CPU_LIST		:= 0xFFFFFFFF

; EFI_PROCESSOR_INFORMATION.StatusFlag
PROCESSOR_AS_BSP_BIT		:= 0x00000001
PROCESSOR_ENABLED_BIT		:= 0x00000002
PROCESSOR_HEALTH_STATUS_BIT	:= 0x00000004

struct EFI_CPU_PHYSICAL_LOCATION
	Package	dd ? ; zero-based physical package number that identifies the cartridge
	Core	dd ? ; zero-based physical core number within package
	Thread	dd ? ; zero-based logical thread number within core
ends

struct EFI_CPU_PHYSICAL_LOCATION2
	Package	dd ? ; 0-based physical package that identifies the cartridge
	Module	dd ? ; 0-based physical module within package
	Tile	dd ? ; 0-based physical tile within module
	Die	dd ? ; 0-based physical die within tile
	Core	dd ? ; 0-based physical core within die
	Thread	dd ? ; 0-based logical thread within core
ends

union EXTENDED_PROCESSOR_INFORMATION
	Location2 EFI_CPU_PHYSICAL_LOCATION2
endu

struct EFI_PROCESSOR_INFORMATION
	ProcessorId		dq ? ;
	StatusFlag		dd ? ;
	Location		EFI_CPU_PHYSICAL_LOCATION
	ExtendedInformation	EXTENDED_PROCESSOR_INFORMATION
ends
