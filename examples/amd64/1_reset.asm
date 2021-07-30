include 'UEFI64_app.inc'

section '.text' executable readable
	entry $
	push rbp
	virtual at RSP
		rq 4 ; shadow space
	; local variable space:
		.FRAME := $ - $$
	end virtual
	sub rsp,.FRAME

	push rdx
	pop rbp		; EFI_SYSTEM_TABLE in non-volatile register

	mov rcx,[rbp + EFI_SYSTEM_TABLE.ConOut]
	lea rdx,[_hello]
	call [rcx + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.OutputString]

	mov rcx,[rbp + EFI_SYSTEM_TABLE.ConIn]
	xor edx,edx ; FALSE ; quick reset
	call [rcx + EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL.Reset]
@@:
	mov rcx,[rbp + EFI_SYSTEM_TABLE.ConIn]
	lea rdx,[_key_read]
	call [rcx + EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL.ReadKeyStrokeEx]
	shl rax,1
	jnc @F ; EFI_SUCCESS, keystroke data has been read (or a warning we ignore)
	cmp rax,2*(EFI_NOT_READY - EFIERR)
	jz @B ; loop if device not ready
	; otherwise skip shutdown, and forward error
	rcr rax,1 ; put high bit back
	add rsp,.FRAME
	pop rbp
	retn
@@:
	mov ecx,EfiResetShutdown
	xor edx,edx ; EFI_SUCCESS
	xor r8,r8
	xor r9,r9
	mov rax,[rbp + EFI_SYSTEM_TABLE.RuntimeServices]
	call [rax + EFI_RUNTIME_SERVICES.ResetSystem]
	int3 ; spec states that ResetSystem() does not return


section '.data' data readable writeable

_hello du \
	'Hello, World!',13,10,10,\
	'Press any key to shutdown.',13,10,0

align 8
_key_read EFI_KEY_DATA
