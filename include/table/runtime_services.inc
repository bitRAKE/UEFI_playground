
define EFI_RUNTIME_SERVICES_SIGNATURE 0x56524553544e5552
define EFI_RUNTIME_SERVICES_REVISION EFI_SPECIFICATION_VERSION

struct EFI_RUNTIME_SERVICES
	Hdr				EFI_TABLE_HEADER

	GetTime				PTR ?	;
	SetTime				PTR ?	;
	GetWakeupTime			PTR ?	;
	SetWakeupTime			PTR ?	;

	SetVirtualAddressMap		PTR ?	;
	ConvertPointer			PTR ?	;

	GetVariable			PTR ?	;
	GetNextVariableName		PTR ?	;
	SetVariable			PTR ?	;

	GetNextHighMonotonicCount	PTR ?	;
	ResetSystem			PTR ?	;

	; UEFI 2.0+

	UpdateCapsule			PTR ?	;
	QueryCapsuleCapabilities	PTR ?	;

	QueryVariableInfo		PTR ?	;
ends



iterate I,EfiResetCold,EfiResetWarm,EfiResetShutdown,EfiResetPlatformSpecific
	I := %-1 ; EFI_RESET_TYPE
end iterate
