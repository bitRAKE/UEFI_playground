include 'UEFI64_app.inc'

section '.text' executable readable
	entry $
	push rbp
	virtual at RSP
		rq 4 ; shadow space

	; local variable space:
		.eindex dq ?

		rb ($10 - (($ - $$) and $F)) and $F ; align 16
		.FRAME := $ - $$
	end virtual
	sub rsp,.FRAME

	push rdx
	pop rbp		; EFI_SYSTEM_TABLE in non-volatile register

	mov rcx,[rbp + EFI_SYSTEM_TABLE.ConOut]
	lea rdx,[_hello]
	call qword [rcx + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.OutputString]

	; rather than spin on key read, use event system to wait for key event:

	push 1 ; just one event in array
	mov rdx,[rbp + EFI_SYSTEM_TABLE.ConIn]
	mov rax,[rbp + EFI_SYSTEM_TABLE.BootServices]
	lea rdx,[rdx + EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL.WaitForKeyEx] ; *EFI_EVENT array
	pop rcx
	lea r8,[.eindex] ; INTN, where to put result
	; must be at TPL_APPLICATION level
	call [rax + EFI_BOOT_SERVICES.WaitForEvent]
	test rax,rax
	jz @F ; EFI_SUCCESS, key event happened

	; otherwise event is invalid, skip shutdown and forward error
	add rsp,.FRAME
	pop rbp
	retn

@@:	; remove the key

	mov rcx, [rbp + EFI_SYSTEM_TABLE.ConIn]
	xor edx,edx ; FALSE ; quick reset
	call qword [rcx + EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL.Reset]

	mov ecx,EfiResetShutdown
	xor edx,edx ; EFI_SUCCESS
	xor r8,r8
	xor r9,r9
	mov rax,[rbp + EFI_SYSTEM_TABLE.RuntimeServices]
	call qword [rax + EFI_RUNTIME_SERVICES.ResetSystem]
	int3 ; spec states that ResetSystem() does not return

_hello du \
	'Hello, World!',13,10,10,\
	'Press any key to shutdown.',13,10,0
