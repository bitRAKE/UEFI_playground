include 'UEFI64_app.inc'

section '.text' executable readable
include 'lib_ucs2.inc'

start:	entry $
	push rbp rbx rsi rdi
	virtual at RSP
		rq 4 ; shadow space

	; local variable space:
		.mode	dd ?
			dd ?
		.cols	dd ?,? ; UINTN ?
		.rows	dd ?,? ; UINTN ?

		rb ($10 - ((8 + $ - $$) and $F)) and $F ; align 16
		.FRAME := $ - $$
	end virtual
	sub rsp,.FRAME

	push rdx
	pop rbp		; EFI_SYSTEM_TABLE in non-volatile register

USTR equ _header du \
	' Mode # | Columns X Rows',13,10,\
	'--------|----------------',13,10,0
USTR equ _unsupported du \
	'      1 | 80 x 50 Unsupported',13,10,0

	or [.mode],-1
	lea rdx,[_header]
.itterator:
	add [.mode],1
	mov rcx,[rbp + EFI_SYSTEM_TABLE.ConOut]
	call [rcx + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.OutputString]

	mov rcx,[rbp + EFI_SYSTEM_TABLE.ConOut]
	mov edx,[.mode]
	; have all modes been displayed?
	mov rax,[rcx + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.Mode]
	; shouldn't they call this ModeCount, ModesAvailible, or ModeLimit?
	cmp edx,[rax + EFI_SIMPLE_TEXT_OUTPUT_MODE.MaxMode]
	jnc .complete
	lea r8,[.cols] ;*UINTN
	lea r9,[.rows] ;*UINTN
	call [rcx + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.QueryMode]
	test rax,rax
	jz .display_mode

	; 80x25 ; mode 0 is required by spec, but
	; 80x50 ; mode 1 can fail with EFI_UNSUPPORTED

	cmp eax,EFI_UNSUPPORTED - EFIERR ; assume high-dword is 0x80..
	jnz .efierr_forward
	cmp [.mode],1
	jnz .efierr_forward
	lea rdx,[_unsupported]
	jmp .itterator

.complete:
	xor eax,eax ; EFI_SUCCESS
.efierr_forward:
	add rsp,.FRAME
	pop rdi rsi rbx rbp
	retn

.display_mode:
	; display mode parameters
	push 10
	pop rcx
	mov eax,[.mode]
	lea rdi,[_template0]
	call UINT64__Baseform_UCS2
	; right-justify mode number by anchoring from number end
	lea rax,[rdi - 16] ; assume 7 digit limit should be fine
	push rax ; preserve start for OutputString
	mov dword [rdi-2],' ' + ('|' shl 16)
	mov dword [rdi+2],' ' + ('?' shl 16)
	scasd
	mov eax,[.cols + 8]
	call UINT64__Baseform_UCS2
	mov dword [rdi-2],' ' + ('x' shl 16)
	mov dword [rdi+2],' ' + ('?' shl 16)
	scasd
	mov eax,[.rows + 8]
	call UINT64__Baseform_UCS2
	mov dword [rdi-2],13 + (10 shl 16)
	and dword [rdi+2],0

	pop rdx
	jmp .itterator


section '.data' data readable writeable

du	'      '
_template0:
	rw 128 ; overkill really (23 should do it?)
