
define EFI_2_90_SYSTEM_TABLE_REVISION ((2 shl 16) or 90)
define EFI_2_80_SYSTEM_TABLE_REVISION ((2 shl 16) or 80)
define EFI_2_70_SYSTEM_TABLE_REVISION ((2 shl 16) or 70)
define EFI_2_60_SYSTEM_TABLE_REVISION ((2 shl 16) or 60)
define EFI_2_50_SYSTEM_TABLE_REVISION ((2 shl 16) or 50)
define EFI_2_40_SYSTEM_TABLE_REVISION ((2 shl 16) or 40)
define EFI_2_31_SYSTEM_TABLE_REVISION ((2 shl 16) or 31)
define EFI_2_30_SYSTEM_TABLE_REVISION ((2 shl 16) or 30)
define EFI_2_20_SYSTEM_TABLE_REVISION ((2 shl 16) or 20)
define EFI_2_10_SYSTEM_TABLE_REVISION ((2 shl 16) or 10)
define EFI_2_00_SYSTEM_TABLE_REVISION ((2 shl 16) or 00)
define EFI_1_10_SYSTEM_TABLE_REVISION ((1 shl 16) or 10)
define EFI_1_02_SYSTEM_TABLE_REVISION ((1 shl 16) or 02)

define EFI_SYSTEM_TABLE_SIGNATURE 0x5453595320494249
define EFI_SYSTEM_TABLE_REVISION EFI_2_90_SYSTEM_TABLE_REVISION
define EFI_SPECIFICATION_VERSION EFI_SYSTEM_TABLE_REVISION

struct EFI_SYSTEM_TABLE
	Hdr			EFI_TABLE_HEADER
	FirmwareVendor		PTR ?		;*CHAR16
	FirmwareRevision	dd ?		;
	ConsoleInHandle		EFI_HANDLE ?	; EFI_SIMPLE_TEXT_INPUT_PROTOCOL/EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL
	ConIn			PTR ?		; EFI_SIMPLE_TEXT_INPUT_PROTOCOL interface
	ConsoleOutHandle	EFI_HANDLE ?	; EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL handle
	ConOut			PTR ?		; EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL interface
	StandardErrorHandle	EFI_HANDLE ?	; EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL handle
	StdErr			PTR ?		; EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL interface
	RuntimeServices		PTR ?		; table
	BootServices		PTR ?		; table
	NumberOfTableEntries	INTN ?		;
	ConfigurationTable	PTR ?		; table
ends



macro gST line& ; compact coding helper
	match -> =ConIn -> A B,line
		WIN64ABI [rbp + EFI_SYSTEM_TABLE.ConIn],B
		call [rcx + EFI_SIMPLE_TEXT_INPUT_PROTOCOL.A]
	else match -> =ConOut -> A B,line
		WIN64ABI [rbp + EFI_SYSTEM_TABLE.ConOut],B
		call [rcx + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.A]
	else match -> =StdErr -> A B,line
		WIN64ABI [rbp + EFI_SYSTEM_TABLE.StdErr],B
		call [rcx + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.A]
	else
		err 'unknown interface in system table use: ',10,9,`line,10
	end match
end macro



macro cmp_GUID g0,g1
	mov rax,[g0]
	mov rdx,[g0+8]
	xor rax,[g1]
	xor rdx,[g1+8]
	or rax,rdx

;	push 4
;	pop rcx
;	repz cmpsd
end macro


