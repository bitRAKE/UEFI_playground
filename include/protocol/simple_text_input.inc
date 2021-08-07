; this interface is compatible with EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL:
?GUID equ EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID \
{0x387477C1,0x69C7,0x11D2,{0x8E,0x39,0x00,0xA0,0xC9,0x69,0x72,0x3B}}
struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL
	Reset			PTR ?	; EFI_STATUS (*this,BOOLEAN)
	ReadKeyStrokeEx		PTR ?	; EFI_STATUS (*this,*EFI_INPUT_KEY)
	WaitForKeyEx		EFI_EVENT ?
ends

struct EFI_INPUT_KEY
	ScanCode	dw ? ;
	UnicodeChar	dw ? ; UCS2 char
ends

?GUID equ EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL_GUID \
{0xDD9E7534,0x7762,0x4698,{0x8C,0x14,0xF5,0x85,0x17,0xA6,0x25,0xAA}}
struct EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL
	Reset			PTR ?	; EFI_STATUS (*this,BOOLEAN)
	ReadKeyStrokeEx		PTR ?	; EFI_STATUS (*this,*EFI_KEY_DATA)
	WaitForKeyEx		EFI_EVENT ?
	SetState		PTR ?	; EFI_STATUS (*this,*EFI_KEY_TOGGLE_STATE)
	RegisterKeyNotify	PTR ?	; EFI_STATUS (*this,*EFI_KEY_DATA,EFI_KEY_NOTIFY_FUNCTION,**)
	UnregisterKeyNotify	PTR ?	; EFI_STATUS (*this,*)
ends

; dword KeyShiftState:
define EFI_SHIFT_STATE_VALID		0x80000000
define EFI_RIGHT_SHIFT_PRESSED		0x00000001
define EFI_LEFT_SHIFT_PRESSED		0x00000002
define EFI_RIGHT_CONTROL_PRESSED	0x00000004
define EFI_LEFT_CONTROL_PRESSED		0x00000008
define EFI_RIGHT_ALT_PRESSED		0x00000010
define EFI_LEFT_ALT_PRESSED		0x00000020
define EFI_RIGHT_LOGO_PRESSED		0x00000040
define EFI_LEFT_LOGO_PRESSED		0x00000080
define EFI_MENU_KEY_PRESSED		0x00000100
define EFI_SYS_REQ_PRESSED		0x00000200

; byte EFI_KEY_TOGGLE_STATE:
define EFI_SCROLL_LOCK_ACTIVE	0x01
define EFI_NUM_LOCK_ACTIVE	0x02
define EFI_CAPS_LOCK_ACTIVE	0x04
define EFI_KEY_STATE_EXPOSED	0x40
define EFI_TOGGLE_STATE_VALID	0x80

struct EFI_KEY_STATE
	KeyShiftState	dd ?
	KeyToggleState	db ? ; EFI_KEY_TOGGLE_STATE
	_pad rb 3
ends

struct EFI_KEY_DATA
	Key		EFI_INPUT_KEY
	KeyState	EFI_KEY_STATE
ends



if used ConIn__WaitForKey
ConIn__WaitForKey:
; RBP : EFI_SYSTEM_TABLE pointer
	sub rsp,32+8 ; align stack, shadow space for Win64abi
	label .Index:8 at RSP+32

	mov ecx,1 ; just one event in array
	mov rdx,[rbp + EFI_SYSTEM_TABLE.ConIn]
	lea rdx,[rdx + EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL.WaitForKeyEx] ; *EFI_EVENT array
	lea r8,[.Index] ; INTN, where to put result
	; must be at TPL_APPLICATION level
	mov rax,[rbp + EFI_SYSTEM_TABLE.BootServices]
	call [rax + EFI_BOOT_SERVICES.WaitForEvent]

	mov rcx, [rbp + EFI_SYSTEM_TABLE.ConIn]
	xor edx,edx ; FALSE ; quick reset
	call qword [rcx + EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL.Reset]

	add rsp,32+8
	retn
end if
