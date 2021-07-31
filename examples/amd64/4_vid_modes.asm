include 'UEFI64_app.inc'
; At the lowest support level, only required protocols are included. So, others
; used must be added manually:
include 'protocol/graphics_output.inc'

; Note: old EFI implementations might require use of UGA protocol.
; (No attempt is made to support it here.)

section '.text' executable readable
include 'lib_ucs2.inc'

start:	entry $
	push rbp rbx rsi rdi
	virtual at RSP
		rq 4 ; shadow space

	; local variable space:
		.image_handle	dq ? ; EFI_HANDLE
		.gop		dq ? ; EFI_GRAPHICS_OUTPUT_PROTOCOL
		.info		dq ? ;
		.info_bytes	dd ?,? ; UINTN ?
		.mode		dd ?,?

		rb ($10 - ((8 + $ - $$) and $F)) and $F ; align 16
		.FRAME := $ - $$
	end virtual
	sub rsp,.FRAME

	mov [.image_handle],rcx
	push rdx
	pop rbp		; EFI_SYSTEM_TABLE in non-volatile register

	; This differs from char modes example in that we need to get gfx
	; interface. Whereas the system table provides char interfaces.

	mov rax,[rbp + EFI_SYSTEM_TABLE.BootServices]
	lea rcx,[gop_guid]
	xor edx,edx
	lea r8,[.gop]
	call [rax + EFI_BOOT_SERVICES.LocateProtocol]
	test rax,rax
	jnz .efierr_forward

	or [.mode],-1
	lea rdx,[_header]
.itterator:
	mov rcx,[rbp + EFI_SYSTEM_TABLE.ConOut]
	call [rcx + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.OutputString]

	add [.mode],1
	mov rcx,[.gop]
	mov edx,[.mode]
	; have all modes been displayed?
	mov rax,[rcx + EFI_GRAPHICS_OUTPUT_PROTOCOL.Mode]
	cmp edx,[rax + EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE.MaxMode]
	jnc .complete
	lea r8,[.info_bytes]	;*UINTN
	lea r9,[.info]		;**EFI_GRAPHICS_OUTPUT_MODE_INFORMATION
	call [rcx + EFI_GRAPHICS_OUTPUT_PROTOCOL.QueryMode]
	test rax,rax ; EFI_SUCCESS
	jnz .efierr_forward

; This is as complicated as it looks. Referencing locals from RSP and gathering
; string fragments on the stack require some finesse. Combine this with trying
; it simplify table display while adding features, and it's a bit of a mess.

	push rbp	; not using UEFI for a while
	mov rbp,[.info + 8]
	virtual at rbp
		.mnfo EFI_GRAPHICS_OUTPUT_MODE_INFORMATION
	end virtual

	push 0				; terminator for MultiCat_UCS2
	lea rax,[_eol]
	push rax

	lea rdi,[_buildup]		; progressive buffer used & updated by generators

	mov eax,[.mnfo.PixelFormat]
	cmp eax,PixelFormatMax
	jnc .unknown_pixel_format
	lea rdx,[Format_Table]
	lea rax,[rax*2 + rdx]
	call rax ; dispatch based on EFI_GRAPHICS_PIXEL_FORMAT
	push rax
.unknown_pixel_format:
	iterate F,	-5:[.mode + 8*13],			' |',\
			-2:[.mnfo.Version],			' |',\
			-5:[.mnfo.HorizontalResolution],	' x ',\
			 5:[.mnfo.VerticalResolution],		'|',\
			-5:[.mnfo.PixelsPerScanLine],		' | '

		indx %% - % + 1 ; reverse order
		match W:V,F ; field spec
			mov edx,V
			push W
			call FieldJustify
		else if elementsof F = 0 \
			& F eqtype 'string'
			_U rax,F ; constant string pointer
		else
			err 'unknown field: ',`F
		end if
		push rax
	end iterate

	lea rdi,[_line_buffer]
	call MultiCat_UCS2
	xchg eax,esi
	stosd ; terminate
	lea rdx,[_line_buffer]
	pop rbp
	jmp .itterator

.complete:
	xor eax,eax ; EFI_SUCCESS
.efierr_forward:
	add rsp,.FRAME
	pop rdi rsi rbx rbp
	retn


Format_RGB:
	_U rax,'RGB'
	retn

Format_BGR:
	_U rax,'BGR'
	retn

Format_Blt:
	_U rax,'blit only'
	retn

Format_Table:
	jmp Format_RGB
	jmp Format_BGR
	jmp Format_Mask
	jmp Format_Blt
	assert $ - Format_Table = PixelFormatMax shl 1

; generate mask string based on EFI_PIXEL_BITMASK values:
;	RXXXXXXXX GXXXXXXXX BXXXXXXXX ?XXXXXXXX
Format_Mask: ; output constant width hex values based on size of pixel
	push rbp
	push rdi
	lea rsi,[r8 + EFI_GRAPHICS_OUTPUT_MODE_INFORMATION.PixelInformation]
	push rsi
	lodsd
	mov ebp,eax
	lodsd
	or ebp,eax
	lodsd
	or ebp,eax
	lodsd
	or ebp,eax
	bsr ebp,ebp	; pixel bits - 1
	add ebp,8
	and ebp,-8	; nibble pairs (bytes to output)
	pop rsi
	lea r8,[MaskHeads]

	push 4
	pop rcx
.parts:
	xchg rsi,r9
	movsd
	xchg rsi,r9
	lodsd
	push rcx
	mov ecx,ebp
	call Nibbles_UCS2
	pop rcx
	loop .parts
	xchg eax,ecx
	stosd
	pop rax
	pop rbp
	retn


FieldJustify:
;	+8  : field width/justification on stack
;	RDI : working buffer (updated)
;	RDX : value to output

; buffer start on stack
	push rdi
	mov ecx,[rsp + 8*2]
	test ecx,ecx
	jns @F
	neg ecx
	mov ax,' '
	rep stosw ; pre-run of spaces, right-justify
@@:	mov ecx,10
	xchg rax,rdx
	call UINT64__Baseform_UCS2
	mov rcx,[rsp + 8*2]
	test ecx,ecx
	js @F
	mov ax,' '
	mov [rdi-2],eax		; remove null-termination
	rep stosw ; post-run of spaces, left-justify
	mov ecx,[rsp + 8*2]
	pop rax			; use starting address as output
	mov word [rax+rcx*2],0	; terminate from start
	lea rdi,[rax+rcx*2+2]	; next string buffer
	retn 8			; remove field width

@@:	pop rax
	lea rax,[rdi+rcx*2-2] ; use truncated tail part
	retn 8			; remove field width


USTR equ gop_guid EFI_GUID EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID

USTR equ MaskHeads du ' R G B ?'
USTR equ _header du \
	' Mode |Ver|    H x V    | Span | Format',13,10,\
	'------|---|-------------|------|-------->'
;	'    0 | 0 | 1920 x 1080 | 1920 | RGB    ',13,10,0
;	'   ?? | 0 | 1920 x 1080 | 1920 | RXXXXXXXX GXXXXXXXX BXXXXXXXX ?XXXXXXXX',13,10,0
USTR equ _eol	du 13,10,0

; span should probably list bytes, but all other values are pixel related?
; non-32-bit pixels are the size of the masks displayed: 1,2,3 bytes.

section '.data' data readable writeable

align 64
label _buildup:256
rw sizeof _buildup

label _line_buffer:128
rw sizeof _line_buffer
